/********************************************************************
  ����� ��� ����� � �� � ��������� ����� ���������������� ���� ��
                   (��������� �������)
                     created by M.E.N.
********************************************************************/
#ifndef _kp_serial_port_
#define _kp_serial_port_

#include <bin_data.h>
#include <serial_port.h>
#include <def_crit_sec.h>
#include <list>

using namespace std;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#define BEGMARK 0x02
#define ENDMARK 0x03
#define PREFIX  0x09
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//���������������� ���� ��� ����� � �� ����� �������� (�������� ISIO-2)
class KpSerialPort : public SerialPort
{
  friend class KpControl;
#ifdef MULTI_THREADED
# ifdef WIN32
   friend DWORD WINAPI ThreadFunc(LPVOID);
# else
   friend void *ThreadFunc(void *);
# endif
#endif
public:
 KpSerialPort();
 virtual ~KpSerialPort();
 bool Ok(){return SerialPort::Ok() && state !=CH_ERROR;}
 int InQueueSize()const;
 int OutQueueSize()const;
 void StartPortPoll();
 void StopPortPoll(); 
 void Send(const unsigned char *data,unsigned len,int channel);//��������� � �������� �������  
protected:  
 //������� ��������� ������ �����:
 enum{
     INIT,        //������ ������ - ���������� ����������������
     START,       //������ ������
     READING,     //������� ������
     PREFIX_SKIP, //������� ����-���������
     CH_ERROR     //������
 };
 const BinData *Recive()const;                         //���������� �� ������� �������  
 mutable BinData Data;
 unsigned CRC(const unsigned char *p, unsigned len)const;
 void Step();     //��� ������-������ 
private:
 //��������� ��� ������� �������� � thread-�:
 void SendStep(); //��� ������
 void ReadStep(); //��� ������
 mutable list<BinData *> InQueue;
 mutable list<BinData *> OutQueue; 
 unsigned char state;               //������� ��������� ��������� ��������
 unsigned char thread_work_flag;    //���� ��� ������ ������ �� �����. ���� �������, �� ����� �����������
 char buff[0xfff];
 unsigned cnt;
#ifdef MULTI_THREADED
  mutable CRIT_SEC_DESCRIPT(in_queue_lock) 	//���������� ������ � ������� ��������
  mutable CRIT_SEC_DESCRIPT(out_queue_lock) //���������� ������ � �������� ��������
# ifdef WIN32
   HANDLE thr;
# else
   pthread_t thr;
# endif
#endif
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif