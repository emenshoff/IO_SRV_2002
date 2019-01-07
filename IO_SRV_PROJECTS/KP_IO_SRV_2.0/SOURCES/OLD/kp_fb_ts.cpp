/***********************************************************
  Считается, что если объект верхнего уровня отрублен
   (Enabled = 0), то он не вызовет ProcPacket и Step
               created by M.E.N
***********************************************************/

#include <kp_dev.h>
#include <kp_fb_ts.h>
#include <kp_types.h>
#include <time_utils.h>
#include <io_srv_dump.h>
#include <kp_events_cash.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
FbTs::FbTs(obj_d &d)
:AFB(d)
{
  for(unsigned i=0;i<TS_COUNT;i++)
   Items[i] = 0;
  state=FB_TS_ERROR;
  SetError(100);   //в начале была недостоверносtь...
  memset(tmp_data,0,sizeof(tmp_data));
  timeout_begin_time=time(0);
}
//---------------------------------------------------------------
FbTs::~FbTs()
{
  SetError(100);
  for(unsigned i=0;i<TS_COUNT;i++)
    if(Items[i])
      delete Items[i];
}
//---------------------------------------------------------------
void FbTs::Enable()
{
  if(Enabled())
  {
    /*SetError(0);
    state = FB_TS_ERROR;*/
  }  
}
//---------------------------------------------------------------
void FbTs::Disable()
{
  /*SetError(100);
  state = FB_TS_DISABLED;*/
}
//---------------------------------------------------------------
unsigned char FbTs::Error()const
{
  return cfg_d[KP_AFB_TS.error().CfgFldId()];
}
//---------------------------------------------------------------
unsigned char FbTs::Place()const
{
  return (unsigned char)cfg_d[KP_AFB_TS.Place().CfgFldId()];
}
//---------------------------------------------------------------
const char *FbTs::Parent()const
{
  return (const char *)cfg_d[KP_AFB_TS.Parent().CfgFldId()];
}
//---------------------------------------------------------------
void FbTs::ProcPacket(const unsigned char *dt,unsigned len)
{
  if(len<21)
    return;
  if(!Enabled())
  {
    Disable();
    return;
  }
  else
  {
    Enable();// снятие блокировки
  }
  int i;
  if((dt[2] >> 4)!=0x2)
  {
    dump.ObjMsg(cfg_d.Id(),D_AFB | D_ERR,"%s : Неверный тип пакета для субблока ТС!",cfg_d.Name());
    return;
  }  
  const unsigned char *data=dt+3; //настраиваемся сразу на начало данных
  GetCurrTime(last_in_time);
  //сравнение прямого и инверсного кодов ТС
  for(i=0;i<8;i++)
    if((data[i] & data[i+8])!=0)
    {
      dump.ObjMsg(cfg_d.Id(),D_AFB | D_ERR,"%s : Несовпадение прямого и инверсного кодов ТС!",cfg_d.Name());
      state=FB_TS_ERROR;
      return;
    }
  unsigned char err = cfg_d[KP_AFB_TS.error().CfgFldId()];
  switch(state)
  {
    case FB_TS_ERROR:   //в этих случаях значения заносим сразу
      SetError(0);
      state=FB_TS_STEP;
      memcpy(tmp_data,data+8,8);  //записываем в буфер данные в прямом коде
      WriteStates();              //пишем текущие значения тс в объекты
      break;
    //отработка задержки. если в момент отработки задержки происходит переключение, то время и буфер состояний обновляются
    case FB_TS_WAIT:
      memcpy(tmp_data,data+8,8);
      break;
    case FB_TS_STEP : case FB_TS_TIMEOUT:
      memcpy(tmp_data,data+8,8);
      if(!ValidTime())                  //таймаут дребезга не отрабатывается (его значение = 0)
        WriteStates();
      else                              //иначе нужна задержка
        state=FB_TS_WAIT;
      break;
    case FB_TS_DISABLED:
      return;
    default:
      break;
  }
}
//---------------------------------------------------------------
char FbTs::Step()
{
  if(!Enabled())
  {
    Disable();
    return 0;
  }
  else
  {
    Enable();
  }
  struct timeval tv;
  GetCurrTime(tv);
  switch(state)
  {
    case FB_TS_STEP:
      if(ResponceTimeout())
      {   
        //dump.Print(D_AFB | D_MSG,"%s: таймаут (%d сек.) ожидания посылки!",cfg_d.Name(),ResponceTimeoutVal());
        dump.ObjMsg(cfg_d.Id(),D_AFB | D_MSG,"%s: таймаут (%d сек.) ожидания посылки!",cfg_d.Name(),ResponceTimeoutVal());
        state=FB_TS_TIMEOUT;
        timeout_begin_time=poll_delay_begin_time=time(0);
      }
      break;
    case FB_TS_WAIT:                    //отработка задержки
      if(ClickTimeout())                //задержка отработана и значения можно записывать
      {
        last_in_time=tv;
        state=FB_TS_STEP;
        WriteStates();
      }
      break;
    case FB_TS_TIMEOUT:
      if(ErrorSetTimeout())
      {
        //dump.Print(D_AFB | D_ERR,"%s: предельный таймаут ожидания посылки! Ошибка субблока...",cfg_d.Name());
        dump.ObjMsg(cfg_d.Id(),D_AFB | D_ERR,"%s: предельный таймаут ожидания посылки! Ошибка субблока...",cfg_d.Name());
        SetError(100);    //установка недостоверности
        state=FB_TS_ERROR;
        timeout_begin_time=time(0); 
      }
      else if(StartPollDelay(poll_delay_begin_time))
      {
        //dump.Print(D_AFB | D_MSG,"%s: попытка повторного вызова информации...",cfg_d.Name());
        dump.ObjMsg(cfg_d.Id(),D_AFB | D_MSG,"%s: попытка повторного вызова информации...",cfg_d.Name());
        parent->SetFbTsPollFlag();
        poll_delay_begin_time=time(0); 
      }
      break;
    case FB_TS_ERROR:
      if(StartPollDelay(timeout_begin_time))
      {
        //dump.Print(D_AFB | D_MSG,"%s: попытка повторного вызова информации...",cfg_d.Name());
        dump.ObjMsg(cfg_d.Id(),D_AFB | D_MSG,"%s: попытка повторного вызова информации...",cfg_d.Name());
        parent->SetFbTsPollFlag();    //попытки вызова продолжаю....
        timeout_begin_time=time(0);   //время начала таймаута обновляется
      }
      break;
    case FB_TS_DISABLED:
      break;
    default:
      break;
  }
  return 0;
}
//---------------------------------------------------------------
void FbTs::SetError(unsigned char err)
{
  unsigned char error = cfg_d[KP_AFB_TS.error().CfgFldId()];
  if(error == err)
    return;
  double tm = EventsCash.TimeInDbl();
  cfg_d[KP_AFB_TS.error().CfgFldId()] = err;
  cfg_d[KP_AFB_TS.etime().CfgFldId()] = tm;
  for(unsigned i=0;i<TS_COUNT;i++)
    if(Items[i])
    {
      //if(Items[i]->Enabled())
        Items[i]->SetError(err);
      //else
        //Items[i]->SetError(100);
    }
  cfg_d.SetFldChangeBit(KP_AFB_TS.error().CfgFldId());
  cfg_d.SetFldChangeBit(KP_AFB_TS.etime().CfgFldId());
  EventsCash.Add(cfg_d.Id());
  cfg_d.ClearFldChangeBit(KP_AFB_TS.error().CfgFldId());
  cfg_d.ClearFldChangeBit(KP_AFB_TS.etime().CfgFldId());
}
//---------------------------------------------------------------
unsigned short FbTs::ValidTime()const
{
  return cfg_d[KP_AFB_TS.ValidTime().CfgFldId()];
}
//---------------------------------------------------------------
bool FbTs::Enabled()const
{
  unsigned char en = cfg_d[KP_AFB_TS.Enabled().CfgFldId()];
  return (en!=0);
}
//---------------------------------------------------------------
bool FbTs::ClickTimeout()const
{
  unsigned short valid_time = cfg_d[KP_AFB_TS.ValidTime().CfgFldId()];
  //миллисекунды
  double time_diff = (EventsCash.TimeInDbl()-Tv2Dbl(last_in_time))/1e3;
  return (time_diff >= valid_time);
}
//---------------------------------------------------------------
void FbTs::WriteStates()
{
  int i=0,by,bt;
  int bit_test;
  for(by=0;by<8;by++)//цикл по байтам
  {
    int mask = 0x80;
    if(tmp_data[by]==tmp_data[by+8])//не изменился байт, значит нет изменения соотв. ТС-ов
	  {
	    i+=8;
      continue;
	  }
    for(bt=0;bt<8;mask >>= 1,bt++,i++)//цикл по битам
    {
      if(Items[i])
      {
        if(Items[i]->Enabled())
        {
          Items[i]->SetError(0);//блокировка снята
          bit_test = tmp_data[by]&mask;
          Items[i]->SetState(bit_test!=0);
          //dump.Print(D_AFB | D_MSG,"%s состояние %d",Items[i]->Cfg_D()->Name(),(bit_test==0 ? 0 :1));
          dump.ObjMsg(D_AFB | D_MSG,cfg_d.Id(),"%s состояние %d",Items[i]->Cfg_D()->Name(),(bit_test==0 ? 0 : 1));
        }
        /*else
          Items[i]->SetError(100);//блокировка установлена
          */
      }
    }
    tmp_data[by+8]=tmp_data[by];
  }
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%