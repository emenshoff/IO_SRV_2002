/***********************************************************
 Класс "Цифровой вход модуля семейства I7000"
               created by M.E.N
***********************************************************/

#ifndef _I7000_dev_digital_input_h_
#define _I7000_dev_digital_input_h_

#include <I7000_types.h>
#include <I7000_dev_input.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class I7000_DigitalInput : public I7000_Device_Input
{
public:
 I7000_DigitalInput(obj_d &d):I7000_Device_Input(d){}
 virtual ~I7000_DigitalInput(){}
 virtual bool Enabled()const{return false;}
protected:
};
//----------------------------------------------------------------
//простой телесигнал
class I7000_Ts : public I7000_DigitalInput
{
public:
 I7000_Ts(obj_d &d);
 virtual ~I7000_Ts();
 void SetError(unsigned char);
 const char *Parent()const;
 unsigned Num()const; 
 void SetState(bool on);
protected:
private:
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif //I7000_dev_digital_input