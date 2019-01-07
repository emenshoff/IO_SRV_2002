/***********************************************************
   Субблок ТС
               created by M.E.N
***********************************************************/

#ifndef _kp_fb_ts_h_
#define _kp_fb_ts_h_

#include <kp_ts.h>
#include <kp_afb.h>

#define TS_COUNT 64
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class FbTs : public AFB
{
 friend class KpConfig;
public:
 FbTs(obj_d &d);
 virtual ~FbTs();
 virtual void ProcPacket(const unsigned char *,unsigned len);  //обработка пришедшего от ФБ пакета 
 virtual void SetError(unsigned char err);
 virtual int Step(); 
 virtual unsigned char Place()const;                  //место в КП (0-15)
 virtual unsigned char Error()const;
 const char *Parent()const;
 virtual unsigned Type()const{return FB_TS;}
 bool Enabled()const;              //разрешение работы
//состояния ФБ TS
 enum{   
  FB_TS_TIMEOUT=1,//таймаут ожидания посылки
  FB_TS_ERROR,    //ошибка ФБ
  FB_TS_WAIT,     //отработка дребезга
  FB_TS_STEP,     //рабочий цикл
  FB_TS_BEGIN
 };
private:  
 inline void WriteStates();                //запись состояний в ТС-ы
 inline unsigned short ValidTime()const;   //таймаут дребезга 
 inline bool ClickTimeout()const;          //таймаут дребезга вышел 
 TSCtl *Items[TS_COUNT];                   //ТС-ы 
 char tmp_data[16];               //временное хранилище состояний ТС (для подавления дребезга)-в прямых кодах
 //вторые 8 байт содержат последние записанные в объекты ТС-ы значения.
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif //kp_fb_ts