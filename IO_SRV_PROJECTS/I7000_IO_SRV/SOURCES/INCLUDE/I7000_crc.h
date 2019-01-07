/***********************************************************
               created by M.E.N
***********************************************************/

#ifndef _I7000_crc_h_
#define _I7000_crc_h_
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
unsigned short ComputeCRC(const unsigned char data[],unsigned len);
//----------------------------------------------------------------
unsigned short CRC(const unsigned char data[],unsigned len,unsigned char* out);
//----------------------------------------------------------------
bool CheckCRC(const unsigned char data[],unsigned len);
//----------------------------------------------------------------
//inline char HexToAscii(short hex);
//----------------------------------------------------------------
//inline char AsciiToHex(char cASCII);

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
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif //I7000_crc