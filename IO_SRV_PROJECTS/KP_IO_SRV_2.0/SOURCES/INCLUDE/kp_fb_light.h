/***********************************************************
             Блок вывода на щит
               created by M.E.N
***********************************************************/

#ifndef _kp_fb_light_h_
#define _kp_fb_light_h_

#include <kp_afb.h>
#include <kp_light.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class FbLight : public AFB
{
 friend class KpConfig;
public:
 FbLight(obj_d &d);
 ~FbLight();  
 virtual int Step();                      //шаг работы: проверка изменения состояний объектов-ламп и отсылка
 virtual void SetError(unsigned char err){}//заглушка
 bool Enabled()const;                       //разрешен вывод на щит
 virtual void Shutdown();                   //гашение ламп щита
 const char *Parent()const;
 virtual unsigned char Place()const;        //место в КП (0-15)
 virtual unsigned char Error()const;
 virtual unsigned Type()const{return FB_LIGHT;}
 unsigned char MaxItems()const{return fb_type;}
 //состояния блока вывода на щит
 enum{ FB_LIGHT_BEGIN, FB_LIGHT_STEP };
private:
 inline int SendData();                    //отсылка данных
 inline bool StateChanged();
 //заглушки на функции базового класса
 unsigned short ResponceTimeoutVal()const{return 0;}
 bool StartPollDelay(time_t)const{return false;}
 bool ResponceTimeout()const{return false;}
 bool ErrorSetTimeout()const{return false;}
 void SendKvit(){}
 LightCtl **Items;                          //64 или 32 лампы
 unsigned char fb_type;                     //тип субблока (64 или 32 лампы)
 //первые 8 байт - текущие значения, вторые - последние посланные
 unsigned char tmp_data[16];
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif //kp_fb_light