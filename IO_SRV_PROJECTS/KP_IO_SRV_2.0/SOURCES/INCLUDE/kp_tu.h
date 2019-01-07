/***********************************************************
  ����� ������� - ��������������.
               created by M.E.N
***********************************************************/

#ifndef _kp_tu_h_
#define _kp_tu_h_

#include <kp_types.h>
#include <io_srv_control_object.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%% �������������� %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
/*
������� �������, ��� �������������� ���������, ���� ��������� ��-�
 �������� �� ���������� ����� ������ �������
*/
class TUCtl : public ControlObject
{
 friend class KpConfig;
public:
 explicit TUCtl(obj_d &d);
 virtual ~TUCtl();
 void SetError(unsigned char);  //��������� ���������������
 void SetVal(unsigned char val);//��������� �������� (��� ���������� �� ��������)
 unsigned Num()const;           //����� � ������
 unsigned char Step();          //���� ���������� "TU_STEP", �� ��� ��������������, "TU_ON" - ��������, "TU_OFF" - ��������� 
 unsigned char LastVal()const{return last_val;}//���. ���������
 unsigned char GroupNum()const;   //����� ������ 
 const char *Parent()const;
 const char *CtlTsName()const;
 const char *EnTsName()const;
 virtual unsigned Type()const{return TU;}
 enum{  
  //��� �������� ���������� ������� ��� ������ ������� Step() :
  TU_STEP=0,  //��� ������
  TU_ON,      //������ ������� �� ��������� ��
  TU_OFF,     //�� ����������
  //��������� ��������� �������� :
  TU_TRY_ON,  //���������� ������� �������� ��������������
  TU_TRY_OFF, //���������� ������� ��������� ��������������  
  TU_DELAY_ON,//��������� �������� ����� ������� ��
  TU_DELAY_OFF
 };
private:
 inline bool Enabled()const;             //������� ���������� ������
 inline bool Timeout()const;             //������� ���������� �������������� �����
 inline bool DelayComplete()const;       //�������� ����� ������� �� ���������� 
 inline unsigned char Value()const;               //���������, ���� �� ������� �� ����������  (1 - on; 0 - off) 
 inline unsigned char CtlTsState()const; //��������� ����������� ������������� ���������� ��
 inline unsigned char NetTsState()const; //���� true, �� ����� ����� �� (���� ��������) 
 inline unsigned char TURetryOn()const;  //���������� ������� ��������� ��������������
 inline unsigned char TURetryOff()const; //���������� ������� ���������� ��������������
 inline unsigned short TUDelayVal()const;//�������� ����� ������� �������������� (c)
 inline unsigned short TUTimeOutVal()const; //������� ���������� �������������� (c) 
 inline void SetResult(char);            //������ ���������� ���������� �������
 void Shutdown();                        //���������� ������ (������ �����!)
 const obj_d *ts_ctl;                    //�������������� ��
 const obj_d *ts_net_ctl;                //�� �������� ����
 unsigned char try_count;                //������� ������� ������� ���������/���������� ��������������
 unsigned char state;                    //������� ��������� ��������� ��������
 unsigned char last_val;                 //���� � 1, �� � ��������� ������ �� ��������,���� � 0 - �� �������� 
 time_t tu_time;                         //����� ���������� ��������� ���������(��������) (���. ����� ��� ��������� ��������) 
 time_t delay_begin_time;                //����� ������ ��������� ��������
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif //kp_tu