/**************************************************************************
    ����� ��� ������ � ������������� �����-������ ��������� i7000
     (������ �� �����, ���� �� ����������� 0x0D, ����� ������ � �����).
     ������� �� �������������, ��� ��� ������ I7000 �������� ������
     � ���������� ������ ������-��������...
      ������������ ������ ������� �� ������ ��������� 0xfff ����)
          created by M.E.N.
**************************************************************************/
#ifndef _i7000_serial_port
#define _i7000_serial_port

#include <math.h>
#include <bin_data.h>
#include <I7000_crc.h>
#include <serial_port.h>
#include <I7000_events_cash.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class I7000_SerialPort: public SerialPort
{
public:
 I7000_SerialPort();
 virtual void Step();                             // ���� ������-������
 bool Init(const char *name,int speed,unsigned buff_size=0xf);
 unsigned DelayVal()const{return delay_val;}
 const unsigned char *Recive(unsigned &sz);
 virtual void Send(const unsigned char *,unsigned);
 bool CanUse()const //����������� ������������� ����� ��� ������.������...
 {
   return ((EventsCash.TimeInDbl() - last_access_time)/1e3 > delay_val);
 } 
protected:
 void ReadStep();
 void SendStep();
 enum{BEGIN,READING,READED};              //��������� ��������� 
 double last_access_time;                 //����� ���������� ������� � �����
 unsigned send_len;
 unsigned delay_val;                      //�����, �����. ��� ������� 10 ���� �� ������ �������� (��)
 unsigned char state;                     //������� ��������� 
 unsigned char buff_in[0xfff];            //����� ��� ������ �������
 unsigned char buff_out[0xfff];           //����� ��� ������ �������
 unsigned char *read_pos;                 //������� � ������ ������ 
private:
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
extern char HexToAscii(const short int hex);
extern char AsciiToHex(const char cASCII);
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif