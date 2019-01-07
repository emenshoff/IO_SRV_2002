/***********************************************************
  ������� ����� ����������� ������ ������� �����-������.
               created by M.E.N
***********************************************************/
#ifndef _drv_ctl_obj_
#define _drv_ctl_obj_

#include <io_srv_modules.h>
#include <io_srv_event_processor.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class DrvModule;
extern DrvModule *GlobalDrv;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

class DrvModule : public EventProcessor
{
public:
  DrvModule(EventSupervisor *evs,StmCfg *cf):cfg(cf),EventProcessor(evs),ok(false){GlobalDrv = this;}
  virtual ~DrvModule(){}
  virtual bool Ok()const{return ok;}     //���������� � ������
  virtual void Step(){}                     //��� ������
  virtual void Init(){}                     //������������� (�������� �������� � �������� ���. ��������)
protected:
  StmCfg *cfg;
  bool ok;
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif