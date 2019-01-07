/***********************************************************
  Класс для проверки описания типов и их полей в
  конфигурации сервера. Проверяет наличие полей в типе и 
  корректность их базового типа (tm_byte,tm_time и.т.д.)
               created by M.E.N
***********************************************************/

#ifndef _drv_types_h_
#define _drv_types_h_
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#include <stm_srv_cfg.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
typedef struct FldDescription{
  char Name[20];             //имя поля в конфе
  unsigned short Type;       //его тип
}FldDescription;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//описатель одного поля
class FldDscr
{
  friend class TypeDscr;
public:
 FldDscr(const char *n,unsigned char tp);
 virtual ~FldDscr();
 const char *CfgFldName()const{return name;}
 unsigned CfgFldId()const{return cfg_fld_id;}
 unsigned Type()const{return type;}
protected:
 char *name;          //имя поля в конфе
 unsigned cfg_fld_id; //идентификатор поля в конфе
 unsigned char type;  //его тип
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//описатель одного типа (базовый класс)
class TypeDscr
{
public:
 TypeDscr(const char *nm);
 virtual ~TypeDscr();
 virtual bool Init(StmCfg * cf);
 const char *CfgTypeName()const{return name;}
 unsigned CfgId()const{return cfg_id;} 
 unsigned char Flags()const{return flags;}
protected:
 char *name;          //имя соответствующего типа в конфе
 unsigned cfg_id;     //идентификатор соответствующего типа в конфе
 unsigned char flags; //флаги ошибок проверки описания типа 
 vector<FldDscr *> fields;
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif //drv_types