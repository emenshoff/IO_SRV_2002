/***********************************************************
  ����� ���������� ����� ������������� ��
               created by M.E.N
***********************************************************/

#ifndef _kp_titid_h_
#define _kp_titid_h_

#include <kp_types.h>
#include <io_srv_control_object.h>
//%%%%%%%%%%%%%%%%%% ������������� ������������� %%%%%%%%%%%%%%%%%%
class TITiCtl : public ControlObject
{
 friend class KpConfig;
 friend class KpControl;
 friend class TITCtl;
public:
 TITiCtl(obj_d &d);
 void Step();                                      //��� ����������
 void SetValue(float val);                         //(��������� ��������)
 void SetError(unsigned char err);                 //��������� ���������������
 virtual unsigned Type()const{return TITi;}
private:          
 inline void SetLastVal(float);                    //������ ���������� ��������  
 inline void SetValid(unsigned char);              //������ ��������������� 
 void UpdateItg();                                 //�������� �������� ������������ ����������
 void UpdateAvr();                                 //�������� �������� ������������ ����������
 inline unsigned short ItgValDiv()const;           //�������� ���������� ���. ��������
 inline float ItgLim1()const;                     //������1
 inline float ItgLim2()const;                     //������2
 inline float ItgLim3()const;                     //������3
 unsigned long itg_period;                         //������ ��������������
 unsigned long avr_val_period;                     //������ ���������� �������� �������� 
 unsigned long itg_measures_count;                 //������� ���������� ������� (������)
 unsigned long avr_measures_count;                 //������� ���������� ������� (�����)
 unsigned char state;                              //���. ���������
 float itg_sum_val;                               //������������ ��� ���������� ������. ��������
 float avr_sum_val;                               //������������ ��� ���������� �������. �������� 
 float last_val;                                   //��������� ��������
 struct timeval itg_last_time;                     //����� ���������� ������ (��� �������������� )
 time_t itg_begin_time;                            //����� ������ ��������������
 time_t avr_begin_time;                            //����� �����a ������ �������� ��������
 time_t last_time;                                 //����� ���������� ������ ��� ������. �� 
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif //kp_titid