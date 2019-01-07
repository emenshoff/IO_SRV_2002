/**********************************************************************************  
   		����� ������� ���������� ���������� �������
                     created by M.E.N.
**********************************************************************************/
#ifndef _kp_control_h
#define _kp_control_h


#include <kp_dev.h>
#include <kp_titi.h>
#include <kp_types.h>
#include <srv_tracer.h>
#include <kp_header.h>
#include <cond_parser.h>
#include <io_srv_drv_module.h>

#define KPHeaderLen 3

class KpIoSrv;
class KpConfig;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

class KpControl : public DrvModule
{
 friend class KpConfig;
public:
  KpControl(EventSupervisor *evs,StmCfg *conf);
 ~KpControl();
 bool Ok()const{return ok;}       //���������� � ������
 void Step();                     //��� ������
 void Init();                     //������������� (�������� �������� � �������� ���. ��������)
 void Clear();                    //���������� ������
 void ProcessEvent(const IoSrvEvent &);
private:
 void Reconfig();
 vector<KpCommDev *> Ports;       //������ ���������������� ������
 vector<TITiCtl *> ItgTits;       //������������� ��� - � 
 CondParser parser;               //��������� ������� (��� ��������-����)
 SrvTracer tracer;                //������ ��� ������������ ���������������� ����� 
 bool ok;                         //���� ���������� � ������
 vector<TITiCtl *>::iterator titi_i;//���������� ������������� ���. ��� 16.04.02 MEN
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif