/***********************************************************
               created by M.E.N
***********************************************************/

#include <I7000_types.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Comm_Dev_Descr COMM_DEV;
I7000_Ts_Descr I7000_TS;
I7000_Tu_Descr I7000_TU;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Comm_Dev_Descr::Comm_Dev_Descr()
:TypeDscr("COMM_DEV")
{
  fields.reserve(4);
  FldDescription dscr[4]={
  {"PortName",	tm_string}, //��� �����
  {"PortSpeed",	tm_ulong},   //�������� �����
  {"error",	tm_byte},   //������ ����� (�� ��������, ������ ��������� �������� �.�.�.)
  {"etime",	tm_time}    //����� ����������� ������
  };
  FldDscr *fld;
  for(int i=0;i<4;i++)
  {
    fld=new FldDscr(dscr[i].Name,dscr[i].Type);
    fields.push_back(fld);
  }
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
I7000_Ts_Descr::I7000_Ts_Descr()
:TypeDscr("I7000_TS")
{
  fields.reserve(7);
  FldDescription dscr[7]={
  {"Parent",	tm_string}, //��� ������ - ��������
  {"Num",	    tm_byte},   //����� � ������
  {"Enabled",	tm_byte},   //���������� ������
  {"Inverted",tm_byte},   //��������
  {"value",	  tm_byte},   //�������� (���������)
  {"time",	  tm_time},   //����� ��������� ���������
  {"error",	  tm_byte},   //���������������
  };
  FldDscr *fld;
  for(int i=0;i<7;i++)
  {
    fld=new FldDscr(dscr[i].Name,dscr[i].Type);
    fields.push_back(fld);
  }
}
//----------------------------------------------------------------
I7000_Tu_Descr::I7000_Tu_Descr()
 :TypeDscr("I7000_TU")
{
  fields.reserve(12);
  FldDescription dscr[12]={
  {"Parent",        tm_string}, //��� ������� ���� I7042_DEV, � �������� ��������� ������ ����� ��
  {"Num",           tm_byte}, //����� ������ (0-12)
  {"Enabled",       tm_byte}, //������� ���������� ������
  {"Value",         tm_byte}, //������� ����������
  {"TU_Control_TS", tm_string}, //��� ������� ���� I7000_TS - �������� ��
  {"TU_Enable_TS",  tm_string}, //��� ������� ���� I7000_TS - ���������� ���������� ������ ��
  {"TURetyOn",      tm_byte}, //���������� ������� ��������� ��������������
  {"TURetryOff",    tm_byte}, //���������� ������� ���������� ��������������
  {"TUDelay",       tm_uint}, //�������� ����� ������� ������� �������������� (c)
  {"TUTimeOut",     tm_uint}, //������� ���������� ��������������  (c)
  {"result",        tm_char}, //��������� ���������� ��
  {"time",          tm_time}, //����� ���������� �� ��� ����� ����������� ������
  };
  FldDscr *fld;
  for(int i=0;i<12;i++)
  {
    fld=new FldDscr(dscr[i].Name,dscr[i].Type);
    fields.push_back(fld);
  }
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%