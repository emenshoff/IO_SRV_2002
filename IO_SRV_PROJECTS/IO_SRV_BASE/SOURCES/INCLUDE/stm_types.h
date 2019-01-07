/* базовые типы первой версии протокола STM <-> Client*/
#ifndef _btypes_h
#define _btypes_h

enum
{ tm_byte   = 1,
  tm_char   = 2,
  tm_uint   = 3,
  tm_int    = 4,
  tm_ulong  = 5,
  tm_long   = 6,
  tm_float  = 7,
  tm_double = 8,
  tm_time   = 9,
  tm_string = 10,
  tm_ubit   = 0x40, // битовое поле беззнаковое
  tm_sbit   = 0xC0, // битовое поле знаковое
  tm_array  = 0x80, // массив базовых (кроме tm_string) типов
  tm_bit    = 0x40, // битовое поле вообще
  tm_hi     = 0xC0  // маска странных типов
};

inline int GetLenForType(unsigned typ)
{
  int ret=0;
  switch(typ)
  {
  case tm_byte:    ret=sizeof(unsigned char);
    break;
  case tm_char:    ret=sizeof(char);
    break;
  case tm_uint:    ret=sizeof(unsigned short);
    break;
  case tm_int:     ret=sizeof(short);
    break;
  case tm_ulong:   ret=sizeof(unsigned long);
    break;
  case tm_long:    ret=sizeof(long);
    break;
  case tm_float:   ret=sizeof(float);
    break;
  case tm_double:  ret=sizeof(double);
    break;
  case tm_time:    ret=sizeof(double);
    break;
  case tm_string:  ret=sizeof(char *);
    break;
  }
  return ret;
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif _btypes_h

