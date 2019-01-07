/***********************************************************
  �����, ��������������� ���� ������ I7042 (�������� �����
  �� 13 �������)
               created by M.E.N
***********************************************************/

#ifndef _I7042_dev_h_
#define _I7042_dev_h_

#include <I7000_dev.h>
#include <I7000_dev_digital_output.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class I7042_Dev : public I7000_Dev
{
   friend class I7000_Config;
public:
 I7042_Dev(obj_d &d);
 ~I7042_Dev();  
 int Step();           //��� ������
 void Init();          //�������������
 void SetError(unsigned char err);//��������� ���������������
 unsigned short ModuleId()const{return 7042;}
 unsigned char MaxItemsAmount()const;
 virtual const char *CommDevName()const; 
private:
  //���������� ��������� ��������� ��������:
 enum
 {
   ST_BEGIN,                //��������� ���������
   ST_OUT_DO_TU,            //������� ��������������
   ST_IN_DO_TU,             //��������� ���������
   //ST_OUT_GET_OUTS_STATUS,  //������� ������� ������� �������
   ST_IN_GET_OUTS_STATUS,   //����� ������� �������
   ST_WAIT,                 //�������� ���
   ST_ERR                   //����� ������
 };
 
 unsigned char state;//������� ��������� ��������� ��������
 unsigned char crc_enabled;//������� ���������� ����������� �����
 unsigned short curr_data; //������� ��������� �������
 double last_in_time;      //����� ������� ��������� ������� �� ������
 I7000_Tu *Items[13];      //������� - ��
};
//----------------------------------------------------------------
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif //I7042_dev