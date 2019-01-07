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
  virtual unsigned char Place()const;  //место в КП (1-16)
  virtual const char *Parent()const;   //имя объекта - КП
  virtual const unsigned char *Step(int &);
  enum
  {
    STEP,     //-----шаг работы---------
    FIRST_G,  //отсылка первой группы тит
    SECOND_G  //----- второй группы
  };
private:
  unsigned char state;
  unsigned char data[19];
  KpeTit *Items[32];
};
//----------------------------------------------------------------
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif //kpe_fb_tit