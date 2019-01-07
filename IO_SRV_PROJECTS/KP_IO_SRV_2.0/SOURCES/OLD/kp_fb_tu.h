/***********************************************************
             субблок телеуправления
               created by M.E.N
***********************************************************/

#ifndef _kp_fb_tu_h_
#define _kp_fb_tu_h_

#include <kp_tu.h>
#include <kp_afb.h>

#define TU_COUNT 128
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class FbTu : public AFB
{
 friend class KpControl;
 friend class KpConfig;
public:
 FbTu(obj_d &d);
 ~FbTu();
 virtual void ProcPacket(const unsigned char *,unsigned len);  //разборка пришедшего пакета
 virtual char Step();                                 //шаг работы (если состояние объекта - ТУ изменилось, то шлем к КП соотв. команду)
 virtual void SetError(unsigned char err);            //установка ошибки при: а) блок не ответил; б) 
 bool Enabled()const; 
 unsigned char Num()const{return num;}
 const char *Parent()const;
 virtual unsigned char Place()const;                  //место в КП (0-15)
 virtual unsigned char Error()const;
 virtual unsigned Type()const{return FB_TU;}
 //состояние блока телеуправления
 enum{
  FB_TU_BEGIN,  //начальное состояние
  FB_TU_STEP,   //шаг работы
  FB_TU_TIMEOUT,//
  FB_TU_TU,     //ожидание ответа субблока на команду телеуправления
  FB_TU_ERROR   //ошибка субблока
 };
private:
 void Shutdown();
 inline void SendTU(unsigned char on,unsigned char grp,unsigned char obj);  //отсылка команды на телеуправления 
 inline void ItemsStep();                             //шаг обработки телесигналов
 void SendKvit(){}
 unsigned char num;           //номер в КП (1 или 2)
 TUCtl * Items[TU_COUNT];       //128 указателей на объекты - ТУ
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif //kp_fb_tu