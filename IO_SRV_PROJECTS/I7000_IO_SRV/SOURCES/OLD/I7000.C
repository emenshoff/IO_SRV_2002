#include <windows.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <process.h>

#include "i7000.h"

// ASCII characters that receive special processing
#define ASCII_BEL  0x07
#define ASCII_BS   0x08
#define ASCII_TAB  0x09
#define ASCII_LF   0x0A
#define ASCII_CR   0x0D
#define ASCII_XON  0x11
#define ASCII_XOFF 0x13

static char szCmd[80], szRec[80];

/* -------------------------------------------------------------------- */
int WINAPI DllMain (HINSTANCE hInstance, DWORD fdwReason, PVOID pvReserved)
     {
     switch(fdwReason)
        {
        case DLL_PROCESS_ATTACH :
                break;
        case DLL_PROCESS_DETACH :
                break;
        }

     return TRUE ;
     }

/* -------------------------------------------------------------------- */

short CALLBACK Short_Sub_2(short nA, short nB)
{
return(nA-nB);
}

/* -------------------------------------------------------------------- */

float CALLBACK Float_Sub_2(float fA, float fB)
{
return(fA-fB);
}

/* -------------------------------------------------------------------- */

WORD CALLBACK Get_Dll_Version(void)
{
return(0x0400);
}

/* -------------------------------------------------------------------- */

EXPORTS WORD CALLBACK Test(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
int i;

strcpy(szSendTo7000,"Command String Send to I7000");
strcpy(szReceiveFrom7000,"Command String Receive From I7000");
for (i=0; i<4; i++)
    {
    w7000[i]++;
    f7000[i]++;
    }
return(NoError);
}

/* -------------------------------------------------------------------- */

char HexToAscii(WORD hex)
{
if (hex<10)return((char)(hex+'0'));
else return((char)(hex-10+'A'));
}

/* -------------------------------------------------------------------- */

char AsciiToHex(char cASCII)
{
if (cASCII > '9') return(cASCII-'A'+10);
else return(cASCII-'0');
}

/*-------- Host is OK --------*/
EXPORTS WORD CALLBACK HostIsOK(WORD w7000[], float f7000[],
                                       char szSendTo7000[], char szReceiveFrom7000[])
{
   char   wPort,wFlag;     //,c1,c2;
   WORD   wAddr,wRetVal,wID,wCheckSum,wTimeOut,err,wT;
   //WORD   wRecStrLen;
   //WORD   wSafeValue,wPowerOnValue,

   //---------------------- input -----------------------------------
   wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
   wAddr=w7000[1];             // not used
   wID=w7000[2];               // not used
   wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];      // TimeOut constant: normal=100
   // w7000[5];   don't care
   wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string


   szCmd[0]='~';           // ~**
   szCmd[1] = '*';
   szCmd[2] = '*';
   szCmd[3]=0;

   if(wFlag==1)
   {
      strcpy(szSendTo7000,szCmd);
   }

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wRetVal != TimeOut)
   {
     err=SendCmdError;
     goto ret_err;
   }

   return(NoError);

 ret_err:
   return(err);
}

/*--- Read the module reset status, $AA5 --------*/
EXPORTS WORD CALLBACK ReadModuleResetStatus(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
   char   wPort,wFlag,c1,c2;
   WORD   wAddr,wRetVal,wID,wCheckSum,wTimeOut,err,wT;
   WORD   wRecStrLen;
   //WORD   wPowerOnValue,wSafeValue;

   //---------------------- input -----------------------------------
   wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
   wAddr=w7000[1];         // module address: 0x00 to 0xFF
   wID=w7000[2];           //  all module
   wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];      // TimeOut constant: normal=100
   // w7000[5];   don't care
   wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string


  szCmd[0]='$';    // $AA5
   c2=(char)(wAddr&0x0f);
   wAddr=wAddr/16;
   c1=(char)(wAddr&0x0f);
   szCmd[1] = HexToAscii(c1);
   szCmd[2] = HexToAscii(c2);
   szCmd[3]='5';
   if ( wID == 0x7014 )
           szCmd[3] = 'R';
   szCmd[4]=0;

   if(wFlag==1)
   {
      strcpy(szSendTo7000,szCmd);
      szReceiveFrom7000[0]=0;
   }

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wFlag==1)
   {
      strcpy(szReceiveFrom7000,szRec);
   }
   if(wRetVal != NoError)
   {
     err=SendCmdError;
     goto ret_err;
   }

   if(szRec[0] != '!')
   {
      err=ResultStrCheckError ;
      goto ret_err;
   }

   if(wCheckSum==0) wRecStrLen=4; else wRecStrLen=6;

   if( strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError ;
      goto ret_err;
   }


   w7000[5]=szRec[3]-'0';   // !AAS,
   return(NoError);                     //  S=0: has not been reset
                                        //  S=1: has been reset
 ret_err:
   return(err);
}

/*--- To setup host watchdog, ~AA3ETT --------*/
EXPORTS WORD CALLBACK ToSetupHostWatchdog(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
   char   wPort,wFlag,c1,c2;
   WORD   wAddr,wRetVal,wID,wCheckSum,wTimeOut,err,wT;
   WORD   wRecStrLen;
   WORD   wEnable,wTimeInterval;

   //---------------------- input -----------------------------------
   wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
   wAddr=w7000[1];             // module address: 0x00 to 0xFF
   wID=w7000[2];               //  all module
   wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];      // TimeOut constant: normal=100
   wEnable=w7000[5];       // 0: disable   1: Enable
   if ( wTimeOut == 0 )     // if TimeOut value = 0 then disable the Host Watchdog
           wEnable = 0;
   wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string
   wTimeInterval=w7000[7]; // time interval

   szCmd[0]='~';           // ~AA3ETT
   c2=(char)(wAddr&0x0f);
   wAddr=wAddr/16;
   c1=(char)(wAddr&0x0f);
   szCmd[1] = HexToAscii(c1);
   szCmd[2] = HexToAscii(c2);
   szCmd[3]='3';
   szCmd[4]=wEnable+'0';
   c2=(char)(wTimeInterval&0x0f);
   wTimeInterval=wTimeInterval/16;
   c1=(char)(wTimeInterval&0x0f);
   szCmd[5] = HexToAscii(c1);
   szCmd[6] = HexToAscii(c2);
   szCmd[7]=0;

   if(wFlag==1)
   {
      strcpy(szSendTo7000,szCmd);
      szReceiveFrom7000[0]=0;
   }

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wRetVal != NoError)
   {
     //err=SendCmdError;
         err = wRetVal ;       //Gary Apr-19, 1999
     goto ret_err;
   }

   if(szRec[0] != '!')
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wCheckSum==0) wRecStrLen=3; else wRecStrLen=5;

   if( strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wFlag==1)
   {
      strcpy(szReceiveFrom7000,szRec);
   }
   return(NoError);                     //    SS=00:  module is OK
                                        //    SS=04:  host watchdog failure
 ret_err:
   return(err);
}

/*--- To read host watchdog setup value, ~AA2 --------*/
EXPORTS WORD CALLBACK ToReadHostWatchdog(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
   char   wPort,wFlag,c1,c2;
   WORD   wAddr,wRetVal,wID,wCheckSum,wTimeOut,err,wT;
   WORD   wRecStrLen;
   WORD   wTimeInterval;

   //---------------------- input -----------------------------------
   wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
   wAddr=w7000[1];         // module address: 0x00 to 0xFF
   wID=w7000[2];                 //  all module
   wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];      // TimeOut constant: normal=100
   wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string
   //----------------------- output ----------------------------------
   //w7000[5];  0: disable   1: enable
   //w7000[7];  time interval

   szCmd[0]='~';    // ~AA2
   c2=(char)(wAddr&0x0f);
   wAddr=wAddr/16;
   c1=(char)(wAddr&0x0f);
   szCmd[1] = HexToAscii(c1);
   szCmd[2] = HexToAscii(c2);
   szCmd[3]='2';
   szCmd[4]=0;

   if(wFlag==1)
   {
      strcpy(szSendTo7000,szCmd);
      szReceiveFrom7000[0]=0;
   }

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wRetVal != NoError)
   {
     err=SendCmdError;
     goto ret_err;
   }

   if(szRec[0] != '!')
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wCheckSum==0) wRecStrLen=6; else wRecStrLen=8;

   if( strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wFlag==1)
   {
      strcpy(szReceiveFrom7000,szRec);
   }
   w7000[5]=szRec[3]-'0';   // 0: Host Watchdog disable,  1: enable
   wTimeInterval=AsciiToHex( szRec[4] )*16 + AsciiToHex( szRec[5] );
   w7000[7]=wTimeInterval;
   return(NoError);
 ret_err:
   return(err);
}



/*--- Read the module status about the host watchdog, ~AA0 --------*/
EXPORTS WORD CALLBACK ReadModuleHostWatchdogStatus(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
   char   wPort,wFlag,c1,c2;
   WORD   wAddr,wRetVal,wID,wCheckSum,wTimeOut,err,wT;
   WORD   wRecStrLen;
   //WORD   wPowerOnValue,wSafeValue;

   //---------------------- input -----------------------------------
   wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
   wAddr=w7000[1];         // module address: 0x00 to 0xFF
   wID=w7000[2];           //  all module
   wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];      // TimeOut constant: normal=100
   // w7000[5];   don't care
   wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string


   szCmd[0]='~';    // ~AA0
   c2=(char)(wAddr&0x0f);
   wAddr=wAddr/16;
   c1=(char)(wAddr&0x0f);
   szCmd[1] = HexToAscii(c1);
   szCmd[2] = HexToAscii(c2);
   szCmd[3]='0';
   szCmd[4]=0;

   if(wFlag==1)
   {
      strcpy(szSendTo7000,szCmd);
      szReceiveFrom7000[0]=0;
   }

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wRetVal != NoError)
   {
     err=SendCmdError;
     goto ret_err;
   }

   if(szRec[0] != '!')
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wCheckSum==0) wRecStrLen=5; else wRecStrLen=7;

   if( strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wFlag==1)
   {
      strcpy(szReceiveFrom7000,szRec);
   }

   w7000[5]=szRec[4]-'0';   // !AASS,
   return(NoError);                     //    SS=00:  module is OK
                                        //    SS=04:  host watchdog failure
 ret_err:
   return(err);
}

/*--- Reset the module status about the host watchdog, ~AA1 --------*/
EXPORTS WORD CALLBACK ResetModuleHostWatchdogStatus(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
   char   wPort,wFlag,c1,c2;
   WORD   wAddr,wRetVal,wID,wCheckSum,wTimeOut,err,wT;
   WORD   wRecStrLen;
   //WORD   wPowerOnValue,wSafeValue;

   //---------------------- input -----------------------------------
   wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
   wAddr=w7000[1];         // module address: 0x00 to 0xFF
   wID=w7000[2];                 //  all module
   wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];      // TimeOut constant: normal=100
   // w7000[5];   don't care
   wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string

   szCmd[0]='~';    // ~AA1
   c2=(char)(wAddr&0x0f);
   wAddr=wAddr/16;
   c1=(char)(wAddr&0x0f);
   szCmd[1] = HexToAscii(c1);
   szCmd[2] = HexToAscii(c2);
   szCmd[3]='1';
   szCmd[4]=0;

   if(wFlag==1)
   {
      strcpy(szSendTo7000,szCmd);
      szReceiveFrom7000[0]=0;
   }

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wRetVal != NoError)
   {
     err=SendCmdError;
     goto ret_err;
   }

   if(szRec[0] != '!')
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wCheckSum==0) wRecStrLen=3; else wRecStrLen=5;

   if( strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wFlag==1)
   {
      strcpy(szReceiveFrom7000,szRec);
   }

   return(NoError);

 ret_err:
   return(err);
}

/*--- Set Safe Value for 42/43/44/50/60/65/67 ---*/
EXPORTS WORD CALLBACK SetSafeValueForDo(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
   char  wPort,wFlag,c1,c2,A,B,C,D;
   WORD  wAddr,wRetVal,wData,wID,wCheckSum,wTimeOut,err,wT;
   WORD  wRecStrLen;
   //char  OutCh[10];
   //float fValue;

   //---------------------- input ---------------------------
   wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
   wAddr=w7000[1];         // module address: 0x00 to 0xFF
   wID=w7000[2];           // module ID: 0x7050/60/65/67/42/43/44
   wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];      // TimeOut constant: normal=100
   wData=w7000[5];         // 16-bit digital data to output
   wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string
   //---------------------- output -----------------------
                        // void

   switch (wID)
   {
       case 0x7050 : //@AADD, 8 bit DO
                     if( wData>0x00ff )
                        return DataError;
                     wID=0;
                     break;
       case 0x7060 : //@AADD, 4 bit DO
                     if( wData>0x000f )
                        return DataError;
                     wID=2;
                     break;
       case 0x7065 : //@AADD, 5 bit DO
                     if( wData>0x001f )
                        return DataError;
                     wID=0;
                     break;
       case 0x7067 : //@AADD, 7 bit DO
                     if( wData>0x007f )
                        return DataError;
                     wID=0;
                     break;
       case 0x7044 : //@AADD, 8 bit DO
                     if( wData>0x00ff )
                        return DataError;
                     wID=0;
                     break;
       case 0x7042 : //@AADDDD, 13 bit DO
                     if( wData>0x1fff )
                        return DataError;
                     wID=1;
                     break;
       case 0x7043 : //@AADDDD, 16 bit DO
                     if( wData>0xffff )
                        return DataError;
                     wID=1;
                     break;
       default: return(ModuleIdError);
   }

   if(wID == 0) //@AADD   7050, 7065, 7067, 7044
   {
      szCmd[0]='@';
      c2=(char)(wAddr&0x0f);
      wAddr=wAddr/16;
      c1=(char)(wAddr&0x0f);
      szCmd[1] = HexToAscii(c1);
      szCmd[2] = HexToAscii(c2);

      B=wData&0x0f; wData=wData/16;
      A=wData&0x0f;

      szCmd[3]=HexToAscii(A);
      szCmd[4]=HexToAscii(B);
      szCmd[5]=0;
   }
   else if( wID==2 )   // @AAD  7060
   {
      szCmd[0]='@';
      c2=(char)(wAddr&0x0f);
      wAddr=wAddr/16;
      c1=(char)(wAddr&0x0f);
      szCmd[1] = HexToAscii(c1);
      szCmd[2] = HexToAscii(c2);

     A=wData&0x0f;

      szCmd[3]=HexToAscii(A);
      szCmd[4]=0;
   }
   else
   {
      szCmd[0]='@';
      c2=(char)(wAddr&0x0f);
      wAddr=wAddr/16;
      c1=(char)(wAddr&0x0f);
      szCmd[1] = HexToAscii(c1);
      szCmd[2] = HexToAscii(c2);

      D=wData&0x0f; wData=wData/16;
      C=wData&0x0f; wData=wData/16;
      B=wData&0x0f; wData=wData/16;
      A=wData&0x0f;

      szCmd[3]=HexToAscii(A);
      szCmd[4]=HexToAscii(B);
      szCmd[5]=HexToAscii(C);
      szCmd[6]=HexToAscii(D);
      szCmd[7]=0;

   }

   if(wFlag==1)
   {
      strcpy(szSendTo7000,szCmd);
      szReceiveFrom7000[0]=0;
   }

   // Firstly, Digital Output
   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wRetVal != NoError)
   {
      err=SendCmdError;
      goto ret_err;
   }

   //if( wID==0 )  // Digit I/O module
   {
      if(szRec[0] != '>')
      {
         err=ResultStrCheckError;
         goto ret_err;
      }
   }

   //if( wID==0 )  // Digit I/O module
   {
      if(wCheckSum==0) wRecStrLen=1; else wRecStrLen=3;
   }


   if(strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError;
      goto ret_err;
   }
   /************** Set PowerOn Value **************/
   szCmd[0]='~';
   wAddr=w7000[1];
   c2=(char)(wAddr&0x0f);
   wAddr=wAddr/16;
   c1=(char)(wAddr&0x0f);
   szCmd[1] = HexToAscii(c1);
   szCmd[2] = HexToAscii(c2);
   szCmd[3]='5';
   szCmd[4]='S';
   szCmd[5]=0;

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wRetVal != NoError)
   {
      err=SendCmdError;
      goto ret_err;
   }

   //if( wID==0 )  // Digit I/O module
   {
      if(szRec[0] != '!')
      {
         err=ResultStrCheckError;
         goto ret_err;
      }
   }

   //if( wID==0 )  // Digit I/O module
   {
      if(wCheckSum==0) wRecStrLen=3; else wRecStrLen=5;
   }

   if(strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError;
      goto ret_err;
   }
   return(NoError);

ret_err:
   return(err);
}

/*--- Set PowerOn Value for 42/43/44/50/60/65/67 ---*/
EXPORTS WORD CALLBACK SetPowerOnValueForDo(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
   char  wPort,wFlag,c1,c2,A,B,C,D;
   WORD  wAddr,wRetVal,wData,wID,wCheckSum,wTimeOut,err,wT;
   WORD  wRecStrLen;
   //char  OutCh[10];
   //float fValue;

   //---------------------- input ---------------------------
   wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
   wAddr=w7000[1];         // module address: 0x00 to 0xFF
   wID=w7000[2];           // module ID: 0x7050/60/65/67/42/43/44
   wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];      // TimeOut constant: normal=100
   wData=w7000[5];         // 16-bit digital data to output
   wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string
   //---------------------- output -----------------------
                        // void

   switch (wID)
   {
       case 0x7050 : //@AADD, 8 bit DO
                     if( wData>0x00ff )
                        return DataError;
                     wID=0;
                     break;
       case 0x7060 : //@AADD, 4 bit DO
                     if( wData>0x000f )
                        return DataError;
                     wID=2;
                     break;
       case 0x7065 : //@AADD, 5 bit DO
                     if( wData>0x001f )
                        return DataError;
                     wID=0;
                     break;
       case 0x7067 : //@AADD, 7 bit DO
                     if( wData>0x007f )
                        return DataError;
                     wID=0;
                     break;
       case 0x7044 : //@AADD, 8 bit DO
                     if( wData>0x00ff )
                        return DataError;
                     wID=0;
                     break;
       case 0x7042 : //@AADDDD, 13 bit DO
                     if( wData>0x1fff )
                        return DataError;
                     wID=1;
                     break;
       case 0x7043 : //@AADDDD, 16 bit DO
                     if( wData>0xffff )
                        return DataError;
                     wID=1;
                     break;
       default: return(ModuleIdError);
   }

   if(wID == 0) //@AADD   7050 7065, 7067, 7044
   {
      szCmd[0]='@';
      c2=(char)(wAddr&0x0f);
      wAddr=wAddr/16;
      c1=(char)(wAddr&0x0f);
      szCmd[1] = HexToAscii(c1);
      szCmd[2] = HexToAscii(c2);

      B=wData&0x0f; wData=wData/16;
      A=wData&0x0f;

      szCmd[3]=HexToAscii(A);
      szCmd[4]=HexToAscii(B);
      szCmd[5]=0;
   }
   else if( wID==2 )  // @AAD  7060
   {
      szCmd[0]='@';
      c2=(char)(wAddr&0x0f);
      wAddr=wAddr/16;
      c1=(char)(wAddr&0x0f);
      szCmd[1] = HexToAscii(c1);
      szCmd[2] = HexToAscii(c2);
      A=wData&0x0f;
      szCmd[3]=HexToAscii(A);
      szCmd[4]=0;

   }
   else
   {
      szCmd[0]='@';
      c2=(char)(wAddr&0x0f);
      wAddr=wAddr/16;
      c1=(char)(wAddr&0x0f);
      szCmd[1] = HexToAscii(c1);
      szCmd[2] = HexToAscii(c2);

      D=wData&0x0f; wData=wData/16;
      C=wData&0x0f; wData=wData/16;
      B=wData&0x0f; wData=wData/16;
      A=wData&0x0f;

      szCmd[3]=HexToAscii(A);
      szCmd[4]=HexToAscii(B);
      szCmd[5]=HexToAscii(C);
      szCmd[6]=HexToAscii(D);
      szCmd[7]=0;

   }

  /*****
   else  // #AA00.000    7021
   {
      szCmd[0]='#';
      c2=(char)(wAddr&0x0f);
      wAddr=wAddr/16;
      c1=(char)(wAddr&0x0f);
      szCmd[1] = HexToAscii(c1);
      szCmd[2] = HexToAscii(c2);

      szCmd[5]='.';
      szCmd[9]=0;

      fVal=f7000[0];
      i=(WORD)(fVal/10.0);
      szCmd[3]=i+0x30; fVal-=i*10;

      i=(int)fVal;
      szCmd[4]=i+0x30; fVal-=i;

      fVal *= (float)10.0;
      i=(int)fVal;
      szCmd[6]=i+0x30; fVal-=(float)i;

      fVal *= (float)10.0;
      i=(int)fVal;
      szCmd[7]=i+0x30; fVal-=(float)i;

      fVal *= (float)10.0;
      i=(int)fVal;
      szCmd[8]=i+0x30;
   }
   else if(wID==4)  // #AAN+00.000    7024
   {
     if( wData<0 || wData>3 )
     {
         return(AdChannelError);
     }
     sprintf(OutCh,"%+07.3f",f7000[0]);
         // sprintf(OutCh,"%+07.1f",fValue);


     szCmd[0]='#';
     c2=(char)(wAddr&0x0f);
     wAddr=wAddr/16;
     c1=(char)(wAddr&0x0f);
     szCmd[1] = HexToAscii(c1);
     szCmd[2] = HexToAscii(c2);
     szCmd[3]=(char)(wChannel+0x30);  // channel No.

     szCmd[4]=OutCh[0];
     szCmd[5]=OutCh[1];
     szCmd[6]=OutCh[2];
     szCmd[7]=OutCh[3];
     szCmd[8]=OutCh[4];
     szCmd[9]=OutCh[5];
     szCmd[10]=OutCh[6];
     szCmd[11]=0;
   }
  ****/

   if(wFlag==1)
   {
      strcpy(szSendTo7000,szCmd);
      szReceiveFrom7000[0]=0;
   }

   // Firstly, Digital Output
   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wRetVal != NoError)
   {
      err=SendCmdError;
      goto ret_err;
   }

   //if( wID==0 )  // Digit I/O module
   {
      if(szRec[0] != '>')
      {
         err=ResultStrCheckError;
         goto ret_err;
      }
   }

   //if( wID==0 )  // Digit I/O module
   {
      if(wCheckSum==0) wRecStrLen=1; else wRecStrLen=3;
   }


   if(strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError;
      goto ret_err;
   }
   /************** Set PowerOn Value **************/
   szCmd[0]='~';
   wAddr=w7000[1];
   c2=(char)(wAddr&0x0f);
   wAddr=wAddr/16;
   c1=(char)(wAddr&0x0f);
   szCmd[1] = HexToAscii(c1);
   szCmd[2] = HexToAscii(c2);
   szCmd[3]='5';
   szCmd[4]='P';
   szCmd[5]=0;

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wRetVal != NoError)
   {
      err=SendCmdError;
      goto ret_err;
   }

   //if( wID==0 )  // Digit I/O module
   {
      if(szRec[0] != '!')
      {
         err=ResultStrCheckError;
         goto ret_err;
      }
   }

   //if( wID==0 )  // Digit I/O module
   {
      if(wCheckSum==0) wRecStrLen=3; else wRecStrLen=5;
   }

   if(strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError;
      goto ret_err;
   }
   return(NoError);

ret_err:
   return(err);
}
/*********************************************************/
/*--- Set Safe Value for 21/24, ~AA5/~AA5N ---*/
EXPORTS WORD CALLBACK SetSafeValueForAo(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
  char    wPort,wFlag,c1,c2;
  WORD    wAddr,wRetVal,i,wID,wCheckSum,wTimeOut,err,wT,wChannel;
  float   fVal;
  WORD    wRecStrLen;
  char    OutCh[10];

  //---------------------- input -------------------------------------
  wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
  wAddr=w7000[1];         // module address: 0x00 to 0xFF
  wID=w7000[2];           // module ID: 0x7021
  wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
  wTimeOut=w7000[4];      // TimeOut constant: normal=100
  wChannel=w7000[5];      // Not used if module ID is 7021
                          // Channel No.(0 to 3) if module ID is 7024
  wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string
  fVal=f7000[0];          // DA output value: from 0.0 to 10.0
  //---------------------- output ------------------------------------
                          // void

  if(wID==0x7021)
  {
   szCmd[0]='#';
   c2=(char)(wAddr&0x0f);
   wAddr=wAddr/16;
   c1=(char)(wAddr&0x0f);
   szCmd[1] = HexToAscii(c1);
   szCmd[2] = HexToAscii(c2);

   szCmd[5]='.';
   szCmd[9]=0;


   i=(WORD)(fVal/10.0);
   szCmd[3]=i+0x30; fVal-=i*10;

   i=(int)fVal;
   szCmd[4]=i+0x30; fVal-=i;

   fVal *= (float)10.0;
   i=(int)fVal;
   szCmd[6]=i+0x30; fVal-=(float)i;

   fVal *= (float)10.0;
   i=(int)fVal;
   szCmd[7]=i+0x30; fVal-=(float)i;

   fVal *= (float)10.0;
   i=(int)fVal;
   szCmd[8]=i+0x30;
  }
  else if( wID==0x7024 )
  {
     if( wChannel<0 || wChannel>3 )
     {
         return(AdChannelError);
     }
     sprintf(OutCh,"%+07.3f",f7000[0]);
         // sprintf(OutCh,"%+07.1f",fValue);

     szCmd[0]='#';
     c2=(char)(wAddr&0x0f);
     wAddr=wAddr/16;
     c1=(char)(wAddr&0x0f);
     szCmd[1] = HexToAscii(c1);
     szCmd[2] = HexToAscii(c2);
     szCmd[3]=(char)(wChannel+0x30);  // channel No.

     szCmd[4]=OutCh[0];
     szCmd[5]=OutCh[1];
     szCmd[6]=OutCh[2];
     szCmd[7]=OutCh[3];
     szCmd[8]=OutCh[4];
     szCmd[9]=OutCh[5];
     szCmd[10]=OutCh[6];
     szCmd[11]=0;
  }
  else
     return(ModuleIdError);


  if(wFlag==1)
  {
    strcpy(szSendTo7000,szCmd);
    szReceiveFrom7000[0]=0;
  }

  wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
  if(wRetVal != NoError)
  {
     err=SendCmdError;
     goto ret_err;
  }

  if(szRec[0] != '>')
  {
     err=ResultStrCheckError;
     goto ret_err;
  }

  if(wCheckSum==0) wRecStrLen=1; else wRecStrLen=3;

  if(strlen(szRec) != wRecStrLen)
  {
     err=ResultStrCheckError;
     goto ret_err;
  }

  if(wFlag==1)
  {
     strcpy(szReceiveFrom7000,szRec);
  }

  /*********** secondly, set the safe value ***********/
  wAddr=w7000[1];         // module address: 0x00 to 0xFF
  if( wID==0x7024 )
  {
     szCmd[0]='~';
     c2=(char)(wAddr&0x0f);
     wAddr=wAddr/16;
     c1=(char)(wAddr&0x0f);
     szCmd[1] = HexToAscii(c1);
     szCmd[2] = HexToAscii(c2);
     szCmd[3] = '5';
     szCmd[4] = wChannel+'0';
     szCmd[5]=0;

  }
  else
  {
     szCmd[0]='~';
     c2=(char)(wAddr&0x0f);
     wAddr=wAddr/16;
     c1=(char)(wAddr&0x0f);
     szCmd[1] = HexToAscii(c1);
     szCmd[2] = HexToAscii(c2);
     szCmd[3] = '5';
     szCmd[4]=0;
  }

  if(wFlag==1)
  {
    strcpy(szSendTo7000,szCmd);
    szReceiveFrom7000[0]=0;
  }

  wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
  if(wRetVal != NoError)
  {
     err=SendCmdError;
     goto ret_err;
  }

  if(szRec[0] != '!')
  {
     err=ResultStrCheckError;
     goto ret_err;
  }

  if(wCheckSum==0) wRecStrLen=3; else wRecStrLen=5;

  if(strlen(szRec) != wRecStrLen)
  {
     err=ResultStrCheckError;
     goto ret_err;
  }

  if(wFlag==1)
  {
     strcpy(szReceiveFrom7000,szRec);
  }

  return(NoError);
ret_err:
  return(err);

}
/*--- Set PowerOn Value for 21/24, $AA4/$AA4N ---*/
EXPORTS WORD CALLBACK SetPowerOnValueForAo(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
  char    wPort,wFlag,c1,c2;
  WORD    wAddr,wRetVal,i,wID,wCheckSum,wTimeOut,err,wT,wChannel;
  float   fVal;
  WORD    wRecStrLen;
  char    OutCh[10];

  //---------------------- input -------------------------------------
  wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
  wAddr=w7000[1];         // module address: 0x00 to 0xFF
  wID=w7000[2];           // module ID: 0x7021
  wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
  wTimeOut=w7000[4];      // TimeOut constant: normal=100
  wChannel=w7000[5];      // Not used if module ID is 7021
                          // Channel No.(0 to 3) if module ID is 7024
  wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string
  fVal=f7000[0];          // DA output value: from 0.0 to 10.0
  //---------------------- output ------------------------------------
                          // void

  if(wID==0x7021)
  {
   szCmd[0]='#';
   c2=(char)(wAddr&0x0f);
   wAddr=wAddr/16;
   c1=(char)(wAddr&0x0f);
   szCmd[1] = HexToAscii(c1);
   szCmd[2] = HexToAscii(c2);

   szCmd[5]='.';
   szCmd[9]=0;


   i=(WORD)(fVal/10.0);
   szCmd[3]=i+0x30; fVal-=i*10;

   i=(int)fVal;
   szCmd[4]=i+0x30; fVal-=i;

   fVal *= (float)10.0;
   i=(int)fVal;
   szCmd[6]=i+0x30; fVal-=(float)i;

   fVal *= (float)10.0;
   i=(int)fVal;
   szCmd[7]=i+0x30; fVal-=(float)i;

   fVal *= (float)10.0;
   i=(int)fVal;
   szCmd[8]=i+0x30;
  }
  else if( wID==0x7024 )
  {
     if( wChannel<0 || wChannel>3 )
     {
         return(AdChannelError);
     }
     sprintf(OutCh,"%+07.3f",f7000[0]);
         // sprintf(OutCh,"%+07.1f",fValue);


     szCmd[0]='#';
     c2=(char)(wAddr&0x0f);
     wAddr=wAddr/16;
     c1=(char)(wAddr&0x0f);
     szCmd[1] = HexToAscii(c1);
     szCmd[2] = HexToAscii(c2);
     szCmd[3]=(char)(wChannel+0x30);  // channel No.

     szCmd[4]=OutCh[0];
     szCmd[5]=OutCh[1];
     szCmd[6]=OutCh[2];
     szCmd[7]=OutCh[3];
     szCmd[8]=OutCh[4];
     szCmd[9]=OutCh[5];
     szCmd[10]=OutCh[6];
     szCmd[11]=0;
  }
  else
     return(ModuleIdError);


  if(wFlag==1)
  {
    strcpy(szSendTo7000,szCmd);
    szReceiveFrom7000[0]=0;
  }

  wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
  if(wRetVal != NoError)
  {
     err=SendCmdError;
     goto ret_err;
  }

  if(szRec[0] != '>')
  {
     err=ResultStrCheckError;
     goto ret_err;
  }

  if(wCheckSum==0) wRecStrLen=1; else wRecStrLen=3;

  if(strlen(szRec) != wRecStrLen)
  {
     err=ResultStrCheckError;
     goto ret_err;
  }

  if(wFlag==1)
  {
     strcpy(szReceiveFrom7000,szRec);
  }
  /*********** secondly, set the PowerOn value ***********/
  wAddr=w7000[1];         // module address: 0x00 to 0xFF
  if( wID==0x7021 )
  {
     szCmd[0]='$';
     c2=(char)(wAddr&0x0f);
     wAddr=wAddr/16;
     c1=(char)(wAddr&0x0f);
     szCmd[1] = HexToAscii(c1);
     szCmd[2] = HexToAscii(c2);
     szCmd[3] = '4';
     szCmd[4]=0;
  }
  else
  {
     szCmd[0]='$';
     c2=(char)(wAddr&0x0f);
     wAddr=wAddr/16;
     c1=(char)(wAddr&0x0f);
     szCmd[1] = HexToAscii(c1);
     szCmd[2] = HexToAscii(c2);
     szCmd[3] = '4';
     szCmd[4] = wChannel+'0';
     szCmd[5]=0;
  }

  if(wFlag==1)
  {
    strcpy(szSendTo7000,szCmd);
    szReceiveFrom7000[0]=0;
  }

  wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
  if(wRetVal != NoError)
  {
     err=SendCmdError;
     goto ret_err;
  }

  if(szRec[0] != '!')
  {
     err=ResultStrCheckError;
     goto ret_err;
  }

  if(wCheckSum==0) wRecStrLen=3; else wRecStrLen=5;

  if(strlen(szRec) != wRecStrLen)
  {
     err=ResultStrCheckError;
     goto ret_err;
  }

  if(wFlag==1)
  {
     strcpy(szReceiveFrom7000,szRec);
  }

  return(NoError);

ret_err:
  return(err);
}


/*-------- 7011/7012/7014 Set PowerOn/Safe value --------*/
EXPORTS WORD CALLBACK SetPowerOnSafeValue(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
   char   wPort,wFlag,c1,c2;
   WORD   wAddr,wRetVal,wID,wCheckSum,wTimeOut,err,wT;
   WORD   wRecStrLen;
   WORD   wPowerOnValue,wSafeValue;

   //---------------------- input -----------------------------------
   wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
   wAddr=w7000[1];         // module address: 0x00 to 0xFF
   wID=w7000[2];                 // module ID: 0x7011, 7012, 7013
   wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];      // TimeOut constant: normal=100
   wPowerOnValue=w7000[5]; // PowerOn Value
   wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string
   wSafeValue=w7000[7];    // Safe value

   switch (wID)
   {
       case 0x7011 :
       case 0x7012 :
       case 0x7014 : break;
       default: return(ModuleIdError);
   }

   if( wPowerOnValue>3 || wSafeValue>3 )
      return DataError;

   szCmd[0]='~';    // ~AA5PPSS
   c2=(char)(wAddr&0x0f);
   wAddr=wAddr/16;
   c1=(char)(wAddr&0x0f);
   szCmd[1] = HexToAscii(c1);
   szCmd[2] = HexToAscii(c2);
   szCmd[3]='5';
   szCmd[4]='0';
   szCmd[5]=wPowerOnValue+'0';
   szCmd[6]='0';
   szCmd[7]=wSafeValue+'0';
   szCmd[8]=0;

   if(wFlag==1)
   {
      strcpy(szSendTo7000,szCmd);
      szReceiveFrom7000[0]=0;
   }

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wRetVal != NoError)
   {
     err=SendCmdError;
     goto ret_err;
   }

   if(szRec[0] != '!')
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wCheckSum==0) wRecStrLen=3; else wRecStrLen=5;

   if( strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wFlag==1)
   {
      strcpy(szReceiveFrom7000,szRec);
   }

   return(NoError);

 ret_err:
   return(err);
}


/*--- Read PowerOn Value for 42/43/44/50/60/65/67 ---*/
EXPORTS WORD CALLBACK ReadPowerOnValueForDo(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
   char  wPort,wFlag,c1,c2,A,B,C,D;
   WORD  wAddr,wRetVal,wData,wID,wCheckSum,wTimeOut,err,wT;
   WORD  wRecStrLen;
   char  *stopstring;
   char  OutCh[10];
   //float fValue;

   //---------------------- input ---------------------------
   wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
   wAddr=w7000[1];         // module address: 0x00 to 0xFF
   wID=w7000[2];           // module ID: 0x7050/60/65/67/42/43/44
   wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];      // TimeOut constant: normal=100
   wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string
   //---------------------- output -----------------------
   //   w7000[5]:   PowerOn Value

   switch (wID)
   {
       case 0x7050 : //  8 bit DO
       case 0x7060 : //  4 bit DO
       case 0x7065 : //  5 bit DO
       case 0x7067 : //  7 bit DO
       case 0x7044 : //  8 bit DO
                     wID=0;
                     break;
       case 0x7042 : //  13 bit DO
       case 0x7043 : //  16 bit DO
                     wID=1;
                     break;
       default: return(ModuleIdError);
   }

   //--- ~AA4P ---
   szCmd[0]='~';
   c2=(char)(wAddr&0x0f);
   wAddr=wAddr/16;
   c1=(char)(wAddr&0x0f);
   szCmd[1] = HexToAscii(c1);
   szCmd[2] = HexToAscii(c2);
   szCmd[3]='4';
   szCmd[4]='P';
   szCmd[5]=0;


   if  (wFlag==1)
   {
    strcpy(szSendTo7000,szCmd);
    szReceiveFrom7000[0]=0;
   }

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wRetVal != NoError)
   {
     err=SendCmdError;
     goto ret_err;
   }

   if(wCheckSum==0) wRecStrLen=7; else wRecStrLen=9;

   if(strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wFlag==1)
   {
      strcpy(szReceiveFrom7000,szRec);
   }

   if( wID==0 )
   {
      OutCh[0]=szRec[3];   OutCh[1]=szRec[4];
      OutCh[2]=0;
   }
   else
   {
      OutCh[0]=szRec[3];   OutCh[1]=szRec[4];
      OutCh[2]=szRec[5];   OutCh[3]=szRec[6];
      OutCh[4]=0;
   }

   w7000[5]=(WORD)(strtoul(OutCh,&stopstring,16));

   return(NoError);

ret_err:
   return(err);
}

/*--- Read Safe Value for 42/43/44/50/60/65/67 ---*/
EXPORTS WORD CALLBACK ReadSafeValueForDo(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
   char  wPort,wFlag,c1,c2,A,B,C,D;
   WORD  wAddr,wRetVal,wData,wID,wCheckSum,wTimeOut,err,wT;
   WORD  wRecStrLen;
   char  *stopstring;
   char  OutCh[10];
   //float fValue;

   //---------------------- input ---------------------------
   wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
   wAddr=w7000[1];         // module address: 0x00 to 0xFF
   wID=w7000[2];           // module ID: 0x7050/60/65/67/42/43/44
   wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];      // TimeOut constant: normal=100
   wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string
   //---------------------- output -----------------------
   //   w7000[5]:   Safe Value Value

   switch (wID)
   {
       case 0x7050 : //  8 bit DO
       case 0x7060 : //  4 bit DO
       case 0x7065 : //  5 bit DO
       case 0x7067 : //  7 bit DO
       case 0x7044 : //  8 bit DO
                     wID=0;
                     break;
       case 0x7042 : //  13 bit DO
       case 0x7043 : //  16 bit DO
                     wID=1;
                     break;
       default: return(ModuleIdError);
   }

   //--- ~AA4S ---
   szCmd[0]='~';
   c2=(char)(wAddr&0x0f);
   wAddr=wAddr/16;
   c1=(char)(wAddr&0x0f);
   szCmd[1] = HexToAscii(c1);
   szCmd[2] = HexToAscii(c2);
   szCmd[3]='4';
   szCmd[4]='S';
   szCmd[5]=0;


   if  (wFlag==1)
   {
    strcpy(szSendTo7000,szCmd);
    szReceiveFrom7000[0]=0;
   }

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wRetVal != NoError)
   {
     err=SendCmdError;
     goto ret_err;
   }

   if(wCheckSum==0) wRecStrLen=7; else wRecStrLen=9;

   if(strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wFlag==1)
   {
      strcpy(szReceiveFrom7000,szRec);
   }

   if( wID==0 )
   {
      OutCh[0]=szRec[3];   OutCh[1]=szRec[4];
      OutCh[2]=0;
   }
   else
   {
      OutCh[0]=szRec[3];   OutCh[1]=szRec[4];
      OutCh[2]=szRec[5];   OutCh[3]=szRec[6];
      OutCh[4]=0;
   }

   w7000[5]=(WORD)(strtoul(OutCh,&stopstring,16));

   return(NoError);

ret_err:
   return(err);
}


/*-------- Read Config Status ---------*/
EXPORTS WORD CALLBACK ReadConfigStatus(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
   char   wPort,wFlag,c1,c2;
   WORD   wAddr,wRetVal,wID,wCheckSum,wTimeOut,err,wT;   //,wAlarmMode
   WORD   wRecStrLen;
   WORD   A,B,C,D,E,F,G,H;
   //---------------------- input -------------------------------------
   wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
   wAddr=w7000[1];         // module address: 0x00 to 0xFF
   wID=w7000[2];           // module ID
   wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];      // TimeOut constant: normal=100
   // wAlarmMode=w7000[5]; // Don't Care
   wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string
   //---------------------- Output -------------------------------------
   // w7000[7]:   module address
   // w7000[8]:   module Range Code
   // w7000[9]:   module baudrate
   // w7000[10]:  module data format

   szCmd[0]='$';    // $AA2
   c2=(char)(wAddr&0x0f);
   wAddr=wAddr/16;
   c1=(char)(wAddr&0x0f);
   szCmd[1] = HexToAscii(c1);
   szCmd[2] = HexToAscii(c2);
   szCmd[3]='2';
   szCmd[4]=0;

   if(wFlag==1)
   {
      strcpy(szSendTo7000,szCmd);
      szReceiveFrom7000[0]=0;
   }

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wRetVal != NoError)
   {
     err=SendCmdError;
     goto ret_err;
   }

   if(szRec[0] != '!')
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wCheckSum==0) wRecStrLen=9; else wRecStrLen=11;

   if( strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wFlag==1)
   {
      strcpy(szReceiveFrom7000,szRec);
   }

   A=AsciiToHex(szRec[1]);
   B=AsciiToHex(szRec[2]);
   w7000[7]=A*16+B;

   C=AsciiToHex(szRec[3]);
   D=AsciiToHex(szRec[4]);
   w7000[8]=C*16+D;

   E=AsciiToHex(szRec[5]);
   F=AsciiToHex(szRec[6]);
   w7000[9]=E*16+F;

   G=AsciiToHex(szRec[7]);
   H=AsciiToHex(szRec[8]);
   w7000[10]=G*16+H;

   return(NoError);

 ret_err:
   return(err);
}



/* -------------------------------------------------------------------- */
EXPORTS WORD CALLBACK AnalogIn(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
char wPort,wFlag,cc[80],c1,c2;
WORD wAddr,wRetVal,i,j,wChannel,wID,wCheckSum,err,wT,wTimeOut;
WORD wRecStrLen;

//---------------------- input -------------------------------------
wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
wAddr=w7000[1];         // module address: 0x00 to 0xFF
wID=w7000[2];           // module ID: 0x7011/12/13/14/17/18/33
wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
wTimeOut=w7000[4];      // TimeOut constant: normal=100
wChannel=w7000[5];      // channel number for multi-channel
wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string
//---------------------- output ------------------------------------
                        // f7000[0]=analog input value
switch (wID)
       {
       case 0x7011 :
       case 0x7012 :
       case 0x7013 :
       case 0x7014 :
       case 0x7016 : wID=0; break;
       case 0x7017 :
       case 0x7018 : wID=1; break;
       case 0x7033 : wID=2; break;
       default: return(ModuleIdError);
       }

if (wID == 0)
   {
   szCmd[0]='#';
   c2=(char)(wAddr&0x0f);
   wAddr=wAddr/16;
   c1=(char)(wAddr&0x0f);
   szCmd[1] = HexToAscii(c1);
   szCmd[2] = HexToAscii(c2);
   szCmd[3]=0;
   }
else if (wID == 1)
   {
   if (wChannel>7) return(AdChannelError);
   szCmd[0]='#';
   c2=(char)(wAddr&0x0f);
   wAddr=wAddr/16;
   c1=(char)(wAddr&0x0f);
   szCmd[1] = HexToAscii(c1);
   szCmd[2] = HexToAscii(c2);
   szCmd[3]=(char)(wChannel+0x30);
   szCmd[4]=0;
   }
else if(wID==2)
   {
   if (wChannel>2) return(AdChannelError);
   szCmd[0]='#';
   c2=(char)(wAddr&0x0f);
   wAddr=wAddr/16;
   c1=(char)(wAddr&0x0f);
   szCmd[1] = HexToAscii(c1);
   szCmd[2] = HexToAscii(c2);
   szCmd[3]=(char)(wChannel+0x30);
   szCmd[4]=0;
   }

if  (wFlag==1)
    {
    strcpy(szSendTo7000,szCmd);
    szReceiveFrom7000[0]=0;
    }

wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
if (wRetVal != NoError)
   {
   err=SendCmdError;
   goto ret_err;
   }

if (szRec[0] != '>')
   {
   err=ResultStrCheckError;
   goto ret_err;
   }


   if(wCheckSum==0) wRecStrLen=8; else wRecStrLen=10;

   if( (w7000[2]!=0x7013 && w7000[2]!=0x7011 && w7000[2]!=0x7018 && w7000[2]!=0x7033 ) && (strlen(szRec)!=wRecStrLen) )
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

    //-----  Added by Shu, 7011, 7013, 7018, 7033
    // if( w7000[2]==0x7013 || w7000[2]==0x7011 || w7000[2]==0x7018 || w7000[2]==0x7033 )
     {
         if( !memcmp(szRec,">-0000",6) )
         {
             err=UnderInputRange;
             goto ret_err;
         }
         if( !memcmp(szRec,">+9999",6) )
         {
             err=ExceedInputRange;
             goto ret_err;
         }
     }

     i=1; j=0;
     while (szRec[i])
     {
        cc[j++]=szRec[i++];
     }
     cc[j]=0;

     if( wCheckSum )  // checksum enable
        cc[j-2]=0;

     f7000[0]=(float)atof(cc);

     if (wFlag==1)
     {
       strcpy(szReceiveFrom7000,szRec);
     }
     return(NoError);

ret_err:
     f7000[0]=(float)0.0;
     return(err);
}

/* -------------------------------------------------------------------- */
EXPORTS WORD CALLBACK AnalogInFsr(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
   char  wPort,wFlag,cc[80],c1,c2;
   WORD  wAddr,wRetVal,i,j,wChannel,wID,wCheckSum,err,wT,wTimeOut;
   WORD  wRecStrLen;

   //---------------------- input --------------------------------
   wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
   wAddr=w7000[1];         // module address: 0x00 to 0xFF
   wID=w7000[2];           // module ID: 0x7011/12/13/14/17/18/33
   wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];      // TimeOut constant: normal=100
   wChannel=w7000[5];      // channel number for multi-channel
   wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string
   //---------------------- output ------------------------------------
                           // f7000[0]=analog input value in % FSR

   switch (wID)
   {
       case 0x7011 :
       case 0x7012 :
       case 0x7013 :
       case 0x7014 : wID=0; break;
       case 0x7017 :
       case 0x7018 : wID=1; break;
       case 0x7033 : wID=2; break;
       default: return(ModuleIdError);
   }

   if(wID == 0)
   {
      szCmd[0]='#';
      c2=(char)(wAddr&0x0f);
      wAddr=wAddr/16;
      c1=(char)(wAddr&0x0f);
      szCmd[1] = HexToAscii(c1);
      szCmd[2] = HexToAscii(c2);
      szCmd[3]=0;
   }
   else if(wID == 1)
   {
      if(wChannel>7) return(AdChannelError);
      szCmd[0]='#';
      c2=(char)(wAddr&0x0f);
      wAddr=wAddr/16;
      c1=(char)(wAddr&0x0f);
      szCmd[1] = HexToAscii(c1);
      szCmd[2] = HexToAscii(c2);
      szCmd[3]=(char)(wChannel+0x30);
      szCmd[4]=0;
   }
   else if(wID == 2)
   {
      if(wChannel>2) return(AdChannelError);
      szCmd[0]='#';
      c2=(char)(wAddr&0x0f);
      wAddr=wAddr/16;
      c1=(char)(wAddr&0x0f);
      szCmd[1] = HexToAscii(c1);
      szCmd[2] = HexToAscii(c2);
      szCmd[3]=(char)(wChannel+0x30);
      szCmd[4]=0;
   }

   if(wFlag==1)
   {
      strcpy(szSendTo7000,szCmd);
      szReceiveFrom7000[0]=0;
   }

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wRetVal != NoError)
   {
      err=SendCmdError;
      goto ret_err;
   }

   if(szRec[0] != '>')
   {
     err=ResultStrCheckError;
     goto ret_err;
   }

    //-----  Added by Shu, 7011, 7013, 7018, 7033
    // if( w7000[2]==0x7013 || w7000[2]==0x7011 || w7000[2]==0x7018 || w7000[2]==0x7033 )
     {
         if( !memcmp(szRec,">-0000",6) )
         {
             err=UnderInputRange;
             goto ret_err;
         }
         if( !memcmp(szRec,">+9999",6) )
         {
             err=ExceedInputRange;
             goto ret_err;
         }
     }


   if(wCheckSum==0) wRecStrLen=8; else wRecStrLen=10;

   if (strlen(szRec)!=wRecStrLen )
   {
      err=ResultStrCheckError;
      goto ret_err;
   }


   i=1; j=0;
   while (szRec[i])
   {
      cc[j++]=szRec[i++];
   }
   cc[j]=0;

   if( wCheckSum )  // checksum enable
        cc[j-2]=0;
   f7000[0]=(float)atof(cc);

   if (wFlag==1)
   {
      strcpy(szReceiveFrom7000,szRec);
   }
   return(NoError);

ret_err:
   f7000[0]=(float)0.0;
return(err);
}


/* -------------------------------------------------------------------- */
EXPORTS WORD CALLBACK AnalogInHex(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
   char   wPort,wFlag,c1,c2;
   WORD   wAddr,wRetVal,wChannel,wID,wCheckSum,err,wT,wTimeOut;
   WORD   wRecStrLen;
   WORD   A,B,C,D;

   //---------------------- input ---------------------------
   wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
   wAddr=w7000[1];         // module address: 0x00 to 0xFF
   wID=w7000[2];           // module ID: 0x7011/12/13/14/17/18/33
   wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];      // TimeOut constant: normal=100
   wChannel=w7000[5];      // channel number for multi-channel
   wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string
   //---------------------- output ---------------------------
                           // W7000[7]=analog input value
   switch (wID)
   {
      case 0x7011 :
      case 0x7012 :
      case 0x7013 :
      case 0x7014 : wID=0; break;
      case 0x7017 :
      case 0x7018 : wID=1; break;
      case 0x7033 : wID=2; break;
      default: return(ModuleIdError);
   }

   if (wID == 0)
   {
      szCmd[0]='#';
      c2=(char)(wAddr&0x0f);
      wAddr=wAddr/16;
      c1=(char)(wAddr&0x0f);
      szCmd[1] = HexToAscii(c1);
      szCmd[2] = HexToAscii(c2);
      szCmd[3]=0;
   }
   else if (wID == 1)
   {
      if (wChannel>7) return(AdChannelError);
      szCmd[0]='#';
      c2=(char)(wAddr&0x0f);
      wAddr=wAddr/16;
      c1=(char)(wAddr&0x0f);
      szCmd[1] = HexToAscii(c1);
      szCmd[2] = HexToAscii(c2);
      szCmd[3]=(char)(wChannel+0x30);
      szCmd[4]=0;
   }
   else if (wID == 2)
   {
      if (wChannel>2) return(AdChannelError);
      szCmd[0]='#';
      c2=(char)(wAddr&0x0f);
      wAddr=wAddr/16;
      c1=(char)(wAddr&0x0f);
      szCmd[1] = HexToAscii(c1);
      szCmd[2] = HexToAscii(c2);
      szCmd[3]=(char)(wChannel+0x30);
      szCmd[4]=0;
   }

   if(wFlag==1)
   {
      strcpy(szSendTo7000,szCmd);
      szReceiveFrom7000[0]=0;
   }

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if (wRetVal != NoError)
   {
      err=SendCmdError;
      goto ret_err;
   }

   if (szRec[0] != '>')
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wCheckSum==0) wRecStrLen=5; else wRecStrLen=7;

   if ( strlen(szRec)!=wRecStrLen )
   {
     err=ResultStrCheckError;
     goto ret_err;
   }

   if(wFlag==1)
   {
      strcpy(szReceiveFrom7000,szRec);
   }

   A=AsciiToHex(szRec[1]);
   B=AsciiToHex(szRec[2]);
   C=AsciiToHex(szRec[3]);
   D=AsciiToHex(szRec[4]);
   w7000[7]=D+C*16+B*256+A*4096;
return(NoError);

ret_err:
   return(err);
}


/* -------------------------------------------------------------------- */

EXPORTS WORD CALLBACK AnalogIn8(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
char wPort,wFlag,cc[80],c1,c2;
WORD wAddr,wRetVal,i,t,j,wID,wCheckSum,wTimeOut,err,wT;
WORD wRecStrLen;

//---------------------- input -------------------------------------
wPort=(char)w7000[0]; // RS-232 port number: 1/2/3/4
wAddr=w7000[1];           // module address: 0x00 to 0xFF
wID=w7000[2];             // module ID: 0x7017/18
wCheckSum=w7000[3];       // checksum: 0=disable, 1=enable
wTimeOut=w7000[4];        // TimeOut constant: normal=100
wFlag=(char)w7000[6]; // flag: 0=no save, 1=save send/receive string
//---------------------- output ------------------------------------
                        // f7000[0]=analog input value of channel_0
                        // f7000[1]=analog input value of channel_1
                        // ........................................
                        // f7000[7]=analog input value of channel_7

switch (wID)
       {
       case 0x7017 :
       case 0x7018 : break;
       default: return(ModuleIdError);
       }

szCmd[0]='#';           //#AA
c2=(char)(wAddr&0x0f);
wAddr=wAddr/16;
c1=(char)(wAddr&0x0f);
szCmd[1] = HexToAscii(c1);
szCmd[2] = HexToAscii(c2);
szCmd[3] = 0;

if  (wFlag==1)
    {
    strcpy(szSendTo7000,szCmd);
    szReceiveFrom7000[0]=0;
    }

wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
if (wRetVal != NoError)
   {
   err=SendCmdError;
   goto ret_err;
   }

if (szRec[0] != '>')
   {
   err=ResultStrCheckError;
   goto ret_err;
   }

if (wCheckSum==0) wRecStrLen=57; else wRecStrLen=59;

if (strlen(szRec) != wRecStrLen)
   {
   err=ResultStrCheckError;
   goto ret_err;
   }

t=1;
for (j=0; j<8; j++)
    {
    for (i=0; i<7; i++) cc[i]=szRec[i+t];
    f7000[j]=(float)atof(cc);
    t=t+7;
    }

if  (wFlag==1)
    {
    strcpy(szReceiveFrom7000,szRec);
    }
return(NoError);

ret_err:
for (j=0; j<7; j++) f7000[j]=(float)0.0;
return(err);
}

/* -------------------------------------------------------------------- */

EXPORTS WORD CALLBACK In8_7017(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
char  wPort,wFlag,cc[80],c1,c2,InHex[5];
WORD  wAddr,wRetVal,t,j,wID,wCheckSum,wTimeOut,err,wT;
float fRange;
DWORD dwVal;
WORD  wRecStrLen;

//---------------------- input -------------------------------------
wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
wAddr=w7000[1];         // module address: 0x00 to 0xFF
wID=w7000[2];           // module type: 08/09/0A/0B/0C/0D
wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
wTimeOut=w7000[4];      // TimeOut constant: normal=100
wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string
//---------------------- output ------------------------------------
                        // f7000[0]=analog input value of channel_0
                        // f7000[1]=analog input value of channel_1
                        // ........................................
                        // f7000[7]=analog input value of channel_7

switch (wID)
       {
       case 0x08 : fRange=(float)10.0;  break;
       case 0x09 : fRange=(float)5.0;   break;
       case 0x0A : fRange=(float)1.0;   break;
       case 0x0B : fRange=(float)500.0; break;
       case 0x0C : fRange=(float)150.0; break;
       case 0x0D : fRange=(float)20.0;  break;
       default: return(ModuleIdError);
       }

szCmd[0]='$';           //$AAA
c2=(char)(wAddr&0x0f);
wAddr=wAddr/16;
c1=(char)(wAddr&0x0f);
szCmd[1] = HexToAscii(c1);
szCmd[2] = HexToAscii(c2);
szCmd[3] = 'A';
szCmd[4] = 0;

if  (wFlag==1)
    {
    strcpy(szSendTo7000,szCmd);
    szReceiveFrom7000[0]=0;
    }

wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
if (wRetVal != NoError)
   {
   err=SendCmdError;
   goto ret_err;
   }

strcpy(szReceiveFrom7000,szRec);

if (szRec[0] != '>')
   {
   err=ResultStrCheckError;
   goto ret_err;
   }

if (wCheckSum==0) wRecStrLen=33; else wRecStrLen=35;

if (strlen(szRec) != wRecStrLen)
   {
   err=ResultStrCheckError;
   goto ret_err;
   }

t=1;
InHex[4]=0;
for (j=0; j<8; j++)
    {
    InHex[0]=szRec[t++];
    InHex[1]=szRec[t++];
    InHex[2]=szRec[t++];
    InHex[3]=szRec[t++];
    dwVal=strtol(InHex,&cc,16);
    if (dwVal<=32767) f7000[j]=(float)dwVal/(float)32767.0*fRange;
    else
        f7000[j]=(float)(65535-dwVal)/(float)32767.0*fRange*(float)(-1.0);
    }

if  (wFlag==1)
    {
    strcpy(szReceiveFrom7000,szRec);
    }
return(NoError);

ret_err:
for (j=0; j<7; j++) f7000[j]=(float)0.0;
return(err);
}

/* -------------------------------------------------------------------- */

EXPORTS WORD CALLBACK AnalogOut(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
  char    wPort,wFlag,c1,c2;
  WORD    wAddr,wRetVal,i,wID,wCheckSum,wTimeOut,err,wT,wChannel;
  float   fVal;
  WORD    wRecStrLen;
  char    OutCh[10];

  //---------------------- input -------------------------------------
  wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
  wAddr=w7000[1];         // module address: 0x00 to 0xFF
  wID=w7000[2];           // module ID: 0x7016/21/24
  wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
  wTimeOut=w7000[4];      // TimeOut constant: normal=100
  wChannel=w7000[5];      // Not used if module ID is 7016/7021
                          // Channel No.(0 to 3) if module ID is 7024
  wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string
  fVal=f7000[0];          // analog output value
  //---------------------- output ------------------------------------
                          // void
  if( wID==0x7016 )
  {
     sprintf(OutCh,"%+07.3f",f7000[0]);

     szCmd[0]='$';
     c2=(char)(wAddr&0x0f);
     wAddr=wAddr/16;
     c1=(char)(wAddr&0x0f);
     szCmd[1]=HexToAscii(c1);
     szCmd[2]=HexToAscii(c2);
     szCmd[3]='7';

     szCmd[4]=OutCh[0];
     szCmd[5]=OutCh[1];
     szCmd[6]=OutCh[2];
     szCmd[7]=OutCh[3];
     szCmd[8]=OutCh[4];
     szCmd[9]=OutCh[5];
     szCmd[10]=OutCh[6];
     szCmd[11]=0;
  }
  else if(wID==0x7021)
  {
   szCmd[0]='#';
   c2=(char)(wAddr&0x0f);
   wAddr=wAddr/16;
   c1=(char)(wAddr&0x0f);
   szCmd[1] = HexToAscii(c1);
   szCmd[2] = HexToAscii(c2);

   szCmd[5]='.';
   szCmd[9]=0;


   i=(WORD)(fVal/10.0);
   szCmd[3]=i+0x30; fVal-=i*10;

   i=(int)fVal;
   szCmd[4]=i+0x30; fVal-=i;

   fVal *= (float)10.0;
   i=(int)fVal;
   szCmd[6]=i+0x30; fVal-=(float)i;

   fVal *= (float)10.0;
   i=(int)fVal;
   szCmd[7]=i+0x30; fVal-=(float)i;

   fVal *= (float)10.0;
   i=(int)fVal;
   szCmd[8]=i+0x30;
  }
  else if( wID==0x7024 )
  {
     if( wChannel<0 || wChannel>3 )
     {
         return(AdChannelError);
     }
     sprintf(OutCh,"%+07.3f",f7000[0]);
         // sprintf(OutCh,"%+07.1f",fValue);


     szCmd[0]='#';
     c2=(char)(wAddr&0x0f);
     wAddr=wAddr/16;
     c1=(char)(wAddr&0x0f);
     szCmd[1] = HexToAscii(c1);
     szCmd[2] = HexToAscii(c2);
     szCmd[3]=(char)(wChannel+0x30);  // channel No.

     szCmd[4]=OutCh[0];
     szCmd[5]=OutCh[1];
     szCmd[6]=OutCh[2];
     szCmd[7]=OutCh[3];
     szCmd[8]=OutCh[4];
     szCmd[9]=OutCh[5];
     szCmd[10]=OutCh[6];
     szCmd[11]=0;
  }
  else
     return(ModuleIdError);


  if(wFlag==1)
  {
    strcpy(szSendTo7000,szCmd);
    szReceiveFrom7000[0]=0;
  }

  wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
  if(wRetVal != NoError)
  {
     err=SendCmdError;
     goto ret_err;
  }

  if( wID==0x7016 )
  {
     if(szRec[0] != '!')
     {
        err=ResultStrCheckError;
        goto ret_err;
     }
     if(wCheckSum==0) wRecStrLen=3; else wRecStrLen=5;
  }
  else    // 7021, 7024
  {
     if(szRec[0] != '>')
     {
        err=ResultStrCheckError;
        goto ret_err;
     }
     if(wCheckSum==0) wRecStrLen=1; else wRecStrLen=3;
  }

  if(strlen(szRec) != wRecStrLen)
  {
     err=ResultStrCheckError;
     goto ret_err;
  }

  if(wFlag==1)
  {
     strcpy(szReceiveFrom7000,szRec);
  }
  return(NoError);

ret_err:
  return(err);
}

/* -------------------------------------------------------------------- */

EXPORTS WORD CALLBACK AnalogOutReadBack(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
   char  wPort,wFlag,cc[80],c1,c2;
   WORD  wAddr,wRetVal,i,j,wReadType,wID,wCheckSum,err,wT,wTimeOut,wChannel;
   WORD  wRecStrLen;

   //---------------------- input -------------------------------------
   wPort=(char)w7000[0];  // RS-232 port number: 1/2/3/4
   wAddr=w7000[1];        // module address: 0x00 to 0xFF
   wID=w7000[2];          // module ID: 0x7016/7021/7024
   wCheckSum=w7000[3];    // checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];     // TimeOut constant: normal=100
   wReadType=w7000[5];    // for 7016 :  Don't Care
                          // for 7021/7024
                          //     0: command read back ($AA6)
                          //     1: analog output of current path read back ($AA8)
   wFlag=(char)w7000[6];  // flag: 0=no save, 1=save send/receive string
   wChannel=w7000[7];     //   Channel No.(0 to 3), if wID is 7024
                          //   Not used,            else
   //---------------------- output ------------------------------------
                          // f7000[0]=analog output value read back

   switch(wID)
   {
       case 0x7016:
             szCmd[0]='$';
             c2=(char)(wAddr&0x0f);
             wAddr=wAddr/16;
             c1=(char)(wAddr&0x0f);
             szCmd[1] = HexToAscii(c1);
             szCmd[2] = HexToAscii(c2);
             szCmd[3]='6';
             szCmd[4]=0;
             break;
       case 0x7021:
             if (wReadType==0) szCmd[3]='6';
             else if (wReadType==1) szCmd[3]='8';
             else return(CmdError);

             szCmd[0]='$';
             c2=(char)(wAddr&0x0f);
             wAddr=wAddr/16;
             c1=(char)(wAddr&0x0f);
             szCmd[1] = HexToAscii(c1);
             szCmd[2] = HexToAscii(c2);
             szCmd[4]=0;
             break;
       case 0x7024:
             if( wChannel<0 || wChannel>3 )
                 return(CmdError);
             szCmd[0]='$';
             c2=(char)(wAddr&0x0f);
             wAddr=wAddr/16;
             c1=(char)(wAddr&0x0f);
             szCmd[1] = HexToAscii(c1);
             szCmd[2] = HexToAscii(c2);

             if (wReadType==0) szCmd[3]='6';
             else if (wReadType==1) szCmd[3]='8';
             else return(CmdError);

             szCmd[4]=(char)(wChannel+'0');  // channel N0.
             szCmd[5]=0;
             break;
       default: return(ModuleIdError);
   }


   if(wFlag==1)
   {
      strcpy(szSendTo7000,szCmd);
      szReceiveFrom7000[0]=0;
   }

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wRetVal != NoError)
   {
      err=SendCmdError;
      goto ret_err;
   }

   if(szRec[0] != '!')
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wCheckSum==0) wRecStrLen=9; else wRecStrLen=11;
   if( wID==0x7016 || wID==0x7024 ) wRecStrLen+=1;

   if(strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   i=3; j=0;
   while (szRec[i])
   {
      cc[j++]=szRec[i++];
   }
   cc[j]=0;

   if( wCheckSum )  // checksum enable
      cc[j-2]=0;
   f7000[0]=(float)atof(cc);

   if(wFlag==1)
   {
      strcpy(szReceiveFrom7000,szRec);
   }
   return(NoError);

ret_err:
   f7000[0]=(float)0.0;
   return(err);
}

/* -------------------------------------------------------------------- */
EXPORTS WORD CALLBACK DigitalOut(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
   char  wPort,wFlag,c1,c2,A,B,C,D,i;
   WORD  wAddr,wAddr2,wRetVal,wData,wID,wCheckSum,wTimeOut,err,wT;
   WORD  wRecStrLen;
   UCHAR OutData[4];
   char szCmd2[80];

   //---------------------- input ---------------------------
   wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
   wAddr=w7000[1];         // module address: 0x00 to 0xFF
   wID=w7000[2];           // module ID: 0x7050/60/63/65/66/67/42/43/44
   wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];      // TimeOut constant: normal=100
   wData=w7000[5];         // 16-bit digital data to output
   wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string
   //---------------------- output -----------------------
                        // void

   wAddr2=wAddr;
   switch (wID)
   {
       case 0x7050 : //#AA00DD
       case 0x7060 : //#AA00DD
       case 0x7063 :
       case 0x7065 :
       case 0x7066 :
       case 0x7067 : //#AA00DD
       case 0x7044 : //#AA00DD
                     wID=0;
                     break;
       case 0x7042 : //@AADDDD
       case 0x7043 : //@AADDDD
                     wID=1;
                     break;
       case 0x7011:     // @AADO0?   0?=00  Bit0:Off   Bit1:Off
       case 0x7012:     //           0?=01  Bit0:On    Bit1:Off
       case 0x7014:     //           0?=02  Bit0:Off   Bit1:On
       case 0x7080:     //           0?=03  Bit0:On    Bit1:On
                     wID=2;
                     break;
      /***
       case 0x7016:
                        //  @AADO0?  for Bit0, Bit1
                        //  @AADO1?  for Bit2, Bit3
                     wID=3;
                     break;
      ***/
       default: return(ModuleIdError);
   }

   if(wID == 0) //#AA00DD
   {
      szCmd[0]='#';
      c2=(char)(wAddr&0x0f);
      wAddr=wAddr/16;
      c1=(char)(wAddr&0x0f);
      szCmd[1] = HexToAscii(c1);
      szCmd[2] = HexToAscii(c2);

      B=wData&0x0f; wData=wData/16;
      A=wData&0x0f;

      szCmd[3]='0';
      szCmd[4]='0';
      szCmd[5]=HexToAscii(A);
      szCmd[6]=HexToAscii(B);
      szCmd[7]=0;
   }
   else if (wID == 1) //@AADDDD
   {
      szCmd[0]='@';
      c2=(char)(wAddr&0x0f);
      wAddr=wAddr/16;
      c1=(char)(wAddr&0x0f);
      szCmd[1] = HexToAscii(c1);
      szCmd[2] = HexToAscii(c2);

      D=wData&0x0f; wData=wData/16;
      C=wData&0x0f; wData=wData/16;
      B=wData&0x0f; wData=wData/16;
      A=wData&0x0f;

      szCmd[3]=HexToAscii(A);
      szCmd[4]=HexToAscii(B);
      szCmd[5]=HexToAscii(C);
      szCmd[6]=HexToAscii(D);
      szCmd[7]=0;
   }
   else if(wID==2)  //@AADO??
   {
      if( wData>3 )
      {
         return DataError;
      }

      szCmd[0]='@';
      c2=(char)(wAddr&0x0f);
      wAddr=wAddr/16;
      c1=(char)(wAddr&0x0f);
      szCmd[1] = HexToAscii(c1);
      szCmd[2] = HexToAscii(c2);

      szCmd[3]='D';
      szCmd[4]='O';
      szCmd[5]='0';
      szCmd[6]=wData+'0';
      szCmd[7]=0;
   }

   if(wFlag==1)
   {
      strcpy(szSendTo7000,szCmd);
      szReceiveFrom7000[0]=0;
   }

   if( wID==3 )   // 7016
   {
      wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
      if(wRetVal != NoError)
      {
         err=SendCmdError;
         goto ret_err;
      }
      if(szRec[0] != '!')
      {
         err=ResultStrCheckError;
         goto ret_err;
      }
      Sleep(10);
      wRetVal=Send_Receive_Cmd(wPort, szCmd2, szRec, wTimeOut, wCheckSum,&wT);
      if(wRetVal != NoError)
      {
         err=SendCmdError;
         goto ret_err;
      }
   }
   else
   {
      wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
      if(wRetVal != NoError)
      {
         err=SendCmdError;
         goto ret_err;
      }
   }

   if( wID==0 || wID==1 )  // Digit I/O module
   {
      if(szRec[0] != '>')
      {
         err=ResultStrCheckError;
         goto ret_err;
      }
   }
   else  // 7011, 7012, 7014, 7080
   {
      if(szRec[0] != '!')
      {
         err=ResultStrCheckError;
         goto ret_err;
      }
   }

   if( wID==0 || wID==1 )  // Digit I/O module
   {
      if(wCheckSum==0) wRecStrLen=1; else wRecStrLen=3;
   }
   else
   {
      if(wCheckSum==0) wRecStrLen=3; else wRecStrLen=5;
   }

   if(strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wFlag==1)
   {
      strcpy(szReceiveFrom7000,szRec);
   }
   return(NoError);

ret_err:
   return(err);
}

/*---------- Output a specific bit for a DI/DO module ------------ */
EXPORTS WORD CALLBACK DigitalBitOut(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
   char  wPort,wFlag,c1,c2,A,B,C,D,i;
   WORD  wAddr,wAddr2,wRetVal,wChannel,wData,wID,wCheckSum,wTimeOut,err,wT;
   WORD  wRecStrLen;
   char  szCmd2[80];

   //---------------------- input ---------------------------
   wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
   wAddr=w7000[1];         // module address: 0x00 to 0xFF
   wID=w7000[2];           // module ID: 0x7050/60/67/42/43/44
   wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];      // TimeOut constant: normal=100
   wData=w7000[5];         // don't care
   wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string
   wChannel=w7000[7];      // which channel to output
   wData=w7000[8];         // data to output
   //---------------------- output -----------------------
                        // void

   if( wData>1 )
     return DataError;
   switch (wID)
   {
       case 0x7050 : if( wChannel>7 )
                        return DataError;
                     wID=0;
                     break;
       case 0x7060 : if( wChannel>3 )
                        return DataError;
                     wID=0;
                     break;
       case 0x7063 : if( wChannel>2 )
                        return DataError;
                     wID=0;
                     break;
       case 0x7065 : if( wChannel>4 )
                        return DataError;
                     wID=0;
                     break;
       case 0x7066 : if( wChannel>6 )
                        return DataError;
                     wID=0;
                     break;
       case 0x7067 : if( wChannel>6 )
                        return DataError;
                     wID=0;
                     break;
       case 0x7044 : if( wChannel>7 )
                        return DataError;
                     wID=0;
                     break;
       case 0x7042 : if( wChannel>12 )
                        return DataError;
                     wID=1;
                     break;
       case 0x7043 : if( wChannel>15 )
                        return DataError;
                     wID=1;
                     break;
       default: return(ModuleIdError);
   }

   if(wID == 0) //#AA00DD
   {
      szCmd[0]='#';
      c2=(char)(wAddr&0x0f);
      wAddr=wAddr/16;
      c1=(char)(wAddr&0x0f);
      szCmd[1] = HexToAscii(c1);
      szCmd[2] = HexToAscii(c2);
      szCmd[3]='1';
      szCmd[4]=wChannel+'0';
      szCmd[5]='0';
      szCmd[6]=wData+'0';
      szCmd[7]=0;
   }
   else
   {
      szCmd[0]='#';
      c2=(char)(wAddr&0x0f);
      wAddr=wAddr/16;
      c1=(char)(wAddr&0x0f);
      szCmd[1] = HexToAscii(c1);
      szCmd[2] = HexToAscii(c2);

      if( wChannel>7 )
      {
         szCmd[3]='B';
         wChannel-=8;
      }
      else
      {
         szCmd[3]='A';
      }
      szCmd[4]=wChannel+'0';
      szCmd[5]='0';
      szCmd[6]=wData+'0';
      szCmd[7]=0;
   }

   if(wFlag==1)
   {
      strcpy(szSendTo7000,szCmd);
      szReceiveFrom7000[0]=0;
   }

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wRetVal != NoError)
   {
       err=SendCmdError;
       goto ret_err;
   }

   if(szRec[0] != '>')
   {
       err=ResultStrCheckError;
       goto ret_err;
   }

   if( wID==0 )  // Digit I/O module
   {
      if(wCheckSum==0) wRecStrLen=1; else wRecStrLen=3;
   }

   if(strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wFlag==1)
   {
      strcpy(szReceiveFrom7000,szRec);
   }
   return(NoError);

ret_err:
   return(err);
}

/* -------------------------------------------------------------------- */
EXPORTS WORD CALLBACK DigitalOut_7016(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
   char  wPort,wFlag,c1,c2,A,B,C,D;
   WORD  wAddr,wRetVal,wData,wID,wCheckSum,wTimeOut,err,wT;
   WORD  wRecStrLen,wWhichBits;
   //UCHAR OutData[4];

   //---------------------- input ---------------------------
   wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
   wAddr=w7000[1];         // module address: 0x00 to 0xFF
   wID=w7000[2];           // module ID: 0x7016
   wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];      // TimeOut constant: normal=100
   wData=w7000[5];         // 2-bit digital data to output
   wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string
   wWhichBits=w7000[7];    // 0: first two bits,  1: last two bits
   //---------------------- output -----------------------
                        // void
   switch (wID)
   {
        case 0x7016:  //  @AADO0?  for Bit0, Bit1
                      //  @AADO1?  for Bit2, Bit3
                      break;
        default: return(ModuleIdError);
   }


   // 7016,  @AADO0?,  @AADO1?
   if( wData>3 )
      return DataError;

   if( wWhichBits==0 )  // first 2 bits,  @AADO0?
   {
      //--- Bit0, Bit1 ---
      szCmd[0]='@';
      c2=(char)(wAddr&0x0f);
      wAddr=wAddr/16;
      c1=(char)(wAddr&0x0f);
      szCmd[1] = HexToAscii(c1);
      szCmd[2] = HexToAscii(c2);
      szCmd[3]='D';
      szCmd[4]='O';
      szCmd[5]='0';
      szCmd[6]=wData+'0';
      szCmd[7]=0;
   }
   else   // last 2 bits,  @AADO0?
   {
      //--- Bit2, Bit3 ---
      szCmd[0]='@';
      c2=(char)(wAddr&0x0f);
      wAddr=wAddr/16;
      c1=(char)(wAddr&0x0f);
      szCmd[1] = HexToAscii(c1);
      szCmd[2] = HexToAscii(c2);
      szCmd[3]='D';
      szCmd[4]='O';
      szCmd[5]='1';
      szCmd[6]=wData+'0';
      szCmd[7]=0;
   }

   if(wFlag==1)
   {
      strcpy(szSendTo7000,szCmd);
      szReceiveFrom7000[0]=0;
   }

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wRetVal != NoError)
   {
      err=SendCmdError;
      goto ret_err;
   }

   if(szRec[0] != '!')
   {
      err=ResultStrCheckError;
      goto ret_err;
   }


   if(wCheckSum==0) wRecStrLen=3; else wRecStrLen=5;

   if(strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wFlag==1)
   {
      strcpy(szReceiveFrom7000,szRec);
   }
   return(NoError);

ret_err:
   return(err);
}

/* -------------------------------------------------------------------- */

EXPORTS WORD CALLBACK DigitalOutReadBack(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
char wPort,wFlag,c1,c2;
WORD wAddr,wRetVal,wID,wCheckSum,wTimeOut,err,wT;
WORD A,B,C,D;
WORD wRecStrLen;

//---------------------- input -------------------------------------
wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
wAddr=w7000[1];         // module address: 0x00 to 0xFF
wID=w7000[2];           // module ID: 0x7050/60/67/42/43/44
wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
wTimeOut=w7000[4];      // TimeOut constant: normal=100
wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string
//---------------------- output ------------------------------------
                        // w7000[5]=digital input data read back

switch (wID)
       {
       case 0x7050 : wID=0; break;
       case 0x7060 : wID=0; break;
       case 0x7067 : wID=0; break;
       case 0x7042 : wID=1; break;
       case 0x7043 : wID=1; break;
       case 0x7044 : wID=0; break;
       default: return(ModuleIdError);
       }

   szCmd[0]='$';
   c2=(char)(wAddr&0x0f);
   wAddr=wAddr/16;
   c1=(char)(wAddr&0x0f);
   szCmd[1] = HexToAscii(c1);
   szCmd[2] = HexToAscii(c2);
   szCmd[3]='6';
   szCmd[4]=0;

if  (wFlag==1)
    {
    strcpy(szSendTo7000,szCmd);
    szReceiveFrom7000[0]=0;
    }

wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
if (wRetVal != NoError)
   {
   err=SendCmdError;
   goto ret_err;
   }

if (szRec[0] != '!')
   {
   err=ResultStrCheckError;
   goto ret_err;
   }

if (wCheckSum==0) wRecStrLen=7; else wRecStrLen=9;

if (strlen(szRec) != wRecStrLen)
   {
   err=ResultStrCheckError;
   goto ret_err;
   }

if (wID == 0) // !OO
   {
   A=AsciiToHex(szRec[1]);
   B=AsciiToHex(szRec[2]);
   w7000[5]=B+A*16;
   }
else if (wID ==1)  // !OOOO
   {
   A=AsciiToHex(szRec[1]);
   B=AsciiToHex(szRec[2]);
   C=AsciiToHex(szRec[3]);
   D=AsciiToHex(szRec[4]);
   w7000[5]=D+C*16+B*256+A*4096;
   }

if (wFlag==1)
   {
   strcpy(szReceiveFrom7000,szRec);
   }
return(NoError);

ret_err:
w7000[5]=0;
return(err);
}

/* -------------------------------------------------------------------- */

EXPORTS WORD CALLBACK DigitalIn(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
   char  wPort,wFlag,c1,c2;
   WORD  wAddr,wRetVal,wID,wCheckSum,wTimeOut,err,mask,wT;
   WORD  A,B,C,D;
   WORD  wRecStrLen;

   //---------------------- input -------------------------------------
   wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
   wAddr=w7000[1];         // module address: 0x00 to 0xFF
   wID=w7000[2];           // module ID: 0x7050/52/53/60/63/65/41/44/11/12/14/16
   wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];      // TimeOut constant: normal=100
   wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string
   //---------------------- output ------------------------------------
   //w7000[5]=16-bit digital input data

   switch (wID)
   {
       case 0x7050 : mask=0x007f; wID=1; break; /* IO    */
       case 0x7052 : mask=0x00ff; wID=2; break; /* DI*8  */
       case 0x7053 : mask=0xffff; wID=0; break; /* DI*16 */
       case 0x7060 : mask=0x000f; wID=1; break; /* IO    */
       case 0x7063 : mask=0x00ff; wID=2; break; /* DI*8    */
       case 0x7065 : mask=0x000f; wID=0; break; /* DI*4    */
       case 0x7041 : mask=0x3fff; wID=0; break; /* DI*16 */
       case 0x7044 : mask=0x000f; wID=0; break; /* IO    */
       case 0x7011:
       case 0x7012:
       case 0x7014:
       case 0x7016:  wID=3;  break;
       default: return(ModuleIdError);
   }

   if ((wID==0)||(wID==1)||(wID==2)) //$016
   {
      szCmd[0]='$';
      c2=(char)(wAddr&0x0f);
      wAddr=wAddr/16;
      c1=(char)(wAddr&0x0f);
      szCmd[1] = HexToAscii(c1);
      szCmd[2] = HexToAscii(c2);
      szCmd[3]='6';
      szCmd[4]=0;
   }
   else   // 7011/7012/7014/7016 read Digital Input, @AADI
   {
      szCmd[0]='@';
      c2=(char)(wAddr&0x0f);
      wAddr=wAddr/16;
      c1=(char)(wAddr&0x0f);
      szCmd[1] = HexToAscii(c1);
      szCmd[2] = HexToAscii(c2);
      szCmd[3]='D';
      szCmd[4]='I';
      szCmd[5]=0;
   }

   if(wFlag==1)
   {
      strcpy(szSendTo7000,szCmd);
      szReceiveFrom7000[0]=0;
   }

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wRetVal != NoError)
   {
      // err=SendCmdError;
      err=wRetVal;
      goto ret_err;
   }

   if(szRec[0] != '!')
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   switch( wID )
   {
      case 0:
      case 1:
      case 2:
         if(wCheckSum==0) wRecStrLen=7; else wRecStrLen=9;  break;
      case 3:  // 7011/7012/7014/7016
         if(wCheckSum==0) wRecStrLen=8; else wRecStrLen=10;  break;
   }

   if(strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   switch( wID )
   {
      case 0:
      case 1:
      case 2:
         A=AsciiToHex(szRec[1]);
         B=AsciiToHex(szRec[2]);
         C=AsciiToHex(szRec[3]);
         D=AsciiToHex(szRec[4]);

         if(wID==0) /* 16*DI */
            wT=D+C*16+B*256+A*4096;
         else if (wID==1) /* IO */
            wT=D+C*16;
         else if (wID==2) /* 8*DI */
            wT=B+A*16;
         w7000[5]=wT&mask;
         break;
      case 3:
         A=AsciiToHex(szRec[7]);
         w7000[5]=A;
         break;
   }

   if(wFlag==1)
   {
      strcpy(szReceiveFrom7000,szRec);
   }
   return(NoError);

ret_err:
   w7000[5]=0;
   return(err);
}


EXPORTS WORD CALLBACK DigitalInCounterRead(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
   char wPort,wFlag,c1,c2;
   WORD wAddr,wRetVal,wID,wCheckSum,wTimeOut,wCounter,err,wT,wChannel;
   WORD A,B,C,D,E,F,G,H;
   WORD wRecStrLen,mask;
   char cc[10];

   //---------------------- input -------------------------------------
   wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
   wAddr=w7000[1];         // module address: 0x00 to 0xFF
   wID=w7000[2];           // module ID: 0x7050/52/53/60/63/65/41/44
   wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];      // TimeOut constant: normal=100
   wChannel=w7000[5];      // Channel No.
   wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string
   //---------------------- output ------------------------------------
   // w7000[7]= DigitalIn Counter Value

   switch (wID)
   {
       case 0x7050 : mask=0x007f;
                     if( wChannel>6 )
                        return InvalidateChannelNo;
                     break;
       case 0x7052 : mask=0x00ff;
                     if( wChannel>7 )
                        return InvalidateChannelNo;
                     break;
       case 0x7053 : mask=0xffff;
                     if( wChannel>15 )
                        return InvalidateChannelNo;
                     break;
       case 0x7060 : mask=0x000f;
                     if( wChannel>3 )
                        return InvalidateChannelNo;
                     break;
       case 0x7063 : mask=0x00ff;
                     if( wChannel>7 )
                        return InvalidateChannelNo;
                     break;
       case 0x7065 : mask=0x000f;
                     if( wChannel>3 )
                        return InvalidateChannelNo;
                     break;
       case 0x7041 : mask=0x3fff;
                     if( wChannel>13 )
                        return InvalidateChannelNo;
                     break;
       case 0x7044 : mask=0x000f;
                     if( wChannel>3 )
                        return InvalidateChannelNo;
                     break;
       default: return(ModuleIdError);
   }

   //***   #AAN  read DigitalIn Channel N's Counter Value
   szCmd[0]='#';
   c2=(char)(wAddr&0x0f);
   wAddr=wAddr/16;
   c1=(char)(wAddr&0x0f);
   szCmd[1] = HexToAscii(c1);
   szCmd[2] = HexToAscii(c2);
   szCmd[3]=HexToAscii(wChannel);
   szCmd[4]=0;

   if(wFlag==1)
   {
      strcpy(szSendTo7000,szCmd);
      szReceiveFrom7000[0]=0;
   }

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wRetVal != NoError)
   {
     err=SendCmdError;
     goto ret_err;
   }

   if(szRec[0] != '!')
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wCheckSum==0) wRecStrLen=8; else wRecStrLen=10;

   if( strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   cc[0]=szRec[3];  cc[1]=szRec[4];  cc[2]=szRec[5];
   cc[3]=szRec[6];  cc[4]=szRec[7];  cc[5]=0;
   w7000[7]=(WORD)(atoi(cc));

   if(wFlag==1)
   {
      strcpy(szReceiveFrom7000,szRec);
   }
   return(NoError);

 ret_err:
   return(err);
}


EXPORTS WORD CALLBACK ClearDigitalInCounter(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
   char wPort,wFlag,c1,c2;
   WORD wAddr,wRetVal,wID,wCheckSum,wTimeOut,wCounter,err,wT,wChannel;
   WORD A,B,C,D,E,F,G,H;
   WORD wRecStrLen,mask;

   //---------------------- input -------------------------------------
   wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
   wAddr=w7000[1];         // module address: 0x00 to 0xFF
   wID=w7000[2];           // module ID: 0x7050/52/53/60/63/65/41/44
   wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];      // TimeOut constant: normal=100
   wChannel=w7000[5];      // Channel No.
   wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string

   switch (wID)
   {
       case 0x7050 : mask=0x007f;
                     if( wChannel>6 )
                        return InvalidateChannelNo;
                     break;
       case 0x7052 : mask=0x00ff;
                     if( wChannel>7 )
                        return InvalidateChannelNo;
                     break;
       case 0x7053 : mask=0xffff;
                     if( wChannel>15 )
                        return InvalidateChannelNo;
                     break;
       case 0x7060 : mask=0x000f;
                     if( wChannel>3 )
                        return InvalidateChannelNo;
                     break;
       case 0x7063 : mask=0x00ff;
                     if( wChannel>7 )
                        return InvalidateChannelNo;
                     break;
       case 0x7065 : mask=0x000f;
                     if( wChannel>3 )
                        return InvalidateChannelNo;
                     break;
       case 0x7041 : mask=0x3fff;
                     if( wChannel>13 )
                        return InvalidateChannelNo;
                     break;
       case 0x7044 : mask=0x000f;
                     if( wChannel>3 )
                        return InvalidateChannelNo;
                     break;
       default: return(ModuleIdError);
   }

   //***   $AACN   Clear DigitalIn Channel N's Counter Value
   szCmd[0]='$';
   c2=(char)(wAddr&0x0f);
   wAddr=wAddr/16;
   c1=(char)(wAddr&0x0f);
   szCmd[1] = HexToAscii(c1);
   szCmd[2] = HexToAscii(c2);
   szCmd[3]='C';
   szCmd[4]=wChannel+'0';
   szCmd[5]=0;

   if(wFlag==1)
   {
      strcpy(szSendTo7000,szCmd);
      szReceiveFrom7000[0]=0;
   }

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wRetVal != NoError)
   {
     err=SendCmdError;
     goto ret_err;
   }

   if(wFlag==1)
   {
      strcpy(szReceiveFrom7000,szRec);
   }

   if(szRec[0] != '!')
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wCheckSum==0) wRecStrLen=3; else wRecStrLen=5;

   if( strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError;
      goto ret_err;
   }


   return(NoError);

 ret_err:
   return(err);
}

EXPORTS WORD CALLBACK DigitalInLatch(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
   char wPort,wFlag,c1,c2;
   WORD wAddr,wRetVal,wID,wCheckSum,wTimeOut,wCounter,err,wT,wLatchStatus;
   WORD A,B,C,D,E,F,G,H;
   WORD wRecStrLen,mask;

   //---------------------- input -------------------------------------
   wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
   wAddr=w7000[1];         // module address: 0x00 to 0xFF
   wID=w7000[2];           // module ID: 0x7050/52/53/60/63/65/41/44
   wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];      // TimeOut constant: normal=100
   wLatchStatus=w7000[5];  // 0: select to latch low
                           // 1: select to latch high
   wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string
   //---------------------- output ------------------------------------
   //  w7000[7]: the latched data

   switch (wID)
   {
       case 0x7050 : mask=0x007f; wID=1; break; /* IO    */
       case 0x7052 : mask=0xff;   wID=3; break; /* DI*8  */
       case 0x7053 : mask=0xffff; wID=0; break; /* DI*16 */
       case 0x7060 : mask=0x000f; wID=1; break; /* IO    */
       case 0x7063 : mask=0x00ff; wID=2; break; /* DI*8    */
       case 0x7065 : mask=0x000f; wID=0; break; /* DI*4    */
       case 0x7041 : mask=0x3fff; wID=0; break; /* DI*16 */
       case 0x7044 : mask=0x000f; wID=0; break; /* IO    */
       default: return(ModuleIdError);
   }

   //***   $AALS  S:0 latch low  S:1 latch high
   szCmd[0]='$';
   c2=(char)(wAddr&0x0f);
   wAddr=wAddr/16;
   c1=(char)(wAddr&0x0f);
   szCmd[1] = HexToAscii(c1);
   szCmd[2] = HexToAscii(c2);
   szCmd[3]='L';
   if( wLatchStatus==0 )
      szCmd[4]='0';
   else
      szCmd[4]='1';
   szCmd[5]=0;

   if(wFlag==1)
   {
      strcpy(szSendTo7000,szCmd);
      szReceiveFrom7000[0]=0;
   }

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wRetVal != NoError)
   {
     err=SendCmdError;
     goto ret_err;
   }

   if(szRec[0] != '!')
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wCheckSum==0) wRecStrLen=7; else wRecStrLen=9;

   if( strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   switch( wID )
   {
      case 0:
      case 1:
      case 2:
      case 3:
         A=AsciiToHex(szRec[1]);
         B=AsciiToHex(szRec[2]);
         C=AsciiToHex(szRec[3]);
         D=AsciiToHex(szRec[4]);

         if(wID==0) /* 16*DI */
            wT=D+C*16+B*256+A*4096;
         else if (wID==1) /* IO */
            wT=D+C*16;
         else if (wID==2) /* 8*DI */
            wT=D+C*16;
         else if (wID==3)
            wT=B+A*16;
         w7000[7]=wT&mask;
         break;
    }

   if(wFlag==1)
   {
      strcpy(szReceiveFrom7000,szRec);
   }
   return(NoError);

 ret_err:
   return(err);
}

EXPORTS WORD CALLBACK ClearDigitalInLatch(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
   char wPort,wFlag,c1,c2;
   WORD wAddr,wRetVal,wID,wCheckSum,wTimeOut,wCounter,err,wT,wLatchStatus;
   WORD A,B,C,D,E,F,G,H;
   WORD wRecStrLen,mask;

   //---------------------- input -------------------------------------
   wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
   wAddr=w7000[1];         // module address: 0x00 to 0xFF
   wID=w7000[2];           // module ID: 0x7050/52/53/60/63/65/41/44
   wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];      // TimeOut constant: normal=100
   // wLatchStatus=w7000[5];  // 0: select to latch low
                              // 1: select to latch high
   wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string

   //---   $AAC  ---
   szCmd[0]='$';
   c2=(char)(wAddr&0x0f);
   wAddr=wAddr/16;
   c1=(char)(wAddr&0x0f);
   szCmd[1] = HexToAscii(c1);
   szCmd[2] = HexToAscii(c2);
   szCmd[3]='C';
   szCmd[4]=0;

   if(wFlag==1)
   {
      strcpy(szSendTo7000,szCmd);
      szReceiveFrom7000[0]=0;
   }

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wRetVal != NoError)
   {
     err=SendCmdError;
     goto ret_err;
   }

   if(szRec[0] != '!')
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wCheckSum==0) wRecStrLen=3; else wRecStrLen=5;

   if( strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError;
      goto ret_err;
   }


   if(wFlag==1)
   {
      strcpy(szReceiveFrom7000,szRec);
   }
   return(NoError);

 ret_err:
   return(err);   
}


//---------------------- input -------------------------------------
// wPort   :             // RS-232 port number: 1/2/3/4
// wTotal  :             // number of modules to access
// wTimeOut:             // TimeOut constant: normal=100
// wAddr[?]:             // module address: 0x00 to 0xFF
// wID[?]  :             // module ID: 0x7011/12/13/14/17/18
// wConfig[?]:           // wConfig=08/09/0A/0B/0C/0D for wID=0x7017
// wCheckSum[?]:         // checksum: 0=disable, 1=enable
//---------------------- output ------------------------------------
                         // f7000[?]=analog values

EXPORTS WORD CALLBACK NetworkAnalogIn(WORD wPort, WORD wTotal, WORD wTimeOut, WORD FAR wAddr[],
     WORD FAR wID[], WORD FAR wConfig[], WORD FAR wCheckSum[], float FAR f7000[])
{
//char wFlag;  // cc[80],c1,c2;
WORD wRetVal,i,j,wType;
WORD wBase;    // wRecStrLen,
WORD wTable[20];
float fTable[20];

for (i=0; i<wTotal; i++)
{
wBase=i*8;

switch (wID[i])
       {
       case 0x7011 :
       case 0x7012 :
       case 0x7013 :
       case 0x7014 : wType=0; break;
       case 0x7017 : wType=2; break;
       case 0x7018 : wType=1; break;
       default: return(ModuleIdError);
       }

if (wType == 0)
   {
//---------------------- input -------------------------------------
// w7000[0];             // RS-232 port number: 1/2/3/4
// w7000[1];             // module address: 0x00 to 0xFF
// w7000[2];             // module ID: 0x7011/12/13/14/17/18
// w7000[3];             // checksum: 0=disable, 1=enable
// w7000[4];             // TimeOut constant: normal=100
// w7000[5];             // channel number for multi-channel
// w7000[6];             // flag: 0=no save, 1=save send/receive string
//---------------------- output ------------------------------------
                         // f7000[0]=analog input value
   wTable[0]=wPort;
   wTable[1]=wAddr[i];
   wTable[2]=wID[i];
   wTable[3]=wCheckSum[i];
   wTable[4]=wTimeOut;
   wTable[6]=0;
   wRetVal=AnalogIn(wTable, fTable, szCmd, szRec);

   if (wRetVal==NoError) f7000[wBase]=fTable[0];
   else return(FunctionError);
   }
else if (wType == 1)
   {
//---------------------- input -------------------------------------
// w7000[0];             // RS-232 port number: 1/2/3/4
// w7000[1];             // module address: 0x00 to 0xFF
// w7000[2];             // module ID: 0x7017/18
// w7000[3];             // checksum: 0=disable, 1=enable
// w7000[4];             // TimeOut constant: normal=100
// w7000[6];             // flag: 0=no save, 1=save send/receive string
//---------------------- output ------------------------------------
                         // f7000[0..7]=analog input value

   wTable[0]=wPort;
   wTable[1]=wAddr[i];
   wTable[2]=wID[i];
   wTable[3]=wCheckSum[i];
   wTable[4]=wTimeOut;
   wTable[6]=0;
   wRetVal=AnalogIn8(wTable, fTable, szCmd, szRec);

   if (wRetVal==NoError)
      for (j=0; j<8; j++) f7000[wBase+j]=fTable[j];
   else return(FunctionError);
   }
else if (wType == 2)
   {
//---------------------- input -------------------------------------
// w7000[0];             // RS-232 port number: 1/2/3/4
// w7000[1];             // module address: 0x00 to 0xFF
// w7000[2];             // module ID: 0x7017
// w7000[3];             // checksum: 0=disable, 1=enable
// w7000[4];             // TimeOut constant: normal=100
// w7000[6];             // flag: 0=no save, 1=save send/receive string
//---------------------- output ------------------------------------
                         // f7000[0..7]=analog input value

   wTable[0]=wPort;
   wTable[1]=wAddr[i];
   wTable[2]=wConfig[i];
   wTable[3]=wCheckSum[i];
   wTable[4]=wTimeOut;
   wTable[6]=0;
   wRetVal=In8_7017(wTable, fTable, szCmd, szRec);

   if (wRetVal==NoError)
      for (j=0; j<8; j++) f7000[wBase+j]=fTable[j];
   else return(FunctionError);
   }

}
return(NoError);
}

/* -------------------------------------------------------------------- */

//---------------------- input -------------------------------------
// wPort   :             // RS-232 port number: 1/2/3/4
// wTotal  :             // number of modules to access
// wTimeOut:             // TimeOut constant: normal=100
// wAddr[?]:             // module address: 0x00 to 0xFF
// wID[?]  :             // module ID: 0x7050/52/53/60/41/44
// wConfig[?]:           // reserved
// wCheckSum[?]:         // checksum: 0=disable, 1=enable
//---------------------- output ------------------------------------
                         // w7000[?]=digital input values

EXPORTS WORD CALLBACK NetworkDigitalIn(WORD wPort, WORD wTotal, WORD wTimeOut, WORD FAR wAddr[],
     WORD FAR wID[], WORD FAR wConfig[], WORD FAR wCheckSum[], WORD FAR w7000[])
{
// char  wFlag,cc[80];
WORD wRetVal,i;  //,wType;
WORD wTable[20];
float fTable[20];

for (i=0; i<wTotal; i++)
{
//---------------------- input -------------------------------------
// w7000[0];             // RS-232 port number: 1/2/3/4
// w7000[1];             // module address: 0x00 to 0xFF
// w7000[2];             // module ID: 0x7050/52/53/60/41/44
// w7000[3];             // checksum: 0=disable, 1=enable
// w7000[4];             // TimeOut constant: normal=100
// w7000[6];             // flag: 0=no save, 1=save send/receive string
//---------------------- output ------------------------------------
                         // w7000[5]=digital input data
   wTable[0]=wPort;
   wTable[1]=wAddr[i];
   wTable[2]=wID[i];
   wTable[3]=wCheckSum[i];
   wTable[4]=wTimeOut;
   wTable[6]=0;
   wRetVal=DigitalIn(wTable, fTable, szCmd, szRec);

   if (wRetVal==NoError) w7000[i]=wTable[5];
   else return(FunctionError);
}
return(NoError);
}


/*-------- 7011 Thermocouple Open Detection ---------*/
EXPORTS WORD CALLBACK ThermocoupleOpen_7011(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
   char   wPort,wFlag,c1,c2;
   WORD   wAddr,wRetVal,wID,wCheckSum,wTimeOut,err,wT;
   WORD   wRecStrLen;

   //---------------------- input -------------------------------------
   wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
   wAddr=w7000[1];         // module address: 0x00 to 0xFF
   wID=w7000[2];           // module ID: 0x7011
   wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];      // TimeOut constant: normal=100
   // w7000[5];            // Don't Care
   wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string
   //---------------------- output ------------------------------------
   // w7000[5]   0: Close     1: Open

   if( wID!=0x7011 )
   {
      return(ModuleIdError);
   }

   szCmd[0]='$';    // $AAB
   c2=(char)(wAddr&0x0f);
   wAddr=wAddr/16;
   c1=(char)(wAddr&0x0f);
   szCmd[1] = HexToAscii(c1);
   szCmd[2] = HexToAscii(c2);
   szCmd[3]='B';
   szCmd[4]=0;

   if(wFlag==1)
   {
      strcpy(szSendTo7000,szCmd);
      szReceiveFrom7000[0]=0;
   }

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wRetVal != NoError)
   {
     err=SendCmdError;
     goto ret_err;
   }

   if(szRec[0] != '!')
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wCheckSum==0) wRecStrLen=4; else wRecStrLen=6;

   if( strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wFlag==1)
   {
      strcpy(szReceiveFrom7000,szRec);
   }

   w7000[5]=szRec[3]-'0';

   return(NoError);

 ret_err:
   return(err);
}

/*-------- 7011/7012/7014/7016 Enable Alarm ---------*/
EXPORTS WORD CALLBACK EnableAlarm(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
   char   wPort,wFlag,c1,c2;
   WORD   wAddr,wRetVal,wID,wCheckSum,wTimeOut,wAlarmMode,err,wT;
   WORD   wRecStrLen;

   //---------------------- input -------------------------------------
   wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
   wAddr=w7000[1];         // module address: 0x00 to 0xFF
   wID=w7000[2];           // module ID: 0x7011
   wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];      // TimeOut constant: normal=100
   wAlarmMode=w7000[5];    // 0: Momentary,  1: Latch
   wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string

   switch (wID)
   {
       case 0x7011 :
       case 0x7012 :
       case 0x7014 :
       case 0x7016 :  break;
       default: return(ModuleIdError);
   }

   szCmd[0]='@';    // @AAEAT
   c2=(char)(wAddr&0x0f);
   wAddr=wAddr/16;
   c1=(char)(wAddr&0x0f);
   szCmd[1] = HexToAscii(c1);
   szCmd[2] = HexToAscii(c2);
   szCmd[3]='E';
   szCmd[4]='A';
   if( wAlarmMode==0 )
      szCmd[5]='M';
   else
      szCmd[5]='L';
   szCmd[6]=0;

   if(wFlag==1)
   {
      strcpy(szSendTo7000,szCmd);
      szReceiveFrom7000[0]=0;
   }

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wRetVal != NoError)
   {
     err=SendCmdError;
     goto ret_err;
   }

   if(szRec[0] != '!')
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wCheckSum==0) wRecStrLen=3; else wRecStrLen=5;

   if( strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wFlag==1)
   {
      strcpy(szReceiveFrom7000,szRec);
   }

   return(NoError);

 ret_err:
   return(err);
}


/*-------- 7011/7012/7014/7016 Disable Alarm ---------*/
EXPORTS WORD CALLBACK DisableAlarm(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
   char   wPort,wFlag,c1,c2;
   WORD   wAddr,wRetVal,wID,wCheckSum,wTimeOut,err,wT;
   WORD   wRecStrLen;
   //float  fValue;
   //char   OutCh[10];

   //---------------------- input -------------------------------------
   wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
   wAddr=w7000[1];         // module address: 0x00 to 0xFF
   wID=w7000[2];           // module ID: 0x7011
   wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];      // TimeOut constant: normal=100
   // w7000[5];            don't care
   wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string

   switch (wID)
   {
       case 0x7011 :
       case 0x7012 :
       case 0x7014 :
       case 0x7016 :  break;
       default: return(ModuleIdError);
   }

   szCmd[0]='@';    // @AADA
   c2=(char)(wAddr&0x0f);
   wAddr=wAddr/16;
   c1=(char)(wAddr&0x0f);
   szCmd[1] = HexToAscii(c1);
   szCmd[2] = HexToAscii(c2);
   szCmd[3]='D';
   szCmd[4]='A';
   szCmd[5]=0;

   if(wFlag==1)
   {
      strcpy(szSendTo7000,szCmd);
      szReceiveFrom7000[0]=0;
   }

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wRetVal != NoError)
   {
     err=SendCmdError;
     goto ret_err;
   }

   if(szRec[0] != '!')
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wCheckSum==0) wRecStrLen=3; else wRecStrLen=5;

   if( strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wFlag==1)
   {
      strcpy(szReceiveFrom7000,szRec);
   }

   return(NoError);

 ret_err:
   return(err);
}

/*-------- 7011/7012/7014/7016 Clear Latch Alarm ---------*/
EXPORTS WORD CALLBACK ClearLatchAlarm(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
   char   wPort,wFlag,c1,c2;
   WORD   wAddr,wRetVal,wID,wCheckSum,wTimeOut,err,wT;
   WORD   wRecStrLen;

   //---------------------- input -------------------------------------
   wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
   wAddr=w7000[1];         // module address: 0x00 to 0xFF
   wID=w7000[2];           // module ID: 0x7011
   wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];      // TimeOut constant: normal=100
   // w7000[5];            don't care
   wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string

   switch (wID)
   {
       case 0x7011 :
       case 0x7012 :
       case 0x7014 :
       case 0x7016 : break;
       default: return(ModuleIdError);
   }

   szCmd[0]='@';    // @AACA
   c2=(char)(wAddr&0x0f);
   wAddr=wAddr/16;
   c1=(char)(wAddr&0x0f);
   szCmd[1] = HexToAscii(c1);
   szCmd[2] = HexToAscii(c2);
   szCmd[3]='C';
   szCmd[4]='A';
   szCmd[5]=0;

   if(wFlag==1)
   {
      strcpy(szSendTo7000,szCmd);
      szReceiveFrom7000[0]=0;
   }

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wRetVal != NoError)
   {
     err=SendCmdError;
     goto ret_err;
   }

   if(szRec[0] != '!')
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wCheckSum==0) wRecStrLen=3; else wRecStrLen=5;

   if( strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wFlag==1)
   {
      strcpy(szReceiveFrom7000,szRec);
   }

   return(NoError);

 ret_err:
   return(err);
}

/*-------- 7011/7012/7014/16 Set Hi/Lo Alarm Limit Value ---------*/
EXPORTS WORD CALLBACK SetAlarmLimitValue(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
   char   wPort,wFlag,c1,c2;
   WORD   wAddr,wRetVal,wID,wCheckSum,wTimeOut,wHiLo,err,wT,wRangeCode;
   WORD   wRecStrLen,wPoints;
   float  fValue;
   char   OutCh[10];

   //---------------------- input -------------------------------------
   wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
   wAddr=w7000[1];         // module address: 0x00 to 0xFF
   wID=w7000[2];           // module ID: 0x7011
   wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];      // TimeOut constant: normal=100
   wHiLo=w7000[5];         // 0: Lo Alarm    1: High Alarm
   wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string
   wRangeCode=w7000[7];
   fValue=f7000[0];        // Alarm Limit Value

   switch (wID)
   {
       case 0x7011 :
       case 0x7012 :
       case 0x7014 :
       case 0x7016 :  break;
       default: return(ModuleIdError);
   }

//   sprintf(OutCh,"%+07.2f",fValue);

   if( wID==7016 )
      if( wRangeCode>6 )
         return ExceedInputRange;

   switch( wRangeCode )
   {
      case 0:
           if( fValue>15.00 )
              return ExceedInputRange;
           if( fValue<-15.00 )
              return UnderInputRange;
           wPoints=3;
           break;
      case 1:
           if( fValue>50.00 )
              return ExceedInputRange;
           if( fValue<-50.00 )
              return UnderInputRange;
           wPoints=3;
           break;
      case 2:
           if( fValue>100.00 )
              return ExceedInputRange;
           if( fValue<-100.00 )
              return UnderInputRange;
           wPoints=2;
           break;
      case 3:
           if( fValue>500.00 )
              return ExceedInputRange;
           if( fValue<-500.00 )
              return UnderInputRange;
           wPoints=2;
           break;
      case 4:
           if( fValue>1.00 )
              return ExceedInputRange;
           if( fValue<-1.00 )
              return UnderInputRange;
           wPoints=4;
           break;
      case 5:
           if( fValue>2.50 )
              return ExceedInputRange;
           if( fValue<-2.50 )
              return UnderInputRange;
           wPoints=4;
           break;
      case 6:
           if( fValue>20.00 )
              return ExceedInputRange;
           if( fValue<-20.00 )
              return UnderInputRange;
           wPoints=3;
           break;
      case 7:
           if( fValue>500.00 )
              return ExceedInputRange;
           if( fValue<-500.00 )
              return UnderInputRange;
           wPoints=2;
           break;
      case 8:
           if( fValue>10.00 )
              return ExceedInputRange;
           if( fValue<-10.00 )
              return UnderInputRange;
           wPoints=3;
           break;
      case 9:
           if( fValue>5.00 )
              return ExceedInputRange;
           if( fValue<-5.00 )
              return UnderInputRange;
           wPoints=4;
           break;
      case 0x0A:
           if( fValue>1.00 )
              return ExceedInputRange;
           if( fValue<-1.00 )
              return UnderInputRange;
           wPoints=4;
           break;
      case 0x0B:
           if( fValue>500.00 )
              return ExceedInputRange;
           if( fValue<-500.00 )
              return UnderInputRange;
           wPoints=2;
           break;
      case 0x0C:
           if( fValue>150.00 )
              return ExceedInputRange;
           if( fValue<-150.00 )
              return UnderInputRange;
           wPoints=2;
           break;
      case 0x0D:
           if( fValue>20.00 )
              return ExceedInputRange;
           if( fValue<-20.00 )
              return UnderInputRange;
           wPoints=3;
           break;
      case 0x0E:
           if( fValue>760.00 )
              return ExceedInputRange;
           if( fValue<0.00 )
              return UnderInputRange;
           wPoints=3;
           break;
      case 0x0F:
           if( fValue>1320.00 )
              return ExceedInputRange;
           if( fValue<0.00 )
              return UnderInputRange;
           wPoints=1;
           break;
      case 0x10:
           if( fValue>400.00 )
              return ExceedInputRange;
           if( fValue<-100.00 )
              return UnderInputRange;
           wPoints=2;
           break;
      case 0x11:
           if( fValue>1000.00 )
              return ExceedInputRange;
           if( fValue<0.00 )
              return UnderInputRange;
           wPoints=1;
           break;
      case 0x12:
           if( fValue>1750.00 )
              return ExceedInputRange;
           if( fValue<500.00 )
              return UnderInputRange;
           wPoints=1;
           break;
      case 0x13:
           if( fValue>1750.00 )
              return ExceedInputRange;
           if( fValue<500.00 )
              return UnderInputRange;
           wPoints=1;
           break;
      case 0x14:
           if( fValue>1800.00 )
              return ExceedInputRange;
           if( fValue<500.00 )
              return UnderInputRange;
           wPoints=1;
           break;
      case 0x15:
           if( fValue>1300.00 )
              return ExceedInputRange;
           if( fValue<-270.00 )
              return UnderInputRange;
           wPoints=1;
           break;
      case 0x16:
           if( fValue>2320.00 )
              return ExceedInputRange;
           if( fValue<0.00 )
              return UnderInputRange;
           wPoints=1;
           break;
       default: return(ModuleIdError);
   }

   switch( wPoints )
   {
      case 1:
         sprintf(OutCh,"%+07.1f",fValue);
         break;
      case 2:
         sprintf(OutCh,"%+07.2f",fValue);
         break;
      case 3:
         sprintf(OutCh,"%+07.3f",fValue);
         break;
      case 4:
         sprintf(OutCh,"%+07.4f",fValue);
         break;
      default: return(ModuleIdError);
   }

                    // @AAHI(data): set high alarm limit
   szCmd[0]='@';    // @AALO(data): set low alarm limit
   c2=(char)(wAddr&0x0f);
   wAddr=wAddr/16;
   c1=(char)(wAddr&0x0f);
   szCmd[1] = HexToAscii(c1);
   szCmd[2] = HexToAscii(c2);
   if( wHiLo==0 )   //  Low Alarm
   {
      szCmd[3]='L';
      szCmd[4]='O';
   }
   else
   {
      szCmd[3]='H';
      szCmd[4]='I';
   }

   szCmd[5]=OutCh[0];
   szCmd[6]=OutCh[1];
   szCmd[7]=OutCh[2];
   szCmd[8]=OutCh[3];
   szCmd[9]=OutCh[4];
   szCmd[10]=OutCh[5];
   szCmd[11]=OutCh[6];
   szCmd[12]=0;

   if(wFlag==1)
   {
      strcpy(szSendTo7000,szCmd);
      szReceiveFrom7000[0]=0;
   }

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wRetVal != NoError)
   {
     err=SendCmdError;
     goto ret_err;
   }

   if(szRec[0] != '!')
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wCheckSum==0) wRecStrLen=3; else wRecStrLen=5;

   if( strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wFlag==1)
   {
      strcpy(szReceiveFrom7000,szRec);
   }

   return(NoError);

 ret_err:
   return(err);
}


/*-------- 7011/7012/7014/7016 Read Hi/Lo Alarm Limit Value ---------*/
EXPORTS WORD CALLBACK ReadAlarmLimitValue(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
   char   wPort,wFlag,c1,c2;
   WORD   i,wAddr,wRetVal,wID,wCheckSum,wTimeOut,wHiLo,err,wT;
   WORD   wRecStrLen;
   //float  fValue;
   char   Buffer[10];

   //---------------------- input -------------------------------------
   wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
   wAddr=w7000[1];         // module address: 0x00 to 0xFF
   wID=w7000[2];           // module ID: 0x7011
   wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];      // TimeOut constant: normal=100
   wHiLo=w7000[5];         // 0: Lo Alarm    1: High Alarm
   wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string
   //---------------------- output -------------------------------------
   //fValue=f7000[0];      // Alarm Limit Value

   switch (wID)
   {
       case 0x7011 :
       case 0x7012 :
       case 0x7014 :
       case 0x7016 : break;
       default: return(ModuleIdError);
   }


                    // @AARH(data): read high alarm limit
   szCmd[0]='@';    // @AARL(data): read low  alarm limit
   c2=(char)(wAddr&0x0f);
   wAddr=wAddr/16;
   c1=(char)(wAddr&0x0f);
   szCmd[1] = HexToAscii(c1);
   szCmd[2] = HexToAscii(c2);
   if( wHiLo==0 )   //  Low Alarm
   {
      szCmd[3]='R';
      szCmd[4]='L';
   }
   else
   {
      szCmd[3]='R';
      szCmd[4]='H';
   }
   szCmd[5]=0;

   if(wFlag==1)
   {
      strcpy(szSendTo7000,szCmd);
      szReceiveFrom7000[0]=0;
   }

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wRetVal != NoError)
   {
     err=SendCmdError;
     goto ret_err;
   }

   if(szRec[0] != '!')
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wCheckSum==0) wRecStrLen=10; else wRecStrLen=12;

   if( strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wFlag==1)
   {
      strcpy(szReceiveFrom7000,szRec);
   }

   for(i=0; i<7; i++)
       Buffer[i]=szRec[i+3];
   Buffer[7]=0;
   f7000[0]=(float) atof(Buffer);

   return(NoError);

 ret_err:
   return(err);
}

/*-------- 7011/7012/7014/7016 Read Event Counter --------*/
EXPORTS WORD CALLBACK ReadEventCounter(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
   char   wPort,wFlag,c1,c2;
   WORD   wAddr,wRetVal,wID,wCheckSum,wTimeOut,err,wT;
   WORD   wRecStrLen;

   //---------------------- input -----------------------------------
   wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
   wAddr=w7000[1];         // module address: 0x00 to 0xFF
   wID=w7000[2];           // module ID: 0x7011
   wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];      // TimeOut constant: normal=100
   // wCount=w7000[5];     // don't care
   wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string
   //---------------------- output ----------------------------------
   // wCount=w7000[7];     // The Event Counter Value

   switch (wID)
   {
       case 0x7011 :
       case 0x7012 :
       case 0x7014 :
       case 0x7016 :  break;
       default: return(ModuleIdError);
   }

   szCmd[0]='@';    // @AARE
   c2=(char)(wAddr&0x0f);
   wAddr=wAddr/16;
   c1=(char)(wAddr&0x0f);
   szCmd[1] = HexToAscii(c1);
   szCmd[2] = HexToAscii(c2);
   szCmd[3]='R';
   szCmd[4]='E';
   szCmd[5]=0;

   if(wFlag==1)
   {
      strcpy(szSendTo7000,szCmd);
      szReceiveFrom7000[0]=0;
   }

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wRetVal != NoError)
   {
     err=SendCmdError;
     goto ret_err;
   }

   if(szRec[0] != '!')
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wCheckSum==0) wRecStrLen=8; else wRecStrLen=10;

   if( strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wFlag==1)
   {
      strcpy(szReceiveFrom7000,szRec);
   }

   w7000[7]=(WORD)(atoi(szRec+3));

   return(NoError);

 ret_err:
   return(err);
}


/*-------- 7011/7012/7014/7016 Clear Event Counter --------*/
EXPORTS WORD CALLBACK ClearEventCounter(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
   char   wPort,wFlag,c1,c2;
   WORD   wAddr,wRetVal,wID,wCheckSum,wTimeOut,err,wT;
   WORD   wRecStrLen;

   //---------------------- input -----------------------------------
   wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
   wAddr=w7000[1];         // module address: 0x00 to 0xFF
   wID=w7000[2];           // module ID: 0x7011
   wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];      // TimeOut constant: normal=100
   // wCount=w7000[5];     // don't care
   wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string

   switch (wID)
   {
       case 0x7011 :
       case 0x7012 :
       case 0x7014 :
       case 0x7016 :  break;
       default: return(ModuleIdError);
   }

   szCmd[0]='@';    // @AACE
   c2=(char)(wAddr&0x0f);
   wAddr=wAddr/16;
   c1=(char)(wAddr&0x0f);
   szCmd[1] = HexToAscii(c1);
   szCmd[2] = HexToAscii(c2);
   szCmd[3]='C';
   szCmd[4]='E';
   szCmd[5]=0;

   if(wFlag==1)
   {
      strcpy(szSendTo7000,szCmd);
      szReceiveFrom7000[0]=0;
   }

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wRetVal != NoError)
   {
     err=SendCmdError;
     goto ret_err;
   }

   if(szRec[0] != '!')
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wCheckSum==0) wRecStrLen=3; else wRecStrLen=5;

   if( strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wFlag==1)
   {
      strcpy(szReceiveFrom7000,szRec);
   }

   return(NoError);

 ret_err:
   return(err);
}

/*-------- Set LED channel Display for 7033 --------*/
EXPORTS WORD CALLBACK SetLedDisplay_7033(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
   char   wPort,wFlag,c1,c2;
   WORD   wAddr,wRetVal,wID,wCheckSum,wTimeOut,err,wT,wChannel;
   WORD   wRecStrLen;

   //---------------------- input -----------------------------------
   wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
   wAddr=w7000[1];         // module address: 0x00 to 0xFF
   wID=w7000[2];           // module ID: 0x7033
   wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];      // TimeOut constant: normal=100
   wChannel=w7000[5];      // LED channel to set
   wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string

   switch (wID)
   {
       case 0x7033 :  break;
       default: return(ModuleIdError);
   }

   if( wChannel>2 )
      return(AdChannelError);

   szCmd[0]='$';    // $AA8N
   c2=(char)(wAddr&0x0f);
   wAddr=wAddr/16;
   c1=(char)(wAddr&0x0f);
   szCmd[1] = HexToAscii(c1);
   szCmd[2] = HexToAscii(c2);
   szCmd[3]='8';
   szCmd[4]=wChannel+'0';
   szCmd[5]=0;

   if(wFlag==1)
   {
      strcpy(szSendTo7000,szCmd);
      szReceiveFrom7000[0]=0;
   }

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wRetVal != NoError)
   {
     err=SendCmdError;
     goto ret_err;
   }

   if(szRec[0] != '!')
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wCheckSum==0) wRecStrLen=3; else wRecStrLen=5;

   if( strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wFlag==1)
   {
      strcpy(szReceiveFrom7000,szRec);
   }

   return(NoError);

 ret_err:
   return(err);
}

/*-------- Set LED channel Display for 7016, 7033 --------*/
EXPORTS WORD CALLBACK SetLedDisplay(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
   char   wPort,wFlag,c1,c2;
   WORD   wAddr,wRetVal,wID,wCheckSum,wTimeOut,err,wT,wChannel;
   WORD   wRecStrLen;

   //---------------------- input -----------------------------------
   wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
   wAddr=w7000[1];         // module address: 0x00 to 0xFF
   wID=w7000[2];           // module ID: 0x7013, 0x7033
   wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];      // TimeOut constant: normal=100
   wChannel=w7000[5];      // LED channel to set
   wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string

   switch (wID)
   {
       case 0x7016 :  wID=0;
                      break;
       case 0x7033 :  wID=1;
                      break;
       default: return(ModuleIdError);
   }


   if( wID==0 )  // 7016
   {
       if( wChannel>1 )
           return(AdChannelError);
        szCmd[0]='$';    // $AA3V
        c2=(char)(wAddr&0x0f);
        wAddr=wAddr/16;
        c1=(char)(wAddr&0x0f);
        szCmd[1] = HexToAscii(c1);
        szCmd[2] = HexToAscii(c2);
        szCmd[3]='3';
        szCmd[4]=wChannel+'0';
        szCmd[5]=0;
   }
   else
   {
       if( wChannel>2 )
          return(AdChannelError);
        szCmd[0]='$';    // $AA8N
        c2=(char)(wAddr&0x0f);
        wAddr=wAddr/16;
        c1=(char)(wAddr&0x0f);
        szCmd[1] = HexToAscii(c1);
        szCmd[2] = HexToAscii(c2);
        szCmd[3]='8';
        szCmd[4]=wChannel+'0';
        szCmd[5]=0;
   }

   if(wFlag==1)
   {
      strcpy(szSendTo7000,szCmd);
      szReceiveFrom7000[0]=0;
   }

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wRetVal != NoError)
   {
     err=SendCmdError;
     goto ret_err;
   }

   if(szRec[0] != '!')
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wCheckSum==0) wRecStrLen=3; else wRecStrLen=5;

   if( strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wFlag==1)
   {
      strcpy(szReceiveFrom7000,szRec);
   }

   return(NoError);

 ret_err:
   return(err);
}

/*-------- Get active LED channel Display for 7013, 7033 --------*/
EXPORTS WORD CALLBACK GetLedDisplay(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
   char   wPort,wFlag,c1,c2;
   WORD   wAddr,wRetVal,wID,wCheckSum,wTimeOut,err,wT;
   WORD   wRecStrLen;

   //---------------------- input -----------------------------------
   wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
   wAddr=w7000[1];         // module address: 0x00 to 0xFF
   wID=w7000[2];                 // module ID: 0x7013, 0x7033
   wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];      // TimeOut constant: normal=100
   // wChannel=w7000[5];   // which LED channel to display
   wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string

   switch (wID)
   {
       case 0x7016 :  wID=0;  break;
       case 0x7033 :  wID=1;  break;
       default: return(ModuleIdError);
   }

   if( wID==0 )
   {
      szCmd[0]='$';    // 7016, $AA3
      c2=(char)(wAddr&0x0f);
      wAddr=wAddr/16;
      c1=(char)(wAddr&0x0f);
      szCmd[1] = HexToAscii(c1);
      szCmd[2] = HexToAscii(c2);
      szCmd[3]='3';
      szCmd[4]=0;
   }
   else
   {
      szCmd[0]='$';    // 7033, $AA8
      c2=(char)(wAddr&0x0f);
      wAddr=wAddr/16;
      c1=(char)(wAddr&0x0f);
      szCmd[1] = HexToAscii(c1);
      szCmd[2] = HexToAscii(c2);
      szCmd[3]='8';
      szCmd[4]=0;
   }

   if(wFlag==1)
   {
      strcpy(szSendTo7000,szCmd);
      szReceiveFrom7000[0]=0;
   }

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wRetVal != NoError)
   {
     err=SendCmdError;
     goto ret_err;
   }

   if(szRec[0] != '!')
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wCheckSum==0) wRecStrLen=4; else wRecStrLen=6;

   if( strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wFlag==1)
   {
      strcpy(szReceiveFrom7000,szRec);
   }

   w7000[5]=szRec[3]-'0';

   return(NoError);

 ret_err:
   return(err);
}


/*******************************************************/
/*-------- setup linear mapping for 7014/7016 ---------*/
/*  making a linear mapping from [a,b] to [c,d]        */
/*  where:  f7000[0]=a,  f7000[1]=b                    */
/*          f7000[2]=c,  f7000[3]=d                    */
/*******************************************************/
EXPORTS WORD CALLBACK SetupLinearMapping(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
   char   wPort,wFlag,c1,c2;
   WORD   wAddr,wRetVal,wID,wCheckSum,wTimeOut,wHiLo,err,wT,wRangeCode;
   WORD   wRecStrLen,wPoints;
   float  fSourceLow,fSourceHigh,fTargetLow,fTargetHigh;
   char   OutCh[20];

   //---------------------- input -------------------------------------
   wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
   wAddr=w7000[1];         // module address: 0x00 to 0xFF
   wID=w7000[2];           // module ID: 0x7014, 7016
   wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];      // TimeOut constant: normal=100
   wHiLo=w7000[5];         // 0: Lo Alarm    1: High Alarm
   wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string
   wRangeCode=w7000[7];
   //fValue=f7000[0];      // Alarm Limit Value
   fSourceLow  = f7000[0];
   fSourceHigh = f7000[1];
   fTargetLow  = f7000[2];
   fTargetHigh = f7000[3];

   switch (wID)
   {
       case 0x7014 :
       case 0x7016 :  break;
       default: return(ModuleIdError);
   }

//   sprintf(OutCh,"%+07.2f",fValue);

   if( wID==7016 )
      if( wRangeCode>6 )
         return ExceedInputRange;

   if( fSourceLow>fSourceHigh )
      return  DataError;

   switch( wRangeCode )
   {
      case 0:
           if( fSourceHigh>15.00 )
              return ExceedInputRange;
           if( fSourceLow<-15.00 )
              return UnderInputRange;
           wPoints=3;
           break;
      case 1:
           if( fSourceHigh>50.00 )
              return ExceedInputRange;
           if( fSourceLow<-50.00 )
              return UnderInputRange;
           wPoints=3;
           break;
      case 2:
           if( fSourceHigh>100.00 )
              return ExceedInputRange;
           if( fSourceLow<-100.00 )
              return UnderInputRange;
           wPoints=2;
           break;
      case 3:
           if( fSourceHigh>500.00 )
              return ExceedInputRange;
           if( fSourceLow<-500.00 )
              return UnderInputRange;
           wPoints=2;
           break;
      case 4:
           if( fSourceHigh>1.00 )
              return ExceedInputRange;
           if( fSourceLow<-1.00 )
              return UnderInputRange;
           wPoints=4;
           break;
      case 5:
           if( fSourceHigh>2.50 )
              return ExceedInputRange;
           if( fSourceLow<-2.50 )
              return UnderInputRange;
           wPoints=4;
           break;
      case 6:
           if( fSourceHigh>20.00 )
              return ExceedInputRange;
           if( fSourceLow<-20.00 )
              return UnderInputRange;
           wPoints=3;
           break;
      case 7:
           if( fSourceHigh>500.00 )
              return ExceedInputRange;
           if( fSourceLow<-500.00 )
              return UnderInputRange;
           wPoints=2;
           break;
      case 8:
           if( fSourceHigh>10.00 )
              return ExceedInputRange;
           if( fSourceLow<-10.00 )
              return UnderInputRange;
           wPoints=3;
           break;
      case 9:
           if( fSourceHigh>5.00 )
              return ExceedInputRange;
           if( fSourceLow<-5.00 )
              return UnderInputRange;
           wPoints=4;
           break;
      case 0x0A:
           if( fSourceHigh>1.00 )
              return ExceedInputRange;
           if( fSourceLow<-1.00 )
              return UnderInputRange;
           wPoints=4;
           break;
      case 0x0B:
           if( fSourceHigh>500.00 )
              return ExceedInputRange;
           if( fSourceLow<-500.00 )
              return UnderInputRange;
           wPoints=2;
           break;
      case 0x0C:
           if( fSourceHigh>150.00 )
              return ExceedInputRange;
           if( fSourceLow<-150.00 )
              return UnderInputRange;
           wPoints=2;
           break;
      case 0x0D:
           if( fSourceHigh>20.00 )
              return ExceedInputRange;
           if( fSourceLow<-20.00 )
              return UnderInputRange;
           wPoints=3;
           break;
      case 0x0E:
           if( fSourceHigh>760.00 )
              return ExceedInputRange;
           if( fSourceLow<0.00 )
              return UnderInputRange;
           wPoints=3;
           break;
      case 0x0F:
           if( fSourceHigh>1320.00 )
              return ExceedInputRange;
           if( fSourceLow<0.00 )
              return UnderInputRange;
           wPoints=1;
           break;
      case 0x10:
           if( fSourceHigh>400.00 )
              return ExceedInputRange;
           if( fSourceLow<-100.00 )
              return UnderInputRange;
           wPoints=2;
           break;
      case 0x11:
           if( fSourceHigh>1000.00 )
              return ExceedInputRange;
           if( fSourceLow<0.00 )
              return UnderInputRange;
           wPoints=1;
           break;
      case 0x12:
           if( fSourceHigh>1750.00 )
              return ExceedInputRange;
           if( fSourceLow<500.00 )
              return UnderInputRange;
           wPoints=1;
           break;
      case 0x13:
           if( fSourceHigh>1750.00 )
              return ExceedInputRange;
           if( fSourceLow<500.00 )
              return UnderInputRange;
           wPoints=1;
           break;
      case 0x14:
           if( fSourceHigh>1800.00 )
              return ExceedInputRange;
           if( fSourceLow<500.00 )
              return UnderInputRange;
           wPoints=1;
           break;
      case 0x15:
           if( fSourceHigh>1300.00 )
              return ExceedInputRange;
           if( fSourceLow<-270.00 )
              return UnderInputRange;
           wPoints=1;
           break;
      case 0x16:
           if( fSourceHigh>2320.00 )
              return ExceedInputRange;
           if( fSourceLow<0.00 )
              return UnderInputRange;
           wPoints=1;
           break;
       default: return(ModuleIdError);
   }

   switch( wPoints )
   {
      case 1:
         sprintf(OutCh,"%+07.1f%+07.1f",fSourceLow,fSourceHigh);
         break;
      case 2:
         sprintf(OutCh,"%+07.2f%+07.2f",fSourceLow,fSourceHigh);
         break;
      case 3:
         sprintf(OutCh,"%+07.3f%+07.3f",fSourceLow,fSourceHigh);
         break;
      case 4:
         sprintf(OutCh,"%+07.4f%+07.4f",fSourceLow,fSourceHigh);
         break;
      default: return(ModuleIdError);
   }


   if( w7000[2]==0x7014 )  // $AA6(data*2)
   {
       szCmd[0]='$';
       c2=(char)(wAddr&0x0f);
       wAddr=wAddr/16;
       c1=(char)(wAddr&0x0f);
       szCmd[1] = HexToAscii(c1);
       szCmd[2] = HexToAscii(c2);
       szCmd[3]='6';
   }
   else    // 7016, @AA6(data*2)
   {
       szCmd[0]='@';
       c2=(char)(wAddr&0x0f);
       wAddr=wAddr/16;
       c1=(char)(wAddr&0x0f);
       szCmd[1] = HexToAscii(c1);
       szCmd[2] = HexToAscii(c2);
       szCmd[3]='6';
   }
   szCmd[4]=OutCh[0];
   szCmd[5]=OutCh[1];
   szCmd[6]=OutCh[2];
   szCmd[7]=OutCh[3];
   szCmd[8]=OutCh[4];
   szCmd[9]=OutCh[5];
   szCmd[10]=OutCh[6];

   szCmd[11]=OutCh[7];
   szCmd[12]=OutCh[8];
   szCmd[13]=OutCh[9];
   szCmd[14]=OutCh[10];
   szCmd[15]=OutCh[11];
   szCmd[16]=OutCh[12];
   szCmd[17]=OutCh[13];
   szCmd[18]=0;

   if(wFlag==1)
   {
      strcpy(szSendTo7000,szCmd);
      szReceiveFrom7000[0]=0;
   }

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wRetVal != NoError)
   {
     err=SendCmdError;
     goto ret_err;
   }

   if(szRec[0] != '!')
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wCheckSum==0) wRecStrLen=3; else wRecStrLen=5;

   if( strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wFlag==1)
   {
      strcpy(szReceiveFrom7000,szRec);
   }
   Sleep(100);
   /******************************************************************/
   if( fTargetLow>19999.0 )
      return ExceedInputRange;
   else if( fTargetLow>10000.0 )
      sprintf(OutCh,"%+07.1f",fTargetLow);
   else if( fTargetLow>1000.0 )
      sprintf(OutCh,"%+07.1f",fTargetLow);
   else if( fTargetLow>100.0 )
      sprintf(OutCh,"%+07.2f",fTargetLow);
   else if( fTargetLow>10.0 )
      sprintf(OutCh,"%+07.3f",fTargetLow);
   else if( fTargetLow>1.0 )
      sprintf(OutCh,"%+07.4f",fTargetLow);
   else if( fTargetLow>0.0 )
      sprintf(OutCh,"%+07.5f",fTargetLow);
   else if( fTargetLow>-1.0 )
      sprintf(OutCh,"%+07.4f",fTargetLow);
   else if( fTargetLow>-10.0 )
      sprintf(OutCh,"%+07.3f",fTargetLow);
   else if( fTargetLow>-100.0 )
      sprintf(OutCh,"%+07.2f",fTargetLow);
   else if( fTargetLow>-1000.0 )
      sprintf(OutCh,"%+07.1f",fTargetLow);
   else if( fTargetLow>-19999.0 )
      sprintf(OutCh,"%+07.1f",fTargetLow);
   else
      return UnderInputRange;

   if( fTargetHigh>19999.0 )
      return ExceedInputRange;
   else if( fTargetHigh>10000.0 )
      sprintf(OutCh+7,"%+07.1f",fTargetHigh);
   else if( fTargetHigh>1000.0 )
      sprintf(OutCh+7,"%+07.1f",fTargetHigh);
   else if( fTargetHigh>100.0 )
      sprintf(OutCh+7,"%+07.2f",fTargetHigh);
   else if( fTargetHigh>10.0 )
      sprintf(OutCh+7,"%+07.3f",fTargetHigh);
   else if( fTargetHigh>1.0 )
      sprintf(OutCh+7,"%+07.4f",fTargetHigh);
   else if( fTargetHigh>0.0 )
      sprintf(OutCh+7,"%+07.5f",fTargetHigh);
   else if( fTargetHigh>-1.0 )
      sprintf(OutCh+7,"%+07.4f",fTargetHigh);
   else if( fTargetHigh>-10.0 )
      sprintf(OutCh+7,"%+07.3f",fTargetHigh);
   else if( fTargetHigh>-100.0 )
      sprintf(OutCh+7,"%+07.2f",fTargetHigh);
   else if( fTargetHigh>-1000.0 )
      sprintf(OutCh+7,"%+07.1f",fTargetHigh);
   else if( fTargetHigh>-19999.0 )
      sprintf(OutCh+7,"%+07.1f",fTargetHigh);
   else
      return UnderInputRange;

   wAddr=w7000[1];         // module address: 0x00 to 0xFF
   if( w7000[2]==0x7014 )  // $AA7(data*2)
   {
       szCmd[0]='$';
       c2=(char)(wAddr&0x0f);
       wAddr=wAddr/16;
       c1=(char)(wAddr&0x0f);
       szCmd[1] = HexToAscii(c1);
       szCmd[2] = HexToAscii(c2);
       szCmd[3]='7';
   }
   else    // 7016, @AA7(data*2)
   {
       szCmd[0]='@';
       c2=(char)(wAddr&0x0f);
       wAddr=wAddr/16;
       c1=(char)(wAddr&0x0f);
       szCmd[1] = HexToAscii(c1);
       szCmd[2] = HexToAscii(c2);
       szCmd[3]='7';
   }
   szCmd[4]=OutCh[0];
   szCmd[5]=OutCh[1];
   szCmd[6]=OutCh[2];
   szCmd[7]=OutCh[3];
   szCmd[8]=OutCh[4];
   szCmd[9]=OutCh[5];
   szCmd[10]=OutCh[6];

   szCmd[11]=OutCh[7];
   szCmd[12]=OutCh[8];
   szCmd[13]=OutCh[9];
   szCmd[14]=OutCh[10];
   szCmd[15]=OutCh[11];
   szCmd[16]=OutCh[12];
   szCmd[17]=OutCh[13];
   szCmd[18]=0;

   if(wFlag==1)
   {
      strcpy(szSendTo7000,szCmd);
      szReceiveFrom7000[0]=0;
   }

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wRetVal != NoError)
   {
     err=SendCmdError;
     goto ret_err;
   }

   if(szRec[0] != '!')
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wCheckSum==0) wRecStrLen=3; else wRecStrLen=5;

   if( strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wFlag==1)
   {
      strcpy(szReceiveFrom7000,szRec);
   }
   Sleep(100);

   return(NoError);

 ret_err:
   return(err);
}

/*-------- Enable Linear Mapping for 7014/7016 ---------*/
EXPORTS WORD CALLBACK EnableLinearMapping(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
   char   wPort,wFlag,c1,c2;
   WORD   wAddr,wRetVal,wID,wCheckSum,wTimeOut,err,wT;
   WORD   wRecStrLen;

   //---------------------- input -------------------------------------
   wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
   wAddr=w7000[1];         // module address: 0x00 to 0xFF
   wID=w7000[2];           // module ID: 0x7014, 0x7016
   wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];      // TimeOut constant: normal=100
   // w7000[5] don't care
   wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string

   switch (wID)
   {
       case 0x7014 :
       case 0x7016 :  break;
       default: return(ModuleIdError);
   }

   if( wID==0x7014 )
   {
      szCmd[0]='$';    // $AAAV
      c2=(char)(wAddr&0x0f);
      wAddr=wAddr/16;
      c1=(char)(wAddr&0x0f);
      szCmd[1] = HexToAscii(c1);
      szCmd[2] = HexToAscii(c2);
      szCmd[3]='A';
      szCmd[4]='1';
      szCmd[5]=0;
   }
   else
   {
      szCmd[0]='@';    // @AAAV
      c2=(char)(wAddr&0x0f);
      wAddr=wAddr/16;
      c1=(char)(wAddr&0x0f);
      szCmd[1] = HexToAscii(c1);
      szCmd[2] = HexToAscii(c2);
      szCmd[3]='A';
      szCmd[4]='1';
      szCmd[5]=0;
   }

   if(wFlag==1)
   {
      strcpy(szSendTo7000,szCmd);
      szReceiveFrom7000[0]=0;
   }

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wRetVal != NoError)
   {
     err=SendCmdError;
     goto ret_err;
   }

   if(szRec[0] != '!')
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wCheckSum==0) wRecStrLen=3; else wRecStrLen=5;

   if( strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wFlag==1)
   {
      strcpy(szReceiveFrom7000,szRec);
   }

   return(NoError);

 ret_err:
   return(err);
}

/*-------- Disable Linear Mapping for 7014/7016 ---------*/
EXPORTS WORD CALLBACK DisableLinearMapping(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
   char   wPort,wFlag,c1,c2;
   WORD   wAddr,wRetVal,wID,wCheckSum,wTimeOut,err,wT;
   WORD   wRecStrLen;

   //---------------------- input -------------------------------------
   wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
   wAddr=w7000[1];         // module address: 0x00 to 0xFF
   wID=w7000[2];           // module ID: 0x7014, 0x7016
   wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];      // TimeOut constant: normal=100
   // w7000[5] don't care
   wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string

   switch (wID)
   {
       case 0x7014 :
       case 0x7016 :  break;
       default: return(ModuleIdError);
   }

   if( wID==0x7014 )
   {
      szCmd[0]='$';    // $AAAV
      c2=(char)(wAddr&0x0f);
      wAddr=wAddr/16;
      c1=(char)(wAddr&0x0f);
      szCmd[1] = HexToAscii(c1);
      szCmd[2] = HexToAscii(c2);
      szCmd[3]='A';
      szCmd[4]='0';
      szCmd[5]=0;
   }
   else
   {
      szCmd[0]='@';    // @AAAV
      c2=(char)(wAddr&0x0f);
      wAddr=wAddr/16;
      c1=(char)(wAddr&0x0f);
      szCmd[1] = HexToAscii(c1);
      szCmd[2] = HexToAscii(c2);
      szCmd[3]='A';
      szCmd[4]='0';
      szCmd[5]=0;
   }

   if(wFlag==1)
   {
      strcpy(szSendTo7000,szCmd);
      szReceiveFrom7000[0]=0;
   }

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wRetVal != NoError)
   {
     err=SendCmdError;
     goto ret_err;
   }

   if(szRec[0] != '!')
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wCheckSum==0) wRecStrLen=3; else wRecStrLen=5;

   if( strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wFlag==1)
   {
      strcpy(szReceiveFrom7000,szRec);
   }

   return(NoError);

 ret_err:
   return(err);
}

/*-------- reading the linear mapping status for 7014/7016 --------*/
EXPORTS WORD CALLBACK ReadLinearMappingStatus(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
   char   wPort,wFlag,c1,c2;
   WORD   wAddr,wRetVal,wID,wCheckSum,wTimeOut,err,wT;
   WORD   wRecStrLen;

   //---------------------- input -----------------------------------
   wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
   wAddr=w7000[1];         // module address: 0x00 to 0xFF
   wID=w7000[2];           // module ID: 0x7014, 0x7016
   wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];      // TimeOut constant: normal=100
   // wChannel=w7000[5];   // which LED channel to display
   wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string

   switch (wID)
   {
       case 0x7014 :  wID=0;  break;
       case 0x7016 :  wID=1;  break;
       default: return(ModuleIdError);
   }

   if( wID==0 )
   {
      szCmd[0]='$';    // 7014, $AAB
      c2=(char)(wAddr&0x0f);
      wAddr=wAddr/16;
      c1=(char)(wAddr&0x0f);
      szCmd[1] = HexToAscii(c1);
      szCmd[2] = HexToAscii(c2);
      szCmd[3]='B';
      szCmd[4]=0;
   }
   else     // 7016, @AAA
   {
      szCmd[0]='@';
      c2=(char)(wAddr&0x0f);
      wAddr=wAddr/16;
      c1=(char)(wAddr&0x0f);
      szCmd[1] = HexToAscii(c1);
      szCmd[2] = HexToAscii(c2);
      szCmd[3]='A';
      szCmd[4]=0;
   }

   if(wFlag==1)
   {
      strcpy(szSendTo7000,szCmd);
      szReceiveFrom7000[0]=0;
   }

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wRetVal != NoError)
   {
     err=SendCmdError;
     goto ret_err;
   }

   if(szRec[0] != '!')
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wCheckSum==0) wRecStrLen=4; else wRecStrLen=6;

   if( strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wFlag==1)
   {
      strcpy(szReceiveFrom7000,szRec);
   }

   w7000[5]=szRec[3]-'0';

   return(NoError);

 ret_err:
   return(err);
}

/*----- Reading Source Value of Linear Mapping for 7014/7016 -----*/
EXPORTS WORD CALLBACK ReadSourceValueOfLM(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
   char   wPort,wFlag,c1,c2,Buffer[10];
   WORD   wAddr,wRetVal,i,j,wID,wCheckSum,wTimeOut,err,wT;
   WORD   wRecStrLen;

   //---------------------- input -------------------------------------
   wPort=(char)w7000[0];  // RS-232 port number: 1/2/3/4
   wAddr=w7000[1];        // module address: 0x00 to 0xFF
   wID=w7000[2];          // module ID: 0x7014/16
   wCheckSum=w7000[3];    // checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];     // TimeOut constant: normal=100
   wFlag=(char)w7000[6];  // flag: 0=no save, 1=save send/receive string
   //---------------------- output ------------------------------------
   // f7000[0]= Low Source Value
   // f7000[1]= High Source Value

   switch (wID)
   {
      case 0x7014 :
      case 0x7016 : break;
      default: return(ModuleIdError);
   }

   if( wID==0x7014 )
   {
      szCmd[0]='$';           //$AA3
      c2=(char)(wAddr&0x0f);
      wAddr=wAddr/16;
      c1=(char)(wAddr&0x0f);
      szCmd[1] = HexToAscii(c1);
      szCmd[2] = HexToAscii(c2);
      szCmd[3] = '3';
      szCmd[4]=0;
   }
   else
   {
      szCmd[0]='@';           //@AA6
      c2=(char)(wAddr&0x0f);
      wAddr=wAddr/16;
      c1=(char)(wAddr&0x0f);
      szCmd[1] = HexToAscii(c1);
      szCmd[2] = HexToAscii(c2);
      szCmd[3] = '6';
      szCmd[4]=0;
   }

   if( wFlag==1 )
   {
      strcpy(szSendTo7000,szCmd);
      szReceiveFrom7000[0]=0;
   }

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wRetVal != NoError)
   {
      err=SendCmdError;
      goto ret_err;
   }

   if(szRec[0] != '!')
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wCheckSum==0) wRecStrLen=17; else wRecStrLen=19;

   if(strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   for(i=0; i<7; i++)
      Buffer[i]=szRec[i+3];
   Buffer[7]=0;
   f7000[0]=(float)atof(Buffer);

   for(i=0; i<7; i++)
      Buffer[i]=szRec[i+10];
   Buffer[7]=0;
   f7000[1]=(float)atof(Buffer);

   if(wFlag==1)
   {
      strcpy(szReceiveFrom7000,szRec);
   }
   return(NoError);

ret_err:
   for (j=0; j<7; j++) f7000[j]=(float)1.1;
   return(err);
}

/*----- Reading Target Value of Linear Mapping for 7014/7016 -----*/
EXPORTS WORD CALLBACK ReadTargetValueOfLM(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
   char   wPort,wFlag,Buffer[10],c1,c2;
   WORD   wAddr,wRetVal,i,j,wID,wCheckSum,wTimeOut,err,wT;
   WORD   wRecStrLen;

   //---------------------- input -------------------------------------
   wPort=(char)w7000[0];  // RS-232 port number: 1/2/3/4
   wAddr=w7000[1];        // module address: 0x00 to 0xFF
   wID=w7000[2];          // module ID: 0x7014/16
   wCheckSum=w7000[3];    // checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];     // TimeOut constant: normal=100
   wFlag=(char)w7000[6];  // flag: 0=no save, 1=save send/receive string
   //---------------------- output ------------------------------------
   // f7000[0]= Low Target Value
   // f7000[1]= High Target Value

   switch (wID)
   {
      case 0x7014 :
      case 0x7016 : break;
      default: return(ModuleIdError);
   }

   if( wID==0x7014 )
   {
      szCmd[0]='$';           //$AA5
      c2=(char)(wAddr&0x0f);
      wAddr=wAddr/16;
      c1=(char)(wAddr&0x0f);
      szCmd[1] = HexToAscii(c1);
      szCmd[2] = HexToAscii(c2);
      szCmd[3] = '5';
      szCmd[4]=0;
   }
   else
   {
      szCmd[0]='@';           //@AA7
      c2=(char)(wAddr&0x0f);
      wAddr=wAddr/16;
      c1=(char)(wAddr&0x0f);
      szCmd[1] = HexToAscii(c1);
      szCmd[2] = HexToAscii(c2);
      szCmd[3] = '7';
      szCmd[4]=0;
   }

   if( wFlag==1 )
   {
      strcpy(szSendTo7000,szCmd);
      szReceiveFrom7000[0]=0;
   }

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wRetVal != NoError)
   {
      err=SendCmdError;
      goto ret_err;
   }

   if(szRec[0] != '!')
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wCheckSum==0) wRecStrLen=17; else wRecStrLen=19;

   if(strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   for(i=0; i<7; i++)
      Buffer[i]=szRec[i+3];
   Buffer[7]=0;
   f7000[0]=(float)atof(Buffer);

   for(i=0; i<7; i++)
      Buffer[i]=szRec[i+10];
   Buffer[7]=0;
   f7000[1]=(float)atof(Buffer);

   if(wFlag==1)
   {
      strcpy(szReceiveFrom7000,szRec);
   }
   return(NoError);

ret_err:
   for (j=0; j<7; j++) f7000[j]=(float)0.0;
   return(err);
}


/*-------- Get active LED channel Display for 7033 --------*/
EXPORTS WORD CALLBACK GetLedDisplay_7033(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
   char   wPort,wFlag,c1,c2;
   WORD   wAddr,wRetVal,wID,wCheckSum,wTimeOut,err,wT;
   WORD   wRecStrLen;

   //---------------------- input -----------------------------------
   wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
   wAddr=w7000[1];         // module address: 0x00 to 0xFF
   wID=w7000[2];           // module ID: 0x7033
   wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];      // TimeOut constant: normal=100
   // wChannel=w7000[5];   // which LED channel to display
   wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string

   switch (wID)
   {
       case 0x7033 :  break;
       default: return(ModuleIdError);
   }

   szCmd[0]='$';    // $AA8
   c2=(char)(wAddr&0x0f);
   wAddr=wAddr/16;
   c1=(char)(wAddr&0x0f);
   szCmd[1] = HexToAscii(c1);
   szCmd[2] = HexToAscii(c2);
   szCmd[3]='8';
   szCmd[4]=0;

   if(wFlag==1)
   {
      strcpy(szSendTo7000,szCmd);
      szReceiveFrom7000[0]=0;
   }

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wRetVal != NoError)
   {
     err=SendCmdError;
     goto ret_err;
   }

   if(szRec[0] != '!')
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wCheckSum==0) wRecStrLen=4; else wRecStrLen=6;

   if( strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wFlag==1)
   {
      strcpy(szReceiveFrom7000,szRec);
   }

   w7000[5]=szRec[3]-'0';

   return(NoError);

 ret_err:
   return(err);
}

/*---- Read Alarm State and Digital Output State ---------*/
EXPORTS WORD CALLBACK ReadOutputAlarmState(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
   char wPort,wFlag,c1,c2;
   WORD wAddr,wRetVal,wID,wCheckSum,wTimeOut,err,wT;
   WORD wRecStrLen;

   //---------------------- input -------------------------------------
   wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
   wAddr=w7000[1];         // module address: 0x00 to 0xFF
   wID=w7000[2];           // module ID: 0x7080
   wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];      // TimeOut constant: normal=100
   //  wCounter=w7000[5];      // Don't Care
   wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string
   //---------------------- output ------------------------------------
   //               Alarm State:
   // w7000(7):0       Disable
   // w7000(7):1       Momentary
   // w7000(7):2       Latch
   //               Digital Output
   // w7000(8):0       Bit:1 Disable  Bit 0: Disable
   // w7000(8):1       Bit:1 Disable  Bit 0: Enable
   // w7000(8):2       Bit:1 Enable   Bit 0: Disable
   // w7000(8):3       Bit:1 Enable   Bit 0: Enable

   switch (wID)
   {
       case 0x7011 :
       case 0x7012 :
       case 0x7014 :
       case 0x7016 : break;
       default: return(ModuleIdError);
   }


   szCmd[0]='@';    // @AADI
   c2=(char)(wAddr&0x0f);
   wAddr=wAddr/16;
   c1=(char)(wAddr&0x0f);
   szCmd[1] = HexToAscii(c1);
   szCmd[2] = HexToAscii(c2);
   szCmd[3]='D';
   szCmd[4]='I';
   szCmd[5]=0;

   if(wFlag==1)
   {
      strcpy(szSendTo7000,szCmd);
      szReceiveFrom7000[0]=0;
   }

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wRetVal != NoError)
   {
     err=SendCmdError;
     goto ret_err;
   }

   if(szRec[0] != '!')
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wCheckSum==0) wRecStrLen=8; else wRecStrLen=10;

   if( strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wFlag==1)
   {
      strcpy(szReceiveFrom7000,szRec);
   }


   w7000[7]=szRec[3]-'0';

   if( wID==0x7016 )
      w7000[8]= AsciiToHex(szRec[5]);
   else
      w7000[8]=(szRec[4]-'0')*2+(szRec[5]-'0');

   return(NoError);

 ret_err:
   return(err);
}


/************************************************************************/
/* -------------- 7080 Counter In-------------------------------------- */
EXPORTS WORD CALLBACK CounterIn_7080(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
   char wPort,wFlag,c1,c2;
   WORD wAddr,wRetVal,wID,wCheckSum,wTimeOut,wCounter,err,wT;
   WORD A,B,C,D,E,F,G,H;
   WORD wRecStrLen;

   //---------------------- input -------------------------------------
   wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
   wAddr=w7000[1];         // module address: 0x00 to 0xFF
   wID=w7000[2];           // module ID: 0x7080
   wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];      // TimeOut constant: normal=100
   wCounter=w7000[5];      // 0: Counter 0,  1: Counter 1
   wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string
   //---------------------- output ------------------------------------
                           // w7000[7]= Hi-Word of Counter Value
                           // w7000[8]= Lo-Word of Counter Value

   if( wID!=0x7080 )
   {
      return(ModuleIdError);
   }
   if( wCounter>1 )
       return(InvalidateCounterNo);

   //***   #AAN  read Counter/Frequency Value
   szCmd[0]='#';
   c2=(char)(wAddr&0x0f);
   wAddr=wAddr/16;
   c1=(char)(wAddr&0x0f);
   szCmd[1] = HexToAscii(c1);
   szCmd[2] = HexToAscii(c2);
   szCmd[3]=wCounter+'0';
   szCmd[4]=0;

   if(wFlag==1)
   {
      strcpy(szSendTo7000,szCmd);
      szReceiveFrom7000[0]=0;
   }

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wRetVal != NoError)
   {
     err=SendCmdError;
     goto ret_err;
   }

   if(szRec[0] != '>')
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wCheckSum==0) wRecStrLen=9; else wRecStrLen=11;

   if( strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   A=AsciiToHex(szRec[1]);
   B=AsciiToHex(szRec[2]);
   C=AsciiToHex(szRec[3]);
   D=AsciiToHex(szRec[4]);
   E=AsciiToHex(szRec[5]);
   F=AsciiToHex(szRec[6]);
   G=AsciiToHex(szRec[7]);
   H=AsciiToHex(szRec[8]);

   w7000[7]=D+C*16+B*256+A*4096;
   w7000[8]=H+G*16+F*256+E*4096;

   if(wFlag==1)
   {
      strcpy(szReceiveFrom7000,szRec);
   }
   return(NoError);

 ret_err:
   return(err);
}

/* -------------- 7080 Read Max Counter Value ------------------------- */
EXPORTS WORD CALLBACK ReadCounterMaxValue_7080(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
   char wPort,wFlag,c1,c2;
   WORD wAddr,wRetVal,wID,wCheckSum,wTimeOut,wCounter,err,wT;
   WORD A,B,C,D,E,F,G,H;
   WORD wRecStrLen;

   //---------------------- input -------------------------------------
   wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
   wAddr=w7000[1];         // module address: 0x00 to 0xFF
   wID=w7000[2];           // module ID: 0x7080
   wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];      // TimeOut constant: normal=100
   wCounter=w7000[5];      // 0: Counter 0,  1: Counter 1
   wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string
   //---------------------- output ------------------------------------
                           // w7000[7]= Hi-Word of Max Counter Value
                           // w7000[8]= Lo-Word of Max Counter Value

   if( wID!=0x7080 )
   {
      return(ModuleIdError);
   }

   if( wCounter>1 )
       return(InvalidateCounterNo);

   //***   $AA3N  read MAX Value
   szCmd[0]='$';
   c2=(char)(wAddr&0x0f);
   wAddr=wAddr/16;
   c1=(char)(wAddr&0x0f);
   szCmd[1] = HexToAscii(c1);
   szCmd[2] = HexToAscii(c2);
   szCmd[3]='3';
   szCmd[4]=wCounter+'0';
   szCmd[5]=0;

   if(wFlag==1)
   {
      strcpy(szSendTo7000,szCmd);
      szReceiveFrom7000[0]=0;
   }

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wRetVal != NoError)
   {
     err=SendCmdError;
     goto ret_err;
   }

   if(szRec[0] != '!')
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wCheckSum==0) wRecStrLen=11; else wRecStrLen=13;

   if( strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   A=AsciiToHex(szRec[3]);
   B=AsciiToHex(szRec[4]);
   C=AsciiToHex(szRec[5]);
   D=AsciiToHex(szRec[6]);
   E=AsciiToHex(szRec[7]);
   F=AsciiToHex(szRec[8]);
   G=AsciiToHex(szRec[9]);
   H=AsciiToHex(szRec[10]);

   w7000[7]=D+C*16+B*256+A*4096;
   w7000[8]=H+G*16+F*256+E*4096;

   if(wFlag==1)
   {
      strcpy(szReceiveFrom7000,szRec);
   }
   return(NoError);

 ret_err:
   return(err);
}

/* -------------- 7080 Set Max Counter Value ------------------------- */
EXPORTS WORD CALLBACK SetCounterMaxValue_7080(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[], double MaxValue)
{
   char    wPort,wFlag,c1,c2;
   WORD    wAddr,wRetVal,wID,wCheckSum,wTimeOut,wCounter,err,wT;
   WORD    wRecStrLen,i;
   DWORD   dwMaxValue;
   char    Buffer[10];

   //---------------------- input -------------------------------------
   wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
   wAddr=w7000[1];         // module address: 0x00 to 0xFF
   wID=w7000[2];           // module ID: 0x7080
   wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];      // TimeOut constant: normal=100
   wCounter=w7000[5];      // 0: Counter 0,  1: Counter 1
   wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string


   if( wID!=0x7080 )
   {
      return(ModuleIdError);
   }

   if( wCounter>1 )
       return(InvalidateCounterNo);

   if( MaxValue>4294967295.1 || MaxValue<0.0 )
      return( InvalidateCounterValue );

   dwMaxValue=(unsigned long)MaxValue;
   sprintf(Buffer,"%08x",dwMaxValue);
   for(i=0; i<8; i++)
      if( Buffer[i]>='a' && Buffer[i]<='f' )
      {
          Buffer[i]=Buffer[i]-'a'+'A';
      }

   //***   $AA3N(Data)  Set MAX Value
   szCmd[0]='$';
   c2=(char)(wAddr&0x0f);
   wAddr=wAddr/16;
   c1=(char)(wAddr&0x0f);
   szCmd[1] = HexToAscii(c1);
   szCmd[2] = HexToAscii(c2);
   szCmd[3]='3';
   szCmd[4]=wCounter+'0';
   szCmd[5]=Buffer[0];
   szCmd[6]=Buffer[1];
   szCmd[7]=Buffer[2];
   szCmd[8]=Buffer[3];
   szCmd[9]=Buffer[4];
   szCmd[10]=Buffer[5];
   szCmd[11]=Buffer[6];
   szCmd[12]=Buffer[7];
   szCmd[13]=0;

   if(wFlag==1)
   {
      strcpy(szSendTo7000,szCmd);
      szReceiveFrom7000[0]=0;
   }

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wRetVal != NoError)
   {
     err=100+wRetVal;
     //err=SendCmdError;
     goto ret_err;
   }

   if(szRec[0] != '!')
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wCheckSum==0) wRecStrLen=3; else wRecStrLen=5;

   if( strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError;
      goto ret_err;
   }


   if(wFlag==1)
   {
      strcpy(szReceiveFrom7000,szRec);
   }
   return(NoError);

 ret_err:
   return(err);
}

/* ------------ 7080 Read Alarm limit value of counter N----------------- */
EXPORTS WORD CALLBACK ReadAlarmLimitValue_7080(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
   char wPort,wFlag,c1,c2;
   WORD wAddr,wRetVal,wID,wCheckSum,wTimeOut,wCounter,err,wT;
   WORD A,B,C,D,E,F,G,H;
   WORD wRecStrLen;

   //---------------------- input -------------------------------------
   wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
   wAddr=w7000[1];         // module address: 0x00 to 0xFF
   wID=w7000[2];           // module ID: 0x7080
   wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];      // TimeOut constant: normal=100
   wCounter=w7000[5];      // 0: Counter 0,  1: Counter 1
   wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string
   //---------------------- output ------------------------------------
                        // w7000[7]= Hi-Word of Max Counter Value
                        // w7000[8]= Lo-Word of Max Counter Value

   if( wID!=0x7080 )
   {
       return(ModuleIdError);
   }

   if( wCounter>1 )
       return(InvalidateCounterNo);

   //***   $AARP: read limit Value of counter 0
   //***   $AARA: read limit Value of counter 1
   szCmd[0]='@';
   c2=(char)(wAddr&0x0f);
   wAddr=wAddr/16;
   c1=(char)(wAddr&0x0f);
   szCmd[1] = HexToAscii(c1);
   szCmd[2] = HexToAscii(c2);
   szCmd[3]='R';
   if( wCounter==0 )    // Counter 0
      szCmd[4]='P';
   else
      szCmd[4]='A';     // Counter 1
   szCmd[5]=0;

   if(wFlag==1)
   {
      strcpy(szSendTo7000,szCmd);
      szReceiveFrom7000[0]=0;
   }

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wRetVal != NoError)
   {
     err=SendCmdError;
     goto ret_err;
   }

   if(szRec[0] != '!')
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wCheckSum==0) wRecStrLen=11; else wRecStrLen=13;

   if( strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   A=AsciiToHex(szRec[3]);
   B=AsciiToHex(szRec[4]);
   C=AsciiToHex(szRec[5]);
   D=AsciiToHex(szRec[6]);
   E=AsciiToHex(szRec[7]);
   F=AsciiToHex(szRec[8]);
   G=AsciiToHex(szRec[9]);
   H=AsciiToHex(szRec[10]);

   w7000[7]=D+C*16+B*256+A*4096;
   w7000[8]=H+G*16+F*256+E*4096;

   if(wFlag==1)
   {
      strcpy(szReceiveFrom7000,szRec);
   }
   return(NoError);

 ret_err:
   return(err);
}


/* ------------ 7080 Set Counter 0/1 Alarm Counter Value ---------------- */
EXPORTS WORD CALLBACK SetAlarmLimitValue_7080(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[], double AlarmValue)
{
   char    wPort,wFlag,c1,c2;
   WORD    wAddr,wRetVal,wID,wCheckSum,wTimeOut,wCounter,err,wT;
   WORD    wRecStrLen,i;
   DWORD   dwMaxValue;
   char    Buffer[10];

   //---------------------- input -------------------------------------
   wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
   wAddr=w7000[1];         // module address: 0x00 to 0xFF
   wID=w7000[2];           // module ID: 0x7080
   wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];      // TimeOut constant: normal=100
   wCounter=w7000[5];      // 0: Counter 0,   1: Counter 1
   wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string


   if( wID!=0x7080 )
   {
      return(ModuleIdError);
   }

   if( wCounter>1 )
       return(InvalidateCounterNo);

   if( AlarmValue>4294967295.1 || AlarmValue<0.0 )
      return( InvalidateCounterValue );

   dwMaxValue=(unsigned long)AlarmValue;
   sprintf(Buffer,"%08x",dwMaxValue);
   for(i=0; i<8; i++)
      if( Buffer[i]>='a' && Buffer[i]<='f' )
      {
          Buffer[i]=Buffer[i]-'a'+'A';
      }

   //***   @AAPA(Data) counter 0,  @AASA(DATA) counter 1
   szCmd[0]='@';
   c2=(char)(wAddr&0x0f);
   wAddr=wAddr/16;
   c1=(char)(wAddr&0x0f);
   szCmd[1] = HexToAscii(c1);
   szCmd[2] = HexToAscii(c2);
   if( w7000[5]==0 )
      szCmd[3]='P';
   else
      szCmd[3]='S';
   szCmd[4]='A';
   szCmd[5]=Buffer[0];
   szCmd[6]=Buffer[1];
   szCmd[7]=Buffer[2];
   szCmd[8]=Buffer[3];
   szCmd[9]=Buffer[4];
   szCmd[10]=Buffer[5];
   szCmd[11]=Buffer[6];
   szCmd[12]=Buffer[7];
   szCmd[13]=0;

   if(wFlag==1)
   {
      strcpy(szSendTo7000,szCmd);
      szReceiveFrom7000[0]=0;
   }

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wRetVal != NoError)
   {
     //err=100+wRetVal;
     err=SendCmdError;
     goto ret_err;
   }

   if(szRec[0] != '!')
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wCheckSum==0) wRecStrLen=3; else wRecStrLen=5;

   if( strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError;
      goto ret_err;
   }


   if(wFlag==1)
   {
      strcpy(szReceiveFrom7000,szRec);
   }
   return(NoError);

 ret_err:
   return(err);
}


/*--------------- 7080D  Enable Counter Alarm ----------------------*/
EXPORTS WORD CALLBACK EnableCounterAlarm_7080D(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
   char wPort,wFlag,c1,c2;
   WORD wAddr,wRetVal,wID,wCheckSum,wTimeOut,wCounter,err,wT;
   WORD wRecStrLen;

   //---------------------- input -------------------------------------
   wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
   wAddr=w7000[1];         // module address: 0x00 to 0xFF
   wID=w7000[2];           // module ID: 0x7080
   wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];      // TimeOut constant: normal=100
   wCounter=w7000[5];      // 0: Momentary,  1: Latch
   wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string
   //---------------------- output ------------------------------------

   if( wID!=0x7080 )
   {
      return(ModuleIdError);
   }

   if( wCounter>1 )
       return(InvalidateCounterNo);

   //***   @AAEAN  Enable Counter Alarm
   szCmd[0]='@';
   c2=(char)(wAddr&0x0f);
   wAddr=wAddr/16;
   c1=(char)(wAddr&0x0f);
   szCmd[1] = HexToAscii(c1);
   szCmd[2] = HexToAscii(c2);
   szCmd[3]='E';
   szCmd[4]='A';
   if( wCounter==0 )   // Momentary Alarm State
      szCmd[5]='M';
   else
      szCmd[5]='L';    // Latch Alarm State
   szCmd[6]=0;

   if(wFlag==1)
   {
      strcpy(szSendTo7000,szCmd);
      szReceiveFrom7000[0]=0;
   }

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wRetVal != NoError)
   {
     err=SendCmdError;
     goto ret_err;
   }

   if(szRec[0] != '!')
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wCheckSum==0) wRecStrLen=3; else wRecStrLen=5;

   if( strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wFlag==1)
   {
      strcpy(szReceiveFrom7000,szRec);
   }
   return(NoError);

 ret_err:
   return(err);
}


/*--------------- 7080D  Disable Counter Alarm ----------------------*/
EXPORTS WORD CALLBACK DisableCounterAlarm_7080D(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
   char wPort,wFlag,c1,c2;
   WORD wAddr,wRetVal,wID,wCheckSum,wTimeOut,wCounter,err,wT;
   WORD wRecStrLen;

   //---------------------- input -------------------------------------
   wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
   wAddr=w7000[1];         // module address: 0x00 to 0xFF
   wID=w7000[2];           // module ID: 0x7080
   wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];      // TimeOut constant: normal=100
   wCounter=w7000[5];      // Don't care
   wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string
   //---------------------- output ------------------------------------

   if( wID!=0x7080 )
   {
      return(ModuleIdError);
   }


   //***   @AADA  Disable Counter Alarm
   szCmd[0]='@';
   c2=(char)(wAddr&0x0f);
   wAddr=wAddr/16;
   c1=(char)(wAddr&0x0f);
   szCmd[1] = HexToAscii(c1);
   szCmd[2] = HexToAscii(c2);
   szCmd[3]='D';
   szCmd[4]='A';
   szCmd[5]=0;

   if(wFlag==1)
   {
      strcpy(szSendTo7000,szCmd);
      szReceiveFrom7000[0]=0;
   }

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wRetVal != NoError)
   {
     err=SendCmdError;
     goto ret_err;
   }

   if(szRec[0] != '!')
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wCheckSum==0) wRecStrLen=3; else wRecStrLen=5;

   if( strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wFlag==1)
   {
      strcpy(szReceiveFrom7000,szRec);
   }
   return(NoError);

 ret_err:
   return(err);
}


/*--------------- 7080 Enable Counter Alarm ----------------------*/
EXPORTS WORD CALLBACK EnableCounterAlarm_7080(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
   char wPort,wFlag,c1,c2;
   WORD wAddr,wRetVal,wID,wCheckSum,wTimeOut,wCounter,err,wT;
   //WORD A;
   WORD wRecStrLen;

   //---------------------- input -------------------------------------
   wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
   wAddr=w7000[1];         // module address: 0x00 to 0xFF
   wID=w7000[2];           // module ID: 0x7080
   wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];      // TimeOut constant: normal=100
   wCounter=w7000[5];      // 0: Counter 0,  1: Counter 1
   wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string
   //---------------------- output ------------------------------------

   if( wID!=0x7080 )
   {
      return(ModuleIdError);
   }

   if( wCounter>1 )
       return(InvalidateCounterNo);

   //***   @AAEAN  Enable Counter Alarm
   szCmd[0]='@';
   c2=(char)(wAddr&0x0f);
   wAddr=wAddr/16;
   c1=(char)(wAddr&0x0f);
   szCmd[1] = HexToAscii(c1);
   szCmd[2] = HexToAscii(c2);
   szCmd[3]='E';
   szCmd[4]='A';
   szCmd[5]=wCounter+'0';
   szCmd[6]=0;

   if(wFlag==1)
   {
      strcpy(szSendTo7000,szCmd);
      szReceiveFrom7000[0]=0;
   }

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wRetVal != NoError)
   {
     err=SendCmdError;
     goto ret_err;
   }

   if(szRec[0] != '!')
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wCheckSum==0) wRecStrLen=3; else wRecStrLen=5;

   if( strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wFlag==1)
   {
      strcpy(szReceiveFrom7000,szRec);
   }
   return(NoError);

 ret_err:
   return(err);
}


/*--------------- 7080 Disable Counter Alarm ----------------------*/
EXPORTS WORD CALLBACK DisableCounterAlarm_7080(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
   char wPort,wFlag,c1,c2;
   WORD wAddr,wRetVal,wID,wCheckSum,wTimeOut,wCounter,err,wT;
   //WORD A;
   WORD wRecStrLen;

   //---------------------- input -------------------------------------
   wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
   wAddr=w7000[1];         // module address: 0x00 to 0xFF
   wID=w7000[2];           // module ID: 0x7080
   wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];      // TimeOut constant: normal=100
   wCounter=w7000[5];      // 0: Counter 0,  1: Counter 1
   wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string
   //---------------------- output ------------------------------------

   if( wID!=0x7080 )
   {
      return(ModuleIdError);
   }

   if( wCounter>1 )
       return(InvalidateCounterNo);

   //***   @AADAN  Enable Counter Alarm
   szCmd[0]='@';
   c2=(char)(wAddr&0x0f);
   wAddr=wAddr/16;
   c1=(char)(wAddr&0x0f);
   szCmd[1] = HexToAscii(c1);
   szCmd[2] = HexToAscii(c2);
   szCmd[3]='D';
   szCmd[4]='A';
   szCmd[5]=wCounter+'0';
   szCmd[6]=0;

   if(wFlag==1)
   {
      strcpy(szSendTo7000,szCmd);
      szReceiveFrom7000[0]=0;
   }

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wRetVal != NoError)
   {
     err=SendCmdError;
     goto ret_err;
   }

   if(szRec[0] != '!')
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wCheckSum==0) wRecStrLen=3; else wRecStrLen=5;

   if( strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wFlag==1)
   {
      strcpy(szReceiveFrom7000,szRec);
   }
   return(NoError);

 ret_err:
   return(err);
}




/* -------------- 7080 Read Counter Status ----------------------- */
EXPORTS WORD CALLBACK ReadCounterStatus_7080(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
   char wPort,wFlag,c1,c2;
   WORD wAddr,wRetVal,wID,wCheckSum,wTimeOut,wCounter,err,wT;
   WORD A;
   WORD wRecStrLen;

   //---------------------- input -------------------------------------
   wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
   wAddr=w7000[1];         // module address: 0x00 to 0xFF
   wID=w7000[2];           // module ID: 0x7080
   wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];      // TimeOut constant: normal=100
   wCounter=w7000[5];      // 0: Counter 0,  1: Counter 1
   wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string
   //---------------------- output ------------------------------------
                           // w7000[7]=0: Counting  1: not counting

   if( wID!=0x7080 )
   {
      return(ModuleIdError);
   }

   if( wCounter>1 )
       return(InvalidateCounterNo);

   //***   $AA5N  read Counter Status
   szCmd[0]='$';
   c2=(char)(wAddr&0x0f);
   wAddr=wAddr/16;
   c1=(char)(wAddr&0x0f);
   szCmd[1] = HexToAscii(c1);
   szCmd[2] = HexToAscii(c2);
   szCmd[3]='5';
   szCmd[4]=wCounter+'0';
   szCmd[5]=0;

   if(wFlag==1)
   {
      strcpy(szSendTo7000,szCmd);
      szReceiveFrom7000[0]=0;
   }

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wRetVal != NoError)
   {
     err=SendCmdError;
     goto ret_err;
   }

   if(szRec[0] != '!')
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wCheckSum==0) wRecStrLen=4; else wRecStrLen=6;

   if( strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   A=AsciiToHex(szRec[3]);

   w7000[7]=A;

   if(wFlag==1)
   {
      strcpy(szReceiveFrom7000,szRec);
   }
   return(NoError);

 ret_err:
   return(err);
}


/* -------------- 7080 Clear Counter ----------------------- */
EXPORTS WORD CALLBACK ClearCounter_7080(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
   char wPort,wFlag,c1,c2;
   WORD wAddr,wRetVal,wID,wCheckSum,wTimeOut,wCounter,err,wT;
   //WORD A;
   WORD wRecStrLen;

   //---------------------- input -------------------------------------
   wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
   wAddr=w7000[1];         // module address: 0x00 to 0xFF
   wID=w7000[2];           // module ID: 0x7080
   wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];      // TimeOut constant: normal=100
   wCounter=w7000[5];      // 0: Counter 0,  1: Counter 1
   wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string
   //---------------------- output ------------------------------------

   if( wID!=0x7080 )
   {
      return(ModuleIdError);
   }

   if( wCounter>1 )
       return(InvalidateCounterNo);

   //***   $AA6N  Clear Counter
   szCmd[0]='$';
   c2=(char)(wAddr&0x0f);
   wAddr=wAddr/16;
   c1=(char)(wAddr&0x0f);
   szCmd[1] = HexToAscii(c1);
   szCmd[2] = HexToAscii(c2);
   szCmd[3]='6';
   szCmd[4]=wCounter+'0';
   szCmd[5]=0;

   if(wFlag==1)
   {
      strcpy(szSendTo7000,szCmd);
      szReceiveFrom7000[0]=0;
   }

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wRetVal != NoError)
   {
     err=SendCmdError;
     goto ret_err;
   }

   if(szRec[0] != '!')
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wCheckSum==0) wRecStrLen=3; else wRecStrLen=5;

   if( strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wFlag==1)
   {
      strcpy(szReceiveFrom7000,szRec);
   }
   return(NoError);

 ret_err:
   return(err);
}

/*---- 7080 Read Counter 0/1 Alarm State and Digital Output State ---------*/
EXPORTS WORD CALLBACK ReadOutputAlarmState_7080(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
   char wPort,wFlag,c1,c2;
   WORD wAddr,wRetVal,wID,wCheckSum,wTimeOut,wCounter,err,wT;
   WORD A,B;
   WORD wRecStrLen;

   //---------------------- input -------------------------------------
   wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
   wAddr=w7000[1];         // module address: 0x00 to 0xFF
   wID=w7000[2];           // module ID: 0x7080
   wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];      // TimeOut constant: normal=100
   wCounter=w7000[5];      // Don't Care
   wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string
   //---------------------- output ------------------------------------
   // w7000(7):0  for 7080 ->  Counter0:Disable   Counter1:Disable
   // w7000(7):1               Counter0:Disable   Counter1:Enable
   // w7000(7):2               Counter0:Enable    Counter1:Disable
   // w7000(7):3               Counter0:Enable    Counter1:Enable
   // w7000(7):0  for 7080D ->  Counter0:Disable
   // w7000(7):1                Counter0:Momentary
   // w7000(7):2                Counter0:Latch

   // w7000(8):0    Bit:1 Disable  Bit 0: Disable
   // w7000(8):1    Bit:1 Disable  Bit 0: Enable
   // w7000(8):2    Bit:1 Enable   Bit 0: Disable
   // w7000(8):3    Bit:1 Enable   Bit 0: Enable

   if( wID!=0x7080 )
   {
      return(ModuleIdError);
   }

   //***   @AADI  read counter's alarm state
   szCmd[0]='@';
   c2=(char)(wAddr&0x0f);
   wAddr=wAddr/16;
   c1=(char)(wAddr&0x0f);
   szCmd[1] = HexToAscii(c1);
   szCmd[2] = HexToAscii(c2);
   szCmd[3]='D';
   szCmd[4]='I';
   szCmd[5]=0;

   if(wFlag==1)
   {
      strcpy(szSendTo7000,szCmd);
      szReceiveFrom7000[0]=0;
   }

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wRetVal != NoError)
   {
     err=SendCmdError;
     goto ret_err;
   }

   if(szRec[0] != '!')
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wCheckSum==0) wRecStrLen=8; else wRecStrLen=10;

   if( strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError;
      goto ret_err;
   }


   A=AsciiToHex(szRec[3]);
   B=(szRec[4]-'0')*2+(szRec[5]-'0');

   w7000[7]=A;
   w7000[8]=B;

   if(wFlag==1)
   {
      strcpy(szReceiveFrom7000,szRec);
   }
   return(NoError);

 ret_err:
   return(err);
}

/*-------- 7080 Set Counter's Input Signal Mode  ---------*/
EXPORTS WORD CALLBACK SetInputSignalMode_7080(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
   char wPort,wFlag,c1,c2;
   WORD wAddr,wRetVal,wID,wCheckSum,wTimeOut,err,wT,wMode;
   //WORD A;
   WORD wRecStrLen;

   //---------------------- input -------------------------------------
   wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
   wAddr=w7000[1];         // module address: 0x00 to 0xFF
   wID=w7000[2];           // module ID: 0x7080
   wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];      // TimeOut constant: normal=100
   wMode=w7000[5];         // Counter 0's and Counter 1's signal mode
   wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string
   //---------------------- output ------------------------------------
                     // w7000(5):0    Counter:0 TTL     Counter:0 TTL
                     // w7000(5):1    Counter:1 PHOTO   Counter:1 PHOTO
                     // w7000(5):2    Counter:0 TTL     Counter:1 PHOTO
                     // w7000(5):3    Counter:1 PHOTO   Counter:0 TTL

   if( wID!=0x7080 )
   {
      return(ModuleIdError);
   }

   //***   $AAB  read counter's input signal state
   szCmd[0]='$';
   c2=(char)(wAddr&0x0f);
   wAddr=wAddr/16;
   c1=(char)(wAddr&0x0f);
   szCmd[1] = HexToAscii(c1);
   szCmd[2] = HexToAscii(c2);
   szCmd[3]='B';
   szCmd[4]=wMode+'0';
   szCmd[5]=0;

   if(wFlag==1)
   {
      strcpy(szSendTo7000,szCmd);
      szReceiveFrom7000[0]=0;
   }

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wRetVal != NoError)
   {
     err=SendCmdError;
     goto ret_err;
   }

   if(szRec[0] != '!')
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wCheckSum==0) wRecStrLen=3; else wRecStrLen=5;

   if( strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError;
      goto ret_err;
   }


   if(wFlag==1)
   {
      strcpy(szReceiveFrom7000,szRec);
   }
   return(NoError);

 ret_err:
   return(err);
}



/*-------- 7080 Read Counter's Input Signal Mode  ---------*/
EXPORTS WORD CALLBACK ReadInputSignalMode_7080(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
   char wPort,wFlag,c1,c2;
   WORD wAddr,wRetVal,wID,wCheckSum,wTimeOut,wCounter,err,wT;
   WORD A;
   WORD wRecStrLen;

   //---------------------- input -------------------------------------
   wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
   wAddr=w7000[1];         // module address: 0x00 to 0xFF
   wID=w7000[2];           // module ID: 0x7080
   wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];      // TimeOut constant: normal=100
   wCounter=w7000[5];      // Don't Care
   wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string
   //---------------------- output ------------------------------------
                     // w7000(7):0    Counter:1 TTL     Counter:0 TTL
                     // w7000(7):1    Counter:1 PHOTO   Counter:0 PHOTO
                     // w7000(7):2    Counter:1         Counter:0
                     // w7000(7):3    Counter:1         Counter:0

   if( wID!=0x7080 )
   {
      return(ModuleIdError);
   }

   //***   $AAB  read counter's input signal state
   szCmd[0]='$';
   c2=(char)(wAddr&0x0f);
   wAddr=wAddr/16;
   c1=(char)(wAddr&0x0f);
   szCmd[1] = HexToAscii(c1);
   szCmd[2] = HexToAscii(c2);
   szCmd[3]='B';
   szCmd[4]=0;

   if(wFlag==1)
   {
      strcpy(szSendTo7000,szCmd);
      szReceiveFrom7000[0]=0;
   }

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wRetVal != NoError)
   {
     err=SendCmdError;
     goto ret_err;
   }

   if(szRec[0] != '!')
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wCheckSum==0) wRecStrLen=4; else wRecStrLen=6;

   if( strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   A=AsciiToHex(szRec[3]);

   w7000[7]=A;

   if(wFlag==1)
   {
      strcpy(szReceiveFrom7000,szRec);
   }
   return(NoError);

 ret_err:
   return(err);
}


/* ------------ 7080 Preset Counter 0/1 Counter Value ---------------- */
EXPORTS WORD CALLBACK PresetCounterValue_7080(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[], double PresetValue)
{
   char    wPort,wFlag,c1,c2;
   WORD    wAddr,wRetVal,wID,wCheckSum,wTimeOut,wCounter,err,wT;
   WORD    wRecStrLen,i;
   DWORD   dwPresetValue;
   char    Buffer[10];

   //---------------------- input -------------------------------------
   wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
   wAddr=w7000[1];         // module address: 0x00 to 0xFF
   wID=w7000[2];           // module ID: 0x7080
   wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];      // TimeOut constant: normal=100
   wCounter=w7000[5];      // 0: Counter 0,   1: Counter 1
   wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string

   if( wID!=0x7080 )
   {
      return(ModuleIdError);
   }

   if( wCounter>1 )
       return(InvalidateCounterNo);

   if( PresetValue>4294967295.1 || PresetValue<0.0 )
      return( InvalidateCounterValue );

   dwPresetValue=(unsigned long)PresetValue;
   sprintf(Buffer,"%08x",dwPresetValue);
   for(i=0; i<8; i++)
      if( Buffer[i]>='a' && Buffer[i]<='f' )
      {
          Buffer[i]=Buffer[i]-'a'+'A';
      }

   //***   @AAPN(Data) N:0 counter 0,  N:1 counter 1
   szCmd[0]='@';
   c2=(char)(wAddr&0x0f);
   wAddr=wAddr/16;
   c1=(char)(wAddr&0x0f);
   szCmd[1] = HexToAscii(c1);
   szCmd[2] = HexToAscii(c2);
   szCmd[3]='P';
   szCmd[4]=wCounter+'0';
   szCmd[5]=Buffer[0];
   szCmd[6]=Buffer[1];
   szCmd[7]=Buffer[2];
   szCmd[8]=Buffer[3];
   szCmd[9]=Buffer[4];
   szCmd[10]=Buffer[5];
   szCmd[11]=Buffer[6];
   szCmd[12]=Buffer[7];
   szCmd[13]=0;

   if(wFlag==1)
   {
      strcpy(szSendTo7000,szCmd);
      szReceiveFrom7000[0]=0;
   }

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wRetVal != NoError)
   {
     //err=100+wRetVal;
     err=SendCmdError;
     goto ret_err;
   }

   if(szRec[0] != '!')
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wCheckSum==0) wRecStrLen=3; else wRecStrLen=5;

   if( strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError;
      goto ret_err;
   }


   if(wFlag==1)
   {
      strcpy(szReceiveFrom7000,szRec);
   }
   return(NoError);

 ret_err:
   return(err);
}


/* ------------ 7080 Read Preset Counter 0/1 Counter Value ------------- */
EXPORTS WORD CALLBACK ReadPresetCounterValue_7080(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
   char wPort,wFlag,c1,c2;
   WORD wAddr,wRetVal,wID,wCheckSum,wTimeOut,wCounter,err,wT;
   WORD A,B,C,D,E,F,G,H;
   WORD wRecStrLen;

   //---------------------- input -------------------------------------
   wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
   wAddr=w7000[1];         // module address: 0x00 to 0xFF
   wID=w7000[2];           // module ID: 0x7080
   wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];      // TimeOut constant: normal=100
   wCounter=w7000[5];      // 0: Counter 0,  1: Counter 1
   wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string
   //---------------------- output ------------------------------------
                      // w7000[7]= Hi-Word of Preset Counter Value
                      // w7000[8]= Lo-Word of Preset Counter Value

   if( wID!=0x7080 )
   {
       return(ModuleIdError);
   }

   if( wCounter>1 )
       return(InvalidateCounterNo);

   //***   @AAGN: N:0 Counter 0,  1: Counter 1
   szCmd[0]='@';
   c2=(char)(wAddr&0x0f);
   wAddr=wAddr/16;
   c1=(char)(wAddr&0x0f);
   szCmd[1] = HexToAscii(c1);
   szCmd[2] = HexToAscii(c2);
   szCmd[3]='G';
   szCmd[4]=wCounter+'0';     // Counter
   szCmd[5]=0;

   if(wFlag==1)
   {
      strcpy(szSendTo7000,szCmd);
      szReceiveFrom7000[0]=0;
   }

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wRetVal != NoError)
   {
     err=SendCmdError;
     goto ret_err;
   }

   if(szRec[0] != '!')
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wCheckSum==0) wRecStrLen=11; else wRecStrLen=13;

   if( strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   A=AsciiToHex(szRec[3]);
   B=AsciiToHex(szRec[4]);
   C=AsciiToHex(szRec[5]);
   D=AsciiToHex(szRec[6]);
   E=AsciiToHex(szRec[7]);
   F=AsciiToHex(szRec[8]);
   G=AsciiToHex(szRec[9]);
   H=AsciiToHex(szRec[10]);

   w7000[7]=D+C*16+B*256+A*4096;
   w7000[8]=H+G*16+F*256+E*4096;

   if(wFlag==1)
   {
      strcpy(szReceiveFrom7000,szRec);
   }
   return(NoError);

 ret_err:
   return(err);
}


/*-------- 7080 Start Counting ---------*/
EXPORTS WORD CALLBACK StartCounting_7080(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
   char wPort,wFlag,c1,c2;
   WORD wAddr,wRetVal,wID,wCheckSum,wTimeOut,wCounter,err,wT;
   WORD wStart;
   WORD wRecStrLen;

   //---------------------- input -------------------------------------
   wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
   wAddr=w7000[1];         // module address: 0x00 to 0xFF
   wID=w7000[2];           // module ID: 0x7080
   wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];      // TimeOut constant: normal=100
   wCounter=w7000[5];      // 0: Counter 0   1: Counter 1
   wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string
   wStart=w7000[7];        // 0: Stop Counting  1: Start Counting

   if( wID!=0x7080 )
   {
      return(ModuleIdError);
   }

   //***   $AA5NS     Start/Stop Counting
   szCmd[0]='$';
   c2=(char)(wAddr&0x0f);
   wAddr=wAddr/16;
   c1=(char)(wAddr&0x0f);
   szCmd[1] = HexToAscii(c1);
   szCmd[2] = HexToAscii(c2);
   szCmd[3]='5';
   szCmd[4]=wCounter+'0';
   szCmd[5]=wStart+'0';
   szCmd[6]=0;

   if(wFlag==1)
   {
      strcpy(szSendTo7000,szCmd);
      szReceiveFrom7000[0]=0;
   }

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wRetVal != NoError)
   {
     err=SendCmdError;
     goto ret_err;
   }

   if(szRec[0] != '!')
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wCheckSum==0)  wRecStrLen=3;      else  wRecStrLen=5;

   if( strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError;
      goto ret_err;
   }


   if(wFlag==1)
   {
      strcpy(szReceiveFrom7000,szRec);
   }
   return(NoError);

 ret_err:
   return(err);
}


/*-------- Set 7080 module mode  ---------*/
EXPORTS WORD CALLBACK SetModuleMode_7080(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
   char wPort,wFlag,c1,c2;
   WORD wAddr,wRetVal,wID,wCheckSum,wTimeOut,wMode,err,wT;
   //WORD wStart;
   WORD wRecStrLen;

   //---------------------- input -------------------------------------
   wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
   wAddr=w7000[1];         // module address: 0x00 to 0xFF
   wID=w7000[2];           // module ID: 0x7080
   wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];      // TimeOut constant: normal=100
   wMode=w7000[5];         // 0: 7080 mode   1: 7080D mode
   wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string

   if( wID!=0x7080 )
   {
      return(ModuleIdError);
   }

   //***   ~AAAM   set module mode
   szCmd[0]='~';
   c2=(char)(wAddr&0x0f);
   wAddr=wAddr/16;
   c1=(char)(wAddr&0x0f);
   szCmd[1] = HexToAscii(c1);
   szCmd[2] = HexToAscii(c2);
   szCmd[3]='A';
   szCmd[4]=wMode+'0';
   szCmd[5]=0;

   if(wFlag==1)
   {
      strcpy(szSendTo7000,szCmd);
      szReceiveFrom7000[0]=0;
   }

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wRetVal != NoError)
   {
     err=SendCmdError;
     goto ret_err;
   }

   if(szRec[0] != '!')
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wCheckSum==0)  wRecStrLen=3;   else  wRecStrLen=5;

   if( strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError;
      goto ret_err;
   }


   if(wFlag==1)
   {
      strcpy(szReceiveFrom7000,szRec);
   }
   return(NoError);

 ret_err:
   return(err);
}


/*-------- Read 7080 module mode  ---------*/
EXPORTS WORD CALLBACK ReadModuleMode_7080(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
   char wPort,wFlag,c1,c2;
   WORD wAddr,wRetVal,wID,wCheckSum,wTimeOut,wMode,err,wT;
   //WORD wStart;
   WORD wRecStrLen;

   //---------------------- input -------------------------------------
   wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
   wAddr=w7000[1];         // module address: 0x00 to 0xFF
   wID=w7000[2];           // module ID: 0x7080
   wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];      // TimeOut constant: normal=100
   wMode=w7000[5];         // Don't care
   wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string
               //------------- Output -------------------
               //   w7000[5]=   0: 7080,    1: 7080D

   if( wID!=0x7080 )
   {
      return(ModuleIdError);
   }

   //***   ~AAA   set module mode
   szCmd[0]='~';
   c2=(char)(wAddr&0x0f);
   wAddr=wAddr/16;
   c1=(char)(wAddr&0x0f);
   szCmd[1] = HexToAscii(c1);
   szCmd[2] = HexToAscii(c2);
   szCmd[3]='A';
   szCmd[4]=0;

   if(wFlag==1)
   {
      strcpy(szSendTo7000,szCmd);
      szReceiveFrom7000[0]=0;
   }

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wRetVal != NoError)
   {
     err=SendCmdError;
     goto ret_err;
   }

   if(szRec[0] != '!')
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wCheckSum==0)  wRecStrLen=4;    else  wRecStrLen=6;

   if( strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError;
      goto ret_err;
   }


   if(wFlag==1)
   {
      strcpy(szReceiveFrom7000,szRec);
   }

   w7000[5]=szRec[3]-'0';

   return(NoError);

 ret_err:
   return(err);
}


/*-------- 7080 Set Level Volt ---------*/
EXPORTS WORD CALLBACK SetLevelVolt_7080(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
   char   wPort,wFlag,c1,c2;
   WORD   wAddr,wRetVal,wID,wCheckSum,wTimeOut,wHiLo,err,wT;
   WORD   wRecStrLen;
   float  fValue;
   char   OutCh[10];

   //---------------------- input -------------------------------------
   wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
   wAddr=w7000[1];         // module address: 0x00 to 0xFF
   wID=w7000[2];           // module ID: 0x7080
   wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];      // TimeOut constant: normal=100
   wHiLo=w7000[5];         // 0: Low Level,  1: High Level
   wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string
   fValue=f7000[0];        // Level Voltage
   //---------------------- output ------------------------------------

   if( wID!=0x7080 )
   {
      return(ModuleIdError);
   }


   if( fValue>5.0 )
      return ExceedInputRange;
   if( fValue<0.0 )
      return UnderInputRange;

   sprintf(OutCh,"%3.1f",fValue);

   if( wHiLo==0 )   // Set Low Level Voltage, $AA1L(data)
   {
      szCmd[0]='$';
      c2=(char)(wAddr&0x0f);
      wAddr=wAddr/16;
      c1=(char)(wAddr&0x0f);
      szCmd[1] = HexToAscii(c1);
      szCmd[2] = HexToAscii(c2);
      szCmd[3]='1';
      szCmd[4]='L';

      szCmd[5]=OutCh[0];
      szCmd[6]=OutCh[2];
      szCmd[7]=0;
   }
   else   // Set High Level Voltage, $AA1H(data)
   {
      szCmd[0]='$';
      c2=(char)(wAddr&0x0f);
      wAddr=wAddr/16;
      c1=(char)(wAddr&0x0f);
      szCmd[1] = HexToAscii(c1);
      szCmd[2] = HexToAscii(c2);
      szCmd[3]='1';
      szCmd[4]='H';

      szCmd[5]=OutCh[0];
      szCmd[6]=OutCh[2];
      szCmd[7]=0;
   }

   if(wFlag==1)
   {
      strcpy(szSendTo7000,szCmd);
      szReceiveFrom7000[0]=0;
   }

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wRetVal != NoError)
   {
     err=SendCmdError;
     goto ret_err;
   }

   if(szRec[0] != '!')
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wCheckSum==0) wRecStrLen=3; else wRecStrLen=5;

   if( strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wFlag==1)
   {
      strcpy(szReceiveFrom7000,szRec);
   }
   return(NoError);

 ret_err:
   return(err);
}

/*-------- 7080 Read Level Volt ---------*/
EXPORTS WORD CALLBACK ReadLevelVolt_7080(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
   char   wPort,wFlag,c1,c2;
   WORD   wAddr,wRetVal,wID,wCheckSum,wTimeOut,wHiLo,err,wT;
   WORD   wRecStrLen;
   //float  fValue;
   //char   OutCh[10];

   //---------------------- input -------------------------------------
   wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
   wAddr=w7000[1];         // module address: 0x00 to 0xFF
   wID=w7000[2];           // module ID: 0x7080
   wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];      // TimeOut constant: normal=100
   wHiLo=w7000[5];         // 0: Low Level,  1: High Level
   wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string
   //---------------------- output ------------------------------------
               //   fValue=f7000[0];        // Level Voltage

   if( wID!=0x7080 )
   {
      return(ModuleIdError);
   }

   if( wHiLo==0 )   // Read Low Level Voltage, $AA1L
   {
      szCmd[0]='$';
      c2=(char)(wAddr&0x0f);
      wAddr=wAddr/16;
      c1=(char)(wAddr&0x0f);
      szCmd[1] = HexToAscii(c1);
      szCmd[2] = HexToAscii(c2);
      szCmd[3]='1';
      szCmd[4]='L';
      szCmd[5]=0;
   }
   else   // Read High Level Voltage, $AA1H
   {
      szCmd[0]='$';
      c2=(char)(wAddr&0x0f);
      wAddr=wAddr/16;
      c1=(char)(wAddr&0x0f);
      szCmd[1] = HexToAscii(c1);
      szCmd[2] = HexToAscii(c2);
      szCmd[3]='1';
      szCmd[4]='H';
      szCmd[5]=0;
   }

   if(wFlag==1)
   {
      strcpy(szSendTo7000,szCmd);
      szReceiveFrom7000[0]=0;
   }

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wRetVal != NoError)
   {
     err=SendCmdError;
     goto ret_err;
   }

   if(szRec[0] != '!')
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wCheckSum==0) wRecStrLen=5; else wRecStrLen=7;

   if( strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wFlag==1)
   {
      strcpy(szReceiveFrom7000,szRec);
   }

   f7000[0]=(float)((szRec[3]-'0')*1.0+(szRec[4]-'0')*0.1);

   return(NoError);

 ret_err:
   return(err);
}


/* -------------- 7080 Set Min Signal Width ------------------------- */
EXPORTS WORD CALLBACK SetMinSignalWidth_7080(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[], long MinWidth)
{
   char    wPort,wFlag,c1,c2;
   WORD    wHiLo,wMinWidth;
   WORD    wAddr,wRetVal,wID,wCheckSum,wTimeOut,err,wT;
   WORD    wRecStrLen;
   char    Buffer[10];

   //---------------------- input -------------------------------------
   wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
   wAddr=w7000[1];         // module address: 0x00 to 0xFF
   wID=w7000[2];           // module ID: 0x7080
   wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];      // TimeOut constant: normal=100
   wHiLo=w7000[5];         // 0: Low Level,  1: High Level
   wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string

   if( wID!=0x7080 )
   {
      return(ModuleIdError);
   }

   if( wHiLo>1 )
       return(InvalidateCounterNo);

   if( MinWidth>65536 || MinWidth<0 )
      return( InvalidateCounterValue );

   wMinWidth=(WORD)MinWidth;
   sprintf(Buffer,"%05u",wMinWidth);

   if( wHiLo==0 ) // $AA0L(data),  Set Min Input Signal Width at Low Level
   {
       szCmd[0]='$';
       c2=(char)(wAddr&0x0f);
       wAddr=wAddr/16;
       c1=(char)(wAddr&0x0f);
       szCmd[1] = HexToAscii(c1);
       szCmd[2] = HexToAscii(c2);
       szCmd[3]='0';
       szCmd[4]='L';
       szCmd[5]=Buffer[0];
       szCmd[6]=Buffer[1];
       szCmd[7]=Buffer[2];
       szCmd[8]=Buffer[3];
       szCmd[9]=Buffer[4];
       szCmd[10]=0;
   }
   else         // $AA0H(data),  Set Min Input Signal Width at High Level
   {
       szCmd[0]='$';
       c2=(char)(wAddr&0x0f);
       wAddr=wAddr/16;
       c1=(char)(wAddr&0x0f);
       szCmd[1] = HexToAscii(c1);
       szCmd[2] = HexToAscii(c2);
       szCmd[3]='0';
       szCmd[4]='H';
       szCmd[5]=Buffer[0];
       szCmd[6]=Buffer[1];
       szCmd[7]=Buffer[2];
       szCmd[8]=Buffer[3];
       szCmd[9]=Buffer[4];
       szCmd[10]=0;
   }

   if(wFlag==1)
   {
      strcpy(szSendTo7000,szCmd);
      szReceiveFrom7000[0]=0;
   }

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wRetVal != NoError)
   {
     err=wRetVal;
     //err=SendCmdError;
     goto ret_err;
   }

   if(szRec[0] != '!')
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wCheckSum==0) wRecStrLen=3; else wRecStrLen=5;

   if( strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError;
      goto ret_err;
   }


   if(wFlag==1)
   {
      strcpy(szReceiveFrom7000,szRec);
   }
   return(NoError);

 ret_err:
   return(err);
}


/* -------------- 7080 Get Min Signal Width ------------------------- */
EXPORTS WORD CALLBACK ReadMinSignalWidth_7080(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
   char    wPort,wFlag,c1,c2;
   WORD    wHiLo;
   WORD    wAddr,wRetVal,wID,wCheckSum,wTimeOut,err,wT;
   WORD    wRecStrLen;
   WORD    A,B,C,D,E;
   //char    Buffer[10];

   //---------------------- input -------------------------------------
   wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
   wAddr=w7000[1];         // module address: 0x00 to 0xFF
   wID=w7000[2];           // module ID: 0x7080
   wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];      // TimeOut constant: normal=100
   wHiLo=w7000[5];         // 0: Low Level,  1: High Level
   wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string
   //---------------------- Output ------------------------------------
   // w7000[7]= Min Width

   if( wID!=0x7080 )
   {
      return(ModuleIdError);
   }

   if( wHiLo>1 )
       return(InvalidateCounterNo);


   if( wHiLo==0 ) // $AA0L,  Get Min Input Signal Width at Low Level
   {
       szCmd[0]='$';
       c2=(char)(wAddr&0x0f);
       wAddr=wAddr/16;
       c1=(char)(wAddr&0x0f);
       szCmd[1] = HexToAscii(c1);
       szCmd[2] = HexToAscii(c2);
       szCmd[3]='0';
       szCmd[4]='L';
       szCmd[5]=0;
   }
   else         // $AA0H,  Get Min Input Signal Width at High Level
   {
       szCmd[0]='$';
       c2=(char)(wAddr&0x0f);
       wAddr=wAddr/16;
       c1=(char)(wAddr&0x0f);
       szCmd[1] = HexToAscii(c1);
       szCmd[2] = HexToAscii(c2);
       szCmd[3]='0';
       szCmd[4]='H';
       szCmd[5]=0;
   }

   if(wFlag==1)
   {
      strcpy(szSendTo7000,szCmd);
      szReceiveFrom7000[0]=0;
   }

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wRetVal != NoError)
   {
     err=wRetVal;
     //err=SendCmdError;
     goto ret_err;
   }

   if(szRec[0] != '!')
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wCheckSum==0) wRecStrLen=8;  else  wRecStrLen=10;

   if( strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wFlag==1)
   {
      strcpy(szReceiveFrom7000,szRec);
   }

   A=AsciiToHex(szRec[3]);
   B=AsciiToHex(szRec[4]);
   C=AsciiToHex(szRec[5]);
   D=AsciiToHex(szRec[6]);
   E=AsciiToHex(szRec[7]);

   w7000[7]=A*10000+B*1000+C*100+D*10+E;

   return(NoError);

 ret_err:
   return(err);
}


/* -------------- 7080 Set Gate Mode ------------------------- */
EXPORTS WORD CALLBACK SetGateMode_7080(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
   char    wPort,wFlag,c1,c2;
   WORD    wGateMode;
   WORD    wAddr,wRetVal,wID,wCheckSum,wTimeOut,err,wT;
   WORD    wRecStrLen;

   //---------------------- input -------------------------------------
   wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
   wAddr=w7000[1];         // module address: 0x00 to 0xFF
   wID=w7000[2];           // module ID: 0x7080
   wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];      // TimeOut constant: normal=100
   wGateMode=w7000[5];         // 0: Low,  1: High,  2: None
   wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string

   if( wID!=0x7080 )
   {
      return(ModuleIdError);
   }

   if( wGateMode>2 )
       return(InvalidateGateMode);

   szCmd[0]='$';
   c2=(char)(wAddr&0x0f);
   wAddr=wAddr/16;
   c1=(char)(wAddr&0x0f);
   szCmd[1] = HexToAscii(c1);
   szCmd[2] = HexToAscii(c2);
   szCmd[3]='A';
   szCmd[4]=wGateMode+'0';
   szCmd[5]=0;

   if(wFlag==1)
   {
      strcpy(szSendTo7000,szCmd);
      szReceiveFrom7000[0]=0;
   }

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wRetVal != NoError)
   {
     err=wRetVal;
     //err=SendCmdError;
     goto ret_err;
   }

   if(szRec[0] != '!')
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wCheckSum==0) wRecStrLen=3;  else  wRecStrLen=5;

   if( strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wFlag==1)
   {
      strcpy(szReceiveFrom7000,szRec);
   }

   return(NoError);

 ret_err:
   return(err);
}


/* -------------- 7080 Read Gate Mode ------------------------- */
EXPORTS WORD CALLBACK ReadGateMode_7080(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
   char    wPort,wFlag,c1,c2;
   WORD    wAddr,wRetVal,wID,wCheckSum,wTimeOut,err,wT;
   WORD    wRecStrLen;

   //---------------------- input -------------------------------------
   wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
   wAddr=w7000[1];         // module address: 0x00 to 0xFF
   wID=w7000[2];           // module ID: 0x7080
   wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];      // TimeOut constant: normal=100
   //  wGateMode=w7000[5];     // 0: Low,  1: High,  2: None
   wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string
   //--------------------- output ---------------------------
   // w7000[5]:   // 0: Low,  1: High,  2: None

   if( wID!=0x7080 )
   {
      return(ModuleIdError);
   }

   //if( wGateMode>2 )
   //    return(InvalidateGateMode);

   szCmd[0]='$';
   c2=(char)(wAddr&0x0f);
   wAddr=wAddr/16;
   c1=(char)(wAddr&0x0f);
   szCmd[1] = HexToAscii(c1);
   szCmd[2] = HexToAscii(c2);
   szCmd[3]='A';
   szCmd[4]=0;

   if(wFlag==1)
   {
      strcpy(szSendTo7000,szCmd);
      szReceiveFrom7000[0]=0;
   }

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wRetVal != NoError)
   {
     err=wRetVal;
     //err=SendCmdError;
     goto ret_err;
   }

   if(szRec[0] != '!')
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wCheckSum==0) wRecStrLen=4;  else  wRecStrLen=6;

   if( strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wFlag==1)
   {
      strcpy(szReceiveFrom7000,szRec);
   }

   w7000[5]=szRec[3]-'0';
   return(NoError);

 ret_err:
   return(err);
}



/*-------- 7080 Send data to LED  ---------*/
EXPORTS WORD CALLBACK DataToLED_7080(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
   char   wPort,wFlag,c1,c2;
   WORD   wAddr,wRetVal,wID,wCheckSum,wTimeOut,wCounter,err,wT;
   WORD   wRecStrLen;
   float  fOutValue;
   char   OutCh[10];

   //---------------------- input -------------------------------------
   wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
   wAddr=w7000[1];         // module address: 0x00 to 0xFF
   wID=w7000[2];           // module ID: 0x7080
   wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];      // TimeOut constant: normal=100
   wCounter=w7000[5];      // Don't Care
   wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string
   fOutValue=f7000[0];     // Data to LED
   //---------------------- output ------------------------------------

   if( wID!=0x7080 )
   {
      return(ModuleIdError);
   }


   if( fOutValue>19999.0 )
      return ExceedInputRange;
   else if( fOutValue>10000.0 )
      sprintf(OutCh,"%6.1f",fOutValue);
   else if( fOutValue>1000.0 )
      sprintf(OutCh,"%06.1f",fOutValue);
   else if( fOutValue>100.0 )
      sprintf(OutCh,"%06.2f",fOutValue);
   else if( fOutValue>10.0 )
      sprintf(OutCh,"%06.3f",fOutValue);
   else if( fOutValue>1.0 )
      sprintf(OutCh,"%06.4f",fOutValue);
   else if( fOutValue>0.0 )
      sprintf(OutCh,"%06.5f",fOutValue);
   else
      return UnderInputRange;


   //***   $AA9(DATA)  read counter's input signal state
   szCmd[0]='$';
   c2=(char)(wAddr&0x0f);
   wAddr=wAddr/16;
   c1=(char)(wAddr&0x0f);
   szCmd[1] = HexToAscii(c1);
   szCmd[2] = HexToAscii(c2);
   szCmd[3]='9';

   szCmd[4]=OutCh[0];
   szCmd[5]=OutCh[1];
   szCmd[6]=OutCh[2];
   szCmd[7]=OutCh[3];
   szCmd[8]=OutCh[4];
   szCmd[9]=OutCh[5];

   szCmd[10]=0;

   if(wFlag==1)
   {
      strcpy(szSendTo7000,szCmd);
      szReceiveFrom7000[0]=0;
   }

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSum,&wT);
   if(wRetVal != NoError)
   {
     err=SendCmdError;
     goto ret_err;
   }

   if(szRec[0] != '!')
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wCheckSum==0) wRecStrLen=3; else wRecStrLen=5;

   if( strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wFlag==1)
   {
      strcpy(szReceiveFrom7000,szRec);
   }
   return(NoError);

 ret_err:
   return(err);
}

/*-------- Set configuration for 7080 ---------*/
EXPORTS WORD CALLBACK SetConfiguration_7080(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[])
{
   char   wPort,wFlag;
   WORD   wRetVal,wID,wTimeOut,err,wT;
   WORD   wRecStrLen;
   WORD   wAddrOld,wAddrNew,wType,wBaudrate,wCheckSumOld,wCheckSumNew;
   WORD   wTypeValue,wFormatValue,wFreqGateTime;

   //---------------------- input -------------------------------------
   wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
   wAddrOld=w7000[1];      // module original address: 0x00 to 0xFF
   wID=w7000[2];           // module ID: 0x7080
   wCheckSumOld=w7000[3];          // module original checksum: 0=disable, 1=enable
   wTimeOut=w7000[4];      // TimeOut constant: normal=100
   wFreqGateTime=w7000[5]; // desired frequency gate time:
                                // 0: 0.1 second
                                // 1: 1.0 second
                                // Don't care w7000[5] if set the module into Counter mode

   wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string

   wAddrNew=w7000[7];      // desired new address
   wType=w7000[8];         // desired Type->  0:Counter mode
                                             //  1:Frequency mode
   wBaudrate=w7000[9];     // desired Baudrate:
                           //    3: 1200 BPS    4:  2400 BPS
                           //    5: 4800 BPS    6:  9600 BPS
                           //    7: 19200 BPS   8:  38400 BPS
                           //    9: 57600 BPS   10: 115200 BPS
   wCheckSumNew=w7000[10]; // desired Checksum Address

   //------------------- check parameter ----------------------
   if( wAddrNew<0 || wAddrNew>255 )
      return(CmdError);
   if( wAddrOld<0 || wAddrOld>255 )
      return(CmdError);
   if( wType<0 || wType>1 )
      return(CmdError);
   if( wBaudrate<3 || wBaudrate>10 )
      return(CmdError);

   if( wID!=0x7080 )
   {
      return(ModuleIdError);
   }

   wTypeValue=(wType==0)?80:81;
   wFormatValue=64*wCheckSumNew+4*wFreqGateTime;

   sprintf(szCmd,"%%%02x%02x%02x%02x%02x",wAddrOld,wAddrNew,wTypeValue,
                                                     wBaudrate,wFormatValue);

   if(wFlag==1)
   {
      strcpy(szSendTo7000,szCmd);
      szReceiveFrom7000[0]=0;
   }

   wRetVal=Send_Receive_Cmd(wPort, szCmd, szRec, wTimeOut, wCheckSumOld,&wT);
   if(wRetVal != NoError)
   {
     err=SendCmdError;
     goto ret_err;
   }

   if(szRec[0] != '!')
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wCheckSumOld==0) wRecStrLen=3; else wRecStrLen=5;

   if( strlen(szRec) != wRecStrLen)
   {
      err=ResultStrCheckError;
      goto ret_err;
   }

   if(wFlag==1)
   {
      strcpy(szReceiveFrom7000,szRec);
   }
   return(NoError);

 ret_err:
   return(err);

}
