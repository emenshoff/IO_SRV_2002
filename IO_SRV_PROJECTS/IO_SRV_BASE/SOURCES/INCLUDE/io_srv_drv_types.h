/***********************************************************
  ����� ��� �������� �������� ����� � �� ����� �
  ������������ �������. ��������� ������� ����� � ���� � 
  ������������ �� �������� ���� (tm_byte,tm_time �.�.�.)
               created by M.E.N
***********************************************************/

#ifndef _drv_types_h_
#define _drv_types_h_
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#include <stm_srv_cfg.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
typedef struct FldDescription{
  char Name[20];             //��� ���� � �����
  unsigned short Type;       //��� ���
}FldDescription;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//��������� ������ ����
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
 char *name;          //��� ���� � �����
 unsigned cfg_fld_id; //������������� ���� � �����
 unsigned char type;  //��� ���
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//��������� ������ ���� (������� �����)
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
 char *name;          //��� ���������������� ���� � �����
 unsigned cfg_id;     //������������� ���������������� ���� � �����
 unsigned char flags; //����� ������ �������� �������� ���� 
 vector<FldDscr *> fields;
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif //drv_types