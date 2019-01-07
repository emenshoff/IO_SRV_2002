//+++++++++++++++ serial port class src file by M.E.N.+++++++
//          It's tested on FreeBSD, Linux and Windows NT
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include <io_srv_dump.h>
#include <serial_port.h>
#include <string>

using namespace std;

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#ifdef MULTI_THREADED
# ifdef WIN32
#   define ENTER_CRITICAL(x)   EnterCriticalSection(&x);
# else
#   define ENTER_CRITICAL(x)   pthread_mutex_lock(&x);
# endif
#else
# define ENTER_CRITICAL(x)  ;
# endif
//--------------------------------------------------------------------------------

#ifdef MULTI_THREADED
# ifdef WIN32
#   define LEAVE_CRITICAL(x)  LeaveCriticalSection(&x);
# else
#   define LEAVE_CRITICAL(x)  pthread_mutex_unlock(&x);
# endif
#else
# define LEAVE_CRITICAL(x)  ;
#endif

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
int SerialPort::Count=0;
int SerialPort::GetCount()
{
  return Count;
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
SerialPort::SerialPort(unsigned buff_size)
 :BufferSize(buff_size),InputBuffer(0),ID(0),speed(9600)
{
  ok=false;  
  InputBuffer=(unsigned char *)malloc(BufferSize);
  Count++;
#ifdef WIN32
  Port=INVALID_HANDLE_VALUE;
#else
  Port=-1;
#endif  
#ifdef MULTI_THREADED
# ifdef WIN32
   InitializeCriticalSection(&PortLock);
# else
  pthread_mutex_init(&PortLock,0);
# endif
#endif
}
//------------------------------------------------------------
SerialPort::~SerialPort()
{
  if(Count)
    Count--;  
#ifdef WIN32
  if(Port!=INVALID_HANDLE_VALUE) CloseHandle(Port);
#else
  if(Port!=-1)close(Port);
#endif
  free(ID);
  free(InputBuffer);
#ifdef MULTI_THREADED
# ifdef WIN32
   DeleteCriticalSection(&PortLock);
# else
   pthread_mutex_destroy(&PortLock);
# endif
#endif
}
//------------------------------------------------------------
bool SerialPort::Reset(const char *port_name,int spd)
{
  speed=spd;
  if(!port_name)
  { 
    return ok=false;
  }
  if(ID)
    free(ID);
  ID=strdup(port_name);
  ENTER_CRITICAL(PortLock);
#ifdef WIN32    //WIN32 platform
  if(Port!=INVALID_HANDLE_VALUE)
    CloseHandle(Port);
  COMMTIMEOUTS t_outs;
  COMMCONFIG	cfg;
  DWORD size;
  Port=CreateFile(
                   ID,GENERIC_READ | GENERIC_WRITE,
                   NULL,    
                   NULL, 
                   OPEN_EXISTING,
                   NULL,   
                   NULL  
                 );
  if(Port==INVALID_HANDLE_VALUE || !GetCommConfig(Port,&cfg,&size)/* || !SetupComm(Port,0x400,0x400)*/)
  {
	  dump.Print(D_ERR,"\tПорт %s не доступен!", ID);
    LEAVE_CRITICAL(PortLock)
    return ok=false;
  }
  t_outs.ReadIntervalTimeout=0;
  t_outs.ReadTotalTimeoutMultiplier=0;
  t_outs.ReadTotalTimeoutConstant=1;
  t_outs.WriteTotalTimeoutMultiplier=0;
  t_outs.WriteTotalTimeoutConstant=0;
  cfg.dcb.BaudRate=speed;
  cfg.dcb.Parity=NOPARITY;
  cfg.dcb.StopBits=ONESTOPBIT;
  cfg.dcb.ByteSize=8;
//---------you can add more, if you want...--------------
  if(!SetCommConfig(Port,&cfg,cfg.dwSize) || !SetCommTimeouts(Port,&t_outs))
  {
    dump.Print(D_ERR,"\tОшибка установки параметров порта %s !", ID);
    LEAVE_CRITICAL(PortLock)
    return ok=false;
  }
#else   //UNIX (posix) platform
  termios Settings;
  if(Port!=-1)
    close(Port);
  Port=open(port_name,O_RDWR | O_NONBLOCK);
  if(Port==-1)
  {
    dump.Print(D_ERR,"\tПорт %s не доступен!", ID);
    LEAVE_CRITICAL(PortLock)
    return ok=false;
  }
  tcgetattr(Port, &Settings);
  cfmakeraw(&Settings);
  cfsetspeed(&Settings, speed);
  Settings.c_cflag &= (CS8 | CREAD | CLOCAL);
  Settings.c_lflag =0;
  Settings.c_iflag &= (IGNCR | IGNBRK | IGNPAR); 
  if (tcsetattr(Port, TCSANOW, &Settings) ==-1)
  {
    dump.Print(D_ERR,"\tОшибка установки параметров порта %s !", ID);
    LEAVE_CRITICAL(PortLock)
    return ok=false;
  }
#endif
  LEAVE_CRITICAL(PortLock)
  dump.Print(D_MSG,"\tПорт %s открыт...", ID);
  return (ok=true);  
}
//------------------------------------------------------------------------
void SerialPort::SetSpeed(int spd)
{  
  if(!ok)
    return;
  ENTER_CRITICAL(PortLock);
#ifdef WIN32
  DWORD size;
  COMMCONFIG cfg;
  if(!GetCommConfig(Port,&cfg,&size))
  {
    dump.Print(D_ERR,"\tОшибка установки скорости порта %s !", ID);
    ok=false;
    LEAVE_CRITICAL(PortLock)
    return;
  }
  cfg.dcb.BaudRate=speed;
  if(!SetCommConfig(Port,&cfg,cfg.dwSize))
  {
    dump.Print(D_ERR,"\tОшибка установки скорости порта %s !", ID);
    ok=false;
    LEAVE_CRITICAL(PortLock)
    return;
  }
#else
  termios Settings;
  cfsetspeed(&Settings, speed);
  int res=0;
  if ((res=tcsetattr(Port, TCSANOW, &Settings)) == -1)
  {
    dump.Print(D_ERR,"\tОшибка установки скорости порта %s !", ID);
    ok=false;
    LEAVE_CRITICAL(PortLock)
    return;
  }
#endif
  speed=spd;
  LEAVE_CRITICAL(PortLock)
}
//---------------------------------------------------
const int port_msg_flags = (D_MSG | D_PORT);
//---------------------------------------------------
void SerialPort::Send(const unsigned char *data,unsigned len,unsigned &writed)
{
  if(!ok)
  { writed=0;return;}
  ENTER_CRITICAL(PortLock)  
#ifdef WIN32
  if(WriteFile(Port,(void*)data,len,(unsigned long *)&writed,NULL))
  {
    /*if((dump.Flags() & port_msg_flags) == port_msg_flags)
    {
      static char vl[10];
      string packet;
      dump.Print(port_msg_flags,"%s: записано %d байт из %d:",ID,writed,len);
      for(unsigned i=0;i<writed;i++)
      {
        sprintf(vl," %X",data[i]);
        packet += vl;
      }
      dump.Print(port_msg_flags," %s",packet.c_str());
    }*/
  }
  else
  {
    dump.Print(D_ERR | D_PORT,"Порт %s: Ошибка записи %d!",ID,GetLastError());
  }
#else
  if((writed=write(Port, (void*)data, len))!= -1)
  {
    /*if((dump.Flags() & port_msg_flags) == port_msg_flags)
    {
      static char vl[10];
      string packet;
      dump.Print(port_msg_flags,"%s: записано %d байт из %d:",ID,writed,len);
      for(unsigned i=0;i<writed;i++)
      {
        sprintf(vl," %X",data[i]);
        packet += vl;
      }
      dump.Print(port_msg_flags," %s",packet.c_str());
    }*/
  }
  else
  {
    dump.Print(D_ERR | D_PORT,"Порт %s: Ошибка записи %d!",ID,errno);
  }
#endif
  LEAVE_CRITICAL(PortLock)
}
//-----------------------------------------------------------------
const unsigned char *SerialPort::Recive(unsigned length,unsigned &size)
{
  unsigned char tmp=0x00;
  size=0;
  if(!ok||length>BufferSize)
  { return (unsigned char*)""; }
  ENTER_CRITICAL(PortLock)  
#ifdef WIN32
  unsigned long num=0;  
  for(unsigned i=0;i<length;i++)
  {
    if(ReadFile(Port,(void*)&tmp,1,&num,NULL))
    {
      *(InputBuffer + size)=tmp;
      size+=num;      
    }
    else
    {
      dump.Print(D_ERR | D_PORT,"Порт %s: ошибка чтения! error= %d", ID,GetLastError());
    }
  }
#else
  int num;
  for(unsigned i=0;i<length;i++)
  {
    if((num=read(Port, (void*)&tmp, 1))!= -1)//читаю по байту
    {
      *(InputBuffer + size)=tmp;
      size+=num;
    }    
    else
    {      
      if(errno==35)
      {
        dump.Print(D_ERR | D_PORT,"%s нет данных!", ID);
      }
      else
        dump.Print(D_ERR | D_PORT,"Порт %s: ошибка чтения! error= %d", ID,errno);
    }
  }  
#endif
  /*if((dump.Flags() & port_msg_flags) == port_msg_flags)
  {
    static char vl[10];
    string packet;
    dump.Print(port_msg_flags,"Порт %s: Прочитано %d байт из %d:",ID,size,length);
    for(unsigned i=0;i<size;i++)
    {
      sprintf(vl," %X",InputBuffer[i]);
      packet += vl;
    }
    dump.Print(port_msg_flags," %s",packet.c_str());
  }*/
  LEAVE_CRITICAL(PortLock)  
  return InputBuffer;
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
