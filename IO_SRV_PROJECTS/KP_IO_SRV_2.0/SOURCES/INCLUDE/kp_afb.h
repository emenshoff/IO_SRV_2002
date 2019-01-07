/********************************************************************
  �����, ����������� �������������� � ���������� ���������������
  ������� ��.
                     created by M.E.N.
********************************************************************/
#ifndef _kp_afb_h
#define _kp_afb_h

#include <kp_types.h>
#include <kp_packets.h>
#include <kp_header.h>
#include <io_srv_control_object.h>

class KP;
class KpCommDev;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//������� ����� - �������������� ����
class AFB : public ControlObject
{
 friend class KpControl;
 friend class KpConfig;
public:
  enum{AFB_STEP,AFB_POLL,AFB_FREE};
 AFB(obj_d &d);
 virtual ~AFB(){}
 virtual void ProcPacket(const unsigned char *,unsigned len){} //��������� ���������� ������
 virtual int Step(){return AFB_FREE;}                //��� ������ ��������  
 virtual void SetError(unsigned char err);            //��������� ��������������� ���� �������� ��������
 virtual unsigned char Place()const{return 17;}       //����� � �� (1-16)
 virtual const char *Parent()const{return "";}        //��� ������� - ��
 virtual unsigned char Error()const{return 100;}      //������ ��������
 virtual bool Enabled()const{return false;} 
protected: 
 bool ResponceTimeout()const;                         //����� ������� ������
 bool ErrorSetTimeout()const;                         //��������� (ErrorTimeout * ReqTimeout) ��������� ������   
 unsigned char state;                                 //������� ��������� 
 struct timeval last_in_time;                         //����� ������� ��������� ������� 
 KP *parent;                                          //��������� �� ������ - ��
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif