#include <algorithm>
#include <io_srv_event_supervisor.h>

typedef list<EventProcessor *>::iterator iter_type;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
EventSupervisor::EventSupervisor()
{
  GlobalSupervisor = this;
}
//--------------------------------------------------------------------------------
EventSupervisor::~EventSupervisor()
{
  processors.clear();  
}
//--------------------------------------------------------------------------------
void EventSupervisor::AddProcessor(EventProcessor *pr)
{
  iter_type i;
  if((i=find(processors.begin(),processors.end(),pr)) == processors.end())
    processors.push_back(pr);
}
//--------------------------------------------------------------------------------
void EventSupervisor::ProcessEvent(EventProcessor *pr,const IoSrvEvent &event)
{
  iter_type i;
  for(i=processors.begin();i!=processors.end();i++)
    if(pr != *i) //���, ��� ������������ �������, �� ������ ��� ������������!
      (*i)->ProcessEvent(event);
}
//--------------------------------------------------------------------------------
EventSupervisor *GlobalSupervisor;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%