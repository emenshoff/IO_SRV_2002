/**********************************************************************************
              Структуры данных конфигурации сервера           
                         created by M.E.N.
************************************************************************************/
/*
Не отрабатывается дублирование юзверей в разных группах!
В конфигурации нет числовых идентификаторов со значением 0! 
Количество кластеров доступа - не более 2^16 

При использовании функций, возвращающих указатель, ОБЯЗАТЕЛЬНО!!! ПРОВЕРЯЙ НА NULL!!!!


Доступ к полям объекта:

1) По индексу поля: (работает не очень быстро, зато удобно)
  obj_d obj;
  obj[10] = (long)32123;
  long x = obj[10];

2) Через функции низкого уровня: (самый быстрый способ)
  unsigned char type = tm_long
  long value =32123;
  obj_d obj;
  obj.SetFldValue(10,(void*)&value,tm_long);
  value = obj.GetFldValue(10,type);
  
*/
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#define TO_VOID(x)  (void*)&x
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#ifndef _srv_StmCfg_h
#define _srv_StmCfg_h

#include <map>
#include <list>
#include <string>
#include <vector>
#include <algorithm>
#include <time.h>
#include <string.h>
#include <stm_types.h>
#include <io_srv_modules.h>
#include <io_srv_event_processor.h>

#include <time.h>
#ifdef WIN32
# include <windows.h>
#else
# include <sys/time.h>
#endif

using namespace std;

#define NOT_ACCESSABLE 0//нет прав на чтение и запись
#define WRITE_ONLY     1//права на запись
#define READ_ONLY      2//права на чтение
#define WRITE_READ     3

#define SUCCESS       1
#define NOT_FOUND    -1
#define ACCESS_DENIED -2


//%%%%%%%%%%% базовый класссс апысатэль (Descriptor) %%%%%%%%%%%%%%%%%
class Dscr
{
public:
  Dscr(const char *Name="",unsigned Id=0);
  virtual ~Dscr();
  const char *Name()const{return name;}
  unsigned Id()const{return id;}
protected:
  char *name;//имя поля
  unsigned id;
};
//%%%%%%%%%%%%%%% хитрожопый описатель поля %%%%%%%%%%%%%%%%%%%%%%%%%%%
class type_d;
class obj_d;
class fld_d : public Dscr
{
  friend class obj_d;//            <-
  friend class type_d;//только эти два | класса могут вызвать конструктор fld_d и работать с его полями
 // friend class StmSrv;
public:
  unsigned Type()const{return (unsigned)type;}
  int Len()const;//возвращает длину типа поля в байтах
  virtual ~fld_d();
//protected:
  fld_d(const char *Name,unsigned Id,unsigned char Type=tm_byte);
  map<unsigned,void *,less<unsigned> > Values;//ключ словаря - соотв. внутр. id объекта - 1 (с нуля)
  const unsigned char type;//тип значения поля
};
//%%%%%%%%%%%%%%%%%%%% описатель объекта%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class obj_d : public Dscr
{
  friend class type_d;
  friend class StmCfg;
public:
  virtual ~obj_d();  
  void SetFldValue(unsigned fld,void *val,unsigned char fld_type);
  const void *GetFldValue(unsigned fld,unsigned char &fld_type)const;
  const type_d *Master()const{return master;}
  unsigned Cluster()const{ return AccessCluster;}
  const unsigned *FldChangesMask()const{return mask_of_fld_changes;}
  void SetFldChangeBit(unsigned fld)const; //установка бита признака изменения поля объекта
  void ClearFldChangeBit(unsigned fld)const; //установка бита признака изменения поля объекта
  void ClearFldChangesMask()const;
  class FldValPxy     //хитрожопый прокси - объект.
  {
    friend class obj_d;
   public:
    //чтение:
    operator char()const;
    operator unsigned char()const;
    operator short()const;
    operator unsigned short()const;    
    operator int()const;
    operator unsigned()const;
    operator long()const;
    operator unsigned long()const;
    operator float()const;
    operator double()const;
    operator const char*()const;
    operator timeval()const;
    //запись:
    void operator=(char);
    void operator=(unsigned char);
    void operator=(short);
    void operator=(unsigned short);
    void operator=(int);
    void operator=(unsigned);
    void operator=(long);
    void operator=(unsigned long);
    void operator=(float);
    void operator=(double);
    void operator=(const char*);
    void operator=(timeval);
   private:
    FldValPxy(void *dt,unsigned char tp):data(dt),type(tp){}
    void *data;
    unsigned char type;
  };
  FldValPxy operator[](unsigned); //доступ к полю объекта
protected:
  obj_d(const char *name,unsigned Id,unsigned short a_cluster,type_d *mr=0);
  void *fld_value(unsigned fld);
  type_d *master;
  mutable unsigned *mask_of_fld_changes;//маска изменившихся полей
  unsigned short AccessCluster;//кластер доступа, к которому относится объект
};
/*%%%%%%%%%%%%%%%%%%%% описатель типа %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
id!=0 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
class StmCfg;
class type_d : public Dscr
{
  friend class obj_d;
  friend class StmCfg;
  friend class ConfCreator;
  friend class StmSrv;
  friend class SrvClient;
  friend class KpControl;
  friend class I7000_Config;
  friend class KpConfig;
  friend class TypeDscr;
public:  
  const fld_d *Fld(unsigned)const;//поле по идентификатору - очень бысто!!!  
  const obj_d *Obj(unsigned)const;//объект по идентификатору
  int ObjCount()const{return Objects.size();}
  int FldCount()const{return Fields.size();}
protected:
  type_d(const char *name,unsigned Id,StmCfg *msr=0);
  virtual ~type_d();
  unsigned AddFld(const char *Name,unsigned Type);//добавление нового поля
  unsigned AddObj(unsigned Id,const char *,unsigned short a_cl);//добавление нового объекта
protected:
  void SetFldValue(unsigned obj,unsigned fld,void *val,unsigned char fld_type);
  const void *GetFldValue(unsigned obj,unsigned fld,unsigned char &fld_type)const;
  map<unsigned, obj_d *, less<unsigned> > Objects;
  map<unsigned, fld_d *, less<unsigned> > Fields;//ключам объектов
  StmCfg *master;
};
//%%%%%%%%%%%%%% U-зверь %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class User : public Dscr
{
public:
  User(unsigned i_d,const char *name,const char *passwd=0);
  ~User();
  const char *Passwd()const{return passwd;}
private:
  char *passwd;
};
//%%%%%%%%%%%%%%% аще адын полезный функц. объект %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
struct NameKey : public binary_function<pair<unsigned, Dscr *>,const char *,bool>
{
  bool operator()(pair<unsigned, Dscr *>f,const char *key)const
  {
    if(f.second)
      return (strcmp(f.second->Name(),key)==0);
    return false;
  }
};
//%%%%%%%%%%%%%% группа юзверей %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class UsrGroup : public Dscr
{
  friend class StmCfg;
public:
  UsrGroup(const char *name,unsigned id):Dscr(name,id){}
  ~UsrGroup();  
  int Rights(unsigned cl);//возвращает права доступа к заданному кластеру
protected:
  map<unsigned ,User *,less<unsigned> > Users;
/*
кластеры доступа (ключ словаря - идентификатор кластера (имена их не храним), содержимое узла определяет
права доступа данной группы к объектам и типам данного кластера. При добавлении каждого нового типа во все группы
добавляются и кластеры доступа. Имена кластеров доступа проигнорированы.
*/
  map<unsigned , char,less<unsigned> > AccessClusters;
};
//%%%%%%%%%%%%%% описатель одной конфигурации %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
/*
  Для увеличения производительности проверку прав доступа обязаны производить 
  объекты, обращающиеся к конфе! (был и другой вариант)...
*/
class StmCfg : public Dscr
{
  friend class SrvClient;
  friend class ConfCreator;
  friend class StmSrv;
public:
  StmCfg(const char *name,unsigned id,unsigned ver);
  virtual ~StmCfg();
  const type_d *Type(unsigned id);
  const obj_d *Obj(unsigned id);
//работа с группами юзверей и юзверями
  const char *UserPasswd(unsigned usr)const;
  const char *UserName(unsigned usr)const;
  unsigned UserId(const char *name);//возвратит идентификатор юзверя, если возвр. 0, то такого юзверя нет в конфе
  unsigned Group(unsigned user_id);//возвратит группу, к которой относится юзверь -//-
//  проверка прав доступа к объектам (реализована через кластеры доступа)
  bool GroupCanObject(unsigned gr,unsigned obj,bool read);//если последний параметр = false, то проверяются права записи  
//полезные функции]
  unsigned TypeId(const char *TypeName);
  unsigned ObjId(unsigned t_id,const char *ObjName);
  unsigned FldId(unsigned t_id,const char *FldName);
  unsigned Version()const{return version;}
  unsigned ObjectsCount()const{return ObjCount;}
  unsigned TypesCount()const{return TypeCount;}
protected:
//создание конфигурации // возвращают иденитификатор добавленного элемента
  unsigned AddType(const char *);
  unsigned AddFld(unsigned type_id,unsigned type,const char *name);
  unsigned AddObj(unsigned type,const char *name,unsigned short a_cl);
  unsigned AddUsrGroup(const char *name);
  unsigned AddUser(unsigned gr,const char *name,const char *pwd);
//установка прав доступа группы к кластеру
  void SetRights(unsigned group,unsigned cl,char rts);
private:
  unsigned short version;
  unsigned ObjCount;
  unsigned TypeCount;
  unsigned UserCount;
  map<unsigned, UsrGroup *,less<unsigned> > Groups;//группы пользователей
  map<unsigned, type_d *, less<unsigned> > Types;

};
//--------------------------------------------------------------------------------
#endif