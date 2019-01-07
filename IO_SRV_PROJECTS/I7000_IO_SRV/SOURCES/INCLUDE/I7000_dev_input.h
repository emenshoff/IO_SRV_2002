/***********************************************************
   ласс "¬ход модул€ семейства I7000"
               created by M.E.N
***********************************************************/

#ifndef _I7000_dev_input_h_
#define _I7000_dev_input_h_

#include <I7000_dev_item.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class I7000_Device_Input : public I7000_Device_Item
{
public:
  explicit I7000_Device_Input(obj_d &d):I7000_Device_Item(d){}
  virtual ~I7000_Device_Input(){}
  virtual unsigned Num()const{return 0;}  
protected:
};
//----------------------------------------------------------------
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif //I7000_dev_input