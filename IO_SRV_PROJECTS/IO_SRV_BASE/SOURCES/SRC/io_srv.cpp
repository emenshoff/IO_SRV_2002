/********************************************************************
          Базовый класс -  сервер ввода-вывода.
                     created by M.E.N.
********************************************************************/
#include <stm_srv.h>
#include <io_srv.h>
#include <io_srv_dump.h>
#include <io_srv_drv_module.h>
#include <stm_srv_cfg_control.h>

#include <time_utils.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
IoSrv::IoSrv()
:srv(0),cfg(0),drv(0),ok(false)
{
}
//-----------------------------------------------------------------
IoSrv::~IoSrv()
{
  if(drv)
    delete drv;
  if(srv)
    delete(srv);
  if(cfg)
    delete cfg;
}
//-----------------------------------------------------------------
void IoSrv::Init(unsigned port,
                   const char *cfg_file,
                   const char *dump_cfg_name,
                   const char *dump_name)
{
  ConfCreator *creator=new ConfCreator(cfg_file);
  if((cfg=creator->ConfCreate())==0)
  {
    delete creator;
    return;
  }
  delete creator;
  dump.ResetTraces(cfg,dump_cfg_name,dump_name);
  srv=new StmSrv(this,cfg,port);
  if(!srv->Ok())
  {
    delete srv;
    srv=0;
    return;
  }
  drv=new DrvModule(this,cfg);
  drv->Init();
  if(!drv->Ok())
  {
    delete srv;
    delete drv;
    srv=0;
    drv=0;
    return;
  }
  ok=true;
}//-----------------------------------------------------------------
void IoSrv::Step()
{
  if(!ok)
    return;
  SLEEP(1);
  drv->Step();
  SLEEP(1);
  srv->Step();  
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%