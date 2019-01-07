/***********************************************************
               created by M.E.N
***********************************************************/

#include <kp_dev.h>
#include <kp_types.h>
#include <kp_fb_tu.h>
#include <io_srv_dump.h>
#include <kp_events_cash.h>

const int fb_flags = (D_AFB | D_MSG);
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
FbTu::FbTu(obj_d &d)
  :AFB(d)
{  
 unsigned char tp;
 for(unsigned i=0;i<TU_COUNT;i++)
   Items[i] = 0;
  SetError(255);
  state=FB_TU_BEGIN;
  num=*(unsigned char*)cfg_d.GetFldValue(KP_AFB_TU.Num().CfgFldId(),tp);
}
//---------------------------------------------------------------
FbTu::~FbTu()
{
  Shutdown();
  SetError(255);
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
  return cfg_d[KP_AFB_TU.Place().CfgFldId()];
}
//---------------------------------------------------------------
const char *FbTu::Parent()const
{
  return cfg_d[KP_AFB_TU.Parent().CfgFldId()];
}
//---------------------------------------------------------------
void FbTu::ProcPacket(const unsigned char *pkt,unsigned len)
{  
  if((dump.Flags() & fb_flags) == fb_flags)
  {
    char vl[10];
    string packet = "";
    for(int i=0;i<len;i++)
    {
      if(pkt[i]<16)
        sprintf(vl," 0%X",(unsigned char)pkt[i]);
      else
        sprintf(vl," %X",(unsigned char)pkt[i]);
      packet += vl;
    }
    dump.ObjMsg(cfg_d.Id(),fb_flags," %s ",packet.c_str());
  }
  if(len<6)
    return;
  if((pkt[5] >> 8)!= 0x8)
  {
    dump.ObjMsg(fb_flags,cfg_d.Id(),"%s : недопустимый тип пакета для субблока ТУ!",cfg_d.Name());
    return;
  }
  else
  {
    dump.ObjMsg(fb_flags,cfg_d.Id(),"%s : пакет \"субблок ТУ СВОБОДЕН\"...",cfg_d.Name());
    SetError(0);
  }  
  switch(state)
  {
    case FB_TU_TU: case FB_TU_BEGIN:      
      state=FB_TU_STEP;
      break;
    case FB_TU_ERROR:
      state=FB_TU_STEP;
      break;
  }
  last_in_time=EventsCash.TimeInTv();
}
//---------------------------------------------------------------
int FbTu::Step()
{
  if(!Enabled())
    return 0;
  struct timeval tv;
  GetCurrTime(tv);
  switch(state)
  {
    case FB_TU_BEGIN:
      state=FB_TU_STEP;
      break;
    case FB_TU_TU:
      if(ResponceTimeout())
      {
        dump.ObjMsg(fb_flags,cfg_d.Id(),"%s: таймаут (%d сек.) ожидания посылки!",cfg_d.Name(),parent->ResponceTimeoutVal());
        state=FB_TU_TIMEOUT;
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
struct TU_COMMANDS
{
  unsigned char  command;   // код команды
  unsigned char inv_command;// инверсный позиционный код команды
};
//---------------------------------------------------------------
static TU_COMMANDS cmd[] =
{
  { 1, 0x7f }, // tu1 on
  { 2, 0xbf }, // tu1 off
  { 3, 0xdf }, // tu2 on
  { 4, 0xef }, // tu2 off
  { 7, 0xfd }, // tu3 on
  { 0, 0xff }  // tu3 off
};
//---------------------------------------------------------------
void FbTu::SendTU(unsigned char on,unsigned char grp,unsigned char obj)
{
  unsigned char data[10];
  unsigned char n = (num - 1) * 2; if (!on) n++;
  unsigned short grp_code = 0x8000 >> (grp-1); //прямой позиц. код группы
  //адрес кп:
  data[0]=parent->Address();
  //прямой код команды
  data[1]=cmd[n].command | 0x10;
  data[2]= *(((unsigned char*)&grp_code)+1);
  data[3]= *(((unsigned char*)&grp_code));
  data[4]=0x80 >> (obj-1);                     //прямой позиц. код объекта
  data[5]=cmd[n].inv_command;              //инверсный поз. код команды
  data[6]=0xff;
  grp_code = ~grp_code;                        //инверсный код группы  
  data[7]=*(((unsigned char*)&grp_code)+1);
  data[8]=*(((unsigned char*)&grp_code));
  data[9]=~data[4];                            //инверсный код объекта
  if(parent->cnl->CanSend())//млин, заглушка такая....
  {
    parent->cnl->Send(data,10);
    //все ответы субблока не фиксируются. таймаут считаю от последнего посланного ТУ
    GetCurrTime(last_in_time);
    state=FB_TU_TU;// :)
  }  
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%