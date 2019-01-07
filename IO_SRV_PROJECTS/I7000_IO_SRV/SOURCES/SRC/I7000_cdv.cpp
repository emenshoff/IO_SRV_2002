/***********************************************************
               created by M.E.N
***********************************************************/

#include <I7000_types.h>
#include <I7000_dev.h>
#include <I7000_cdv.h>
#include <I7000_events_cash.h>
#include <io_srv_dump.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
I7000_CommDev::I7000_CommDev(obj_d &d)
:I7000_SerialPort(),ControlObject(d)
{
  SetError(100);
  Items.reserve(256);
  curr_item = Items.begin();
}
//----------------------------------------------------------------
I7000_CommDev::~I7000_CommDev()
{
  for(curr_item = Items.begin();curr_item!=Items.end();curr_item++)
    delete (*curr_item);
  Items.clear();
}
//----------------------------------------------------------------
void I7000_CommDev::Step()
{  
  if(curr_item != Items.end())
  {
    if((*curr_item)->Step() != DEV_STEP)
      ++curr_item;  
  }
  else
    curr_item = Items.begin();
}
//----------------------------------------------------------------
bool I7000_CommDev::Init()
{
  const char *name = (const char*)cfg_d[COMM_DEV.PortName().CfgFldId()];
  unsigned long speed = (unsigned long)cfg_d[COMM_DEV.PortSpeed().CfgFldId()];
  if(!I7000_SerialPort::Init(name,speed,0xff))
  {
    SetError(100);
    return false;
  }
  //for(curr_item = Items.begin();curr_item!=Items.end();curr_item++)
    //(*curr_item)->Init();
  SetError(0);
  return true;
}
//----------------------------------------------------------------
void I7000_CommDev::SetError(unsigned char err)
{
  cfg_d[COMM_DEV.error().CfgFldId()] = err;
  cfg_d[COMM_DEV.etime().CfgFldId()] = EventsCash.TimeInDbl();
  EventsCash.Add(cfg_d.Id());
  cfg_d.ClearFldChangesMask();
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%