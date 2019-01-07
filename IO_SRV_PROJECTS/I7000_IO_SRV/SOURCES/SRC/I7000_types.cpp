/***********************************************************
               created by M.E.N
***********************************************************/

#include <I7000_types.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Comm_Dev_Descr COMM_DEV;
I7000_Ts_Descr I7000_TS;
I7000_Tu_Descr I7000_TU;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Comm_Dev_Descr::Comm_Dev_Descr()
:TypeDscr("COMM_DEV")
{
  fields.reserve(4);
  FldDescription dscr[4]={
  {"PortName",	tm_string}, //имя порта
  {"PortSpeed",	tm_ulong},   //скорость порта
  {"error",	tm_byte},   //ошибка порта (не доступен, ошибка установки скорости и.т.д.)
  {"etime",	tm_time}    //время наступления ошибки
  };
  FldDscr *fld;
  for(int i=0;i<4;i++)
  {
    fld=new FldDscr(dscr[i].Name,dscr[i].Type);
    fields.push_back(fld);
  }
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
I7000_Ts_Descr::I7000_Ts_Descr()
:TypeDscr("I7000_TS")
{
  fields.reserve(7);
  FldDescription dscr[7]={
  {"Parent",	tm_string}, //имя модуля - родителя
  {"Num",	    tm_byte},   //номер в модуле
  {"Enabled",	tm_byte},   //разрешение работы
  {"Inverted",tm_byte},   //инверсия
  {"value",	  tm_byte},   //значение (состояние)
  {"time",	  tm_time},   //время изменения состояния
  {"error",	  tm_byte},   //недостоверность
  };
  FldDscr *fld;
  for(int i=0;i<7;i++)
  {
    fld=new FldDscr(dscr[i].Name,dscr[i].Type);
    fields.push_back(fld);
  }
}
//----------------------------------------------------------------
I7000_Tu_Descr::I7000_Tu_Descr()
 :TypeDscr("I7000_TU")
{
  fields.reserve(12);
  FldDescription dscr[12]={
  {"Parent",        tm_string}, //имя объекта типа I7042_DEV, к которому относится данный выход ту
  {"Num",           tm_byte}, //Номер выхода (0-12)
  {"Enabled",       tm_byte}, //Признак разрешения работы
  {"Value",         tm_byte}, //Текущее значениеТУ
  {"TU_Control_TS", tm_string}, //имя объекта типа I7000_TS - контроль ТУ
  {"TU_Enable_TS",  tm_string}, //имя объекта типа I7000_TS - аппаратное разрешение работы ТУ
  {"TURetyOn",      tm_byte}, //количество попыток включения телеуправления
  {"TURetryOff",    tm_byte}, //количество попыток выключения телеуправления
  {"TUDelay",       tm_uint}, //задержка перед выдачей попыток телеуправления (c)
  {"TUTimeOut",     tm_uint}, //таймаут выполнения телеуправления  (c)
  {"result",        tm_char}, //результат выполнения ту
  {"time",          tm_time}, //время выполнения ту или время наступления ошибки
  };
  FldDscr *fld;
  for(int i=0;i<12;i++)
  {
    fld=new FldDscr(dscr[i].Name,dscr[i].Type);
    fields.push_back(fld);
  }
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%