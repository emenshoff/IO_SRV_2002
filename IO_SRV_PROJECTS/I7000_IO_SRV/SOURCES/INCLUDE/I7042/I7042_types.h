/**************************************************************
                    by M.E.N.
/**************************************************************/
#ifndef _I7042_types_h_
#define _I7042_types_h_

#include <io_srv_drv_types.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//контроллер ТУ I7042
class I7042_Dev_Dscr : public TypeDscr
{
public:
 I7042_Dev_Dscr();
 FldDscr &CommDev()const{return *fields[0];} //имя объекта - порта, к которому контроллер подключен
 FldDscr &Address()const{return *fields[1];} //Адрес на линии (0-255)
 FldDscr &Enabled()const{return *fields[2];} //Признак разрешения работы
 FldDscr &error()const{return *fields[3];} //Ошибка устройства
 FldDscr &etime()const{return *fields[4];} //Время наступления ошибки
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
extern I7042_Dev_Dscr I7042_DEV;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif //_I7042_types_h_