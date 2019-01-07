/********************************************************************
        Класс объекта связи с конфигурацией сервера.
                     created by M.E.N.
********************************************************************/
#ifndef _control_object_h
#define _control_object_h

#include <stm_srv_cfg.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class ControlObject
{
 //friend class KpControl;
public:
 ControlObject(obj_d &d):cfg_d(d){}
 virtual ~ControlObject(){}
 const obj_d *Cfg_D()const{return &cfg_d;}
 virtual unsigned Type()const{return 0;}
protected: 
 obj_d &cfg_d;
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif