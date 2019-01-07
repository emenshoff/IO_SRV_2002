/***********************************************************
               created by M.E.N
***********************************************************/

#include <io_srv_dump.h>
#include <kpe_dev.h>
#include <kpe_fb_ts.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
KpeFbTs::KpeFbTs(obj_d &d)
:KpeAfb(d)
{
  for(int i=0;i<64;i++)
    Items[i] = 0;
  data[0] = parent->Address();
  data[1] = 0x04 | Place();
  data[2] = 0x02 << 4;
  memset(data+3,0x0,8);
  memset(data+3,0xff,8);
}
//----------------------------------------------------------------
KpeFbTs::~KpeFbTs()
{
  for(int i=0;i<64;i++)
    if(Items[i])
      delete Items[i];
}
//----------------------------------------------------------------
unsigned char KpeFbTs::Place()const
{
  return (unsigned char)cfg_d[KPE_AFB_TS.Place().CfgFldId()];
}
//----------------------------------------------------------------
const char *KpeFbTs::Parent()const
{
  return (const char*)cfg_d[KPE_AFB_TS.Parent().CfgFldId()];
}
//----------------------------------------------------------------
const unsigned char *KpeFbTs::Step(int &len)
{
  unsigned char tp,enabled;
  len = 0;  
  enabled = *(unsigned char*)cfg_d.GetFldValue(KPE_AFB_TS.Enabled().CfgFldId(),tp);
  if(enabled)
  {
    int i=0,bt;
    unsigned long msk,tmp[2] = {0};
    for(int pos=0;pos<2;pos++)
    {
      msk = 0x100000000;
      for(bt=0;bt<32;msk >>= 1,bt++,i++)//цикл по битам
        if(Items[i])
          if(Items[i]->Value())
            tmp[pos] |= msk;
    }
    if(*(unsigned long*)(data+11) != tmp[0] && *(unsigned long*)(data+15) != tmp[1])
    {
      len = sizeof(data);
      *(unsigned long*)(data+3) = tmp[0];
      *(unsigned long*)(data+7) = tmp[1];
      tmp[0] != tmp[0];
      tmp[1] != tmp[1];
      *(unsigned long*)(data+11) = tmp[0];
      *(unsigned long*)(data+15) = tmp[1];
      return data;
    }
  }
  return 0;
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%