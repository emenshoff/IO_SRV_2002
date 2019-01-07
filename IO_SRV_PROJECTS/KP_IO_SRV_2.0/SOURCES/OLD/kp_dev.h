/********************************************************************
	        	Класс для представления КП "ГРАНИТ"
                     created by M.E.N.
********************************************************************/
/*
Шаг Обработки должен начинаться с обработки ФБ, а не портов!
*/
#ifndef _kp_dev_h
#define _kp_dev_h

#include <kp_types.h>
#include <kp_header.h>
#include <kp_afb.h>
#include <kp_cdv.h>
#include <kp_abstract_dev.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//класс - контрольный пункт телеметрии (КП) "Граныт"
//--------------------------------------------------------------------
class KP : public KpAbstractDev
{
 friend class KpCnl;
 friend class KpConfig;
public:
 KP(obj_d &d);
 ~KP();
 virtual unsigned Type()const{return KP_DEVICE;}
 virtual void ProcPacket(const unsigned char *,unsigned len);  //обработка одного пакета   
 void SetFbError(unsigned char n,unsigned char err);   //установка ошибки одного ФБ
 virtual void Step();       //шаг работы 
 void SetFbTiiPollFlag();   //
 void SetFbTitPollFlag();   //
 void SetFbTsPollFlag();    // 
 void SetError(unsigned char err);
 void Enable();
 void Disable();
 bool Enabled()const;       //признак разрешения работы
 virtual const char *Parent()const; 
 //состояния конечного автомата:
 enum{
  KP_BEGIN,       //начало работы
  KP_STEP,        //рабочий цикл
  KP_POLL_TS,     //опрос ТС
  KP_POLL_TIT,    //опрос ТИТ
  KP_ERROR,       //отказ КП
  KP_DISABLED     //блокировка КП
 };
private: 
 inline void SetFbError(int,unsigned char);//установка недостоверности
 inline void SetFbsError(unsigned char);
 void Shutdown();             //завершение работы  
 inline void PollFbTs();      //опрос всех субблоков ТС
 inline void PollFbTit();     //опрос всех субблоков ТИТ
 inline void PollFbTii();     //опрос всех субблоков ТИИ
 void Kvit(unsigned char pl); //квитирование ФБ, которых нет в конфе (а посылка может прийти!) 
 long poll_begin_time;        //время начала опроса
 unsigned char state;         //текущее состояние кон. автомата 
 unsigned char poll_flags;    //флаги опроса
 unsigned char old_poll_flags;
 unsigned char msg[2];
 AFB *Items[16];              //функц. блоки
 double ItemsInTime[16];      //время прихода посылок от субблоков - 16.10.01
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif