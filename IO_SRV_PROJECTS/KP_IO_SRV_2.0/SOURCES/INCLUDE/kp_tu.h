/***********************************************************
  Класс объекта - телеуправления.
               created by M.E.N
***********************************************************/

#ifndef _kp_tu_h_
#define _kp_tu_h_

#include <kp_types.h>
#include <io_srv_control_object.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%% Телеуправление %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
/*
Автомат считает, что телеуправление произошло, если состояние тс-а
 контроля ту изменилось после подачи команды
*/
class TUCtl : public ControlObject
{
 friend class KpConfig;
public:
 explicit TUCtl(obj_d &d);
 virtual ~TUCtl();
 void SetError(unsigned char);  //установка недостоверности
 void SetVal(unsigned char val);//установка значения (при отсутствии тс контроля)
 unsigned Num()const;           //Номер в группе
 unsigned char Step();          //если возвращает "TU_STEP", то нет телеуправления, "TU_ON" - включить, "TU_OFF" - выключить 
 unsigned char LastVal()const{return last_val;}//тек. состояние
 unsigned char GroupNum()const;   //номер группы 
 const char *Parent()const;
 const char *CtlTsName()const;
 const char *EnTsName()const;
 virtual unsigned Type()const{return TU;}
 enum{  
  //эти значения возвращает автомат при вызове функции Step() :
  TU_STEP=0,  //шаг работы
  TU_ON,      //подать команду на включение ту
  TU_OFF,     //на выключение
  //состояния конечного автомата :
  TU_TRY_ON,  //выполнение попытки включить телеуправление
  TU_TRY_OFF, //выполнение попытки выключить телеуправление  
  TU_DELAY_ON,//отработка задержки перед выдачей ту
  TU_DELAY_OFF
 };
private:
 inline bool Enabled()const;             //признак разрешения работы
 inline bool Timeout()const;             //таймаут выполнения телеуправления истек
 inline bool DelayComplete()const;       //задержка перед выдачей ту отработана 
 inline unsigned char Value()const;               //проверяет, есть ли команда на выключение  (1 - on; 0 - off) 
 inline unsigned char CtlTsState()const; //состояние телесигнала подтверждения выполнения тс
 inline unsigned char NetTsState()const; //если true, то можно слать ту (цепь исправна) 
 inline unsigned char TURetryOn()const;  //количество попыток включения телеуправления
 inline unsigned char TURetryOff()const; //количество попыток выключения телеуправления
 inline unsigned short TUDelayVal()const;//задержка перед выдачей телеуправления (c)
 inline unsigned short TUTimeOutVal()const; //таймаут выполнения телеуправления (c) 
 inline void SetResult(char);            //запись результата выполнения команды
 void Shutdown();                        //завершение работы (важная штука!)
 const obj_d *ts_ctl;                    //контролирующий тс
 const obj_d *ts_net_ctl;                //тс контроля цепи
 unsigned char try_count;                //текущий счетчик попыток включения/выключения телеуправления
 unsigned char state;                    //текущее состояние конечного автомата
 unsigned char last_val;                 //если в 1, то в настоящий момент ту включено,если в 0 - то включено 
 time_t tu_time;                         //время последнего изменения состояния(значения) (исп. также при отработке задержек) 
 time_t delay_begin_time;                //время начала отработки задержки
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif //kp_tu