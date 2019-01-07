/***********************************************************
               created by M.E.N
***********************************************************/

#include <kp_fb_tit.h>
#include <kp_dev.h>
#include <kp_types.h>
#include <io_srv_dump.h>
#include <kp_events_cash.h>

const int fb_flags = (D_AFB | D_MSG);
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
FbTit::FbTit(obj_d &d)
  :AFB(d)
{
  for(unsigned i=0;i<TIT_COUNT;i++)
    Items[i] = 0;
  SetError(255);
  memset(tmp_data,0,sizeof(tmp_data));
  state=FB_TIT_BEGIN;
}
//---------------------------------------------------------------
FbTit::~FbTit()
{
  SetError(255);
  for(unsigned i=0;i<TIT_COUNT;i++)
    if(Items[i])
      delete Items[i];
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
  GetCurrTime(last_in_time);        //фиксируем время прихода посылки  
/*  if(!Enabled())
  {

    return;
  }
  else
  {

  }
  */
  dump.ObjMsg(cfg_d.Id(),fb_flags,"%s : пришел пакет с данными...",cfg_d.Name());
  //дамп пакета
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
  unsigned char info_type=(dt[2] >> 4);
  if((info_type !=0x06) && (info_type != 0x07) && (info_type != 0x03))
  {
    dump.ObjMsg(cfg_d.Id(),D_AFB | D_ERR,"%s : Неверный тип пакета для субблока ТИТ!",cfg_d.Name());
    return;
  }
  KPHeader *kph = (KPHeader*)dt;
  const unsigned char *data=dt+3;
  //SetError(0);//6.2.2//14.04.02
  switch(state)
  {
    case FB_TIT_STEP://14.04.02
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
      break;
      //14.04.02
    case FB_TIT_TIMEOUT: case FB_TIT_ERROR:case FB_TIT_BEGIN: //возможен и такой случай
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
      SetError(0);//6/2/2//14.04.02
      state=FB_TIT_STEP;
      break;
    default:
      return;
  }  
}
//---------------------------------------------------------------
int FbTit::Step()
{
  struct timeval tv;
  GetCurrTime(tv);
  switch(state)
  {
    case FB_TIT_BEGIN:
      state = FB_TIT_STEP;
      break;
    case FB_TIT_STEP:
      if(ResponceTimeout())
      {
        dump.ObjMsg(cfg_d.Id(),fb_flags,"%s: таймаут (%d сек.) ожидания посылки!",cfg_d.Name(),parent->ResponceTimeoutVal());
        state=FB_TIT_TIMEOUT;  
        return AFB_POLL;
      }
      break;
    case FB_TIT_TIMEOUT:
      if(ErrorSetTimeout())
      {
        dump.ObjMsg(cfg_d.Id(),D_AFB | D_ERR,"%s: предельный таймаут ожидания посылки! Ошибка субблока...",cfg_d.Name());
        SetError(100);    //установка недостоверности
        state=FB_TIT_ERROR;
      }
      return AFB_POLL;
    case FB_TIT_ERROR:
      //SetError(100);    //установка недостоверности
      return AFB_POLL;
    default:
      break;
  }
  return AFB_FREE;
}
//---------------------------------------------------------------
void FbTit::WriteStates(int grp)
{
  int i=(grp==0 ? 0 : 16),h=(grp==0 ? 16 : 32);
  for(;i<h;i++)
    //if(tmp_data[i]!=tmp_data[i+32]) //ALN
    {
      //dump.ObjMsg(cfg_d.Id(),fb_flags," %s: значение ТИТ %u = %u",cfg_d.Name(),i+1,tmp_data[i]); //ALN
      if(Items[i])
      {
        if(Items[i]->Enabled())
        {          
          Items[i]->SetValue(tmp_data[i]);
          //dump.Print(fb_flags," %s: значение %u...",Items[i]->Cfg_D()->Name(),tmp_data[i]);
          //dump.ObjMsg(cfg_d.Id(),fb_flags," %s: значение %u...",Items[i]->Cfg_D()->Name(),tmp_data[i]); //ALN
        }
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
      Items[i]->SetError(err);
  cfg_d.SetFldChangeBit(KP_AFB_TIT.error().CfgFldId());
  cfg_d.SetFldChangeBit(KP_AFB_TIT.etime().CfgFldId());
  EventsCash.Add(cfg_d.Id());
  cfg_d.ClearFldChangeBit(KP_AFB_TIT.error().CfgFldId());
  cfg_d.ClearFldChangeBit(KP_AFB_TIT.etime().CfgFldId());
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%