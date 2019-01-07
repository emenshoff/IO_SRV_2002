/***********************************************************
               created by M.E.N
***********************************************************/

#ifndef _kpe_tit_h_
#define _kpe_tit_h_

#include <kpe_types.h>
#include <io_srv_control_object.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class KpeTit : public ControlObject
{
public:
  KpeTit(obj_d&);
  virtual unsigned Type()const;
  unsigned char Value()const;
  inline bool Enabled()const;
  unsigned char Num()const;
  const char *Parent()const;
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif //kpe_tit