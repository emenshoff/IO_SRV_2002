/********************************************************************
          Базовый класс -  сервер ввода-вывода.
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
 virtual void Init(unsigned port,							//порт сервера
                   const char *cfg_file,      //имя файла конфигурации сервера
                   const char *dump_cfg_name, //имя конф. файла для дампа
                   const char *dump_name);    //имя файла дампа
protected:
 StmSrv *srv;
 StmCfg *cfg;
 DrvModule *drv;
 bool ok;
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif