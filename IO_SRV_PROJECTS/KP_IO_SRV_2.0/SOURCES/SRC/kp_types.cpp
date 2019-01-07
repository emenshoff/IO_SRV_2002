#include <kp_types.h>

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
comm_dev     COMM_DEV;
kp_channel   KP_CHANNEL;
kp_dev       KP_DEV;
kp_afb_ts    KP_AFB_TS;
kp_afb_tit   KP_AFB_TIT;
kp_afb_tii   KP_AFB_TII;
kp_afb_tu    KP_AFB_TU;
kp_afb_light KP_AFB_LIGHT;
kp_ts        KP_TS;
kp_tit       KP_TIT;
kp_tii       KP_TII;
kp_titi      KP_TITi;
kp_light     KP_LIGHT;
kp_tu        KP_TU;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
comm_dev::comm_dev()
:TypeDscr("COMM_DEV")
{
  fields.reserve(4);
  FldDescription dscr[4]={
  {"PortName",	tm_string}, //имя порта
  {"PortSpeed",	tm_ulong}, //скорость порта
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
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
kp_channel::kp_channel()
:TypeDscr("KP_CHANNEL")
{
  fields.reserve(8);
  FldDescription dscr[8]={
  {"CommDev",	tm_string}, //имя объекта типа COMM_DEV
  {"Number" , tm_byte},   //номер канала  
  {"KpSpeed",	tm_uint},   //скорость гранитовского канала
  {"error",	tm_byte},     //ошибка (не доступен, ошибка установки скорости и.т.д.)
  {"recived_count",	tm_ulong},      //число принятых пакетов
  {"sended_count",	tm_ulong},      //число переданных пакетов
  {"transfer_err_count",	tm_ulong},//число ошибок передачи
  {"nonkvit_count",	tm_ulong},      //число неквитанций
  };
  FldDscr *fld;
  for(int i=0;i<8;i++)
  {
    fld=new FldDscr(dscr[i].Name,dscr[i].Type);
    fields.push_back(fld);
  }
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
kp_dev::kp_dev()
:TypeDscr("KP_DEV")
{
  fields.reserve(8);
  FldDescription dscr[40]={
  {"ChannelName",		tm_string},//канал, к которому подключен КП
  {"Address",		tm_byte}, //адрес КП на канале
  {"ErrorTimeout",  	tm_byte},//число крат от ResponceTimeout(1-10) для объявления недостоверными объектов, порождаемых из данноого функционального блока
  {"ResponceTimeout",	tm_uint},//предельный интервал ожидания посылок (сек) (диапазон допустимых значений 15 сек - 30 мин);  
  {"StartPollDelay",	tm_byte},//Задержка начала опросов КП после начала интервала ResponceTimeout - (25%-95%) значения ResponceTimeout
  {"Enabled",		tm_byte},//разрешение работы
  {"error",		tm_byte},//'Отказ КП';
  {"etime",		tm_time},//'время отказа КП';
  {"fb16_error",		tm_byte},//'Отказ субблока';
  {"fb1_error",		tm_byte},//'Отказ субблока';
  {"fb2_error",		tm_byte},//'Отказ субблока';  
  {"fb3_error",		tm_byte},//'Отказ субблока';  
  {"fb4_error",		tm_byte},//'Отказ субблока';
  {"fb5_error",		tm_byte},//'Отказ субблока';
  {"fb6_error",		tm_byte},//'Отказ субблока';
  {"fb7_error",		tm_byte},//'Отказ субблока';
  {"fb8_error",		tm_byte},//'Отказ субблока';
  {"fb9_error",		tm_byte},//'Отказ субблока';
  {"fb10_error",		tm_byte},//'Отказ субблока';
  {"fb11_error",		tm_byte},//'Отказ субблока';
  {"fb12_error",		tm_byte},//'Отказ субблока';
  {"fb13_error",		tm_byte},//'Отказ субблока';
  {"fb14_error",		tm_byte},//'Отказ субблока';
  {"fb15_error",		tm_byte},//'Отказ субблока';
  {"fb16_etime",		tm_time},//'время отказа субблока;
  {"fb1_etime",		tm_time},//'время отказа субблока;
  {"fb2_etime",		tm_time},//'время отказа субблока;
  {"fb3_etime",		tm_time},//'время отказа субблока;  
  {"fb4_etime",		tm_time},//'время отказа субблока;  
  {"fb5_etime",		tm_time},//'время отказа субблока;  
  {"fb6_etime",		tm_time},//'время отказа субблока;  
  {"fb7_etime",		tm_time},//'время отказа субблока;  
  {"fb8_etime",		tm_time},//'время отказа субблока;  
  {"fb9_etime",		tm_time},//'время отказа субблока;  
  {"fb10_etime",		tm_time},//'время отказа субблока;  
  {"fb11_etime",		tm_time},//'время отказа субблока;  
  {"fb12_etime",		tm_time},//'время отказа субблока;  
  {"fb13_etime",		tm_time},//'время отказа субблока;  
  {"fb14_etime",		tm_time},//'время отказа субблока;  
  {"fb15_etime",		tm_time},//'время отказа субблока;
  };
  FldDscr *fld;
  for(int i=0;i<40;i++)
  {
    fld=new FldDscr(dscr[i].Name,dscr[i].Type);
    fields.push_back(fld);
  }
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
kp_afb_ts::kp_afb_ts()
:TypeDscr("KP_AFB_TS")
{
  fields.reserve(6);
  FldDescription dscr[6]={
  {"Parent",    tm_string},//имя КП, в котором установлен данный субблок  
  {"ValidTime", tm_uint},//таймаут дребезга, мс
  {"Place",     tm_byte},//место в КП
  {"Enabled",   tm_byte},//разрешение работы
  {"error",     tm_byte},//'Отказ'
  {"etime",     tm_time},//'время отказа'
  };
  FldDscr *fld;
  for(int i=0;i<6;i++)
  {
    fld=new FldDscr(dscr[i].Name,dscr[i].Type);
    fields.push_back(fld);
  }
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
kp_afb_tit::kp_afb_tit()
:TypeDscr("KP_AFB_TIT")
{
  fields.reserve(5);
  FldDescription dscr[5]={
  {"Parent",    tm_string},//имя КП, в котором установлен данный субблок
  {"Place",     tm_byte},//место в КП
  {"Enabled",   tm_byte},//разрешение работы
  {"error",     tm_byte},//'Отказ'
  {"etime",     tm_time},//'время отказа'
  };
  FldDscr *fld;
  for(int i=0;i<5;i++)
  {
    fld=new FldDscr(dscr[i].Name,dscr[i].Type);
    fields.push_back(fld);
  }
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
kp_afb_tii::kp_afb_tii()
:TypeDscr("KP_AFB_TII")
{
  fields.reserve(5);
  FldDescription dscr[5]={
  {"Parent",    tm_string},//имя КП, в котором установлен данный субблок
  {"Place",     tm_byte},//место в КП
  {"Enabled",   tm_byte},//разрешение работы
  {"error",     tm_byte},//'Отказ'
  {"etime",     tm_time},//'время отказа'
  };
  FldDscr *fld;
  for(int i=0;i<5;i++)
  {
    fld=new FldDscr(dscr[i].Name,dscr[i].Type);
    fields.push_back(fld);
  }
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
kp_afb_tu::kp_afb_tu()
:TypeDscr("KP_AFB_TU")
{
  fields.reserve(6);
  FldDescription dscr[6]={
  {"Parent", tm_string},//имя КП, в котором установлен данный субблок
  {"Place",  tm_byte},//место в КП
  {"Num",    tm_byte},//номер субблока (1..2)
  {"Enabled",tm_byte},//разрешение работы
  {"error",  tm_byte},//'Отказ'
  {"etime",  tm_time},//'время отказа'
  };
  FldDscr *fld;
  for(int i=0;i<6;i++)
  {
    fld=new FldDscr(dscr[i].Name,dscr[i].Type);
    fields.push_back(fld);
  }
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
kp_afb_light::kp_afb_light()
:TypeDscr("KP_AFB_LIGHT")
{
  fields.reserve(6);
  FldDescription dscr[6]={
  {"Parent", tm_string},//имя КП, в котором установлен данный субблок
  {"Place",  tm_byte},//место в КП
  {"Type",   tm_byte,},
  {"Enabled",tm_byte},//разрешение работы
  {"error",  tm_byte},//'Отказ'
  {"etime",  tm_time},//'время отказа'
  };
  FldDscr *fld;
  for(int i=0;i<6;i++)
  {
    fld=new FldDscr(dscr[i].Name,dscr[i].Type);
    fields.push_back(fld);
  }
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
kp_ts::kp_ts()
:TypeDscr("KP_TS")
{
  fields.reserve(7);
  FldDescription dscr[7]={
  {"Parent",	tm_string},//имя объекта типа KP_AFB_TS, к которому относиться данный объект
  {"Num",	tm_byte},//номер в ФБ
  {"Enabled",	tm_byte},//признак разрешения работы
  {"Inverted",	tm_byte},//признак логической инверсии
  {"value",	tm_byte},//'текущее значение (0, 1)';
  {"error",	tm_byte},//'признак недостоверности';
  {"time",	tm_time},//'время смены значения или установления недостоверности';
  };
  FldDscr *fld;
  for(int i=0;i<7;i++)
  {
    fld=new FldDscr(dscr[i].Name,dscr[i].Type);
    fields.push_back(fld);
  }
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
kp_tit::kp_tit()
:TypeDscr("KP_TIT")
{
  fields.reserve(22);
  FldDescription dscr[22]={
   {"Parent",		tm_string},//имя АФБ, которому принадлежит данный объект
   {"Num",		tm_byte},//номер в ФБ
   {"Enabled",		tm_byte},//признак разрешения работы
   {"TITid_Name",	tm_string},//если пусто, то интегр. не считаются
   {"Jump",		tm_char},//квантовое значение "скачка" или 0: если не 0 и очередное значение ТИТ больше (или меньше) величины Jump, серверу будет отправлен признак "скачка": -1 -если вниз; +1, если вверх; а так же само значение скачка. ** Обработка "скачка" может быть заблокирована черезмерной величиной Gist.
   {"Gist",		tm_byte},//процент гистерезиса от fHi, на квантовое значение ТИТ (нечувствительность к изменению квантовых значений)
   {"Ka",		tm_float},//'смещение';
   {"Kb",		tm_float},//'множитель';
   {"LowTech",		tm_float},//'нижний  технологический предел';
   {"HighTech",		tm_float},//'верхний технологический предел';
   {"LowAlarm",		tm_float},//'нижний  аварийный предел';
   {"HighAlarm",	tm_float},//'верхний аварийный предел';
   {"max_jump",		tm_byte},//'абсолютный предел скачка'; - выходное поле
   {"Nominal",		tm_float},//'номинальное значение в именованных единицах'; 
   // выход: значения тит
   {"cvalue",		tm_byte},//'текущее значение в квантах';
   {"error",		tm_byte},//'признак недостоверности';
   {"cjump",		tm_int},//'текущее значение скачка в квантах';
   {"limit",		tm_char},//'признак выхода за предел';
    //  0 - норма,
    //  1 - верхний технологический
    //  2 - верхний аварийный,
    // -1 - нижний технологический,
    // -2 - верхний аварийный
   {"procent",		tm_float},//'текущее значение в процентах от номинального';
   {"jump",		    tm_char},//'признак выхода за предел скачка (0,-1,+1)';
   {"value",		  tm_float},//'текущее значение в именованных единицах';
   {"time",		    tm_time},//время последнего измерения, либо установки недостоверности
  };
  FldDscr *fld;
  for(int i=0;i<22;i++)
  {
    fld=new FldDscr(dscr[i].Name,dscr[i].Type);
    fields.push_back(fld);
  }
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
kp_tii::kp_tii()
:TypeDscr("KP_TII")
{
  fields.reserve(5);
  FldDescription dscr[5]={
  {"Parent",	tm_string},//имя АФБ, которому принадлежит данный объект
  {"Enabled",	tm_byte},//разрешение работы
  {"Num",	    tm_byte},//номер в АФБ
  {"error",	  tm_byte},//'Отказ'
  {"time",	  tm_time},//время замера (отказа)
  };
  FldDscr *fld;
  for(int i=0;i<5;i++)
  {
    fld=new FldDscr(dscr[i].Name,dscr[i].Type);
    fields.push_back(fld);
  }
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
kp_titi::kp_titi()
:TypeDscr("KP_TITi")
{
  fields.reserve(15);
  FldDescription dscr[15]={
  {"IntegralValLimit1",	tm_float},//'предел 1'; !!Включить проверку значений!!
  {"IntegralValLimit2",	tm_float},//'предел 2';
  {"IntegralValLimit3",	tm_float},//'предел 3';
  {"AverageValPeriod", 	tm_ulong},//'период для вычисления среднего значения в секундах'; (1,3,5,15,30,60 и.т.д)
  {"IntegratePeriod",	  tm_ulong},//'период интегрирования в секундах'; (1,3,5,15,30,60 и.т.д)
  //выход значения TITid
  {"average_val",	      tm_float},//среднее значение за период
  {"integral_val",	    tm_float},//'интегральное значение за период
  {"average_time",	    tm_time},//'время окончания вычисления среднего значения(конец периода)';
  {"integral_time",	    tm_time},//'время окончания вычисления значения(конец периода)';
  {"error",		          tm_byte},//'достоверность';
  {"avr_measures_count",tm_ulong},//'текущее число замеров'; (средн)
  {"itg_measures_count",tm_ulong},//'текущее число замеров'; (интегр)
  {"last_val",		      tm_float},//'последний замер (им. ед.)';
  {"limit",		          tm_char},//'признак выхода за предел (0, 1, 2, 3)';
  {"IntegralValDivider",tm_uint},//делитель интегрального значения (для получения результата в именованных единицах
  };
  FldDscr *fld;
  for(int i=0;i<15;i++)
  {
    fld=new FldDscr(dscr[i].Name,dscr[i].Type);
    fields.push_back(fld);
  }
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
kp_light::kp_light()
:TypeDscr("KP_LIGHT")
{
  fields.reserve(4);
  FldDescription dscr[4]={
  {"Parent",	  tm_string},//имя субблока
  {"Num",     	tm_byte},//Номер в субблоке
  {"Enabled",	  tm_byte},//признак разрешения работы
  {"Condition",	tm_string},//логическое условие
  //{"value",	    tm_byte},//значение (если не указано условие, то считается, что объект включается извне)
  };
  FldDscr *fld;
  for(int i=0;i<4;i++)
  {
    fld=new FldDscr(dscr[i].Name,dscr[i].Type);
    fields.push_back(fld);
  }
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
kp_tu::kp_tu()
:TypeDscr("KP_TU")
{
  fields.reserve(13);
  FldDescription dscr[13]={
  {"Parent",		    tm_string},//имя КП!
  {"TU_Control_TS",	tm_string},//тс подтверждения телеуправления
  {"TU_Enable_TS",	tm_string},//телесигнал разрешения ТУ
  {"GroupNum",		  tm_byte},//Номер группы (1..16)
  {"Num",		        tm_byte},//Номер в группе (1..8)
  {"Enabled",		    tm_byte},//признак разрешения работы
  {"TURetyOn",		  tm_byte},//количество попыток включения телеуправления
  {"TURetryOff",	  tm_byte},//количество попыток выключения телеуправления
  {"TUDelay",		    tm_uint},//задержка перед выдачей попыток телеуправления (c)
  {"TUTimeOut",		  tm_uint},//таймаут выполнения телеуправления  (c)
  {"Value",		      tm_byte},//1 - включить ту, 0 - выключить
  //выход
  {"result",		    tm_char},//результат выполнения ту
  // 0  - команда выполнена, телеуправление отключено
  // 1  - команда выполнена, телеуправление включено
  // -1 - ту запрещено пользователем
  // -2 - ту запрещено аппаратно
  // -3 - попытки исчерпаны
  // -4 - команда в стадии выполнения, блок занят
  // -5 - команда ту соответствует контролируемого тс-у
  // -100 - состояние не определено (начальное состояние)
  {"time",		      tm_time},//время выполнения ту или время наступления ошибки
  };
  FldDscr *fld;
  for(int i=0;i<13;i++)
  {
    fld=new FldDscr(dscr[i].Name,dscr[i].Type);
    fields.push_back(fld);
  }
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
