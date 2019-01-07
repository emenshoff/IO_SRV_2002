/***********************************************************
               created by M.E.N
***********************************************************/

#include <kpe_types.h>

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
kpe_dev       KPE_DEV;
kpe_afb_ts    KPE_AFB_TS;
kpe_afb_tit   KPE_AFB_TIT;
kpe_ts        KPE_TS;
kpe_tit       KPE_TIT;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
kpe_dev::kpe_dev()
:TypeDscr("KPE_DEV")
{
  fields.reserve(3);
  FldDescription dscr[3]={
  {"ChannelName",		tm_string},//канал, к которому подключен "КП"
  {"Address",		tm_byte},      //адрес КП на канале  
  {"Enabled",		tm_byte},      //разрешение работы
  };
  FldDscr *fld;
  for(int i=0;i<3;i++)
  {
    fld=new FldDscr(dscr[i].Name,dscr[i].Type);
    fields.push_back(fld);
  }
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
kpe_afb_ts::kpe_afb_ts()
:TypeDscr("KPE_AFB_TS")
{
  fields.reserve(3);
  FldDescription dscr[3]={
  {"Parent",    tm_string},//имя КП, в котором установлен данный субблок  
  {"Place",     tm_byte},//место в КП
  {"Enabled",   tm_byte},//разрешение работы
  };
  FldDscr *fld;
  for(int i=0;i<3;i++)
  {
    fld=new FldDscr(dscr[i].Name,dscr[i].Type);
    fields.push_back(fld);
  }
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
kpe_afb_tit::kpe_afb_tit()
:TypeDscr("KPE_AFB_TIT")
{
  fields.reserve(3);
  FldDescription dscr[3]={
  {"Parent",    tm_string},//имя КП, в котором установлен данный субблок
  {"Place",     tm_byte},//место в КП
  {"Enabled",   tm_byte},//разрешение работы
  };
  FldDscr *fld;
  for(int i=0;i<3;i++)
  {
    fld=new FldDscr(dscr[i].Name,dscr[i].Type);
    fields.push_back(fld);
  }
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
kpe_ts::kpe_ts()
:TypeDscr("KPE_TS")
{
  fields.reserve(4);
  FldDescription dscr[4]={
  {"Parent",	tm_string},//имя объекта типа KPE_AFB_TS, к которому относиться данный объект
  {"Num",	    tm_byte},//номер в ФБ
  {"Enabled",	tm_byte},//признак разрешения работы
  {"Value",	  tm_byte},//'текущее значение (0, 1)'
  };
  FldDscr *fld;
  for(int i=0;i<4;i++)
  {
    fld=new FldDscr(dscr[i].Name,dscr[i].Type);
    fields.push_back(fld);
  }
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
kpe_tit::kpe_tit()
:TypeDscr("KPE_TIT")
{
  fields.reserve(4);
  FldDescription dscr[4]={
   {"Parent",		tm_string},//имя АФБ, которому принадлежит данный объект
   {"Num",		tm_byte},//номер в ФБ
   {"Enabled",		tm_byte},//признак разрешения работы
   {"Value",		tm_byte},//'текущее значение в квантах';
  };
  FldDscr *fld;
  for(int i=0;i<4;i++)
  {
    fld=new FldDscr(dscr[i].Name,dscr[i].Type);
    fields.push_back(fld);
  }
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%