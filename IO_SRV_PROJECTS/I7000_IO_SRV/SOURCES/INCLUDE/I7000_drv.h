/***********************************************************
               created by M.E.N
***********************************************************/

#ifndef _I7000_drv_h_
#define _I7000_drv_h_

//#include <I7000_config.h>
#include <I7000_dev.h>
#include <I7000_cdv.h>
#include <srv_tracer.h>
#include <io_srv_drv_module.h>

class I7000_Config;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class I7000_Drv : public DrvModule
{
 friend class I7000_Config;
public:
 I7000_Drv(EventSupervisor *evs,StmCfg *cf);
 virtual ~I7000_Drv();
 virtual bool Ok()const{return ok;}     //готовность к работе
 virtual void Step();                   //шаг работы
 virtual void Init();                   //инициализация
 virtual void ProcessEvent(const IoSrvEvent &);
protected:
 void Clear();
 //vector<I7000_sensor *>  
 SrvTracer traces;
 vector<I7000_CommDev *> ports;
private:
};
//----------------------------------------------------------------
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif //I7000_drv