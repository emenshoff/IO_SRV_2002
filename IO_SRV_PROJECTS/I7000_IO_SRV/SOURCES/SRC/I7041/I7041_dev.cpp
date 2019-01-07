/***********************************************************
               created by M.E.N
***********************************************************/

#include <I7041_types.h>
#include <I7041_dev.h>
#include <I7000_proto.h>
#include <I7000_events_cash.h>
#include <io_srv_dump.h>

#define MAX_ERR_COUNT 15

#ifdef WIN32
  #define SEND_DELAY Sleep(port->DelayVal());
#else
  #define SEND_DELAY usleep(port->DelayVal()*1e3);
#endif
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
I7041_Dev::I7041_Dev(obj_d &d)
:I7000_Dev(d) 
{
  for(int i=0;i<14;i++)
    Items[i] = 0;
  err_count = 0;
  cash_data[0] = 0;  
  cash_data[1] = 0;  
  state = ST_BEGIN;
  SetError(100);  
  address = (unsigned char)cfg_d[I7041_DEV.Address().CfgFldId()];
}
//--------------------------------------------------------------------
I7041_Dev::~I7041_Dev()
{
  SetError(100);
  for(int i=0;i<14;i++)
    if(Items[i])
      delete Items[i];
}
//--------------------------------------------------------------------
unsigned char I7041_Dev::MaxItemsAmount()const
{
  return 14;
}
//--------------------------------------------------------------------
int I7041_Dev::Step()
{
  unsigned result;
  unsigned char st,tp;
  if(!*(unsigned char*)cfg_d.GetFldValue(I7041_DEV.Enabled().CfgFldId(),tp))
  {
    if((unsigned char)cfg_d[I7041_DEV.error().CfgFldId()] == 0)
      SetError(100);
    state = ST_WAIT;
    return DEV_FREE;
  }
  switch(state)
  { 
    case ST_WAIT:
      if(port->CanUse())
      {
        OutDigitalIn(port,address,crc_enabled,ModuleId(),result);
        state = ST_FIRST_DATA_GET;
      }
      return DEV_STEP;
    case ST_FIRST_DATA_GET://чтение данных
      if(!port->CanUse())
        return DEV_STEP;
      if(InDigitalIn(port,address,crc_enabled,ModuleId(),result) == OK)
      {
        if((unsigned char)cfg_d[I7041_DEV.error().CfgFldId()] != 0)
          SetError(0);
        last_in_time = EventsCash.TimeInDbl();        
        if(cash_data[0] != (unsigned short)result)//состояние какого-то тс изменилось..
        {
          dump.ObjMsg(cfg_d.Id(),D_MSG | D_DEV,"Изменилось состояние входов модуля %s...",cfg_d.Name());
          if(ValidTime())
          {
            //dump.ObjMsg(cfg_d.Id(),D_MSG | D_DEV,"%s : отработка дребезга...",cfg_d.Name());
            state = ST_DELAY;
            delay_begin_time = last_in_time;
            cash_data[1] = (unsigned short)result;
          }
          else
          {
            //dump.ObjMsg(cfg_d.Id(),D_MSG | D_DEV,"%s : запись состояний входов...",cfg_d.Name());
            state = ST_WAIT;            
            for(int i=0;i<14;i++)
              if(Items[i])
              {
                //dump.Print(D_MSG,"%s : %d",Items[i]->Cfg_D()->Name(),!((result>>i)&0x01));
                st = (result>>i)&0x01;
                if(st != ((cash_data[0]>>i)&0x01))
                {
                  dump.ObjMsg(cfg_d.Id(),D_MSG,"%s: Изменилось состояние входа %s[%d]...",cfg_d.Name(),Items[i]->Cfg_D()->Name(),i);
                  Items[i]->SetState(st != 0x01);                
                }
              }
            cash_data[0] = (unsigned short)result;
          }
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
    case ST_DELAY:
      if(port->CanUse())
      {
        OutDigitalIn(port,address,crc_enabled,ModuleId(),result);
        state = ST_SECOND_DATA_GET;
      }
      return DEV_STEP;
    case ST_SECOND_DATA_GET:
      if(!port->CanUse())
        return DEV_STEP;            
      if(InDigitalIn(port,address,crc_enabled,ModuleId(),result) == OK)
      {
        if((unsigned char)cfg_d[I7041_DEV.error().CfgFldId()] !=0)
          SetError(0);
        last_in_time = EventsCash.TimeInDbl();
        if(cash_data[1] != (unsigned short)result)
        {
          delay_begin_time = last_in_time;
          cash_data[1] = (unsigned short)result;
          state = ST_DELAY;
        }
        else
        {
          if((last_in_time - delay_begin_time)/1e3 > ValidTime())
          { 
            //dump.ObjMsg(cfg_d.Id(),D_MSG | D_DEV,"%s : Дребезг отработан; запись состояний входов...",cfg_d.Name());
           for(int i=0;i<14;i++)
            if(Items[i])
            {
              st = (result>>i)&0x01;
              if(st != ((cash_data[0]>>i)&0x01))
              {
                dump.ObjMsg(cfg_d.Id(),D_MSG,"%s: Изменилось состояние входа %s[%d]...",cfg_d.Name(),Items[i]->Cfg_D()->Name(),i);
                Items[i]->SetState(st != 0x01);
              }
            }
            cash_data[0] = cash_data[1]; 
            state = ST_WAIT;
          }
          else
            state = ST_DELAY;
        }
        return DEV_FREE;
      }
      else
      {
        dump.ObjMsg(cfg_d.Id(),D_ERR | D_DEV,"%s : ошибка чтения данных из порта %s...",cfg_d.Name(),port->Name());
       state = ST_ERR;
      }
      break;
    case ST_BEGIN:
      Init();
      state = ST_WAIT;
      return DEV_STEP;
    case ST_ERR:     //отказ модуля   
      if(++err_count > MAX_ERR_COUNT)
      {
        err_count = 0;
        if(!(unsigned char)cfg_d[I7041_DEV.error().CfgFldId()])
        {
          dump.ObjMsg(cfg_d.Id(),D_ERR | D_DEV,"%s : ошибка модуля...",cfg_d.Name());
          SetError(100);
        }
      }
      state = ST_WAIT;
      break;
  }      
  return DEV_ERR;
}
//--------------------------------------------------------------------
void I7041_Dev::Init()
{
  unsigned result;  
  crc_enabled = 0;
  last_in_time = EventsCash.TimeInDbl();
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
    OutSetConfig(port,address,crc_enabled,ModuleId(),cfg);
    crc_enabled = 1;
    SEND_DELAY
    SEND_DELAY
    if(InSetConfig(port,address,crc_enabled,ModuleId(),result) == OK)
    {      
      SEND_DELAY
      OutDigitalIn(port,address,crc_enabled,ModuleId(),result);
      SEND_DELAY
      if(InDigitalIn(port,address,crc_enabled,ModuleId(),result) == OK)
      {
        unsigned char st;
        SetError(0);
        dump.Print(D_MSG,"  Ok...");
        last_in_time = EventsCash.TimeInDbl();
        for(int i=0;i<14;i++)
          if(Items[i])
          {
            st = (result>>i)&0x01;            
              Items[i]->SetState(st != 0x01);
          }
        cash_data[0] = cash_data[1] = (unsigned short)result; 
        return;
      }
      else
        goto fail;
    }
  }
fail:
  SetError(100);
  dump.Print(D_MSG,"  Ошибка!");  
}
//--------------------------------------------------------------------
const char *I7041_Dev::CommDevName()const
{
  return (const char*)cfg_d[I7041_DEV.CommDev().CfgFldId()];
}
//--------------------------------------------------------------------
void I7041_Dev::SetError(unsigned char err)
{
  cfg_d[I7041_DEV.error().CfgFldId()] = err;
  cfg_d[I7041_DEV.etime().CfgFldId()] = EventsCash.TimeInDbl();
  EventsCash.Add(cfg_d.Id());
  cfg_d.ClearFldChangeBit(I7041_DEV.error().CfgFldId());
  cfg_d.ClearFldChangeBit(I7041_DEV.etime().CfgFldId());
  for(int i=0;i<14;i++)
    if(Items[i])
      Items[i]->SetError(err);
}
//--------------------------------------------------------------------
unsigned short I7041_Dev::ValidTime()const
{
  return (unsigned short)cfg_d[I7041_DEV.ValidTime().CfgFldId()];
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
