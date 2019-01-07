/***********************************************************
             ���� ������ �� ���
               created by M.E.N
***********************************************************/

#ifndef _kp_fb_light_h_
#define _kp_fb_light_h_

#include <kp_afb.h>
#include <kp_light.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class FbLight : public AFB
{
 friend class KpConfig;
public:
 FbLight(obj_d &d);
 ~FbLight();  
 virtual int Step();                      //��� ������: �������� ��������� ��������� ��������-���� � �������
 virtual void SetError(unsigned char err){}//��������
 bool Enabled()const;                       //�������� ����� �� ���
 virtual void Shutdown();                   //������� ���� ����
 const char *Parent()const;
 virtual unsigned char Place()const;        //����� � �� (0-15)
 virtual unsigned char Error()const;
 virtual unsigned Type()const{return FB_LIGHT;}
 unsigned char MaxItems()const{return fb_type;}
 //��������� ����� ������ �� ���
 enum{ FB_LIGHT_BEGIN, FB_LIGHT_STEP };
private:
 inline int SendData();                    //������� ������
 inline bool StateChanged();
 //�������� �� ������� �������� ������
 unsigned short ResponceTimeoutVal()const{return 0;}
 bool StartPollDelay(time_t)const{return false;}
 bool ResponceTimeout()const{return false;}
 bool ErrorSetTimeout()const{return false;}
 void SendKvit(){}
 LightCtl **Items;                          //64 ��� 32 �����
 unsigned char fb_type;                     //��� �������� (64 ��� 32 �����)
 //������ 8 ���� - ������� ��������, ������ - ��������� ���������
 unsigned char tmp_data[16];
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif //kp_fb_light