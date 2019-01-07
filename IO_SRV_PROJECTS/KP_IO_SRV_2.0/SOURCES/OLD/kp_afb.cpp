#include <kp_afb.h>
#include <kp_dev.h>
#include <kp_io_srv.h>
#include <io_srv_dump.h>
#include <kp_events_cash.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
AFB::AFB(obj_d *d,unsigned char tp)
   :ControlObject(d,tp),parent(NULL)
{
  error=100;
}
//---------------------------------------------------------------
void AFB::SetError(unsigned char err)
{  
}
//---------------------------------------------------------------
bool AFB::StartPollDelay(time_t tm)const
{
  time_t curr_time = time(0);
  long poll_timeout;
  unsigned char poll_val,tp;
  poll_val = *(unsigned char*)parent->Cfg_D()->GetFldValue(KP_DEV.StartPoll().CfgFldId(),tp);  
  poll_timeout=ResponceTimeoutVal()  * poll_val / 100;    
  return ((curr_time-tm) >= poll_timeout); //интервал вышел и нужно слать запрос
}
//---------------------------------------------------------------
unsigned short AFB::ResponceTimeoutVal()const
{  
  unsigned char tp;
  return *(unsigned short*)parent->Cfg_D()->GetFldValue(KP_DEV.ResponceTimeout().CfgFldId(),tp);
}
//---------------------------------------------------------------
bool AFB::ResponceTimeout()const
{
  return (EventsCash.TimeInTv().tv_sec-last_in_time.tv_sec >= ResponceTimeoutVal());
}
//---------------------------------------------------------------
bool AFB::ErrorSetTimeout()const
{
  time_t curr_time = time(0);
  unsigned char tp,err_to_val;
  err_to_val = *(unsigned char*)parent->Cfg_D()->GetFldValue(KP_DEV.ErrorTimeout().CfgFldId(),tp);
  return ((curr_time-timeout_begin_time) >= (ResponceTimeoutVal() * err_to_val));
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

//---------------------------------------------------------------