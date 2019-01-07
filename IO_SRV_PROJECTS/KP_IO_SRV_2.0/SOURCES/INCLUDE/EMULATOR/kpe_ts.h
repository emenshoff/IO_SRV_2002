/***********************************************************
               created by M.E.N
***********************************************************/

#ifndef _kpe_ts_h_
#define _kpe_ts_h_

#include <kpe_types.h>
#include <io_srv_control_object.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class KpeTs : public ControlObject
{
public:
 KpeTs(obj_d &);
 virtual unsigned Type()const;
 unsigned char Value()const; 
 unsigned char Num()const;
 const char *Parent()const;
private:
 inline bool Enabled()const;
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif //kpe_ts