/***********************************************************
               created by M.E.N
***********************************************************/

#include <I7042_types.h>
#include <I7042_dev.h>
#include <I7000_proto.h>
#include <I7000_events_cash.h>
#include <io_srv_dump.h>

#ifdef WIN32
  #define SEND_DELAY Sleep(port->DelayVal());
#else
  #define SEND_DELAY usleep(port->DelayVal()*1e3);
#endif
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
I7042_Dev::I7042_Dev(obj_d &d)
:I7000_Dev(d) 
{
  for(int i=0;i<13;i++)
    Items[i] = 0;
  SetError(100);
  curr_data = 0xffff;
  state = ST_BEGIN;
  last_in_time = EventsCash.TimeInDbl();
  address = (unsigned char)cfg_d[I7042_DEV.Address().CfgFldId()];
}
//----------------------------------------------------------------
I7042_Dev::~I7042_Dev()
{
  SetError(100);
  for(int i=0;i<13;i++)
    if(Items[i])
      delete Items[i];
}
//----------------------------------------------------------------
unsigned char I7042_Dev::MaxItemsAmount()const
{
  return 13;
}
//----------------------------------------------------------------
void I7042_Dev::Init()
{
  unsigned result;
  crc_enabled = 0;
//  last_in_time = EventsCash.TimeInDbl();
  dump.Print(D_MSG,"Инициализация устройства %s: ",cfg_d.Name());  
  //отсылка сброса без CRC
  OutResetModuleStatus(port,address,crc_enabled,ModuleId(),0);
  SEND_DELAY
  if(InResetModuleStatus(port,address,crc_enabled,ModuleId(),result) != OK)
  {
    crc_enabled = 1;
    SEND_DELAY
    //отсылка сброса с CRC:
    OutResetModuleStatus(port,address,crc_enabled,ModuleId(),0);
    SEND_DELAY
    if(InResetModuleStatus(port,address,crc_enabled,ModuleId(),result) != OK)
      goto fail;
  }
  SEND_DELAY
  OutSetWatchdogTimeoutValue(port,address,crc_enabled,ModuleId(),0); 
  SEND_DELAY
  if(InSetWatchdogTimeoutValue(port,address,crc_enabled,ModuleId(),result) == OK)
  {
    unsigned cfg;
    unsigned char spd_val = GetSpeed_I7000(port->Speed());
    cfg = 0;
    cfg |= (address);    //адрес модуля
    cfg |= (0x40<<8);    //тип модуля
    cfg |= (spd_val<<16);//скорость
    cfg |= (0xC0<<24);   //формат данных
    SEND_DELAY
    OutSetConfig(port,address,crc_enabled,ModuleId(),cfg);
    crc_enabled = 1;
    SEND_DELAY
    SEND_DELAY
    if(InSetConfig(port,address,crc_enabled,ModuleId(),result) == OK)
    {       
      curr_data = 0x0;
      unsigned data = curr_data;
      SEND_DELAY
      OutSetDigitalOut(port,address,crc_enabled,ModuleId(),data);
      SEND_DELAY
      if(InSetDigitalOut(port,address,crc_enabled,ModuleId(),data) == OK)
      {
        last_in_time = EventsCash.TimeInDbl();
        dump.Print(D_MSG,"  Ok...");
        SetError(0);
        return;
      }      
    }
  }
fail:
  SetError(100);
  dump.Print(D_MSG,"  Ошибка!");
}
//----------------------------------------------------------------
int I7042_Dev::Step()
{
  if(!(unsigned char)cfg_d[I7042_DEV.Enabled().CfgFldId()])
  {
    if(!(unsigned char)cfg_d[I7042_DEV.error().CfgFldId()])
      SetError(100);
    return DEV_FREE;
  }
  int i;
  unsigned result;
  unsigned char st;
  unsigned short tmp;
  switch(state)
  {
    case ST_WAIT:
      if(port->CanUse())
      {
        OutGetDigitalIOStatus(port,address,crc_enabled,ModuleId(),result);
        state = ST_IN_GET_OUTS_STATUS;
      }
      return DEV_STEP;
    /*case ST_OUT_GET_OUTS_STATUS:
      break;*/
    case ST_IN_GET_OUTS_STATUS:
      if(!port->CanUse())
        return DEV_STEP;
      if(InGetDigitalIOStatus(port,address,crc_enabled,ModuleId(),result) == OK)
      {
        if((unsigned char)cfg_d[I7042_DEV.error().CfgFldId()] !=0 )
          SetError(0);
        if(curr_data != (unsigned short)result)
        {
          for(int i=0;i<13;i++)
            if(Items[i])
            {
              st = (result>>i)&0x01;
              Items[i]->SetVal(st);
              //dump.Print(D_MSG,"%s : %d",Items[i]->Cfg_D()->Name(),st);
            }
          curr_data = (unsigned short)result;
        }
        //проверка
        tmp = 0;
        for(i=0;i<13;i++)
          if(Items[i])
          {
            int step_val = Items[i]->Step();
            if(step_val == I7000_Tu::TU_ON)
            {
              //dump.Print(D_MSG,"ON");
              tmp |= (0x01<<i);
            }
            else if(step_val == I7000_Tu::TU_OFF)
            {
              //dump.Print(D_MSG,"OFF");
              // tmp ^= (0x01<<i);*/
            }             
            else if(step_val == I7000_Tu::TU_STEP)
            {
              tmp |= (Items[i]->LastVal() ? (0x01<<i) : 0);
            }
          }
        if(tmp != curr_data)
        {
          state = ST_OUT_DO_TU;
          curr_data = tmp;
        }
        else
        {
          state = ST_WAIT;
          return DEV_FREE;
        }
        return DEV_STEP;
      }
      else
      {
        dump.ObjMsg(cfg_d.Id(),D_ERR | D_DEV,"%s : ошибка чтения данных из порта %s...",cfg_d.Name(),port->Name());
        state = ST_ERR;
      }
      break;
    case ST_OUT_DO_TU:
      if(port->CanUse())
      {
        OutSetDigitalOut(port,address,crc_enabled,ModuleId(),(unsigned)curr_data);
        state = ST_IN_DO_TU;
      }
      return DEV_STEP;
    case ST_IN_DO_TU:
      if(!port->CanUse())
        return DEV_STEP;
      if(InSetDigitalOut(port,address,crc_enabled,ModuleId(),result) == OK)
      {        
        if((unsigned char)cfg_d[I7042_DEV.error().CfgFldId()] !=0)
          SetError(0);
        last_in_time = EventsCash.TimeInDbl();
        state = ST_WAIT;
        return DEV_FREE;
      }
      else
      {
        dump.ObjMsg(cfg_d.Id(),D_ERR | D_DEV,"Ошибка отсылки команды телеуправления модулю %s!",cfg_d.Name());
        state = ST_ERR;
      }
      break;
    case ST_BEGIN:
      Init();
      state = ST_WAIT;
      return DEV_FREE;
      break;
    case ST_ERR:
      state = ST_WAIT;
      if(!(unsigned char)cfg_d[I7042_DEV.error().CfgFldId()])
        SetError(100);
      break;
  }
  return DEV_ERR;
}
//----------------------------------------------------------------
const char *I7042_Dev::CommDevName()const
{
  return (const char*)cfg_d[I7042_DEV.CommDev().CfgFldId()];
}
//----------------------------------------------------------------
void I7042_Dev::SetError(unsigned char err)
{
  cfg_d[I7042_DEV.error().CfgFldId()] = err;
  cfg_d[I7042_DEV.etime().CfgFldId()] = EventsCash.TimeInDbl();
  for(int i=0;i<13;i++)
    if(Items[i])
      Items[i]->SetError(err);
  EventsCash.Add(cfg_d.Id());
  cfg_d.ClearFldChangeBit(I7042_DEV.error().CfgFldId());
  cfg_d.ClearFldChangeBit(I7042_DEV.etime().CfgFldId());
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
