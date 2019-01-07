/***********************************************************
               created by M.E.N
***********************************************************/

#include <io_srv_dump.h>
#include <I7000_types.h>
#include <time_utils.h>
#include <I7000_events_cash.h>
#include <I7000_dev_digital_output.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
I7000_Tu ::I7000_Tu (obj_d &d)
  :I7000_DigitalOutput(d)
{
  last_val=0;  
  try_count=0;
  ts_ctl = 0;
  ts_net_ctl = 0;
  state=TU_STEP;
  SetError(100);
}
//---------------------------------------------------------------
I7000_Tu ::~I7000_Tu ()
{
  Shutdown();
}
//---------------------------------------------------------------
unsigned I7000_Tu ::Num()const
{
  return (unsigned)(unsigned char)cfg_d[I7000_TU.Num().CfgFldId()];
}
//---------------------------------------------------------------
const char *I7000_Tu ::Parent()const
{  
  return (const char *)cfg_d[I7000_TU.Parent().CfgFldId()];
}
//---------------------------------------------------------------
void I7000_Tu ::SetError(unsigned char err)
{  
  if(err)
    SetResult(-100);
}
//---------------------------------------------------------------
const char *I7000_Tu::CtlTsName()const
{
  return (const char*)cfg_d[I7000_TU.TU_Control_TS().CfgFldId()];
}
//---------------------------------------------------------------
const char *I7000_Tu::EnTsName()const
{
  return (const char*)cfg_d[I7000_TU.TU_Enable_TS().CfgFldId()];
}
//---------------------------------------------------------------
unsigned char I7000_Tu ::Step()
{  
  unsigned char st=(unsigned char)Value();
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
              SetResult(-4);
              state=TU_TRY_ON;
              return TU_ON;
            }
            else
            {
              dump.ObjMsg(cfg_d.Id(),D_MSG | D_SENSOR,"%s: Попытка выполнить выключение ту...",cfg_d.Name());
              SetResult(-4);
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
          dump.ObjMsg(cfg_d.Id(),D_MSG | D_SENSOR,"%s: Телеуправление заблокировано аппаратно!",cfg_d.Name());
          SetResult(-2);
        }
      }
      break;
    case TU_TRY_ON:
      if(CtlTsState()==1) //телеуправление выполнено
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
            last_val=st; //спорный момент, но, по-моему, так верно
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
            last_val=st;//????????
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
void I7000_Tu::SetVal(unsigned char val)
{
  if(!ts_ctl)
    if(last_val != val)
    {
      SetResult(val ? 1 : 0);
      last_val = val;
    }
}
//---------------------------------------------------------------
bool I7000_Tu ::Enabled()const
{
  return ((unsigned char)cfg_d[I7000_TU.Enabled().CfgFldId()]!=0);
}
//---------------------------------------------------------------
bool I7000_Tu ::Timeout()const
{
  return (time(0)-tu_time >= (long)TUTimeOutVal());
}
//---------------------------------------------------------------
bool I7000_Tu ::DelayComplete()const
{
  return (time(0)-delay_begin_time >= (long)TUDelayVal());
}
//---------------------------------------------------------------
unsigned char I7000_Tu ::CtlTsState()const
{
  if(!ts_ctl)
    return last_val;
  unsigned char tp,ts_state,invertion;
  ts_state = *(unsigned char*)ts_ctl->GetFldValue(I7000_TS.value().CfgFldId(),tp);
  invertion = *(unsigned char*)ts_ctl->GetFldValue(I7000_TS.Inverted().CfgFldId(),tp);
  ts_state ^= invertion;
  return ts_state;
}
//---------------------------------------------------------------
unsigned char I7000_Tu ::NetTsState()const
{
  if(!ts_net_ctl)
    return 1;
  unsigned char tp,ts_state,invertion;
  ts_state = *(unsigned char*)ts_net_ctl->GetFldValue(I7000_TS.value().CfgFldId(),tp);
  invertion = *(unsigned char*)ts_net_ctl->GetFldValue(I7000_TS.Inverted().CfgFldId(),tp);
  ts_state ^= invertion;
  return ts_state;
}
//---------------------------------------------------------------
bool I7000_Tu ::Value()const
{
  unsigned char tp;
  return (*(unsigned char*)cfg_d.GetFldValue(I7000_TU.Value().CfgFldId(),tp)!=0);
}
//---------------------------------------------------------------
unsigned char I7000_Tu ::TURetryOn()const
{
  unsigned char tp;
  return (*(unsigned char*)cfg_d.GetFldValue(I7000_TU.TURetryOn().CfgFldId(),tp)!=0);
}
//---------------------------------------------------------------
unsigned char I7000_Tu ::TURetryOff()const
{
  unsigned char tp;
  return (*(unsigned char*)cfg_d.GetFldValue(I7000_TU.TURetryOff().CfgFldId(),tp)!=0);
}
//---------------------------------------------------------------
unsigned short I7000_Tu ::TUDelayVal()const
{
  unsigned char tp;
  return *(unsigned char*)cfg_d.GetFldValue(I7000_TU.TUDelay().CfgFldId(),tp);
}
//---------------------------------------------------------------
unsigned short I7000_Tu ::TUTimeOutVal()const
{
  unsigned char tp;
  return *(unsigned char*)cfg_d.GetFldValue(I7000_TU.TUTimeOut().CfgFldId(),tp);
}
//---------------------------------------------------------------
void I7000_Tu ::SetResult(char res)
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
  cfg_d.SetFldValue(I7000_TU.result().CfgFldId(),(void*)&res,I7000_TU.result().Type());
  cfg_d.SetFldValue(I7000_TU.time().CfgFldId(),(void*)&tm,I7000_TU.time().Type());
  cfg_d.SetFldChangeBit(I7000_TU.result().CfgFldId());
  cfg_d.SetFldChangeBit(I7000_TU.time().CfgFldId());
  EventsCash.Add(cfg_d.Id());
  cfg_d.ClearFldChangeBit(I7000_TU.result().CfgFldId());
  cfg_d.ClearFldChangeBit(I7000_TU.time().CfgFldId());  
}
//---------------------------------------------------------------
void I7000_Tu ::Shutdown()
{
  SetResult(-100);
}
//----------------------------------------------------------------
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%