/***********************************************************
               created by M.E.N
***********************************************************/

#include <kpe_ts.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
KpeTs::KpeTs(obj_d &d)
:ControlObject(d)
{
}
//----------------------------------------------------------------
unsigned KpeTs::Type()const
{
  return TSE;
}
//----------------------------------------------------------------
unsigned char KpeTs::Value()const
{
  unsigned char tp;
  return *(unsigned char*)cfg_d.GetFldValue(KPE_TS.Value().CfgFldId(),tp);
}
//----------------------------------------------------------------
bool KpeTs::Enabled()const
{
  unsigned char tp;
  return (*(unsigned char*)cfg_d.GetFldValue(KPE_TS.Enabled().CfgFldId(),tp)!=0);
}
//----------------------------------------------------------------
unsigned char KpeTs::Num()const
{
  return (unsigned char)cfg_d[KPE_TS.Num().CfgFldId()];  
}
//----------------------------------------------------------------
const char *KpeTs::Parent()const
{
  return (const char*)cfg_d[KPE_TS.Parent().CfgFldId()];
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%