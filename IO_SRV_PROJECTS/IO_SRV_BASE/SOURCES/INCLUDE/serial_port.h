/********************************************************************
          ������� ����� - ���������������� ���� (������� API ��)
          (������������� ��: NT,FreeBsd � Linux (���� �� 2.14 � ����)
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
  SerialPort(unsigned buff_size=0xff);    //����������� ��������� ������ �������� ������
  virtual ~SerialPort();
 //------------------------------------------  
  virtual bool Reset(const char *port_name,int spd);		//������������� (��� ����� � ��� ��������)
  virtual bool Ok(){return ok;}                         //������� ���������� � ������
  virtual void SetSpeed(int);                           //��������� ��������
  virtual void Send(const unsigned char *data,unsigned len,unsigned &sended);//������� len ���� �� ������ data. ����� ���������� ���� - sended
  virtual const unsigned char *Recive(unsigned length,unsigned &size);       //������ lenght ����. ����� ����������� - � 3-� ���������
  //------------------------------------------
  int Speed()const{return speed;}          //���. �������� �����
#ifdef WIN32                              
  HANDLE GetHandle(){return Port;}         //���������� ������������� ��������� ����� (���� ������ ������ ���������� ��������� ����������)
#else                                      
  int GetHandle(){return Port;}            //
#endif
  static int GetCount();                   //������� ����� ����������� ������
  const char *Name()const{return ID;}      //��� �����
protected:
#ifdef WIN32
  HANDLE Port;                             
#else
  int Port;                                
#endif
  int speed;                               
  unsigned char *InputBuffer;              //������� �����
  const unsigned BufferSize;               //������ �������� ������
private:  
  bool ok;     
  char *ID;                                
  static int Count;                        
#ifdef MULTI_THREADED
# ifdef WIN32
    CRITICAL_SECTION PortLock;	      //����������� ������ 
# else
    pthread_mutex_t PortLock;           //���������� (POSIX mutex)
# endif
#endif
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif
		
