/***********************************************************
 ��������� ������� � ���������� ��������� �� �����. �������.
               created by M.E.N
***********************************************************/

#include <kp_tit.h>
#include <kp_types.h>
#include <kp_titi.h>
#include <io_srv_dump.h>
#include <kp_events_cash.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
TITCtl::TITCtl(obj_d &d)
  :ControlObject(d)
{
  float f=0;
  unsigned char b=254;
  char c=0;
  short s=0;
  last_val=0;
  titi_d=0;
  SetError(255);
  cfg_d[KP_TIT.MaxJump().CfgFldId()] = b;
  //�������� � ����������� ��������
  cfg_d[KP_TIT.value().CfgFldId()] = f;
  //��������� ��������  
  cfg_d[KP_TIT.cvalue().CfgFldId()] = b;
  //������ � ����������� ��������
  cfg_d[KP_TIT.jump().CfgFldId()] = c;
  //����� �� �����
  cfg_d[KP_TIT.limit().CfgFldId()] = c;
  //������ � �������
  cfg_d[KP_TIT.cjump().CfgFldId()] = s;
  cfg_d.SetFldChangeBit(KP_TIT.MaxJump().CfgFldId());
  cfg_d.SetFldChangeBit(KP_TIT.value().CfgFldId());
  cfg_d.SetFldChangeBit(KP_TIT.cvalue().CfgFldId());
  cfg_d.SetFldChangeBit(KP_TIT.jump().CfgFldId());
  cfg_d.SetFldChangeBit(KP_TIT.limit().CfgFldId());
  cfg_d.SetFldChangeBit(KP_TIT.cjump().CfgFldId());
  EventsCash.Add(cfg_d.Id());
  cfg_d.ClearFldChangeBit(KP_TIT.MaxJump().CfgFldId());
  cfg_d.ClearFldChangeBit(KP_TIT.value().CfgFldId());
  cfg_d.ClearFldChangeBit(KP_TIT.cvalue().CfgFldId());
  cfg_d.ClearFldChangeBit(KP_TIT.jump().CfgFldId());
  cfg_d.ClearFldChangeBit(KP_TIT.limit().CfgFldId());
  cfg_d.ClearFldChangeBit(KP_TIT.cjump().CfgFldId());
}
//---------------------------------------------------------------
const char *TITCtl::Parent()const
{
  return (const char*)cfg_d[KP_TIT.Parent().CfgFldId()];
}
//---------------------------------------------------------------
unsigned char TITCtl::Num()const
{
  unsigned char num = cfg_d[KP_TIT.Num().CfgFldId()];
  return num;//cfg_d[KP_TIT.Num().CfgFldId()];
}
//---------------------------------------------------------------
void TITCtl::SetValue(unsigned char val)//��, ����� ��������� ���� �������
{
  dump.ObjMsg(cfg_d.Id(),D_MSG,"%s : ������ �������� %d...",cfg_d.Name(),val);
  if(val == last_val)
    return;
  short cj_val = 0;         //��������� �������� ������
  char limit=0;             //������� ������ �� ������
  char j_val=0;             //������
  unsigned char jump=0;
  float procent;     //������� ����. � % �� ��������  
  float fval;        //�������� � ����������� ��������  
  double tm;          //������� �����
  //----------��������� �������� � ����������� �������� ---------------
  fval=(float)(val * Kb() + Ka()); 
  if(Gist()!=0)
  {
    unsigned char pred,delta;
    // 250 - ������ ������������ 100-%
    pred = (unsigned char)((250 * Gist()) / 100);
    delta = val > last_val ? val - last_val : last_val - val;
    if(pred > delta)
    {
   //   SetError(0);//16.04.02
      return;
    }
  }
  dump.ObjMsg(cfg_d.Id(),D_MSG | D_SENSOR,"������ %s: ��������� ��������:",cfg_d.Name());
  tm = EventsCash.TimeInDbl();
  //��������� �������
  cfg_d[KP_TIT.time().CfgFldId()] = tm;
  //������ �������� (float)
  cfg_d[KP_TIT.value().CfgFldId()] = fval;
  //������ �������� � �������
  cfg_d[KP_TIT.cvalue().CfgFldId()] = val;
  //dump.ObjMsg(cfg_d.Id(),D_MSG | D_SENSOR,"\t\t%s = %f",KP_TIT.value().CfgFldName(),fval);
  //----------��������� ������� ������� �� ��������--------------------
  if(Nominal()!=0)
  {
    procent = (float)((double)fval / Nominal() * 100);
    //dump.ObjMsg(cfg_d.Id(),D_MSG | D_SENSOR , "������ %s: ������� �� �������� %f",cfg_d.Name(),KP_TIT.procent().CfgFldName(),procent);
  }
  else
    procent = 0;
  cfg_d[KP_TIT.procent().CfgFldId()] = procent;
  //--------�������� �� ������ ��������----------------  
  //unsigned char maxj_val = (unsigned char)(jump > 0 ? jump : last_val - val);
  //cfg_d.SetFldValue(KP_TIT.MaxJump().CfgFldId(),(void*)&maxj_val);  
  if(Jump())
  {
    if(val > last_val)
    {
      jump=val-last_val;
      if(jump > Jump())
      {
        j_val = 1;
        cj_val = jump;
        dump.ObjMsg(cfg_d.Id(),D_MSG | D_SENSOR,"\t\t������ �����");
      }
    }
    else
    {
      jump=last_val-val;
      if(jump > Jump())
      {
        j_val = -1;
        cj_val = -jump;
        dump.ObjMsg(cfg_d.Id(),D_MSG | D_SENSOR,"\t\t������ ����");
      }
    }
    //dump.ObjMsg(cfg_d.Id(),D_MSG | D_SENSOR,"\t\t%s: %d",KP_TIT.cjump().CfgFldName(),(short)jump);
  }  
  cfg_d[KP_TIT.jump().CfgFldId()] = j_val;
  cfg_d[KP_TIT.cjump().CfgFldId()] = cj_val;
  cfg_d[KP_TIT.MaxJump().CfgFldId()] = jump;
  //--------�������� ������ �� �������-------------  
  if (fval < LowTech())
  {
    dump.ObjMsg(cfg_d.Id(),D_MSG | D_SENSOR,"\t\t����� �� ������ ����. ������!");
    if (fval < LowAlarm())
    {
      limit = -2;
      dump.ObjMsg(cfg_d.Id(),D_MSG | D_SENSOR,"\t\t����� �� ������ ���������. ������!");
    }
    else
    {
      limit = -1;
      //Log
    }
  }
  else if(fval > HighTech())
  {
    dump.ObjMsg(cfg_d.Id(),D_MSG | D_SENSOR,"\t\t����� �� ������� ����. ������!");
    if (fval > HighAlarm())
    {
      limit = 2;
      dump.ObjMsg(cfg_d.Id(),D_MSG | D_SENSOR,"\t\t����� �� ������� ���������. ������!");
    }
    else
    {
      limit = 1;
      //Log
    }
  }
  cfg_d[KP_TIT.limit().CfgFldId()] = limit;//������������� �����
  last_val=val;           //����� ���. ��������
  if(titi_d)
    ((TITiCtl *)titi_d)->SetValue(fval);   //��������� ��������������
  cfg_d[KP_TIT.error().CfgFldId()] = (unsigned char)0;
  cfg_d.SetFldChangeBit(KP_TIT.MaxJump().CfgFldId());
  cfg_d.SetFldChangeBit(KP_TIT.value().CfgFldId());
  cfg_d.SetFldChangeBit(KP_TIT.cvalue().CfgFldId());
  cfg_d.SetFldChangeBit(KP_TIT.jump().CfgFldId());
  cfg_d.SetFldChangeBit(KP_TIT.limit().CfgFldId());
  cfg_d.SetFldChangeBit(KP_TIT.cjump().CfgFldId());
  cfg_d.SetFldChangeBit(KP_TIT.time().CfgFldId());
  cfg_d.SetFldChangeBit(KP_TIT.procent().CfgFldId());
  cfg_d.SetFldChangeBit(KP_TIT.error().CfgFldId());
  EventsCash.Add(cfg_d.Id());
  cfg_d.ClearFldChangeBit(KP_TIT.MaxJump().CfgFldId());
  cfg_d.ClearFldChangeBit(KP_TIT.value().CfgFldId());
  cfg_d.ClearFldChangeBit(KP_TIT.cvalue().CfgFldId());
  cfg_d.ClearFldChangeBit(KP_TIT.jump().CfgFldId());
  cfg_d.ClearFldChangeBit(KP_TIT.limit().CfgFldId());
  cfg_d.ClearFldChangeBit(KP_TIT.cjump().CfgFldId());
  cfg_d.ClearFldChangeBit(KP_TIT.time().CfgFldId());
  cfg_d.ClearFldChangeBit(KP_TIT.procent().CfgFldId());
  cfg_d.ClearFldChangeBit(KP_TIT.error().CfgFldId());
}
//---------------------------------------------------------------
void TITCtl::SetError(unsigned char err)
{
  unsigned char error = cfg_d[KP_TIT.error().CfgFldId()];
  if(err == error)
    return;
  double tm = EventsCash.TimeInDbl();
  cfg_d[KP_TIT.error().CfgFldId()] = err;
  cfg_d[KP_TIT.time().CfgFldId()] = tm;
  if(titi_d)
    ((TITiCtl *)titi_d)->SetError(err);
  cfg_d.SetFldChangeBit(KP_TIT.time().CfgFldId());
  cfg_d.SetFldChangeBit(KP_TIT.error().CfgFldId());
  EventsCash.Add(cfg_d.Id());
  cfg_d.ClearFldChangeBit(KP_TIT.time().CfgFldId());
  cfg_d.ClearFldChangeBit(KP_TIT.error().CfgFldId());
}
//---------------------------------------------------------------
unsigned char TITCtl::MaxJump()const
{
  unsigned char tp;
  return *(unsigned char*)cfg_d.GetFldValue(KP_TIT.MaxJump().CfgFldId(),tp);
}
//---------------------------------------------------------------
unsigned char TITCtl::Error()const
{
  return cfg_d[KP_TIT.error().CfgFldId()];
}
//---------------------------------------------------------------
bool TITCtl::Enabled()const
{
  unsigned char en = cfg_d[KP_TIT.Enabled().CfgFldId()];
  return en != 0;
}
//---------------------------------------------------------------
float TITCtl::Ka()const
{
  return cfg_d[KP_TIT.Ka().CfgFldId()];
}
//---------------------------------------------------------------
float TITCtl::Kb()const
{
  return cfg_d[KP_TIT.Kb().CfgFldId()];
}
//---------------------------------------------------------------
unsigned char TITCtl::Gist()const
{
  return cfg_d[KP_TIT.Gist().CfgFldId()];
}
//---------------------------------------------------------------
char TITCtl::Jump()const
{
  return cfg_d[KP_TIT.Jump().CfgFldId()];
}
//---------------------------------------------------------------
float TITCtl::Nominal()const
{
  return cfg_d[KP_TIT.Nominal().CfgFldId()];
}
//---------------------------------------------------------------
float TITCtl::HighTech()const
{
  return cfg_d[KP_TIT.HighTech().CfgFldId()];
}
//---------------------------------------------------------------
float TITCtl::LowTech()const
{
  return cfg_d[KP_TIT.LowTech().CfgFldId()];
}
//---------------------------------------------------------------
float TITCtl::HighAlarm()const
{
  return cfg_d[KP_TIT.HighAlarm().CfgFldId()];
}
//---------------------------------------------------------------
float TITCtl::LowAlarm()const
{
  return cfg_d[KP_TIT.LowAlarm().CfgFldId()];
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%