/***********************************************************
               created by M.E.N
***********************************************************/

#ifndef _kpe_afb_h_
#define _kpe_afb_h_

#include <kp_types.h>
#include <kpe_types.h>
#include <kp_packets.h>
#include <kp_header.h>
#include <io_srv_control_object.h>

class KpeDev;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class KpeAfb : public ControlObject
{
public:
  KpeAfb(obj_d&);
  virtual ~KpeAfb(){}
 virtual unsigned char Place()const;       //место в КП (1-16)
 virtual const char *Parent()const;        //имя объекта - КП
 virtual const unsigned char *Step(int &);
protected:
  KpeDev *parent;
};
//----------------------------------------------------------------
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif //kpe_afb