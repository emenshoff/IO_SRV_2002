/********************************************************************
  класс для связи с КП и адаптером через последовательный порт ПК
                   (канальный уровень)
                     created by M.E.N.
********************************************************************/
#ifndef _kp_serial_port_
#define _kp_serial_port_

#include <bin_data.h>
#include <serial_port.h>
#include <def_crit_sec.h>
#include <list>

using namespace std;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#define BEGMARK 0x02
#define ENDMARK 0x03
#define PREFIX  0x09
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//последовательный порт для связи с КП через адаптеры (протокол ISIO-2)
class KpSerialPort : public SerialPort
{
  friend class KpControl;
#ifdef MULTI_THREADED
# ifdef WIN32
   friend DWORD WINAPI ThreadFunc(LPVOID);
# else
   friend void *ThreadFunc(void *);
# endif
#endif
public:
 KpSerialPort();
 virtual ~KpSerialPort();
 bool Ok(){return SerialPort::Ok() && state !=CH_ERROR;}
 int InQueueSize()const;
 int OutQueueSize()const;
 void StartPortPoll();
 void StopPortPoll(); 
 void Send(const unsigned char *data,unsigned len,int channel);//помещение в выходную очередь  
protected:  
 //текущее состояние канала связи:
 enum{
     INIT,        //начало работы - необходимо инициализировать
     START,       //начало чтения
     READING,     //процесс чтения
     PREFIX_SKIP, //отсечка байт-стаффинга
     CH_ERROR     //ошибка
 };
 const BinData *Recive()const;                         //извлечение из входной очереди  
 mutable BinData Data;
 unsigned CRC(const unsigned char *p, unsigned len)const;
 void Step();     //шаг чтения-записи 
private:
 //следующие три функции работают в thread-е:
 void SendStep(); //шаг записи
 void ReadStep(); //шаг чтения
 mutable list<BinData *> InQueue;
 mutable list<BinData *> OutQueue; 
 unsigned char state;               //текущее состояние конечного автомата
 unsigned char thread_work_flag;    //флаг для потока чтения из порта. если нулевой, то поток завершается
 char buff[0xfff];
 unsigned cnt;
#ifdef MULTI_THREADED
  mutable CRIT_SEC_DESCRIPT(in_queue_lock) 	//блокировка работы с входной очередью
  mutable CRIT_SEC_DESCRIPT(out_queue_lock) //блокировка работы с выходной очередью
# ifdef WIN32
   HANDLE thr;
# else
   pthread_t thr;
# endif
#endif
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif