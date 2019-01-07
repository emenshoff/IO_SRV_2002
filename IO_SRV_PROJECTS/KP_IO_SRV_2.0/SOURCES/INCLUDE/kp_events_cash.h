/********************************************************************
         ”тилиты дл€ работы со временем и генерации событий.
                     created by M.E.N.
********************************************************************/
#ifndef kp_events_cash_h
#define kp_events_cash_h

#include <time_utils.h>
#include <io_srv_event.h>
#include <io_srv_drv_module.h>
#include <io_srv_event_supervisor.h>

using namespace std;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class KpEventsCash
{
public:
 KpEventsCash();
 ~KpEventsCash(){} 
 void Update(); 
 void Add(unsigned obj_id);
 double TimeInDbl()const{return time_in_dbl;}
 const struct timeval &TimeInTv()const{return tv;}
private:
 IoSrvEvent event;
 struct timeval tv;
 double time_in_dbl;
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
extern KpEventsCash EventsCash;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif