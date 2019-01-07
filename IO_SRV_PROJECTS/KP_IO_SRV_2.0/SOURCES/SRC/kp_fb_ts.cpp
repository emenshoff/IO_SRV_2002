/***********************************************************
               created by M.E.N
***********************************************************/

#include <kp_dev.h>
#include <kp_fb_ts.h>
#include <kp_types.h>
#include <time_utils.h>
#include <io_srv_dump.h>
#include <kp_events_cash.h>

const int fb_flags = (D_AFB | D_MSG);
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
FbTs::FbTs(obj_d &d)
:AFB(d)
{
  for(unsigned i=0;i<TS_COUNT;i++)
   Items[i] = 0;
  state=FB_TS_BEGIN;
  dump.ObjMsg(cfg_d.Id(),D_MSG,"Субблок %s state->FB_TS_BEGIN",cfg_d.Name());
  SetError(255);   //в начале была недостоверносtь...
  memset(tmp_data,0,sizeof(tmp_data));
}
//---------------------------------------------------------------
FbTs::~FbTs()
{
  SetError(255);
  for(unsigned i=0;i<TS_COUNT;i++)
    if(Items[i])
      delete Items[i];
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
  last_in_time = EventsCash.TimeInTv();

  int i;
  dump.ObjMsg(cfg_d.Id(),fb_flags,"%s : пришел пакет с данными...",cfg_d.Name());
  if((dump.Flags() & fb_flags) == fb_flags)
  {
    char vl[10];
    string packet = "";
    for(int i=0;i<len;i++)
    {
      if(dt[i]<16)
        sprintf(vl," 0%X",(unsigned char)dt[i]);
      else
        sprintf(vl," %X",(unsigned char)dt[i]);
      packet += vl;
    }
    dump.ObjMsg(cfg_d.Id(),fb_flags," %s ",packet.c_str());
  }
  if((dt[2] >> 4)!=0x2)
  {
    dump.ObjMsg(cfg_d.Id(),D_AFB | D_ERR,"%s : Неверный тип пакета для субблока ТС!",cfg_d.Name());
    return;
  }  
  const char *data=(const char*)dt+3; //настраиваемся сразу на начало данных  
  //сравнение прямого и инверсного кодов ТС
  for(i=0;i<8;i++)
    if((data[i] & data[i+8])!=0)
    {
      dump.ObjMsg(cfg_d.Id(),D_AFB | D_ERR,"%s : Несовпадение прямого и инверсного кодов ТС!",cfg_d.Name());
      SetError(100);
      state=FB_TS_ERROR;
      return;
    } 
  //SetError(0);//6/2/2 //16.04.02
  switch(state)
  {
    case FB_TS_ERROR:case FB_TS_BEGIN:   //в этих случаях значения заносим сразу      
      state=FB_TS_STEP;
      memcpy(tmp_data,data+8,8);  //записываем в буфер данные в прямом коде
      WriteStates();              //пишем текущие значения тс в объекты      
      SetError(0);//6/2/2 //16.04.02
      break;
    //отработка задержки. если в момент отработки задержки происходит переключение, то время и буфер состояний обновляются
    case FB_TS_WAIT:
      if(memcmp(tmp_data,data+8,8) != 0)
        memcpy(tmp_data,data+8,8);
      break;
    case FB_TS_TIMEOUT:      
      state=FB_TS_STEP;
      if(memcmp(tmp_data+8,data+8,8) != 0)
      {
        memcpy(tmp_data,data+8,8);
        WriteStates();        
      }
      SetError(0);//6/2/2 //16.04.02
      break;
    case FB_TS_STEP : 
      if(memcmp(tmp_data+8,data+8,8) != 0)
      {
        memcpy(tmp_data,data+8,8);
        if(!ValidTime())                  //таймаут дребезга не отрабатывается (его значение = 0)
          WriteStates();
        else                              //иначе нужна задержка
          state=FB_TS_WAIT;
      }
      break;
    default:
      break;
  }
}
//---------------------------------------------------------------
int FbTs::Step()
{
  /*if(!Enabled())
  {
    return 0;
  }
  else
  {
   
  }*/
  //dump.ObjMsg(cfg_d.Id(),fb_flags,"%s: Step...",cfg_d.Name());
  struct timeval tv = EventsCash.TimeInTv();
  switch(state)
  {
    case FB_TS_BEGIN:
      state = FB_TS_STEP;
      break;
    case FB_TS_STEP:
      if(ResponceTimeout())
      {
        dump.ObjMsg(cfg_d.Id(),fb_flags,"%s: таймаут (%d сек.) ожидания посылки!",cfg_d.Name(),parent->ResponceTimeoutVal());
        state=FB_TS_TIMEOUT;
        return AFB_POLL;
      }
      break;
    case FB_TS_WAIT:                    //отработка задержки
      dump.ObjMsg(cfg_d.Id(),D_MSG,"Отработка дребезга ...");
      if(ClickTimeout())                //задержка отработана и значения можно записывать
      {
        dump.ObjMsg(cfg_d.Id(),D_MSG,"Отработка дребезга завершена...");
        //last_in_time=tv;
        state=FB_TS_STEP;
        //dump.ObjMsg(cfg_d.Id(),D_MSG,"Субблок %s state->FB_TS_STEP",cfg_d.Name());
        WriteStates();
        SetError(0);
      }
      break;
    case FB_TS_TIMEOUT:
      if(ErrorSetTimeout())
      {
        dump.ObjMsg(cfg_d.Id(),D_AFB | D_ERR,"%s: предельный таймаут ожидания посылки! Ошибка субблока...",cfg_d.Name());
        SetError(100);    //установка недостоверности
        state=FB_TS_ERROR;
      }
      return AFB_POLL;
    case FB_TS_ERROR:
      //SetError(100);    //установка недостоверности
      return AFB_POLL;
    default:
      break;
  }
  return AFB_FREE;
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
      Items[i]->SetError(err);
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
  unsigned char result;
  for(by=0;by<8;by++)//цикл по байтам
  {
    int mask = 0x80;
    /*if(tmp_data[by]==tmp_data[by+8])//не изменился байт, значит нет изменения соотв. ТС-ов
	  {
	    i+=8;
      continue;
	  }//ALN*/
    for(bt=0;bt<8;mask >>= 1,bt++,i++)//цикл по битам
    {
      if(Items[i])
      {
        if(Items[i]->Enabled())
        {
          bit_test = tmp_data[by]&mask;
          result = (bit_test==0 ? 0 : 1);
          Items[i]->SetState(result);
          //dump.Print(fb_flags,"%s состояние %d",Items[i]->Cfg_D()->Name(),(bit_test==0 ? 0 :1));
          dump.ObjMsg(fb_flags,cfg_d.Id(),"%s состояние %d",Items[i]->Cfg_D()->Name(),result);//ALN
        }
      }
    }
    tmp_data[by+8]=tmp_data[by];
  }
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%