/**********************************************************************************
      Класс слежения за изменением полей объектов  
                     created by M.E.N.
**********************************************************************************/
#ifndef _srv_tracer_h
#define _srv_tracer_h

#include <stm_srv_cfg.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class SrvTracer
{           
public:
 SrvTracer(StmCfg *conf);
 ~SrvTracer();
 void ClearTraces();                      //удалить все отслеживания
 void AddTrace(unsigned obj,unsigned fld);//добавить запрос на отслеживание значения поля
 void DelTrace(unsigned obj,unsigned fld);//удалить одно отслеживание 
 const unsigned *GetMask(unsigned)const;  //получить маску заказов полей объекта
private:
 StmCfg*cfg;
//Ключ - идентификатор объекта в конфе, элемент - маска заказанных полей
 map<unsigned, unsigned *,less<unsigned> > Traces;
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif