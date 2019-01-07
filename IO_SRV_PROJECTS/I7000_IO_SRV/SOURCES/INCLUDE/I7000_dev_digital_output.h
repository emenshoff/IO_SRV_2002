/***********************************************************
  Класс "Цифровой выход модуля семейства I7000"
               created by M.E.N
***********************************************************/

#ifndef _I7000_dev_digital_output_h_
#define _I7000_dev_digital_output_h_

#include <I7000_dev_output.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class I7000_DigitalOutput : public I7000_Device_Output
{
public:
 explicit I7000_DigitalOutput(obj_d &d):I7000_Device_Output(d){}
 virtual ~I7000_DigitalOutput(){}
 virtual bool Enabled()const{return false;}
protected:
};
//----------------------------------------------------------------
//телеуправление
class I7000_Tu : public I7000_DigitalOutput
{
 friend class I7000_Config;
public:
 explicit I7000_Tu(obj_d &d);
 virtual ~I7000_Tu();
 void SetError(unsigned char);  //установка недостоверности
 void SetVal(unsigned char val);//установка значения (при отсутствии тс контроля)
 unsigned Num()const;           //номер выхода в устройстве
 unsigned char Step();          //если возвращает "TU_STEP", то нет телеуправления, "TU_ON" - включить, "TU_OFF" - выключить 
 unsigned char LastVal()const{return last_val;}//тек. состояние
 const char *Parent()const;     
 const char *CtlTsName()const;
 const char *EnTsName()const;
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
private: inline bool Enabled()const;             //признак разрешения работы
 inline bool Timeout()const;             //таймаут выполнения телеуправления истек
 inline bool DelayComplete()const;       //задержка перед выдачей ту отработана 
 inline bool Value()const;               //проверяет, есть ли команда на выключение  (1 - on; 0 - off) 
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
#endif //I7000_dev_digital_output