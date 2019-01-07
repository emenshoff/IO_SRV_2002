/***********************************************************
               created by M.E.N
***********************************************************/

#include <kp_events_cash.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
KpEventsCash EventsCash;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
KpEventsCash::KpEventsCash()
{
  event.ev_id = IO_SRV_EV_OBJ_CHANGED;
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
void KpEventsCash::Update()
{
  GetCurrTime(tv);
  time_in_dbl = Tv2Dbl(tv);
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
void KpEventsCash::Add(unsigned obj_id)
{
  event.data = &obj_id;
  event.ev_size = sizeof(unsigned);
  GlobalSupervisor->ProcessEvent(GlobalDrv,event);
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

