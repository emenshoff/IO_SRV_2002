/********************************************************************
		    Классы описания типов "ГРАНИТА" и их полей.
                     created by M.E.N.
********************************************************************/
//Если задумаешь что - то менять, обрати внимание на макросы внизу (порядок полей очень важен!)
#ifndef _kp_types_h
#define _kp_types_h

#include <io_srv_drv_types.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//типы (для RTTI :)  )
enum{
  TS  = 1,
  TIT,
  TITi,
  TII,
  TU,
  LIGHT,
  // типы ФБ
  FB_TS,
  FB_TIT,
  FB_TII,
  FB_TU,
  FB_LIGHT,
  KP_DEVICE,
  KP_COMM_DEV,
  KP_CNL
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//Описание типа "последовательный порт пк"
class comm_dev :public TypeDscr
{
public:
 comm_dev();
 FldDscr &PortName()const{return *fields[0];}
 FldDscr &PortSpeed()const{return *fields[1];} 
 FldDscr &error()const{return *fields[2];}
 FldDscr &etime()const{return *fields[3];}
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//Описание типа "канал КПМ "Гранит" ( адаптер канала )"
class kp_channel :public TypeDscr
{
public:
 kp_channel();
 FldDscr &CommDev()const{return *fields[0];} 
 FldDscr &Number()const{return *fields[1];}
 FldDscr &KpSpeed()const{return *fields[2];}
 FldDscr &error()const{return *fields[3];}
 FldDscr &recived_count()const{return *fields[4];}
 FldDscr &sended_count()const{return *fields[5];}
 FldDscr &transfer_err_count()const{return *fields[6];}
 FldDscr &nonkvit_count()const{return *fields[7];}
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//один КП "Гранит"
class kp_dev :public TypeDscr
{
public:
  kp_dev();
 FldDscr &CnlName()const{return(*fields[0]);}
 FldDscr &Address()const{return (*fields[1]);} 
 FldDscr &ErrorTimeout()const{return (*fields[2]);}
 FldDscr &ResponceTimeout()const{return (*fields[3]);}
 FldDscr &StartPoll()const{return (*fields[4]);}
 FldDscr &Enabled()const{return (*fields[5]);}
 FldDscr &error()const{return (*fields[6]);}
 FldDscr &etime()const{return (*fields[7]);}
 //субблок с индексом 0 - в КП есть субблок на месте 16
 FldDscr &fb_error(unsigned i)const{return (*fields[8+i]);}
 FldDscr &fb_etime(unsigned i)const{return (*fields[24+i]);}
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//функциональный блок ТС
class kp_afb_ts :public TypeDscr
{
public:
 kp_afb_ts();
 FldDscr &Parent()const{return(*fields[0]);}
 FldDscr &ValidTime()const{return (*fields[1]);}
 FldDscr &Place()const{return(*fields[2]);}
 FldDscr &Enabled()const{return(*fields[3]);}
 FldDscr &error()const{return(*fields[4]);}
 FldDscr &etime()const{return(*fields[5]);}
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//функциональный блок ТИТ
class kp_afb_tit :public TypeDscr
{
public:
 kp_afb_tit();
 FldDscr &Parent()const{return(*fields[0]);}
 FldDscr &Place()const{return(*fields[1]);}
 FldDscr &Enabled()const{return(*fields[2]);}
 FldDscr &error()const{return(*fields[3]);}
 FldDscr &etime()const{return(*fields[4]);}
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//функциональный блок интегральные ТИ
class kp_afb_tii :public TypeDscr
{
public:
 kp_afb_tii();
 FldDscr &Parent()const{return(*fields[0]);}
 FldDscr &Place()const{return(*fields[1]);}
 FldDscr &Enabled()const{return(*fields[2]);}
 FldDscr &error()const{return(*fields[3]);}   
 FldDscr &etime()const{return(*fields[4]);}
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//функциональный блок телеуправления (точнее,
//KP_AFB_TU - информация о всех блоках ту (до 4 штук) данного кп
class kp_afb_tu :public TypeDscr
{
public:
 kp_afb_tu();
 FldDscr &Parent()const{return(*fields[0]);}
 FldDscr &Place()const{return(*fields[1]);}
 FldDscr &Num()const{return(*fields[2]);}
 FldDscr &Enabled()const{return(*fields[3]);}
 FldDscr &error()const{return(*fields[4]);}
 FldDscr &etime()const{return(*fields[5]);}
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//описание субблока вывода на щит
class kp_afb_light :public TypeDscr
{
public:
 kp_afb_light();
 FldDscr &Parent()const{return(*fields[0]);}
 FldDscr &Place()const{return(*fields[1]);}
 FldDscr &Type()const{return(*fields[2]);} 
 FldDscr &Enabled()const{return(*fields[3]);}
 FldDscr &error()const{return(*fields[4]);}
 FldDscr &etime()const{return(*fields[5]);}
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

class kp_ts :public TypeDscr
{
public:
  kp_ts();
  FldDscr &Parent()const{return(*fields[0]);}
  FldDscr &Num()const{return(*fields[1]);}
  FldDscr &Enabled()const{return(*fields[2]);}
  FldDscr &Inverted()const{return(*fields[3]);}
  FldDscr &value()const{return(*fields[4]);}
  FldDscr &error()const{return(*fields[5]);}
  FldDscr &time()const{return(*fields[6]);}
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class kp_tit :public TypeDscr
{
public:
  kp_tit();
  FldDscr &Parent()const{return(*fields[0]);}
  FldDscr &Num()const{return(*fields[1]);}
  FldDscr &Enabled()const{return(*fields[2]);}
  FldDscr &TITid_Name()const{return(*fields[3]);}
  FldDscr &Jump()const{return(*fields[4]);}
  FldDscr &Gist()const{return(*fields[5]);}
  FldDscr &Ka()const{return(*fields[6]);}
  FldDscr &Kb()const{return(*fields[7]);}
  FldDscr &LowTech()const{return(*fields[8]);}
  FldDscr &HighTech()const{return(*fields[9]);}
  FldDscr &LowAlarm()const{return(*fields[10]);}
  FldDscr &HighAlarm()const{return(*fields[11]);}
  FldDscr &MaxJump()const{return(*fields[12]);}
  FldDscr &Nominal()const{return(*fields[13]);}
  FldDscr &cvalue()const{return(*fields[14]);}
  FldDscr &error()const{return(*fields[15]);}
  FldDscr &cjump()const{return(*fields[16]);}
  FldDscr &limit()const{return(*fields[17]);}
  FldDscr &procent()const{return(*fields[18]);}
  FldDscr &jump()const{return(*fields[19]);}
  FldDscr &value()const{return(*fields[20]);}
  FldDscr &time()const{return(*fields[21]);}
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class kp_tii :public TypeDscr
{
public:
  kp_tii();
  FldDscr &Parent()const{return(*fields[0]);}
  FldDscr &Enabled()const{return(*fields[1]);}
  FldDscr &Num()const{return(*fields[2]);}
  FldDscr &error()const{return(*fields[3]);}
  FldDscr &time()const{return(*fields[4]);}
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class kp_titi :public TypeDscr
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
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class kp_light :public TypeDscr
{
public:
  kp_light();
    FldDscr &Parent()const{return(*fields[0]);}
  FldDscr &Num()const{return(*fields[1]);}
  FldDscr &Enabled()const{return(*fields[2]);}
  FldDscr &Condition()const{return(*fields[3]);}
  FldDscr &value()const{return(*fields[4]);}
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class kp_tu :public TypeDscr
{
public:
  kp_tu();
    FldDscr &Parent()const{return(*fields[0]);}
  FldDscr &TU_Confirm_TS()const{return(*fields[1]);}
  FldDscr &TU_Enable_TS()const{return(*fields[2]);}
  FldDscr &GroupNum()const{return(*fields[3]);}
  FldDscr &Num()const{return(*fields[4]);}
  FldDscr &Enabled()const{return(*fields[5]);}
  FldDscr &TURetryOn()const{return(*fields[6]);}
  FldDscr &TURetryOff()const{return(*fields[7]);}
  FldDscr &TUDelay()const{return(*fields[8]);}
  FldDscr &TUTimeOut()const{return(*fields[9]);}
  FldDscr &Value()const{return(*fields[10]);}
  FldDscr &result()const{return(*fields[11]);}
  FldDscr &time()const{return(*fields[12]);}
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
extern comm_dev     COMM_DEV;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
extern kp_channel   KP_CHANNEL;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
extern kp_dev       KP_DEV;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
extern kp_afb_ts    KP_AFB_TS;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
extern kp_afb_tit   KP_AFB_TIT;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
extern kp_afb_tii   KP_AFB_TII;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
extern kp_afb_tu    KP_AFB_TU;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
extern kp_afb_light KP_AFB_LIGHT;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
extern kp_ts        KP_TS;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
extern kp_tit       KP_TIT;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
extern kp_tii       KP_TII;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
extern kp_titi      KP_TITi;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
extern kp_light     KP_LIGHT;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
extern kp_tu        KP_TU;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif