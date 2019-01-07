#include <kp_abstract_dev.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
KpAbstractDev::KpAbstractDev(obj_d &d):ControlObject(d)
{
  addr = 128;
  cnl=0;
}
//--------------------------------------------------------------------
const char *KpAbstractDev::Parent()const
{
  return "";
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%