/********************************************************************
                     created by M.E.N.
********************************************************************/
#include <kp_serial_port.h>
#include <io_srv_dump.h>
#include <string>

using namespace std;
const int cdv_msg_flags = (D_PORT | D_MSG);
//-------------------------------------------------------------------
#ifdef MULTI_THREADED
# ifdef WIN32
    DWORD WINAPI ThreadFunc(LPVOID port_obj)
    {
      while(1)
      {
        ((KpSerialPort *)port_obj)->Step();
      }
      return 1;
    }
# else
    void *ThreadFunc(void *port_obj)
    {
      while(1)
      {
        ((KpSerialPort *)port_obj)->Step();
      }
      return NULL;
    }
# endif
#endif
//-------------------------------------------------------------------
const int cnl_flags = (D_MSG | D_PORT);
const int cnl_msg_flags = (D_CNL | D_MSG);
//-------------------------------------------------------------------
KpSerialPort::KpSerialPort()
  :SerialPort(),state(INIT)
{
  CRIT_SEC_CREATE(in_queue_lock)
  CRIT_SEC_CREATE(out_queue_lock)
  thread_work_flag=0;
#ifdef MULTI_THREADED
# ifdef WIN32
  thr=CreateThread( 
        NULL,                        // no security attributes 
        0,                           // use default stack size  
        ThreadFunc,                  // thread function 
        (void *)this,                // argument to thread function 
        CREATE_SUSPENDED ,           // изначально наша нить "спит" :)
        NULL);                        // returns the thread identifier 
  if(thr!=NULL)
  {
    ResumeThread(thr);
    dump.Print(D_PORT | D_MSG,"Порт %s: нить опроса порта создана! ",Name());
  }
  else
  {
    dump.Print(D_PORT | D_ERR,"Порт %s: ошибка создания нити опроса порта! %d",Name(),GetLastError());
  }
# else
  int res = pthread_create( &thr, NULL, ThreadFunc,(void*)this );
  if(res)
  {
    dump.Print(D_PORT | D_ERR,"Порт %s: ошибка создания нити опроса порта! %d",Name(),errno);
  }
  else
    dump.Print(D_PORT | D_MSG,"Порт %s: нить опроса порта создана! ",Name());
# endif
#endif
}
//-------------------------------------------------------------------
KpSerialPort::~KpSerialPort()
{
  StopPortPoll();
#ifdef MULTI_THREADED
  dump.Print(D_PORT | D_MSG,"Порт %s: нить опроса порта удалена! ",Name());
# ifdef WIN32
  TerminateThread(thr,0);
# else
  pthread_kill(thr,SIGKILL);
# endif
#endif
  list<BinData *>::iterator i;
  for(i=InQueue.begin();i!=InQueue.end();i++)
    delete (*i);
  for(i=OutQueue.begin();i!=OutQueue.end();i++)
    delete (*i);
  InQueue.clear();
  OutQueue.clear();
  Data.Free();
  CRIT_SEC_FREE(in_queue_lock)
  CRIT_SEC_FREE(out_queue_lock)
}
//-------------------------------------------------------------------
void KpSerialPort::StartPortPoll()
{
  thread_work_flag=1;
}
//-------------------------------------------------------------------
void KpSerialPort::StopPortPoll()
{
  thread_work_flag=0;
}
//-------------------------------------------------------------------
//Во Входную очередь помещаются данные уже без байт-стаффингов
void KpSerialPort::Step()
{
  if(Ok() && thread_work_flag)
  {
/*#ifdef WIN32
      Sleep(2);
#else
      usleep(2000);
#endif*/
    //clock_t cl2=clock();//kill me!
    ReadStep();
    SendStep();
    //Logdef(Print(DEBUG_STEP,"Цикл обработки порта %d",clock()-cl2))
  }
}
//-------------------------------------------------------------------
int KpSerialPort::InQueueSize()const
{
  int result;
  CRIT_SEC_BEGIN(in_queue_lock)
  result = InQueue.size();
  CRIT_SEC_END(in_queue_lock)
  return result;
}
//-------------------------------------------------------------------
int KpSerialPort::OutQueueSize()const
{
  int result;
  CRIT_SEC_BEGIN(out_queue_lock)
  result = OutQueue.size();
  CRIT_SEC_END(out_queue_lock)
  return result;
}
//-------------------------------------------------------------------
void KpSerialPort::Send(const unsigned char *data,unsigned len,int cnl)
{
  int i,sz;
  static unsigned char tmp[256];
  unsigned char *ptr;
  unsigned crc;
  if(len > sizeof(tmp)+4)
    return;
  //заголовок для адаптера
  tmp[0]=BEGMARK;
  *(unsigned *)&tmp[1]=0xffffffff;
  tmp[4]=(unsigned char)cnl;
  tmp[5]=0x08;
  ptr=tmp+6;
  memcpy(ptr,data,len);//:(((((((    
  crc=CRC(tmp+1,len+5);
  if(cnl == 0x02 || cnl == 0x3)
  {
    tmp[4] = PREFIX;
    tmp[5]=(unsigned char)cnl;
    tmp[6]=0x08;
    ptr=tmp+7;
  }
  else
  {
    ptr=tmp+6;
  }
  //посылка к КП
  for(i=0;i<len;i++)
  {
    if(data[i]==BEGMARK || data[i]==ENDMARK || data[i]==PREFIX)
    { //установка байт-стаффингов
      *ptr=PREFIX;
      *++ptr=data[i];
    }
    else
      *ptr=data[i];
    ptr++;
  }  
  *ptr++=(unsigned char)crc;
  *ptr++=(unsigned char)(crc >> 8);
  *ptr=ENDMARK;
  sz=ptr-tmp+1;
  if((dump.Flags() & cdv_msg_flags) == cdv_msg_flags)
  {
    char vl[10];
    string packet = "";
    dump.Print(cdv_msg_flags,"Отправка в %s[%u]:",Name(),cnl);
    for(int i=0;i<sz;i++)
    {
      sprintf(vl," %X",(unsigned char)tmp[i]);
      packet += vl;
    }
    dump.Print(cdv_msg_flags," %s ",packet.c_str());
  }  
  CRIT_SEC_BEGIN(out_queue_lock)
  OutQueue.push_back(new BinData((void *)tmp,sz));
  CRIT_SEC_END(out_queue_lock)
}
//-------------------------------------------------------------------
const BinData *KpSerialPort::Recive()const
{
  unsigned char *data;
  list<BinData *>::iterator i;
  CRIT_SEC_BEGIN(in_queue_lock)
  i=InQueue.begin();
  if(i!=InQueue.end())
  {
    data=(unsigned char *)*(*i);
    if(CRC(data,(*i)->Len()))
    {
      dump.Print(D_ERR | D_PORT,"Порт %s: ошибка контрольной суммы пакета!",Name());
      Data.Free();
    }
    else
    {
      Data=*(*i);
      if((dump.Flags() & cnl_flags) == cnl_flags)
      {
        char vl[10];
        string packet = "";
        dump.Print(cnl_flags,"Чтение из %s:",Name());
        for(int i=0;i<Data.Len();i++)
        {
          sprintf(vl," %X",(unsigned char)data[i]);
          packet += vl;
        }
        dump.Print(cnl_flags," %s",packet.c_str());
      }
    }
    delete (*i);
    InQueue.erase(i);
  }
  else
    Data.Free();
  CRIT_SEC_END(in_queue_lock)
  return &Data;
}
//-------------------------------------------------------------------
void KpSerialPort::ReadStep()
{
  char c;
  unsigned size=1;
  if(!Ok())
    return;
  c=*SerialPort::Recive(1,size);
  while(size)
  {    
    switch (state)
    {
      case START:
        if(c == BEGMARK)
        {
          state = READING;
          cnt = 0;
        }
        break;
      case READING:
        if(c == PREFIX)
          state = PREFIX_SKIP;
        else
        if(c == ENDMARK)
        {
          CRIT_SEC_BEGIN(in_queue_lock)
          InQueue.push_back(new BinData((void *)buff,cnt));//пакет прочитан
          CRIT_SEC_END(in_queue_lock)
          state = START;
          //return; //kill ME!!!!!!!!
        }
        else
        {
          if (cnt >= sizeof(buff))
          {
           dump.Print(D_ERR | D_PORT,"%s : переполн.вход.буфер драйвера от КП\n  Слишком длинная посылка от КП",Name());
            state = START;
            //cnt=0;
          }
          else
            buff[cnt++] = c;
        }
        break;
      case PREFIX_SKIP:  //удаление байт-стаффингов
        if(c != BEGMARK && c != ENDMARK)
        {
          if (cnt >= sizeof(buff))
          {
           dump.Print(D_ERR | D_PORT,"%s : переполн.вход.буфер драйвера от КП\n  Слишком длинная посылка от КП",Name());
            state = START;
          }
          else
            buff[cnt++] = PREFIX;
        }
        if (c != PREFIX)
        {
          if (cnt >= sizeof(buff))
          {
            dump.Print(D_ERR | D_PORT,"%s : переполн.вход.буфер драйвера от КП\n  Слишком длинная посылка от КП",Name());
            state = START;
          }
          else
            buff[cnt++] = c;
        }
        state = READING;
        break;
      default:
        state = START;
    }
    c=*SerialPort::Recive(1,size);
  }
}
//-------------------------------------------------------------------
void KpSerialPort::SendStep()
{
  unsigned sended;
  list<BinData *>::iterator i;
  CRIT_SEC_BEGIN(out_queue_lock)
  for(i=OutQueue.begin();i!=OutQueue.end();i++)
  {
    SerialPort::Send((unsigned char*)(*i)->Data(),(*i)->Len(),sended);
    delete (*i);
  }
  OutQueue.clear();
  CRIT_SEC_END(out_queue_lock)
}
//-------------------------------------------------------------------
unsigned KpSerialPort::CRC(const unsigned char *p, unsigned len)const
{
  static int crctabl[16] =
  {
    0x0000, 0x1081, 0x2102, 0x3183, 0x4204, 0x5285, 0x6306, 0x7387,
    0x8408, 0x9489, 0xa50a, 0xb58b, 0xc60c, 0xd68d, 0xe70e, 0xf78f
  };
  unsigned k;
  for (k=0; len--; p++)
  {
    k = (( k >> 4 ) & 0x0fff ) ^ crctabl[(( k ^ (*p) ) & 0x0f)];
    k = (( k >> 4 ) & 0x0fff ) ^ crctabl[(( k ^ ((*p) >> 4)) & 0x0f)];
  }
  return k;
}
//-------------------------------------------------------------------

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%