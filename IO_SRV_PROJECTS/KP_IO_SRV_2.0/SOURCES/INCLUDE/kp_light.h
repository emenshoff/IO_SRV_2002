/***********************************************************
  Класс - лампа (вывод на щит).
               created by M.E.N
***********************************************************/

#ifndef _kp_light_h_
#define _kp_light_h_

#include <kp_types.h>
#include <cond_parser.h>
#include <io_srv_control_object.h>
//%%%%%%%%%%% Лампа щита %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class LightCtl:public ControlObject
{
 friend class KpConfig;
public:
 LightCtl(obj_d &d,CondParser &p);
 bool On()const;               //тек. состояние (если true, то лампа должна загореться (отсылка))
 const char *Parent()const;
 unsigned char Num()const;
 virtual unsigned Type()const{return LIGHT;}
private:
 inline bool ConditionPresent()const;             //проверка наличия условия
 inline bool CheckCondition()const;               //проверка логического условия
 inline unsigned char Enabled()const;             //признак разрешения работы 
 CondParser &parser;                              //указатель на объект-разборщик выражений
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif //kp_light