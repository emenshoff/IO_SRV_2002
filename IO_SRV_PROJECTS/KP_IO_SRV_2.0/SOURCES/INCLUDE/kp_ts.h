/***********************************************************
       объект управления одним телесигналом
               created by M.E.N
***********************************************************/

#ifndef _kp_ts_h_
#define _kp_ts_h_

#include <kp_types.h>
#include <io_srv_control_object.h>
//%%%%%%%%%%% Телесигнал (ТС) %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class TSCtl : public ControlObject
{
 friend class KpControl;
 friend class KpConfig;
public:
 TSCtl(obj_d &d);
 void SetState(unsigned char on);             //установка значения
 void SetError(unsigned char err);   //установка недостоверности 
 const char *Parent()const;
 unsigned char Num()const;
 virtual unsigned Type()const{return TS;}
 inline unsigned char Error()const;
 bool Enabled()const;
private:
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif //kp_ts