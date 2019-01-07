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
 FldDscr &CommDev()  const{return *fields[0];} //��� ������� - �����
 FldDscr &Address()  const{return *fields[1];} //����� �� ����������
 FldDscr &Enabled()  const{return *fields[2];} //���������� ������
 FldDscr &ValidTime()const{return *fields[3];} //������� �������� (mc)
 //�������� ����:
 FldDscr &error()    const{return *fields[4];} //������ ����������
 FldDscr &etime()    const{return *fields[5];} //����� ����������� ������
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
extern I7041_Dev_Dscr I7041_DEV;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif //I7041_types