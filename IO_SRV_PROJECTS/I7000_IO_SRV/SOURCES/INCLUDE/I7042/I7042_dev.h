/***********************************************************
  Класс, инкапсулирующий один модуль I7042 (цифровой выход
  на 13 каналов)
               created by M.E.N
***********************************************************/

#ifndef _I7042_dev_h_
#define _I7042_dev_h_

#include <I7000_dev.h>
#include <I7000_dev_digital_output.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class I7042_Dev : public I7000_Dev
{
   friend class I7000_Config;
public:
 I7042_Dev(obj_d &d);
 ~I7042_Dev();  
 int Step();           //шаг работы
 void Init();          //инициализация
 void SetError(unsigned char err);//установка недостоверности
 unsigned short ModuleId()const{return 7042;}
 unsigned char MaxItemsAmount()const;
 virtual const char *CommDevName()const; 
private:
  //многжество состояний конечного автомата:
 enum
 {
   ST_BEGIN,                //начальное состояние
   ST_OUT_DO_TU,            //отсылка телеуправления
   ST_IN_DO_TU,             //получение квитанции
   //ST_OUT_GET_OUTS_STATUS,  //отсылка запроса статуса выходов
   ST_IN_GET_OUTS_STATUS,   //прием статуса выходов
   ST_WAIT,                 //холостой ход
   ST_ERR                   //отказ модуля
 };
 
 unsigned char state;//текущее состояние конечного автомата
 unsigned char crc_enabled;//признак вычисления контрольной суммы
 unsigned short curr_data; //текущее состояние выходов
 double last_in_time;      //время прихода последней посылки от модуля
 I7000_Tu *Items[13];      //объекты - ТУ
};
//----------------------------------------------------------------
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif //I7042_dev