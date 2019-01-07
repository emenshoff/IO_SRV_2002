/*************************************************************
     Маленькая, но полезная хитрость....
            by M.E.N.
*************************************************************/
#ifndef _VAL_H_
#define _VAL_H_
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
typedef  union{
              unsigned char u_ch;
              char ch;
              unsigned short u_sh;
              short sh;
              unsigned int u_i;
              int i;
              unsigned long u_l;
              long l;
              float f;
              double d;
            }VAL;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif