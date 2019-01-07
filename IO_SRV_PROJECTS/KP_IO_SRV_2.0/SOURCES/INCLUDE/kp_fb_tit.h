/***********************************************************
       Субблок текущих телеизмерений.
               created by M.E.N
***********************************************************/

#ifndef _kp_fb_tit_h_
#define _kp_fb_tit_h_

#include <kp_tit.h>
#include <kp_afb.h>

#define TIT_COUNT 32
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class FbTit : public AFB
{
 friend class KpControl;
 friend class KpConfig;
public:
 FbTit(obj_d &d);
 virtual ~FbTit();
 virtual int Step();  //шаг работы кон. автомата 
 virtual void ProcPacket(const unsigned char *,unsigned len);     //разбор пришедшего пакета
 virtual void SetError(unsigned char err); //установка недостоверности 
 virtual const char *Parent()const;
 virtual unsigned char Place()const;       //место в КП (0-15)
 virtual unsigned char Error()const;
 virtual unsigned Type()const{return FB_TIT;}
 bool Enabled()const;                      //признак разрешения работы
 //состояния ФБ TiT
 enum{
  FB_TIT_STEP=1,    //шаг работы
  FB_TIT_TIMEOUT,   //таймаут ожидания посылки
  FB_TIT_ERROR,     //ошибка субблока (отказ)
  FB_TIT_DISABLED,  //работа запрещена
  FB_TIT_BEGIN
 };
private:
 inline void WriteStates(int grp);         //запись значений
 TITCtl *Items[TIT_COUNT];                 //объекты - ТИТ-ы
 //первые 16 байт - знач. первой группы ТИТ, вторые, соответственно, второй группы. далее - последние значения, записанные в объекты - ТИТ-ы
 unsigned char tmp_data[64]; 
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif //kp_fb_tit