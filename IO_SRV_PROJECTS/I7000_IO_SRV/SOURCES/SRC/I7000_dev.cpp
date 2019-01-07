/***********************************************************
               created by M.E.N
***********************************************************/

#include <I7000_dev.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
unsigned char I7000_Dev::MaxItemsAmount()const
{
  return 255;
}
//----------------------------------------------------------------
unsigned char GetSpeed_I7000(int speed)
{
  unsigned char spd_val;
  switch(speed)
  {
   case 1200:
     spd_val = 0x03;
     break;
   case 2400:
     spd_val = 0x04;
     break;
   case 4800:
     spd_val = 0x05;
     break;
   case 9600:
     spd_val = 0x06;
     break;
   case 19200:
     spd_val = 0x07;
     break;
   case 38400:
     spd_val = 0x08;
     break;
   case 57600:
     spd_val = 0x09;
     break;
   case 115200:
     spd_val = 0x0A;
     break;
   default:
     spd_val = 0x06;
  }
  return spd_val;
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%