/***********************************************************
               created by M.E.N
***********************************************************/

#include <kp_fb_light.h>
#include <kp_dev.h>
#include <kp_types.h>
#include <io_srv_dump.h>
#include <kp_events_cash.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
FbLight::FbLight(obj_d &d)
  :AFB(d)
{
 fb_type = (unsigned char)cfg_d[KP_AFB_LIGHT.Type().CfgFldId()];
 Items = new LightCtl*[fb_type];
 for(unsigned i=0;i<fb_type;i++)
   Items[i] = 0;
 memset(tmp_data,0,sizeof(tmp_data));
}
//---------------------------------------------------------------
FbLight::~FbLight()
{
  Shutdown();
  for(unsigned i=0;i<fb_type;i++)
    if(Items[i])
      delete Items[i];
  delete [] Items;
}
//---------------------------------------------------------------
unsigned char FbLight::Error()const
{
  return cfg_d[KP_AFB_LIGHT.error().CfgFldId()];
}
//---------------------------------------------------------------
unsigned char FbLight::Place()const
{
  unsigned char tp;
  return *(unsigned char*)cfg_d.GetFldValue(KP_AFB_LIGHT.Place().CfgFldId(),tp);
}
//---------------------------------------------------------------
const char *FbLight::Parent()const
{
  unsigned char tp;
  return (const char *)cfg_d.GetFldValue(KP_AFB_LIGHT.Parent().CfgFldId(),tp);
}
//---------------------------------------------------------------
int FbLight::Step()
{
  if(!Enabled())
    return AFB_FREE;
/*
  int i;//,sz=(type==FB_LIGHT_32 ? 4 : 8);
  switch(state)
  {
    case FB_LIGHT_STEP:
      if(StateChanged())
        SendData();
      break;
    case FB_LIGHT_BEGIN:
      StateChanged();//инициализация массива :)
      if(SendData())
      {
        for(i=0;i<8;i++)
          tmp_data[i+8]=tmp_data[i];
        state=FB_LIGHT_STEP;
      }
      break;
    default:
      break;
  }*/
  return AFB_FREE;
}
//---------------------------------------------------------------
int FbLight::SendData()
{
  unsigned char data[18];
  data[0]=parent->Address();
  data[1]=Place();
  //как в предыдущей версии - все в прямом коде
  if(fb_type==64)
  {
    *(unsigned*)&data[2] = *(unsigned*)&tmp_data[3];
    *(unsigned*)&data[6] = *(unsigned*)&tmp_data[0];
  }
  else
  {
    *(unsigned*)&data[2] = *(unsigned*)&tmp_data[3];
    *(unsigned*)&data[6] = *(unsigned*)&tmp_data[3];
    *(unsigned*)&data[10] = *(unsigned*)&tmp_data[3];
    *(unsigned*)&data[14] = *(unsigned*)&tmp_data[3];
  }
  if(parent->cnl->CanSend())
  {
    parent->cnl->Send(data,18);
    for(int i=0;i<8;i++)
      tmp_data[i+8]=tmp_data[i];
    return 1;
  }
  return 0;
}
//---------------------------------------------------------------
bool FbLight::StateChanged()
{
  bool result=false;
  int i=0,by,bt,sz=(fb_type==32 ? 4 : 8);
  for(by=0;by<sz;by++)
  {
    unsigned char mask = 0x80;
    for(bt=0;bt<8;mask>>=1,bt++,i++)
      if(Items[i])
      {
        if(Items[i]->On())
          tmp_data[by] |= mask;
        else
          tmp_data[by] ^= mask;
      }
    if(tmp_data[by+8]!=tmp_data[by])
      result=true;
  }
  return result;
}
//---------------------------------------------------------------
void FbLight::Shutdown()
{
  if(parent)
  {
    unsigned char data[18];
    data[0]=parent->Address();
    data[1]=Place();
    memset(data+2,0,sizeof(data)-2);
    parent->cnl->Send(data,18); //здесь контроля нет!
  }
}
//---------------------------------------------------------------
bool FbLight::Enabled()const
{
  unsigned char tp;
  return (*(unsigned char*)cfg_d.GetFldValue(KP_AFB_LIGHT.Enabled().CfgFldId(),tp)!=0);
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%