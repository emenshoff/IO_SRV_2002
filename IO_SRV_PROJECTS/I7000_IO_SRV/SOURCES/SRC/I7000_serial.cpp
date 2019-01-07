#include <io_srv_dump.h>
#include <I7000_serial.h>
#include <I7000_events_cash.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
I7000_SerialPort::I7000_SerialPort()
  :SerialPort(0xff)
{  
  state=BEGIN;
  send_len = 0;
  last_access_time = EventsCash.TimeInDbl();
}
//--------------------------------------------------------------------
bool I7000_SerialPort::Init(const char *name,int speed,unsigned buff_size)
{
  switch(speed)
  {
   case 1200:
     delay_val = 81;
     break;
   case 2400:
     delay_val = 41;
     break;
   case 4800:
     delay_val = 21;
     break;
   case 9600:
     delay_val = 11;
     break;
   case 19200:
     delay_val = 6;
     break;
   case 38400:
     delay_val = 4;
     break;
   case 57600:
     delay_val = 3;
     break;
   case 115200:
     delay_val = 2;
     break;
   default:
     delay_val = 11;
  }
  if(SerialPort::Reset(name,speed))
  {
    state=BEGIN;
    return true;
  }
  return false;
}
//--------------------------------------------------------------------
void I7000_SerialPort::Step()
{
  if(SerialPort::Ok())
  {
    ReadStep();
    SendStep();
  }   
}
//--------------------------------------------------------------------
void I7000_SerialPort::Send(const unsigned char *data,unsigned len)
{
  unsigned sz;
  //send_len = 0;//commented
  SerialPort::Send(data,len,sz);
  last_access_time = EventsCash.TimeInDbl();
  /*if(sz != len)//commented
  {
    memcpy(buff_out,data + sz,len - sz);    
    send_len = len - sz;
  }*/
}
//--------------------------------------------------------------------
const unsigned char *I7000_SerialPort::Recive(unsigned &sz)
{
  ReadStep();//added
  if(state == READED)
  {
    state=BEGIN;
    sz = read_pos-buff_in+1;    
  }
  else
    sz = 0;
  return buff_in;
}
//--------------------------------------------------------------------
void I7000_SerialPort::ReadStep()
{
  unsigned sz;
  unsigned char ch;
  ch=*SerialPort::Recive(1,sz);
  while(sz)
  { 
    switch(ch)
    {
      case 0x21: case 0x3E: case 0x3F:  //маркеры начала пакетов
        state=READING;
        read_pos=buff_in;
        *read_pos=ch;
        break;
      case 0x0D:        //маркер окончания пакета                 
        if(read_pos-buff_in+1<sizeof(buff_in))
        {
          state = READED;
          *++read_pos=ch;
          last_access_time = EventsCash.TimeInDbl();
        }
        else
        {
          //Logdef(Print(DEBUG_ERROR,"Порт %s : слишком длинная посылка от адаптера!",Name()))
          state=BEGIN;
        }
        break;
      default:
        if(state==READING)
        {
          if(read_pos-buff_in+1<sizeof(buff_in))
            *++read_pos=ch;
          else
          {
            //Logdef(Print(DEBUG_ERROR,"Порт %s : слишком длинная посылка от адаптера!",Name()))
            state=BEGIN;
          }
        }
        break;
    }
    if(state == READED)
      break;
    else
      ch=*SerialPort::Recive(1,sz);
  }
}
//--------------------------------------------------------------------
void I7000_SerialPort::SendStep()
{
  if(send_len)
    Send(buff_out,send_len);
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%