/********************************************************************
  ����� ��� ����� � �� � ��������� ����� ���������������� ���� ��
                     created by M.E.N.
********************************************************************/
#ifndef _kp_comm_dev_h
#define _kp_comm_dev_h

#ifndef WIN32
	#include <sys/types.h>
	#include <sys/un.h>
	#include <sys/socket.h>
	#include <termios.h>
	#include <fcntl.h>
	#include <unistd.h>
	#include <signal.h>	
	#include <sys/errno.h>
  	#include <pthread.h>
#else
	#include <windows.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <kp_types.h>
#include <stm_srv_cfg.h>
#include <kp_channel.h>
#include <bin_data.h>
#include <def_crit_sec.h>
#include <list>

using namespace std;

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#define BEGMARK 0x02
#define ENDMARK 0x03
#define PREFIX  0x09
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// ��������� ������ �� ��������
struct RawHeader
{
  unsigned char paddle[3];  //����� ����� - �� (0xff)  :)
  unsigned char address;    //����� ������
  unsigned char proto;      //��� ���������
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#define RawHeaderLen 5
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//���� ������� �� ��������:
#define PK_CFG_REQ    0xa0  //������� ����������� ������������
#define PK_CFG_SEND   0x90  //����� � �������� � �������������
#define KP_CFG_ACCEPT 0xb0  //������� ������ ������������
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//���������������� ���������� (���� �������)
class KpCommDev : public ControlObject
{
 friend class KpControl;
 friend class KpConfig;
public:
 KpCommDev(obj_d &cfg_id);
 virtual ~KpCommDev();
 bool Ok(){return ok && state !=CH_ERROR;}
 void Process();                                            //���� ���������
 void SetError(unsigned char err);                          //��������� ���������������
 bool Init();                                              //������������� ����� 
 virtual unsigned Type()const{return KP_COMM_DEV;}         //RTTI   :)
 void Send(const unsigned char *data,unsigned len,int channel);//��������� � �������� �������  
 int Speed()const{return speed;}          //���. �������� �����
 const char *Name()const{return port_name;}      //��� �����
private:
 void InitChannels();                                      //������������� �������
 int InQueueSize()const;
 int OutQueueSize()const;
 //������� ��������� ������ �����:
 enum
 {
     INIT,        //������ ������ - ���������� ����������������
     START,       //������ ������
     READING,     //������� ������
     PREFIX_SKIP, //������� ����-���������
     CH_ERROR     //������
 };
 //����� ������
 template <int SIZE=256>
 struct PortBuff
  {
    int Len;
    char Buff[SIZE];
    //
    int Size()const{return SIZE;}
    //
    void Out(int len)
    {
      if(len > 0)
        if(len <= Len)
          memmove(Buff,Buff + len,Len -= len);
    }
  };
 int SendPort(const unsigned char *data,unsigned len,int &writed); //������� � ���� ��.
 int ReadPort(unsigned char *,unsigned length,int &sended);        //������ �� ����� ��.
 const BinData *Recive()const;                         //���������� �� ������� �������
 unsigned CRC(const unsigned char *p, unsigned len)const;//���������� ����� �����
 void Step();                                          //��� ������-������  
 void SendStep(); //��� ������
 void ReadStep(); //��� ������
 void SendChannelInit(KpCnl*); //������� ������ ������������� ��� ������ ������
 KpCnl *channels[4];		       //��������� ��������
 mutable BinData Data;                                 //������� ������
 mutable list<BinData *> InQueue;                      //�������
 mutable list<BinData *> OutQueue; 
 unsigned char state;               //������� ��������� ��������� �������� 
 time_t last_init_time;             //����� ��������� ������������� ���������.
 mutable PortBuff<256> OutBuff;
 mutable PortBuff<0xfff> InBuff;
 bool Reset(const char *port_name,int spd);		//������������� (��� ����� � ��� ��������)  
 int Send(const unsigned char *data,unsigned len,unsigned &sended);//������� len ���� �� ������ data. ����� ���������� ���� - sended
 int Recive(unsigned char *,unsigned length,unsigned &readed);       //������ lenght ����. ����� ����������� - � 3-� ���������
 //------------------------------------------ 
 int port_lock;
 int skip_packets_cnt;
 //------------------------------------------ 
#ifdef WIN32
  HANDLE Port;                             
#else
  int Port;                                
#endif
  int speed;
private:  
  bool ok;     
  char *port_name;
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#endif