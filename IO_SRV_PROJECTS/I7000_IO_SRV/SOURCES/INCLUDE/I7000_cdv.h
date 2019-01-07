/***********************************************************
               created by M.E.N
***********************************************************/

#ifndef _I7000_cdv_h_
#define _I7000_cdv_h_

#include <I7000_dev.h>
#include <I7000_proto.h>
#include <I7000_serial.h>
#include <vector>

using namespace std;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class I7000_CommDev : public ControlObject,public I7000_SerialPort
{
 friend class I7000_Config;
public:
 I7000_CommDev(obj_d &d);
 ~I7000_CommDev();
 void Step();
 bool Init();
protected:
 void SetError(unsigned char err);
 vector<I7000_Dev *>::iterator curr_item;
 vector<I7000_Dev *> Items;
};
//----------------------------------------------------------------
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif //I7000_cdv