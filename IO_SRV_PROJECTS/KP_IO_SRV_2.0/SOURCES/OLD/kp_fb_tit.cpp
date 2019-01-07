/***********************************************************
               created by M.E.N
***********************************************************/

#include <kp_fb_tit.h>
#include <kp_dev.h>
#include <kp_types.h>
#include <io_srv_dump.h>
#include <kp_events_cash.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
FbTit::FbTit(obj_d &d)
  :AFB(d)
{
  for(unsigned i=0;i<TIT_COUNT;i++)
    Items[i] = 0;
  SetError(100);
  memset(tmp_data,0,sizeof(tmp_data));
  timeout_begin_time=time(0);
  state=FB_TIT_ERROR;
}
//---------------------------------------------------------------
FbTit::~FbTit()
{
  SetError(100);
  for(unsigned i=0;i<TIT_COUNT;i++)
    if(Items[i])
      delete Items[i];
}
//---------------------------------------------------------------
void FbTit::Enable()
{
  if(Enabled())
  {
    /*SetError(0);
    state = FB_TS_ERROR;*/
  }  
}
//---------------------------------------------------------------
void FbTit::Disable()
{
  /*SetError(100);
  state = FB_TS_DISABLED;*/
}
//---------------------------------------------------------------
unsigned char FbTit::Error()const
{
  return cfg_d[KP_AFB_TIT.error().CfgFldId()];
}
//---------------------------------------------------------------
unsigned char FbTit::Place()const
{
  return cfg_d[KP_AFB_TIT.Place().CfgFldId()];
}
//---------------------------------------------------------------
const char *FbTit::Parent()const
{
  return (const char *)cfg_d[KP_AFB_TIT.Parent().CfgFldId()];
}
//---------------------------------------------------------------
bool FbTit::Enabled()const
{
  unsigned char en = cfg_d[KP_AFB_TIT.Enabled().CfgFldId()];
  return (en!=0);
}
//---------------------------------------------------------------
void FbTit::ProcPacket(const unsigned char *dt,unsigned len)
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
    Enable();
  }
  unsigned char info_type=(dt[2] >> 4);
  if((info_type !=0x06) && (info_type != 0x07) && (info_type != 0x03))
  {
    //dump.Print(D_AFB | D_ERR,"%s : Неверный тип пакета для субблока ТИТ!",cfg_d.Name());
    dump.ObjMsg(cfg_d.Id(),D_AFB | D_ERR,"%s : Неверный тип пакета для субблока ТИТ!",cfg_d.Name());
    return;
  }
  KPHeader *kph = (KPHeader*)dt;
  const unsigned char *data=dt+3;
  GetCurrTime(last_in_time);        //фиксируем время прихода посылки  
  switch(state)
  {
    case FB_TIT_STEP:
      break;
    case FB_TIT_TIMEOUT: case FB_TIT_ERROR:    //возможен и такой случай
      SetError(0);
      state=FB_TIT_STEP;
      break;
    case FB_TIT_DISABLED:
      return;
    default:
      return;
  }
  if(!(kph->ngr & 1)) //первый пакет
  {
    memcpy(tmp_data,data,16);
    WriteStates(0);
  }
  else                //второй пакет
  {
    memcpy(tmp_data + 16,data,16);
    WriteStates(1);
  }
}
//---------------------------------------------------------------
char FbTit::Step()
{
  if(!Enabled())
  {
    Disable();
    return 0;
  }
  else
    Enable();
  struct timeval tv;
  GetCurrTime(tv);
  switch(state)
  {
    case FB_TIT_STEP:
      if(ResponceTimeout())
      {
        //dump.Print(D_AFB | D_MSG,"%s: таймаут (%d сек.) ожидания посылки!",cfg_d.Name(),ResponceTimeoutVal());
        dump.ObjMsg(cfg_d.Id(),D_AFB | D_MSG,"%s: таймаут (%d сек.) ожидания посылки!",cfg_d.Name(),ResponceTimeoutVal());
        state=FB_TIT_TIMEOUT;
        timeout_begin_time=poll_delay_begin_time=time(0);
      }
      break;
    case FB_TIT_TIMEOUT:
      if(ErrorSetTimeout())
      {
        //dump.Print(D_AFB | D_ERR,"%s: предельный таймаут ожидания посылки! Ошибка субблока...",cfg_d.Name());
        dump.ObjMsg(cfg_d.Id(),D_AFB | D_ERR,"%s: предельный таймаут ожидания посылки! Ошибка субблока...",cfg_d.Name());
        SetError(100);    //установка недостоверности
        state=FB_TIT_ERROR;
      }
      if(StartPollDelay(poll_delay_begin_time))
      {
        //dump.Print(D_AFB | D_MSG,"%s: попытка повторного вызова информации...",cfg_d.Name());
        dump.ObjMsg(cfg_d.Id(),D_AFB | D_MSG,"%s: попытка повторного вызова информации...",cfg_d.Name());
        parent->SetFbTitPollFlag();
        poll_delay_begin_time=time(0);
      }
      break;
    case FB_TIT_ERROR:
      if(StartPollDelay(timeout_begin_time))
      {
        //dump.Print(D_AFB | D_MSG,"%s: попытка повторного вызова информации...",cfg_d.Name());
        dump.ObjMsg(cfg_d.Id(),D_AFB | D_MSG,"%s: попытка повторного вызова информации...",cfg_d.Name());
        parent->SetFbTitPollFlag();    //попытки вызова продолжаю....
        timeout_begin_time=time(0);
      }
      break;
    case FB_TIT_DISABLED:
      break;
    default:
      break;
  }
  return 0;
}
//---------------------------------------------------------------
void FbTit::WriteStates(int grp)
{
  int i=(grp==0 ? 0 : 16),h=(grp==0 ? 16 : 32);
  for(;i<h;i++)
    if(tmp_data[i]!=tmp_data[i+32])
    {
      if(Items[i])
      {
        if(Items[i]->Enabled())
        {
          Items[i]->SetError(0);
          Items[i]->SetValue(tmp_data[i]);
          //dump.Print(D_AFB | D_MSG," %s: значение %u...",Items[i]->Cfg_D()->Name(),tmp_data[i]);
          dump.ObjMsg(cfg_d.Id(),D_AFB | D_MSG," %s: значение %u...",Items[i]->Cfg_D()->Name(),tmp_data[i]);
        }
        /*else
          Items[i]->SetError(100);*/
      }
      tmp_data[i+32]=tmp_data[i];
    }
}
//---------------------------------------------------------------
void FbTit::SetError(unsigned char err)
{
  unsigned char error = cfg_d[KP_AFB_TIT.error().CfgFldId()];
  if(error == err)
    return;
  double tm = EventsCash.TimeInDbl();
  cfg_d[KP_AFB_TIT.error().CfgFldId()] = err;
  cfg_d[KP_AFB_TIT.etime().CfgFldId()] = tm;
  for(unsigned i=0;i<TIT_COUNT;i++)
    if(Items[i])
    {
      //if(Items[i]->Enabled())
        Items[i]->SetError(err);
      //else
        //Items[i]->SetError(100);
    }
  cfg_d.SetFldChangeBit(KP_AFB_TIT.error().CfgFldId());
  cfg_d.SetFldChangeBit(KP_AFB_TIT.etime().CfgFldId());
  EventsCash.Add(cfg_d.Id());
  cfg_d.ClearFldChangeBit(KP_AFB_TIT.error().CfgFldId());
  cfg_d.ClearFldChangeBit(KP_AFB_TIT.etime().CfgFldId());
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%