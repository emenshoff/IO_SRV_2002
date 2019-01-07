/********************************************************************
	        	����� ��� ������������� �� "������"
                     created by M.E.N.
********************************************************************/
/*
��� ��������� ������ ���������� � ��������� ��, � �� ������!
*/
#ifndef _kp_dev_h
#define _kp_dev_h

#include <kp_types.h>
#include <kp_header.h>
#include <kp_afb.h>
#include <kp_cdv.h>
#include <kp_abstract_dev.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//����� - ����������� ����� ���������� (��) "������"
//--------------------------------------------------------------------
class KP : public KpAbstractDev
{
 friend class KpCnl;
 friend class KpConfig;
public:
 KP(obj_d &d);
 ~KP();
 virtual unsigned Type()const{return KP_DEVICE;}
 virtual void ProcPacket(const unsigned char *,unsigned len);  //��������� ������ ������   
 void SetFbError(unsigned char n,unsigned char err);   //��������� ������ ������ ��
 virtual void Step();       //��� ������ 
 void SetFbTiiPollFlag();   //
 void SetFbTitPollFlag();   //
 void SetFbTsPollFlag();    // 
 void SetError(unsigned char err);
 void Enable();
 void Disable();
 bool Enabled()const;       //������� ���������� ������
 virtual const char *Parent()const; 
 //��������� ��������� ��������:
 enum{
  KP_BEGIN,       //������ ������
  KP_STEP,        //������� ����
  KP_POLL_TS,     //����� ��
  KP_POLL_TIT,    //����� ���
  KP_ERROR,       //����� ��
  KP_DISABLED     //���������� ��
 };
private: 
 inline void SetFbError(int,unsigned char);//��������� ���������������
 inline void SetFbsError(unsigned char);
 void Shutdown();             //���������� ������  
 inline void PollFbTs();      //����� ���� ��������� ��
 inline void PollFbTit();     //����� ���� ��������� ���
 inline void PollFbTii();     //����� ���� ��������� ���
 void Kvit(unsigned char pl); //������������ ��, ������� ��� � ����� (� ������� ����� ������!) 
 long poll_begin_time;        //����� ������ ������
 unsigned char state;         //������� ��������� ���. �������� 
 unsigned char poll_flags;    //����� ������
 unsigned char old_poll_flags;
 unsigned char msg[2];
 AFB *Items[16];              //�����. �����
 double ItemsInTime[16];      //����� ������� ������� �� ��������� - 16.10.01
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif