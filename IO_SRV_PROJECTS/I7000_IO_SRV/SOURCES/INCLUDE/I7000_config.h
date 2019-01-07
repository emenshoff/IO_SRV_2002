/***********************************************************
               created by M.E.N
***********************************************************/

#ifndef _I7000_config_h_
#define _I7000_config_h_

#include <I7000_drv.h>
#include <I7000_types.h>
#include <I7000_dev.h>
#include <I7000_dev_input.h>
#include <I7000_dev_output.h>

//typedef vector<pair<ControlObject *,unsigned char> > ObjVectorType;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class I7000_Config
{
public:
  I7000_Config(I7000_Drv *d):drv(d){}
  ~I7000_Config();
  bool Configure();
  void CheckForErrors();
private:
  void CheckTypeDescription();
  void Clear();
  I7000_Drv *drv;
  vector<pair<I7000_CommDev *,unsigned char> > CDVs;
  vector<pair<I7000_Dev *,unsigned char> > Devs;
  vector<pair<I7000_Device_Input *,unsigned char> > D_Is;
  vector<pair<I7000_Device_Output *,unsigned char> > D_Os;
};
//----------------------------------------------------------------
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif //I7000_config