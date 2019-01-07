/***********************************************************
  ������������� ��������������� (�� �����������)
               created by M.E.N
***********************************************************/

#ifndef _kp_titi_h_
#define _kp_titi_h_

#include <kp_types.h>
#include <io_srv_control_object.h>
//%%%%%%%%%%% ������������� ������������ (���) %%%%%%%%%%%%%%%%%%%%%
//� ��������� ������ �� ����������� ��������� ����� ���� ��
class TIICtl:public ControlObject
{
 friend class KpControl;
 friend class KpConfig;
public:
 TIICtl(obj_d &d);
 const char *Parent()const;
 unsigned char Num()const;
 void SetError(unsigned char err);//��������� ���������������
 virtual unsigned Type()const{return TII;}
private: 
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif //kp_titi