/***********************************************************
               created by M.E.N
***********************************************************/

#ifndef _kpe_fb_tit_h_
#define _kpe_fb_tit_h_

#include <kpe_tit.h>
#include <kpe_afb.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class KpeFbTit : public KpeAfb
{
public:
  KpeFbTit(obj_d &);
  ~KpeFbTit();
  virtual unsigned Type()const;
  virtual unsigned char Place()const;  //����� � �� (1-16)
  virtual const char *Parent()const;   //��� ������� - ��
  virtual const unsigned char *Step(int &);
  enum
  {
    STEP,     //-----��� ������---------
    FIRST_G,  //������� ������ ������ ���
    SECOND_G  //----- ������ ������
  };
private:
  unsigned char state;
  unsigned char data[19];
  KpeTit *Items[32];
};
//----------------------------------------------------------------
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif //kpe_fb_tit