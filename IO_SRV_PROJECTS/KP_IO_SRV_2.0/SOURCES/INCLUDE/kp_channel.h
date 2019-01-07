/********************************************************************	
              ��������� ������� (������ ���� ���-4)
                     created by M.E.N.
********************************************************************/
#ifndef _kp_cnl_
#define _kp_cnl_


#include <kp_types.h>
#include <io_srv_control_object.h>

class KP;
class KpCommDev;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
struct cnl_diagnostic_info
{
  unsigned char error;
  unsigned long recv_cnt;
  unsigned long snd_cnt;
  unsigned long err_cnt;
  unsigned long nonkvit_cnt;
};
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
��������� ���������� ����������� �� ���� ��������� ��������� ��
���������� ��������:

  �� ��� ���, ���� �� ������ ��������� ����� ������� ������, ���������
  ������� �� ��������� �� ������ �� �� � ������� ��. ������� �������
  ��������.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
class KpCnl:public ControlObject
{
 friend class KpConfig;
 friend class KpCommDev;
public:
 KpCnl(obj_d &);
 ~KpCnl();
 time_t last_in_time;           //����� ������� ���������� ������ ��� ���������
 unsigned short KpSpeed()const;	//�������� ������������� ������
 unsigned char Num()const;			//����� ��������
 bool CanSend()const;  
 void Step();
 bool KvitIsRecived()const{return kvit_is_recived;} 
 void SetDiagnosticInfo(cnl_diagnostic_info &diag);
 void Process(const unsigned char *data,unsigned lan);
 void Send(const unsigned char *data,unsigned len);
 bool Timeout()const{return (time(0) - last_in_time) > 180;}  //������� ������� ������� �� ��������
 const char *CommDev()const;    //��� ����. ����������, � �������� ���������� ��������� �������
 virtual unsigned Type()const{return KP_CNL;}
 unsigned char s_flag;
private: 
 cnl_diagnostic_info curr_info; 
 //������� ��� �������� ��������� �� ��������
 bool DataIsSent()const{return data_is_sent;}
 void InKvit(){kvit_is_recived=true;data_is_sent=false;} //��� ������ �������� ������ KpCdv 
 void OutData(){kvit_is_recived=false;data_is_sent=true;}
 bool kvit_is_recived; //���� ��������� ���������
 bool data_is_sent;     //���� �������� ���������� ������ ������
 double kvit_in_time,data_send_time; //��� ��� ��������� ���������
 unsigned char num;
 int curr_item_index;  //������ ��, ������� � ������� ������ ����� ����.
 KP* Items[128]; 
 KpCommDev *parent;
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif