/********************************************************************
          Базовый класс примитивного обработчика событий.
                     created by M.E.N.
********************************************************************/
#ifndef _event_processor_h
#define _event_processor_h

#include <io_srv_event.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class EventSupervisor;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class EventProcessor
{
public:
 virtual void ProcessEvent(const IoSrvEvent &){}
protected:
 EventProcessor(EventSupervisor *s):supervisor(s){}
 virtual ~EventProcessor(){}
 EventSupervisor *supervisor;
private:
 EventProcessor(EventProcessor&){}
 void operator=(EventProcessor&){} 
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif