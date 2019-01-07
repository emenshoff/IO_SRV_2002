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
  {"ChannelName",		tm_string},//�����, � �������� ��������� "��"
  {"Address",		tm_byte},      //����� �� �� ������  
  {"Enabled",		tm_byte},      //���������� ������
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
  {"Parent",    tm_string},//��� ��, � ������� ���������� ������ �������  
  {"Place",     tm_byte},//����� � ��
  {"Enabled",   tm_byte},//���������� ������
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
  {"Parent",    tm_string},//��� ��, � ������� ���������� ������ �������
  {"Place",     tm_byte},//����� � ��
  {"Enabled",   tm_byte},//���������� ������
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
  {"Parent",	tm_string},//��� ������� ���� KPE_AFB_TS, � �������� ���������� ������ ������
  {"Num",	    tm_byte},//����� � ��
  {"Enabled",	tm_byte},//������� ���������� ������
  {"Value",	  tm_byte},//'������� �������� (0, 1)'
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
   {"Parent",		tm_string},//��� ���, �������� ����������� ������ ������
   {"Num",		tm_byte},//����� � ��
   {"Enabled",		tm_byte},//������� ���������� ������
   {"Value",		tm_byte},//'������� �������� � �������';
  };
  FldDscr *fld;
  for(int i=0;i<4;i++)
  {
    fld=new FldDscr(dscr[i].Name,dscr[i].Type);
    fields.push_back(fld);
  }
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%