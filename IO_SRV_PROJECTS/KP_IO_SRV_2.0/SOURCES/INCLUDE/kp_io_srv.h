/********************************************************************
           ����������, ��� ������ ����� - ������ "�������"...
                     created by M.E.N.
********************************************************************/
#ifndef _kp_io_srv
#define _kp_io_srv

#include <io_srv.h>

class KpControl;
class StmSrv;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class KpIoSrv : public IoSrv
{
public:
 KpIoSrv();
 virtual void Step();
 virtual void Init(unsigned port,							//���� �������
                   const char *cfg_file,      //��� ����� ������������ �������
                   const char *dump_cfg_name, //��� ����. ����� ��� �����
                   const char *dump_name);    //��� ����� �����
private:
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif