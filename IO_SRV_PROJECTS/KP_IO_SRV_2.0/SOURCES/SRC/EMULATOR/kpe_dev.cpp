/***********************************************************
               created by M.E.N
***********************************************************/

#include <kpe_dev.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
KpeDev::KpeDev(obj_d &d)
:KpAbstractDev(d)
{
  port = 0;
  curr_item = 0;
  last_out_time = time(0);
  for(int i=0;i<16;i++)
    Items[i]= 0;
  addr = (unsigned char)cfg_d[KPE_DEV.Address().CfgFldId()];
}
//----------------------------------------------------------------
KpeDev::~KpeDev()
{
  for(int i=0;i<16;i++)
    if(Items[i])
      delete Items[i];
}
//----------------------------------------------------------------
void KpeDev::ProcPacket(const unsigned char *pk,unsigned len)
{
}
//----------------------------------------------------------------
void KpeDev::Step()
{
  int len;
  long curr_time = time(0);
  if(curr_time - last_out_time >Delay())
    for(;Items[curr_item] && curr_item<16;curr_item++)
    {
      const unsigned char *data = Items[curr_item]->Step(len);
      if(len)
      {
        last_out_time = curr_time;
        port->Send(data,len,channel);
        if(++curr_item >=16)
          curr_item = 0;
        return;
      }
    }
}
//----------------------------------------------------------------
bool KpeDev::Enabled()const
{
  unsigned char tp;
  return *(unsigned char*)cfg_d.GetFldValue(KPE_DEV.Enabled().CfgFldId(),tp);
}
//----------------------------------------------------------------
unsigned short KpeDev::Delay()const
{
  unsigned char tp;
  return *(unsigned short*)cfg_d.GetFldValue(KPE_DEV.SendDelay().CfgFldId(),tp);
}
//----------------------------------------------------------------
const char *KpeDev::Parent()const
{
  return (const char*)cfg_d[KPE_DEV.CnlName().CfgFldId()];
}
//----------------------------------------------------------------
unsigned KpeDev::Type()const
{
  return KPE_DEVICE;
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%