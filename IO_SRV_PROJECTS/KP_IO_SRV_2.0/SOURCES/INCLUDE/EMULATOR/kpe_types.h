/***********************************************************
  �������� ����� �����, ����������� ��� ���������� 
  ��������� ������� �� "������"
               created by M.E.N
***********************************************************/

#ifndef _kpe_types_h_
#define _kpe_types_h_

#include <io_srv_drv_types.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
enum{
  TSE  = 100,
  TITE,
  // ���� ��
  FBE_TS,
  FBE_TIT,
  KPE_DEVICE,
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//�������� ������ �� "������"
class kpe_dev :public TypeDscr
{
public:
  kpe_dev();
 FldDscr &CnlName()const{return(*fields[0]);}//�����, � �������� ��������� "��"
 FldDscr &Address()const{return (*fields[1]);}//����� "��" �� ������
 FldDscr &Enabled()const{return (*fields[2]);}//���������� ������
 FldDscr &NoKvit() const{return (*fields[3]);}
 FldDscr &SendDelay()const{return (*fields[4]);}
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//�������������� ���� �� (��������)
class kpe_afb_ts :public TypeDscr
{
public:
 kpe_afb_ts();
 FldDscr &Parent()const{return(*fields[0]);} //��� ��, � ������� ���������� ������ ������� 
 FldDscr &Place()const{return(*fields[1]);}  //����� � ��
 FldDscr &Enabled()const{return(*fields[2]);}//���������� ������
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//�������������� ���� ��� (��������)
class kpe_afb_tit :public TypeDscr
{
public:
 kpe_afb_tit();
 FldDscr &Parent()const{return(*fields[0]);}//��� ��, � ������� ���������� ������ �������
 FldDscr &Place()const{return(*fields[1]);}//����� � ��
 FldDscr &Enabled()const{return(*fields[2]);}//���������� ������
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//�������� ��
class kpe_ts :public TypeDscr
{
public:
  kpe_ts();
  FldDscr &Parent()const{return(*fields[0]);}//��� ������� ���� KPE_AFB_TS, � �������� ���������� ������ ������
  FldDscr &Num()const{return(*fields[1]);}//����� � ��
  FldDscr &Enabled()const{return(*fields[2]);}//������� ���������� ������
  FldDscr &Value()const{return(*fields[3]);}//'������� �������� (0, 1)'
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//��� (��������)
class kpe_tit :public TypeDscr
{
public:
  kpe_tit();
  FldDscr &Parent()const{return(*fields[0]);}//��� ���, �������� ����������� ������ ������
  FldDscr &Num()const{return(*fields[1]);}//����� � ��
  FldDscr &Enabled()const{return(*fields[2]);}//������� ���������� ������
  FldDscr &Value()const{return(*fields[3]);}//'������� �������� � �������';
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