/***********************************************************
               created by M.E.N
***********************************************************/

#include <kpe_tit.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
KpeTit::KpeTit(obj_d &d)
:ControlObject(d)
{
}
//----------------------------------------------------------------
unsigned KpeTit::Type()const
{
  return TITE;
}
//----------------------------------------------------------------
unsigned char KpeTit::Value()const
{
  unsigned char tp;
  return *(unsigned char*)cfg_d.GetFldValue(KPE_TIT.Value().CfgFldId(),tp);
}
//----------------------------------------------------------------
bool KpeTit::Enabled()const
{
  unsigned char tp;
  return *(unsigned char*)cfg_d.GetFldValue(KPE_TIT.Enabled().CfgFldId(),tp);
}
//----------------------------------------------------------------
unsigned char KpeTit::Num()const
{
  return (unsigned char)cfg_d[KPE_TIT.Num().CfgFldId()];
}
//----------------------------------------------------------------
const char *KpeTit::Parent()const
{
  return (const char*)cfg_d[KPE_TIT.Parent().CfgFldId()];
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%