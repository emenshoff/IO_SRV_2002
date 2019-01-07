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


class I7000_SerialPort;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
/*class I7000_Proto
{
public:*/
  //---------- Read Digital Input --------------------------
  int OutDigitalIn(I7000_SerialPort *,
                   unsigned char addr,        //адрес модуля
                   unsigned char crc_enable,  //признак вычисления/проверки контрольной суммы
                   unsigned short module_id,   //идентификатор модуля
                   unsigned not_used);        //не используется
  int InDigitalIn(I7000_SerialPort *,
                   unsigned char addr,        //адрес модуля
                   unsigned char crc_enable,  //признак вычисления/проверки контрольной суммы
                   unsigned short module_id,  //идентификатор модуля
                   unsigned &result);         //4 - х байтовый результат
  //---------- Digital Output --------------------------
  int OutDigitalOut(I7000_SerialPort *,
                   unsigned char addr,        //адрес модуля
                   unsigned char crc_enable,  //признак вычисления/проверки контрольной суммы
                   unsigned short module_id,  //идентификатор модуля
                   unsigned out);             //значения выходов
  int InDigitalOut(I7000_SerialPort *,
                   unsigned char addr,        //адрес модуля
                   unsigned char crc_enable,  //признак вычисления/проверки контрольной суммы
                   unsigned short module_id,  //идентификатор модуля
                   unsigned &result);         //4 - х байтовый результат
  //---------- Set Digital Output --------------------------
  int OutSetDigitalOut(I7000_SerialPort *,
                   unsigned char addr,        //адрес модуля
                   unsigned char crc_enable,  //признак вычисления/проверки контрольной суммы
                   unsigned short module_id,  //идентификатор модуля
                   unsigned out);             //значения выходов
  int InSetDigitalOut(I7000_SerialPort *,
                   unsigned char addr,        //адрес модуля
                   unsigned char crc_enable,  //признак вычисления/проверки контрольной суммы
                   unsigned short module_id,  //идентификатор модуля
                   unsigned &result);         //4 - х байтовый результат  
  //---------- Read Digital IO status ----------------------
  int OutGetDigitalIOStatus(I7000_SerialPort *,
                   unsigned char addr,        //адрес модуля
                   unsigned char crc_enable,  //признак вычисления/проверки контрольной суммы
                   unsigned short module_id,  //идентификатор модуля
                   unsigned out);             //значения выходов
  int InGetDigitalIOStatus(I7000_SerialPort *,
                   unsigned char addr,        //адрес модуля
                   unsigned char crc_enable,  //признак вычисления/проверки контрольной суммы
                   unsigned short module_id,  //идентификатор модуля
                   unsigned &result);         //4 - х байтовый результат  
  //-------- Read Config Status ----------------------------
  int OutReadConfig(I7000_SerialPort *,        
                    unsigned char addr,       //адрес модуля
                    unsigned char crc_enable, //признак вычисления/проверки контрольной суммы
                    unsigned short module_id, //идентификатор модуля
                    unsigned not_used);       //  
  int InReadConfig(I7000_SerialPort *,
                   unsigned char addr,       //адрес модуля
                   unsigned char crc_enable, //признак вычисления/проверки контрольной суммы
                   unsigned short module_id, //идентификатор модуля
                   unsigned &cfg);           //конфигурация
  // cfg[0]:   module Range Code
  // cfg[1]:   module baudrate
  // cfg[2]:   module data format
  //-------- Set Config  ------------------------------------
  int OutSetConfig(I7000_SerialPort *,        
                    unsigned char addr,       //адрес модуля
                    unsigned char crc_enable, //признак вычисления/проверки контрольной суммы
                    unsigned short module_id, //идентификатор модуля
                    unsigned cfg);            //конфигурация
  // cfg[0]:   new module adddress
  // cfg[1]:   module Range Code
  // cfg[2]:   module baudrate
  // cfg[3]:   module data format
  int InSetConfig(I7000_SerialPort *,
                   unsigned char addr,       //адрес модуля
                   unsigned char crc_enable, //признак вычисления/проверки контрольной суммы
                   unsigned short module_id, //идентификатор модуля
                   unsigned &result);        //результат операции
  //-------- Read Reset Status ------------------------------
  int OutReadResetStatus(I7000_SerialPort *,        
                    unsigned char addr,       //адрес модуля
                    unsigned char crc_enable, //признак вычисления/проверки контрольной суммы
                    unsigned short module_id, //идентификатор модуля
                    unsigned not_used);       //
  int InReadResetStatus(I7000_SerialPort *,
                   unsigned char addr,       //адрес модуля
                   unsigned char crc_enable, //признак вычисления/проверки контрольной суммы
                   unsigned short module_id, //идентификатор модуля
                   unsigned &result);        //результат операции

  /*********** Host Watchdog Commands Set **************************************************/
  //---------Широковещательный пакет "HOST Ok",  ~** ---------
  int HostIsOK(I7000_SerialPort *,unsigned char crc_enable);
  int HostIsOK(I7000_SerialPort *,        
                    unsigned char addr,       //адрес модуля
                    unsigned char crc_enable, //признак вычисления/проверки контрольной суммы
                    unsigned short module_id, //идентификатор модуля
                    unsigned not_used);       //
  //-------- Read Module Status ------------------------------
  int OutReadModuleStatus(I7000_SerialPort *,        
                    unsigned char addr,       //адрес модуля
                    unsigned char crc_enable, //признак вычисления/проверки контрольной суммы
                    unsigned short module_id, //идентификатор модуля
                    unsigned not_used);       //
  int InReadModuleStatus(I7000_SerialPort *,
                   unsigned char addr,       //адрес модуля
                   unsigned char crc_enable, //признак вычисления/проверки контрольной суммы
                   unsigned short module_id, //идентификатор модуля
                   unsigned &result);        //результат операции
  //-------- Reset Module Status ------------------------------
  int OutResetModuleStatus(I7000_SerialPort *,        
                    unsigned char addr,       //адрес модуля
                    unsigned char crc_enable, //признак вычисления/проверки контрольной суммы
                    unsigned short module_id, //идентификатор модуля
                    unsigned not_used);       //
  int InResetModuleStatus(I7000_SerialPort *,
                   unsigned char addr,       //адрес модуля
                   unsigned char crc_enable, //признак вычисления/проверки контрольной суммы
                   unsigned short module_id, //идентификатор модуля
                   unsigned &result);        //результат операции
  //-------- Read Watchdog Timeout Value-----------------------
  int OutReadWatchdogTimeoutValue(I7000_SerialPort *,        
                    unsigned char addr,       //адрес модуля
                    unsigned char crc_enable, //признак вычисления/проверки контрольной суммы
                    unsigned short module_id, //идентификатор модуля
                    unsigned not_used);       //
  int InReadWatchdogTimeoutValue(I7000_SerialPort *,
                   unsigned char addr,       //адрес модуля
                   unsigned char crc_enable, //признак вычисления/проверки контрольной суммы
                   unsigned short module_id, //идентификатор модуля
                   unsigned &result);        //результат операции
  //-------- Set Watchdog Timeout Value-----------------------
  int OutSetWatchdogTimeoutValue(I7000_SerialPort *,        
                    unsigned char addr,       //адрес модуля
                    unsigned char crc_enable, //признак вычисления/проверки контрольной суммы
                    unsigned short module_id, //идентификатор модуля
                    unsigned value);          //
  int InSetWatchdogTimeoutValue(I7000_SerialPort *,
                   unsigned char addr,       //адрес модуля
                   unsigned char crc_enable, //признак вычисления/проверки контрольной суммы
                   unsigned short module_id, //идентификатор модуля
                   unsigned &result);        //результат операции
  /*
protected:
private:
};*/
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif //I7000_proto