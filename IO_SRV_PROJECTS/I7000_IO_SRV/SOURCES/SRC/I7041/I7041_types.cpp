/***********************************************************
               created by M.E.N
***********************************************************/

#include <I7041_types.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
I7041_Dev_Dscr::I7041_Dev_Dscr()
:TypeDscr("I7041_DEV")
{
  FldDescription dscr[6]={
  {"CommDev",	  tm_string}, //��� ������� - �����
  {"Address",	  tm_byte},   //����� �� ����������
  {"Enabled", 	tm_byte},   //���������� ������
  {"ValidTime",	tm_uint},   //������� ��������
  {"error",	    tm_byte},   //������ ����������
  {"etime",	    tm_time}    //����� ����������� ������
  };
  FldDscr *fld;
  for(int i=0;i<6;i++)
  {
    fld=new FldDscr(dscr[i].Name,dscr[i].Type);
    fields.push_back(fld);
  }
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
I7041_Dev_Dscr I7041_DEV;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
