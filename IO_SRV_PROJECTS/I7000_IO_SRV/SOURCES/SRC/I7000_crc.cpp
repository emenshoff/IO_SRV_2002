/***********************************************************
               created by M.E.N
***********************************************************/

#include <I7000_crc.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
unsigned short ComputeCRC(const unsigned char data[],unsigned len)
{
	unsigned sum=0;
	for (unsigned i=0; i<len; i++)
		 sum+=(data[i]&0xff);
	sum=sum&0xff;
  return (unsigned short)sum;
}
//-------------------------------------------------------------------
unsigned short CRC(const unsigned char data[],unsigned len,unsigned char* out)
{
  unsigned sum=0;
	for (unsigned i=0; i<len; i++)
		 sum+=(data[i]&0xff);
	sum=sum&0xff;
  out[0] = (unsigned char)HexToAscii((sum>>4)&0x0f);
  out[1] = (unsigned char)HexToAscii(sum&0x0f);
  return (unsigned short)sum;
}
//-------------------------------------------------------------------
bool CheckCRC(const unsigned char data[],unsigned len)
{
	unsigned i,sum;
	char h,l; sum=0;
	for (i=0; i<len-2; i++)
		sum+=(data[i])&0xff;
	sum=sum&0xff;
	h=HexToAscii(sum/16);
	l=HexToAscii(sum%16);
	if ((data[len-2]==h)&&(data[len-1]==l))
		return true;
	return false;
}/*
//----------------------------------------------
inline char HexToAscii(short hex)
{
  if (hex<10)return((char)(hex+'0'));
  else return((char)(hex-10+'A'));
}
//----------------------------------------------
inline char AsciiToHex(char cASCII)
{
  if (cASCII > '9') return(cASCII-'A'+10);
  else return(cASCII-'0');
}*/
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%