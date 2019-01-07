/********************************************************************
           Собственно, сам сервер ввода - вывода "Гранита"...
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
 virtual void Init(unsigned port,							//порт сервера
                   const char *cfg_file,      //имя файла конфигурации сервера
                   const char *dump_cfg_name, //имя конф. файла для дампа
                   const char *dump_name);    //имя файла дампа
private:
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif