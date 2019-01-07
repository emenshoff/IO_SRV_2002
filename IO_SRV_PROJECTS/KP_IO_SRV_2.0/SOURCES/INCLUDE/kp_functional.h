/********************************************************************
	Классы для проверки конфигурации на наличие необходимых типов, 
        правильность описания их полей, наличие конфликтов.
                     created by M.E.N.
********************************************************************/
#ifndef _kp_functional_h
#define _kp_functional_h


#include <kp_dev.h>
#include <kp_fb_ts.h>
#include <kp_fb_tit.h>
#include <kp_fb_tii.h>
#include <kp_fb_tu.h>
#include <kp_fb_light.h>
#include <io_srv_dump.h>

#define ASSIGNED 0x1      //объект ассоциирован с остальными (определено его назначение)
#define OBJ_FAILED 0x2    //ошибка описания объекта
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//проверка на ошибки типа KpCommDev:
struct CommDevWrongSpeed : unary_function<const KpCommDev *,bool>
{
  bool operator()(const KpCommDev *d)const
  {
    /*int speed=d->Speed();
    if(
        speed != 1200 && speed != 2400 &&
        speed != 4800 && speed != 9600 &&
        speed != 19200 && speed != 38400
       )     
    {
      dump.Print(D_ERR,"\tнедопустимое значение поля %s объекта %s: %d!",COMM_DEV.PortSpeed().CfgFldName(),d->Cfg_D()->Name(),speed);
      return true;
    }*/
    return false;
  }
};
//----------------------------------------------------------------
struct CommDevNotOk : unary_function<KpCommDev *,bool>
{
  bool operator()(KpCommDev *d)const
  {
    return false;
  }
};
//----------------------------------------------------------------
struct CommDevWrong : unary_function<KpCommDev *,bool>
{
  bool operator()(KpCommDev *d)const
  {
    if(d)
      if(CommDevNotOk()(d) || CommDevWrongSpeed()(d))
      {
        return true;
      }
    return false;
  }
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//проверка на ошибки типа KP_CHANNEL:
struct KpCnlNumWrong : unary_function<KpCnl *,bool>
{
  bool operator()(KpCnl *d)const
  {
    if(d->Num() > 3)
    {
      dump.Print(D_ERR,"\tНедопустимое значение поля %s объекта %s :%d!",KP_CHANNEL.Number().CfgFldName(),d->Cfg_D()->Name(),d->Num());
      return true;
    }
    return false;
  }
};
//----------------------------------------------------------------
struct KpCnlSpeedWrong : unary_function<KpCnl *,bool>
{
  bool operator()(KpCnl *d)const
  {
    unsigned short speed=d->KpSpeed();
    if
     (
      speed != 100 && speed !=200  &&
      speed != 300 && speed !=600  &&
      speed != 1200 && speed != 2400 && speed != 4800
      )       
    {
      dump.Print(D_ERR,"\tнедопустимое значение поля %s объекта %s : %d!",KP_CHANNEL.KpSpeed().CfgFldName(),d->Cfg_D()->Name(),speed);
      return true;
    }
    return false;
  }
};
//----------------------------------------------------------------
struct KpCnlWrong : unary_function<KpCnl *,bool>
{
  bool operator()(KpCnl *d)const
  {
    if(d)
      if(KpCnlSpeedWrong()(d) || KpCnlNumWrong()(d))
        return true;
    return false;
  }
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//проверка на ошибки типа KPDEV:
struct KpNumWrong : unary_function<KP *,bool>
{
  bool operator()(KP *d)const
  {
    if(d->Address() > 127)
    {
      dump.Print(D_ERR,"\tНедопустимое значение поля %s объекта %s : %d!",KP_DEV.Address().CfgFldName(),d->Cfg_D()->Name(),d->Address());
      return true;
    }
    return false;
  }
};
//----------------------------------------------------------------
struct KpErrorTimeoutWrong : unary_function<KP *,bool>
{
  bool operator()(KP *d)const
  {
    unsigned char tp,val = *(unsigned char*)d->Cfg_D()->GetFldValue(KP_DEV.ErrorTimeout().CfgFldId(),tp);
    if(!val || val > 10)
    {
      dump.Print(D_ERR,"\tНедопустимое значение поля %s объекта %s :%d!",KP_DEV.ErrorTimeout().CfgFldName(),d->Cfg_D()->Name(),val);
      return true;
    }
    return false;
  }
};
//----------------------------------------------------------------
struct KpResponceTimeoutWrong : unary_function<KP *,bool>
{
  bool operator()(KP *d)const
  {
    unsigned char tp;
    unsigned short val = *(unsigned short*)d->Cfg_D()->GetFldValue(KP_DEV.ResponceTimeout().CfgFldId(),tp);
    if(val < 5 || val > 30 * 60)
    {
      dump.Print(D_ERR,"\tНедопустимое значение поля %s объекта %s : %d!",KP_DEV.ResponceTimeout().CfgFldName(),d->Cfg_D()->Name(),val);
      return true;
    }
    return false;
  }
};
//----------------------------------------------------------------
struct KpStartPollWrong : unary_function<KP *,bool>
{
  bool operator()(KP *d)const
  {
    unsigned char tp,val = *(unsigned char*)d->Cfg_D()->GetFldValue(KP_DEV.StartPoll().CfgFldId(),tp);
    if(val < 25 || val > 95)
    {
      dump.Print(D_ERR,"\tНедопустимое значение поля %s объекта %s : %d!",KP_DEV.StartPoll().CfgFldName(),d->Cfg_D()->Name(),val);
      return true;
    }
    return false;
  }
};
//----------------------------------------------------------------
//полная проверка КП
struct KpWrong : unary_function<KP *,bool>
{
  bool operator()(KP *d)const
  {
    if(d)
      if( KpNumWrong()(d) || KpErrorTimeoutWrong()(d) || KpResponceTimeoutWrong()(d) || KpStartPollWrong()(d))
      {
        return true;
      }
    return false;
  }
};

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//                  проверка субблоков (общая)
//недопустимое посадочное место ФБ
struct FbPlaceWrong : unary_function<AFB *,bool>
{
  bool operator()(AFB *d)const
  {
    unsigned place=d->Place();
    if(place >16 || place < 1)
    {
      dump.Print(D_ERR,"\tНедопустимое значение поля Place объекта %s : %u!",d->Cfg_D()->Name(),place);
      return true;
    }
    return false;
  }
};
//----------------------------------------------------------------
//Проверка таймаута дребезга ФБ ТС
struct FbTsValidTimeWrong : unary_function<FbTs *,bool>
{
  bool operator()(FbTs *d)const
  {
    //отключено
    return false;
  }
};
//----------------------------------------------------------------
//проверка номера субблока ТУ
struct FbTuNumWrong : unary_function<FbTu *,bool>
{
  bool operator()(FbTu *d)const
  {
    unsigned num=d->Num();
    if(num < 1 || num > 3)
    {
      dump.Print(D_ERR,"\tНедопустимое значение поля %s объекта %s : %d!",KP_AFB_TU.Num().CfgFldName(),d->Cfg_D()->Name(),num);
      return true;
    }
    return false;
  }
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//проверка субблока (полная, для всех типов)
struct FbWrong : unary_function<AFB *,bool>
{
  bool operator()(AFB *d)const
  {
    if(d)
    {
      if(FbPlaceWrong()(d))
        return true;
      switch(d->Type())
      {
        case FB_TS:
          return FbTsValidTimeWrong()((FbTs*)d);
        case FB_TIT:case FB_TII:case FB_LIGHT:         
          break;
        case FB_TU:
          return FbTuNumWrong()((FbTu*)d);
        default:
          break;
      }
      return false;
    }
    return true;
  }
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//            Проверка ТС-ов      
//проверка номера ТС-а в субблоке
struct TsNumWrong : unary_function<TSCtl *,bool>
{
  bool operator()(TSCtl *d)const
  {
    unsigned num=d->Num();
    if(num > 64 || num <1)
    {
      dump.Print(D_ERR,"\tНедопустимое значение поля %s объекта %s : %d!",KP_TS.Num().CfgFldName(),d->Cfg_D()->Name(),num);
      return true;
    }
    return false;
  }
};
//----------------------------------------------------------------
//проверка ТС-а
struct TsWrong : unary_function<TSCtl *,bool>
{
  bool operator()(TSCtl *d)const
  {
   if(d)
    if(TsNumWrong()(d))
    {
      return true;
    }
    return false;
  }
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//            проверка  ТИТ-а
//проверка номера ТИТ-а в субблоке
struct TitNumWrong : unary_function<TITCtl *,bool>
{
  bool operator()(TITCtl *d)const
  {
    unsigned num = d->Num();
    if(num > 32 || num <1)
    {
      dump.Print(D_ERR,"\tНедопустимое значение поля %s объекта %s: %d!",KP_TIT.Num().CfgFldName(),d->Cfg_D()->Name(),num);
      return true;
    }
    return false;
  }
};
//----------------------------------------------------------------
struct TitWrong : unary_function<TITCtl *,bool>
{
  bool operator()(TITCtl *d)const
  {
   if(d)
    if(TitNumWrong()(d))
    {
      return true;
    }
    return false;
  }
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//        проверка интегр. ТИТ-а
//проверка номера ТИИ в субблоке
struct TiiNumWrong : unary_function<TIICtl *,bool>
{
  bool operator()(TIICtl *d)const
  {
    if(d->Num() > 32 || d->Num() <1)
    {
      dump.Print(D_ERR,"\tНедопустимое значение поля %s объекта %s: %d!",KP_TIT.Num().CfgFldName(),d->Cfg_D()->Name(),d->Num());
      return true;
    }
    return false;
  }
};
//----------------------------------------------------------------
struct TiiWrong : unary_function<TIICtl *,bool>
{
  bool operator()(TIICtl *d)const
  {
   if(d)
    if(TiiNumWrong()(d))
    {
      return true;
    }
    return false;
  }
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//              проверка лампы
//проверка номера лампы в субблоке
struct LightNumWrong : unary_function<LightCtl *,bool>
{
  bool operator()(LightCtl *d)const
  {
    if(d->Num() > 64 || d->Num() <1)
    {
      dump.Print(D_ERR,"\tНедопустимое значение поля %s объекта %s: %d!",KP_TIT.Num().CfgFldName(),d->Cfg_D()->Name(),d->Num());
      return true;
    }
    return false;
  }
};

//----------------------------------------------------------------
struct LightWrong : unary_function<LightCtl *,bool>
{ 
  bool operator()(LightCtl *d)const
  {
   if(d)
    if(LightNumWrong()(d))
    {
      return true;
    }
    return false;
  }
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//              Проверка ТУ
//проверка номера группы ту
struct TuGrpWrong : unary_function<TUCtl *,bool>
{
  bool operator()(TUCtl *d)const
  {
    if(d->GroupNum() > 16 || d->GroupNum() < 1)
    {
      dump.Print(D_ERR,"\tНедопустимое значение поля %s объекта %s : %d!",KP_TU.GroupNum().CfgFldName(),d->Cfg_D()->Name(),d->GroupNum());
      return true;
    }
    return false;
  }
};
//----------------------------------------------------------------
//проверка номера ту в группе
struct TuNumWrong : unary_function<TUCtl *,bool>
{
  bool operator()(TUCtl *d)const
  {
    if(d->Num() > 8 || d->Num() <1)
    {
      dump.Print(D_ERR,"\tНедопустимое значение поля %s объекта %s : %d!",KP_TU.Num().CfgFldName(),d->Cfg_D()->Name(),d->Num());
      return true;
    }
    return false;
  }
};
//----------------------------------------------------------------
struct TuWrong : unary_function<TUCtl *,bool>
{
  bool operator()(TUCtl *d)const
  {
   if(d)
    if(TuNumWrong()(d) || TuGrpWrong()(d))
    {      
      return true;
    }
    return false;
  }
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//                        Проверки на конфликты
//----------------------------------------------------------------
//проверка на конфликты типа KpCommDev:
struct CommDevsEqual : binary_function<KpCommDev *,KpCommDev *,bool>
{
  bool operator()(KpCommDev * d1,KpCommDev * d2)const
  {
    if(d1!=d2)
      if(!strcmp(d1->Name(),d2->Name()))
      {
        dump.Print(D_ERR,"\tСовпадение имен последовательных портов! Объекты %s и %s удалены!",d1->Cfg_D()->Name(),d2->Cfg_D()->Name());
        return true;
      }
    return false;
  }
};
//----------------------------------------------------------------
//проверка на конфликты типа KP_CHANNEL:
struct KpCnlsEqual : binary_function<KpCnl *,KpCnl *,bool>
{
  bool operator()(KpCnl * d1,KpCnl * d2)const
  {
    if(d1!=d2)
      if(!strcmp(d1->CommDev(),d2->CommDev()) && d1->Num()==d2->Num())
      {
        dump.Print(D_ERR,"\tСовпадение каналов: %s и %s !",d1->Cfg_D()->Name(),d2->Cfg_D()->Name());
        return true;
      }
    return false;
  }
};
//----------------------------------------------------------------
//проверка на конфликты типа KpDev:
struct KpsEqual : binary_function<KP *,KP *,bool>
{
  bool operator()(KP * d1,KP * d2)const
  {    
    if(d1!=d2)
    {
      const char *cdv1,*cdv2;
      unsigned char tp;
      cdv1=(const char*)d1->Cfg_D()->GetFldValue(KP_DEV.CnlName().CfgFldId(),tp);
      cdv2=(const char*)d2->Cfg_D()->GetFldValue(KP_DEV.CnlName().CfgFldId(),tp);
      if(d1->Address() == d2->Address() && !strcmp(cdv1,cdv2))
      {
        dump.Print(D_ERR,"\tСовпадение адресов двух КП: %s и %s !",d1->Cfg_D()->Name(),d2->Cfg_D()->Name());
        return true;
      }
    }
    return false;
  }
};
//----------------------------------------------------------------
//конфликт ФБ
struct FbsEqual : binary_function<AFB*,AFB*,bool>
{
  bool operator()(AFB *d1,AFB *d2)const
  {
    if(d1!=d2)
      if(strcmp(d1->Parent(),d2->Parent())==0)
      {
        if(d1->Place() == d2->Place())
        {
          dump.Print(D_ERR,"\tСовпадение мест двух АФБ: %s и %s : %d!",d1->Cfg_D()->Name(),d2->Cfg_D()->Name(),d1->Place());
          return true;
        }
        if(d1->Type() == d2->Type())
          if(d1->Type()==FB_TU)
            if(((FbTu*)(d1))->Num() == ((FbTu*)(d2))->Num())
            {
              dump.Print(D_ERR,"\tСовпадение номеров двух АФБ ТУ: %s и %s : %d!",d1->Cfg_D()->Name(),d2->Cfg_D()->Name(),((FbTu*)(d1))->Num());
              return true;
            }
      }
    return false;
  }
};
//----------------------------------------------------------------
//проверка на совпадение ТС-ов
struct TssEqual : binary_function<TSCtl *,TSCtl *,bool>
{
  bool operator()(TSCtl *d1,TSCtl *d2)const
  {
   if(d1!=d2)
    if(d1->Num() == d2->Num() && !strcmp(d1->Parent(),d2->Parent()))
    {
      dump.Print(D_ERR,"\tСовпадение номеров ТC в одном субблоке: %s и %s !",d1->Cfg_D()->Name(),d2->Cfg_D()->Name());
      return true;
    }
    return false;
  }
};
//----------------------------------------------------------------
//Проверка на повтор интегрируемых. ТИТ-ов у ТИТ-ов текущих
struct TitisEqual : binary_function<TITCtl *,TITCtl *,bool>
{
  bool operator()(TITCtl * d1,TITCtl * d2)const
  {
   if(d1!=d2)
   {
     unsigned char tp;
    const char *name1=(const char*)d1->Cfg_D()->GetFldValue(KP_TIT.TITid_Name().CfgFldId(),tp);
    const char *name2=(const char*)d2->Cfg_D()->GetFldValue(KP_TIT.TITid_Name().CfgFldId(),tp);
    if(!strlen(name1) || !strlen(name2))
      return false;
    else if(!strcmp(name1,name2))
    {
      dump.Print(D_ERR,"\tСовпадение имен интегрируемых ТИТ: %s и %s !",d1->Cfg_D()->Name(),d2->Cfg_D()->Name());
      return true;
    }
   }
    return false;
  }
};
//----------------------------------------------------------------
//Проверка на совпадение номеров ТИТ-ов в одном субблоке
struct TitsEqual : binary_function<TITCtl *,TITCtl *,bool>
{
  bool operator()(TITCtl * d1,TITCtl * d2)const
  {
   if(d1!=d2)
    if(d1->Num() == d2->Num() && !strcmp(d1->Parent(),d2->Parent()))
    {
      dump.Print(D_ERR,"\tСовпадение номеров ТИТ в одном субблоке: %s и %s !",d1->Cfg_D()->Name(),d2->Cfg_D()->Name());
      return true;
    }
    return false;
  }
};
//----------------------------------------------------------------
//Проверка на совпадение номеров ТИИ в одном субблоке
struct TiisEqual : binary_function<TIICtl *,TIICtl *,bool>
{
  bool operator()(TIICtl * d1,TIICtl * d2)const
  {
   if(d1!=d2)
    if(d1->Num() == d2->Num() && !strcmp(d1->Parent(),d2->Parent()))
    {
      dump.Print(D_ERR,"\tСовпадение номеров ТИИ в одном субблоке: %s и %s !",d1->Cfg_D()->Name(),d2->Cfg_D()->Name());
      return true;
    }
    return false;
  }
};
//----------------------------------------------------------------
//Проверка на совпадение номеров ламп щита в одном субблоке
struct LightsEqual : binary_function<LightCtl *,LightCtl *,bool>
{
  bool operator()(LightCtl * d1,LightCtl * d2)const
  {
   if(d1!=d2)
    if(d1->Num() == d2->Num() && !strcmp(d1->Parent(),d2->Parent()))
    {
      dump.Print(D_ERR,"\tСовпадение номеров ламп в одном субблоке: %s и %s !",d1->Cfg_D()->Name(),d2->Cfg_D()->Name());
      return true;
    }
    return false;
  }
};
//----------------------------------------------------------------
//Проверка на совпадение номеров ламп щита в одном субблоке
struct TusEqual : binary_function<TUCtl *,TUCtl *,bool>
{
  bool operator()(TUCtl * d1,TUCtl * d2)const
  {
   if(d1!=d2)
    if(d1->Num() == d2->Num() && d1->GroupNum() == d2->GroupNum() && !strcmp(d1->Parent(),d2->Parent()))
    {
      dump.Print(D_ERR,"\tСовпадение номеров ТУ в одной группе: %s и %s !",d1->Cfg_D()->Name(),d2->Cfg_D()->Name());
      return true;
    }
    return false;
  }
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//проверка на ошибки всех типов объектов
struct ChkCtlObjectForWrong : unary_function<pair<ControlObject *,unsigned char>,bool>
{
  bool operator()(pair<ControlObject *,unsigned char> d)const
  {    
    if(d.second & OBJ_FAILED)
      return true;
    switch(d.first->Type())
    {
      case TS:
        return TsWrong()((TSCtl*)d.first);
      case TIT:
        return TitWrong()((TITCtl*)d.first);
      case TITi:
        return false;
        break;
      case TII:
        return TiiWrong()((TIICtl*)d.first);
      case TU:
        return TuWrong()((TUCtl*)d.first);
      case LIGHT:
        return LightWrong()((LightCtl*)d.first);
      case FB_TS:case FB_TIT:case FB_TII:case FB_TU:case FB_LIGHT:
        return FbWrong()((AFB*)d.first);
      case KP_DEVICE:
        return KpWrong()((KP*)d.first);
      case KP_COMM_DEV:
        return CommDevWrong()((KpCommDev*)d.first);
      case KP_CNL:
        return KpCnlWrong()((KpCnl*)d.first);
      default:
        break;
    }
    return true;
  }
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//проверка конфликтов
//считаю, что объекты одного типа находятся в одном контейнере (исключение - AFB)
struct ChkForCtlObjectEqual : binary_function<pair<ControlObject *,unsigned char>,pair<ControlObject *,unsigned char>,bool>
{
  bool operator()(pair<ControlObject *,unsigned char> d1,pair<ControlObject *,unsigned char> d2)const
  {
    if(d1==d2)
      return false;
    if((!d1.first || !d2.first)) //|| ((d1.second & OBJ_FAILED) || (d2.second & OBJ_FAILED)))
      return false;
    if(d1.first->Type()==d2.first->Type())
    {
      switch(d1.first->Type())
      {
        case TS:
          return TssEqual()((TSCtl*)d1.first,(TSCtl*)d2.first);
        case TIT:
          if(TitsEqual()((TITCtl*)d1.first,(TITCtl*)d2.first))
            return true;
          if(TitisEqual()((TITCtl*)d1.first,(TITCtl*)d2.first))
            return true;
          break;
        case TITi:
          break;
        case TII:
          return TiisEqual()((TIICtl*)d1.first,(TIICtl*)d2.first);
        case TU:
          return TusEqual()((TUCtl*)d1.first,(TUCtl*)d2.first);
        case LIGHT:
          return LightsEqual()((LightCtl*)d1.first,(LightCtl*)d2.first);
        case KP_DEVICE:
          return KpsEqual()((KP*)d1.first,(KP*)d2.first);
        case KP_COMM_DEV:
          return CommDevsEqual()((KpCommDev*)d1.first,(KpCommDev*)d2.first);
        case KP_CNL:
          return KpCnlsEqual()((KpCnl*)d1.first,(KpCnl*)d2.first);
        default:
          break;
      }
    }
    else//аккуратней с этим!!!!
      //case FB_TS : case FB_TIT : case FB_TII : case FB_TU : case FB_LIGHT_32 : case FB_LIGHT_64:
          return FbsEqual()((AFB*)d1.first,(AFB*)d2.first);
    return false;
  }
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//ключ поиска по имени порта
struct CtlObjNameKey : binary_function<pair<ControlObject *,unsigned char>,const char *,bool>
{
  bool operator()(pair<ControlObject *,unsigned char> d,const char *key)const
  {
    if(!key || (d.second & OBJ_FAILED))
      return false;
    if(key == "")
      return false;    
    return strcmp(d.first->Cfg_D()->Name(),key)==0;
  }
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
struct IfCtlObjectFail : unary_function<pair<ControlObject *,unsigned char>,bool>
{
  bool operator()(pair<ControlObject *,unsigned char> d)const
  {    
    return (d.second & OBJ_FAILED)==0;
  }
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

struct IfFlagIsSet : binary_function<pair<ControlObject *,unsigned char>,unsigned char,bool>
{
  bool operator()(pair<ControlObject *,unsigned char> d,unsigned char flag)const
  {
    return (d.second & flag)!=0;
  }
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
struct IfPortInited : unary_function<pair<ControlObject *,unsigned char>,bool>
{
  bool operator()(pair<ControlObject *,unsigned char> d)const
  {    
    if(d.first->Type()==KP_COMM_DEV)
      return static_cast<KpCommDev*>(d.first)->Init();
    return false;
  }
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//                      КЛЮЧИ ПОИСКА
//ключ поиска по имени порта канала
struct CnlParentNameKey : binary_function<pair<ControlObject *,unsigned char>,const char *,bool>
{
  bool operator()(pair<ControlObject *,unsigned char> d,const char *key)const
  {
    if(!key || (d.second & OBJ_FAILED))
      return false;
    if(!strlen(key))
      return false;
    if(d.first->Type()!=KP_CNL)
      return false;
    const char *name=static_cast<KpCnl*>(d.first)->CommDev();
    return (strcmp(name,key)==0);
  }
};
//----------------------------------------------------------------
//ключ поиска по имени КП
struct FbParentNameKey : binary_function<pair<ControlObject *,unsigned char>,const char *,bool>
{
  bool operator()(pair<ControlObject *,unsigned char> d,const char *key)const
  {
    if(!key || (d.second & OBJ_FAILED))
      return false;
    if(!strlen(key))
      return false;
    unsigned char type=d.first->Type();
    if(type!=FB_TS && type!=FB_TIT && type!=FB_TII 
     && type!=FB_TU && type!=FB_LIGHT)
     return false;
    const char *parent=static_cast<AFB*>(d.first)->Parent();
    return (strcmp(parent,key)==0);
  }
};
//----------------------------------------------------------------
//ключ поиска по имени канала
struct KpCnlNameKey : binary_function<pair<ControlObject *,unsigned char>,const char *,bool>
{
  bool operator()(pair<ControlObject *,unsigned char> d,const char *key)const
  {
    if(!key || (d.second & OBJ_FAILED))
      return false;
    if(!strlen(key))
      return false;
    if(d.first->Type()!=KP_DEVICE)
      return false;
    unsigned char tp;
    const char *cnl_name=(const char*)d.first->Cfg_D()->GetFldValue(KP_DEV.CnlName().CfgFldId(),tp);
    return (strcmp(cnl_name,key)==0);
  }
};
//----------------------------------------------------------------
//ключ поиска по имени FB_TS:
struct TsParentNameKey : binary_function<pair<ControlObject *,unsigned char>,const char *,bool>
{
  bool operator()(pair<ControlObject *,unsigned char> d,const char *key)const
  {
    if(!key || (d.second & OBJ_FAILED))
      return false;
    if(!strlen(key))
      return false;
    if(d.first->Type()!=TS)
      return false;
    const char *parent=static_cast<TSCtl*>(d.first)->Parent();
    return (strcmp(parent,key)==0);
  }
};
//----------------------------------------------------------------
//ключ поиска по имени FB_TIT:
struct TitParentNameKey : binary_function<pair<ControlObject *,unsigned char>,const char *,bool>
{
  bool operator()(pair<ControlObject *,unsigned char> d,const char *key)const
  {
    if(!key || (d.second & OBJ_FAILED))
      return false;
    if(!strlen(key))
      return false;
    if(d.first->Type()!=TIT)
      return false;
    const char *parent=static_cast<TITCtl*>(d.first)->Parent();
    return strcmp(parent,key)==0;
  }
};
//----------------------------------------------------------------
//ключ поиска по имени FB_TII:
struct TiiParentNameKey : binary_function<pair<ControlObject *,unsigned char>,const char *,bool>
{
  bool operator()(pair<ControlObject *,unsigned char> d,const char *key)const
  {
    if(!key || (d.second & OBJ_FAILED))
      return false;
    if(!strlen(key))
      return false;
    if(d.first->Type()!=TII)
      return false;
    const char *parent=static_cast<TIICtl*>(d.first)->Parent();
    return (strcmp(parent,key)==0);
  }
};
//----------------------------------------------------------------
//ключ поиска по имени FB_TU:
struct TuParentNameKey : binary_function<pair<ControlObject *,unsigned char>,const char *,bool>
{
  bool operator()(pair<ControlObject *,unsigned char> d,const char *key)const
  {
    if(!key || (d.second & OBJ_FAILED))
      return false;
    if(!strlen(key))
      return false;
    if(d.first->Type()!=TU)
      return false;
    const char *parent=static_cast<TUCtl*>(d.first)->Parent();
    return (strcmp(parent,key)==0);
  }
};
//----------------------------------------------------------------
//ключ поиска по имени FB_LIGHT:
struct LghParentNameKey : binary_function<pair<ControlObject *,unsigned char>,const char *,bool>
{
  bool operator()(pair<ControlObject *,unsigned char> d,const char *key)const
  {
    if(!key || (d.second & OBJ_FAILED))
      return false;
    if(!strlen(key))
      return false;
    if(d.first->Type()!=LIGHT)
      return false;
    const char *parent=static_cast<LightCtl*>(d.first)->Parent();
    return (strcmp(parent,key)==0);
  }
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif