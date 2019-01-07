/***********************************************************
               created by M.E.N
***********************************************************/

#include <kp_dev.h>
#include <kp_types.h>
#include <kp_fb_tu.h>
#include <io_srv_dump.h>
#include <kp_events_cash.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
FbTu::FbTu(obj_d &d)
  :AFB(d)
{  
 unsigned char tp;
 for(unsigned i=0;i<TU_COUNT;i++)
   Items[i] = 0;
  SetError(100);
  state=FB_TU_BEGIN;
  num=*(unsigned char*)cfg_d.GetFldValue(KP_AFB_TU.Num().CfgFldId(),tp);
}
//---------------------------------------------------------------
FbTu::~FbTu()
{
  Shutdown();
  SetError(100);
  for(unsigned i=0;i<TU_COUNT;i++)
    if(Items[i])
      delete Items[i];
}
//---------------------------------------------------------------
unsigned char FbTu::Error()const
{
  return cfg_d[KP_AFB_TU.error().CfgFldId()];
}
//---------------------------------------------------------------
unsigned char FbTu::Place()const
{
  unsigned char tp;
  return *(unsigned char*)cfg_d.GetFldValue(KP_AFB_TU.Place().CfgFldId(),tp);
}
//---------------------------------------------------------------
const char *FbTu::Parent()const
{
  unsigned char tp;
  return (const char *)cfg_d.GetFldValue(KP_AFB_TU.Parent().CfgFldId(),tp);
}
//---------------------------------------------------------------
void FbTu::ProcPacket(const unsigned char *pkt,unsigned len)
{
  if(len<6)
    return;
  if((pkt[5] >> 8)!= 0x8)
  {
    dump.ObjMsg(D_AFB | D_MSG,cfg_d.Id(),"%s : недопустимый тип пакета для субблока ТУ!",cfg_d.Name());
    return;
  }
  unsigned char err = cfg_d[KP_AFB_TU.error().CfgFldId()];
  if(err != 0)
    SetError(0);
  switch(state)
  {
    case FB_TU_TU: case FB_TU_BEGIN:
      SetError(0);
      state=FB_TU_STEP;
      break;
    case FB_TU_ERROR:
      SetError(0);
      state=FB_TU_STEP;
      break;
  }
  last_in_time=EventsCash.TimeInTv();
}
//---------------------------------------------------------------
char FbTu::Step()
{
  if(!Enabled())
    return 0;
  struct timeval tv;
  GetCurrTime(tv);
  switch(state)
  {
    case FB_TU_TU:
      if(ResponceTimeout())
      {
        dump.ObjMsg(D_AFB | D_MSG,cfg_d.Id(),"%s: таймаут (%d сек.) ожидания посылки!",cfg_d.Name(),ResponceTimeoutVal());
        state=FB_TU_TIMEOUT;
        timeout_begin_time=time(0);
      }
      break;
    case FB_TU_TIMEOUT:
      if(ErrorSetTimeout())
      {
        dump.ObjMsg(D_AFB | D_ERR,cfg_d.Id(),"%s: предельный таймаут ожидания посылки! Ошибка субблока...",cfg_d.Name());
        state=FB_TU_ERROR;        
        SetError(100);
      }
      break;
    case FB_TU_ERROR:         //продолжаем попытки
      state=FB_TU_STEP;
      ItemsStep();
      break;
    case FB_TU_STEP:
      ItemsStep();
      break;
    default:
      break;
  }
  return 0;
}
//---------------------------------------------------------------
void FbTu::SetError(unsigned char err)
{
  unsigned char error = cfg_d[KP_AFB_TU.error().CfgFldId()];
  if(error==err)
    return;
  double tm = EventsCash.TimeInDbl();
  cfg_d[KP_AFB_TU.error().CfgFldId()] = err;
  cfg_d[KP_AFB_TU.etime().CfgFldId()] = tm;  
  for(unsigned i=0;i<TU_COUNT;i++)
    if(Items[i])
      Items[i]->SetError(err);
  cfg_d.SetFldChangeBit(KP_AFB_TU.error().CfgFldId());
  cfg_d.SetFldChangeBit(KP_AFB_TU.etime().CfgFldId());
  EventsCash.Add(cfg_d.Id());
  cfg_d.ClearFldChangeBit(KP_AFB_TU.error().CfgFldId());
  cfg_d.ClearFldChangeBit(KP_AFB_TU.etime().CfgFldId());
} 
//---------------------------------------------------------------
bool FbTu::Enabled()const
{
  unsigned char tp;
  return (*(unsigned char*)cfg_d.GetFldValue(KP_AFB_TU.Enabled().CfgFldId(),tp)!=0);
}
//---------------------------------------------------------------
void FbTu::Shutdown()
{
  for(unsigned i=0;i<TU_COUNT;i++)
    if(Items[i])
      while(Items[i]->Step()!=TUCtl::TU_STEP)
      {
      }
      //все команды телеуправления, выполняющиеся в данный момент, должны быть завершены! 
      //если в этот момент попытаться послать ТУ, то оно не пройдет...
}
//---------------------------------------------------------------
void FbTu::ItemsStep()
{
  for(unsigned i=0;i<TU_COUNT;i++)
    if(Items[i])
      switch(Items[i]->Step())
      {
        case TUCtl::TU_STEP:
          break;
        case TUCtl::TU_ON:
          SendTU(1,Items[i]->GroupNum(),Items[i]->Num());
          break;
        case TUCtl::TU_OFF:
          SendTU(0,Items[i]->GroupNum(),Items[i]->Num());
          break;
        default:
          break;
      }
}
//---------------------------------------------------------------
void FbTu::SendTU(unsigned char on,unsigned char grp,unsigned char obj)
{
  unsigned char data[10];
  data[0]=parent->Address();
  data[1]=(num==1 ? (on ? 1 : 2) : (on ? 3 :4)) | 0x10; //номер блока: 1 или 2
  *(unsigned short *)&data[2]= 0x8000 >> (grp-1); //прямой код группы
  data[4]=0x80 >> (obj-1);                        //прямой код объекта
  *(unsigned short *)&data[7]=~*(unsigned short *)&data[2]; //инверсный поз. код афб
  *(unsigned short *)&data[5]=~ (0x8000 >> Place());        //инверсный код группы
  data[9]=~data[4];                                         //инверсный код объекта
  parent->Send(data,10);
  //все ответы субблока не фиксируются. таймаут считаю от последнего посланного ТУ
  GetCurrTime(last_in_time);
  state=FB_TU_TU;// :)
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%