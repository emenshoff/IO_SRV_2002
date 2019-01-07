/***********************************************************
               created by M.E.N
***********************************************************/

#ifndef _i7000_functional_h_
#define _i7000_functional_h_

#include <algorithm>
#include <functional>
#include <io_srv_dump.h>
#include <I7000_cdv.h>
#include <I7000_types.h>
#include <I7000_dev_input.h>
#include <I7000_dev_output.h>

using namespace std;

#define ASSIGNED 0x1      //объект ассоциирован с остальными (определено его назначение)
#define OBJ_FAILED 0x2    //ошибка описания объекта
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//проверка корректности значений полей объекта типа COMM_DEV
bool CommDevWrongSpeed(const I7000_CommDev *d)
{
  int speed=d->Speed();
  if(
      speed != 1200 && speed != 2400 &&
      speed != 4800 && speed != 9600 &&
      speed != 19200 && speed != 38400 &&
      speed != 57600 && speed != 115200
     )     
  {
    dump.Print(D_ERR,"\tнедопустимое значение поля %s объекта %s: %d!",COMM_DEV.PortSpeed().CfgFldName(),d->Cfg_D()->Name(),speed);
    return true;
  }
  return false;
}
//----------------------------------------------------------------
bool CommDevNotOk(I7000_CommDev *d)
{
  return false;
}
//----------------------------------------------------------------
bool CommDevWrong(I7000_CommDev *d)
{
  if(d)
    if(CommDevNotOk(d) || CommDevWrongSpeed(d))
    {
      return true;
    }
  return false;
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//                    проверка на конфликты:
//----------------------------------------------------------------
//проверка на конфликты типа CommDev:
bool CommDevsEqual(I7000_CommDev * d1,I7000_CommDev * d2)
{
  if(d1!=d2)
  {
    const char *name1,*name2;
    name1 = (const char*)(*const_cast<obj_d*>(d1->Cfg_D()))[COMM_DEV.PortName().CfgFldId()];
    name2 = (const char*)(*const_cast<obj_d*>(d2->Cfg_D()))[COMM_DEV.PortName().CfgFldId()];
    if(!strcmp(name1,name2))
    {
      dump.Print(D_ERR,"\tСовпадение имен последовательных портов %s и %s!",d1->Cfg_D()->Name(),d1->Cfg_D()->Name());
      return true;
    }
  }
  return false;
}
//----------------------------------------------------------------
//проверка на конфликты типа I7000_Dev:
bool I7000DevsEqual(I7000_Dev * d1,I7000_Dev * d2)
{
  if(d1!=d2)
    if(!strcmp(d1->CommDevName(),d2->CommDevName())
      && (d1->Addr() == d2->Addr()))
    {
      dump.Print(D_ERR,"\tСовпадение портов и адресов объектов %s и %s !",d1->Cfg_D()->Name(),d2->Cfg_D()->Name());
      return true;
    }
  return false;
}
//----------------------------------------------------------------
//проверка на конфликты объектов - входов
bool I7000InEqual(I7000_Device_Input * d1,I7000_Device_Input * d2)
{
  if(d1!=d2)
    if(!strcmp(d1->Parent(),d2->Parent())
      && (d1->Num() == d2->Num()))
    {
      dump.Print(D_ERR,"\tСовпадение входов %s и %s !",d1->Cfg_D()->Name(),d2->Cfg_D()->Name());
      return true;
    }
  return false;
}
//----------------------------------------------------------------
//проверка на конфликты объектов - выходов
  bool I7000OutEqual(I7000_Device_Output * d1,I7000_Device_Output * d2)
  {
    if(d1!=d2)
      if(!strcmp(d1->Parent(),d2->Parent())
        && (d1->Num() == d2->Num()))
      {
        dump.Print(D_ERR,"\tСовпадение выходов %s и %s !",d1->Cfg_D()->Name(),d2->Cfg_D()->Name());
        return true;
      }
    return false;
  }
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//                      КЛЮЧИ ПОИСКА
//ключ поиска по имени порта
  bool CdvNameKey(I7000_Dev * d,const char *key)
  {
    if(!key)
      return false;
    if(!strlen(key))
      return false;
    const char *name=d->CommDevName();
    return (strcmp(name,key)==0);
  }
//----------------------------------------------------------------
//ключ поиска по имени устройства:
  bool DevNameKey(I7000_Device_Item *d,const char *key)
  {
    if(!key)
      return false;
    if(!strlen(key))
      return false;
    const char *name=d->Parent();
    return (strcmp(name,key)==0);
  }
//----------------------------------------------------------------
//ключ поиска по имени ТС:
struct ItemNameKey : binary_function<pair<I7000_Device_Item *,unsigned char>,const char *,bool>
{
  bool operator()(pair<I7000_Device_Item *,unsigned char> d,const char *key)const
  {
    if(!key)
      return false;
    if(!strlen(key))
      return false;
    const char *name=d.first->Cfg_D()->Name();
    return (strcmp(name,key)==0);
  }
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif// _i7000_functional_h_
