/**********************************************************************************
              ��������� ������ ������������ �������           
                         created by M.E.N.
************************************************************************************/
/*
�� �������������� ������������ ������� � ������ �������!
� ������������ ��� �������� ��������������� �� ��������� 0! 
���������� ��������� ������� - �� ����� 2^16 

��� ������������� �������, ������������ ���������, �����������!!! �������� �� NULL!!!!


������ � ����� �������:

1) �� ������� ����: (�������� �� ����� ������, ���� ������)
  obj_d obj;
  obj[10] = (long)32123;
  long x = obj[10];

2) ����� ������� ������� ������: (����� ������� ������)
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

#define NOT_ACCESSABLE 0//��� ���� �� ������ � ������
#define WRITE_ONLY     1//����� �� ������
#define READ_ONLY      2//����� �� ������
#define WRITE_READ     3

#define SUCCESS       1
#define NOT_FOUND    -1
#define ACCESS_DENIED -2


//%%%%%%%%%%% ������� ������� ��������� (Descriptor) %%%%%%%%%%%%%%%%%
class Dscr
{
public:
  Dscr(const char *Name="",unsigned Id=0);
  virtual ~Dscr();
  const char *Name()const{return name;}
  unsigned Id()const{return id;}
protected:
  char *name;//��� ����
  unsigned id;
};
//%%%%%%%%%%%%%%% ���������� ��������� ���� %%%%%%%%%%%%%%%%%%%%%%%%%%%
class type_d;
class obj_d;
class fld_d : public Dscr
{
  friend class obj_d;//            <-
  friend class type_d;//������ ��� ��� | ������ ����� ������� ����������� fld_d � �������� � ��� ������
 // friend class StmSrv;
public:
  unsigned Type()const{return (unsigned)type;}
  int Len()const;//���������� ����� ���� ���� � ������
  virtual ~fld_d();
//protected:
  fld_d(const char *Name,unsigned Id,unsigned char Type=tm_byte);
  map<unsigned,void *,less<unsigned> > Values;//���� ������� - �����. �����. id ������� - 1 (� ����)
  const unsigned char type;//��� �������� ����
};
//%%%%%%%%%%%%%%%%%%%% ��������� �������%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
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
  void SetFldChangeBit(unsigned fld)const; //��������� ���� �������� ��������� ���� �������
  void ClearFldChangeBit(unsigned fld)const; //��������� ���� �������� ��������� ���� �������
  void ClearFldChangesMask()const;
  class FldValPxy     //���������� ������ - ������.
  {
    friend class obj_d;
   public:
    //������:
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
    //������:
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
  FldValPxy operator[](unsigned); //������ � ���� �������
protected:
  obj_d(const char *name,unsigned Id,unsigned short a_cluster,type_d *mr=0);
  void *fld_value(unsigned fld);
  type_d *master;
  mutable unsigned *mask_of_fld_changes;//����� ������������ �����
  unsigned short AccessCluster;//������� �������, � �������� ��������� ������
};
/*%%%%%%%%%%%%%%%%%%%% ��������� ���� %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
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
  const fld_d *Fld(unsigned)const;//���� �� �������������� - ����� �����!!!  
  const obj_d *Obj(unsigned)const;//������ �� ��������������
  int ObjCount()const{return Objects.size();}
  int FldCount()const{return Fields.size();}
protected:
  type_d(const char *name,unsigned Id,StmCfg *msr=0);
  virtual ~type_d();
  unsigned AddFld(const char *Name,unsigned Type);//���������� ������ ����
  unsigned AddObj(unsigned Id,const char *,unsigned short a_cl);//���������� ������ �������
protected:
  void SetFldValue(unsigned obj,unsigned fld,void *val,unsigned char fld_type);
  const void *GetFldValue(unsigned obj,unsigned fld,unsigned char &fld_type)const;
  map<unsigned, obj_d *, less<unsigned> > Objects;
  map<unsigned, fld_d *, less<unsigned> > Fields;//������ ��������
  StmCfg *master;
};
//%%%%%%%%%%%%%% U-����� %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class User : public Dscr
{
public:
  User(unsigned i_d,const char *name,const char *passwd=0);
  ~User();
  const char *Passwd()const{return passwd;}
private:
  char *passwd;
};
//%%%%%%%%%%%%%%% ��� ���� �������� �����. ������ %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
struct NameKey : public binary_function<pair<unsigned, Dscr *>,const char *,bool>
{
  bool operator()(pair<unsigned, Dscr *>f,const char *key)const
  {
    if(f.second)
      return (strcmp(f.second->Name(),key)==0);
    return false;
  }
};
//%%%%%%%%%%%%%% ������ ������� %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class UsrGroup : public Dscr
{
  friend class StmCfg;
public:
  UsrGroup(const char *name,unsigned id):Dscr(name,id){}
  ~UsrGroup();  
  int Rights(unsigned cl);//���������� ����� ������� � ��������� ��������
protected:
  map<unsigned ,User *,less<unsigned> > Users;
/*
�������� ������� (���� ������� - ������������� �������� (����� �� �� ������), ���������� ���� ����������
����� ������� ������ ������ � �������� � ����� ������� ��������. ��� ���������� ������� ������ ���� �� ��� ������
����������� � �������� �������. ����� ��������� ������� ���������������.
*/
  map<unsigned , char,less<unsigned> > AccessClusters;
};
//%%%%%%%%%%%%%% ��������� ����� ������������ %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
/*
  ��� ���������� ������������������ �������� ���� ������� ������� ����������� 
  �������, ������������ � �����! (��� � ������ �������)...
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
//������ � �������� ������� � ��������
  const char *UserPasswd(unsigned usr)const;
  const char *UserName(unsigned usr)const;
  unsigned UserId(const char *name);//��������� ������������� ������, ���� �����. 0, �� ������ ������ ��� � �����
  unsigned Group(unsigned user_id);//��������� ������, � ������� ��������� ������ -//-
//  �������� ���� ������� � �������� (����������� ����� �������� �������)
  bool GroupCanObject(unsigned gr,unsigned obj,bool read);//���� ��������� �������� = false, �� ����������� ����� ������  
//�������� �������]
  unsigned TypeId(const char *TypeName);
  unsigned ObjId(unsigned t_id,const char *ObjName);
  unsigned FldId(unsigned t_id,const char *FldName);
  unsigned Version()const{return version;}
  unsigned ObjectsCount()const{return ObjCount;}
  unsigned TypesCount()const{return TypeCount;}
protected:
//�������� ������������ // ���������� �������������� ������������ ��������
  unsigned AddType(const char *);
  unsigned AddFld(unsigned type_id,unsigned type,const char *name);
  unsigned AddObj(unsigned type,const char *name,unsigned short a_cl);
  unsigned AddUsrGroup(const char *name);
  unsigned AddUser(unsigned gr,const char *name,const char *pwd);
//��������� ���� ������� ������ � ��������
  void SetRights(unsigned group,unsigned cl,char rts);
private:
  unsigned short version;
  unsigned ObjCount;
  unsigned TypeCount;
  unsigned UserCount;
  map<unsigned, UsrGroup *,less<unsigned> > Groups;//������ �������������
  map<unsigned, type_d *, less<unsigned> > Types;

};
//--------------------------------------------------------------------------------
#endif