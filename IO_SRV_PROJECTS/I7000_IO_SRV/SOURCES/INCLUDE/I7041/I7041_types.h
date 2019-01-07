/***********************************************************
               created by M.E.N
***********************************************************/

#ifndef _I7041_types_h_
#define _I7041_types_h_

#include <io_srv_drv_types.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class I7041_Dev_Dscr : public TypeDscr
{
public:
 I7041_Dev_Dscr();
 FldDscr &CommDev()  const{return *fields[0];} //имя объекта - порта
 FldDscr &Address()  const{return *fields[1];} //адрес на магистрали
 FldDscr &Enabled()  const{return *fields[2];} //разрешение работы
 FldDscr &ValidTime()const{return *fields[3];} //таймаут дребезга (mc)
 //выходные поля:
 FldDscr &error()    const{return *fields[4];} //ошибка устройства
 FldDscr &etime()    const{return *fields[5];} //время наступления ошибки
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
extern I7041_Dev_Dscr I7041_DEV;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif //I7041_types