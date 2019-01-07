/***********************************************************
 Объект пассивен - обработка отказов и блокировок возложена
 на верхний уровень (объект - субблок).
               created by M.E.N
***********************************************************/

#include <kp_ts.h>
#include <kp_types.h>
#include <io_srv_dump.h>
#include <kp_events_cash.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//#define TS_WAS_DISABLED 0x01
//#define TS_WAS_ERRORED  0x02
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
TSCtl::TSCtl(obj_d &d)
  :ControlObject(d)
{
  unsigned char inv = cfg_d[KP_TS.Inverted().CfgFldId()];
  //изначально значение ТС недостоверно
  SetError(255);
  //инициализируем поле value
  cfg_d[KP_TS.value().CfgFldId()] = (unsigned char)5;  
  cfg_d.SetFldChangeBit(KP_TS.value().CfgFldId());
  EventsCash.Add(cfg_d.Id());  
  cfg_d.ClearFldChangeBit(KP_TS.value().CfgFldId());
}
//---------------------------------------------------------------
bool TSCtl::Enabled()const
{
  unsigned char en = cfg_d[KP_TS.Enabled().CfgFldId()];
  return en != 0;
}
//---------------------------------------------------------------
unsigned char TSCtl::Error()const
{
  return cfg_d[KP_TS.error().CfgFldId()];
}
//---------------------------------------------------------------
const char *TSCtl::Parent()const
{
  return (const char*)cfg_d[KP_TS.Parent().CfgFldId()];
}
//---------------------------------------------------------------
unsigned char TSCtl::Num()const
{
  return cfg_d[KP_TS.Num().CfgFldId()];
}
//---------------------------------------------------------------
void TSCtl::SetError(unsigned char err)
{
  unsigned char error = cfg_d[KP_TS.error().CfgFldId()];
  if(error == err)
    return;
  double tm = EventsCash.TimeInDbl();
  cfg_d[KP_TS.error().CfgFldId()] = err;
  cfg_d[KP_TS.time().CfgFldId()] = tm;
  cfg_d.SetFldChangeBit(KP_TS.error().CfgFldId());
  cfg_d.SetFldChangeBit(KP_TS.time().CfgFldId());
  EventsCash.Add(cfg_d.Id());
  cfg_d.ClearFldChangeBit(KP_TS.error().CfgFldId());
  cfg_d.ClearFldChangeBit(KP_TS.time().CfgFldId());
}
//---------------------------------------------------------------
void TSCtl::SetState(unsigned char state)
{
  //признак разрешения работы
  double tm;
  //признак логич. инверсии  
  unsigned char Inverted = cfg_d[KP_TS.Inverted().CfgFldId()];
  unsigned char last_state = cfg_d[KP_TS.value().CfgFldId()];
  dump.ObjMsg(cfg_d.Id(),D_MSG | D_SENSOR,"%s : пришло значение %d...",cfg_d.Name(),state);
  unsigned char st = state ^ Inverted;
  if(last_state == 5) //первое переключение
  {
    if(Inverted)
      dump.ObjMsg(cfg_d.Id(),D_MSG | D_SENSOR,"%s : инверсия переключения",cfg_d.Name());
    dump.ObjMsg(cfg_d.Id(),D_MSG | D_SENSOR,"%s : первое переключение в %d...",cfg_d.Name(),st);
    tm = EventsCash.TimeInDbl();
    cfg_d[KP_TS.error().CfgFldId()] = (unsigned char)0;
    cfg_d[KP_TS.value().CfgFldId()] = st;
    cfg_d[KP_TS.time().CfgFldId()] = tm;
    cfg_d.SetFldChangeBit(KP_TS.time().CfgFldId());
    cfg_d.SetFldChangeBit(KP_TS.value().CfgFldId());
    cfg_d.SetFldChangeBit(KP_TS.error().CfgFldId());
    EventsCash.Add(cfg_d.Id());
    cfg_d.ClearFldChangeBit(KP_TS.time().CfgFldId());
    cfg_d.ClearFldChangeBit(KP_TS.value().CfgFldId());
    cfg_d.ClearFldChangeBit(KP_TS.error().CfgFldId());
  }
  else if(last_state != st)
  {
    if(Inverted)
      dump.ObjMsg(cfg_d.Id(),D_MSG | D_SENSOR,"%s : инверсия переключения",cfg_d.Name());
    dump.ObjMsg(cfg_d.Id(),D_MSG | D_SENSOR,"%s : переключение в %d...",cfg_d.Name(),st);
    tm = EventsCash.TimeInDbl();
    /*tm = EventsCash.TimeInDbl();
    cfg_d[KP_TS.error().CfgFldId()] = (unsigned char)0;
    if(Inverted)
    {
      dump.ObjMsg(cfg_d.Id(),D_MSG | D_SENSOR,"%s : инверсия переключения  %d...",cfg_d.Name(),!state);
      cfg_d[KP_TS.value().CfgFldId()] = !state;
    }
    else*/
      cfg_d[KP_TS.value().CfgFldId()] = st;
    cfg_d[KP_TS.error().CfgFldId()] = (unsigned char)0;
    cfg_d[KP_TS.time().CfgFldId()] = tm;
    cfg_d.SetFldChangeBit(KP_TS.time().CfgFldId());
    cfg_d.SetFldChangeBit(KP_TS.value().CfgFldId());
    cfg_d.SetFldChangeBit(KP_TS.error().CfgFldId());
    EventsCash.Add(cfg_d.Id());
    cfg_d.ClearFldChangeBit(KP_TS.time().CfgFldId());
    cfg_d.ClearFldChangeBit(KP_TS.value().CfgFldId());
    cfg_d.ClearFldChangeBit(KP_TS.error().CfgFldId());
  }
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%