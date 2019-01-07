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
 //млин, не бейте меня, братаны - затычка, типа, это:
 friend class FbLight;
 friend class FbTu;
public:
 KP(obj_d &d);
 ~KP();
 virtual unsigned Type()const{return KP_DEVICE;}
 virtual void ProcPacket(const unsigned char *,unsigned len);  //обработка одного пакета    
 virtual void Step();       //шаг работы 
 //virtual void Send(const unsigned char *data,unsigned len);
 //void SetFbError(unsigned char n,unsigned char err);   //установка ошибки одного ФБ  
 void SetError(unsigned char err);
 bool Enabled()const;       //признак разрешения работы
 unsigned short ResponceTimeoutVal()const;
 unsigned char ErrorSetTimeoutVal()const;
 unsigned char StartPollVal()const;
 virtual const char *Parent()const; 
 //состояния конечного автомата:
 enum{
  KP_BEGIN,       //начало работы
  KP_STEP,        //рабочий цикл
  KP_POLL,        //опрос //18.04.02
  //KP_POLL_TS,     //опрос ТС //18.04.02
  //KP_POLL_TIT,    //опрос ТИТ //18.04.02
  KP_ERROR,       //отказ КП
  KP_DISABLED     //блокировка КП
 };
private:   
 //inline void SetFbError(int,unsigned char);//установка недостоверности
 //inline void SetFbsError(unsigned char);
 //void Shutdown();             //завершение работы  
 void SetFbTiiPollFlag();   //
 void SetFbTitPollFlag();   //
 void SetFbTsPollFlag();    //
 void ProcessFbs();
 inline void PollFbTs();      //опрос всех субблоков ТС
 inline void PollFbTit();     //опрос всех субблоков ТИТ
 inline void PollFbTii();     //опрос всех субблоков ТИИ
 inline void Kvit(unsigned char pl); //квитирование ФБ, которых нет в конфе (а посылка может прийти!) 
 time_t last_ts_in_time;      //время прихода последней посылки от субблока ТС
 time_t last_tit_in_time;     //время прихода последней посылки от субблока ТИТ
 time_t last_ts_poll_time;    //время последнего опроса ТС
 time_t last_tit_poll_time;   //время последнего опроса ТИТ
 unsigned char state;         //текущее состояние кон. автомата 
 unsigned char poll_flags;    //флаги опроса
 unsigned char msg[2];        //посылка квитирования субблока
 AFB *Items[16];              //функц. блоки
 int f_ts,f_tit;
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif