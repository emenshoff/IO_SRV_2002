/***********************************************************
  �������� �� ������ � ��������� ��� ����������� ��������
               created by M.E.N
***********************************************************/

#ifndef _kpe_fn_h_
#define _kpe_fn_h_
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#include <kpe_dev.h>
#include <kpe_fb_ts.h>
#include <kpe_fb_tit.h>
#include <kpe_types.h>
#include <io_srv_dump.h>

#ifndef ASSIGNED
#define ASSIGNED 0x1      //������ ������������ � ���������� (���������� ��� ����������)
#endif

#ifndef OBJ_FAILED
#define OBJ_FAILED 0x2    //������ �������� �������
#endif
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//�������� �� ������ ���� KPE_DEV:
bool KpeDevNumWrong(KpeDev *d)
{
  if(d->Address() > 127)
  {
    dump.Print(D_ERR,"\t������������ �������� ���� %s ������� %s : %d!",KPE_DEV.Address().CfgFldName(),d->Cfg_D()->Name(),d->Address());
    return true;
  }
  return false;
}
//----------------------------------------------------------------
//������ ��������
bool KpeWrong(KpeDev *d)
{
  if(d)
    if(KpeNumWrong(d))
    {
      return true;
    }
  return false;
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//                  �������� ��������� (�����)
//������������ ���������� ����� ��
bool FbePlaceWrong(KpeAfb *d)
{
  unsigned place=d->Place();
  if(place >16 || place < 1)
  {
    dump.Print(D_ERR,"\t������������ �������� ���� Place ������� %s : %u!",d->Cfg_D()->Name(),place);
    return true;
  }
  return false;
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//            �������� ��-��      
//�������� ������ ��-� � ��������
bool TseNumWrong(KpeTs *d)
{
  unsigned num=d->Num();
  if(num > 64 || num <1)
  {
    dump.Print(D_ERR,"\t������������ �������� ���� %s ������� %s : %d!",KPE_TS.Num().CfgFldName(),d->Cfg_D()->Name(),num);
    return true;
  }
  return false;
}
//----------------------------------------------------------------
//�������� ��-�
bool TseWrong(KpeTs *d)
{
 if(d)
  if(TseNumWrong()(d))
  {
    return true;
  }
  return false;
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//            ��������  ���-�
//�������� ������ ���-� � ��������
bool TiteNumWrong(KpeTit *d)
{
  unsigned char num = d->Num();
  if(num > 32 || num <1)
  {
    dump.Print(D_ERR,"\t������������ �������� ���� %s ������� %s: %d!",KPE_TIT.Num().CfgFldName(),d->Cfg_D()->Name(),num);
    return true;
  }
  return false;
}
//----------------------------------------------------------------
bool TiteWrong(KpeTit *d)
{
 if(d)
  if(TiteNumWrong()(d))
  {
    return true;
  }
  return false;
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//�������� �� ��������� ���� KpDev:
bool KpsEqual(KpeDev * d1,KpeDev * d2)
{    
  if(d1!=d2)
  {
    const char *cdv1,*cdv2;
    unsigned char tp;
    cdv1=(const char*)d1->Cfg_D()->GetFldValue(KPE_DEV.CnlName().CfgFldId(),tp);
    cdv2=(const char*)d2->Cfg_D()->GetFldValue(KPE_DEV.CnlName().CfgFldId(),tp);
    if(d1->Address() == d2->Address() && !strcmp(cdv1,cdv2))
    {
      dump.Print(D_ERR,"\t���������� ������� ���� ��: %s � %s !",d1->Cfg_D()->Name(),d2->Cfg_D()->Name());
      return true;
    }
  }
  return false;
}
//----------------------------------------------------------------
//�������� ��
bool FbsEqual(KpeAfb *d1,KpeAfb *d2)
{
  if(d1!=d2)
    if(strcmp(d1->Parent(),d2->Parent())==0)
      if(d1->Place() == d2->Place())
      {
        dump.Print(D_ERR,"\t���������� ���� ���� ���: %s � %s : %d!",d1->Cfg_D()->Name(),d2->Cfg_D()->Name(),d1->Place());
        return true;
      }
  return false;
}
//----------------------------------------------------------------
//�������� �� ���������� ��-��
bool TseEqual(KpeTs *d1,KpeTs *d2)
{
 if(d1!=d2)
  if(d1->Num() == d2->Num() && !strcmp(d1->Parent(),d2->Parent()))
  {
    dump.Print(D_ERR,"\t���������� ������� �C � ����� ��������: %s � %s !",d1->Cfg_D()->Name(),d2->Cfg_D()->Name());
    return true;
  }
  return false;
}
//----------------------------------------------------------------
//�������� �� ���������� ������� ���-�� � ����� ��������
bool TitsEqual(KpeTit * d1,KpeTit * d2)
{
 if(d1!=d2)
  if(d1->Num() == d2->Num() && !strcmp(d1->Parent(),d2->Parent()))
  {
    dump.Print(D_ERR,"\t���������� ������� ��� � ����� ��������: %s � %s !",d1->Cfg_D()->Name(),d2->Cfg_D()->Name());
    return true;
  }
  return false;
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif //kpe_fn