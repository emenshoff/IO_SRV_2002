/********************************************************************
          Ѕазовый класс - последовательный порт (уровень API ос)
          (тестировалось на: NT,FreeBsd и Linux (€дро от 2.14 и выше)
                     created by M.E.N.
********************************************************************/

#ifndef _serial_port_h
#define _serial_port_h

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

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class SerialPort
{
public:
  SerialPort(unsigned buff_size=0xff);    //конструктор принимает размер входного буфера
  virtual ~SerialPort();
 //------------------------------------------  
  virtual bool Reset(const char *port_name,int spd);		//инициализаци€ (им€ порта и его скорость)
  virtual bool Ok(){return ok;}                         //признак готовнисти к работе
  virtual void SetSpeed(int);                           //установка скорости
  virtual void Send(const unsigned char *data,unsigned len,unsigned &sended);//отсылка len байт из буфера data. число отосланных байт - sended
  virtual const unsigned char *Recive(unsigned length,unsigned &size);       //чтение lenght байт. число прочитанных - в 3-м параметре
  //------------------------------------------
  int Speed()const{return speed;}          //тек. скорость порта
#ifdef WIN32                              
  HANDLE GetHandle(){return Port;}         //возвращает неконстантный описатель порта (если юзверь желает изощренной установки параметров)
#else                                      
  int GetHandle(){return Port;}            //
#endif
  static int GetCount();                   //текущее число экземпл€ров класса
  const char *Name()const{return ID;}      //им€ порта
protected:
#ifdef WIN32
  HANDLE Port;                             
#else
  int Port;                                
#endif
  int speed;                               
  unsigned char *InputBuffer;              //входной буфер
  const unsigned BufferSize;               //размер входного буфера
private:  
  bool ok;     
  char *ID;                                
  static int Count;                        
#ifdef MULTI_THREADED
# ifdef WIN32
    CRITICAL_SECTION PortLock;	      //критическа€ секци€ 
# else
    pthread_mutex_t PortLock;           //блокоровка (POSIX mutex)
# endif
#endif
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif
		
