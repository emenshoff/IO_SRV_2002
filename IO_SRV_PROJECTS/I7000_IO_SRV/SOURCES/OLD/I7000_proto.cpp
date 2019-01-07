/***********************************************************
               created by M.E.N
***********************************************************/

#include <I7000_crc.h>
#include <I7000_proto.h>
#include <I7000_serial.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
I7000_Proto::I7000_Proto()
{
}
//----------------------------------------------------------------
I7000_Proto::~I7000_Proto()
{
}
//----------------------------------------------------------------
int OutDigitalIn(SerialPortI7000 *Port,
                   unsigned char addr,        //адрес модуля
                   unsigned char crc_enable,   //признак вычисления/проверки контрольной суммы
                   unsigned short module_id)  //идентификатор модуля 
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
int InDigitalIn(SerialPortI7000 *Port,
                   unsigned char addr,        //адрес модуля
                   unsigned char crc_enable,   //признак вычисления/проверки контрольной суммы
                   unsigned short module_id,   //идентификатор модуля
                   unsigned &result)          //4 - х байтовый результат
{
  unsigned len;
  unsigned char in_address;
  const unsigned char *data=Port->Recive(len);
  result = 0;
  if(!len)
    return READ_ERROR;
  if(data[0] == '?')
    return WRONG_COMMAND;
  else if((char)data[0] == '>')
  {
    in_address = ((AsciiToHex(data[1])<<4) & AsciiToHex(data[2]));
    if(addr != in_address)
      return WRONG_ADDR;
    if(crc_enable)
    {
      if(len != 8)
        return WRONG_PACKET;
      if(!CheckCRC(data,len-3))
        return CRC_ERROR;
    }
    else if(len !=6)
      return WRONG_PACKET;
    switch(module_id)
    {
      case 7041:        
        result = (unsigned)((AsciiToHex(data[1]<<8)) & AsciiToHex(data[2]));        
        break;
      default:
        return WRONG_COMMAND;
    }
  }  
  return OK;
}
//----------------------------------------------------------------
int OutDigitalOut(SerialPortI7000 *Port,
                   unsigned char addr,        //адрес модуля
                   unsigned char crc_enable,   //признак вычисления/проверки контрольной суммы
                   unsigned short module_id,   //идентификатор модуля
                   unsigned out)             //значения выходов
{
  char data[10];
  data[0] = '@';
  data[1] = HexToAscii((addr>>4)&0x0f);
  data[2] = HexToAscii(addr&0x0f);
  switch(module_id)
  {
    case 7042:
      data[3] = HexToAscii((out>>16)&0x0f);
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
int InDigitalOut(SerialPortI7000 *Port,
                   unsigned char addr,        //адрес модуля
                   unsigned char crc_enable,   //признак вычисления/проверки контрольной суммы
                   unsigned short module_id)  //идентификатор модуля
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
      if(!CheckCRC(data,len-3))
         return CRC_ERROR;
    }
    else if(len !=2)
      return WRONG_PACKET;
    return OK;
  }
  else
    return WRONG_PACKET;
}
/*
//----------------------------------------------------------------
int I7000_Proto::HostIsOK(SerialPortI7000 *Port,unsigned char crc_enable)
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
  return 1;
}
//----------------------------------------------------------------
int I7000_Proto::OutReadModuleResetStatus(SerialPortI7000 *Port,
                               unsigned char addr,        
                               unsigned char crc_enable)
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
    data[6] = 0x0d;
    Port->Send((unsigned char*)data,5);
  }  
  return 1;
}
//----------------------------------------------------------------
int I7000_Proto::InReadModuleResetStatus(SerialPortI7000 *Port,
                              unsigned char addr,
                              unsigned char crc_enable,
                              unsigned char &reset_status)
{
  unsigned char in_address;
  const BinData * pk=Port->Recive();
  if(!pk->Len())
    return READ_ERROR;  
  const unsigned char *data=(unsigned char*)*pk;
  if(data[0] == '?')
    return WRONG_COMMAND;
  else if(data[0] == '!' && pk->Len() >= 5)
  {
    in_address = ((AsciiToHex(data[1])<<4) & AsciiToHex(data[2]));
    if(addr != in_address)
      return WRONG_ADDR;
    if(crc_enable)
    {
      if(!CheckCRC(data,pk->Len()-3)
        return CRC_ERROR;
    }
    reset_status = data[3];
    return OK;
  }
  return WRONG_PACKET;
}
//----------------------------------------------------------------
int I7000_Proto::OutSetHostWatchdog(SerialPortI7000 *Port,
                          unsigned char addr,
                          unsigned char crc_enable,
                          unsigned char enable_host_watchdog,
                          unsigned char timeout_value)
{
  char data[10];
  data[0] = '~';
  data[1] = HexToAscii((addr>>4)&0x0f);
  data[2] = HexToAscii(addr&0x0f);
  data[3] = '3';
  data[4] = enable_host_watchdog!=0 ? 1 : 0;
  data[5] = HexToAscii((timeout_value>>4)&0x0f);
  data[6] = HexToAscii(timeout_value&0x0f);
  if(crc_enable)
  {
    CRC(data,7,data+7);
    data[9] = 0x0d;
    Port->Send(data,10);
  }
  else
  {
    data[7] = 0x0d;
    Port->Send(data,8);
  }
  return OK;
}
//----------------------------------------------------------------
int I7000_Proto::InSetHostWatchdog(SerialPortI7000 *Port,
                                   unsigned char addr,
                                   unsigned char crc_enable)
{
  unsigned char in_address;
  const BinData * pk=Port->Recive();
  if(!pk->Len())
    return READ_ERROR;
  const unsigned char *data=(unsigned char*)*pk;
  if(data[0] == '?')
    return WRONG_COMMAND;
  else if(data[0] == '!' && pk->Len() >= 4)
  {
    in_address = ((AsciiToHex(data[1])<<4) & AsciiToHex(data[2]));
    if(addr != in_address)
      return WRONG_ADDR;
    if(crc_enable)
    {
      if(!CheckCRC(data,pk->Len()-3)
        return CRC_ERROR;
    }
    return OK;
  }
  return WRONG_PACKET;
}
//----------------------------------------------------------------
int I7000_Proto::OutReadHostWatchdog(SerialPortI7000 *Port,
                                     unsigned char addr,
                                     unsigned char crc_enable)
{
  char data[7];
  data[0] = '~';
  data[1] = HexToAscii((addr>>4)&0x0f);
  data[2] = HexToAscii(addr&0x0f);
  data[3] = '2';
  if(crc_enable)
  {
    CRC(data,4,data+4);
    data[6] = 0x0d;
    Port->Send(data,7);
  }
  else
  {
    data[4] = 0x0d;
    Port->Send(data,5);
  }
  return OK;
}
//----------------------------------------------------------------
int I7000_Proto::InReadHostWatchdog(SerialPortI7000 *Port,
                                    unsigned char addr,
                                    unsigned char crc_enable,
                                    unsigned char &val)
{
  const BinData * pk=Port->Recive();
  if(!pk->Len())
    return READ_ERROR;
  const unsigned char *data=(unsigned char*)*pk;
  if(data[0] == '?')
    return WRONG_COMMAND;
  else if(data[0] == '!' && pk->Len() >= 6)
  {
    unsigned char in_address = ((AsciiToHex(data[1])<<4) & AsciiToHex(data[2]));
    if(addr != in_address)
      return WRONG_ADDR;
    if(crc_enable)
    {
      if(!CheckCRC(data,pk->Len()-3)
        return CRC_ERROR;
    }
    val = ((AsciiToHex(data[3]<<4)) & AsciiToHex(data[4]));
    return OK;
  }
  return WRONG_PACKET;
}
//----------------------------------------------------------------
int I7000_Proto::OutReadModuleHostWatchdogStatus(SerialPortI7000 *Port,
                                                 unsigned char addr,
                                                 unsigned char crc_enable)
{
  char data[7];
  data[0] = '~';
  data[1] = HexToAscii((addr>>4)&0x0f);
  data[2] = HexToAscii(addr&0x0f);
  data[3] = '0';
  if(crc_enable)
  {
    CRC(data,4,data+4);
    data[6] = 0x0d;
    Port->Send(data,7);
  }
  else
  {
    data[4] = 0x0d;
    Port->Send(data,5);
  }
  return OK;
}
//----------------------------------------------------------------
int I7000_Proto::InReadModuleHostWatchdogStatus(SerialPortI7000 *,
                                                unsigned char addr,
                                                unsigned char crc_enable,
                                                unsigned char &status)
{
  const BinData * pk=Port->Recive();
  if(!pk->Len())
    return READ_ERROR;
  const unsigned char *data=(unsigned char*)*pk;
  if(data[0] == '?')
    return WRONG_COMMAND;
  else if(data[0] == '!' && pk->Len() >= 6)
  {
    unsigned char in_address = ((AsciiToHex(data[1])<<4) & AsciiToHex(data[2]));
    if(addr != in_address)
      return WRONG_ADDR;
    if(crc_enable)
    {
      if(!CheckCRC(data,pk->Len()-3)
        return CRC_ERROR;
    }
    status = data[4] == 4 ? 1 : 0;
    return OK;
  }
  return WRONG_PACKET;
}
//----------------------------------------------------------------
int I7000_Proto::OutResetModuleHostWatchdogStatus(SerialPortI7000 *Port,
                                     unsigned char addr,
                                     unsigned char crc_enable)
{
  char data[7];
  data[0] = '~';
  data[1] = HexToAscii((addr>>4)&0x0f);
  data[2] = HexToAscii(addr&0x0f);
  data[3] = '1';
  if(crc_enable)
  {
    CRC(data,4,data+4);
    data[6] = 0x0d;
    Port->Send(data,7);
  }
  else
  {
    data[4] = 0x0d;
    Port->Send(data,5);
  }
  return OK;
}
//----------------------------------------------------------------
int I7000_Proto::InResetModuleHostWatchdogStatus(SerialPortI7000 *Port,
                                    unsigned char addr,
                                    unsigned char crc_enable)
{
  const BinData * pk=Port->Recive();
  if(!pk->Len())
    return READ_ERROR;
  const unsigned char *data=(unsigned char*)*pk;
  if(data[0] == '?')
    return WRONG_COMMAND;
  else if(data[0] == '!' && pk->Len() >= 4)
  {
    unsigned char in_address = ((AsciiToHex(data[1])<<4) & AsciiToHex(data[2]));
    if(addr != in_address)
      return WRONG_ADDR;
    if(crc_enable)
    {
      if(!CheckCRC(data,pk->Len()-3)
        return CRC_ERROR;
    }
    return OK;
  }
  return WRONG_PACKET;
}
//----------------------------------------------------------------
int I7000_Proto::OutReadConfig(SerialPortI7000 *,
                               unsigned char addr,
                               unsigned char crc_enable)
{
  char data[7];
  data[0] = '$';
  data[1] = HexToAscii((addr>>4)&0x0f);
  data[2] = HexToAscii(addr&0x0f);
  data[3] = '2';
  if(crc_enable)
  {
    CRC(data,4,data+4);
    data[6] = 0x0d;
    Port->Send(data,7);
  }
  else
  {
    data[4] = 0x0d;
    Port->Send(data,5);
  }
  return OK;
}
//----------------------------------------------------------------
int I7000_Proto::InReadConfig(SerialPortI7000 *,
                              unsigned char addr,
                              unsigned char crc_enable,
                              unsigned char cfg[])
{
  const BinData * pk=Port->Recive();
  if(!pk->Len())
    return READ_ERROR;
  const unsigned char *data=(unsigned char*)*pk;
  if(data[0] == '?')
    return WRONG_COMMAND;
  else if(data[0] == '!' && pk->Len() >= 10)
  {
    unsigned char in_address = ((AsciiToHex(data[1])<<4) & AsciiToHex(data[2]));
    if(addr != in_address)
      return WRONG_ADDR;
    if(crc_enable)
    {
      if(!CheckCRC(data,pk->Len()-3)
        return CRC_ERROR;
    }
    cfg[0] = (AsciiToHex(data[3]<<4) & AsciiToHex(data[4]));//   module Range Code
    cfg[1] = (AsciiToHex(data[5]<<4) & AsciiToHex(data[6]));//   module baudrate
    cfg[2] = (AsciiToHex(data[7]<<) & AsciiToHex(data[8]));//   module data format
    return OK;
  }
  return WRONG_PACKET;
}
//----------------------------------------------------------------
int I7000_Proto::OutAnalogIn(SerialPortI7000 *Port,
                             unsigned char addr,
                             unsigned char crc_enable)
{
  char data[6];
  data[0] = '#';
  data[1] = HexToAscii((addr>>4)&0x0f);
  data[2] = HexToAscii(addr&0x0f);
  if(crc_enable)
  {
    CRC(data,3,data+3);
    data[5] = 0x0d;
    Port->Send(data,6);
  }
  else
  {
    data[3] = 0x0d;
    Port->Send(data,4);
  }
  return OK;
}
//----------------------------------------------------------------
!int I7000_Proto::InAnalogIn(SerialPortI7000 *Port,
               unsigned char addr,
               unsigned char crc_enable,
               unsigned char mode,
               float value[])
{
  const BinData * pk=Port->Recive();
  if(!pk->Len())
    return READ_ERROR;
  const unsigned char *data=(unsigned char*)*pk;
  if(data[0] == '>' && (mode==HEX_MODE ? pk->Len() >= 34 : pk->Len() >= ))
  {
    unsigned char in_address = ((AsciiToHex(data[1])<<4) & AsciiToHex(data[2]));
    if(addr != in_address)
      return WRONG_ADDR;
    if(crc_enable)
    {
      if(!CheckCRC(data,pk->Len()-3)
        return CRC_ERROR;
    }
    switch(mode)
    {
      case MODE_HEX:
        break;
      case MODE_ENG:
        break;
      case MODE_FSTR:
        break;
      default:
        return WRONG_PACKET;
    };
    cfg[0] = (AsciiToHex(data[3]<<4) & AsciiToHex(data[4]));//   module Range Code
    cfg[1] = (AsciiToHex(data[5]<<4) & AsciiToHex(data[6]));//   module baudrate
    cfg[2] = (AsciiToHex(data[7]<<) & AsciiToHex(data[8])); //    module data format
    return OK;
  }
  return WRONG_PACKET;
}
//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
/*int I7000_Proto::OutSetSafeValue(SerialPortI7000 *Port,
                      unsigned char addr,
                      unsigned short id,        //module ID: 0x7050/60/67/42/43/44
                      unsigned char crc_enable,
                      unsigned char value)
{
  char data[10];
  data[0] = '@';
  data[0] = 
  data[0] = 
  data[0] = 
}
//----------------------------------------------------------------
int I7000_Proto::InSetSafeValue(SerialPortI7000 *Port,
                     unsigned char addr,        //module ID: 0x7050/60/67/42/43/44
                     unsigned char crc_enable)
{
  
}*/
//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%