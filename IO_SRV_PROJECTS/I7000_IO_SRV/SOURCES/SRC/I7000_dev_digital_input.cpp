/***********************************************************
               created by M.E.N
***********************************************************/

#include <I7000_types.h>
#include <I7000_events_cash.h>
#include <I7000_dev_digital_input.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
I7000_Ts::I7000_Ts(obj_d &d)
:I7000_DigitalInput(d)
{
  SetError(100);
  unsigned char val = cfg_d[I7000_TS.Inverted().CfgFldId()]; 
  SetState(val == 1);
}
//----------------------------------------------------------------
I7000_Ts::~I7000_Ts()
{
  SetError(100);
}
//----------------------------------------------------------------
unsigned I7000_Ts::Num()const
{
  return (unsigned char)cfg_d[I7000_TS.Num().CfgFldId()];
}
//----------------------------------------------------------------
const char *I7000_Ts::Parent()const
{
  return (const char*)cfg_d[I7000_TS.Parent().CfgFldId()]; 
}
//----------------------------------------------------------------
void I7000_Ts::SetError(unsigned char err)
{
  if(err == (unsigned char)cfg_d[I7000_TS.error().CfgFldId()])
    return;
  cfg_d[I7000_TS.error().CfgFldId()] = err;
  cfg_d[I7000_TS.time().CfgFldId()] = EventsCash.TimeInDbl();
  cfg_d.SetFldChangeBit(I7000_TS.error().CfgFldId());
  cfg_d.SetFldChangeBit(I7000_TS.time().CfgFldId());
  EventsCash.Add(cfg_d.Id());
  cfg_d.ClearFldChangeBit(I7000_TS.error().CfgFldId());
  cfg_d.ClearFldChangeBit(I7000_TS.time().CfgFldId());
}
//----------------------------------------------------------------
void I7000_Ts::SetState(bool on)
{
  unsigned char tp,enabled,val;
  enabled = *(unsigned char*)cfg_d.GetFldValue(I7000_TS.Enabled().CfgFldId(),tp);  
  if(!enabled)
  {
    if((unsigned char)cfg_d[I7000_TS.error().CfgFldId()] == 0)
      SetError(100);
    return;  
  }
  double tm = EventsCash.TimeInDbl();
  /*val = *(unsigned char*)cfg_d.GetFldValue(I7000_TS.value().CfgFldId(),tp);
  if(val==(unsigned char)on)
    return;*/
  val = *(unsigned char*)cfg_d.GetFldValue(I7000_TS.Inverted().CfgFldId(),tp);
  val ^= (unsigned char)on;
  cfg_d.SetFldValue(I7000_TS.value().CfgFldId(),(void*)&val,tm_byte);
  cfg_d.SetFldValue(I7000_TS.time().CfgFldId(),(void*)&tm,tm_time);
  cfg_d.SetFldChangeBit(I7000_TS.value().CfgFldId());
  cfg_d.SetFldChangeBit(I7000_TS.time().CfgFldId());
  EventsCash.Add(cfg_d.Id());
  cfg_d.ClearFldChangeBit(I7000_TS.time().CfgFldId());
  cfg_d.ClearFldChangeBit(I7000_TS.value().CfgFldId());
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%