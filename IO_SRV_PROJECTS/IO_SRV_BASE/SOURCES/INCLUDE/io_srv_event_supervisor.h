/********************************************************************
          Базовый класс -  диспетчер событий.
                     created by M.E.N.
********************************************************************/
#ifndef _event_supervisor_h
#define _event_supervisor_h

#include <list>
#include <io_srv_event_processor.h>

using namespace std;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class EventSupervisor
{
public:
 EventSupervisor();
 virtual ~EventSupervisor();
 virtual void AddProcessor(EventProcessor *);
 virtual void ProcessEvent(EventProcessor *pr,const IoSrvEvent &);
protected:
 list<EventProcessor *> processors;
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
extern EventSupervisor *GlobalSupervisor;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif