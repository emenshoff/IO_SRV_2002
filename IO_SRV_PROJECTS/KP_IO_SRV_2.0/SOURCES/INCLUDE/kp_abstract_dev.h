/********************************************************************
	        	����� ��� ������������� �� "������"
                     created by M.E.N.
********************************************************************/
#ifndef _kp_abstract_dev_
#define _kp_abstract_dev_

#include <kp_channel.h>
#include <io_srv_control_object.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class KpAbstractDev : public ControlObject
{
public:
 KpAbstractDev(obj_d &d);
 virtual ~KpAbstractDev(){}
 virtual const char *Parent()const;
 virtual void ProcPacket(const unsigned char *,unsigned len){}  //��������� ������ ������   
 virtual void Step(){}
 //virtual void Send(const unsigned char *data,unsigned len){}
 unsigned char Channel()const{return cnl->Num();}
 unsigned char Address()const{return addr;}   				 //����� �� 
protected:
 unsigned char addr;          //����� �� 
 KpCnl *cnl;                 //��������� �� ����. ����
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif