#include <kp_io_srv.h>
#include <kp_control.h>
#include <io_srv_dump.h>
#include <stm_srv.h>
#include <stm_srv_cfg_control.h>

#include <time_utils.h>
//-----------------------------------------------------------------
KpIoSrv::KpIoSrv()
{
}
//-----------------------------------------------------------------
void KpIoSrv::Init(unsigned port,
                   const char *cfg_file,
                   const char *dump_cfg_name,
                   const char *dump_name)
{
  IO_StartupTime  = time(0);
  IO_InitDevTime = 0;
  ConfCreator *creator=new ConfCreator(cfg_file);
  if((cfg=creator->ConfCreate())==0)
  {
    delete creator;
    return;
  }
  delete creator;
  dump.ResetTraces(cfg,dump_cfg_name,dump_name);
  dump.Print(D_MSG,"***Сервер ввода-вывода кп \"Гранит\". Версия 2.0 (17.04.02)***");
  srv=new StmSrv(this,cfg,port);
  if(!srv->Ok())
  {
    delete srv;
    srv=0;
    return;
  }
  drv=new KpControl(this,cfg);
  drv->Init();
  if(!drv->Ok())
  {
    delete srv;
    delete drv;
    srv=0;
    drv=0;
    return;
  }
  AddProcessor(srv);
  AddProcessor(drv);
  AddProcessor(&dump);
  ok=true;
}
//-----------------------------------------------------------------
void KpIoSrv::Step()
{
  if(!ok)
    return;
/*  double time;
  struct timeval t1,t2,t3;
  GetCurrTime(t1);*/
  SLEEP(1);
  drv->Step();
  //GetCurrTime(t2);
  //SLEEP(1);
  srv->Step();
  /*GetCurrTime(t3);
  dump.Print(D_MSG,"Время работы драйв. модуля: %f время работы серв. модуля: %f",Tv2Dbl(t2-t1)/1000,Tv2Dbl(t3-t2)/1000);*/
}
//-----------------------------------------------------------------