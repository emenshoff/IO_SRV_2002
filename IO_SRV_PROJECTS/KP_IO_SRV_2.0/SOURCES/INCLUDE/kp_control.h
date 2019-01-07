/**********************************************************************************  
   		Класс объекта управления драйверным модулем
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
 bool Ok()const{return ok;}       //готовность к работе
 void Step();                     //шаг работы
 void Init();                     //инициализация (проверка объектов и создание лог. структур)
 void Clear();                    //завершение работы
 void ProcessEvent(const IoSrvEvent &);
private:
 void Reconfig();
 vector<KpCommDev *> Ports;       //массив последовательных портов
 vector<TITiCtl *> ItgTits;       //интегрируемые ТИТ - ы 
 CondParser parser;               //разборщик условий (для объектов-ламп)
 SrvTracer tracer;                //объект для отслеживания конфигурационных полей 
 bool ok;                         //флаг готовности к работе
 vector<TITiCtl *>::iterator titi_i;//последнего обработанного инт. тит 16.04.02 MEN
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif