/***********************************************************
               created by M.E.N
***********************************************************/

#ifndef _I7041_proto_h_
#define _I7041_proto_h_

#include <I7000_proto.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class I7041_Proto
{
public:
 explicit I7041_Proto(I7000_Proto *);
 ~I7041_Proto();
private:
 I7041_Proto(I7041_Proto &){}
 void operator=(I7041_Proto &){}
};
//----------------------------------------------------------------
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif //I7041_proto
