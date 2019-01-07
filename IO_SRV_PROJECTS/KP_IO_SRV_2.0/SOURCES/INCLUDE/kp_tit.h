/***********************************************************
 объект управления одним текущим и одним интегрируемым (опционально)
  телеизмерением
               created by M.E.N
***********************************************************/

#ifndef _kp_tit_h_
#define _kp_tit_h_

#include <kp_types.h>
#include <kp_titi.h>
#include <io_srv_control_object.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class TITCtl : public ControlObject
{
 friend class KpConfig;
public:
 TITCtl(obj_d &d);
 void SetValue(unsigned char val);     //установка значения (основная функция)   
 void SetError(unsigned char err);     //установка недостоверности 
 const char *Parent()const;
 bool Enabled()const;
 unsigned char Num()const;
 inline unsigned char Error()const;
 virtual unsigned Type()const{return TIT;}
private:  
 inline float Ka()const;//'смещение'
 inline float Kb()const;//'множитель' 
 inline unsigned char Gist()const;//процент гистерезиса от fHi, на квантовое значение ТИТ (нечувствительность к изменению квантовых значений)
 inline char Jump()const;//квантовое значение "скачка" или 0: если не 0 и очередное значение ТИТ больше (или меньше) величины Jump, будет отправлен признак "скачка": -1 -если вниз; +1, если вверх; а так же само значение скачка. ** Обработка "скачка" может быть заблокирована черезмерной величиной Gist. 
 inline float Nominal()const;//'номинальное значение в именованных единицах'; 
 inline float HighTech()const;
 inline float LowTech()const;
 inline float HighAlarm()const;
 inline float LowAlarm()const;
 inline unsigned char MaxJump()const;                //'абсолютный предел скачка';
 unsigned char last_val;                             //предыдущее значение (квант)  
 TITiCtl *titi_d;                                    //инт. объект
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif //kp_tit