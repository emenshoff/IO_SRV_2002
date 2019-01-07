#include <kp_types.h>

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
comm_dev     COMM_DEV;
kp_channel   KP_CHANNEL;
kp_dev       KP_DEV;
kp_afb_ts    KP_AFB_TS;
kp_afb_tit   KP_AFB_TIT;
kp_afb_tii   KP_AFB_TII;
kp_afb_tu    KP_AFB_TU;
kp_afb_light KP_AFB_LIGHT;
kp_ts        KP_TS;
kp_tit       KP_TIT;
kp_tii       KP_TII;
kp_titi      KP_TITi;
kp_light     KP_LIGHT;
kp_tu        KP_TU;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
comm_dev::comm_dev()
:TypeDscr("COMM_DEV")
{
  fields.reserve(4);
  FldDescription dscr[4]={
  {"PortName",	tm_string}, //��� �����
  {"PortSpeed",	tm_ulong}, //�������� �����
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
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
kp_channel::kp_channel()
:TypeDscr("KP_CHANNEL")
{
  fields.reserve(8);
  FldDescription dscr[8]={
  {"CommDev",	tm_string}, //��� ������� ���� COMM_DEV
  {"Number" , tm_byte},   //����� ������  
  {"KpSpeed",	tm_uint},   //�������� ������������� ������
  {"error",	tm_byte},     //������ (�� ��������, ������ ��������� �������� �.�.�.)
  {"recived_count",	tm_ulong},      //����� �������� �������
  {"sended_count",	tm_ulong},      //����� ���������� �������
  {"transfer_err_count",	tm_ulong},//����� ������ ��������
  {"nonkvit_count",	tm_ulong},      //����� �����������
  };
  FldDscr *fld;
  for(int i=0;i<8;i++)
  {
    fld=new FldDscr(dscr[i].Name,dscr[i].Type);
    fields.push_back(fld);
  }
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
kp_dev::kp_dev()
:TypeDscr("KP_DEV")
{
  fields.reserve(8);
  FldDescription dscr[40]={
  {"ChannelName",		tm_string},//�����, � �������� ��������� ��
  {"Address",		tm_byte}, //����� �� �� ������
  {"ErrorTimeout",  	tm_byte},//����� ���� �� ResponceTimeout(1-10) ��� ���������� �������������� ��������, ����������� �� �������� ��������������� �����
  {"ResponceTimeout",	tm_uint},//���������� �������� �������� ������� (���) (�������� ���������� �������� 15 ��� - 30 ���);  
  {"StartPollDelay",	tm_byte},//�������� ������ ������� �� ����� ������ ��������� ResponceTimeout - (25%-95%) �������� ResponceTimeout
  {"Enabled",		tm_byte},//���������� ������
  {"error",		tm_byte},//'����� ��';
  {"etime",		tm_time},//'����� ������ ��';
  {"fb16_error",		tm_byte},//'����� ��������';
  {"fb1_error",		tm_byte},//'����� ��������';
  {"fb2_error",		tm_byte},//'����� ��������';  
  {"fb3_error",		tm_byte},//'����� ��������';  
  {"fb4_error",		tm_byte},//'����� ��������';
  {"fb5_error",		tm_byte},//'����� ��������';
  {"fb6_error",		tm_byte},//'����� ��������';
  {"fb7_error",		tm_byte},//'����� ��������';
  {"fb8_error",		tm_byte},//'����� ��������';
  {"fb9_error",		tm_byte},//'����� ��������';
  {"fb10_error",		tm_byte},//'����� ��������';
  {"fb11_error",		tm_byte},//'����� ��������';
  {"fb12_error",		tm_byte},//'����� ��������';
  {"fb13_error",		tm_byte},//'����� ��������';
  {"fb14_error",		tm_byte},//'����� ��������';
  {"fb15_error",		tm_byte},//'����� ��������';
  {"fb16_etime",		tm_time},//'����� ������ ��������;
  {"fb1_etime",		tm_time},//'����� ������ ��������;
  {"fb2_etime",		tm_time},//'����� ������ ��������;
  {"fb3_etime",		tm_time},//'����� ������ ��������;  
  {"fb4_etime",		tm_time},//'����� ������ ��������;  
  {"fb5_etime",		tm_time},//'����� ������ ��������;  
  {"fb6_etime",		tm_time},//'����� ������ ��������;  
  {"fb7_etime",		tm_time},//'����� ������ ��������;  
  {"fb8_etime",		tm_time},//'����� ������ ��������;  
  {"fb9_etime",		tm_time},//'����� ������ ��������;  
  {"fb10_etime",		tm_time},//'����� ������ ��������;  
  {"fb11_etime",		tm_time},//'����� ������ ��������;  
  {"fb12_etime",		tm_time},//'����� ������ ��������;  
  {"fb13_etime",		tm_time},//'����� ������ ��������;  
  {"fb14_etime",		tm_time},//'����� ������ ��������;  
  {"fb15_etime",		tm_time},//'����� ������ ��������;
  };
  FldDscr *fld;
  for(int i=0;i<40;i++)
  {
    fld=new FldDscr(dscr[i].Name,dscr[i].Type);
    fields.push_back(fld);
  }
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
kp_afb_ts::kp_afb_ts()
:TypeDscr("KP_AFB_TS")
{
  fields.reserve(6);
  FldDescription dscr[6]={
  {"Parent",    tm_string},//��� ��, � ������� ���������� ������ �������  
  {"ValidTime", tm_uint},//������� ��������, ��
  {"Place",     tm_byte},//����� � ��
  {"Enabled",   tm_byte},//���������� ������
  {"error",     tm_byte},//'�����'
  {"etime",     tm_time},//'����� ������'
  };
  FldDscr *fld;
  for(int i=0;i<6;i++)
  {
    fld=new FldDscr(dscr[i].Name,dscr[i].Type);
    fields.push_back(fld);
  }
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
kp_afb_tit::kp_afb_tit()
:TypeDscr("KP_AFB_TIT")
{
  fields.reserve(5);
  FldDescription dscr[5]={
  {"Parent",    tm_string},//��� ��, � ������� ���������� ������ �������
  {"Place",     tm_byte},//����� � ��
  {"Enabled",   tm_byte},//���������� ������
  {"error",     tm_byte},//'�����'
  {"etime",     tm_time},//'����� ������'
  };
  FldDscr *fld;
  for(int i=0;i<5;i++)
  {
    fld=new FldDscr(dscr[i].Name,dscr[i].Type);
    fields.push_back(fld);
  }
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
kp_afb_tii::kp_afb_tii()
:TypeDscr("KP_AFB_TII")
{
  fields.reserve(5);
  FldDescription dscr[5]={
  {"Parent",    tm_string},//��� ��, � ������� ���������� ������ �������
  {"Place",     tm_byte},//����� � ��
  {"Enabled",   tm_byte},//���������� ������
  {"error",     tm_byte},//'�����'
  {"etime",     tm_time},//'����� ������'
  };
  FldDscr *fld;
  for(int i=0;i<5;i++)
  {
    fld=new FldDscr(dscr[i].Name,dscr[i].Type);
    fields.push_back(fld);
  }
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
kp_afb_tu::kp_afb_tu()
:TypeDscr("KP_AFB_TU")
{
  fields.reserve(6);
  FldDescription dscr[6]={
  {"Parent", tm_string},//��� ��, � ������� ���������� ������ �������
  {"Place",  tm_byte},//����� � ��
  {"Num",    tm_byte},//����� �������� (1..2)
  {"Enabled",tm_byte},//���������� ������
  {"error",  tm_byte},//'�����'
  {"etime",  tm_time},//'����� ������'
  };
  FldDscr *fld;
  for(int i=0;i<6;i++)
  {
    fld=new FldDscr(dscr[i].Name,dscr[i].Type);
    fields.push_back(fld);
  }
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
kp_afb_light::kp_afb_light()
:TypeDscr("KP_AFB_LIGHT")
{
  fields.reserve(6);
  FldDescription dscr[6]={
  {"Parent", tm_string},//��� ��, � ������� ���������� ������ �������
  {"Place",  tm_byte},//����� � ��
  {"Type",   tm_byte,},
  {"Enabled",tm_byte},//���������� ������
  {"error",  tm_byte},//'�����'
  {"etime",  tm_time},//'����� ������'
  };
  FldDscr *fld;
  for(int i=0;i<6;i++)
  {
    fld=new FldDscr(dscr[i].Name,dscr[i].Type);
    fields.push_back(fld);
  }
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
kp_ts::kp_ts()
:TypeDscr("KP_TS")
{
  fields.reserve(7);
  FldDescription dscr[7]={
  {"Parent",	tm_string},//��� ������� ���� KP_AFB_TS, � �������� ���������� ������ ������
  {"Num",	tm_byte},//����� � ��
  {"Enabled",	tm_byte},//������� ���������� ������
  {"Inverted",	tm_byte},//������� ���������� ��������
  {"value",	tm_byte},//'������� �������� (0, 1)';
  {"error",	tm_byte},//'������� ���������������';
  {"time",	tm_time},//'����� ����� �������� ��� ������������ ���������������';
  };
  FldDscr *fld;
  for(int i=0;i<7;i++)
  {
    fld=new FldDscr(dscr[i].Name,dscr[i].Type);
    fields.push_back(fld);
  }
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
kp_tit::kp_tit()
:TypeDscr("KP_TIT")
{
  fields.reserve(22);
  FldDescription dscr[22]={
   {"Parent",		tm_string},//��� ���, �������� ����������� ������ ������
   {"Num",		tm_byte},//����� � ��
   {"Enabled",		tm_byte},//������� ���������� ������
   {"TITid_Name",	tm_string},//���� �����, �� ������. �� ���������
   {"Jump",		tm_char},//��������� �������� "������" ��� 0: ���� �� 0 � ��������� �������� ��� ������ (��� ������) �������� Jump, ������� ����� ��������� ������� "������": -1 -���� ����; +1, ���� �����; � ��� �� ���� �������� ������. ** ��������� "������" ����� ���� ������������� ����������� ��������� Gist.
   {"Gist",		tm_byte},//������� ����������� �� fHi, �� ��������� �������� ��� (������������������ � ��������� ��������� ��������)
   {"Ka",		tm_float},//'��������';
   {"Kb",		tm_float},//'���������';
   {"LowTech",		tm_float},//'������  ��������������� ������';
   {"HighTech",		tm_float},//'������� ��������������� ������';
   {"LowAlarm",		tm_float},//'������  ��������� ������';
   {"HighAlarm",	tm_float},//'������� ��������� ������';
   {"max_jump",		tm_byte},//'���������� ������ ������'; - �������� ����
   {"Nominal",		tm_float},//'����������� �������� � ����������� ��������'; 
   // �����: �������� ���
   {"cvalue",		tm_byte},//'������� �������� � �������';
   {"error",		tm_byte},//'������� ���������������';
   {"cjump",		tm_int},//'������� �������� ������ � �������';
   {"limit",		tm_char},//'������� ������ �� ������';
    //  0 - �����,
    //  1 - ������� ���������������
    //  2 - ������� ���������,
    // -1 - ������ ���������������,
    // -2 - ������� ���������
   {"procent",		tm_float},//'������� �������� � ��������� �� ������������';
   {"jump",		    tm_char},//'������� ������ �� ������ ������ (0,-1,+1)';
   {"value",		  tm_float},//'������� �������� � ����������� ��������';
   {"time",		    tm_time},//����� ���������� ���������, ���� ��������� ���������������
  };
  FldDscr *fld;
  for(int i=0;i<22;i++)
  {
    fld=new FldDscr(dscr[i].Name,dscr[i].Type);
    fields.push_back(fld);
  }
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
kp_tii::kp_tii()
:TypeDscr("KP_TII")
{
  fields.reserve(5);
  FldDescription dscr[5]={
  {"Parent",	tm_string},//��� ���, �������� ����������� ������ ������
  {"Enabled",	tm_byte},//���������� ������
  {"Num",	    tm_byte},//����� � ���
  {"error",	  tm_byte},//'�����'
  {"time",	  tm_time},//����� ������ (������)
  };
  FldDscr *fld;
  for(int i=0;i<5;i++)
  {
    fld=new FldDscr(dscr[i].Name,dscr[i].Type);
    fields.push_back(fld);
  }
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
kp_titi::kp_titi()
:TypeDscr("KP_TITi")
{
  fields.reserve(15);
  FldDescription dscr[15]={
  {"IntegralValLimit1",	tm_float},//'������ 1'; !!�������� �������� ��������!!
  {"IntegralValLimit2",	tm_float},//'������ 2';
  {"IntegralValLimit3",	tm_float},//'������ 3';
  {"AverageValPeriod", 	tm_ulong},//'������ ��� ���������� �������� �������� � ��������'; (1,3,5,15,30,60 �.�.�)
  {"IntegratePeriod",	  tm_ulong},//'������ �������������� � ��������'; (1,3,5,15,30,60 �.�.�)
  //����� �������� TITid
  {"average_val",	      tm_float},//������� �������� �� ������
  {"integral_val",	    tm_float},//'������������ �������� �� ������
  {"average_time",	    tm_time},//'����� ��������� ���������� �������� ��������(����� �������)';
  {"integral_time",	    tm_time},//'����� ��������� ���������� ��������(����� �������)';
  {"error",		          tm_byte},//'�������������';
  {"avr_measures_count",tm_ulong},//'������� ����� �������'; (�����)
  {"itg_measures_count",tm_ulong},//'������� ����� �������'; (������)
  {"last_val",		      tm_float},//'��������� ����� (��. ��.)';
  {"limit",		          tm_char},//'������� ������ �� ������ (0, 1, 2, 3)';
  {"IntegralValDivider",tm_uint},//�������� ������������� �������� (��� ��������� ���������� � ����������� ��������
  };
  FldDscr *fld;
  for(int i=0;i<15;i++)
  {
    fld=new FldDscr(dscr[i].Name,dscr[i].Type);
    fields.push_back(fld);
  }
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
kp_light::kp_light()
:TypeDscr("KP_LIGHT")
{
  fields.reserve(4);
  FldDescription dscr[4]={
  {"Parent",	  tm_string},//��� ��������
  {"Num",     	tm_byte},//����� � ��������
  {"Enabled",	  tm_byte},//������� ���������� ������
  {"Condition",	tm_string},//���������� �������
  //{"value",	    tm_byte},//�������� (���� �� ������� �������, �� ���������, ��� ������ ���������� �����)
  };
  FldDscr *fld;
  for(int i=0;i<4;i++)
  {
    fld=new FldDscr(dscr[i].Name,dscr[i].Type);
    fields.push_back(fld);
  }
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
kp_tu::kp_tu()
:TypeDscr("KP_TU")
{
  fields.reserve(13);
  FldDescription dscr[13]={
  {"Parent",		    tm_string},//��� ��!
  {"TU_Control_TS",	tm_string},//�� ������������� ��������������
  {"TU_Enable_TS",	tm_string},//���������� ���������� ��
  {"GroupNum",		  tm_byte},//����� ������ (1..16)
  {"Num",		        tm_byte},//����� � ������ (1..8)
  {"Enabled",		    tm_byte},//������� ���������� ������
  {"TURetyOn",		  tm_byte},//���������� ������� ��������� ��������������
  {"TURetryOff",	  tm_byte},//���������� ������� ���������� ��������������
  {"TUDelay",		    tm_uint},//�������� ����� ������� ������� �������������� (c)
  {"TUTimeOut",		  tm_uint},//������� ���������� ��������������  (c)
  {"Value",		      tm_byte},//1 - �������� ��, 0 - ���������
  //�����
  {"result",		    tm_char},//��������� ���������� ��
  // 0  - ������� ���������, �������������� ���������
  // 1  - ������� ���������, �������������� ��������
  // -1 - �� ��������� �������������
  // -2 - �� ��������� ���������
  // -3 - ������� ���������
  // -4 - ������� � ������ ����������, ���� �����
  // -5 - ������� �� ������������� ��������������� ��-�
  // -100 - ��������� �� ���������� (��������� ���������)
  {"time",		      tm_time},//����� ���������� �� ��� ����� ����������� ������
  };
  FldDscr *fld;
  for(int i=0;i<13;i++)
  {
    fld=new FldDscr(dscr[i].Name,dscr[i].Type);
    fields.push_back(fld);
  }
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
