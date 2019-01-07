/********************************************************************	
        ������������ ���������������� ����������� ������.
                     created by M.E.N.
********************************************************************/
#ifndef _kp_config_h
#define _kp_config_h

#include <stdio.h>
#include <kp_dev.h>
#include <kp_control.h>
#include <kp_functional.h>

using namespace std;
//second ���� �������� �����, ���������� ������� ��������� ���������������� �������.
typedef vector<pair<ControlObject *,unsigned char> > ObjVectorType;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class KpConfig
{
public:
  KpConfig(KpControl *ctl);
  ~KpConfig();
  bool Init();
private:
  bool Create();                  //�������� �������� ������, ����������� ��� ���������������� ���.
  void ClearAll(bool must_die);   //������� ��������
  void CheckForErrors();          //�������� �� ���������    
  void CheckTypesDescription();   //�������� ������������ �������� ����� � �� ����� � �����. (� ���������� ������� �� id)
  void PrintKillingMessage(const char *);     //����� ��������� �� �������� �������
  void Clear(ObjVectorType &vk,bool must_die);//������� ������� ��� �������� ������ ��������
  void Clear(ObjVectorType &vk,unsigned char flag,bool must_die);//---/-- � ��������� ������...
  void CheckForErrors(ObjVectorType &vk);
  //��������� ��������� ����������� ��������:
  ObjVectorType TSs;
  ObjVectorType TITs;
  ObjVectorType TITis;
  ObjVectorType TIIs;
  ObjVectorType TUs;
  ObjVectorType LGHs;
  ObjVectorType FBs;
  ObjVectorType KPs;
  ObjVectorType CNLs;
  ObjVectorType CDVs;  
  KpControl *kp_ctl;  
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif