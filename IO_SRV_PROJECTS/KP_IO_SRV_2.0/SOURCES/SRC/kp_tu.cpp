/***********************************************************
               created by M.E.N
***********************************************************/

#include <kp_tu.h>
#include <kp_types.h>
#include <io_srv_dump.h>
#include <kp_events_cash.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
TUCtl ::TUCtl (obj_d &d)
  :ControlObject(d)
{
  last_val=0;  
  try_count=0;
  ts_ctl = 0;
  ts_net_ctl = 0;
  state=TU_STEP;
  SetError(255);
}
//---------------------------------------------------------------
TUCtl ::~TUCtl ()
{
  Shutdown();
}
//---------------------------------------------------------------
unsigned TUCtl ::Num()const
{
  return (unsigned)(unsigned char)cfg_d[KP_TU.Num().CfgFldId()];
}
//---------------------------------------------------------------
unsigned char TUCtl::GroupNum()const
{
  return cfg_d[KP_TU.GroupNum().CfgFldId()];
}
//---------------------------------------------------------------
const char *TUCtl ::Parent()const
{  
  return cfg_d[KP_TU.Parent().CfgFldId()];
}
//---------------------------------------------------------------
void TUCtl ::SetError(unsigned char err)
{  
  if(err)
    SetResult(-100);
}
//---------------------------------------------------------------
const char *TUCtl::CtlTsName()const
{
  return cfg_d[KP_TU.TU_Confirm_TS().CfgFldId()];
}
//---------------------------------------------------------------
const char *TUCtl::EnTsName()const
{
  return cfg_d[KP_TU.TU_Enable_TS().CfgFldId()];
}
//---------------------------------------------------------------
unsigned char TUCtl ::Step()
{  
  unsigned char st=Value();
  switch(state)
  {
    case TU_STEP:
      if(last_val!=st)
      {
        if(NetTsState())
        {
          if(Enabled())
          {
            tu_time=time(0);//фиксируем время 
            last_val=st;
            if(st)
            {
              dump.ObjMsg(cfg_d.Id(),D_MSG | D_SENSOR,"%s: Попытка выполнить включение ту...",cfg_d.Name());
//              SetResult(-4); // закоментировано по настоянию А. Невзорова.
              state=TU_TRY_ON;
              return TU_ON;
            }
            else
            {
              dump.ObjMsg(cfg_d.Id(),D_MSG | D_SENSOR,"%s: Попытка выполнить выключение ту...",cfg_d.Name());
//              SetResult(-4); // закоментировано по настоянию А. Невзорова.
              state=TU_TRY_OFF;
              return TU_OFF;
            }
          }
          else
          {
            dump.ObjMsg(cfg_d.Id(),D_MSG | D_SENSOR,"%s: Телеуправление заблокировано пользователем!",cfg_d.Name());
            SetResult(-1);
          }
        }
        else
        {
          //dump.ObjMsg(cfg_d.Id(),D_MSG | D_SENSOR,"%s: Телеуправление заблокировано аппаратно!",cfg_d.Name());
          SetResult(-2);
        }
      }
      break;
    case TU_TRY_ON:
      if(CtlTsState()!=0) //телеуправление выполнено
      {
        last_val = 1;
        dump.ObjMsg(cfg_d.Id(),D_MSG | D_SENSOR,"%s: Команда \"включить ту\" выполнена!",cfg_d.Name());
        SetResult(1);
        try_count=0;
        state=TU_STEP;        
      } 
      else
      {
        if(Timeout())
        {
          try_count++;          
          if(try_count>=TURetryOn())
          {
            dump.ObjMsg(cfg_d.Id(),D_ERR | D_SENSOR,"%s: Не могу включить ту! Количество попыток исчерпано!",cfg_d.Name());
            //last_val=st; //спорный момент, но, по-моему, так верно
            cfg_d[KP_TU.Value().CfgFldId()] = last_val;
            try_count=0;
            SetResult(-3);
            state=TU_STEP;
          }
          else
          {
            delay_begin_time=time(0);
            state=TU_DELAY_ON;
          }
        }
      }
      break;
    case TU_TRY_OFF:
      if(CtlTsState()==0) //телеуправление уже выполнено
      {
        last_val = 0;
        dump.ObjMsg(cfg_d.Id(),D_MSG | D_SENSOR,"%s: Команда \"выключить ту\" выполнена!",cfg_d.Name());
        SetResult(0);
        try_count=0;
        state=TU_STEP;        
      }
      else
      {
        if(Timeout())
        {
          try_count++;
          if(try_count>=TURetryOn())
          {
            dump.ObjMsg(cfg_d.Id(),D_ERR | D_SENSOR,"%s: Не могу включить ту! Количество попыток исчерпано!",cfg_d.Name());
            //last_val=st;//????????
            cfg_d[KP_TU.Value().CfgFldId()] = last_val;
            try_count=0;
            SetResult(-3);
            state=TU_STEP;
          }
          else
          {            
            delay_begin_time=time(0);
            state=TU_DELAY_OFF;
          }
        }
      }
      break;
    case TU_DELAY_ON:
      if(DelayComplete())
      {
        state=TU_TRY_ON;
        return TU_ON;
      }
      break;
    case TU_DELAY_OFF:
      if(DelayComplete())
      {
        state=TU_TRY_OFF;
        return TU_OFF;
      }
      break;
    default:      
      break;
  }
  return TU_STEP;
}
//---------------------------------------------------------------
void TUCtl::SetVal(unsigned char val)
{
  if(!ts_ctl)
    if(last_val != val)
    {
      SetResult(val ? 1 : 0);
      last_val = val;
    }
}
//---------------------------------------------------------------
bool TUCtl ::Enabled()const
{
  unsigned char en = cfg_d[KP_TU.Enabled().CfgFldId()];
  return (en!=0);
}
//---------------------------------------------------------------
bool TUCtl ::Timeout()const
{
  return (time(0)-tu_time >= (long)TUTimeOutVal());
}
//---------------------------------------------------------------
bool TUCtl ::DelayComplete()const
{
  return (time(0)-delay_begin_time >= (long)TUDelayVal());
}
//---------------------------------------------------------------
unsigned char TUCtl ::CtlTsState()const
{
  if(!ts_ctl)
    return last_val;
  unsigned char tp,ts_state;
  ts_state = *(unsigned char*)ts_ctl->GetFldValue(KP_TS.value().CfgFldId(),tp);
  return ts_state;
}
//---------------------------------------------------------------
unsigned char TUCtl ::NetTsState()const
{
  if(!ts_net_ctl)
    return 1;
  unsigned char tp,ts_state;
  ts_state = *(unsigned char*)ts_net_ctl->GetFldValue(KP_TS.value().CfgFldId(),tp);
  return ts_state;
}
//---------------------------------------------------------------
unsigned char TUCtl ::Value()const
{
  unsigned char val = cfg_d[KP_TU.Value().CfgFldId()];
  return (val!=0);
}
//---------------------------------------------------------------
unsigned char TUCtl ::TURetryOn()const
{
  return cfg_d[KP_TU.TURetryOn().CfgFldId()];
}
//---------------------------------------------------------------
unsigned char TUCtl ::TURetryOff()const
{
  return cfg_d[KP_TU.TURetryOff().CfgFldId()];
}
//---------------------------------------------------------------
unsigned short TUCtl ::TUDelayVal()const
{
  return cfg_d[KP_TU.TUDelay().CfgFldId()];
}
//---------------------------------------------------------------
unsigned short TUCtl ::TUTimeOutVal()const
{
  return cfg_d[KP_TU.TUTimeOut().CfgFldId()];
}
//---------------------------------------------------------------
void TUCtl ::SetResult(char res)
{
  // 0  - команда выполнена, телеуправление отключено
  // 1  - команда выполнена, телеуправление включено
  // -1 - ту запрещено пользователем
  // -2 - ту запрещено аппаратно
  // -3 - попытки исчерпаны
  // -4 - команда в стадии выполнения, блок занят
  // -5 - команда ту соответствует контролируемого тс-у
  // -100 - состояние не определено (начальное состояние)
  double tm = EventsCash.TimeInDbl();//фиксируем время ошибки или выполнения ту
  cfg_d.SetFldValue(KP_TU.result().CfgFldId(),(void*)&res,KP_TU.result().Type());
  cfg_d.SetFldValue(KP_TU.time().CfgFldId(),(void*)&tm,KP_TU.time().Type());
  cfg_d.SetFldChangeBit(KP_TU.result().CfgFldId());
  cfg_d.SetFldChangeBit(KP_TU.time().CfgFldId());
  EventsCash.Add(cfg_d.Id());
  cfg_d.ClearFldChangeBit(KP_TU.result().CfgFldId());
  cfg_d.ClearFldChangeBit(KP_TU.time().CfgFldId());  
}
//---------------------------------------------------------------
void TUCtl ::Shutdown()
{
  SetResult(-100);
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%