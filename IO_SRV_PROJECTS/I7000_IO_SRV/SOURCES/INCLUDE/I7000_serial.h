/**************************************************************************
    класс для работы с контроллерами ввода-вывода семейства i7000
     (читаем из порта, пока не встретиться 0x0D, затем кладем в буфер).
     Очереди не предусмотрены, так как модули I7000 работают только
     в синхронном режиме приема-передачи...
      Максимальный размер посылки не должен превышать 0xfff байт)
          created by M.E.N.
**************************************************************************/
#ifndef _i7000_serial_port
#define _i7000_serial_port

#include <math.h>
#include <bin_data.h>
#include <I7000_crc.h>
#include <serial_port.h>
#include <I7000_events_cash.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class I7000_SerialPort: public SerialPort
{
public:
 I7000_SerialPort();
 virtual void Step();                             // цикл чтения-записи
 bool Init(const char *name,int speed,unsigned buff_size=0xf);
 unsigned DelayVal()const{return delay_val;}
 const unsigned char *Recive(unsigned &sz);
 virtual void Send(const unsigned char *,unsigned);
 bool CanUse()const //возможность использования порта для чтения.записи...
 {
   return ((EventsCash.TimeInDbl() - last_access_time)/1e3 > delay_val);
 } 
protected:
 void ReadStep();
 void SendStep();
 enum{BEGIN,READING,READED};              //возможные состояния 
 double last_access_time;                 //время последнего доступа к порту
 unsigned send_len;
 unsigned delay_val;                      //время, необх. для отсылки 10 байт на данной скорости (мс)
 unsigned char state;                     //текущее состояние 
 unsigned char buff_in[0xfff];            //буфер для чтения посылок
 unsigned char buff_out[0xfff];           //буфер для чтения посылок
 unsigned char *read_pos;                 //позиция в буфере приема 
private:
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
extern char HexToAscii(const short int hex);
extern char AsciiToHex(const char cASCII);
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif