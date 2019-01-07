/********************************************************************

                     created by M.E.N.
********************************************************************/
#include <I7000_events_cash.h>
#include <io_srv_drv_module.h>
#include <io_srv_event_supervisor.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
I7000_EventsCash::I7000_EventsCash()
{
  event.ev_id = IO_SRV_EV_OBJ_CHANGED;
  Update();
}
//----------------------------------------------------------------
void I7000_EventsCash::Update()
{
  GetCurrTime(tv);
  time_in_dbl = Tv2Dbl(tv);
}
//----------------------------------------------------------------
void I7000_EventsCash::Add(unsigned obj_id)
{
  event.data = &obj_id;
  event.ev_size = sizeof(unsigned);
  GlobalSupervisor->ProcessEvent(GlobalDrv,event);
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
I7000_EventsCash EventsCash;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
