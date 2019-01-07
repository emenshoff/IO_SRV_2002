#include <kp_cdv.h>
#include <kp_types.h>
#include <kp_channel.h>
#include <kp_io_srv.h>
#include <kp_dev.h>
#include <kp_header.h>
#include <kp_events_cash.h>
#include <io_srv_dump.h>

const int cnl_flags = (D_CNL | D_MSG);
//-----------------------------------------------------------------
KpCnl::KpCnl(obj_d &obj)
:ControlObject(obj),s_flag(0)
{
  cnl_diagnostic_info curr_info;
  curr_info.err_cnt=0;
  curr_info.error=255;
  curr_info.nonkvit_cnt=0;
  curr_info.recv_cnt=0;
  curr_info.snd_cnt=0;
  SetDiagnosticInfo(curr_info);
  for(int i=0;i<128;i++)
    Items[i]=0;
  last_in_time = time(0);
  num = Num();
  InKvit();
}
//-----------------------------------------------------------------
KpCnl::~KpCnl()
{
  for(int i=0;i<128;i++)
    if(Items[i])
      delete Items[i];
}
//-----------------------------------------------------------------
bool KpCnl::CanSend()const
{
  if(kvit_is_recived)
    return true;
  else
    if(EventsCash.TimeInDbl() - data_send_time > 60000)
      return true;
  return false;
}
//-----------------------------------------------------------------
void KpCnl::Send(const unsigned char *data,unsigned len)
{
  /*if((dump.Flags() & cnl_flags) == cnl_flags)
  {
    char vl[10];
    string packet = "";
    dump.ObjMsg(cfg_d.Id(),cnl_flags,"Отправка в канал %s:",cfg_d.Name());
    for(int i=0;i<len;i++)
    {
      sprintf(vl," %X",(unsigned char)data[i]);
      packet += vl;
    }
    dump.ObjMsg(cfg_d.Id(),cnl_flags," %s ",packet.c_str());
  }*/
  if(CanSend())
  {
    parent->Send(data,len,num);
    OutData();
    data_send_time = EventsCash.TimeInDbl();
  }
  //kvit_is_recived = false;//5.11.01
}
//-----------------------------------------------------------------
unsigned short KpCnl::KpSpeed()const
{
  return cfg_d[KP_CHANNEL.KpSpeed().CfgFldId()];
}
//-----------------------------------------------------------------
unsigned char KpCnl::Num()const
{
  return cfg_d[KP_CHANNEL.Number().CfgFldId()];
}
//-----------------------------------------------------------------
void KpCnl::SetDiagnosticInfo(cnl_diagnostic_info &diag)
{  
  /*if(memcmp(&curr_info,&diag,sizeof(cnl_diagnostic_info))==0)
    return;
  memcpy(&curr_info,&diag,sizeof(cnl_diagnostic_info));*/
  cfg_d[KP_CHANNEL.error().CfgFldId()] = diag.error;
  cfg_d[KP_CHANNEL.recived_count().CfgFldId()] = diag.recv_cnt;
  cfg_d[KP_CHANNEL.sended_count().CfgFldId()] = diag.snd_cnt;
  cfg_d[KP_CHANNEL.transfer_err_count().CfgFldId()] = diag.err_cnt;
  cfg_d[KP_CHANNEL.nonkvit_count().CfgFldId()] = diag.nonkvit_cnt;
  cfg_d.SetFldChangeBit(KP_CHANNEL.error().CfgFldId());
  cfg_d.SetFldChangeBit(KP_CHANNEL.recived_count().CfgFldId());
  cfg_d.SetFldChangeBit(KP_CHANNEL.sended_count().CfgFldId());
  cfg_d.SetFldChangeBit(KP_CHANNEL.transfer_err_count().CfgFldId());
  cfg_d.SetFldChangeBit(KP_CHANNEL.nonkvit_count().CfgFldId());
  EventsCash.Add(cfg_d.Id());
  cfg_d.ClearFldChangeBit(KP_CHANNEL.error().CfgFldId());
  cfg_d.ClearFldChangeBit(KP_CHANNEL.recived_count().CfgFldId());
  cfg_d.ClearFldChangeBit(KP_CHANNEL.sended_count().CfgFldId());
  cfg_d.ClearFldChangeBit(KP_CHANNEL.transfer_err_count().CfgFldId());
  cfg_d.ClearFldChangeBit(KP_CHANNEL.nonkvit_count().CfgFldId());
}
//-----------------------------------------------------------------
const char *KpCnl::CommDev()const
{
  return (const char *)cfg_d[KP_CHANNEL.CommDev().CfgFldId()];
}
//-----------------------------------------------------------------
void KpCnl::Step()
{
  for(int i=0;i<128;i++)
    if(Items[i])
      Items[i]->Step();
}
//-----------------------------------------------------------------
void KpCnl::Process(const unsigned char *data,unsigned size)
{
  KPHeader *head=(KPHeader*)&data[5];  
  if(Items[head->nkp])  
  {
    Items[head->nkp]->ProcPacket(data+5,size-5);
    Items[head->nkp]->Step();
    //dump.Print(D_MSG | D_CNL,"канал %s(%u): Пакет от КП(%u) субблока(%u)...",cfg_d.Name(),Num(),head->nkp,head->afb);
    dump.ObjMsg(cfg_d.Id(),D_MSG | D_CNL,"Канал %s(%u): Пакет от КП(%u) субблока(%u)...",cfg_d.Name(),Num(),head->nkp,head->afb);
  }
}
//-----------------------------------------------------------------