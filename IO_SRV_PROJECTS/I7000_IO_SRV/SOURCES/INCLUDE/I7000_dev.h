/***********************************************************
   Базовый класс для модулей ввода-вывода серии I7000.
               created by M.E.N
***********************************************************/

#ifndef _I7000_dev_h_
#define _I7000_dev_h_

#include <I7000_serial.h>
#include <io_srv_control_object.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#define DEV_FREE   0
#define DEV_STEP   1
#define DEV_ERR    -1
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class I7000_Dev : public ControlObject
{
  friend class I7000_Config;
public:
  I7000_Dev(obj_d &d):ControlObject(d){}
  virtual ~I7000_Dev(){}
  virtual int Step(){return 0;}
  virtual void Init(){}
  virtual unsigned short ModuleId()const{return 7000;}  
  virtual unsigned char MaxItemsAmount()const;
  unsigned char Addr()const{return address;}
  virtual const char *CommDevName()const{return "";}
protected:
  I7000_SerialPort *port;
  unsigned char address;
private:
  void operator=(I7000_Dev&){}
};
//----------------------------------------------------------------
unsigned char GetSpeed_I7000(int);
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif //I7000_dev