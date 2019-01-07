/***********************************************************
               created by M.E.N
***********************************************************/

#include <kp_light.h>
#include <kp_types.h>
#include <io_srv_dump.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
LightCtl::LightCtl(obj_d &d, CondParser &p)
  :ControlObject(d),parser(p)
{
}
//---------------------------------------------------------------
const char *LightCtl::Parent()const
{
  return (const char *)cfg_d[KP_LIGHT.Parent().CfgFldId()];
}
//---------------------------------------------------------------
unsigned char LightCtl::Num()const
{
  return (unsigned char)cfg_d[KP_LIGHT.Num().CfgFldId()];
}
//---------------------------------------------------------------
bool LightCtl::On()const
{ 
  if(!Enabled())
    return false;
  if(ConditionPresent())
    return CheckCondition();
  unsigned char tp;
  unsigned char value = *(unsigned char*)cfg_d.GetFldValue(KP_LIGHT.value().CfgFldId(),tp);
  return value!=0;
}
//---------------------------------------------------------------
bool LightCtl::ConditionPresent()const
{
  unsigned char tp;
  const char *cond = (const char *)cfg_d.GetFldValue(KP_LIGHT.Condition().CfgFldId(),tp);
  return (strcmp(cond,"")==0);
}
//---------------------------------------------------------------
bool LightCtl::CheckCondition()const
{
  int result;
  unsigned char tp;
  const char *cond = (const char *)cfg_d.GetFldValue(KP_LIGHT.Condition().CfgFldId(),tp);
  try
  {
    result=parser.ConditionIsTrue(cond);
  }
  catch(char *msg)
  {
    dump.ObjMsg(D_ERR | D_SENSOR,cfg_d.Id(),"Условие объекта %s \"%s\" : %s",cfg_d.Name(),cond,msg);
    return false;
  }
  if(result)
  {
    dump.ObjMsg(D_ERR | D_SENSOR,cfg_d.Id(),"Условие объекта %s \"%s\" : истина!",cfg_d.Name(),cond);
    return true;
  }
  else
  {
    dump.ObjMsg(D_ERR | D_SENSOR,cfg_d.Id(),"Условие объекта %s \"%s\" : ложь!",cfg_d.Name(),cond);
    return false;
  }
}
//---------------------------------------------------------------
unsigned char LightCtl::Enabled()const
{
  unsigned char tp;
  return *(unsigned char*)cfg_d.GetFldValue(KP_LIGHT.Enabled().CfgFldId(),tp);
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%