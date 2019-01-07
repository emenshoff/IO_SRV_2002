/***********************************************************
 ������ ���������� ����� ������� � ����� ������������� (�����������)
  ��������������
               created by M.E.N
***********************************************************/

#ifndef _kp_tit_h_
#define _kp_tit_h_

#include <kp_types.h>
#include <kp_titi.h>
#include <io_srv_control_object.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class TITCtl : public ControlObject
{
 friend class KpConfig;
public:
 TITCtl(obj_d &d);
 void SetValue(unsigned char val);     //��������� �������� (�������� �������)   
 void SetError(unsigned char err);     //��������� ��������������� 
 const char *Parent()const;
 bool Enabled()const;
 unsigned char Num()const;
 inline unsigned char Error()const;
 virtual unsigned Type()const{return TIT;}
private:  
 inline float Ka()const;//'��������'
 inline float Kb()const;//'���������' 
 inline unsigned char Gist()const;//������� ����������� �� fHi, �� ��������� �������� ��� (������������������ � ��������� ��������� ��������)
 inline char Jump()const;//��������� �������� "������" ��� 0: ���� �� 0 � ��������� �������� ��� ������ (��� ������) �������� Jump, ����� ��������� ������� "������": -1 -���� ����; +1, ���� �����; � ��� �� ���� �������� ������. ** ��������� "������" ����� ���� ������������� ����������� ��������� Gist. 
 inline float Nominal()const;//'����������� �������� � ����������� ��������'; 
 inline float HighTech()const;
 inline float LowTech()const;
 inline float HighAlarm()const;
 inline float LowAlarm()const;
 inline unsigned char MaxJump()const;                //'���������� ������ ������';
 unsigned char last_val;                             //���������� �������� (�����)  
 TITiCtl *titi_d;                                    //���. ������
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif //kp_tit