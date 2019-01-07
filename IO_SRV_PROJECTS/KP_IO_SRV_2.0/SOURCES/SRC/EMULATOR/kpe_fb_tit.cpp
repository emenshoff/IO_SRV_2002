/***********************************************************
               created by M.E.N
***********************************************************/

#include <kpe_dev.h>
#include <kpe_types.h>
#include <kpe_fb_tit.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
KpeFbTit::KpeFbTit(obj_d &d)
:KpeAfb(d)
{
  for(int i=0;i<32;i++)
    Items[i] = 0;
  state = FIRST_G;
  data[0] = parent->Address();
  data[1] = (0x04 << 4) | Place();
}
//----------------------------------------------------------------
KpeFbTit::~KpeFbTit()
{
  for(int i=0;i<32;i++)
    if(Items[i])
      delete Items[i];
}
//----------------------------------------------------------------
unsigned KpeFbTit::Type()const
{
  return FBE_TIT;
}
//----------------------------------------------------------------
unsigned char KpeFbTit::Place()const
{
  return (unsigned char)cfg_d[KPE_AFB_TIT.Place().CfgFldId()];
}
//----------------------------------------------------------------
const char *KpeFbTit::Parent()const
{
  return (const char*)cfg_d[KPE_AFB_TIT.Parent().CfgFldId()];
}
//----------------------------------------------------------------
const unsigned char *KpeFbTit::Step(int &len)
{
  len = 0;
  int i;
  bool changed = false;
  unsigned char tp,enabled,tmp[16];
  enabled = *(unsigned char*)cfg_d.GetFldValue(KPE_AFB_TIT.Enabled().CfgFldId(),tp);  
  switch(state)
  {
    case FIRST_G:
      for(i=0;i<16;i++)
      {
        if(Items[i])
          tmp[i] = Items[i]->Value();
        else
          tmp[i] = 0;
        if(*(data+3+i) != tmp[i])
          changed = true;
      }
      data[2] = 0x06<<4;
      state = SECOND_G;
      break;
    case SECOND_G:
      for(i=15;i<32;i++)
      {
        if(Items[i])
          tmp[i] = Items[i]->Value();
        else
          tmp[i] = 0;
        if(*(data+3+i) != tmp[i])
          changed = true;
      }
      data[2] = (0x06<<4) | 0x01;
      state = FIRST_G;
      break;
  }
  if(changed)
  {
    len = 19;
    memcpy(data+3,tmp,16);
    return data;
  }
  return 0;
}
//----------------------------------------------------------------
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%