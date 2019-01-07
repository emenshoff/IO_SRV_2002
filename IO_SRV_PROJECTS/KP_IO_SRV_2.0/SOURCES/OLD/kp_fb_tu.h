/***********************************************************
             ������� ��������������
               created by M.E.N
***********************************************************/

#ifndef _kp_fb_tu_h_
#define _kp_fb_tu_h_

#include <kp_tu.h>
#include <kp_afb.h>

#define TU_COUNT 128
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class FbTu : public AFB
{
 friend class KpControl;
 friend class KpConfig;
public:
 FbTu(obj_d &d);
 ~FbTu();
 virtual void ProcPacket(const unsigned char *,unsigned len);  //�������� ���������� ������
 virtual char Step();                                 //��� ������ (���� ��������� ������� - �� ����������, �� ���� � �� �����. �������)
 virtual void SetError(unsigned char err);            //��������� ������ ���: �) ���� �� �������; �) 
 bool Enabled()const; 
 unsigned char Num()const{return num;}
 const char *Parent()const;
 virtual unsigned char Place()const;                  //����� � �� (0-15)
 virtual unsigned char Error()const;
 virtual unsigned Type()const{return FB_TU;}
 //��������� ����� ��������������
 enum{
  FB_TU_BEGIN,  //��������� ���������
  FB_TU_STEP,   //��� ������
  FB_TU_TIMEOUT,//
  FB_TU_TU,     //�������� ������ �������� �� ������� ��������������
  FB_TU_ERROR   //������ ��������
 };
private:
 void Shutdown();
 inline void SendTU(unsigned char on,unsigned char grp,unsigned char obj);  //������� ������� �� �������������� 
 inline void ItemsStep();                             //��� ��������� ������������
 void SendKvit(){}
 unsigned char num;           //����� � �� (1 ��� 2)
 TUCtl * Items[TU_COUNT];       //128 ���������� �� ������� - ��
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif //kp_fb_tu