/***********************************************************
  Описание полей типов, необходимых для реализации 
  эмулятора пакетов КП "Гранит"
               created by M.E.N
***********************************************************/

#ifndef _kpe_types_h_
#define _kpe_types_h_

#include <io_srv_drv_types.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
enum{
  TSE  = 100,
  TITE,
  // типы ФБ
  FBE_TS,
  FBE_TIT,
  KPE_DEVICE,
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//эмуляция одного КП "Гранит"
class kpe_dev :public TypeDscr
{
public:
  kpe_dev();
 FldDscr &CnlName()const{return(*fields[0]);}//канал, к которому подключен "КП"
 FldDscr &Address()const{return (*fields[1]);}//адрес "КП" на канале
 FldDscr &Enabled()const{return (*fields[2]);}//разрешение работы
 FldDscr &NoKvit() const{return (*fields[3]);}
 FldDscr &SendDelay()const{return (*fields[4]);}
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//функциональный блок ТС (эмуляция)
class kpe_afb_ts :public TypeDscr
{
public:
 kpe_afb_ts();
 FldDscr &Parent()const{return(*fields[0]);} //имя КП, в котором установлен данный субблок 
 FldDscr &Place()const{return(*fields[1]);}  //место в КП
 FldDscr &Enabled()const{return(*fields[2]);}//разрешение работы
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//функциональный блок ТИТ (эмуляция)
class kpe_afb_tit :public TypeDscr
{
public:
 kpe_afb_tit();
 FldDscr &Parent()const{return(*fields[0]);}//имя КП, в котором установлен данный субблок
 FldDscr &Place()const{return(*fields[1]);}//место в КП
 FldDscr &Enabled()const{return(*fields[2]);}//разрешение работы
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//эмуляция ТС
class kpe_ts :public TypeDscr
{
public:
  kpe_ts();
  FldDscr &Parent()const{return(*fields[0]);}//имя объекта типа KPE_AFB_TS, к которому относиться данный объект
  FldDscr &Num()const{return(*fields[1]);}//номер в ФБ
  FldDscr &Enabled()const{return(*fields[2]);}//признак разрешения работы
  FldDscr &Value()const{return(*fields[3]);}//'текущее значение (0, 1)'
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//ТИТ (эмуляция)
class kpe_tit :public TypeDscr
{
public:
  kpe_tit();
  FldDscr &Parent()const{return(*fields[0]);}//имя АФБ, которому принадлежит данный объект
  FldDscr &Num()const{return(*fields[1]);}//номер в ФБ
  FldDscr &Enabled()const{return(*fields[2]);}//признак разрешения работы
  FldDscr &Value()const{return(*fields[3]);}//'текущее значение в квантах';
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
extern kpe_dev       KPE_DEV;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
extern kpe_afb_ts    KPE_AFB_TS;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
extern kpe_afb_tit   KPE_AFB_TIT;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
extern kpe_ts        KPE_TS;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
extern kpe_tit       KPE_TIT;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif //kpe_types