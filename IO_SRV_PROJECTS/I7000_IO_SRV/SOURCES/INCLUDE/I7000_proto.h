/***********************************************************
  ������� ��� ������������ � ������� ������� ���������
  I7000. ��� ���� ������� (��� ����������� ������) : 
  �-� ������� �������(� ��������� "Out") � �-� ������ ������
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
                   unsigned char addr,        //����� ������
                   unsigned char crc_enable,  //������� ����������/�������� ����������� �����
                   unsigned short module_id,   //������������� ������
                   unsigned not_used);        //�� ������������
  int InDigitalIn(I7000_SerialPort *,
                   unsigned char addr,        //����� ������
                   unsigned char crc_enable,  //������� ����������/�������� ����������� �����
                   unsigned short module_id,  //������������� ������
                   unsigned &result);         //4 - � �������� ���������
  //---------- Digital Output --------------------------
  int OutDigitalOut(I7000_SerialPort *,
                   unsigned char addr,        //����� ������
                   unsigned char crc_enable,  //������� ����������/�������� ����������� �����
                   unsigned short module_id,  //������������� ������
                   unsigned out);             //�������� �������
  int InDigitalOut(I7000_SerialPort *,
                   unsigned char addr,        //����� ������
                   unsigned char crc_enable,  //������� ����������/�������� ����������� �����
                   unsigned short module_id,  //������������� ������
                   unsigned &result);         //4 - � �������� ���������
  //---------- Set Digital Output --------------------------
  int OutSetDigitalOut(I7000_SerialPort *,
                   unsigned char addr,        //����� ������
                   unsigned char crc_enable,  //������� ����������/�������� ����������� �����
                   unsigned short module_id,  //������������� ������
                   unsigned out);             //�������� �������
  int InSetDigitalOut(I7000_SerialPort *,
                   unsigned char addr,        //����� ������
                   unsigned char crc_enable,  //������� ����������/�������� ����������� �����
                   unsigned short module_id,  //������������� ������
                   unsigned &result);         //4 - � �������� ���������  
  //---------- Read Digital IO status ----------------------
  int OutGetDigitalIOStatus(I7000_SerialPort *,
                   unsigned char addr,        //����� ������
                   unsigned char crc_enable,  //������� ����������/�������� ����������� �����
                   unsigned short module_id,  //������������� ������
                   unsigned out);             //�������� �������
  int InGetDigitalIOStatus(I7000_SerialPort *,
                   unsigned char addr,        //����� ������
                   unsigned char crc_enable,  //������� ����������/�������� ����������� �����
                   unsigned short module_id,  //������������� ������
                   unsigned &result);         //4 - � �������� ���������  
  //-------- Read Config Status ----------------------------
  int OutReadConfig(I7000_SerialPort *,        
                    unsigned char addr,       //����� ������
                    unsigned char crc_enable, //������� ����������/�������� ����������� �����
                    unsigned short module_id, //������������� ������
                    unsigned not_used);       //  
  int InReadConfig(I7000_SerialPort *,
                   unsigned char addr,       //����� ������
                   unsigned char crc_enable, //������� ����������/�������� ����������� �����
                   unsigned short module_id, //������������� ������
                   unsigned &cfg);           //������������
  // cfg[0]:   module Range Code
  // cfg[1]:   module baudrate
  // cfg[2]:   module data format
  //-------- Set Config  ------------------------------------
  int OutSetConfig(I7000_SerialPort *,        
                    unsigned char addr,       //����� ������
                    unsigned char crc_enable, //������� ����������/�������� ����������� �����
                    unsigned short module_id, //������������� ������
                    unsigned cfg);            //������������
  // cfg[0]:   new module adddress
  // cfg[1]:   module Range Code
  // cfg[2]:   module baudrate
  // cfg[3]:   module data format
  int InSetConfig(I7000_SerialPort *,
                   unsigned char addr,       //����� ������
                   unsigned char crc_enable, //������� ����������/�������� ����������� �����
                   unsigned short module_id, //������������� ������
                   unsigned &result);        //��������� ��������
  //-------- Read Reset Status ------------------------------
  int OutReadResetStatus(I7000_SerialPort *,        
                    unsigned char addr,       //����� ������
                    unsigned char crc_enable, //������� ����������/�������� ����������� �����
                    unsigned short module_id, //������������� ������
                    unsigned not_used);       //
  int InReadResetStatus(I7000_SerialPort *,
                   unsigned char addr,       //����� ������
                   unsigned char crc_enable, //������� ����������/�������� ����������� �����
                   unsigned short module_id, //������������� ������
                   unsigned &result);        //��������� ��������

  /*********** Host Watchdog Commands Set **************************************************/
  //---------����������������� ����� "HOST Ok",  ~** ---------
  int HostIsOK(I7000_SerialPort *,unsigned char crc_enable);
  int HostIsOK(I7000_SerialPort *,        
                    unsigned char addr,       //����� ������
                    unsigned char crc_enable, //������� ����������/�������� ����������� �����
                    unsigned short module_id, //������������� ������
                    unsigned not_used);       //
  //-------- Read Module Status ------------------------------
  int OutReadModuleStatus(I7000_SerialPort *,        
                    unsigned char addr,       //����� ������
                    unsigned char crc_enable, //������� ����������/�������� ����������� �����
                    unsigned short module_id, //������������� ������
                    unsigned not_used);       //
  int InReadModuleStatus(I7000_SerialPort *,
                   unsigned char addr,       //����� ������
                   unsigned char crc_enable, //������� ����������/�������� ����������� �����
                   unsigned short module_id, //������������� ������
                   unsigned &result);        //��������� ��������
  //-------- Reset Module Status ------------------------------
  int OutResetModuleStatus(I7000_SerialPort *,        
                    unsigned char addr,       //����� ������
                    unsigned char crc_enable, //������� ����������/�������� ����������� �����
                    unsigned short module_id, //������������� ������
                    unsigned not_used);       //
  int InResetModuleStatus(I7000_SerialPort *,
                   unsigned char addr,       //����� ������
                   unsigned char crc_enable, //������� ����������/�������� ����������� �����
                   unsigned short module_id, //������������� ������
                   unsigned &result);        //��������� ��������
  //-------- Read Watchdog Timeout Value-----------------------
  int OutReadWatchdogTimeoutValue(I7000_SerialPort *,        
                    unsigned char addr,       //����� ������
                    unsigned char crc_enable, //������� ����������/�������� ����������� �����
                    unsigned short module_id, //������������� ������
                    unsigned not_used);       //
  int InReadWatchdogTimeoutValue(I7000_SerialPort *,
                   unsigned char addr,       //����� ������
                   unsigned char crc_enable, //������� ����������/�������� ����������� �����
                   unsigned short module_id, //������������� ������
                   unsigned &result);        //��������� ��������
  //-------- Set Watchdog Timeout Value-----------------------
  int OutSetWatchdogTimeoutValue(I7000_SerialPort *,        
                    unsigned char addr,       //����� ������
                    unsigned char crc_enable, //������� ����������/�������� ����������� �����
                    unsigned short module_id, //������������� ������
                    unsigned value);          //
  int InSetWatchdogTimeoutValue(I7000_SerialPort *,
                   unsigned char addr,       //����� ������
                   unsigned char crc_enable, //������� ����������/�������� ����������� �����
                   unsigned short module_id, //������������� ������
                   unsigned &result);        //��������� ��������
  /*
protected:
private:
};*/
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif //I7000_proto