/***********************************************************
               created by M.E.N
***********************************************************/

#include <kp_fb_tii.h>
#include <kp_events_cash.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
FbTii::FbTii(obj_d &d)
 :AFB(d)
{
  for(unsigned i=0;i<32;i++)
   Items.push_back(NULL);
  SetError(255);
}
//---------------------------------------------------------------
FbTii::~FbTii()
{
  SetError(255);
  vector<TIICtl *>::iterator tii_i;
  for(tii_i=Items.begin();tii_i!=Items.end();tii_i++)
    if(*tii_i)
      delete (*tii_i);
  Items.clear();
}
//---------------------------------------------------------------
unsigned char FbTii::Place()const
{
  unsigned char tp;
  return *(unsigned char*)cfg_d.GetFldValue(KP_AFB_TII.Place().CfgFldId(),tp);
}
//---------------------------------------------------------------
const char *FbTii::Parent()const
{
  unsigned char tp;
  return (const char *)cfg_d.GetFldValue(KP_AFB_TII.Parent().CfgFldId(),tp);
}
//---------------------------------------------------------------
void FbTii::SetError(unsigned char err)
{ 
  double tm = EventsCash.TimeInDbl();
  vector<TIICtl *>::iterator tii_i;
  cfg_d.SetFldValue(KP_AFB_TII.error().CfgFldId(),(void*)&err,KP_AFB_TII.error().Type());
  cfg_d.SetFldValue(KP_AFB_TII.etime().CfgFldId(),(void*)&tm,KP_AFB_TII.etime().Type());
  for(tii_i=Items.begin();tii_i!=Items.end();tii_i++)
    if(*tii_i)
      (*tii_i)->SetError(err);
  cfg_d.SetFldChangeBit(KP_AFB_TII.error().CfgFldId());
  cfg_d.SetFldChangeBit(KP_AFB_TII.etime().CfgFldId());
  EventsCash.Add(cfg_d.Id());
  cfg_d.ClearFldChangeBit(KP_AFB_TII.error().CfgFldId());
  cfg_d.ClearFldChangeBit(KP_AFB_TII.etime().CfgFldId());
}
//---------------------------------------------------------------
bool FbTii::Enabled()const
{
  unsigned char tp;  
  return (*(unsigned char*)cfg_d.GetFldValue(KP_AFB_TII.Enabled().CfgFldId(),tp)!=0);
}
//---------------------------------------------------------------
void FbTii::ProcPacket(const unsigned char *,unsigned len)
{
}
//---------------------------------------------------------------
int FbTii::Step()
{
  return AFB_FREE;
}
//----------------------------------------------------------------
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%