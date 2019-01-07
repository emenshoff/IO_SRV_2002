/***********************************************************
               created by M.E.N
***********************************************************/

#include <io_srv_dump.h>
#include <I7000_crc.h>
#include <I7000_proto.h>
#include <I7000_serial.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

int /*I7000_Proto::*/OutDigitalIn(I7000_SerialPort *Port,
                   unsigned char addr,        //адрес модуля
                   unsigned char crc_enable,   //признак вычисления/проверки контрольной суммы
                   unsigned short module_id,  //идентификатор модуля 
                   unsigned not_used)        //не используется
{
  int len;
  char data[6];
  switch(module_id)
  {
    case 7041:
      len = 3;
      data[0] = '@';
      data[1] = HexToAscii((addr>>4)&0x0f);
      data[2] = HexToAscii(addr&0x0f);
      if(crc_enable)
      {
        data[5] = 0x0d;
        CRC((unsigned char*)data,3,(unsigned char*)data+3);
        Port->Send((unsigned char*)data,sizeof(data));
      }
      else
      {
        data[3] = 0x0d;
        Port->Send((unsigned char*)data,4);
      }      
      break;
    default:
      break;
  }
  return OK;
}
//----------------------------------------------------------------
int /*I7000_Proto::*/InDigitalIn(I7000_SerialPort *Port,
                   unsigned char addr,        //адрес модуля
                   unsigned char crc_enable,  //признак вычисления/проверки контрольной суммы
                   unsigned short module_id,  //идентификатор модуля
                   unsigned &result)          //4 - х байтовый результат
{
  unsigned len;
  unsigned char in_address = 0;
  const unsigned char *data=Port->Recive(len);
  result = 0;
  if(!len)
    return READ_ERROR;
  if(data[0] == '?')
    return WRONG_COMMAND;
  else if((char)data[0] == '>')
  {    
    if(crc_enable)
    {
      if(len != 8)
        return WRONG_PACKET;
      if(!CheckCRC(data,len-1))
        return CRC_ERROR;//CrcError();
    }
    else if(len !=6)
      return WRONG_PACKET;
    switch(module_id)
    {
      case 7041:
        result = (unsigned)((AsciiToHex(data[1])<<12)
                  | (AsciiToHex(data[2])<<8)
                  | (AsciiToHex(data[3])<<4)
                  | AsciiToHex(data[4]));
        break;
      default:
        return WRONG_COMMAND;
    }
  }  
  return OK;
}
//----------------------------------------------------------------
int /*I7000_Proto::*/OutDigitalOut(I7000_SerialPort *Port,
                   unsigned char addr,        //адрес модуля
                   unsigned char crc_enable,  //признак вычисления/проверки контрольной суммы
                   unsigned short module_id,  //идентификатор модуля
                   unsigned out)             //значения выходов
{
  char data[10];
  data[0] = '#';
  data[1] = HexToAscii((addr>>4)&0x0f);
  data[2] = HexToAscii(addr&0x0f);
  switch(module_id)
  {
    case 7042:
      data[3] = HexToAscii((out>>12)&0x0f);
      data[4] = HexToAscii((out>>8)&0x0f);
      data[5] = HexToAscii((out>>4)&0x0f);
      data[6] = HexToAscii(out&0x0f);
      break;
    default:
      return WRONG_COMMAND;
  }
  if(crc_enable)
  {
    data[9] = 0x0d;
    CRC((unsigned char*)data,7,(unsigned char*)data+7);
    Port->Send((unsigned char*)data,sizeof(data));
  }
  else
  {
    data[7] = 0x0d;
    Port->Send((unsigned char*)data,8);
  }
  return OK;
}
//----------------------------------------------------------------
int /*I7000_Proto::*/InDigitalOut(I7000_SerialPort *Port,
                   unsigned char addr,        //адрес модуля
                   unsigned char crc_enable,  //признак вычисления/проверки контрольной суммы
                   unsigned short module_id, //идентификатор модуля
                   unsigned &result)         //4 - х байтовый результат
{
  unsigned len;
  const unsigned char *data = Port->Recive(len);
  if(!len)
    return READ_ERROR;
  if(data[0] == '?')
    return WRONG_COMMAND;
  else if((char)data[0] == '!')
    return IGNORED_COMMAND;
  else if((char)data[0] == '>')
  {
    if(crc_enable)
    {
      if(len != 4)
        return WRONG_PACKET;
      if(!CheckCRC(data,len-1))
         return CRC_ERROR;
    }
    else if(len !=2)
      return WRONG_PACKET;
    return result = OK;
  }  
  return WRONG_PACKET;
}
//----------------------------------------------------------------
int /*I7000_Proto::*/OutSetDigitalOut(I7000_SerialPort *Port,
                   unsigned char addr,        //адрес модуля
                   unsigned char crc_enable,  //признак вычисления/проверки контрольной суммы
                   unsigned short module_id,  //идентификатор модуля
                   unsigned out)             //значения выходов
{
  unsigned len;
  char data[10];
  data[0] = '@';
  data[1] = HexToAscii((addr>>4)&0x0f);
  data[2] = HexToAscii(addr&0x0f);
  switch(module_id)
  {
    case 7042: case 7043:
      data[3] = HexToAscii((out>>12)&0x0f);
      data[4] = HexToAscii((out>>8)&0x0f);
      data[5] = HexToAscii((out>>4)&0x0f);
      data[6] = HexToAscii(out&0x0f);
      len = 7;
      break;
    case 7044: case 7050: case 7065: case 7066:
      data[3] = HexToAscii((out>>4)&0x0f);
      data[4] = HexToAscii((out>>0)&0x0f);
      len = 5;
      break;
    case 7060: case 7063:
      data[3] = HexToAscii((out>>0)&0x0f);
      len = 4;
    default:
      return WRONG_COMMAND;
  }
  if(crc_enable)
  {
    data[len+2] = 0x0d;
    CRC((unsigned char*)data,len,(unsigned char*)data+len);
    Port->Send((unsigned char*)data,len+3);
  }
  else
  {
    data[len] = 0x0d;
    Port->Send((unsigned char*)data,len+1);
  }
  return OK;
}
//----------------------------------------------------------------
int /*I7000_Proto::*/InSetDigitalOut(I7000_SerialPort *Port,
                   unsigned char addr,        //адрес модуля
                   unsigned char crc_enable,  //признак вычисления/проверки контрольной суммы
                   unsigned short module_id,  //идентификатор модуля
                   unsigned &result)         //4 - х байтовый результат          
{
  return InDigitalOut(Port,addr,crc_enable,module_id,result);
}
//----------------------------------------------------------------
int /*I7000_Proto::*/OutReadConfig(I7000_SerialPort *Port,        
                    unsigned char addr,       //адрес модуля
                    unsigned char crc_enable, //признак вычисления/проверки контрольной суммы
                    unsigned short module_id, //идентификатор модуля
                    unsigned cfg)             //конфигурация
{
  char data[7];
  data[0] = '$';
  data[1] = HexToAscii((addr>>4)&0x0f);
  data[2] = HexToAscii(addr&0x0f);
  data[3] = '2';
  if(crc_enable)
  {
    data[6] = 0x0d;
    CRC((unsigned char*)data,4,(unsigned char*)data+4);
    Port->Send((unsigned char*)data,sizeof(data));
  }
  else
  {
    data[4] = 0x0d;
    Port->Send((unsigned char*)data,5);
  }   
  return OK;
}
//----------------------------------------------------------------
int /*I7000_Proto::*/OutGetDigitalIOStatus(I7000_SerialPort *Port,
                   unsigned char addr,        //адрес модуля
                   unsigned char crc_enable,  //признак вычисления/проверки контрольной суммы
                   unsigned short module_id,  //идентификатор модуля
                   unsigned out)              //значения выходов
{
  char data[7];
  data[0] = '$';
  data[1] = HexToAscii((addr>>4)&0x0f);
  data[2] = HexToAscii(addr&0x0f);
  data[3] = '6';
  if(crc_enable)
  {
    data[6] = 0x0d;
    CRC((unsigned char*)data,4,(unsigned char*)data+4);
    Port->Send((unsigned char*)data,sizeof(data));
  }
  else
  {
    data[4] = 0x0d;
    Port->Send((unsigned char*)data,5);
  }
  return OK;
}
//----------------------------------------------------------------
int /*I7000_Proto::*/InGetDigitalIOStatus(I7000_SerialPort *Port,
                   unsigned char addr,        //адрес модуля
                   unsigned char crc_enable,  //признак вычисления/проверки контрольной суммы
                   unsigned short module_id,  //идентификатор модуля
                   unsigned &result)          //4 - х байтовый результат  
{
  unsigned len;
  unsigned char in_address = 0;
  const unsigned char *data=Port->Recive(len);
  result = 0;
  if(!len)
    return READ_ERROR;
  if(data[0] == '?')
    return WRONG_COMMAND;
  else if((char)data[0] == '!')
  {    
    if(crc_enable)
    {
      if(len != 10)
        return WRONG_PACKET;
      if(!CheckCRC(data,len-1))
        return CRC_ERROR;//CrcError();
    }
    else if(len !=8)
      return WRONG_PACKET;
    switch(module_id)
    {
      case 7041: case 7042:
        result = (unsigned)((AsciiToHex(data[1])<<12)
                  | (AsciiToHex(data[2])<<8)
                  | (AsciiToHex(data[3])<<4)
                  | AsciiToHex(data[4]));
        break;
      default:
        return WRONG_COMMAND;
    }
  }  
  return OK;
}
//----------------------------------------------------------------
int /*I7000_Proto::*/InReadConfig(I7000_SerialPort *Port,
                   unsigned char addr,       //адрес модуля                  
                   unsigned char crc_enable, //признак вычисления/проверки контрольной суммы
                   unsigned short module_id, //идентификатор модуля
                   unsigned &cfg)
{
  unsigned len;
  unsigned char in_address = 0;
  const unsigned char *data = Port->Recive(len);
  if(data[0] == '?')
    return WRONG_COMMAND;
  else if(data[0] == '!')
  {
    in_address = ((AsciiToHex(data[1])<<4) | AsciiToHex(data[2]));
    if(addr != in_address)
      return WRONG_ADDR;
    if(crc_enable)
    {
      if(len != 12)
        return WRONG_PACKET;
      if(!CheckCRC(data,len-1))
        return CRC_ERROR;
    }
    else if(len !=10)
      return WRONG_PACKET;
    unsigned char type_code = ((AsciiToHex(data[3])<<4) & AsciiToHex(data[4]));
    unsigned char baudrate  = ((AsciiToHex(data[5])<<4) & AsciiToHex(data[6]));
    unsigned char data_format = ((AsciiToHex(data[7])<<4) & AsciiToHex(data[8]));
    cfg = (unsigned)((type_code<<16) & (baudrate<<8) & (data_format));
    return OK;
  }
  return WRONG_PACKET;
}
//----------------------------------------------------------------
int /*I7000_Proto::*/OutSetConfig(I7000_SerialPort *Port,        
                    unsigned char addr,       //адрес модуля
                    unsigned char crc_enable, //признак вычисления/проверки контрольной суммы
                    unsigned short module_id, //идентификатор модуля
                    unsigned cfg)             //конфигурация
  // cfg[0]:   new module adddress
  // cfg[1]:   module Range Code
  // cfg[2]:   module baudrate
  // cfg[3]:   module data format
{
  char data[14];
  unsigned char new_addr = cfg&0xff;
  unsigned char type_code = (cfg>>8)&0xff;
  unsigned char baudrate = (cfg>>16)&0xff;
  unsigned char data_format = (cfg>>24)&0xff;
  data[0] = '%';
  data[1] = HexToAscii((addr>>4)&0x0f);
  data[2] = HexToAscii(addr&0x0f);
  data[3] = HexToAscii((new_addr>>4)&0x0f);
  data[4] = HexToAscii(new_addr&0x0f);
  data[5] = HexToAscii((type_code>>4)&0x0f);
  data[6] = HexToAscii(type_code&0x0f);
  data[7] = HexToAscii((baudrate>>4)&0x0f);
  data[8] = HexToAscii(baudrate&0x0f);
  data[9] = HexToAscii((data_format>>4)&0x0f);
  data[10] = HexToAscii(data_format&0x0f);
  if(crc_enable)
  {
    data[13] = 0x0d;
    CRC((unsigned char*)data,11,(unsigned char*)data+11);
    Port->Send((unsigned char*)data,sizeof(data));
  }
  else
  {
    data[11] = 0x0d;
    Port->Send((unsigned char*)data,12);
  }
  return OK;
}
//----------------------------------------------------------------
int /*I7000_Proto::*/InSetConfig(I7000_SerialPort *Port,
                   unsigned char addr,       //адрес модуля
                   unsigned char crc_enable, //признак вычисления/проверки контрольной суммы
                   unsigned short module_id, //идентификатор модуля
                   unsigned &result)         //результат операции
{
  unsigned len;
  unsigned char in_address = 0;
  const unsigned char *data = Port->Recive(len);
  if(data[0] == '?')
    return WRONG_COMMAND;
  else if(data[0] == '!')
  {
    in_address = ((AsciiToHex(data[1])<<4) | AsciiToHex(data[2]));
    if(addr != in_address)
      return WRONG_ADDR;
    if(crc_enable)
    {
      if(len != 6)
        return WRONG_PACKET;
      if(!CheckCRC(data,len-1))
        return CRC_ERROR;
    }
    else if(len !=4)
      return WRONG_PACKET;
    return OK;
  }
  return WRONG_PACKET;
}
//----------------------------------------------------------------
int /*I7000_Proto::*/OutReadResetStatus(I7000_SerialPort *Port,        
                    unsigned char addr,       //адрес модуля
                    unsigned char crc_enable, //признак вычисления/проверки контрольной суммы
                    unsigned short module_id, //идентификатор модуля
                    unsigned not_used)        //
{
  char data[7];
  data[0] = '$';
  data[1] = HexToAscii((addr>>4)&0x0f);
  data[2] = HexToAscii(addr&0x0f);
  data[3] = '5';
  if(crc_enable)
  {
    data[6] = 0x0d;
    CRC((unsigned char*)data,4,(unsigned char*)data+4);
    Port->Send((unsigned char*)data,sizeof(data));
  }
  else
  {
    data[4] = 0x0d;
    Port->Send((unsigned char*)data,5);
  }
  return OK;
}
//----------------------------------------------------------------
int /*I7000_Proto::*/InReadResetStatus(I7000_SerialPort *Port,
                   unsigned char addr,       //адрес модуля
                   unsigned char crc_enable, //признак вычисления/проверки контрольной суммы
                   unsigned short module_id, //идентификатор модуля
                   unsigned &result)         //результат операции
{
  unsigned len;
  unsigned char in_address = 0;
  const unsigned char *data = Port->Recive(len);
  if(data[0] == '?')
    return WRONG_COMMAND;
  else if(data[0] == '!')
  {
    in_address = ((AsciiToHex(data[1])<<4) | AsciiToHex(data[2]));
    if(addr != in_address)
      return WRONG_ADDR;
    if(crc_enable)
    {
      if(len != 7)
        return WRONG_PACKET;
      if(!CheckCRC(data,len-1))
        return CRC_ERROR;
    }
    else if(len !=5)
      return WRONG_PACKET;
    result = (data[3] == '1' ? 1 : 0);
    return OK;
  }
  return WRONG_PACKET;
}
//----------------------------------------------------------------
int /*I7000_Proto::*/HostIsOK(I7000_SerialPort *Port,
                          unsigned char crc_enable)
{
  char data[6];
  data[0] = '~';
  data[1] = '*';
  data[2] = '*';
  if(crc_enable)
  {
    data[5] = 0x0d;
    CRC((unsigned char*)data,3,(unsigned char*)data+3);
    Port->Send((unsigned char*)data,sizeof(data));
  }
  else
  {
    data[3] = 0x0d;
    Port->Send((unsigned char*)data,4);
  }
  return OK;
}
//----------------------------------------------------------------
int /*I7000_Proto::*/HostIsOK(I7000_SerialPort *Port,        
                    unsigned char addr,       //адрес модуля
                    unsigned char crc_enable, //признак вычисления/проверки контрольной суммы
                    unsigned short module_id, //идентификатор модуля
                    unsigned not_used)       //
{
  return HostIsOK(Port,crc_enable);
}
//----------------------------------------------------------------
int /*I7000_Proto::*/OutReadModuleStatus(I7000_SerialPort *Port,        
                    unsigned char addr,       //адрес модуля
                    unsigned char crc_enable, //признак вычисления/проверки контрольной суммы
                    unsigned short module_id, //идентификатор модуля
                    unsigned not_used)       //
{
  char data[7];
  data[0] = '~';
  data[1] = HexToAscii((addr>>4)&0x0f);
  data[2] = HexToAscii(addr&0x0f);
  data[3] = '0';
  if(crc_enable)
  {
    data[6] = 0x0d;
    CRC((unsigned char*)data,4,(unsigned char*)data+4);
    Port->Send((unsigned char*)data,sizeof(data));
  }
  else
  {
    data[4] = 0x0d;
    Port->Send((unsigned char*)data,5);
  }
  return OK;
}
//----------------------------------------------------------------
int /*I7000_Proto::*/InReadModuleStatus(I7000_SerialPort *Port,
                   unsigned char addr,       //адрес модуля
                   unsigned char crc_enable, //признак вычисления/проверки контрольной суммы
                   unsigned short module_id, //идентификатор модуля
                   unsigned &result)         //результат операции
{
  unsigned len;
  unsigned char in_address = 0;
  const unsigned char *data = Port->Recive(len);
  if(data[0] == '?')
    return WRONG_COMMAND;
  else if(data[0] == '!')
  {
    in_address = ((AsciiToHex(data[1])<<4) | AsciiToHex(data[2]));
    if(addr != in_address)
      return WRONG_ADDR;
    if(crc_enable)
    {
      if(len != 8)
        return WRONG_PACKET;
      if(!CheckCRC(data,len-1))
        return CRC_ERROR;
    }
    else if(len !=6)
      return WRONG_PACKET;
    result = (data[4] == '4' ? 1 : 0);
    return OK;
  }
  return WRONG_PACKET;
}
//----------------------------------------------------------------
int /*I7000_Proto::*/OutResetModuleStatus(I7000_SerialPort *Port,        
                    unsigned char addr,       //адрес модуля
                    unsigned char crc_enable, //признак вычисления/проверки контрольной суммы
                    unsigned short module_id, //идентификатор модуля
                    unsigned not_used)        //
{
  char data[7];
  data[0] = '~';
  data[1] = HexToAscii((addr>>4)&0x0f);
  data[2] = HexToAscii(addr&0x0f);
  data[3] = '1';
  if(crc_enable)
  {
    data[6] = 0x0d;
    CRC((unsigned char*)data,4,(unsigned char*)data+4);
    Port->Send((unsigned char*)data,sizeof(data));
  }
  else
  {
    data[4] = 0x0d;
    Port->Send((unsigned char*)data,5);
  }
  return OK;
}
//----------------------------------------------------------------
int /*I7000_Proto::*/InResetModuleStatus(I7000_SerialPort *Port,
                   unsigned char addr,       //адрес модуля
                   unsigned char crc_enable, //признак вычисления/проверки контрольной суммы
                   unsigned short module_id, //идентификатор модуля
                   unsigned &result)         //результат операции
{
  unsigned len;
  unsigned char in_address = 0;
  const unsigned char *data = Port->Recive(len);
  if(data[0] == '?')
    return WRONG_COMMAND;
  else if(data[0] == '!')
  {
    in_address = ((AsciiToHex(data[1])<<4) | AsciiToHex(data[2]));
    if(addr != in_address)
      return WRONG_ADDR;
    if(crc_enable)
    {
      if(len != 6)
        return WRONG_PACKET;
      if(!CheckCRC(data,len-1))
        return CRC_ERROR;
    }
    else if(len !=4)
      return WRONG_PACKET;
    return result = OK;
  }
  return WRONG_PACKET;
}
//----------------------------------------------------------------
int /*I7000_Proto::*/OutReadWatchdogTimeoutValue(I7000_SerialPort *Port,        
                    unsigned char addr,       //адрес модуля
                    unsigned char crc_enable, //признак вычисления/проверки контрольной суммы
                    unsigned short module_id, //идентификатор модуля
                    unsigned not_used)        //
{
  char data[7];
  data[0] = '~';
  data[1] = HexToAscii((addr>>4)&0x0f);
  data[2] = HexToAscii(addr&0x0f);
  data[3] = '2';
  if(crc_enable)
  {
    data[6] = 0x0d;
    CRC((unsigned char*)data,4,(unsigned char*)data+4);
    Port->Send((unsigned char*)data,sizeof(data));
  }
  else
  {
    data[4] = 0x0d;
    Port->Send((unsigned char*)data,5);
  }
  return OK;
}
//----------------------------------------------------------------
int /*I7000_Proto::*/InReadWatchdogTimeoutValue(I7000_SerialPort *Port,
                   unsigned char addr,       //адрес модуля
                   unsigned char crc_enable, //признак вычисления/проверки контрольной суммы
                   unsigned short module_id, //идентификатор модуля
                   unsigned &result)         //результат операции
{
  unsigned len;
  unsigned char in_address = 0;
  const unsigned char *data = Port->Recive(len);
  result = 0;
  if(data[0] == '?')
    return WRONG_COMMAND;
  else if(data[0] == '!')
  {
    in_address = ((AsciiToHex(data[1])<<4) | AsciiToHex(data[2]));
    if(addr != in_address)
      return WRONG_ADDR;
    if(crc_enable)
    {
      if(len != 8)
        return WRONG_PACKET;
      if(!CheckCRC(data,len-1))
        return CRC_ERROR;
    }
    else if(len !=6)
      return WRONG_PACKET;
    result = (unsigned)((AsciiToHex(data[3]<<8)) & AsciiToHex(data[4]));
    return OK;
  }
  return WRONG_PACKET;
}
//----------------------------------------------------------------
int /*I7000_Proto::*/OutSetWatchdogTimeoutValue(I7000_SerialPort *Port,        
                    unsigned char addr,       //адрес модуля
                    unsigned char crc_enable, //признак вычисления/проверки контрольной суммы
                    unsigned short module_id, //идентификатор модуля
                    unsigned value)           //
{
  //установленный старший бит в value - признак разрешения HW
  //значение HW - младший байт value (0x0 - 0xff - по 0.1 с на единицу)
  char data[10];
  unsigned char bt_value = (unsigned char)value;
  data[0] = '~';
  data[1] = HexToAscii((addr>>4)&0x0f);
  data[2] = HexToAscii(addr&0x0f);
  data[3] = '3';
  data[4] = (value&0x10000000 ? '1' : '0');
  data[5] = HexToAscii((bt_value>>4)&0x0f);
  data[6] = HexToAscii(bt_value&0x0f);
  if(crc_enable)
  {
    data[9] = 0x0d;
    CRC((unsigned char*)data,7,(unsigned char*)data+7);
    Port->Send((unsigned char*)data,sizeof(data));
  }
  else
  {
    data[7] = 0x0d;
    Port->Send((unsigned char*)data,8);
  }
  return OK;
}
//----------------------------------------------------------------
int /*I7000_Proto::*/InSetWatchdogTimeoutValue(I7000_SerialPort *Port,
                   unsigned char addr,       //адрес модуля
                   unsigned char crc_enable, //признак вычисления/проверки контрольной суммы
                   unsigned short module_id, //идентификатор модуля
                   unsigned &result)         //результат операции
{
  unsigned len;
  unsigned char in_address = 0;
  const unsigned char *data = Port->Recive(len);
  if(data[0] == '?')
    return WRONG_COMMAND;
  else if(data[0] == '!')
  {
    in_address = ((AsciiToHex(data[1])<<4) | AsciiToHex(data[2]));
    if(addr != in_address)
      return WRONG_ADDR;
    if(crc_enable)
    {
      if(len != 6)
        return WRONG_PACKET;
      if(!CheckCRC(data,len-1))
        return CRC_ERROR;
    }
    else if(len !=4)
      return WRONG_PACKET;
    return result = OK;
  }
  return WRONG_PACKET;
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%