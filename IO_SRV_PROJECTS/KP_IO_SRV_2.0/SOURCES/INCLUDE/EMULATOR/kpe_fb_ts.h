/***********************************************************
               created by M.E.N
***********************************************************/

#ifndef _kpe_fb_ts_h_
#define _kpe_fb_ts_h_

#include <kpe_ts.h>
#include <kpe_afb.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class KpeFbTs : public KpeAfb
{
public:
  KpeFbTs(obj_d &);
  ~KpeFbTs();
  virtual unsigned Type()const;
  virtual unsigned char Place()const;  //место в КП (1-16)
  virtual const char *Parent()const;   //имя объекта - КП
  virtual const unsigned char *Step(int &);
private:
  unsigned char data[19];
  KpeTs *Items[64];
};
//----------------------------------------------------------------
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif //kpe_fb_ts