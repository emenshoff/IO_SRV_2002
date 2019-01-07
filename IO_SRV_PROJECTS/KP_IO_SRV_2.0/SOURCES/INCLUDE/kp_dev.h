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
 //����, �� ����� ����, ������� - �������, ����, ���:
 friend class FbLight;
 friend class FbTu;
public:
 KP(obj_d &d);
 ~KP();
 virtual unsigned Type()const{return KP_DEVICE;}
 virtual void ProcPacket(const unsigned char *,unsigned len);  //��������� ������ ������    
 virtual void Step();       //��� ������ 
 //virtual void Send(const unsigned char *data,unsigned len);
 //void SetFbError(unsigned char n,unsigned char err);   //��������� ������ ������ ��  
 void SetError(unsigned char err);
 bool Enabled()const;       //������� ���������� ������
 unsigned short ResponceTimeoutVal()const;
 unsigned char ErrorSetTimeoutVal()const;
 unsigned char StartPollVal()const;
 virtual const char *Parent()const; 
 //��������� ��������� ��������:
 enum{
  KP_BEGIN,       //������ ������
  KP_STEP,        //������� ����
  KP_POLL,        //����� //18.04.02
  //KP_POLL_TS,     //����� �� //18.04.02
  //KP_POLL_TIT,    //����� ��� //18.04.02
  KP_ERROR,       //����� ��
  KP_DISABLED     //���������� ��
 };
private:   
 //inline void SetFbError(int,unsigned char);//��������� ���������������
 //inline void SetFbsError(unsigned char);
 //void Shutdown();             //���������� ������  
 void SetFbTiiPollFlag();   //
 void SetFbTitPollFlag();   //
 void SetFbTsPollFlag();    //
 void ProcessFbs();
 inline void PollFbTs();      //����� ���� ��������� ��
 inline void PollFbTit();     //����� ���� ��������� ���
 inline void PollFbTii();     //����� ���� ��������� ���
 inline void Kvit(unsigned char pl); //������������ ��, ������� ��� � ����� (� ������� ����� ������!) 
 time_t last_ts_in_time;      //����� ������� ��������� ������� �� �������� ��
 time_t last_tit_in_time;     //����� ������� ��������� ������� �� �������� ���
 time_t last_ts_poll_time;    //����� ���������� ������ ��
 time_t last_tit_poll_time;   //����� ���������� ������ ���
 unsigned char state;         //������� ��������� ���. �������� 
 unsigned char poll_flags;    //����� ������
 unsigned char msg[2];        //������� ������������ ��������
 AFB *Items[16];              //�����. �����
 int f_ts,f_tit;
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif