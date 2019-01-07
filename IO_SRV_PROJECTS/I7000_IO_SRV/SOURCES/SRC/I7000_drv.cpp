/***********************************************************
               created by M.E.N
***********************************************************/

#include <io_srv_dump.h>
#include <I7000_io_srv.h>
#include <I7000_config.h>
#include <I7000_drv.h>
#include <I7000_events_cash.h>
typedef map<unsigned,unsigned char,less<unsigned> > map_type;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
DrvModule *GlobalDrv;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
I7000_Drv::I7000_Drv(EventSupervisor *evs,StmCfg *cf)
:DrvModule(evs,cf),traces(cf)
{
  ports.reserve(32);
}
//----------------------------------------------------------------
I7000_Drv::~I7000_Drv()
{
  Clear();
}
//----------------------------------------------------------------
void I7000_Drv::Clear()
{  
  vector<I7000_CommDev *>::iterator i;
  for(i=ports.begin();i!=ports.end();i++)
    delete (*i);
  ports.clear();
  traces.ClearTraces();
}
//----------------------------------------------------------------
void I7000_Drv::Init()
{
  Clear();
  EventsCash.Update();
  I7000_Config *conf = new I7000_Config(this);
  if(!conf->Configure())
    ok = false;
  else
    ok = true;
}
//----------------------------------------------------------------
void I7000_Drv::Step()
{
  vector<I7000_CommDev *>::iterator i;
  for(i=ports.begin();i!=ports.end();i++)
  {
    EventsCash.Update();
    (*i)->Step();
  }
}
//----------------------------------------------------------------
void I7000_Drv::ProcessEvent(const IoSrvEvent &ev)
{
  switch(ev.ev_id)
  {
    case IO_SRV_EV_OBJ_CHANGED:
      if(*traces.GetMask(*(unsigned*)ev.data)
        & *cfg->Obj(*(unsigned*)ev.data)->FldChangesMask())
      {
        dump.Print(D_MSG,"Изменилось конфигурационное поле одного из объектов! Рестарт драйверного модуля...");
        Init();
      }
      break;
    default:
      break;
  }
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%