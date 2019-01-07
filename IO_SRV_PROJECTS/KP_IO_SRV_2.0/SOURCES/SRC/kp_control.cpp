#include <kp_control.h>
#include <kp_functional.h>
#include <kp_config.h>
#include <kp_io_srv.h>
#include <io_srv_dump.h>
#include <kp_events_cash.h>

DrvModule *GlobalDrv;
typedef map<unsigned,unsigned char,less<unsigned> > map_type;
//-----------------------------------------------------------------
KpControl::KpControl(EventSupervisor *evs,StmCfg *cf) 
  :DrvModule(evs,cf),ok(false),tracer(cf),parser(cf)
{
  EventsCash.Update();
  Ports.reserve(32);
  ItgTits.reserve(256);
}
//-----------------------------------------------------------------
KpControl::~KpControl()
{
  Clear();
}
//-----------------------------------------------------------------
void KpControl::ProcessEvent(const IoSrvEvent &ev)
{
  switch(ev.ev_id)
  {
    case IO_SRV_EV_OBJS_CHANGED:
     /* {
        map_type *mp;
        map_type::const_iterator i;
        mp = (map_type*)ev.data;
        for(i = mp->begin();i!=mp->end();i++)
          if(tracer.GetMask((*i).first) != 0)
          {
            dump.Print(D_MSG,"Обнаружено изменение конфигурационного поля одного из объектов. Рестарт драйверного модуля...");
            Reconfig();
          }
      }*/
      break;
    case IO_SRV_EV_OBJ_CHANGED:
      if(*tracer.GetMask(*(unsigned*)ev.data)
        & *cfg->Obj(*(unsigned*)ev.data)->FldChangesMask())
      {
        dump.Print(D_MSG,"Обнаружено изменение конфигурационного поля одного из объектов. Рестарт драйверного модуля...");
        Reconfig();
      }
      break;
    case IO_SRV_EV_SHUTDOWN:
      break;
    case IO_SRV_EV_HALT:
      break;
    case IO_SRV_EV_SUSPEND:
      break;
    case IO_SRV_EV_RESUME:
      break;
    default:
      return;
  }
}
//-----------------------------------------------------------------
void KpControl::Reconfig()
{
  static IoSrvEvent evn;
  evn.ev_id = IO_SRV_EV_SUSPEND;
  supervisor->ProcessEvent(this,evn);
  Clear();
  Init();
  evn.ev_id = IO_SRV_EV_RESUME;
  supervisor->ProcessEvent(this,evn);
}
//-----------------------------------------------------------------
void KpControl::Step()
{
  vector<KpCommDev *>::iterator port_i;  
  if(!ok)
    return;
  //цикл по портам
  EventsCash.Update();  
  for(port_i=Ports.begin();port_i!=Ports.end();port_i++)
  {
    (*port_i)->Process();
  }
  int i;
  if(titi_i==ItgTits.end())
    titi_i=ItgTits.begin();
  for(i=0;titi_i!=ItgTits.end();titi_i++,i++)
  {     
    if(i<5)
      (*titi_i)->Step();
    else
      break;
    //15.04.02 - исправлено для уменьшения загрузки процессора. MEN
    
  }
}
//-----------------------------------------------------------------
void KpControl::Init()
{
  vector<KpCommDev *>::iterator port_i;
  KpConfig *config=new KpConfig(this);
  if(!config->Init())
    ok=false;
  else
  {
    for(port_i=Ports.begin();port_i!=Ports.end();port_i++)
    {}//  (*port_i)->StartPortPoll();
    titi_i=ItgTits.begin();
    ok=true;
  }
  delete config;
}
//-----------------------------------------------------------------
void KpControl::Clear()
{
  vector<KpCommDev *>::iterator pi;
  vector<TITiCtl *>::iterator itg_i;  
  for(pi=Ports.begin();pi!=Ports.end();pi++)
    delete (*pi);
  for(itg_i=ItgTits.begin();itg_i!=ItgTits.end();itg_i++)
    delete (*itg_i);  
  Ports.clear();
  ItgTits.clear();
  tracer.ClearTraces();
}
//-----------------------------------------------------------------