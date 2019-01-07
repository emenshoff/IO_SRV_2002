/***********************************************************
               created by M.E.N
***********************************************************/

#include <I7000_drv.h>
#include <stm_srv.h>
#include <io_srv_dump.h>
#include <I7000_io_srv.h>
#include <stm_srv_cfg_control.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//----------------------------------------------------------------
void I7000_IoSrv::Init(unsigned port,							//порт сервера
                   const char *cfg_file,      //имя файла конфигурации сервера
                   const char *dump_cfg_name, //имя конф. файла для дампа
                   const char *dump_name)    //имя файла дампа
{
  ConfCreator *creator=new ConfCreator(cfg_file);
  if((cfg=creator->ConfCreate())==0)
  {
    puts("Configuration failed!");
    delete creator;
    return;
  }  
  delete creator;  
  dump.Reset(cfg,dump_cfg_name,dump_name);  
  srv=new StmSrv(this,cfg,port);
  if(!srv->Ok())
  {
    delete srv;
    srv=0;
    return;
  }  
  drv=new I7000_Drv(this,cfg);
  drv->Init();
  if(!drv->Ok())
  {
    delete srv;
    delete drv;
    srv=0;
    drv=0;
    return;
  }
  AddProcessor(&dump);
  AddProcessor(srv);
  AddProcessor(drv);
  ok=true;
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%