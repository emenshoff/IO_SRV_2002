/***********************************************************
  ����� "�������� ����� ������ ��������� I7000"
               created by M.E.N
***********************************************************/

#ifndef _I7000_dev_digital_output_h_
#define _I7000_dev_digital_output_h_

#include <I7000_dev_output.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class I7000_DigitalOutput : public I7000_Device_Output
{
public:
 explicit I7000_DigitalOutput(obj_d &d):I7000_Device_Output(d){}
 virtual ~I7000_DigitalOutput(){}
 virtual bool Enabled()const{return false;}
protected:
};
//----------------------------------------------------------------
//��������������
class I7000_Tu : public I7000_DigitalOutput
{
 friend class I7000_Config;
public:
 explicit I7000_Tu(obj_d &d);
 virtual ~I7000_Tu();
 void SetError(unsigned char);  //��������� ���������������
 void SetVal(unsigned char val);//��������� �������� (��� ���������� �� ��������)
 unsigned Num()const;           //����� ������ � ����������
 unsigned char Step();          //���� ���������� "TU_STEP", �� ��� ��������������, "TU_ON" - ��������, "TU_OFF" - ��������� 
 unsigned char LastVal()const{return last_val;}//���. ���������
 const char *Parent()const;     
 const char *CtlTsName()const;
 const char *EnTsName()const;
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
private: inline bool Enabled()const;             //������� ���������� ������
 inline bool Timeout()const;             //������� ���������� �������������� �����
 inline bool DelayComplete()const;       //�������� ����� ������� �� ���������� 
 inline bool Value()const;               //���������, ���� �� ������� �� ����������  (1 - on; 0 - off) 
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
#endif //I7000_dev_digital_output