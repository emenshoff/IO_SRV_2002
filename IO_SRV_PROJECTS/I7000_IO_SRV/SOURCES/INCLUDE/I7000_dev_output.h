/***********************************************************
 Класс "Выход модуля семейства I7000"
               created by M.E.N
***********************************************************/

#ifndef _I7000_dev_output_h_
#define _I7000_dev_output_h_

#include <I7000_dev_item.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class I7000_Device_Output : public I7000_Device_Item
{
public:
  explicit I7000_Device_Output(obj_d &d):I7000_Device_Item(d){}
  virtual ~I7000_Device_Output(){}
  virtual unsigned Num()const{return 0;}  
protected:
};
//----------------------------------------------------------------
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif //I7000_dev_output