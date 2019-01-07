/***********************************************************************
            Переделка кошмарного класса XDRPacket....  :))
             (выброшены некоторые функции, но суть та же)
                        by M.E.N.
***********************************************************************/

#ifndef _xdr_utils_h
#define _xdr_utils_h
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//Проверял только со стековыми (локальными) экземплярами классов
#ifndef WIN32
  extern "C" {
  #include <rpc/types.h>
  #include <rpc/xdr.h>
  }
#else
   extern "C" {
   #include <xdr.h>
   }
#endif




//#include <val.h>
#include <stm_types.h>
//#include <win2koi_koi2win.h>
//-----------------------------------------------------------------------
typedef unsigned long ulong;
typedef unsigned short ushort;
typedef unsigned char uchar;
typedef unsigned int uint;

typedef enum{XDR_PACK_ERROR=1,XDR_UNPACK_ERROR}XDR_EXCEPT;

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class XDRPacket
{
public:
  XDRPacket(int direct,const void *data=0,int Ln=0);
  virtual ~XDRPacket()=0;                                //
  void Set(const void *data,int len,int direct);          //
  int BuffLen()const{return len;}
  int PacketLen(){return getpos();}
  void Free();    
  bool Ready()const{return ready;}
  const char *Buff()const{return buffer;}
  enum{DR_IN=0,DR_OUT};
protected:  
  XDR xdr;
  char *buffer;
  int len;
  bool ready;
  unsigned int getpos()
     {return (unsigned)xdr_getpos(&xdr);}
  bool_t setpos(XDR * xdrs, unsigned int pos)
     { return(*(bool_t  (*)(XDR *, unsigned int))((xdrs)->x_ops->x_setpostn))(xdrs, pos); }
};
//%%%%%%%%%%  Десериализация данных (SRV <- клиент) %%%%%%%%%%%%%%%%%%%%%%%%
class XDRIn : public XDRPacket
{
public:
  XDRIn():XDRPacket(DR_IN){}
  XDRIn(const void *data,int len):XDRPacket(DR_IN,data,len){} 
  void Set(const void *data,int len){XDRPacket::Set(data,len,DR_IN);}
  unsigned getUInt();
  int getInt();
  ulong getULong();
  long getLong();
  ushort getUShort();
  short getShort();
  char getChar();
  uchar getUChar();
  float getFloat();
  double getDouble();
  void getString(char * str, int maxSize, int doKoiWin);  
};
//%%%%%%%%%%% Сериализация данных (SRV->клиент) %%%%%%%%%%%%%%%%%%%%%%%%%%%%
#define XDR_BUFFER_SIZE    (1024 * 8)

class XDROut : public XDRPacket
{
public:
  XDROut(int len=XDR_BUFFER_SIZE):XDRPacket(DR_OUT,0,len){}
  void Set(const void *data,int len){XDRPacket::Set(data,len,DR_OUT);}
  void putUInt(unsigned param);
  void putInt(int param);
  void putULong(ulong param);
  void putLong(long param);
  void putUShort(ushort param);
  void putShort(short param);
  void putChar(char param);
  void putUChar(uchar param);
  void putFloat(float param);
  void putDouble(double param);
  void putString(const char * str, bool doWinKoi);
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif