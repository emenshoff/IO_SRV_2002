/***********************************************************
               created by M.E.N
***********************************************************/

#include <kp_types.h>
#include <kp_titi.h>
#include <time_utils.h>
#include <io_srv_dump.h>
#include <kp_events_cash.h>

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
TITiCtl::TITiCtl(obj_d &d)
  :ControlObject(d)
{   
  state=0;
  last_val=0;   
  UpdateItg();
  UpdateAvr();
  SetError(255);
}
//---------------------------------------------------------------
#define ITG_STEP  0x1   //флаг вычисления инт. значения
#define AVR_STEP  0x2   //флаг вычисления ср. значения
void TITiCtl::Step()
{
  struct timeval tv;
  GetCurrTime(tv);
  time_t tm=time(0);
  double curr_time = EventsCash.TimeInDbl();
  if(state & AVR_STEP)
  {
    avr_sum_val+=last_val;
    avr_measures_count++;    
    if(tm-avr_begin_time >= avr_val_period) //период вычисления ср. значения закончен
    {
      avr_sum_val /= avr_measures_count;  //результат готов
      //записываем число замеров:
      cfg_d.SetFldValue(KP_TITi.avr_measures_count().CfgFldId(),(void*)&avr_measures_count,KP_TITi.avr_measures_count().Type());      
      //записываем значение и время
      float val = (float)avr_sum_val;
      cfg_d.SetFldValue(KP_TITi.average_val().CfgFldId(),(void*)&val,KP_TITi.average_val().Type());
      cfg_d.SetFldValue(KP_TITi.average_time().CfgFldId(),(void*)&curr_time,KP_TITi.average_time().Type());      
      UpdateAvr();                        //если период изменился, обновим значение
      dump.ObjMsg(cfg_d.Id(),D_MSG | D_SENSOR,"%s: изменение значений:",cfg_d.Name());
      avr_sum_val=0;
      avr_measures_count=0;
      avr_begin_time=tm;
      cfg_d.SetFldChangeBit(KP_TITi.average_val().CfgFldId());
      cfg_d.SetFldChangeBit(KP_TITi.average_time().CfgFldId());
      cfg_d.SetFldChangeBit(KP_TITi.avr_measures_count().CfgFldId());
      EventsCash.Add(cfg_d.Id());
      cfg_d.ClearFldChangeBit(KP_TITi.average_val().CfgFldId());
      cfg_d.ClearFldChangeBit(KP_TITi.average_time().CfgFldId());
      cfg_d.ClearFldChangeBit(KP_TITi.avr_measures_count().CfgFldId());
    }
  }
  else    //это сработает в начале работы
  {
    //записываем значение и время
    cfg_d.SetFldValue(KP_TITi.average_val().CfgFldId(),(void*)&last_val,KP_TITi.average_val().Type());
    cfg_d.SetFldValue(KP_TITi.average_time().CfgFldId(),(void*)&curr_time,KP_TITi.average_time().Type());    
    state |= AVR_STEP;
    avr_begin_time=0;
    avr_sum_val=0;
    avr_measures_count=0;
    cfg_d.SetFldChangeBit(KP_TITi.average_val().CfgFldId());
    cfg_d.SetFldChangeBit(KP_TITi.average_time().CfgFldId());
    EventsCash.Add(cfg_d.Id());
    cfg_d.ClearFldChangeBit(KP_TITi.average_val().CfgFldId());
    cfg_d.ClearFldChangeBit(KP_TITi.average_time().CfgFldId());
  }
  if(state & ITG_STEP)
  {
    if(tm - itg_begin_time >= itg_period) //время периода интегрирования закончилось
    {
      unsigned short div=ItgValDiv();
      itg_sum_val /= (div==0 ? 1 : div);
      //запись интегрального значения 
      cfg_d.SetFldValue(KP_TITi.integral_val().CfgFldId(),(void*)&itg_sum_val,KP_TITi.integral_val().Type());
      cfg_d.SetFldValue(KP_TITi.integral_time().CfgFldId(),(void*)&curr_time,KP_TITi.integral_time().Type());      
      dump.ObjMsg(cfg_d.Id(),D_MSG | D_SENSOR,"%s: изменение значений:",cfg_d.Name());
      //Logdef(Print(DEBUG_FULL,"Объект %s: интегральное значение за период %u сек. : %f. Число замеров: %u",cfg_d.Name(),itg_period,itg_sum_val,itg_measures_count))
      //запись числа замеров
      cfg_d.SetFldValue(KP_TITi.itg_measures_count().CfgFldId(),(void*)&itg_measures_count,KP_TITi.itg_measures_count().Type());      
      UpdateItg();                        //если период изменился, обновим значение      
      //запись признака выхода за предел
      char lm;
      if(itg_sum_val > ItgLim3())
      {
        //Log
        lm = 3;
      }
      else if(itg_sum_val > ItgLim2())
      {
        //Log
        lm=2;
      }
      else if(itg_sum_val > ItgLim1())
      {
        //Log
        lm=1;
      }
      else
        lm=0;
      cfg_d.SetFldValue(KP_TITi.limit().CfgFldId(),(void*)&lm,KP_TITi.limit().Type());
      itg_sum_val=0;
      itg_measures_count=0;
      itg_begin_time=tm;
      cfg_d.SetFldChangeBit(KP_TITi.itg_measures_count().CfgFldId());        
      cfg_d.SetFldChangeBit(KP_TITi.limit().CfgFldId());
      cfg_d.SetFldChangeBit(KP_TITi.integral_val().CfgFldId());
      cfg_d.SetFldChangeBit(KP_TITi.integral_time().CfgFldId());  
      EventsCash.Add(cfg_d.Id());
      cfg_d.ClearFldChangeBit(KP_TITi.limit().CfgFldId());
      cfg_d.ClearFldChangeBit(KP_TITi.integral_val().CfgFldId());
      cfg_d.ClearFldChangeBit(KP_TITi.integral_time().CfgFldId());  
      cfg_d.ClearFldChangeBit(KP_TITi.itg_measures_count().CfgFldId());  
    }
    else  //считаем 
    {
      struct timeval diff_of_time;
      diff_of_time=tv - itg_last_time;
      itg_sum_val += last_val * (diff_of_time.tv_sec + diff_of_time.tv_usec * 1e-6);
      itg_measures_count++;
      itg_last_time=tv;       //обновили время текущего замера!!!!!!!!
    }    
  }
  else
  {
    state |= ITG_STEP;
    float bg_val = last_val * itg_period;
    cfg_d.SetFldValue(KP_TITi.integral_val().CfgFldId(),(void*)&bg_val,KP_TITi.integral_val().Type());
    cfg_d.SetFldValue(KP_TITi.integral_time().CfgFldId(),(void*)&curr_time,KP_TITi.integral_time().Type());    
    itg_begin_time=time(0);
    itg_last_time=tv;
    memset((void*)&itg_last_time,0,sizeof(itg_last_time));
    itg_sum_val=0;
    itg_measures_count=0;
    cfg_d.SetFldChangeBit(KP_TITi.integral_val().CfgFldId());
    cfg_d.SetFldChangeBit(KP_TITi.integral_time().CfgFldId());  
    EventsCash.Add(cfg_d.Id());
    cfg_d.ClearFldChangeBit(KP_TITi.integral_val().CfgFldId());
    cfg_d.ClearFldChangeBit(KP_TITi.integral_time().CfgFldId());  
  }  
}
#undef ITG_STEP
#undef AVR_STEP
//---------------------------------------------------------------
void TITiCtl::SetValue(float fval)
{
  SetLastVal(last_val=fval);
  Step();
}
//---------------------------------------------------------------
void TITiCtl::SetError(unsigned char err)
{
  SetValid(err);
  if(err)
    state = 0;
}
//---------------------------------------------------------------
void TITiCtl::SetLastVal(float val)
{
  cfg_d.SetFldValue(KP_TITi.last_val().CfgFldId(),(void*)&val,KP_TITi.last_val().Type());
  cfg_d.SetFldChangeBit(KP_TITi.last_val().CfgFldId());
  EventsCash.Add(cfg_d.Id());
  cfg_d.ClearFldChangeBit(KP_TITi.last_val().CfgFldId());
}
//---------------------------------------------------------------
void TITiCtl::SetValid(unsigned char val)
{
  cfg_d[KP_TITi.error().CfgFldId()] = val;
  cfg_d.SetFldChangeBit(KP_TITi.error().CfgFldId());
  EventsCash.Add(cfg_d.Id());
  cfg_d.ClearFldChangeBit(KP_TITi.error().CfgFldId());
}
//---------------------------------------------------------------
void TITiCtl::UpdateItg()
{
  unsigned char tp;
  unsigned long new_val = *(unsigned long*)cfg_d.GetFldValue(KP_TITi.IntegratePeriod().CfgFldId(),tp);
  itg_period=new_val;
}
//---------------------------------------------------------------
void TITiCtl::UpdateAvr()
{
  unsigned char tp;
  unsigned long new_val = *(unsigned long*)cfg_d.GetFldValue(KP_TITi.AverageValPeriod().CfgFldId(),tp);
  avr_val_period=new_val;
}
//---------------------------------------------------------------
float TITiCtl::ItgLim1()const
{
  unsigned char tp;
  return *(float*)cfg_d.GetFldValue(KP_TITi.IntegralValLimit1().CfgFldId(),tp);
}
//---------------------------------------------------------------
float TITiCtl::ItgLim2()const
{
  unsigned char tp;
  return *(float*)cfg_d.GetFldValue(KP_TITi.IntegralValLimit2().CfgFldId(),tp);
}
//---------------------------------------------------------------
float TITiCtl::ItgLim3()const
{
  unsigned char tp;
  return *(float*)cfg_d.GetFldValue(KP_TITi.IntegralValLimit3().CfgFldId(),tp);
}
//---------------------------------------------------------------
unsigned short TITiCtl::ItgValDiv()const
{
  unsigned char tp;
  return *(unsigned short*)cfg_d.GetFldValue(KP_TITi.IntegralValDiv().CfgFldId(),tp);
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%