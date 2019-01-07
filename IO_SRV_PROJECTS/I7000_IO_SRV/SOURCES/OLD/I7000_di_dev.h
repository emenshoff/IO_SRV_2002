/***********************************************************
               created by M.E.N
***********************************************************/

#ifndef _I7000_di_dev_h_
#define _I7000_di_dev_h_
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
template <unsigned short MODULE_TYPE = 7000,unsigned char ITEM_AMOUNT = 0>
class I7000_Dev : public ControlObject
{
  friend class I7000_Config;
 I7000_Dev(obj_d &d):ControlObject(d){}
 virtual ~I7000_Dev(){}
public:
protected:
 I7000_Ts *Items[];
private:
};
//----------------------------------------------------------------
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif //I7000_di_dev