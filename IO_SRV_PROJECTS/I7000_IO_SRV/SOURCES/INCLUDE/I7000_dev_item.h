/***********************************************************
      ласс "Ёлемент модул€ семейства I7000"
               created by M.E.N
***********************************************************/

#ifndef _I7000_dev_item_h_
#define _I7000_dev_item_h_

#include <io_srv_control_object.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class I7000_Device_Item : public ControlObject
{
public:
 explicit I7000_Device_Item(obj_d &d):ControlObject(d){}
 virtual ~I7000_Device_Item(){}
 virtual unsigned Num()const{return 0;}
 virtual const char *Parent()const{return "";}
 virtual void SetError(){}
protected:
};
//----------------------------------------------------------------
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif //I7000_dev_item