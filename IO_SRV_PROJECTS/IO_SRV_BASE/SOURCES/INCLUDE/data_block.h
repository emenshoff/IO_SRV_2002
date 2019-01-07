//---------------------------------------------------------------------------
// класс AZBlok(малость испохаблен)- теперь он сам по себе
//
//---------------------------------------------------------------------------
#ifndef DataBlockH
#define DataBlockH

#include <stdlib.h>
#include <string.h>     
#include "bin_data.h"
// «апись констант в варианте протокола 1
#ifndef TM_CONST
#define TM_CONST

#define TM_BYTE		1
#define TM_CHAR		2
#define TM_UINT		3
#define TM_INT 		4
#define TM_ULONG	5
#define TM_LONG		6
#define TM_FLOAT	7
#define TM_DOUBLE	8
#define TM_TIME		9
#define TM_STRING      10
#define TM_BLOK        11

#endif

// «апись констант в варианте протокола 2
#define STM_BLANK       0
#define STM_U1          TM_BYTE
#define STM_I1          TM_CHAR
#define STM_U2          TM_UINT
#define STM_I2          TM_INT
#define STM_U4          TM_ULONG
#define STM_I4          TM_LONG
#define STM_F4          TM_FLOAT
#define STM_F8          TM_DOUBLE
#define STM_TIME        TM_TIME  /* не совместимо по смыслу, но совместимо по формату */
#define STM_PACKET      TM_BLOK
// пока не реализованы
//#define STM_U8          12
//#define STM_I8          13
//#define STM_STRING      14
#define STM_MEMO        15


//#define AZBLOK_HEADER_TYPE unsigned short
//#define AZBLOK_MAX_SIZE    64000
#define AZBLOK_HEADER_TYPE unsigned long
#define AZBLOK_MAX_SIZE    50000000l

#ifndef DEF_AZBLOK_OPER
#define DEF_AZBLOK_OPER
typedef enum
{
  BlkCopy = 0,  //
  BlkInit = 1,
  BlkUse  = 2
} DataBlockOperation;
#endif //DEF_AZBLOK_OPER

class DataBlock {
public:
  int Err;

  DataBlock(unsigned Delta=1024);
  DataBlock(char * Data, DataBlockOperation Oper, unsigned Delta=1024, unsigned BufLen=0);
  virtual ~DataBlock();

  unsigned Size()            { return *((AZBLOK_HEADER_TYPE *)data); }
  unsigned GetHeadSize()     { return sizeof(AZBLOK_HEADER_TYPE); }
  unsigned GetReadSize()     { return RFlag; }
  void ResetRead()           { RFlag=GetHeadSize(); }
  char * GetRead()           { return (data+RFlag); }
  char * GetData()           { return data; }
  bool Next();
  char GetNextTyp()          { if(IsEnd()) return 0; else return *GetRead();}
  bool IsEnd()               { return ( RFlag>=Size() ); }
  bool SetMaxSize(unsigned NewSize);
  void Copy(DataBlock & blk);
  void Free();
  friend bool operator>>(DataBlock& b, unsigned char& d);
  friend bool operator>>(DataBlock& b, char& d);
  friend bool operator>>(DataBlock& b, short& d);
  friend bool operator>>(DataBlock& b, unsigned short& d);
  friend bool operator>>(DataBlock& b, long& d);
  friend bool operator>>(DataBlock& b, unsigned long& d);
  friend bool operator>>(DataBlock& b, float& d);
  friend bool operator>>(DataBlock& b, double& d);
  friend bool operator>>(DataBlock& b, BinData & d);
  friend bool operator>>(DataBlock& b, DataBlock & d)     { return b.Map(d); }
  friend bool operator>>(DataBlock& b, DataBlock * & d);  // вызывает конструктор
  bool Map(char * &d);
  bool Map(DataBlock & d);
  bool NextIsBlank(); // ¬озвращает TRUE и переходит на следующее
                      // поле, если тип пол€ STM_BLANK
private:
  unsigned RFlag;
  unsigned BufLen;
  unsigned DeltaBlok;
  unsigned MaxSize;
  bool ExternData;
  char * data;

  bool IsAdd(unsigned size);
  AZBLOK_HEADER_TYPE * GetSize(const char * d) { return (AZBLOK_HEADER_TYPE *)d; }
  AZBLOK_HEADER_TYPE * GetSize() { return (AZBLOK_HEADER_TYPE *)data; }
  char * GetEnd() { return (data+(*GetSize())); }
  
};


//---------------------------------------------------------------------------
#endif
