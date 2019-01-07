#include <kp_afb.h>
#include <kp_dev.h>
#include <kp_types.h>
#include <kp_io_srv.h>
#include <io_srv_dump.h>
#include <kp_events_cash.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
AFB::AFB(obj_d &d)
   :ControlObject(d),parent(NULL)
{
  GetCurrTime(last_in_time);
}
//---------------------------------------------------------------
void AFB::SetError(unsigned char err)
{  
}
//---------------------------------------------------------------
bool AFB::ResponceTimeout()const
{
  return (EventsCash.TimeInTv().tv_sec-last_in_time.tv_sec >= parent->ResponceTimeoutVal());
}
//---------------------------------------------------------------
bool AFB::ErrorSetTimeout()const//18.04.02
{
  struct timeval curr_tv;
  GetCurrTime(curr_tv);
  long timeout_begin_time = last_in_time.tv_sec;
  unsigned char err_to_val = parent->ErrorSetTimeoutVal();
  return ((curr_tv.tv_sec-timeout_begin_time) >= (parent->ResponceTimeoutVal() * err_to_val));
  /*
  struct timeval curr_tv;
  GetCurrTime(curr_tv);
  long timeout_begin_time = last_in_time.tv_sec+parent->ResponceTimeoutVal();
  unsigned char err_to_val = parent->ErrorSetTimeoutVal();
  return ((curr_tv.tv_sec-timeout_begin_time) >= (parent->ResponceTimeoutVal() * err_to_val));
  */
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
