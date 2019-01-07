#include <kp_dev.h>
#include <kp_io_srv.h>
#include <io_srv_dump.h>
#include <kp_packets.h>
#include <kp_events_cash.h>

#define KVIT_TIMEOUT 3   //таймаут прихода квитанции от адаптера
//---------------------------------------------------------------
#define FB_TS_POLL_FLAG   0x01
#define FB_TIT_POLL_FLAG   0x02
#define FB_TII_POLL_FLAG   0x04
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
const int kp_msg_flag = (D_MSG | D_DEV);
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
KP::KP(obj_d &d)
  :KpAbstractDev(d)
{
  for(unsigned i=0;i<16;i++)
    Items[i] = 0;
  unsigned char err=100;
  poll_flags = 0;
  old_poll_flags = 0;
  addr=(unsigned char)cfg_d[KP_DEV.Address().CfgFldId()];
  state=KP_BEGIN;  
  SetError(100);
  //SetFbsError(100);
  msg[0]=addr;
}
//---------------------------------------------------------------
KP::~KP()
{
  SetError(100);
  //SetFbsError(100);
  for(int x = 0;x<16;x++)
    if(Items[x])
      delete Items[x];
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
void KP::Kvit(unsigned char pl)
{
  dump.ObjMsg(cfg_d.Id(),D_MSG | D_DEV,"КП %s(%u): квитирую субблок(%u)!...",cfg_d.Name(),addr,pl);
  unsigned char msg[2];
  msg[0]=addr;
  msg[1]=pl;
  cnl->Send(msg,2);
}
//---------------------------------------------------------------
void KP::ProcPacket(const unsigned char *dt,unsigned len)
{
  unsigned char err = cfg_d[KP_DEV.error().CfgFldId()];
  if(!Enabled())
  {
    /*if(err == 0)
    {
      SetError(100);
      SetFbsError(100);
    }*/
    return;
  }
  unsigned char fb_err;
  KPHeader *kph = (KPHeader*)dt;
  switch(kph->mode)
  {
    case PACKET_DATA:
      switch(kph->fa)
      {
        case PACKET_TS_DATA: case PACKET_TIT_DATA1:case PACKET_TIT_DATA2:case PACKET_TIT_DATA3: case PACKET_TII_DATA: case PACKET_TU_FREE:
          Kvit(kph->afb);    //если не отслеживаем субблок, квитанция все равно отсылается!
          ItemsInTime[kph->afb] = EventsCash.TimeInDbl();
          if(Items[kph->afb])
          {
            dump.ObjMsg(cfg_d.Id(),kp_msg_flag,"КП %s(%u): Посылка для субблока(%d)...",cfg_d.Name(),addr,kph->afb);
            Items[kph->afb]->ProcPacket(dt,len);
          }
          else
          {
            /*fb_err = cfg_d[KP_DEV.fb_error(kph->afb).CfgFldId()];
            if(fb_err)
            {
              cfg_d[KP_DEV.fb_error(kph->afb).CfgFldId()] = (unsigned char)0;
              cfg_d.SetFldChangeBit(KP_DEV.fb_error(kph->afb).CfgFldId());
              EventsCash.Add(cfg_d.Id());
              cfg_d.ClearFldChangeBit(KP_DEV.fb_error(kph->afb).CfgFldId());
            }*/
            //dump.ObjMsg(cfg_d.Id(),D_ERR | D_DEV,"КП %s(%u): в конфигурации не обнаружен субблок %d, однако посылка пришла - квитирую...",cfg_d.Name(),addr,kph->afb);            
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
      dump.ObjMsg(cfg_d.Id(),kp_msg_flag,"КП %s(%u): ошибка КП !",cfg_d.Name(),addr);
      if(Items[kph->afb])
        Items[kph->afb]->SetError(100);
      /*fb_err = cfg_d[KP_DEV.fb_error(kph->afb).CfgFldId()];
      if(fb_err)
      {
        cfg_d[KP_DEV.fb_error(kph->afb).CfgFldId()] = (unsigned char)0;
        cfg_d.SetFldChangeBit(KP_DEV.fb_error(kph->afb).CfgFldId());
        EventsCash.Add(cfg_d.Id());
        cfg_d.ClearFldChangeBit(KP_DEV.fb_error(kph->afb).CfgFldId());
      }*/
      break;
    default:
      break;
  }
  if((unsigned char)cfg_d[KP_DEV.error().CfgFldId()])
  {
    SetError(0);
    state=KP_STEP;
  }
}
//---------------------------------------------------------------
void KP::Step()
{
  if(!Enabled())
  {
    /*SetError(100);
    SetFbsError(100);*/
    return;
  }
  if(!cnl)
    return;
  int i,cnt = 0;
  unsigned failed_fb_count=0;
  unsigned char err;
  switch(state)
  {
    case KP_BEGIN:
      SetFbTitPollFlag();
      SetFbTsPollFlag();
      state = KP_POLL_TS;
      break;
    case KP_STEP:
      for(i=0;i<16;i++)
      {
        //err = cfg_d[KP_DEV.fb_error(i).CfgFldId()];
        if(Items[i])
        {
          cnt++;
          Items[i]->Step();
          if(Items[i]->Error())
          {
            if(err == 0)
            {
              /*cfg_d[KP_DEV.fb_error(i).CfgFldId()] = (unsigned char)100;
              cfg_d[KP_DEV.fb_etime(i).CfgFldId()] = EventsCash.TimeInTv();            
              cfg_d.SetFldChangeBit(KP_DEV.fb_error(i).CfgFldId());
              cfg_d.SetFldChangeBit(KP_DEV.fb_etime(i).CfgFldId());
              EventsCash.Add(cfg_d.Id());
              cfg_d.ClearFldChangeBit(KP_DEV.fb_error(i).CfgFldId());
              cfg_d.ClearFldChangeBit(KP_DEV.fb_etime(i).CfgFldId());*/
            }
            failed_fb_count++;
          }
          /*else
          {            
            if(err)
            {
              cfg_d[KP_DEV.fb_error(i).CfgFldId()] = (unsigned char)0;           
              cfg_d.SetFldChangeBit(KP_DEV.fb_error(i).CfgFldId());
              EventsCash.Add(cfg_d.Id());
              cfg_d.ClearFldChangeBit(KP_DEV.fb_error(i).CfgFldId());
            }
          }*/
        }
        /*else
          if((EventsCash.TimeInDbl() - ItemsInTime[i])/1e6 > (unsigned short)cfg_d[KP_DEV.ResponceTimeout().CfgFldId()])
            {
              if(err == 0)
              {
                cfg_d[KP_DEV.fb_error(i).CfgFldId()] = (unsigned char)100;           
                cfg_d.SetFldChangeBit(KP_DEV.fb_error(i).CfgFldId());
                cfg_d.SetFldChangeBit(KP_DEV.fb_etime(i).CfgFldId());
                EventsCash.Add(cfg_d.Id());
                cfg_d.ClearFldChangeBit(KP_DEV.fb_error(i).CfgFldId());
                cfg_d.ClearFldChangeBit(KP_DEV.fb_etime(i).CfgFldId());
              }
            }*/
      }
      if(failed_fb_count == cnt)
      {
        dump.ObjMsg(cfg_d.Id(),D_ERR | D_DEV,"КП %s(%u): ошибка! Cубблоки не отвечают!",cfg_d.Name(),addr);
        state=KP_ERROR;
        SetError(100);
      }      
      else
      {
        SetError(0);
      }
      if(poll_flags & FB_TS_POLL_FLAG)
        state = KP_POLL_TS;
      else if(poll_flags & FB_TIT_POLL_FLAG)
        state = KP_POLL_TIT;
      break;
    case KP_POLL_TS:
      if(poll_flags & FB_TS_POLL_FLAG)
      {
        msg[1]=0x18;
        //dump.Print(kp_msg_flag,"КП %s(%u): Опрос TС...",cfg_d.Name(),addr);
        dump.ObjMsg(cfg_d.Id(),kp_msg_flag,"КП %s(%u): Опрос TС...",cfg_d.Name(),addr);
        cnl->Send(msg,2);  //вызов ТС-ов    
        poll_begin_time = time(0);
        poll_flags ^= FB_TS_POLL_FLAG;
      }
      else if(old_poll_flags & FB_TS_POLL_FLAG)
      {
        if(cnl->KvitIsRecived())
        {
          old_poll_flags ^= FB_TS_POLL_FLAG;
          state = KP_STEP;
        }
        else if(time(0) - poll_begin_time >KVIT_TIMEOUT)
        {
          old_poll_flags ^= FB_TS_POLL_FLAG;
          state = KP_STEP;
        }
      }
      break;
    case KP_POLL_TIT:
      if(poll_flags & FB_TIT_POLL_FLAG)
      {
        msg[1]=0x19;
        //dump.Print(kp_msg_flag,"КП %s(%u): Опрос TИТ...",cfg_d.Name(),addr);
        dump.ObjMsg(cfg_d.Id(),kp_msg_flag,"КП %s(%u): Опрос TИТ...",cfg_d.Name(),addr);
        cnl->Send(msg,2);  //вызов ТИТ-ов    
        poll_begin_time = time(0);
        poll_flags ^= FB_TIT_POLL_FLAG;
      }
      else if(old_poll_flags & FB_TIT_POLL_FLAG)
      {
        if(cnl->KvitIsRecived())
        {
          old_poll_flags ^= FB_TIT_POLL_FLAG;
          state = KP_STEP;
        }
        else if(time(0) - poll_begin_time >KVIT_TIMEOUT)
        {
          old_poll_flags ^= FB_TIT_POLL_FLAG;
          state = KP_STEP;
        }
      }
      break;
    default:
      break;
  }
}
//---------------------------------------------------------------
void KP::SetFbError(int n,unsigned char err)
{
  if(Items[n])
    Items[n]->SetError(err);
}
//---------------------------------------------------------------
void KP::SetFbsError(unsigned char err)
{
  /*double tm = EventsCash.TimeInDbl();
  for(unsigned i=0;i<16;i++)
  {
    if(Items[i])
    {
      if(Items[i]->Enabled())
        Items[i]->SetError(err);
      else
        Items[i]->SetError(100);
    }
    /*cfg_d[KP_DEV.fb_error(i).CfgFldId()] = err;
    cfg_d[KP_DEV.fb_etime(i).CfgFldId()] = tm;
    cfg_d.SetFldChangeBit(KP_DEV.fb_error(i).CfgFldId());
    cfg_d.SetFldChangeBit(KP_DEV.fb_etime(i).CfgFldId());
    EventsCash.Add(cfg_d.Id());
    cfg_d.ClearFldChangeBit(KP_DEV.fb_error(i).CfgFldId());
    cfg_d.ClearFldChangeBit(KP_DEV.fb_etime(i).CfgFldId());*/
  }
}
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
void KP::Shutdown()
{
  SetError(100);
}
//---------------------------------------------------------------
void KP::SetFbTiiPollFlag()
{
  /*poll_flags |= FB_TII_POLL_FLAG;
  old_poll_flags |= FB_TII_POLL_FLAG;*/
}
//---------------------------------------------------------------
void KP::SetFbTitPollFlag()
{
  poll_flags |= FB_TIT_POLL_FLAG;
  old_poll_flags |= FB_TIT_POLL_FLAG;
}
//---------------------------------------------------------------
void KP::SetFbTsPollFlag()
{
  poll_flags |= FB_TS_POLL_FLAG;
  old_poll_flags |= FB_TS_POLL_FLAG;
}
//---------------------------------------------------------------

//---------------------------------------------------------------
#undef FB_TS_POLL_FLAG
#undef FB_TIT_POLL_FLAG
#undef FB_TII_POLL_FLAG
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%