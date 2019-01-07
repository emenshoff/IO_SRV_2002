/***********************************************************
  ���������, ��� ���� ������ �������� ������ ��������
   (Enabled = 0), �� �� �� ������� ProcPacket � Step
               created by M.E.N
***********************************************************/

#include <kp_dev.h>
#include <kp_fb_ts.h>
#include <kp_types.h>
#include <time_utils.h>
#include <io_srv_dump.h>
#include <kp_events_cash.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
FbTs::FbTs(obj_d &d)
:AFB(d)
{
  for(unsigned i=0;i<TS_COUNT;i++)
   Items[i] = 0;
  state=FB_TS_ERROR;
  SetError(100);   //� ������ ���� �������������t�...
  memset(tmp_data,0,sizeof(tmp_data));
  timeout_begin_time=time(0);
}
//---------------------------------------------------------------
FbTs::~FbTs()
{
  SetError(100);
  for(unsigned i=0;i<TS_COUNT;i++)
    if(Items[i])
      delete Items[i];
}
//---------------------------------------------------------------
void FbTs::Enable()
{
  if(Enabled())
  {
    /*SetError(0);
    state = FB_TS_ERROR;*/
  }  
}
//---------------------------------------------------------------
void FbTs::Disable()
{
  /*SetError(100);
  state = FB_TS_DISABLED;*/
}
//---------------------------------------------------------------
unsigned char FbTs::Error()const
{
  return cfg_d[KP_AFB_TS.error().CfgFldId()];
}
//---------------------------------------------------------------
unsigned char FbTs::Place()const
{
  return (unsigned char)cfg_d[KP_AFB_TS.Place().CfgFldId()];
}
//---------------------------------------------------------------
const char *FbTs::Parent()const
{
  return (const char *)cfg_d[KP_AFB_TS.Parent().CfgFldId()];
}
//---------------------------------------------------------------
void FbTs::ProcPacket(const unsigned char *dt,unsigned len)
{
  if(len<21)
    return;
  if(!Enabled())
  {
    Disable();
    return;
  }
  else
  {
    Enable();// ������ ����������
  }
  int i;
  if((dt[2] >> 4)!=0x2)
  {
    dump.ObjMsg(cfg_d.Id(),D_AFB | D_ERR,"%s : �������� ��� ������ ��� �������� ��!",cfg_d.Name());
    return;
  }  
  const unsigned char *data=dt+3; //������������� ����� �� ������ ������
  GetCurrTime(last_in_time);
  //��������� ������� � ���������� ����� ��
  for(i=0;i<8;i++)
    if((data[i] & data[i+8])!=0)
    {
      dump.ObjMsg(cfg_d.Id(),D_AFB | D_ERR,"%s : ������������ ������� � ���������� ����� ��!",cfg_d.Name());
      state=FB_TS_ERROR;
      return;
    }
  unsigned char err = cfg_d[KP_AFB_TS.error().CfgFldId()];
  switch(state)
  {
    case FB_TS_ERROR:   //� ���� ������� �������� ������� �����
      SetError(0);
      state=FB_TS_STEP;
      memcpy(tmp_data,data+8,8);  //���������� � ����� ������ � ������ ����
      WriteStates();              //����� ������� �������� �� � �������
      break;
    //��������� ��������. ���� � ������ ��������� �������� ���������� ������������, �� ����� � ����� ��������� �����������
    case FB_TS_WAIT:
      memcpy(tmp_data,data+8,8);
      break;
    case FB_TS_STEP : case FB_TS_TIMEOUT:
      memcpy(tmp_data,data+8,8);
      if(!ValidTime())                  //������� �������� �� �������������� (��� �������� = 0)
        WriteStates();
      else                              //����� ����� ��������
        state=FB_TS_WAIT;
      break;
    case FB_TS_DISABLED:
      return;
    default:
      break;
  }
}
//---------------------------------------------------------------
char FbTs::Step()
{
  if(!Enabled())
  {
    Disable();
    return 0;
  }
  else
  {
    Enable();
  }
  struct timeval tv;
  GetCurrTime(tv);
  switch(state)
  {
    case FB_TS_STEP:
      if(ResponceTimeout())
      {   
        //dump.Print(D_AFB | D_MSG,"%s: ������� (%d ���.) �������� �������!",cfg_d.Name(),ResponceTimeoutVal());
        dump.ObjMsg(cfg_d.Id(),D_AFB | D_MSG,"%s: ������� (%d ���.) �������� �������!",cfg_d.Name(),ResponceTimeoutVal());
        state=FB_TS_TIMEOUT;
        timeout_begin_time=poll_delay_begin_time=time(0);
      }
      break;
    case FB_TS_WAIT:                    //��������� ��������
      if(ClickTimeout())                //�������� ���������� � �������� ����� ����������
      {
        last_in_time=tv;
        state=FB_TS_STEP;
        WriteStates();
      }
      break;
    case FB_TS_TIMEOUT:
      if(ErrorSetTimeout())
      {
        //dump.Print(D_AFB | D_ERR,"%s: ���������� ������� �������� �������! ������ ��������...",cfg_d.Name());
        dump.ObjMsg(cfg_d.Id(),D_AFB | D_ERR,"%s: ���������� ������� �������� �������! ������ ��������...",cfg_d.Name());
        SetError(100);    //��������� ���������������
        state=FB_TS_ERROR;
        timeout_begin_time=time(0); 
      }
      else if(StartPollDelay(poll_delay_begin_time))
      {
        //dump.Print(D_AFB | D_MSG,"%s: ������� ���������� ������ ����������...",cfg_d.Name());
        dump.ObjMsg(cfg_d.Id(),D_AFB | D_MSG,"%s: ������� ���������� ������ ����������...",cfg_d.Name());
        parent->SetFbTsPollFlag();
        poll_delay_begin_time=time(0); 
      }
      break;
    case FB_TS_ERROR:
      if(StartPollDelay(timeout_begin_time))
      {
        //dump.Print(D_AFB | D_MSG,"%s: ������� ���������� ������ ����������...",cfg_d.Name());
        dump.ObjMsg(cfg_d.Id(),D_AFB | D_MSG,"%s: ������� ���������� ������ ����������...",cfg_d.Name());
        parent->SetFbTsPollFlag();    //������� ������ ���������....
        timeout_begin_time=time(0);   //����� ������ �������� �����������
      }
      break;
    case FB_TS_DISABLED:
      break;
    default:
      break;
  }
  return 0;
}
//---------------------------------------------------------------
void FbTs::SetError(unsigned char err)
{
  unsigned char error = cfg_d[KP_AFB_TS.error().CfgFldId()];
  if(error == err)
    return;
  double tm = EventsCash.TimeInDbl();
  cfg_d[KP_AFB_TS.error().CfgFldId()] = err;
  cfg_d[KP_AFB_TS.etime().CfgFldId()] = tm;
  for(unsigned i=0;i<TS_COUNT;i++)
    if(Items[i])
    {
      //if(Items[i]->Enabled())
        Items[i]->SetError(err);
      //else
        //Items[i]->SetError(100);
    }
  cfg_d.SetFldChangeBit(KP_AFB_TS.error().CfgFldId());
  cfg_d.SetFldChangeBit(KP_AFB_TS.etime().CfgFldId());
  EventsCash.Add(cfg_d.Id());
  cfg_d.ClearFldChangeBit(KP_AFB_TS.error().CfgFldId());
  cfg_d.ClearFldChangeBit(KP_AFB_TS.etime().CfgFldId());
}
//---------------------------------------------------------------
unsigned short FbTs::ValidTime()const
{
  return cfg_d[KP_AFB_TS.ValidTime().CfgFldId()];
}
//---------------------------------------------------------------
bool FbTs::Enabled()const
{
  unsigned char en = cfg_d[KP_AFB_TS.Enabled().CfgFldId()];
  return (en!=0);
}
//---------------------------------------------------------------
bool FbTs::ClickTimeout()const
{
  unsigned short valid_time = cfg_d[KP_AFB_TS.ValidTime().CfgFldId()];
  //������������
  double time_diff = (EventsCash.TimeInDbl()-Tv2Dbl(last_in_time))/1e3;
  return (time_diff >= valid_time);
}
//---------------------------------------------------------------
void FbTs::WriteStates()
{
  int i=0,by,bt;
  int bit_test;
  for(by=0;by<8;by++)//���� �� ������
  {
    int mask = 0x80;
    if(tmp_data[by]==tmp_data[by+8])//�� ��������� ����, ������ ��� ��������� �����. ��-��
	  {
	    i+=8;
      continue;
	  }
    for(bt=0;bt<8;mask >>= 1,bt++,i++)//���� �� �����
    {
      if(Items[i])
      {
        if(Items[i]->Enabled())
        {
          Items[i]->SetError(0);//���������� �����
          bit_test = tmp_data[by]&mask;
          Items[i]->SetState(bit_test!=0);
          //dump.Print(D_AFB | D_MSG,"%s ��������� %d",Items[i]->Cfg_D()->Name(),(bit_test==0 ? 0 :1));
          dump.ObjMsg(D_AFB | D_MSG,cfg_d.Id(),"%s ��������� %d",Items[i]->Cfg_D()->Name(),(bit_test==0 ? 0 : 1));
        }
        /*else
          Items[i]->SetError(100);//���������� �����������
          */
      }
    }
    tmp_data[by+8]=tmp_data[by];
  }
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%