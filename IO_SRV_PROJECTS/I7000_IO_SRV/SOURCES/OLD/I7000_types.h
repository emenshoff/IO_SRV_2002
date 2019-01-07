/***********************************************************
               created by M.E.N
***********************************************************/

#ifndef _I7000_types_h_
#define _I7000_types_h_

#include <drv_types.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class I7000_TS : public TypeDscr
{
public:
 I7000_TS();
 FldDscr &Parent()  const{return *fields[0];} //имя объекта - устройства
 FldDscr &Num()     const{return *fields[1];} //номер входа устройства
 FldDscr &Enabled() const{return *fields[2];} //признак разрешения работы
 FldDscr &Inverted()const{return *fields[3];} //инверсия
 //выходные поля:
 FldDscr &value()   const{return *fields[4];} //текущее значение
 FldDscr &time()    const{return *fields[5];} //время изменения состояния
 FldDscr &error()   const{return *fields[6];} //недостоверность
private:
};
//----------------------------------------------------------------
class I7000_TU : public TypeDscr
{
public:
 I7000_TS();
 FldDscr &Parent()const       {return *fields[0];}  //имя объекта типа, к которому относится данный выход ту
 FldDscr &Num()const          {return *fields[1];}  //Номер выхода (0-12)
 FldDscr &Enabled()const      {return *fields[2];}  //Признак разрешения работы
 FldDscr &Value()const        {return *fields[3];}  //Текущее значенияеТУ
 FldDscr &TU_Control_TS()const{return *fields[4];}  //имя объекта типа I7000_TS - контроль ТУ
 FldDscr &TU_Enable_TS()const {return *fields[5];}  //имя объекта типа I7000_TS - аппаратное разрешение работы ТУ
 FldDscr &TURetyOn()const     {return *fields[6];}  //количество попыток включения телеуправления
 FldDscr &TURetryOff()const   {return *fields[7];}  //количество попыток выключения телеуправления
 FldDscr &TUDelay()const      {return *fields[8];}  //задержка перед выдачей попыток телеуправления (c)
 FldDscr &TUTimeOut()const    {return *fields[9];}  //таймаут выполнения телеуправления  (c)
 FldDscr &result()const       {return *fields[10];} //результат выполнения ту
  // 0  - команда выполнена, телеуправление отключено
  // 1  - команда выполнена, телеуправление включено
  // -1 - ту запрещено пользователем
  // -2 - ту запрещено аппаратно
  // -3 - попытки исчерпаны
  // -4 - команда в стадии выполнения, блок занят
  // -5 - команда ту соответствует контролируемого тс-у
  // -100 - состояние не определено (начальное состояние)
 FldDscr &time()const{return *fields[11];} //время выполнения ту или время наступления ошибки
};
//----------------------------------------------------------------
/*class I7000_TIT : public TypeDscr
{
public:
 I7000_TIT();
  FldDscr &Parent()const    {return(*fields[0]);}
  FldDscr &Num()const       {return(*fields[1]);}
  FldDscr &Enabled()const   {return(*fields[2]);}
  FldDscr &TITid_Name()const{return(*fields[3]);}
  FldDscr &Jump()const      {return(*fields[4]);}
  FldDscr &Gist()const      {return(*fields[5]);}
  FldDscr &Ka()const        {return(*fields[6]);}
  FldDscr &Kb()const        {return(*fields[7]);}
  FldDscr &LowTech()const   {return(*fields[8]);}
  FldDscr &HighTech()const  {return(*fields[9]);}
  FldDscr &LowAlarm()const  {return(*fields[10]);}
  FldDscr &HighAlarm()const {return(*fields[11]);}
  FldDscr &MaxJump()const   {return(*fields[12]);}
  FldDscr &Nominal()const   {return(*fields[13]);}
  FldDscr &cvalue()const    {return(*fields[14]);}
  FldDscr &error()const     {return(*fields[15]);}
  FldDscr &cjump()const     {return(*fields[16]);}
  FldDscr &limit()const     {return(*fields[17]);}
  FldDscr &procent()const   {return(*fields[18]);}
  FldDscr &jump()const      {return(*fields[19]);}
  FldDscr &value()const     {return(*fields[20]);}
  FldDscr &time()const      {return(*fields[21]);}
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class I7000_TITi :public TypeDscr
{
public:
  kp_titi();
    FldDscr &IntegralValLimit1()const{return(*fields[0]);}
  FldDscr &IntegralValLimit2()const{return(*fields[1]);}
  FldDscr &IntegralValLimit3()const{return(*fields[2]);}
  FldDscr &AverageValPeriod()const{return(*fields[3]);}
  FldDscr &IntegratePeriod()const{return(*fields[4]);}
  FldDscr &IntegralValDiv()const{return(*fields[14]);}
  FldDscr &average_val()const{return(*fields[5]);}
  FldDscr &integral_val()const{return(*fields[6]);}
  FldDscr &average_time()const{return(*fields[7]);}
  FldDscr &integral_time()const{return(*fields[8]);}
  FldDscr &error()const{return(*fields[9]);}
  FldDscr &avr_measures_count()const{return(*fields[10]);}
  FldDscr &itg_measures_count()const{return(*fields[11]);}
  FldDscr &last_val()const{return(*fields[12]);}
  FldDscr &limit()const{return(*fields[13]);}
};*/
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif //I7000_types