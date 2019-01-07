/***********************************************************
  ������� ������������ ������������� (�� ����������)
               created by M.E.N
***********************************************************/

#ifndef _kp_fb_tii_h_
#define _kp_fb_tii_h_

#include <kp_afb.h>
#include <kp_tii.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//������� ����� ����� �� ��������������!
class FbTii : public AFB
{
 friend class KpControl;
 friend class KpConfig;
public:
 FbTii(obj_d &d);
 ~FbTii();
 virtual int Step();
 virtual void ProcPacket(const unsigned char *,unsigned len);
 virtual void SetError(unsigned char err);
 bool Enabled()const;
 const char *Parent()const;
 virtual unsigned char Place()const;                  //����� � �� (0-15)
 //��������� �� Tii
 enum{
  FB_TII_BEGIN,
  FB_TII_STEP,
  FB_TII_ERROR
 };
private:
 vector<TIICtl *> Items;
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif //kp_fb_tii