/***********************************************************
       ������� ������� �������������.
               created by M.E.N
***********************************************************/

#ifndef _kp_fb_tit_h_
#define _kp_fb_tit_h_

#include <kp_tit.h>
#include <kp_afb.h>

#define TIT_COUNT 32
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class FbTit : public AFB
{
 friend class KpControl;
 friend class KpConfig;
public:
 FbTit(obj_d &d);
 virtual ~FbTit();
 virtual char Step();  //��� ������ ���. �������� 
 virtual void Disable();
 virtual void Enable();
 virtual void ProcPacket(const unsigned char *,unsigned len);     //������ ���������� ������
 virtual void SetError(unsigned char err); //��������� ��������������� 
 virtual const char *Parent()const;
 virtual unsigned char Place()const;       //����� � �� (0-15)
 virtual unsigned char Error()const;
 virtual unsigned Type()const{return FB_TIT;}
 bool Enabled()const;                      //������� ���������� ������
 //��������� �� TiT
 enum{
  FB_TIT_STEP=1,    //��� ������
  FB_TIT_TIMEOUT,   //������� �������� �������
  FB_TIT_ERROR,     //������ �������� (�����)
  FB_TIT_DISABLED   //������ ���������
 };
private:
 inline void WriteStates(int grp);         //������ ��������
 time_t poll_delay_begin_time;             //�������� ����� �������� �� ��������� ��������
 TITCtl *Items[TIT_COUNT];                 //������� - ���-�
 //������ 16 ���� - ����. ������ ������ ���, ������, ��������������, ������ ������. ����� - ��������� ��������, ���������� � ������� - ���-�
 unsigned char tmp_data[64]; 
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif //kp_fb_tit