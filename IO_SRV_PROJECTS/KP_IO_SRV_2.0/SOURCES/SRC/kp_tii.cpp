/***********************************************************
               created by M.E.N
***********************************************************/

#include <kp_tii.h>
#include <io_srv_dump.h>
#include <kp_events_cash.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
TIICtl::TIICtl(obj_d &d)
  :ControlObject(d)
{
}
//---------------------------------------------------------------
const char *TIICtl::Parent()const
{
  unsigned char tp;
  return (const char *)cfg_d.GetFldValue(KP_TII.Parent().CfgFldId(),tp);
}
//---------------------------------------------------------------
unsigned char TIICtl::Num()const
{
  unsigned char tp;
  return *(unsigned char*)cfg_d.GetFldValue(KP_TII.Num().CfgFldId(),tp);
}
//---------------------------------------------------------------
void TIICtl::SetError(unsigned char err)
{
  double tm = EventsCash.TimeInDbl();
  cfg_d[KP_TII.error().CfgFldId()] = err;
  cfg_d[KP_TII.time().CfgFldId()] = tm;
  cfg_d.SetFldChangeBit(KP_TII.error().CfgFldId());
  cfg_d.SetFldChangeBit(KP_TII.time().CfgFldId());
  EventsCash.Add(cfg_d.Id());
  cfg_d.ClearFldChangeBit(KP_TII.error().CfgFldId());
  cfg_d.ClearFldChangeBit(KP_TII.time().CfgFldId());
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%