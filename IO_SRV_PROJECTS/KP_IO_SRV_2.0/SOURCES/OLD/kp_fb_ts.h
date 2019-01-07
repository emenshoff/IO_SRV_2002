/***********************************************************
   ������� ��
               created by M.E.N
***********************************************************/

#ifndef _kp_fb_ts_h_
#define _kp_fb_ts_h_

#include <kp_ts.h>
#include <kp_afb.h>

#define TS_COUNT 64
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class FbTs : public AFB
{
 friend class KpConfig;
public:
 FbTs(obj_d &d);
 virtual ~FbTs();
 virtual void ProcPacket(const unsigned char *,unsigned len);  //��������� ���������� �� �� ������ 
 virtual void SetError(unsigned char err);
 virtual void Disable();
 virtual void Enable();
 virtual char Step(); 
 virtual unsigned char Place()const;                  //����� � �� (0-15)
 virtual unsigned char Error()const;
 const char *Parent()const;
 virtual unsigned Type()const{return FB_TS;}
 bool Enabled()const;              //���������� ������
//��������� �� TS
 enum{
  FB_TS_TIMEOUT=1,//������� �������� ������� �����
  FB_TS_ERROR,    //������ ��
  FB_TS_WAIT,     //��������� ��������
  FB_TS_STEP,     //������� ����
  FB_TS_DISABLED  //���������� ������
 };
private:  
 inline void WriteStates();                //������ ��������� � ��-�
 inline unsigned short ValidTime()const;   //������� �������� 
 inline bool ClickTimeout()const;          //������� �������� �����
 time_t poll_delay_begin_time;             //�������� ����� �������� �� ��������� ��������
 TSCtl *Items[TS_COUNT];                   //��-� 
 unsigned char tmp_data[16];               //��������� ��������� ��������� �� (��� ���������� ��������)-� ������ �����
 //������ 8 ���� �������� ��������� ���������� � ������� ��-� ��������.
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif //kp_fb_ts