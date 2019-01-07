/***********************************************************
  Класс управления одним интегрируемым ТИ
               created by M.E.N
***********************************************************/

#ifndef _kp_titid_h_
#define _kp_titid_h_

#include <kp_types.h>
#include <io_srv_control_object.h>
//%%%%%%%%%%%%%%%%%% Телеизмерение интегрируемое %%%%%%%%%%%%%%%%%%
class TITiCtl : public ControlObject
{
 friend class KpConfig;
 friend class KpControl;
 friend class TITCtl;
public:
 TITiCtl(obj_d &d);
 void Step();                                      //шаг вычисления
 void SetValue(float val);                         //(поступило значение)
 void SetError(unsigned char err);                 //установка недостоверности
 virtual unsigned Type()const{return TITi;}
private:          
 inline void SetLastVal(float);                    //запись последнего значения  
 inline void SetValid(unsigned char);              //запись недостоверности 
 void UpdateItg();                                 //обновить значения изменившихся параметров
 void UpdateAvr();                                 //обновить значения изменившихся параметров
 inline unsigned short ItgValDiv()const;           //делитель результата инт. значения
 inline float ItgLim1()const;                     //предел1
 inline float ItgLim2()const;                     //предел2
 inline float ItgLim3()const;                     //предел3
 unsigned long itg_period;                         //период интегрирования
 unsigned long avr_val_period;                     //период вычисления среднего значения 
 unsigned long itg_measures_count;                 //текущее количество замеров (интегр)
 unsigned long avr_measures_count;                 //текущее количество замеров (средн)
 unsigned char state;                              //тек. состояние
 float itg_sum_val;                               //используется при вычислении интегр. значений
 float avr_sum_val;                               //используется при вычислении средних. значений 
 float last_val;                                   //последнее значение
 struct timeval itg_last_time;                     //время последнего замера (для интегрирования )
 time_t itg_begin_time;                            //время начала интегрирования
 time_t avr_begin_time;                            //время началa замера среднего значения
 time_t last_time;                                 //время последнего замера для интегр. ТИ 
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif //kp_titid