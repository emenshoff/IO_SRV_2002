#include <kp_types.h>
#include <kp_cdv.h>
#include <kp_io_srv.h>
#include <io_srv_dump.h>
#include <kp_events_cash.h>
#include <string>

using namespace std;

const int cnl_msg_flags = (D_CNL | D_MSG);
const int cdv_msg_flags = (D_PORT | D_MSG);
#ifndef WIN32
  #define INVALID_HANDLE_VALUE -1
#endif
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
KpCommDev::KpCommDev(obj_d &cfg_ds)
 :ControlObject(cfg_ds),state(INIT),Port(INVALID_HANDLE_VALUE),
 port_lock(0),skip_packets_cnt(0)
{
  ok = false;
  unsigned char err;
  unsigned port_speed = cfg_d[COMM_DEV.PortSpeed().CfgFldId()];
  const char *name = cfg_d[COMM_DEV.PortName().CfgFldId()];
  port_name = strdup(name);
  OutBuff.Len=0;
  //инициализация порта:
  if(Reset(name,(int)port_speed))
    err = 0;
  else
    err = 100;
  SetError(err);
  for(int i=0;i<=3;i++)
    channels[i]=0;
}
//---------------------------------------------------------------
KpCommDev::~KpCommDev()
{
  //удаление каналов
  for(int i=0;i<=3;i++)
    if(channels[i])
      delete channels[i];
  free(port_name);
  //очистка очередей
  list<BinData *>::iterator x;
  for(x=InQueue.begin();x!=InQueue.end();x++)
    delete (*x);
  for(x=OutQueue.begin();x!=OutQueue.end();x++)
    delete (*x);
  InQueue.clear();
  OutQueue.clear();
  Data.Free();
}
//---------------------------------------------------------------
bool KpCommDev::Reset(const char *pt_name,int spd)
{
  speed=spd;
  if(!pt_name)
  { 
    return ok=false;
  }
  free(port_name);
  port_name=strdup(pt_name);
#ifdef WIN32    //WIN32 platform
  if(Port!=INVALID_HANDLE_VALUE)
    CloseHandle(Port);
  COMMTIMEOUTS t_outs;
  COMMCONFIG	cfg;
  DWORD size;
  Port=CreateFile(
                   port_name,GENERIC_READ | GENERIC_WRITE,
                   NULL,    
                   NULL, 
                   OPEN_EXISTING,
                   NULL,   
                   NULL  
                 );
  if(Port==INVALID_HANDLE_VALUE || !GetCommConfig(Port,&cfg,&size))
  {
	  dump.Print(D_ERR,"\tПорт %s не доступен!", port_name);    
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
  if(!SetCommConfig(Port,&cfg,cfg.dwSize) || !SetCommTimeouts(Port,&t_outs))
  {
    dump.Print(D_ERR,"\tОшибка установки параметров порта %s !", port_name);
    return ok=false;
  }
#else   //UNIX (posix) platform
  termios Settings;
  if(Port!=-1)
    close(Port);
  Port=open(port_name,O_RDWR | O_NONBLOCK);
  if(Port==-1)
  {
    dump.Print(D_ERR,"\tПорт %s не доступен!", port_name);
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
    dump.Print(D_ERR,"\tОшибка установки параметров порта %s !", port_name);
    return ok=false;
  }
#endif
  dump.Print(D_MSG,"\tПорт %s открыт...", port_name);
  return (ok=true);  
}
//---------------------------------------------------------------
int KpCommDev::SendPort(const unsigned char *data,unsigned len,int &writed)
{
  if(!ok)
    return writed = -1;
  int error_id;
 #ifdef WIN32
  if(WriteFile(Port,(void*)data,len,(unsigned long *)&writed,NULL))
 #else
  if((writed=write(Port, (void*)data, len))!= -1)
 #endif
  {    
  }
  else
  {
   #ifdef WIN32
    error_id = GetLastError();
   #else
    error_id = errno;
   #endif
    dump.ObjMsg(cfg_d.Id(),D_ERR | D_PORT,"Порт %s: Ошибка записи %d (%s)!",port_name,error_id,strerror(errno));
  }
  return writed;
}
//---------------------------------------------------------------
int KpCommDev::ReadPort(unsigned char *read_buff,unsigned length,int &size)
{
  int err_code = 0;
  unsigned char tmp=0x00;
  size=0;
  if(!ok)
    return size = -1;
#ifdef WIN32
  unsigned long num=0;  
#else
  int num;
#endif
  for(unsigned i=0;i<length;i++)
  {
    #ifdef WIN32
    if(ReadFile(Port,(void*)&tmp,1,&num,NULL))
    #else
    if((num=read(Port, (void*)&tmp, 1))!= -1)
    #endif
    {
      *(read_buff + size)=tmp;
      size+=num;      
    }
    else
    {
      #ifdef WIN32
      err_code = GetLastError();
      #else
      err_code = errno;
      #endif
      break;
    }
  }
  if(err_code)
  {
    #ifndef WIN32
    if(err_code = 35)
    {
      dump.ObjMsg(cfg_d.Id(),D_ERR | D_PORT,"Порт %s: ошибка чтения! error= %d (%s)", port_name,err_code,strerror(errno));
    }//  dump.ObjMsg(cfg_d.Id(),D_ERR | D_PORT,"%s нет данных!", port_name);
    else
    #endif
    dump.ObjMsg(cfg_d.Id(),D_ERR | D_PORT,"Порт %s: ошибка чтения! error= %d", port_name,err_code);
  }
  return size;
}
//---------------------------------------------------------------
bool KpCommDev::Init()
{
  return Ok();
}
//---------------------------------------------------------------
void KpCommDev::InitChannels()
{
  for(int i=0;i<=3;i++)
    if(channels[i])
      SendChannelInit(channels[i]);
  last_init_time = time(0);
}
//---------------------------------------------------------------
void KpCommDev::SendChannelInit(KpCnl* cnl)
{  
  int sz;
  unsigned char speed;
  if(cnl)
  {
        switch(cnl->KpSpeed())
        {
          case 100:
            speed=0;
            break;
          case 200:
            speed=1;
            break;
          case 300:
            speed=2;
            break;
          case 600:
            speed=3;
            break;
          case 1200:
            speed=4;
            break;
          case 2400:
            speed=5;
            break;
          case 4800:
            speed=6;
            break;
          default:
            return;
        }
        static unsigned char conf[] =  //енто конфигурация адаптера
        {
          BEGMARK,
          0xff,
          0xff,
          0xff,
          PK_CFG_SEND, // (тип пакета)
          0x30+cnl->Num(),  //N   - номер адаптера задается перемычками 
          ' ',
          '8',  //Pid - идентификатор протокола (8-радиальный, 2-магистральный с радиомодемом)
          ' ',
          0x30+speed,//V   - скорость(кп) 0-100, 1-200, 2-300, 3-600, 4-1200, 5-2400, 6-4800
          ' ',
          '0',  //W   - выдержка между ответом модема и началом передачи в тиках 50 мс
          0x0d,
          0x0a,
          0,
          0,          
          ENDMARK
        };
    conf[5] = 0x30 + cnl->Num();
    conf[9] = 0x30 + speed;
    unsigned crc=CRC(conf+1,sizeof(conf)-4);
    conf[14]=(unsigned char)crc;
    conf[15]=(unsigned char)(crc >> 8);
    SendPort(conf,sizeof(conf),sz);
    //дамп пакета
    if((dump.Flags() & cnl_msg_flags) == cnl_msg_flags) //дамп посылки (уровень канала)
    {
      char vl[10];
      string packet = "";
      dump.ObjMsg(cnl->Cfg_D()->Id(),cnl_msg_flags,"Канал %s : Отсылка конфигурации :",cnl->Cfg_D()->Name());
      for(register int i=0;i<sizeof(conf);i++)
      {
        sprintf(vl," %X",conf[i]);
        packet += vl;
      }
      dump.ObjMsg(cnl->Cfg_D()->Id(),cnl_msg_flags," %s ",packet.c_str());
    }
    if((dump.Flags() & cdv_msg_flags) == cdv_msg_flags) //дамп посылки (уровень порта)
    {
      char vl[10];
      string packet = "";
      dump.ObjMsg(cfg_d.Id(),cdv_msg_flags,"Порт %s(%s)[%d] : Отсылка конфигурации канала %d:",cfg_d.Name(),Name(),cnl->Num(),cnl->Num());      
      for(register int i=0;i<sizeof(conf);i++)
      {
        sprintf(vl," %X",conf[i]);
        packet += vl;
      }
      dump.ObjMsg(cfg_d.Id(),cdv_msg_flags," %s ",packet.c_str());
    }
  }
}
//---------------------------------------------------------------
void KpCommDev::Process()
{
  if(!Ok())
    return ;
  int sz;
#ifndef MULTI_THREADED
  Step();
#endif
  unsigned char tmp[8],*data=(unsigned char*)(KpCommDev::Recive()->Data());
  if(Data.Len())
  {
    RawHeader *head=(RawHeader *)data; 
    if(head->address > 3) //пакет канального уровня
    {
      switch(data[3])     //тип пакета
      {
        case 0xa0:
          dump.ObjMsg(cfg_d.Id(),cdv_msg_flags,"Порт %s(%s), адаптер канала : запрос конфигурации...",cfg_d.Name(),Name());
          InitChannels();        
          break;
        case 0xb0:
          dump.ObjMsg(cfg_d.Id(),cdv_msg_flags,"Порт %s(%s), адаптер канала : сконфигурирован!",cfg_d.Name(),Name());
          memcpy(tmp+1,data,8);
          tmp[0]=0x02; tmp[7]=0x03;
          SendPort(tmp,sizeof(tmp),sz);//квитирую пакет, отправляя его обратно
          break;
        default:
          break;
      }
      return ;
    }
    char vl[10];
    string packet = "";
    if(head->proto == 0x08)     //пакет с данными!
    {
      int flags = dump.Flags();
      if((flags & cnl_msg_flags) == cnl_msg_flags)//||(flags & cnl_msg_flags) == cnl_msg_flags)
        for(int i=0;i<Data.Len();i++) //подготовка к печати пакета
        {
          if(data[i]<16)
            sprintf(vl," 0%X",data[i]);
          else
            sprintf(vl," %X",data[i]);
          packet += vl;
        }
      if(Data.Len()<=8) //отсеиваем квитанции (во вх. очереди они нам не нужны!)
      {
        dump.ObjMsg(cfg_d.Id(),cdv_msg_flags,"Порт %s(%s)[%u]: пришла квитанция от адаптера...",cfg_d.Name(),Name(),data[3]);
        if((dump.Flags() & cnl_msg_flags) == cnl_msg_flags)
          if(channels[head->address])
          {           
            unsigned id = channels[head->address]->Cfg_D()->Id();
            dump.ObjMsg(id,cnl_msg_flags,"Канал %s: пришла квитанция:",channels[head->address]->Cfg_D()->Name());            
            dump.ObjMsg(id,cnl_msg_flags," %s",packet.c_str());
            channels[head->address]->InKvit();
          }
      }
      else
      {        
        //дамп пакета
        //уровень канала
        if((dump.Flags() & cnl_msg_flags) == cnl_msg_flags)
          if(channels[head->address])
        {
          unsigned id = channels[head->address]->Cfg_D()->Id();
          dump.ObjMsg(id,cnl_msg_flags,"Канал %s: пришел пакет с данными:",channels[head->address]->Cfg_D()->Name());
          dump.ObjMsg(id,cnl_msg_flags," %s",packet.c_str());
        }
        //уровень порта
        if((dump.Flags() & cdv_msg_flags) == cdv_msg_flags)
        {
          dump.ObjMsg(cfg_d.Id(),cdv_msg_flags,"Порт %s(%s)[%u]: пришел пакет с данными...",cfg_d.Name(),Name(),data[3]);
          //dump.ObjMsg(cfg_d.Id(),cdv_msg_flags," %s",packet.c_str());          
        }
        if(channels[head->address])
        {
          channels[head->address]->Process(data,Data.Len());
          channels[head->address]->Step();
          channels[head->address]->s_flag=1;
          channels[head->address]->last_in_time = time(0);
        }
      }
    }
    else if(head->proto == 0xfd)
    {
      if((dump.Flags() & cdv_msg_flags) == cdv_msg_flags)
        dump.ObjMsg(cfg_d.Id(),cdv_msg_flags,"Порт %s(%s)[%u]: Пакет с диагностикой канала связи...",cfg_d.Name(),Name(),data[3]);        
      if(channels[data[3]])
      {
        unsigned id = channels[data[3]]->Cfg_D()->Id();
        if((dump.Flags() & cnl_msg_flags) == cnl_msg_flags)
        {
          dump.ObjMsg(id,cnl_msg_flags,"Канал %s: Пакет с диагностикой канала связи:",channels[data[3]]->Cfg_D()->Name());        
          dump.ObjMsg(id,cnl_msg_flags,"\t\tАвария %u",data[6]);
          dump.ObjMsg(id,cnl_msg_flags,"\t\tЧисло принятых пакетов %u",*(unsigned*)&data[7]);
          dump.ObjMsg(id,cnl_msg_flags,"\t\tЧисло переданных пакетов %u",*(unsigned*)&data[11]);
          dump.ObjMsg(id,cnl_msg_flags,"\t\tЧисло ошибок приема %u",*(unsigned*)&data[15]);
          dump.ObjMsg(id,cnl_msg_flags,"\t\tЧисло неквитанций %u",*(unsigned*)&data[19]);
        }        
        cnl_diagnostic_info diag;
        diag.err_cnt=*(unsigned long *)&data[15];
        diag.error=data[6];
        diag.nonkvit_cnt=*(unsigned long*)&data[19];
        diag.recv_cnt=*(unsigned long*)&data[7];
        diag.snd_cnt=*(unsigned long*)&data[11];
        channels[data[3]]->SetDiagnosticInfo(diag);
        channels[head->address]->Step();
        channels[head->address]->s_flag=1;
      }
    }
    else
    {
      if((dump.Flags() & cdv_msg_flags) == cdv_msg_flags)
      {
        dump.ObjMsg(cfg_d.Id(),cdv_msg_flags,"Порт %s(%s)[%u]: От Адаптера пришел мусор :",cfg_d.Name(),Name(),head->address);
        char vl[10];
        string packet = "";          
        for(int i=0;i<Data.Len();i++)
        {
          if(tmp[i]<16)
            sprintf(vl," 0%X",(unsigned char)tmp[i]);
          else
            sprintf(vl," %X",data[i]);
          packet += vl;
        }
        dump.ObjMsg(cfg_d.Id(),cdv_msg_flags," %s",packet.c_str());
      }
    }
  }
  //заплатка
  bool must_init = false;
  for(int i=0;i<3;i++)
    if(channels[i])
    {
      if(channels[i]->s_flag)
        channels[i]->s_flag=0;
      else
        channels[i]->Step();
      //if(!channels[i]->KvitIsRecived())
        if(channels[i]->Timeout())
        {
          //dump.ObjMsg(channels[i]->Cfg_D()->Id(),"Канал %s(%u) : таймаут ожидания посылок и квитанций! переинициализация канала...",channels[i]->Cfg_D()->Name(),channels[i]->Num());
          //dump.ObjMsg(cfg_d.Id(),cdv_msg_flags,"Порт %s(%s): таймаут ожидания посылок и квитанций с канала %d!",cfg_d.Name(),Name(),i);
          must_init = true;
        }
    }    
  if(must_init)
    if(time(0)-last_init_time > 240)//600)
    {
      dump.ObjMsg(cfg_d.Id(),cdv_msg_flags,"Порт %s(%s): инициализация каналов...",cfg_d.Name(),Name());
      InitChannels();
    }
}
//---------------------------------------------------------------
void KpCommDev::SetError(unsigned char err)
{
  double tm = EventsCash.TimeInDbl();
  cfg_d.SetFldValue(COMM_DEV.error().CfgFldId(),(void*)&err,COMM_DEV.error().Type());
  cfg_d.SetFldValue(COMM_DEV.etime().CfgFldId(),(void*)&tm,COMM_DEV.etime().Type());
  cfg_d.SetFldChangeBit(COMM_DEV.error().CfgFldId());
  cfg_d.SetFldChangeBit(COMM_DEV.etime().CfgFldId());
  EventsCash.Add(cfg_d.Id());
  cfg_d.ClearFldChangeBit(COMM_DEV.error().CfgFldId());
  cfg_d.ClearFldChangeBit(COMM_DEV.etime().CfgFldId());
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

//-------------------------------------------------------------------
//Во Входную очередь помещаются данные уже без байт-стаффингов
void KpCommDev::Step()
{
  if(Ok())
  {
/*#ifdef WIN32
      Sleep(2);
#else
      usleep(2000);
#endif*/
    //clock_t cl2=clock();//kill me!
    ReadStep();
    //dump.Print(D_MSG,"размер входной очереди порта %s: %d",port_name,InQueueSize());
    //dump.Print(D_MSG,"размер выходной очереди порта %s: %d",port_name,OutQueueSize());
    SendStep();
    //Logdef(Print(DEBUG_STEP,"Цикл обработки порта %d",clock()-cl2))
  }
}
//-------------------------------------------------------------------
int KpCommDev::InQueueSize()const
{
  int result;  
  result = InQueue.size();  
  return result;
}
//-------------------------------------------------------------------
int KpCommDev::OutQueueSize()const
{
  int result;  
  result = OutQueue.size();  
  return result;
}
//-------------------------------------------------------------------
void KpCommDev::Send(const unsigned char *data,unsigned len,int cnl)
{
  int i,sz;
  static unsigned char tmp[256];//буфер для формирования посылки
  unsigned char *ptr,crc_h,crc_l;
  unsigned crc;
  if(len > sizeof(tmp)+4)
    return;
  //заголовок для адаптера
  tmp[0]=BEGMARK;
  *(unsigned *)&tmp[1]=0xffffffff;
  tmp[4]=(unsigned char)cnl;
  tmp[5]=0x08;
  ptr=tmp+6;
  memcpy(ptr,data,len);
  //считаем контр.сумму:
  crc=CRC(tmp+1,len+5);
  crc_h = (unsigned char)crc;
  crc_l = (unsigned char)(crc >> 8);
  //вставляем байт-стаффинги в каналньный префикс:
  if(cnl == 0x02 || cnl == 0x03)
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
  //вставка байт-стаффингов в тело пакета:
  for(i=0;i<len;i++)
  {
	  //установка байт-стаффингов
    if(data[i]==BEGMARK || data[i]==ENDMARK || data[i]==PREFIX)
      *ptr++=PREFIX;
    *ptr++=data[i];
  }  
  //вставка байт-стаффингов в контр.сумму:
  if(crc_h==BEGMARK || crc_h==ENDMARK || crc_h==PREFIX)
    *ptr++ = PREFIX;
  *ptr++ = crc_h;
  if(crc_l==BEGMARK || crc_l==ENDMARK || crc_l==PREFIX)
    *ptr++ = PREFIX;
  *ptr++ = crc_l;
  *ptr=ENDMARK;
  sz=ptr-tmp+1;
  if((dump.Flags() & cdv_msg_flags) == cdv_msg_flags)
  {
    char vl[10];
    string packet = "";
    dump.ObjMsg(cfg_d.Id(),cdv_msg_flags,"Порт %s(%s) : Отправка: ",cfg_d.Name(),Name());
    for(int i=0;i<sz;i++)
    {
      if(tmp[i]<16)
        sprintf(vl," 0%X",(unsigned char)tmp[i]);
      else
        sprintf(vl," %X",(unsigned char)tmp[i]);
      packet += vl;
    }
    dump.ObjMsg(cfg_d.Id(),cdv_msg_flags," %s ",packet.c_str());
  }  
  if((dump.Flags() & cnl_msg_flags) == cnl_msg_flags)
    if(channels[cnl])
    {
      char vl[10];
      string packet = "";
      dump.ObjMsg(channels[cnl]->Cfg_D()->Id(),cnl_msg_flags,"Канал %s: отправка:",channels[cnl]->Cfg_D()->Name());
      for(int i=0;i<sz;i++)
      {
        if(tmp[i]<16)
          sprintf(vl," 0%X",(unsigned char)tmp[i]);
        else
          sprintf(vl," %X",(unsigned char)tmp[i]);
        packet += vl;
      }
      dump.ObjMsg(channels[cnl]->Cfg_D()->Id(),cnl_msg_flags," %s ",packet.c_str());
    }
  
  OutQueue.push_back(new BinData((void *)tmp,sz));  
}
//-------------------------------------------------------------------
const BinData *KpCommDev::Recive()const
{
  unsigned char *data;
  list<BinData *>::iterator i;
  
  i=InQueue.begin();
  if(i!=InQueue.end())
  {
    data=(unsigned char *)*(*i);
    if(CRC(data,(*i)->Len()))
    {
      dump.ObjMsg(cfg_d.Id(),D_ERR | D_PORT,"Порт %s(%s): ошибка контрольной суммы пакета!",cfg_d.Name(),Name());
      Data.Free();
    }
    else
    {
      Data=*(*i);
      if((dump.Flags() & cdv_msg_flags) == cdv_msg_flags)
      {
        char vl[10];
        string packet = "";
        dump.ObjMsg(cfg_d.Id(),cdv_msg_flags,"Порт %s(%s): Чтение:",cfg_d.Name(),Name());
        for(int i=0;i<Data.Len();i++)
        {
          sprintf(vl," %X",(unsigned char)data[i]);
          packet += vl;
        }
        dump.ObjMsg(cfg_d.Id(),cdv_msg_flags," %s",packet.c_str());
      }
    }
    delete (*i);
    InQueue.erase(i);
  }
  else
    Data.Free();
  
  return &Data;
}
//-------------------------------------------------------------------
void KpCommDev::ReadStep()
{
  unsigned char c;
  int size=1;
  if(!Ok())
    return;
  ReadPort(&c,1,size);
  while(size > 0)// && size < 30)//14.04.02 by MEN
  {    
    switch (state)
    {
      case START:
        if(c == BEGMARK)
        {
          state = READING;
          InBuff.Len = 0;
        }
        break;
      case READING:
        if(c == PREFIX)
          state = PREFIX_SKIP;
        else
        if(c == ENDMARK)
        {          
          InQueue.push_back(new BinData((void *)InBuff.Buff,InBuff.Len));//пакет прочитан          
          state = START;
          if(InQueue.size() > 10);
            return; //kill ME!!!!!!!!
        }
        else
        {
          if (InBuff.Len >= InBuff.Size())
          {
           dump.ObjMsg(cfg_d.Id(),D_ERR | D_PORT,"Порт %s(%s): переполн.вход.буфер! Слишком длинная посылка от КП",cfg_d.Name(),Name());
            state = START;
            //InBuff.Len=0;
          }
          else
            InBuff.Buff[InBuff.Len++] = c;
        }
        break;
      case PREFIX_SKIP:  //удаление байт-стаффингов
        if(c != BEGMARK && c != ENDMARK)
        {
          if (InBuff.Len >= InBuff.Size())
          {
           dump.ObjMsg(cfg_d.Id(),D_ERR | D_PORT,"Порт %s(%s): переполн.вход.буфер! Слишком длинная посылка от КП",cfg_d.Name(),Name());
            state = START;
          }
          else
            InBuff.Buff[InBuff.Len++] = PREFIX;
        }
        if (c != PREFIX)
        {
          if (InBuff.Len >= InBuff.Size())
          {
            dump.ObjMsg(cfg_d.Id(),D_ERR | D_PORT,"Порт %s(%s): переполн.вход.буфер! Слишком длинная посылка от КП",cfg_d.Name(),Name());
            state = START;
          }
          else
            InBuff.Buff[InBuff.Len++] = c;
        }
        state = READING;
        break;
      default:
        state = START;
    }
    ReadPort(&c,1,size);
  }
}
//-------------------------------------------------------------------
void KpCommDev::SendStep()//MEN 15.04.02
{
  int sended;
  list<BinData *>::iterator i;
  //проверяем, осталось ли что-нибудь от прошлой попытки отсылки:
  if(OutBuff.Len)
  {
    SendPort((unsigned char*)OutBuff.Buff,OutBuff.Len,sended);
    if(sended != OutBuff.Len)
      OutBuff.Out(OutBuff.Len-sended);
    else
      OutBuff.Len = 0;
  }
  //иначе делаем попытку отсылки вых. очереди:
 else
  for(i=OutQueue.begin();i!=OutQueue.end();i++)
  {     

    //попытка отправки пакета целиком
    SendPort((unsigned char*)(*i)->Data(),(*i)->Len(),sended);
    if(sended == -1)
    {
      if(errno == 5)
      { 
      OutBuff.Len = 0;
      delete (*i);
      OutQueue.erase(i);
      i=OutQueue.begin();
      dump.ObjMsg(cfg_d.Id(),D_ERR | D_PORT,"Порт %s(%s): переполн.выход.буфер!",cfg_d.Name(),Name());

      }
    }
    //иначе дошлем потом - в порт не пишется.
    else if(sended != (*i)->Len())
    {
      OutBuff.Len = (*i)->Len() - sended;    
      memcpy(OutBuff.Buff,(void*)((char*)(*i)->Data() + sended),OutBuff.Len);      
      return;//возврат - очередь не отослана полностью
    }
    //иначе удаляем отосланый пакет из очереди
    else if(sended == (*i)->Len())
    {
      OutBuff.Len = 0;
      delete (*i);
      OutQueue.erase(i);
      i=OutQueue.begin();
    }
  }
  OutQueue.clear();
}
//-------------------------------------------------------------------
unsigned KpCommDev::CRC(const unsigned char *p, unsigned len)const
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
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%