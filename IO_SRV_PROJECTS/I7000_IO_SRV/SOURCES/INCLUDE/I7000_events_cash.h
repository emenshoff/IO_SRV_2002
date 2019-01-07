/********************************************************************

                     created by M.E.N.
********************************************************************/
#ifndef i7000_events_cash_h
#define i7000_events_cash_h

#include <map>
#include <time_utils.h>
#include <io_srv_event.h>

using namespace std;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class I7000_EventsCash
{
public:
 I7000_EventsCash();
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
extern I7000_EventsCash EventsCash;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif