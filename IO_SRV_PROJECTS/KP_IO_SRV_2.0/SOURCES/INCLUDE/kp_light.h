/***********************************************************
  ����� - ����� (����� �� ���).
               created by M.E.N
***********************************************************/

#ifndef _kp_light_h_
#define _kp_light_h_

#include <kp_types.h>
#include <cond_parser.h>
#include <io_srv_control_object.h>
//%%%%%%%%%%% ����� ���� %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class LightCtl:public ControlObject
{
 friend class KpConfig;
public:
 LightCtl(obj_d &d,CondParser &p);
 bool On()const;               //���. ��������� (���� true, �� ����� ������ ���������� (�������))
 const char *Parent()const;
 unsigned char Num()const;
 virtual unsigned Type()const{return LIGHT;}
private:
 inline bool ConditionPresent()const;             //�������� ������� �������
 inline bool CheckCondition()const;               //�������� ����������� �������
 inline unsigned char Enabled()const;             //������� ���������� ������ 
 CondParser &parser;                              //��������� �� ������-��������� ���������
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif //kp_light