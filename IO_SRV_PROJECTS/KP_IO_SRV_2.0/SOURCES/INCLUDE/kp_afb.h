/********************************************************************
  Класс, реализующий взаимодействие с различными функциональными
  блоками КП.
                     created by M.E.N.
********************************************************************/
#ifndef _kp_afb_h
#define _kp_afb_h

#include <kp_types.h>
#include <kp_packets.h>
#include <kp_header.h>
#include <io_srv_control_object.h>

class KP;
class KpCommDev;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//базовый класс - функциональный блок
class AFB : public ControlObject
{
 friend class KpControl;
 friend class KpConfig;
public:
  enum{AFB_STEP,AFB_POLL,AFB_FREE};
 AFB(obj_d &d);
 virtual ~AFB(){}
 virtual void ProcPacket(const unsigned char *,unsigned len){} //обработка пришедшего пакета
 virtual int Step(){return AFB_FREE;}                //шаг работы автомата  
 virtual void SetError(unsigned char err);            //установка недостоверности всем объектам субблока
 virtual unsigned char Place()const{return 17;}       //место в КП (1-16)
 virtual const char *Parent()const{return "";}        //имя объекта - КП
 virtual unsigned char Error()const{return 100;}      //ошибка субблока
 virtual bool Enabled()const{return false;} 
protected: 
 bool ResponceTimeout()const;                         //истек таймаут ответа
 bool ErrorSetTimeout()const;                         //кратность (ErrorTimeout * ReqTimeout) превысила предел   
 unsigned char state;                                 //текущее состояние 
 struct timeval last_in_time;                         //время прихода последней посылки 
 KP *parent;                                          //указатель на объект - КП
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif