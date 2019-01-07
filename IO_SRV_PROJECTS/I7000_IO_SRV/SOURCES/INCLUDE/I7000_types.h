/***********************************************************
               created by M.E.N
***********************************************************/

#ifndef _I7000_types_h_
#define _I7000_types_h_

#include <io_srv_drv_types.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//Описание типа "последовательный порт пк"
class Comm_Dev_Descr :public TypeDscr
{
public:
 Comm_Dev_Descr();
 FldDscr &PortName()const{return *fields[0];}
 FldDscr &PortSpeed()const{return *fields[1];} 
 FldDscr &error()const{return *fields[2];}
 FldDscr &etime()const{return *fields[3];}
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class I7000_Ts_Descr : public TypeDscr
{
public:
 I7000_Ts_Descr();
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
class I7000_Tu_Descr : public TypeDscr
{
public:
 I7000_Tu_Descr();
 FldDscr &Parent()const       {return *fields[0];}  //имя объекта типа, к которому относится данный выход ту
 FldDscr &Num()const          {return *fields[1];}  //Номер выхода (0-12)
 FldDscr &Enabled()const      {return *fields[2];}  //Признак разрешения работы
 FldDscr &Value()const        {return *fields[3];}  //Текущее значенияеТУ
 FldDscr &TU_Control_TS()const{return *fields[4];}  //имя объекта типа I7000_TS - контроль ТУ
 FldDscr &TU_Enable_TS()const {return *fields[5];}  //имя объекта типа I7000_TS - аппаратное разрешение работы ТУ
 FldDscr &TURetryOn()const     {return *fields[6];}  //количество попыток включения телеуправления
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
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
extern Comm_Dev_Descr COMM_DEV;
extern I7000_Ts_Descr I7000_TS;
extern I7000_Tu_Descr I7000_TU;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif //I7000_types