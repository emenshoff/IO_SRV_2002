/********************************************************************
  класс для связи с КП и адаптером через последовательный порт ПК
                     created by M.E.N.
********************************************************************/
#ifndef _kp_comm_dev_h
#define _kp_comm_dev_h

#ifndef WIN32
	#include <sys/types.h>
	#include <sys/un.h>
	#include <sys/socket.h>
	#include <termios.h>
	#include <fcntl.h>
	#include <unistd.h>
	#include <signal.h>	
	#include <sys/errno.h>
  	#include <pthread.h>
#else
	#include <windows.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <kp_types.h>
#include <stm_srv_cfg.h>
#include <kp_channel.h>
#include <bin_data.h>
#include <def_crit_sec.h>
#include <list>

using namespace std;

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#define BEGMARK 0x02
#define ENDMARK 0x03
#define PREFIX  0x09
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// заголовок пакета от адаптера
struct RawHeader
{
  unsigned char paddle[3];  //херня какая - то (0xff)  :)
  unsigned char address;    //адрес канала
  unsigned char proto;      //код протокола
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#define RawHeaderLen 5
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//типы пакетов от адаптера:
#define PK_CFG_REQ    0xa0  //адаптер запрашивает конфигурацию
#define PK_CFG_SEND   0x90  //пакет к адаптеру с конфигурацией
#define KP_CFG_ACCEPT 0xb0  //адаптер принял конфигурацию
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//коммуникационное устройство (один адаптер)
class KpCommDev : public ControlObject
{
 friend class KpControl;
 friend class KpConfig;
public:
 KpCommDev(obj_d &cfg_id);
 virtual ~KpCommDev();
 bool Ok(){return ok && state !=CH_ERROR;}
 void Process();                                            //цикл обработки
 void SetError(unsigned char err);                          //установка недостоверности
 bool Init();                                              //инициализация порта 
 virtual unsigned Type()const{return KP_COMM_DEV;}         //RTTI   :)
 void Send(const unsigned char *data,unsigned len,int channel);//помещение в выходную очередь  
 int Speed()const{return speed;}          //тек. скорость порта
 const char *Name()const{return port_name;}      //имя порта
private:
 void InitChannels();                                      //инициализация каналов
 int InQueueSize()const;
 int OutQueueSize()const;
 //текущее состояние канала связи:
 enum
 {
     INIT,        //начало работы - необходимо инициализировать
     START,       //начало чтения
     READING,     //процесс чтения
     PREFIX_SKIP, //отсечка байт-стаффинга
     CH_ERROR     //ошибка
 };
 //класс буфера
 template <int SIZE=256>
 struct PortBuff
  {
    int Len;
    char Buff[SIZE];
    //
    int Size()const{return SIZE;}
    //
    void Out(int len)
    {
      if(len > 0)
        if(len <= Len)
          memmove(Buff,Buff + len,Len -= len);
    }
  };
 int SendPort(const unsigned char *data,unsigned len,int &writed); //отсылка в порт пк.
 int ReadPort(unsigned char *,unsigned length,int &sended);        //чтение из порта пк.
 const BinData *Recive()const;                         //извлечение из входной очереди
 unsigned CRC(const unsigned char *p, unsigned len)const;//вычисление контр суммы
 void Step();                                          //шаг чтения-записи  
 void SendStep(); //шаг записи
 void ReadStep(); //шаг чтения
 void SendChannelInit(KpCnl*); //отсылка строки инициализации для одного канала
 KpCnl *channels[4];		       //канальные адаптеры
 mutable BinData Data;                                 //подобие буфера
 mutable list<BinData *> InQueue;                      //очереди
 mutable list<BinData *> OutQueue; 
 unsigned char state;               //текущее состояние конечного автомата 
 time_t last_init_time;             //время последней инициализации адаптеров.
 mutable PortBuff<256> OutBuff;
 mutable PortBuff<0xfff> InBuff;
 bool Reset(const char *port_name,int spd);		//инициализация (имя порта и его скорость)  
 int Send(const unsigned char *data,unsigned len,unsigned &sended);//отсылка len байт из буфера data. число отосланных байт - sended
 int Recive(unsigned char *,unsigned length,unsigned &readed);       //чтение lenght байт. число прочитанных - в 3-м параметре
 //------------------------------------------ 
 int port_lock;
 int skip_packets_cnt;
 //------------------------------------------ 
#ifdef WIN32
  HANDLE Port;                             
#else
  int Port;                                
#endif
  int speed;
private:  
  bool ok;     
  char *port_name;
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#endif