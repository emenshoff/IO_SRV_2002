/************************************************************************
        Binary Data class by M.E.N.
************************************************************************/
#ifndef _bin_data_h
#define _bin_data_h

#include <string.h>
#include <stdlib.h>

#define SET_NULL true;

extern void *extern_data;

class BinData
{

enum{BIN_DATA,STR_DATA};

public:

  BinData():len(0),data(NULL),Type(BIN_DATA){}
//--------------------------------------------------------------------------------------
  BinData(unsigned sz,bool opt)      {
                                      data=malloc(len=sz);
                                      Type=BIN_DATA;
                                      if(opt)
                                        memset(data,0,len);
                                     }//если opt=0, то не обнуляется
//--------------------------------------------------------------------------------------
  BinData(const void *d,unsigned sz) {
                                       data=malloc(len=sz);
                                       memcpy(data,d,sz);
                                       Type=BIN_DATA;
                                     }
//--------------------------------------------------------------------------------------
  explicit BinData(const char *d)    {
                                       data=strdup(d);
                                       len = strlen((char*)data)+1;
                                       Type=STR_DATA;
                                     }
//--------------------------------------------------------------------------------------
  BinData(BinData &d)                {
                                       Type=d.Type;
                                       if(Type==BIN_DATA)
                                       {
                                         data=malloc(len=d.len);
                                         memcpy(data,d.data,len);
                                         Type=BIN_DATA;
                                       }
                                       else
                                       {
                                         data=strdup(d.c_str());
                                         Type=STR_DATA;
                                       }
                                     }
//--------------------------------------------------------------------------------------
  virtual ~BinData()                 {
                                       if(len)
                                         free(data);
                                     }
//--------------------------------------------------------------------------------------
  unsigned Len()const                {
                                       if(Type==BIN_DATA)
                                         return len;
                                       if(Type==STR_DATA)
                                         return strlen((const char *)data);
                                       return 0;
                                     }
//--------------------------------------------------------------------------------------
  bool IsBin()const                   {return Type==BIN_DATA;}
//--------------------------------------------------------------------------------------
  bool IsStr()const                   {return Type==STR_DATA;}
//--------------------------------------------------------------------------------------
  const void *Data()const            {return data;}
//--------------------------------------------------------------------------------------
  void Free()                        {
                                       if(len)
                                         free(data);
                                       len=0;
                                       data=0;
                                     }
//--------------------------------------------------------------------------------------
  operator unsigned char*()         {
                                      return (unsigned char*)data;
                                    }
//--------------------------------------------------------------------------------------
  void operator=(const char *d)      {
                                       Free();
                                       data=strdup(d);
                                       Type=STR_DATA;
                                     }
//--------------------------------------------------------------------------------------
  void operator=(const BinData &d)         {
                                       if(&d==this)
                                         return;
                                       Free();
                                       if((Type=d.Type)==BIN_DATA)
                                       {
                                         data=malloc(len=d.len);
                                         memcpy(data,d.data,len);
                                         Type=BIN_DATA;
                                       }
                                       else
                                       {
                                         data=strdup(d.c_str());
                                         Type=STR_DATA;
                                       }
                                     }
//--------------------------------------------------------------------------------------
  void SetData(const char *d)        {
                                       Free();
                                       data=strdup(d);
                                       len = strlen(d)+1;
                                       Type=STR_DATA;
                                     }
//--------------------------------------------------------------------------------------
  void SetData(const void *d,unsigned sz)
                                       {
                                         Free();
                                         data=malloc(len=sz);
                                         memcpy(data,d,sz);
                                         Type=BIN_DATA;
                                       }
//--------------------------------------------------------------------------------------
void SetLen(unsigned BloksCount, unsigned BlokLen) 
                                    {
                                      Free();
                                      data=calloc(BloksCount,BlokLen);
                                      Type=BIN_DATA;
                                      len = BloksCount*BlokLen;
                                     }
//--------------------------------------------------------------------------------------
  const char *c_str()const           {
                                       if(Type==STR_DATA)
                                         return (data==0) ? "" : (const char *)data;
                                       return 0;
                                     }
//--------------------------------------------------------------------------------------
protected:
  void *data;
  unsigned len;
  unsigned char Type;
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif