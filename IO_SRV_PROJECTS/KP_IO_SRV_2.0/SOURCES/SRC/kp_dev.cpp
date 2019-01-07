#include <kp_dev.h>
#include <kp_io_srv.h>
#include <io_srv_dump.h>
#include <kp_packets.h>
#include <kp_events_cash.h>
/*************************************************************************
все в заплатках....
:(
*************************************************************************/
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#define KVIT_TIMEOUT 1   //таймаут прихода квитанции от адаптера
//---------------------------------------------------------------
#define FB_TS_POLL_FLAG      0x01
#define FB_TIT_POLL_FLAG     0x02
#define FB_TII_POLL_FLAG     0x04
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
const int kp_msg_flag = (D_MSG | D_DEV);
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
KP::KP(obj_d &d)
  :KpAbstractDev(d),f_ts(0),f_tit(0)
{
  for(unsigned i=0;i<16;i++)
    Items[i] = 0;
  poll_flags = 0;
  addr=(unsigned char)cfg_d[KP_DEV.Address().CfgFldId()];
  state=KP_BEGIN;  
  SetError(255);
  for(int x = 0;x<16;x++)
  {
    cfg_d[KP_DEV.fb_error(x).CfgFldId()] = (unsigned char)255;
    cfg_d[KP_DEV.fb_etime(x).CfgFldId()] = EventsCash.TimeInTv();
    cfg_d.SetFldChangeBit(KP_DEV.fb_error(x).CfgFldId());
    cfg_d.SetFldChangeBit(KP_DEV.fb_etime(x).CfgFldId());
    EventsCash.Add(cfg_d.Id());
    cfg_d.ClearFldChangeBit(KP_DEV.fb_error(x).CfgFldId());
    cfg_d.ClearFldChangeBit(KP_DEV.fb_etime(x).CfgFldId());
  }
  last_ts_poll_time = last_tit_poll_time = last_ts_in_time = last_tit_in_time = time(0);
  msg[0]=addr;
}
//---------------------------------------------------------------
KP::~KP()
{
  SetError(255);  
  for(int x = 0;x<16;x++)
  {
    if(Items[x])
      delete Items[x];    
    cfg_d[KP_DEV.fb_error(x).CfgFldId()] = (unsigned char)255;
    cfg_d[KP_DEV.fb_etime(x).CfgFldId()] = EventsCash.TimeInTv();
    cfg_d.SetFldChangeBit(KP_DEV.fb_error(x).CfgFldId());
    cfg_d.SetFldChangeBit(KP_DEV.fb_etime(x).CfgFldId());
    EventsCash.Add(cfg_d.Id());
    cfg_d.ClearFldChangeBit(KP_DEV.fb_error(x).CfgFldId());
    cfg_d.ClearFldChangeBit(KP_DEV.fb_etime(x).CfgFldId());
  }
}
//---------------------------------------------------------------
unsigned short KP::ResponceTimeoutVal()const
{
  return cfg_d[KP_DEV.ResponceTimeout().CfgFldId()];
}
//---------------------------------------------------------------
unsigned char KP::StartPollVal()const
{
  return cfg_d[KP_DEV.StartPoll().CfgFldId()];
}
//---------------------------------------------------------------
const char *KP::Parent()const
{
  return (const char*)cfg_d[KP_DEV.CnlName().CfgFldId()];
}
//---------------------------------------------------------------
bool KP::Enabled()const
{
  unsigned char en = cfg_d[KP_DEV.Enabled().CfgFldId()];
  return en!=0;
}
//---------------------------------------------------------------
unsigned char KP::ErrorSetTimeoutVal()const
{
  return cfg_d[KP_DEV.ErrorTimeout().CfgFldId()];
}
//---------------------------------------------------------------
void KP::Kvit(unsigned char pl)
{
  dump.ObjMsg(cfg_d.Id(),D_MSG | D_DEV,"КП %s(%u): квитирую субблок[%u]!...",cfg_d.Name(),addr,pl);
  unsigned char msg[2];
  msg[0]=addr;
  msg[1]=pl;
  cnl->Send(msg,2);
}
//---------------------------------------------------------------
void KP::ProcPacket(const unsigned char *dt,unsigned len)
{
  unsigned char err = cfg_d[KP_DEV.error().CfgFldId()];
  if((dump.Flags() & kp_msg_flag) == kp_msg_flag)
  {
    dump.ObjMsg(cfg_d.Id(),kp_msg_flag,"КП %s :пришла посылка:",cfg_d.Name());
    char vl[10];
    string packet = "";
    for(int i=0;i<len;i++)
    {
      if(dt[i]<16)
        sprintf(vl," 0%X",(unsigned char)dt[i]);
      else
        sprintf(vl," %X",(unsigned char)dt[i]);
      packet += vl;
    }
    dump.ObjMsg(cfg_d.Id(),kp_msg_flag," %s ",packet.c_str());
  }
  KPHeader *kph = (KPHeader*)dt;
  switch(kph->mode)
  {
    case PACKET_DATA: 
      switch(kph->fa)
      {
        case PACKET_TS_DATA:
        case PACKET_TIT_DATA1:
        case PACKET_TIT_DATA2:
        case PACKET_TIT_DATA3:
        case PACKET_TII_DATA: 
        case PACKET_TU_FREE:
          if(kph->fa != PACKET_TU_FREE)
            Kvit(kph->afb);    //шлем квитанцию
          if(!Enabled())
          {
            
            return;
          }
          
          if(Items[kph->afb])
          {
            if(Items[kph->afb]->Type() == FB_TS)
              last_ts_in_time = time(0);
            else if(Items[kph->afb]->Type() == FB_TIT)
              last_tit_in_time = time(0);
            dump.ObjMsg(cfg_d.Id(),kp_msg_flag,"КП %s(%u): Посылка от субблока[%d]...",cfg_d.Name(),addr,kph->afb);
            Items[kph->afb]->ProcPacket(dt,len);
            err = cfg_d[KP_DEV.fb_error(kph->afb).CfgFldId()]; 
            if(err)
              if(!Items[kph->afb]->Error())
              {
                cfg_d[KP_DEV.fb_error(kph->afb).CfgFldId()] = (unsigned char)0;
                cfg_d[KP_DEV.fb_etime(kph->afb).CfgFldId()] = EventsCash.TimeInTv();
                cfg_d.SetFldChangeBit(KP_DEV.fb_error(kph->afb).CfgFldId());
                cfg_d.SetFldChangeBit(KP_DEV.fb_etime(kph->afb).CfgFldId());
                EventsCash.Add(cfg_d.Id());
                cfg_d.ClearFldChangeBit(KP_DEV.fb_error(kph->afb).CfgFldId());
                cfg_d.ClearFldChangeBit(KP_DEV.fb_etime(kph->afb).CfgFldId());
              }
          }
          else
          {
            dump.ObjMsg(cfg_d.Id(),kp_msg_flag,"КП %s(%u): Посылка от неотслеживаемого субблока[%d]...",cfg_d.Name(),addr,kph->afb);
          }
          break;
        default:
          dump.ObjMsg(cfg_d.Id(),D_ERR | D_DEV,"КП %s(%u): Неизвестный тип информации в пакете!",cfg_d.Name(),addr);
          break;
      }
      break;
    case PACKET_NO_DATA:
      dump.ObjMsg(cfg_d.Id(),kp_msg_flag,"КП %s(%u): нет данных...",cfg_d.Name(),addr);
      break;
    case PACKET_ERROR:
      dump.ObjMsg(cfg_d.Id(),kp_msg_flag,"КП %s(%u): ошибка субблока [%u] !",cfg_d.Name(),addr,kph->afb);
      /*if(Items[kph->afb])
        Items[kph->afb]->SetError(100); По насtоянию Невзорова А.*/
      break;
    default:
      break;
  }  
  //заплатка:
  /*for(int i=0;i<16;i++)
  {
    err = cfg_d[KP_DEV.fb_error(i).CfgFldId()]; 
    if(!err)
    {
      SetError(0);
      break;
    }
  }*/
}
//---------------------------------------------------------------
void KP::Step()
{  
  int i;  
  if(!cnl)
    return;  
  long poll_timeout;
  unsigned short rsp;
  time_t curr_time = time(0);
  switch(state)
  {
    case KP_ERROR:
      //dump.ObjMsg(cfg_d.Id(),D_ERR | D_DEV,"КП %s(%u): state = [KP_ERROR]",cfg_d.Name(),addr);
      state = KP_STEP;
      break;
    case KP_BEGIN:
     //dump.ObjMsg(cfg_d.Id(),D_ERR | D_DEV,"КП %s(%u): state = [KP_BEGIN]",cfg_d.Name(),addr);
      f_ts = f_tit = 0;
      for(i=0;i<16;i++)
        if(Items[i])
        {
          if(Items[i]->Type()== FB_TS)
            f_ts++;
          else if(Items[i]->Type() == FB_TIT)
            f_tit++;
        }
      if(f_tit)
        SetFbTitPollFlag();
      if(f_ts)
        SetFbTsPollFlag();
      state = KP_STEP;//12/2/2
      break;
    case KP_STEP:
     // dump.ObjMsg(cfg_d.Id(),D_ERR | D_DEV,"КП %s(%u): state = [KP_STEP]",cfg_d.Name(),addr);
      //проверка таймаутов ожидания посылок:
      rsp = ResponceTimeoutVal();
      poll_timeout=rsp-(rsp  * StartPollVal() / 100);
      //формируем опросы
      if(curr_time - last_ts_in_time >= poll_timeout)
        if(f_ts)
        {
          //dump.ObjMsg(cfg_d.Id(),D_ERR | D_DEV,"КП %s(%u): нужен опрос ТС",cfg_d.Name(),addr);
          //dump.ObjMsg(cfg_d.Id(),D_ERR | D_DEV,"КП %s(%u): curr_time = %d",cfg_d.Name(),addr,curr_time);
          //dump.ObjMsg(cfg_d.Id(),D_ERR | D_DEV,"КП %s(%u): last_ts_in_time = %d",cfg_d.Name(),addr,last_ts_in_time);
          SetFbTsPollFlag();
        }
      if(curr_time - last_tit_in_time >= poll_timeout)
        if(f_tit)
        {
          //dump.ObjMsg(cfg_d.Id(),D_ERR | D_DEV,"КП %s(%u): нужен опрос ТИТ",cfg_d.Name(),addr);
          //dump.ObjMsg(cfg_d.Id(),D_ERR | D_DEV,"КП %s(%u): curr_time = %d",cfg_d.Name(),addr,curr_time);
          //dump.ObjMsg(cfg_d.Id(),D_ERR | D_DEV,"КП %s(%u): last_ts_in_time = %d",cfg_d.Name(),addr,last_ts_in_time);
          SetFbTitPollFlag();
        }
      ProcessFbs();
      if((poll_flags & FB_TS_POLL_FLAG) || (poll_flags & FB_TIT_POLL_FLAG))
        state = KP_POLL;
      break;
    case KP_POLL://18.04.02
      //dump.ObjMsg(cfg_d.Id(),D_ERR | D_DEV,"КП %s(%u): state = [KP_POLL]",cfg_d.Name(),addr);
      if(curr_time - last_ts_in_time >= ResponceTimeoutVal())
      {
        dump.ObjMsg(cfg_d.Id(),D_ERR | D_DEV,"КП %s(%u): опрос ТС закончить!",cfg_d.Name(),addr);
        //dump.ObjMsg(cfg_d.Id(),D_ERR | D_DEV,"КП %s(%u): curr_time = %d",cfg_d.Name(),addr,curr_time);
        //dump.ObjMsg(cfg_d.Id(),D_ERR | D_DEV,"КП %s(%u): last_ts_in_time = %d",cfg_d.Name(),addr,last_ts_in_time);
        last_ts_in_time = curr_time;
        poll_flags ^= FB_TS_POLL_FLAG;
        //state = KP_STEP;
        //return;
      }
      if(curr_time - last_tit_in_time >= ResponceTimeoutVal())
      {
        dump.ObjMsg(cfg_d.Id(),D_ERR | D_DEV,"КП %s(%u): прос ТИТ закончить!",cfg_d.Name(),addr);
        last_tit_in_time = curr_time;
        poll_flags ^= FB_TIT_POLL_FLAG;
        //state = KP_STEP;
        //return;
      }
      if(last_ts_poll_time  ==  last_tit_poll_time)
        last_tit_poll_time++;
        if(last_ts_poll_time < last_tit_poll_time)//тс опрашивались раньше, чем тит
        {
          if(curr_time - last_tit_poll_time > 1)//время вышло, можно опрашивать тс
          {
            if(poll_flags & FB_TS_POLL_FLAG)
            {
              msg[1]=0x18;
              if(cnl->CanSend())
              {
                dump.ObjMsg(cfg_d.Id(),kp_msg_flag,"КП %s(%u): Опрос TС...",cfg_d.Name(),addr);
                cnl->Send(msg,2);  //вызов ТС-ов    
                last_ts_poll_time = curr_time;
                //poll_flags ^= FB_TS_POLL_FLAG; // сброс флага
              }
            }
          }
        }
        else if(last_ts_poll_time > last_tit_poll_time)//тит опрашивались раньше, чем тc
        {
          if(curr_time - last_ts_poll_time > 1)//время вышло можно опрашивать тит
          {
            if(poll_flags & FB_TIT_POLL_FLAG)
            {
              msg[1]=0x19;          
              if(cnl->CanSend())
              {
                dump.ObjMsg(cfg_d.Id(),kp_msg_flag,"КП %s(%u): Опрос TИТ...",cfg_d.Name(),addr);
                cnl->Send(msg,2);  //вызов ТИТ-ов    
                last_tit_poll_time = curr_time;
                //poll_flags ^= FB_TIT_POLL_FLAG;
              }
            }
          }
        }
        state = KP_STEP;
      break;
    default:
      break;
  }
}
//---------------------------------------------------------------
/*void KP::Send(const unsigned char *data,unsigned len)
{
  cnl->Send(data,len);
}*/
//---------------------------------------------------------------
void KP::SetError(unsigned char err)
{
  unsigned char error = cfg_d[KP_DEV.error().CfgFldId()];  
  if(error == err)
    return;
  double tm = EventsCash.TimeInDbl();
  cfg_d[KP_DEV.error().CfgFldId()] = err;
  cfg_d[KP_DEV.etime().CfgFldId()] = tm;
  cfg_d.SetFldChangeBit(KP_DEV.error().CfgFldId());
  cfg_d.SetFldChangeBit(KP_DEV.etime().CfgFldId());
  EventsCash.Add(cfg_d.Id());
  cfg_d.ClearFldChangeBit(KP_DEV.error().CfgFldId());
  cfg_d.ClearFldChangeBit(KP_DEV.etime().CfgFldId());  
}
//---------------------------------------------------------------
void KP::ProcessFbs()
{
  int step_val;
  int i,cnt = 0,failed_fb_count=0,no_data_fb_cnt=0,valid_fb_count=0;
  unsigned char err,fb_err;
  for(i=0;i<16;i++)
      {
        if(Items[i])
        {
          cnt++;
          step_val = Items[i]->Step();          
          //проверяем, надо ли опрашивать субблоки
          if(step_val == AFB::AFB_POLL)
          {
            if(Items[i]->Type()== FB_TS)
              SetFbTsPollFlag();
            else if(Items[i]->Type() == FB_TIT)
              SetFbTitPollFlag();
          }
          err = cfg_d[KP_DEV.fb_error(i).CfgFldId()];
          fb_err = Items[i]->Error();
          if(fb_err != err)
          {
              cfg_d[KP_DEV.fb_error(i).CfgFldId()] = Items[i]->Error();
              cfg_d[KP_DEV.fb_etime(i).CfgFldId()] = EventsCash.TimeInTv();
              cfg_d.SetFldChangeBit(KP_DEV.fb_error(i).CfgFldId());
              cfg_d.SetFldChangeBit(KP_DEV.fb_etime(i).CfgFldId());
              EventsCash.Add(cfg_d.Id());
              cfg_d.ClearFldChangeBit(KP_DEV.fb_error(i).CfgFldId());
              cfg_d.ClearFldChangeBit(KP_DEV.fb_etime(i).CfgFldId());
          }
          if(fb_err == 100)
            failed_fb_count++;
          else if(fb_err == 255)
            no_data_fb_cnt++;
          else
            valid_fb_count++;
        }
      }
      if(valid_fb_count == 0)
      {
        if(failed_fb_count)
        {
          dump.ObjMsg(cfg_d.Id(),D_ERR | D_DEV,"КП %s(%u): ошибка! Отказ субблоков!",cfg_d.Name(),addr);
          //dump.ObjMsg(cfg_d.Id(),D_ERR | D_DEV,"КП %s(%u): ->[KP_ERROR]",cfg_d.Name(),addr);
          state=KP_ERROR;
          SetError(100);
        }
        else if(no_data_fb_cnt == cnt)
          SetError(255);
      }
      else 
        SetError(0);
}
//---------------------------------------------------------------
void KP::SetFbTiiPollFlag()
{
  /*poll_flags |= FB_TII_POLL_FLAG;*/
  
}
//---------------------------------------------------------------
void KP::SetFbTitPollFlag()
{
  poll_flags |= FB_TIT_POLL_FLAG;
}
//---------------------------------------------------------------
void KP::SetFbTsPollFlag()
{
  poll_flags |= FB_TS_POLL_FLAG;
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%