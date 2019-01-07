#include <data_block.h>
//
void *extern_data=NULL;
// *****************************************************************************
// Class DataBlock
// *****************************************************************************
DataBlock::DataBlock(unsigned Delta)
{
  MaxSize=AZBLOK_MAX_SIZE;
  DeltaBlok=Delta;
  BufLen=GetHeadSize() + DeltaBlok;
  ExternData=false;
  Err=0;
  data=new char[BufLen];
  RFlag=*(GetSize())=sizeof(AZBLOK_HEADER_TYPE);
}

// -----------------------------------------------------------------------------
DataBlock::DataBlock(char * AData, DataBlockOperation Oper, unsigned Delta, unsigned aBufLen)
{
  unsigned u;
  Err=0;
  MaxSize=AZBLOK_MAX_SIZE;
  DeltaBlok=(Delta<GetHeadSize()) ? GetHeadSize() : Delta;
  switch(Oper)
  {
  case BlkCopy:
    ExternData=false;
    u=*GetSize(AData);
    BufLen=u;
    if(BufLen>=MaxSize)
      MaxSize=BufLen+1;
    data=new char[BufLen];
    if(data==0)
      Err=1;
    else
    {
      memcpy(data,AData,BufLen);
      ResetRead();
    }
    break;
  case BlkInit:
    ExternData=true;
    if(aBufLen<=GetHeadSize())
    {
      //Logdef(Print(DEBUG_ERROR,"Попытка инициализровать буфер размером %d меньше заголовка %d", aBufLen,GetHeadSize()))
      Err=1;
    }
    else
    {
      BufLen=aBufLen;
      MaxSize=BufLen+1;
      data=AData;
      RFlag=*(GetSize())=sizeof(AZBLOK_HEADER_TYPE);
    }
    break;
  case BlkUse:
    ExternData=true;
    BufLen=*GetSize(AData);
    MaxSize=BufLen+1;
    data=AData;
    ResetRead();
    break;
  }
}

// -----------------------------------------------------------------------------
DataBlock::~DataBlock()
{
  Free();
}

// -----------------------------------------------------------------------------
void DataBlock::Free()
{
  if((!Err)&&(!ExternData))
    delete data;
  data=0;
}

// -----------------------------------------------------------------------------
void DataBlock::Copy(DataBlock & blk)
{
  Free();

  Err=0;
  MaxSize=blk.MaxSize;
  DeltaBlok=blk.DeltaBlok;
  ExternData=false;
  data=new char[blk.Size()];
  if(data==0)
    Err=1;
  else
  {
    memcpy(data,blk.GetData(),blk.Size());
    ResetRead();
  }
}

// -----------------------------------------------------------------------------
bool DataBlock::IsAdd(unsigned size)
{
  unsigned u,i;
  char * tmp;
  if(Err)
    return false;
  if(size>MaxSize)
  {
    Err=3;
    //Logdef(Print(DEBUG_ERROR,"Ошибка %d: превышение максимальной длинны блока.", Err))
    return false;
  }
  u=size+ *GetSize();
  if(u>=MaxSize)
  {
    Err=3;
    //Logdef(Print(DEBUG_ERROR,"Ошибка %d: превышение максимальной длинны блока.", Err))
    return false;
  }
  if(u<=BufLen)
    return true;
  if(ExternData || (u>MaxSize))
  {
    Err=2;
    //Logdef(Print(DEBUG_ERROR,"Ошибка %d: превышение максимальной длинны блока.", Err))
    return false;
  }
  i=u+DeltaBlok;
  tmp=new char[i];
  if(tmp==0)
  {
    Err=1;
    return false;
  }
  memcpy(tmp,data,*GetSize());
  delete data;
  data=tmp;
  BufLen=i;
  return true;
}

// -----------------------------------------------------------------------------
bool DataBlock::Next()
{
  char typ;
  if( Err )
    return false;
  typ=GetNextTyp();
  switch(typ)
  {
    case STM_BLANK:  RFlag+=(unsigned)1;
		   break;
    case TM_BYTE:    RFlag+=(unsigned)(1+sizeof(unsigned char));
		   break;
    case TM_CHAR:    RFlag+=(unsigned)(1+sizeof(char));
		   break;
    case TM_UINT:    RFlag+=(unsigned)(1+sizeof(unsigned short));
		   break;
    case TM_INT:     RFlag+=(unsigned)(1+sizeof(short));
		   break;
    case TM_ULONG:   RFlag+=(unsigned)(1+sizeof(unsigned long));
		   break;
    case TM_LONG:    RFlag+=(unsigned)(1+sizeof(long));
		   break;
    case TM_FLOAT:   RFlag+=(unsigned)(1+sizeof(float));
		   break;
    case TM_DOUBLE:  RFlag+=(unsigned)(1+sizeof(double));
		   break;
    case TM_STRING:  RFlag++;
         RFlag+=*((unsigned char *)(GetRead()))+(unsigned)sizeof(unsigned char);
		   break;
    case TM_BLOK:    RFlag++;
		     RFlag+=( *GetSize(GetRead()) );
		   break;
    default:       return false;

  }
  return true;
}

// *****************************************************************************
// -----------------------------------------------------------------------------
bool operator>>(DataBlock& b, unsigned char& d)
{
  if( (b.Err) ||  b.IsEnd() ||
      (*(b.GetRead())!=TM_BYTE) )
    return false;
  b.RFlag++;
  d= *((unsigned char *)(b.GetRead()));
  b.RFlag+=(unsigned)sizeof(unsigned char);
  return true;
}


// -----------------------------------------------------------------------------
bool operator>>(DataBlock& b, char& d)
{
  if( (b.Err) || b.IsEnd() ||
      (*(b.GetRead())!=TM_CHAR) )
    return false;
  b.RFlag++;
  d= *((char *)(b.GetRead()));
  b.RFlag+=(unsigned)sizeof(char);
  return true;
}


// -----------------------------------------------------------------------------
bool operator>>(DataBlock& b, unsigned short& d)
{
  if( (b.Err) || b.IsEnd() ||
      (*(b.GetRead())!=TM_UINT) )
    return false;
  b.RFlag++;
  d= *((unsigned short *)(b.GetRead()));
  b.RFlag+=(unsigned)sizeof(unsigned short);
  return true;
}


// -----------------------------------------------------------------------------
bool operator>>(DataBlock& b, short& d)
{
  if( (b.Err) || b.IsEnd() ||
      (*(b.GetRead())!=TM_INT) )
    return false;
  b.RFlag++;
  d= *((short *)(b.GetRead()));
  b.RFlag+=(unsigned)sizeof(short);
  return true;
}

// -----------------------------------------------------------------------------
bool operator>>(DataBlock& b, long& d)
{
  if( (b.Err) || b.IsEnd() ||
      (*(b.GetRead())!=TM_LONG) )
    return false;
  b.RFlag++;
  d= *((long *)(b.GetRead()));
  b.RFlag+=(unsigned)sizeof(long);
  return true;
}

// -----------------------------------------------------------------------------
bool operator>>(DataBlock& b, unsigned long& d)
{
  if( (b.Err) || b.IsEnd() ||
      (*(b.GetRead())!=TM_ULONG) )
    return false;
  b.RFlag++;
  d= *((unsigned long *)(b.GetRead()));
  b.RFlag+=(unsigned)sizeof(unsigned long);
  return true;
}

// -----------------------------------------------------------------------------
bool operator>>(DataBlock& b, float& d)
{
  if( (b.Err) || b.IsEnd() ||
      (*(b.GetRead())!=TM_FLOAT) )
    return false;
  b.RFlag++;
  d= *((float *)(b.GetRead()));
  b.RFlag+=(unsigned)sizeof(float);
  return true;
}

// -----------------------------------------------------------------------------
bool operator>>(DataBlock& b, double& d)
{
  if( (b.Err) || b.IsEnd() ||
      (*(b.GetRead())!=TM_DOUBLE) )
    return false;
  b.RFlag++;
  d= *((double *)(b.GetRead()));
  b.RFlag+=(unsigned)sizeof(double);
  return true;
}
// -----------------------------------------------------------------------------
bool operator>>(DataBlock& b, BinData &d)
{
  if( (b.Err) || b.IsEnd())
    return false;
  switch(*(b.GetRead()))
  {
    case TM_STRING:
      {
        char * tmp;
        if(!b.Map(tmp))
          return false;
        d=tmp;
      }
      break;
    case STM_MEMO:
      {
        unsigned long u;
        b.RFlag++;
        u= *((unsigned long *)(b.GetRead()));
        b.RFlag+=(unsigned)sizeof(unsigned long);
        d.Free();
        if(u!=0)
          d.SetData(b.GetRead(),u);
        b.RFlag+=u;
      }
      break;
    default:
      return false;
  }
  return true;
}

// -----------------------------------------------------------------------------
bool DataBlock::Map(char * &d)
{
  unsigned char u;
  if( Err || IsEnd() ||
      (*(GetRead())!=TM_STRING) )
    return false;
  RFlag++;
  u= *((unsigned char *)(GetRead()));
  RFlag+=(unsigned)sizeof(unsigned char);
  d=GetRead();
  RFlag+=u;
  return true;
}

// -----------------------------------------------------------------------------
bool operator>>(DataBlock& b, DataBlock * &d)
{
  unsigned u;
  if( (b.Err) || b.IsEnd() ||
      (*(b.GetRead())!=TM_BLOK) )
    return false;
  b.RFlag++;
  d=new DataBlock(b.GetRead(),BlkCopy);
  u= *(d->GetSize());
  b.RFlag+=u;
  return true;
}

// -----------------------------------------------------------------------------
bool DataBlock::Map(DataBlock &d)
{
  unsigned u;
  if( (Err) || IsEnd() ||
      (*(GetRead())!=TM_BLOK) )
    return false;
  RFlag++;
  d.Free();
  d.Err=0;
  d.DeltaBlok=d.GetHeadSize();
  d.ExternData=true;
  u=*GetSize(GetRead());
  d.data=GetRead();
  d.BufLen=u;
  d.RFlag=GetHeadSize();
  RFlag+=u;
  return true;
}

// -----------------------------------------------------------------------------
bool DataBlock::NextIsBlank()
{
  if( Err || IsEnd() ||
      (*(GetRead())!=TM_BLOK) )
    return false;
  RFlag+=(unsigned)1;
  return true;
}

// -----------------------------------------------------------------------------
bool DataBlock::SetMaxSize(unsigned NewSize)
{ if((NewSize>=BufLen)&&(NewSize<=AZBLOK_MAX_SIZE))
  {
    MaxSize=NewSize;
    return true;
  }
  return false;
}

