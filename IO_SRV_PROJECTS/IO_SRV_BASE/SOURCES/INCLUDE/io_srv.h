/********************************************************************
          ������� ����� -  ������ �����-������.
                     created by M.E.N.
********************************************************************/
#ifndef _io_srv_h_
#define _io_srv_h_

#include <io_srv_event_supervisor.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class Dump;
class StmSrv;
class StmCfg;
class DrvModule;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class IoSrv : public EventSupervisor
{
public:
 IoSrv();
 virtual ~IoSrv();
 virtual void Step();
 bool Ok()const{return ok;}
 virtual void Init(unsigned port,							//���� �������
                   const char *cfg_file,      //��� ����� ������������ �������
                   const char *dump_cfg_name, //��� ����. ����� ��� �����
                   const char *dump_name);    //��� ����� �����
protected:
 StmSrv *srv;
 StmCfg *cfg;
 DrvModule *drv;
 bool ok;
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif