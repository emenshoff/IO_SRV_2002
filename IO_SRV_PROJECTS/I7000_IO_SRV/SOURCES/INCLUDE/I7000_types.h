/***********************************************************
               created by M.E.N
***********************************************************/

#ifndef _I7000_types_h_
#define _I7000_types_h_

#include <io_srv_drv_types.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//�������� ���� "���������������� ���� ��"
class Comm_Dev_Descr :public TypeDscr
{
public:
 Comm_Dev_Descr();
 FldDscr &PortName()const{return *fields[0];}
 FldDscr &PortSpeed()const{return *fields[1];} 
 FldDscr &error()const{return *fields[2];}
 FldDscr &etime()const{return *fields[3];}
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class I7000_Ts_Descr : public TypeDscr
{
public:
 I7000_Ts_Descr();
 FldDscr &Parent()  const{return *fields[0];} //��� ������� - ����������
 FldDscr &Num()     const{return *fields[1];} //����� ����� ����������
 FldDscr &Enabled() const{return *fields[2];} //������� ���������� ������
 FldDscr &Inverted()const{return *fields[3];} //��������
 //�������� ����:
 FldDscr &value()   const{return *fields[4];} //������� ��������
 FldDscr &time()    const{return *fields[5];} //����� ��������� ���������
 FldDscr &error()   const{return *fields[6];} //���������������
private:
};
//----------------------------------------------------------------
class I7000_Tu_Descr : public TypeDscr
{
public:
 I7000_Tu_Descr();
 FldDscr &Parent()const       {return *fields[0];}  //��� ������� ����, � �������� ��������� ������ ����� ��
 FldDscr &Num()const          {return *fields[1];}  //����� ������ (0-12)
 FldDscr &Enabled()const      {return *fields[2];}  //������� ���������� ������
 FldDscr &Value()const        {return *fields[3];}  //������� �����������
 FldDscr &TU_Control_TS()const{return *fields[4];}  //��� ������� ���� I7000_TS - �������� ��
 FldDscr &TU_Enable_TS()const {return *fields[5];}  //��� ������� ���� I7000_TS - ���������� ���������� ������ ��
 FldDscr &TURetryOn()const     {return *fields[6];}  //���������� ������� ��������� ��������������
 FldDscr &TURetryOff()const   {return *fields[7];}  //���������� ������� ���������� ��������������
 FldDscr &TUDelay()const      {return *fields[8];}  //�������� ����� ������� ������� �������������� (c)
 FldDscr &TUTimeOut()const    {return *fields[9];}  //������� ���������� ��������������  (c)
 FldDscr &result()const       {return *fields[10];} //��������� ���������� ��
  // 0  - ������� ���������, �������������� ���������
  // 1  - ������� ���������, �������������� ��������
  // -1 - �� ��������� �������������
  // -2 - �� ��������� ���������
  // -3 - ������� ���������
  // -4 - ������� � ������ ����������, ���� �����
  // -5 - ������� �� ������������� ��������������� ��-�
  // -100 - ��������� �� ���������� (��������� ���������)
 FldDscr &time()const{return *fields[11];} //����� ���������� �� ��� ����� ����������� ������
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
extern Comm_Dev_Descr COMM_DEV;
extern I7000_Ts_Descr I7000_TS;
extern I7000_Tu_Descr I7000_TU;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif //I7000_types