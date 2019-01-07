/***********************************************************
               created by M.E.N
***********************************************************/

#ifndef _I7041_dev_h_
#define _I7041_dev_h_


#include <I7000_dev.h>
#include <I7000_dev_digital_input.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class I7041_Dev : public I7000_Dev
{
   friend class I7000_Config;
public:
 I7041_Dev(obj_d &d);
 ~I7041_Dev();
 int Step();
 void Init();
 unsigned short ModuleId()const{return 7041;}
 virtual unsigned char MaxItemsAmount()const;
 virtual const char *CommDevName()const; 
private:
  //состояния конечного автомата:
 enum{
   ST_BEGIN,           //нач. состояние    
   ST_DELAY,
   ST_FIRST_DATA_GET,  //первое чтение данных
   ST_SECOND_DATA_GET, //второе чтение данных (подавление дребезга)
   ST_WAIT,            //холостой цикл   
   ST_TIMEOUT,         //
   ST_ERR              //отказ модуля
 };
 void SetError(unsigned char err); 
 inline unsigned short ValidTime()const;//таймаут дребезга 
 unsigned err_count;
 unsigned char crc_enabled;     //признак вычисления CRC
 unsigned char state;           //текущее состояние конечного автомата
 //cash_data[0] - последние записанные значения
 //cash_data[1] - значения зафиксированные в начале интервала отработки дребезга
 unsigned short cash_data[2];
 double delay_begin_time;//время начала отработки задержки (под-е дребезга)
 double last_in_time;   //время прихода последнего пакета 
 I7000_Ts *Items[14];
 void operator=(I7041_Dev&){}
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif