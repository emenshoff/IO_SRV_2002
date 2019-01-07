/*            Типы пакетов протокола stm <-> client              */
#ifndef __IDENTS_H
#define __IDENTS_H
//%%%%%%%%%%%%% типы пакетов, приходящих от клиентов %%%%%%%%%%%%%%%%%%%%
enum {
  C_USER = 1,
  C_PASSW,
  C_SRVLIST,
  C_TYPELIST,
  C_OBJLIST,
  C_ADDREQ,
  C_DELREQ,
  C_GETOBJ,
  C_GETOBJS,
  C_SETOBJ
};
//%%%%%%%%%%%% типы пакетов, отсылаемых клиентам %%%%%%%%%%%%%%%%%%%%%%%%
enum {
  S_KEYS = 1,
  S_RET,
  S_VERSION,
  S_SRVLIST,
  S_TYPELIST,
  S_OBJLIST1,
  S_OBJLIST2,
  S_FLDVAL,
  S_OBJVAL,
  S_OBJECT,
  S_OBJECTS,
  S_OBJEND,
  S_SETOBJ
};
#endif

