/***********************************************************
  проверка на ошибки и конфликты для эмулируемых объектов
               created by M.E.N
***********************************************************/

#ifndef _kpe_fn_h_
#define _kpe_fn_h_
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#include <kpe_dev.h>
#include <kpe_fb_ts.h>
#include <kpe_fb_tit.h>
#include <kpe_types.h>
#include <io_srv_dump.h>

#ifndef ASSIGNED
#define ASSIGNED 0x1      //объект ассоциирован с остальными (определено его назначение)
#endif

#ifndef OBJ_FAILED
#define OBJ_FAILED 0x2    //ошибка описания объекта
#endif
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//проверка на ошибки типа KPE_DEV:
bool KpeDevNumWrong(KpeDev *d)
{
  if(d->Address() > 127)
  {
    dump.Print(D_ERR,"\tНедопустимое значение поля %s объекта %s : %d!",KPE_DEV.Address().CfgFldName(),d->Cfg_D()->Name(),d->Address());
    return true;
  }
  return false;
}
//----------------------------------------------------------------
//полная проверка
bool KpeWrong(KpeDev *d)
{
  if(d)
    if(KpeNumWrong(d))
    {
      return true;
    }
  return false;
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//                  проверка субблоков (общая)
//недопустимое посадочное место ФБ
bool FbePlaceWrong(KpeAfb *d)
{
  unsigned place=d->Place();
  if(place >16 || place < 1)
  {
    dump.Print(D_ERR,"\tНедопустимое значение поля Place объекта %s : %u!",d->Cfg_D()->Name(),place);
    return true;
  }
  return false;
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//            Проверка ТС-ов      
//проверка номера ТС-а в субблоке
bool TseNumWrong(KpeTs *d)
{
  unsigned num=d->Num();
  if(num > 64 || num <1)
  {
    dump.Print(D_ERR,"\tНедопустимое значение поля %s объекта %s : %d!",KPE_TS.Num().CfgFldName(),d->Cfg_D()->Name(),num);
    return true;
  }
  return false;
}
//----------------------------------------------------------------
//проверка ТС-а
bool TseWrong(KpeTs *d)
{
 if(d)
  if(TseNumWrong()(d))
  {
    return true;
  }
  return false;
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//            проверка  ТИТ-а
//проверка номера ТИТ-а в субблоке
bool TiteNumWrong(KpeTit *d)
{
  unsigned char num = d->Num();
  if(num > 32 || num <1)
  {
    dump.Print(D_ERR,"\tНедопустимое значение поля %s объекта %s: %d!",KPE_TIT.Num().CfgFldName(),d->Cfg_D()->Name(),num);
    return true;
  }
  return false;
}
//----------------------------------------------------------------
bool TiteWrong(KpeTit *d)
{
 if(d)
  if(TiteNumWrong()(d))
  {
    return true;
  }
  return false;
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//проверка на конфликты типа KpDev:
bool KpsEqual(KpeDev * d1,KpeDev * d2)
{    
  if(d1!=d2)
  {
    const char *cdv1,*cdv2;
    unsigned char tp;
    cdv1=(const char*)d1->Cfg_D()->GetFldValue(KPE_DEV.CnlName().CfgFldId(),tp);
    cdv2=(const char*)d2->Cfg_D()->GetFldValue(KPE_DEV.CnlName().CfgFldId(),tp);
    if(d1->Address() == d2->Address() && !strcmp(cdv1,cdv2))
    {
      dump.Print(D_ERR,"\tСовпадение адресов двух КП: %s и %s !",d1->Cfg_D()->Name(),d2->Cfg_D()->Name());
      return true;
    }
  }
  return false;
}
//----------------------------------------------------------------
//конфликт ФБ
bool FbsEqual(KpeAfb *d1,KpeAfb *d2)
{
  if(d1!=d2)
    if(strcmp(d1->Parent(),d2->Parent())==0)
      if(d1->Place() == d2->Place())
      {
        dump.Print(D_ERR,"\tСовпадение мест двух АФБ: %s и %s : %d!",d1->Cfg_D()->Name(),d2->Cfg_D()->Name(),d1->Place());
        return true;
      }
  return false;
}
//----------------------------------------------------------------
//проверка на совпадение ТС-ов
bool TseEqual(KpeTs *d1,KpeTs *d2)
{
 if(d1!=d2)
  if(d1->Num() == d2->Num() && !strcmp(d1->Parent(),d2->Parent()))
  {
    dump.Print(D_ERR,"\tСовпадение номеров ТC в одном субблоке: %s и %s !",d1->Cfg_D()->Name(),d2->Cfg_D()->Name());
    return true;
  }
  return false;
}
//----------------------------------------------------------------
//Проверка на совпадение номеров ТИТ-ов в одном субблоке
bool TitsEqual(KpeTit * d1,KpeTit * d2)
{
 if(d1!=d2)
  if(d1->Num() == d2->Num() && !strcmp(d1->Parent(),d2->Parent()))
  {
    dump.Print(D_ERR,"\tСовпадение номеров ТИТ в одном субблоке: %s и %s !",d1->Cfg_D()->Name(),d2->Cfg_D()->Name());
    return true;
  }
  return false;
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif //kpe_fn