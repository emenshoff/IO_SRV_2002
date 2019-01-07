/***********************************************************
               created by M.E.N
***********************************************************/

#ifndef _I7041_dev_h_
#define _I7041_dev_h_


#include <I7000_dev.h>
#include <I7000_dev_digital_input.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class I7041_Dev : public I7000_Dev
{
   friend class I7000_Config;
public:
 I7041_Dev(obj_d &d);
 ~I7041_Dev();
 int Step();
 void Init();
 unsigned short ModuleId()const{return 7041;}
 virtual unsigned char MaxItemsAmount()const;
 virtual const char *CommDevName()const; 
private:
  //��������� ��������� ��������:
 enum{
   ST_BEGIN,           //���. ���������    
   ST_DELAY,
   ST_FIRST_DATA_GET,  //������ ������ ������
   ST_SECOND_DATA_GET, //������ ������ ������ (���������� ��������)
   ST_WAIT,            //�������� ����   
   ST_TIMEOUT,         //
   ST_ERR              //����� ������
 };
 void SetError(unsigned char err); 
 inline unsigned short ValidTime()const;//������� �������� 
 unsigned err_count;
 unsigned char crc_enabled;     //������� ���������� CRC
 unsigned char state;           //������� ��������� ��������� ��������
 //cash_data[0] - ��������� ���������� ��������
 //cash_data[1] - �������� ��������������� � ������ ��������� ��������� ��������
 unsigned short cash_data[2];
 double delay_begin_time;//����� ������ ��������� �������� (���-� ��������)
 double last_in_time;   //����� ������� ���������� ������ 
 I7000_Ts *Items[14];
 void operator=(I7041_Dev&){}
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif