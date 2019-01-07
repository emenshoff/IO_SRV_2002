/***********************************************************
  Функции для формирования и разбора пакетов протокола
  I7000. Два типа функций (для асинхронной работы) : 
  ф-я отсылки запроса(с префиксом "Out") и ф-я чтения ответа
  ("In").
               created by M.E.N
***********************************************************/

#ifndef _I7000_proto_h_
#define _I7000_proto_h_

#define OK               1
#define CRC_ERROR       -2
#define READ_ERROR      -1
#define WRONG_PACKET    -3
#define WRONG_ADDRESS   -4
#define WRONG_COMMAND   -5
#define WRONG_ADDR      -6
#define IGNORED_COMMAND -7


class SerialPortI7000;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class I7000_Proto
{
public:
  I7000_Proto();
  virtual ~I7000_Proto();
protected:
  //---------- Read Digital Input --------------------------
  int OutDigitalIn(SerialPortI7000 *,
                   unsigned char addr,        //адрес модуля
                   unsigned char crc_enable,   //признак вычисления/проверки контрольной суммы
                   unsigned short module_id); //идентификатор модуля   
  int InDigitalIn(SerialPortI7000 *,
                   unsigned char addr,        //адрес модуля
                   unsigned char crc_enable,   //признак вычисления/проверки контрольной суммы
                   unsigned short module_id,   //идентификатор модуля
                   unsigned &result);         //4 - х байтовый результат
  //---------- Set Digital Output --------------------------
  int OutDigitalOut(SerialPortI7000 *,
                   unsigned char addr,        //адрес модуля
                   unsigned char crc_enable,   //признак вычисления/проверки контрольной суммы
                   unsigned short module_id); //идентификатор модуля
  int InDigitalOut(SerialPortI7000 *,
                   unsigned char addr,        //адрес модуля
                   unsigned char crc_enable,   //признак вычисления/проверки контрольной суммы
                   unsigned short module_id,  //идентификатор модуля
                   unsigned out);             //значения выходов
  /*//Широковещательный пакет "HOST Ok",  ~**
  int HostIsOK(SerialPortI7000 *,unsigned char crc_enable);
  //--- Read the module reset status, $AA5 --------
  int OutReadModuleResetStatus(SerialPortI7000 *,
                               unsigned char addr,        //адрес модуля
                               unsigned char crc_enable); //признак вычисления/проверки контрольной суммы
  int InReadModuleResetStatus(SerialPortI7000 *,
                              unsigned char addr,
                              unsigned char crc_enable,
                              unsigned char &reset_status);//результат
  //--- To setup host watchdog, ~AA3ETT --------
  int OutSetHostWatchdog(SerialPortI7000 *,
                          unsigned char addr,
                          unsigned char crc_enable,
                          unsigned char enable_host_watchdog,
      // a time interval for watchdog, unit is 0.1 second, e.x  45, the time interval is 4.5 second
                          unsigned char timeout_value);
  int InSetHostWatchdog(SerialPortI7000 *,unsigned char addr, unsigned char crc_enable);
  //--- To read host watchdog setup value, ~AA2 ----------
  int OutReadHostWatchdog(SerialPortI7000 *,unsigned char addr,unsigned char crc_enable);
  int InReadHostWatchdog(SerialPortI7000 *,unsigned char addr,unsigned char crc_enable,unsigned char &val);
  //--- Read the module status about the host watchdog, ~AA0 --------
  int OutReadModuleHostWatchdogStatus(SerialPortI7000 *,unsigned char addr,unsigned char crc_enable);
  int InReadModuleHostWatchdogStatus(SerialPortI7000 *,unsigned char addr,unsigned char crc_enable,unsigned char &status);
  //--- Reset the module status about the host watchdog, ~AA1 --------
  int OutResetModuleHostWatchdogStatus(SerialPortI7000 *,unsigned char addr,unsigned char crc_enable);
  int InResetModuleHostWatchdogStatus(SerialPortI7000 *,unsigned char addr,unsigned char crc_enable);
  //-------- Read Config Status ---------
  int OutReadConfig(SerialPortI7000 *,unsigned char addr,unsigned char crc_enable);  
  // cfg[0]:   module Range Code
  // cfg[1]:   module baudrate
  // cfg[2]:  module data format
  int InReadConfig(SerialPortI7000 *,unsigned char addr,unsigned char crc_enable,unsigned char cfg[]);
  //-------- Analog Input-----------------
  int OutAnalogIn(SerialPortI7000 *,unsigned char addr,unsigned char crc_enable);
  enum{ENG_MODE,FSTR_MODE,HEX_MODE};  
  !int InAnalogIn(SerialPortI7000 *,unsigned char addr,unsigned char crc_enable,unsigned char mode,float value[]);
  //-------- Analog InputN-----------------
  !int OutAnalogInN(SerialPortI7000 *,unsigned char addr,unsigned char crc_enable,unsigned char cnl_num);
  !int InAnalogInN(SerialPortI7000 *,unsigned char addr,unsigned char crc_enable,unsigned char cnl_num,unsigned char mode,float value[]);
  //---------------------------------------
  */
protected:
private:
};
//----------------------------------------------------------------
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
/*
//--Set PowerOn value for all DO module(42/43/44/50/60/65/67) ---
  int OutSetPowerOnValue(SerialPortI7000 *,unsigned char addr,unsigned char crc_enable,unsigned char value);
  int InSetPowerOnValue(SerialPortI7000 *,unsigned char addr,unsigned char crc_enable);
//--Set SafeValue for all Ao module(21/24), ~AA5 ---
  /*int SetSafeValueForAo(SerialPortI7000 *,unsigned char addr,unsigned char crc_enable);
//---------------------- input ---------------------------
//  w7000[0];   // RS-232 port number: 1/2/3/4
//  w7000[1];   // module address: 0x00 to 0xFF
//  w7000[2];   // module ID: 0x7021/0x7024
//  w7000[3];   // checksum: 0=disable, 1=enable
//  w7000[4];   // TimeOut constant: normal=100
//  w7000[5];   // Channel No
//  w7000[6];   // flag: 0=no save, 1=save send/receive string
//  f7000[0];   // safe value
   

  //--Set PowerOn value for all Ao module(21/24), $AA4 ---
int SetPowerOnValueForAo();
//  w7000[0];   // RS-232 port number: 1/2/3/4
//  w7000[1];   // module address: 0x00 to 0xFF
//  w7000[2];   // module ID: 0x7021/0x7024
//  w7000[3];   // checksum: 0=disable, 1=enable
//  w7000[4];   // TimeOut constant: normal=100
//  w7000[5];   // PowerOn value
//  w7000[6];   // flag: 0=no save, 1=save send/receive string
//  f7000[0];   // PowerOn value
  */
/*-------- 7011/7012/7014 Set PowerOn/Safe value ----------
/*int SetPowerOnSafeValue();
//  w7000[0];   // RS-232 port number: 1/2/3/4
//  w7000[1];   // module address: 0x00 to 0xFF
//  w7000[2];   // module ID: 0x7011/0x7012/0x7014
//  w7000[3];   // checksum: 0=disable, 1=enable
//  w7000[4];   // TimeOut constant: normal=100
//  w7000[5];   // PowerOn value
//  w7000[6];   // flag: 0=no save, 1=save send/receive string
//  w7000[7];   // safe value

  //--Read PowerOn value for all DO module(42/43/44/50/60/65/67) ---
  int OutReadPowerOnValue(SerialPortI7000 *,unsigned char addr,unsigned char crc_enable);
  int InReadPowerOnValue(SerialPortI7000 *,unsigned char addr,unsigned char crc_enable,unsigned char &value);
  //--Read SafeValue for all DO module(42/43/44/50/60/65/67) -----
  int OutReadSafeValue(SerialPortI7000 *,unsigned char addr,unsigned char crc_enable);
  int InReadSafeValue(SerialPortI7000 *,unsigned char addr,unsigned char crc_enable,unsigned char &value);
    

int In8_7017(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[]);
//---------------------- input -------------------------------------
// w7000[0];             // RS-232 port number: 1/2/3/4
// w7000[1];             // module address: 0x00 to 0xFF
// w7000[2];             // module Type: 08/09/0A/0B/0C/0D
// w7000[3];             // checksum: 0=disable, 1=enable
// w7000[4];             // TimeOut constant: normal=100
// w7000[6];             // flag: 0=no save, 1=save send/receive string
//---------------------- output ------------------------------------
                         // f7000[0]=analog input value of channel_0
                         // f7000[1]=analog input value of channel_1
                         // ........................................
                         // f7000[7]=analog input value of channel_7


int AnalogOut(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[]);
//---------------------- input -------------------------------------
// w7000[0];   // RS-232 port number: 1/2/3/4
// w7000[1];   // module address: 0x00 to 0xFF
// w7000[2];   // module ID: 0x7016/21/24
// w7000[3];   // checksum: 0=disable, 1=enable
// w7000[4];   // TimeOut constant: normal=100
// w7000[5];   // Not used if module ID is 7016/7021
                           // Channel No.(0 to 3) if module ID is 7024
// w7000[6];   // flag: 0=no save, 1=save send/receive string
// f7000[0];   // analog output value
//---------------------- output ------------------------------------
                           // void

int AnalogOutReadBack(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[]);
//---------------------- input -------------------------------------
// w7000[0];  // RS-232 port number: 1/2/3/4
// w7000[1];  // module address: 0x00 to 0xFF
// w7000[2];  // module ID: 0x7021 or 0x7024
// w7000[3];  // checksum: 0=disable, 1=enable
// w7000[4];  // TimeOut constant: normal=100
// w7000[5];  // When 7016: Don't care
              // When 7021/7024
              //   0: command read back ($AA6)
              //   1: when module ID is 0x7021
              //        analog output of current path read back ($AA8)
              //        when module ID is 0x7024
              //        the updating value in a specific Slew rate($AA8)
              //        (see 7024 manual for more detail information)
// w7000[6];  // flag: 0=no save, 1=save send/receive string
// w7000[7];  // Channel No.(0 to 3), if module ID is 7024
              //   Not used,        else
//---------------------- output ------------------------------------
              //   f7000[0]=analog output value read back


int DigitalOut(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[]);
//---------------------- input -------------------------------------
// w7000[0];             // RS-232 port number: 1/2/3/4
// w7000[1];             // module address: 0x00 to 0xFF
// w7000[2];             // module ID: 0x7050/60/63/65/66/67/42/43/44/11/12/14/80
// w7000[3];             // checksum: 0=disable, 1=enable
// w7000[4];             // TimeOut constant: normal=100
// w7000[5];             // 16-bit digital data to output
// w7000[6];             // flag: 0=no save, 1=save send/receive string
//---------------------- output ------------------------------------
                         // void
int DigitalOut_7016(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[]);
//---------------------- input -------------------------------------
// w7000[0];             // RS-232 port number: 1/2/3/4
// w7000[1];             // module address: 0x00 to 0xFF
// w7000[2];             // module ID: 0x7016
// w7000[3];             // checksum: 0=disable, 1=enable
// w7000[4];             // TimeOut constant: normal=100
// w7000[5];             // 2-bit digital data to output
// w7000[6];             // flag: 0=no save, 1=save send/receive string
// w7000[7];             // 0: Bit0, Bit1 output
                         // 1: Bit2, Bit3 output
//---------------------- output ------------------------------------
                         // void

int DigitalOutReadBack(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[]);
//---------------------- input -------------------------------------
// w7000[0];             // RS-232 port number: 1/2/3/4
// w7000[1];             // module address: 0x00 to 0xFF
// w7000[2];             // module ID: 0x7050/60/67/42/43/44
// w7000[3];             // checksum: 0=disable, 1=enable
// w7000[4];             // TimeOut constant: normal=100
// w7000[6];             // flag: 0=no save, 1=save send/receive string
//---------------------- output ------------------------------------
                         // w7000[5]=digital output data read back


int DigitalIn(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[]);
//---------------------- input -------------------------------------
// w7000[0];             // RS-232 port number: 1/2/3/4
// w7000[1];             // module address: 0x00 to 0xFF
// w7000[2];             // module ID: 0x7050/52/53/60/63/65/41/44/11/12/14/16
// w7000[3];             // checksum: 0=disable, 1=enable
// w7000[4];             // TimeOut constant: normal=100
// w7000[6];             // flag: 0=no save, 1=save send/receive string
//---------------------- output ------------------------------------
                         // w7000[5]=digital input data

int DigitalInCounterRead(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[]);
//---------------------- input -------------------------------------
// w7000[0];             // RS-232 port number: 1/2/3/4
// w7000[1];             // module address: 0x00 to 0xFF
// w7000[2];             // module ID: 0x7050/52/53/60/63/65/41/44
// w7000[3];             // checksum: 0=disable, 1=enable
// w7000[4];             // TimeOut constant: normal=100
// w7000[5];             // Channel Number
// w7000[6];             // flag: 0=no save, 1=save send/receive string
//---------------------- output ------------------------------------
// w7000[7]=Counter Value of Channel N's Digital Input


int ClearDigitalInCounter(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[]);
//---------------------- input -------------------------------------
// w7000[0];             // RS-232 port number: 1/2/3/4
// w7000[1];             // module address: 0x00 to 0xFF
// w7000[2];             // module ID: 0x7050/52/53/60/63/65/41/44
// w7000[3];             // checksum: 0=disable, 1=enable
// w7000[4];             // TimeOut constant: normal=100
// w7000[5];             // Channel Number
// w7000[6];             // flag: 0=no save, 1=save send/receive string


int DigitalInLatch(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[]);
//---------------------- input -------------------------------------
// w7000[0];             // RS-232 port number: 1/2/3/4
// w7000[1];             // module address: 0x00 to 0xFF
// w7000[2];             // module ID: 0x7050/52/53/60/63/65/41/44
// w7000[3];             // checksum: 0=disable, 1=enable
// w7000[4];             // TimeOut constant: normal=100
// w7000[5];             // 0: Latch Low,   1: Latch High
// w7000[6];             // flag: 0=no save, 1=save send/receive string
//---------------------- output ------------------------------------
// w7000[7];             // latch value


int ClearDigitalInLatch(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[]);
//---------------------- input -------------------------------------
// w7000[0];             // RS-232 port number: 1/2/3/4
// w7000[1];             // module address: 0x00 to 0xFF
// w7000[2];             // module ID: 0x7050/52/53/60/63/65/41/44
// w7000[3];             // checksum: 0=disable, 1=enable
// w7000[4];             // TimeOut constant: normal=100
// w7000[5];             // Don't care
// w7000[6];             // flag: 0=no save, 1=save send/receive string

/*-------- 7011 Thermocouple Open Detection -----------
int ThermocoupleOpen_7011(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[]);
//---------------------- input -------------------------------------
//w7000[0];   // RS-232 port number: 1/2/3/4
//w7000[1];   // module address: 0x00 to 0xFF
//w7000[2];   // module ID: 0x7011
//w7000[3];   // checksum: 0=disable, 1=enable
//w7000[4];   // TimeOut constant: normal=100
//w7000[6];   // flag: 0=no save, 1=save send/receive string
//---------------------- output ------------------------------------
//w7000[5]   0: Close     1: Open


/*----- 7011/7012/7014 Set Power On and Safe value -----------
int SetPowerOnAndSafeValue(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[]);
//---------------------- input -------------------------------------
//w7000[0];   // RS-232 port number: 1/2/3/4
//w7000[1];   // module address: 0x00 to 0xFF
//w7000[2];   // module ID: 0x7011
//w7000[3];   // checksum: 0=disable, 1=enable
//w7000[4];   // TimeOut constant: normal=100  , 0.1 second
//w7000[5];   // 0xPPSS:  PP=Power On value   SS=safe value
              //              00: DO0 -> Off   DO1 -> Off
              //              01: DO0 -> On    DO1 -> Off
              //              02: DO0 -> Off   DO1 -> On
              //              03: DO0 -> On    DO1 -> On
//w7000[6];   // flag: 0=no save, 1=save send/receive string



/*----- 7011/7012/7014 Enable Alarm -----------
int EnableAlarm(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[]);
//---------------------- input -------------------------------------
//w7000[0];   // RS-232 port number: 1/2/3/4
//w7000[1];   // module address: 0x00 to 0xFF
//w7000[2];   // module ID: 0x7011
//w7000[3];   // checksum: 0=disable, 1=enable
//w7000[4];   // TimeOut constant: normal=100  , 0.1 second
//w7000[5];   // 0: Momentary,  1: Latch
//w7000[6];   // flag: 0=no save, 1=save send/receive string


/*----- 7011/7012/7014 Disable Alarm -----------
int DisableAlarm(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[]);
//---------------------- input -------------------------------------
//w7000[0];   // RS-232 port number: 1/2/3/4
//w7000[1];   // module address: 0x00 to 0xFF
//w7000[2];   // module ID: 0x7011/0x7012/0x7014
//w7000[3];   // checksum: 0=disable, 1=enable
//w7000[4];   // TimeOut constant: normal=100  , 0.1 second
//w7000[5];   don't care
//w7000[6];   // flag: 0=no save, 1=save send/receive string


/*-------- 7011/7012/7014/7016 Clear Latch Alarm -----------
int ClearLatchAlarm(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[]);
//---------------------- input -------------------------------------
//w7000[0];   // RS-232 port number: 1/2/3/4
//w7000[1];   // module address: 0x00 to 0xFF
//w7000[2];   // module ID: 0x7011
//w7000[3];   // checksum: 0=disable, 1=enable
//w7000[4];   // TimeOut constant: normal=100  , 0.1 second
//w7000[5];   don't care
//w7000[6];   // flag: 0=no save, 1=save send/receive string


/*--- 7011/7012/7014/7016 Set Hi/Lo Alarm Limit Value --------
/*******************************************************/
/* please get the module's range code by calling       */
/* ReadConfigStatus(.......) function before           */
/* calling SetAlarmLimitValue(......).                 */
/*******************************************************/
/*int SetAlarmLimitValue(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[]);
//---------------------- input -------------------------------------
//w7000[0];   // RS-232 port number: 1/2/3/4
//w7000[1];   // module address: 0x00 to 0xFF
//w7000[2];   // module ID: 0x7011
//w7000[3];   // checksum: 0=disable, 1=enable
//w7000[4];   // TimeOut constant: normal=100  , 0.1 second
//w7000[5];   // 0: Lo Alarm     1: Hi Alarm
//w7000[6];   // flag: 0=no save, 1=save send/receive string
//w7000[7];   // range code of this module
//f7000[0]:   // Alarm Limit Value

/*-------- 7011/7012/7014/7016 Read Hi/Lo Alarm Limit Value -----------
int ReadAlarmLimitValue(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[]);
//---------------------- input ----------------------------------
//w7000[0];   // RS-232 port number: 1/2/3/4
//w7000[1];   // module address: 0x00 to 0xFF
//w7000[2];   // module ID: 0x7011
//w7000[3];   // checksum: 0=disable, 1=enable
//w7000[4];   // TimeOut constant: normal=100  , 0.1 second
//w7000[5];   // 0: Lo Alarm     1: Hi Alarm
//w7000[6];   // flag: 0=no save, 1=save send/receive string
//---------------------- output ---------------------------------
//f7000[0]:   // Alarm Limit Value

/*---- Read Alarm State and Digital Output State -----------
int ReadOutputAlarmState(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[]);
//---------------------- input -------------------------------------
//w7000[0];   // RS-232 port number: 1/2/3/4
//w7000[1];   // module address: 0x00 to 0xFF
//w7000[2];   // module ID
//w7000[3];   // checksum: 0=disable, 1=enable
//w7000[4];   // TimeOut constant: normal=100
//w7000[5];   // Don't Care
//w7000[6];   // flag: 0=no save, 1=save send/receive string
//---------------------- output ------------------------------------
//               Alarm State:
// w7000(7):0       Disable
// w7000(7):1       Momentary
// w7000(7):2       Latch
//               Digital Output
// w7000(8)    -> a decimal number to represent the digital output status


/*-------- 7011/7012/7014 Read Event Counter ----------
int ReadEventCounter(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[]);
//---------------------- input -------------------------------------
//w7000[0];   // RS-232 port number: 1/2/3/4
//w7000[1];   // module address: 0x00 to 0xFF
//w7000[2];   // module ID
//w7000[3];   // checksum: 0=disable, 1=enable
//w7000[4];   // TimeOut constant: normal=100
//w7000[5];   // Don't Care
//w7000[6];   // flag: 0=no save, 1=save send/receive string
//---------------------- output ------------------------------------
//w7000[7];   // The Event Counter Value

/*-------- 7011/7012/7014/7016 Clear Event Counter ----------
int ClearEventCounter(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[]);
//---------------------- input -------------------------------------
//w7000[0];   // RS-232 port number: 1/2/3/4
//w7000[1];   // module address: 0x00 to 0xFF
//w7000[2];   // module ID
//w7000[3];   // checksum: 0=disable, 1=enable
//w7000[4];   // TimeOut constant: normal=100
//w7000[5];   // Don't Care
//w7000[6];   // flag: 0=no save, 1=save send/receive string

/*-------- Set LED channel Display for 7033 ----------
int SetLedDisplay_7033(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[]);
//---------------------- input -------------------------------------
//w7000[0];   // RS-232 port number: 1/2/3/4
//w7000[1];   // module address: 0x00 to 0xFF
//w7000[2];   // module ID
//w7000[3];   // checksum: 0=disable, 1=enable
//w7000[4];   // TimeOut constant: normal=100
//w7000[5];   // set display channel for 7033, 0<=channel<=2
//w7000[6];   // flag: 0=no save, 1=save send/receive string

/*-------- Get LED channel Display for 7033 ----------
int GetLedDisplay_7033(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[]);
//---------------------- input -------------------------------------
//w7000[0];   // RS-232 port number: 1/2/3/4
//w7000[1];   // module address: 0x00 to 0xFF
//w7000[2];   // module ID
//w7000[3];   // checksum: 0=disable, 1=enable
//w7000[4];   // TimeOut constant: normal=100
//w7000[6];   // flag: 0=no save, 1=save send/receive string
//----------------------- output -----------------------------------
//w7000[5];   // which channel display

/**********************************************************/
/*-------- Set LED channel Display for 7016, 7033 ----------
int SetLedDisplay(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[]);
//---------------------- input -------------------------------------
//w7000[0];   // RS-232 port number: 1/2/3/4
//w7000[1];   // module address: 0x00 to 0xFF
//w7000[2];   // module ID
//w7000[3];   // checksum: 0=disable, 1=enable
//w7000[4];   // TimeOut constant: normal=100
//w7000[5];   // set display channel for 7013, 7033
//w7000[6];   // flag: 0=no save, 1=save send/receive string

/*-------- Get LED channel Display for 7016, 7033 ----------
int GetLedDisplay(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[]);
//---------------------- input -------------------------------------
//w7000[0];   // RS-232 port number: 1/2/3/4
//w7000[1];   // module address: 0x00 to 0xFF
//w7000[2];   // module ID
//w7000[3];   // checksum: 0=disable, 1=enable
//w7000[4];   // TimeOut constant: normal=100
//w7000[6];   // flag: 0=no save, 1=save send/receive string
//----------------------- output -----------------------------------
//w7000[5];   // which channel display


/*******************************************************/
/* please get the module's range code by calling       */
/* ReadConfigStatus(.......) function before           */
/* calling SetupLinearMapping(......).                 */
/*-------- setup linear mapping for 7014/7016 ---------*/
/*  making a linear mapping from [a,b] to [c,d]        */
/*  where:  f7000[0]=a,  f7000[1]=b                    */
/*          f7000[2]=c,  f7000[3]=d                    */
/*******************************************************/
/*int SetupLinearMapping(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[]);
//---------------------- input -------------------------------------
//w7000[0];   // RS-232 port number: 1/2/3/4
//w7000[1];   // module address: 0x00 to 0xFF
//w7000[2];   // module ID: 0x7014, 0x7016
//w7000[3];   // checksum: 0=disable, 1=enable
//w7000[4];   // TimeOut constant: normal=100  , 0.1 second
//w7000[5];   // do't care
//w7000[6];   // flag: 0=no save, 1=save send/receive string
//w7000[7];   // range code of this module
//f7000[0]:   // source low value,  a
//f7000[1];   // source high value, b
//f7000[2];   // target low value,  c
//f7000[3];   // target high value, d

/*----- Enable linear mapping for 7014/7016 -----------
int EnableLinearMapping(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[]);
//---------------------- input -------------------------------------
//w7000[0];   // RS-232 port number: 1/2/3/4
//w7000[1];   // module address: 0x00 to 0xFF
//w7000[2];   // module ID: 0x7014, 0x7016
//w7000[3];   // checksum: 0=disable, 1=enable
//w7000[4];   // TimeOut constant: normal=100  , 0.1 second
//w7000[5];   don't care
//w7000[6];   // flag: 0=no save, 1=save send/receive string

/*----- Disable linear mapping for 7014/7016 -----------
int DisableLinearMapping(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[]);
//---------------------- input -------------------------------------
//w7000[0];   // RS-232 port number: 1/2/3/4
//w7000[1];   // module address: 0x00 to 0xFF
//w7000[2];   // module ID: 0x7014, 0x7016
//w7000[3];   // checksum: 0=disable, 1=enable
//w7000[4];   // TimeOut constant: normal=100  , 0.1 second
//w7000[5];   don't care
//w7000[6];   // flag: 0=no save, 1=save send/receive string

/*----- Reading Linear Mapping Status for 7014/7016 -------
int ReadLinearMappingStatus(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[]);
//---------------------- input -------------------------------------
//w7000[0];   // RS-232 port number: 1/2/3/4
//w7000[1];   // module address: 0x00 to 0xFF
//w7000[2];   // module ID: 0x7014, 0x7016
//w7000[3];   // checksum: 0=disable, 1=enable
//w7000[4];   // TimeOut constant: normal=100  , 0.1 second
//w7000[6];   // flag: 0=no save, 1=save send/receive string
//--------------------- output -------------------------------------
//w7000[5];   // 0: linear mapping is disable
              // 1: linear mapping is enable

/*----- Reading Source Value of Linear Mapping for 7014/7016 -------
int ReadSourceValueOfLM(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[]);
//---------------------- input -------------------------------------
//w7000[0];   // RS-232 port number: 1/2/3/4
//w7000[1];   // module address: 0x00 to 0xFF
//w7000[2];   // module ID: 0x7014, 0x7016
//w7000[3];   // checksum: 0=disable, 1=enable
//w7000[4];   // TimeOut constant: normal=100  , 0.1 second
//w7000[6];   // flag: 0=no save, 1=save send/receive string
//--------------------- output -------------------------------------
//f7000[0];   // Low Source Value
//f7000[1];   // High Source Value

/*----- Reading Target Value of Linear Mapping for 7014/7016 -------
int ReadTargetValueOfLM(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[]);
//---------------------- input -------------------------------------
//w7000[0];   // RS-232 port number: 1/2/3/4
//w7000[1];   // module address: 0x00 to 0xFF
//w7000[2];   // module ID: 0x7014, 0x7016
//w7000[3];   // checksum: 0=disable, 1=enable
//w7000[4];   // TimeOut constant: normal=100  , 0.1 second
//w7000[6];   // flag: 0=no save, 1=save send/receive string
//--------------------- output -------------------------------------
//f7000[0];   // Low Target Value
//f7000[1];   // High Target Value

/**************************************************************************/
/*int CounterIn_7080(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[]);
//---------------------- input -------------------------------------
// w7000[0];             // RS-232 port number: 1/2/3/4
// w7000[1];             // module address: 0x00 to 0xFF
// w7000[2];             // module ID: 0x7080
// w7000[3];             // checksum: 0=disable, 1=enable
// w7000[4];             // TimeOut constant: normal=100
// w7000[5];             // 0: Counter 0,  1: Counter 1
// w7000[6];             // flag: 0=no save, 1=save send/receive string
//---------------------- output ------------------------------------
                         // w7000[7]  Hi-Word of counter value
                         // w7000[8]  Lo-Word of counter value


int ReadCounterMaxValue_7080(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[]);
//---------------------- input -------------------------------------
// w7000[0];             // RS-232 port number: 1/2/3/4
// w7000[1];             // module address: 0x00 to 0xFF
// w7000[2];             // module ID: 0x7080
// w7000[3];             // checksum: 0=disable, 1=enable
// w7000[4];             // TimeOut constant: normal=100
// w7000[5];             // 0: Counter 0,  1: Counter 1
// w7000[6];             // flag: 0=no save, 1=save send/receive string
//---------------------- output ------------------------------------
                         // w7000[7]  Hi-Word of counter value
                         // w7000[8]  Lo-Word of counter value


int SetCounterMaxValue_7080(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[], double MaxValue);
//---------------------- input -------------------------------------
// w7000[0];             // RS-232 port number: 1/2/3/4
// w7000[1];             // module address: 0x00 to 0xFF
// w7000[2];             // module ID: 0x7080
// w7000[3];             // checksum: 0=disable, 1=enable
// w7000[4];             // TimeOut constant: normal=100
// w7000[5];             // Don't Care
// w7000[6];             // flag: 0=no save, 1=save send/receive string
// MaxValue    Setting the Counter's Max Value


int ReadAlarmLimitValue_7080(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[]);
//---------------------- input -------------------------------------
// w7000[0];             // RS-232 port number: 1/2/3/4
// w7000[1];             // module address: 0x00 to 0xFF
// w7000[2];             // module ID: 0x7080
// w7000[3];             // checksum: 0=disable, 1=enable
// w7000[4];             // TimeOut constant: normal=100
// w7000[5];             // 0: Counter 0,  1: Counter 1
// w7000[6];             // flag: 0=no save, 1=save send/receive string
//---------------------- output ------------------------------------
// w7000[7]  Hi-Word of counter value
// w7000[8]  Lo-Word of counter value


int SetAlarmLimitValue_7080(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[], double AlarmValue);
//---------------------- input -------------------------------------
// w7000[0];             // RS-232 port number: 1/2/3/4
// w7000[1];             // module address: 0x00 to 0xFF
// w7000[2];             // module ID: 0x7080
// w7000[3];             // checksum: 0=disable, 1=enable
// w7000[4];             // TimeOut constant: normal=100
// w7000[5];             // when in 7080 alarm mode(mode 0)
//                                      0:   To set Counter 0 alarm value
//                              1:   To set Counter 1 alarm value
//                               // when in 7080D alarm mode(mode 0)
//                                      0:   To set Counter 0 high alarm value
//                                      1:   To set Counter 0 high-high alarm value
// w7000[6];             // flag: 0=no save, 1=save send/receive string


int EnableCounterAlarm_7080(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[]);
//---------------------- input -------------------------------------
// w7000[0];             // RS-232 port number: 1/2/3/4
// w7000[1];             // module address: 0x00 to 0xFF
// w7000[2];             // module ID: 0x7080
// w7000[3];             // checksum: 0=disable, 1=enable
// w7000[4];             // TimeOut constant: normal=100
// w7000[5];             // 0: counter 0 enable, 1: counter 1 enable
// w7000[6];             // flag: 0=no save, 1=save send/receive string

int DisableCounterAlarm_7080(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[]);
//---------------------- input -------------------------------------
// w7000[0];             // RS-232 port number: 1/2/3/4
// w7000[1];             // module address: 0x00 to 0xFF
// w7000[2];             // module ID: 0x7080
// w7000[3];             // checksum: 0=disable, 1=enable
// w7000[4];             // TimeOut constant: normal=100
// w7000[5];             // Don't care
// w7000[6];             // flag: 0=no save, 1=save send/receive string


int EnableCounterAlarm_7080D(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[]);
//---------------------- input -------------------------------------
// w7000[0];             // RS-232 port number: 1/2/3/4
// w7000[1];             // module address: 0x00 to 0xFF
// w7000[2];             // module ID: 0x7080
// w7000[3];             // checksum: 0=disable, 1=enable
// w7000[4];             // TimeOut constant: normal=100
// w7000[5];             // 0: momentary, 1: latch
// w7000[6];             // flag: 0=no save, 1=save send/receive string

int DisableCounterAlarm_7080D(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[]);
//---------------------- input -------------------------------------
// w7000[0];             // RS-232 port number: 1/2/3/4
// w7000[1];             // module address: 0x00 to 0xFF
// w7000[2];             // module ID: 0x7080
// w7000[3];             // checksum: 0=disable, 1=enable
// w7000[4];             // TimeOut constant: normal=100
// w7000[5];             // Don't care
// w7000[6];             // flag: 0=no save, 1=save send/receive string


int ReadCounterStatus_7080(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[]);
//---------------------- input -------------------------------------
// w7000[0];             // RS-232 port number: 1/2/3/4
// w7000[1];             // module address: 0x00 to 0xFF
// w7000[2];             // module ID: 0x7080
// w7000[3];             // checksum: 0=disable, 1=enable
// w7000[4];             // TimeOut constant: normal=100
// w7000[5];             // 0: Counter 0,  1: Counter 1
// w7000[6];             // flag: 0=no save, 1=save send/receive string
//---------------------- output ------------------------------------
// w7000[7]  0: Counting   1: Not Counting

int ClearCounter_7080(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[]);
//---------------------- input -------------------------------------
// w7000[0];             // RS-232 port number: 1/2/3/4
// w7000[1];             // module address: 0x00 to 0xFF
// w7000[2];             // module ID: 0x7080
// w7000[3];             // checksum: 0=disable, 1=enable
// w7000[4];             // TimeOut constant: normal=100
// w7000[5];             // 0: Counter 0,  1: Counter 1
// w7000[6];             // flag: 0=no save, 1=save send/receive string


int ReadOutputAlarmState_7080(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[]);
//---------------------- input -------------------------------------
// w7000[0];             // RS-232 port number: 1/2/3/4
// w7000[1];             // module address: 0x00 to 0xFF
// w7000[2];             // module ID: 0x7080
// w7000[3];             // checksum: 0=disable, 1=enable
// w7000[4];             // TimeOut constant: normal=100
// w7000[5];             // Don't Care
// w7000[6];             // flag: 0=no save, 1=save send/receive string
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


int SetInputSignalMode_7080(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[]);
//---------------------- input -------------------------------------
// w7000[0];             // RS-232 port number: 1/2/3/4
// w7000[1];             // module address: 0x00 to 0xFF
// w7000[2];             // module ID: 0x7080
// w7000[3];             // checksum: 0=disable, 1=enable
// w7000[4];             // TimeOut constant: normal=100
// w7000[5];             // Counter 0's and 1's signal mode
// w7000[6];             // flag: 0=no save, 1=save send/receive string
//---------------------- Input -------------------------------------
                    // w7000(5):0    Counter:0 TTL      Counter:0  TTL
                    // w7000(5):1    Counter:1 Photo    Counter:1  Photo
                    // w7000(5):2    Counter:0 TTL      Counter:1  Photo
                    // w7000(5):3    Counter:1 Photo    Counter:0  TTL


int ReadInputSignalMode_7080(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[]);
//---------------------- input -------------------------------------
// w7000[0];             // RS-232 port number: 1/2/3/4
// w7000[1];             // module address: 0x00 to 0xFF
// w7000[2];             // module ID: 0x7080
// w7000[3];             // checksum: 0=disable, 1=enable
// w7000[4];             // TimeOut constant: normal=100
// w7000[5];             // Don't Care
// w7000[6];             // flag: 0=no save, 1=save send/receive string
//---------------------- output ------------------------------------
                    // w7000(7):0    Counter:0 TTL      Counter:0  TTL
                    // w7000(7):1    Counter:1 Photo    Counter:1  Photo
                    // w7000(7):2    Counter:0 TTL      Counter:1  Photo
                    // w7000(7):3    Counter:1 Photo    Counter:0  TTL


// ------------ 7080 Preset Counter 0/1 Counter Value ---------------- --
int PresetCounterValue_7080(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[], double PresetValue);

// ------------ 7080 Read Preset Counter 0/1 Counter Value ------------- --
int ReadPresetCounterValue_7080(WORD w7000[], float f7000[],
                            char szSendTo7000[], char szReceiveFrom7000[]);
//---------------------- output ------------------------------------
                      // w7000[7]= Hi-Word of Preset Counter Value
                      // w7000[8]= Lo-Word of Preset Counter Value

//-------- 7080 Start Counting -----------
int StartCounting_7080(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[]);
//---------------------- input -------------------------------------
//w7000[0];   // RS-232 port number: 1/2/3/4
//w7000[1];   // module address: 0x00 to 0xFF
//w7000[2];   // module ID: 0x7080
//w7000[3];   // checksum: 0=disable, 1=enable
//w7000[4];   // TimeOut constant: normal=100
//w7000[5];   // 0: Counter 0   1: Counter 1
//w7000[6];   // flag: 0=no save, 1=save send/receive string
//w7000[7];   // 0: Stop Counting  1: Start Counting


//-------- Set 7080 module mode  -----------
int SetModuleMode_7080(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[]);
//---------------------- input -------------------------------------
//w7000[0];   // RS-232 port number: 1/2/3/4
//w7000[1];   // module address: 0x00 to 0xFF
//w7000[2];   // module ID: 0x7080
//w7000[3];   // checksum: 0=disable, 1=enable
//w7000[4];   // TimeOut constant: normal=100
//w7000[5];   // 0: 7080 mode   1: 7080D mode
//w7000[6];   // flag: 0=no save, 1=save send/receive string

//-------- Read 7080 module mode  -----------
int ReadModuleMode_7080(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[]);
//---------------------- input -------------------------------------
//w7000[0];   // RS-232 port number: 1/2/3/4
//w7000[1];   // module address: 0x00 to 0xFF
//w7000[2];   // module ID: 0x7080
//w7000[3];   // checksum: 0=disable, 1=enable
//w7000[4];   // TimeOut constant: normal=100
//w7000[5];   // don't care
//w7000[6];   // flag: 0=no save, 1=save send/receive string
//------------------ output ---------------------
//w7000[5]= 0: 7080 mode,  1: 7080D mode


//-------- 7080 Set Level Volt -----------
int SetLevelVolt_7080(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[]);
//---------------------- input -------------------------------------
//w7000[0];   // RS-232 port number: 1/2/3/4
//w7000[1];   // module address: 0x00 to 0xFF
//w7000[2];   // module ID: 0x7080
//w7000[3];   // checksum: 0=disable, 1=enable
//w7000[4];   // TimeOut constant: normal=100
//w7000[5];   // 0: Low Level,  1: High Level
//w7000[6];   // flag: 0=no save, 1=save send/receive string
//f7000[0];   // Level Voltage


//-------- 7080 Read Level Volt -----------
int ReadLevelVolt_7080(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[]);
//---------------------- input ------------------------------------
//w7000[0];   // RS-232 port number: 1/2/3/4
//w7000[1];   // module address: 0x00 to 0xFF
//w7000[2];   // module ID: 0x7080
//w7000[3];   // checksum: 0=disable, 1=enable
//w7000[4];   // TimeOut constant: normal=100
//w7000[5];   // 0: Low Level,  1: High Level
//w7000[6];   // flag: 0=no save, 1=save send/receive string
//----------------------- output ----------------------------------
//f7000[0];        // Level Voltage


// -------------- 7080 Set Min Signal Width ------------------------- --
int SetMinSignalWidth_7080(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[], long MinWidth);
//---------------------- input -------------------------------------
//w7000[0];   // RS-232 port number: 1/2/3/4
//w7000[1];   // module address: 0x00 to 0xFF
//w7000[2];   // module ID: 0x7080
//w7000[3];   // checksum: 0=disable, 1=enable
//w7000[4];   // TimeOut constant: normal=100
//w7000[5];   // 0: Low Level,  1: High Level
//w7000[6];   // flag: 0=no save, 1=save send/receive string
//MinWidth    // the Input Signal Min Width

// -------------- 7080 Read Min Signal Width ------------------------- --
int ReadMinSignalWidth_7080(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[]);
//---------------------- input ----------------------------------
//w7000[0];   // RS-232 port number: 1/2/3/4
//w7000[1];   // module address: 0x00 to 0xFF
//w7000[2];   // module ID: 0x7080
//w7000[3];   // checksum: 0=disable, 1=enable
//w7000[4];   // TimeOut constant: normal=100
//w7000[5];   // 0: Low Level,  1: High Level
//w7000[6];   // flag: 0=no save, 1=save send/receive string
//---------------------- Output ---------------------------------
//w7000[7]    // the Reading Input Signal Min Width


// -------------- 7080 Set Gate Mode ------------------------- --
int SetGateMode_7080(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[]);
//---------------------- input -------------------------------------
//w7000[0];   // RS-232 port number: 1/2/3/4
//w7000[1];   // module address: 0x00 to 0xFF
//w7000[2];   // module ID: 0x7080
//w7000[3];   // checksum: 0=disable, 1=enable
//w7000[4];   // TimeOut constant: normal=100
//w7000[5];   // 0: Low,  1: High,  2: None
//w7000[6];   // flag: 0=no save, 1=save send/receive string

// -------------- 7080 Read Gate Mode ------------------------- --
int ReadGateMode_7080(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[]);
//---------------------- input ---------------------------------
//w7000[0];   // RS-232 port number: 1/2/3/4
//w7000[1];   // module address: 0x00 to 0xFF
//w7000[2];   // module ID: 0x7080
//w7000[3];   // checksum: 0=disable, 1=enable
//w7000[4];   // TimeOut constant: normal=100
//w7000[6];   // flag: 0=no save, 1=save send/receive string
//---------------------- output --------------------------------
//w7000[5];   // 0: Low,  1: High,  2: None


//-------- 7080 Send data to LED  -----------
int DataToLED_7080(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[]);
//---------------------- input -------------------------------------
//wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
//wAddr=w7000[1];         // module address: 0x00 to 0xFF
//wID=w7000[2];           // module ID: 0x7080
//wCheckSum=w7000[3];     // checksum: 0=disable, 1=enable
//wTimeOut=w7000[4];      // TimeOut constant: normal=100
//wCounter=w7000[5];      // Don't Care
//wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string
//fOutValue=f7000[0];     // Data to LED


//-------- Set configuration for 7080 -----------
int SetConfiguration_7080(WORD w7000[], float f7000[],
        char szSendTo7000[], char szReceiveFrom7000[]);
///** Notice: if you change the Baudrate or Checksum, please short the INIT*(pin6) to GND(pin10)
//---------------------- input -------------------------------------
//wPort=(char)w7000[0];   // RS-232 port number: 1/2/3/4
//wAddrOld=w7000[1];      // module original address: 0x00 to 0xFF
//wID=w7000[2];           // module ID: 0x7080
//wCheckSumOld=w7000[3];          // module original checksum: 0=disable, 1=enable
//wTimeOut=w7000[4];      // TimeOut constant: normal=100
//wFreqGateTime=w7000[5]; // desired frequency gate time:
                          //      0: 0.1 second
                          //      1: 1.0 second
                          // Don't care w7000[5],if set the module into Counter mode
//wFlag=(char)w7000[6];   // flag: 0=no save, 1=save send/receive string
//wAddrNew=w7000[7];      // desired new address
//wType=w7000[8];         // desired Type->  0:Counter mode
                          //                 1:Frequency mode
//wBaudrate=w7000[9];     // desired Baudrate:
                          //    3: 1200 BPS    4:  2400 BPS
                          //    5: 4800 BPS    6:  9600 BPS
                          //    7: 19200 BPS   8:  38400 BPS
                          //    9: 57600 BPS   10: 115200 BPS
//wCheckSumNew=w7000[10]; // desired Checksum Address
*/
#endif //I7000_proto