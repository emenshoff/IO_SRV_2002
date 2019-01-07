/***********************************************************
//черновой вариант...
               created by M.E.N
***********************************************************/

#include <I7000_functional.h>
#include <I7000_types.h>
#include <I7041_types.h>
#include <I7042_types.h>
#include <I7041_dev.h>
#include <I7042_dev.h>
#include <I7000_config.h>
#include <io_srv_dump.h>

void PrintKillingMessage(const char *name)
{
  dump.Print(D_WARN," Объект %s удален!",name);
}
//-----------------------------------------------------------------
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
bool I7000_Config::Configure()
{
  bool result;
  int i,cdv_cnt=0,dev_cnt=0,ts=0,tu=0;
  I7000_CommDev *cdv;
  I7000_Dev *dev;
  CheckTypeDescription();
  CheckForErrors();  
  dump.Print(D_MSG,"*** Определение логическогих связей между объектами: ***");
  {
    vector<pair<I7000_CommDev *,unsigned char> >::iterator cdv_i;
    //инициализация портов    
    for(cdv_i=CDVs.begin();cdv_i!=CDVs.end();cdv_i++)
    {
      if((*cdv_i).second & OBJ_FAILED)
        continue;
      cdv = (*cdv_i).first;
      if(cdv->Init())
      {
        const char *cdv_name = cdv->Cfg_D()->Name();
        vector<pair<I7000_Dev *,unsigned char> >::iterator dev_i;
        //определяем соответствие девайсов портам:
        for(dev_i=Devs.begin();dev_i!=Devs.end();dev_i++)
        {
          if((*dev_i).second & OBJ_FAILED)
            continue;
          dev = (*dev_i).first;
          if(CdvNameKey(dev,cdv_name))
          {
            const char *dev_name = dev->Cfg_D()->Name();
            vector<pair<I7000_Device_Input *,unsigned char> >::iterator inp_i;
            //Определение соотв - я входов девайсам:
            for(inp_i=D_Is.begin();inp_i!=D_Is.end();inp_i++)
            {
              if((*inp_i).second & OBJ_FAILED)
                continue;
              if(DevNameKey(static_cast<I7000_Device_Item*>((*inp_i).first),dev_name))
                if(dev->ModuleId() == 7041)
                {
                  if(dev->MaxItemsAmount() >= (*inp_i).first->Num())
                  {
                    dump.Print(D_MSG,"  %s  ->  %s[%u]",(*inp_i).first->Cfg_D()->Name(),dev->Cfg_D()->Name(),(*inp_i).first->Num());
                    static_cast<I7041_Dev*>(dev)->Items[(*inp_i).first->Num()] = static_cast<I7000_Ts*>((*inp_i).first);
                    (*inp_i).second |= ASSIGNED;
                    ts++;
                  }
                  else
                  {
                    dump.Print(D_ERR,"%s: недопустимый номер входа (%u) для модуля %u",(*inp_i).first->Cfg_D()->Name(),(*inp_i).first->Num(),dev->ModuleId());
                  }
                }
            }
            //Определение соотв - я выходов девайсам:
            vector<pair<I7000_Device_Output *,unsigned char> >::iterator item_i;
            for(item_i=D_Os.begin();item_i!=D_Os.end();item_i++)
            {
              if((*item_i).second & OBJ_FAILED)
                continue;
              if(DevNameKey(static_cast<I7000_Device_Item*>((*item_i).first),dev_name))
              {
                if(dev->ModuleId() == 7042)
                  if(dev->MaxItemsAmount() >= (*item_i).first->Num())
                  {
                    const char *ctl_ts_name = static_cast<I7000_Tu*>((*item_i).first)->CtlTsName();
                    const char *net_ts_name = static_cast<I7000_Tu*>((*item_i).first)->EnTsName();
                    dump.Print(D_MSG,"  %s  ->  %s[%u]",(*item_i).first->Cfg_D()->Name(),dev->Cfg_D()->Name(),(*item_i).first->Num());
                    static_cast<I7042_Dev*>(dev)->Items[(*item_i).first->Num()] = static_cast<I7000_Tu*>((*item_i).first);
                    if((inp_i = find_if(D_Is.begin(),D_Is.end(),bind2nd(ItemNameKey(),ctl_ts_name))) != D_Is.end())
                    {
                      static_cast<I7000_Tu*>((*item_i).first)->ts_ctl = const_cast<obj_d*>((*inp_i).first->Cfg_D());
                      dump.Print(D_MSG,"  %s <-> %s (ТС контроля ТУ)",(*item_i).first->Cfg_D()->Name(),(*inp_i).first->Cfg_D()->Name());
                    }
                    else
                    {
                      dump.Print(D_WARN,"  %s: не обнаружен ТС контроля ТУ с именем (%s)",(*item_i).first->Cfg_D()->Name(),ctl_ts_name);
                    }
                    if((inp_i = find_if(D_Is.begin(),D_Is.end(),bind2nd(ItemNameKey(),net_ts_name))) != D_Is.end())
                    {
                      static_cast<I7000_Tu*>((*item_i).first)->ts_ctl = const_cast<obj_d*>((*inp_i).first->Cfg_D());
                      dump.Print(D_MSG,"  %s <-> %s (ТС контроля цепи)",(*item_i).first->Cfg_D()->Name(),(*inp_i).first->Cfg_D()->Name());
                    }
                    else
                    {
                      dump.Print(D_WARN,"  %s: не обнаружен ТС контроля цепи с именем (%s)",(*item_i).first->Cfg_D()->Name(),net_ts_name);
                    }
                    (*item_i).second |= ASSIGNED;
                    tu++;
                  }
              }
            }   
            dev_cnt++;
            (*dev_i).second |= ASSIGNED;
            (*dev_i).first->port = (*cdv_i).first;
            (*cdv_i).first->Items.push_back((*dev_i).first);
            dump.Print(D_MSG,"  %s  ->  %s[%u]",(*dev_i).first->Cfg_D()->Name(),(*cdv_i).first->Cfg_D()->Name(),(*dev_i).first->Addr());
          }          
        }  
        cdv_cnt++;          
        (*cdv_i).second |= ASSIGNED;
        drv->ports.push_back((*cdv_i).first);
      }
    }
  }
  unsigned char killing_flag=(0 | ASSIGNED);  //объект считаю готовым к работе, если установлен флаг ASSIGNED
  //удаляем из векторов объекты, логическая связь которых с другими определена (деструкторы не вызываем!)
  //данные объекты будут удалены только при вызове деструкторов объектов - родителей, либо специальным образом, при реконфигурации 
  for(i=0;i<CDVs.size();i++)
   if(!CDVs[i].second)
    //if((CDVs[i].second & ASSIGNED) == 0)      
    {
      PrintKillingMessage(CDVs[i].first->Cfg_D()->Name());
      delete CDVs[i].first;
    }
  for(i=0;i<Devs.size();i++)
   if(!Devs[i].second)
    //if((Devs[i].second & ASSIGNED) == 0)      
    {
      PrintKillingMessage(Devs[i].first->Cfg_D()->Name());
      delete Devs[i].first;
    }
  for(i=0;i<D_Is.size();i++)
   if(!D_Is[i].second)
    //if((D_Is[i].second & ASSIGNED) == 0)      
    {
      PrintKillingMessage(D_Is[i].first->Cfg_D()->Name());
      delete D_Is[i].first;
    }
  for(i=0;i<D_Os.size();i++)
   if(!D_Os[i].second)
    //if((D_Os[i].second & ASSIGNED) == 0)      
    {
      PrintKillingMessage(D_Os[i].first->Cfg_D()->Name());
      delete D_Os[i].first;
    }
  dump.Print(D_MSG,"\tКоличество портов %d",cdv_cnt);
  dump.Print(D_MSG,"\tКоличество Модулей I7000 %d",dev_cnt);
  dump.Print(D_MSG,"\tКоличество ТC %d",ts);
  //dump.Print(D_MSG,"\tКоличество ТИТ %d",tit_cnt);
  //dump.Print(D_MSG,"\tКоличество ТИИ %d",tii_cnt);
  //dump.Print(D_MSG,"\tКоличество интегрируемых ТИТ %d",titi_cnt);
  dump.Print(D_MSG,"\tКоличество объектов ТУ %d",tu);
  //dump.Print(D_MSG,"\tКоличество ламп щита %d",lgh_cnt);
  if(!drv->ports.size())
  {   

    dump.Print(D_ERR,"\tРабота драйверного модуля невозможна: в конфигурации нет объектов типа %s, либо они неверно описаны!",COMM_DEV.CfgTypeName());  
    drv->Clear();
    result = false;
  }
  else
  {
    dump.Print(D_MSG,"\tРазбор конфигурации: Ok...");
    result = true;
  }
  CDVs.clear();
  Devs.clear();
  D_Is.clear();
  D_Os.clear();  
  return result;
}
//-------------------------------------------------------------
void I7000_Config::CheckForErrors()
{//тупо, однако...
  int i;
  dump.Print(D_MSG,"*** Проверка объектов на ошибки и конфликты ***");
  for(i=0;i<CDVs.size();i++)
  {
    if(CDVs[i].second & OBJ_FAILED)
      continue;
    //удаляем объекты с ошибками:
    if(CommDevWrong(CDVs[i].first))
    {
      PrintKillingMessage(CDVs[i].first->Cfg_D()->Name());
      delete CDVs[i].first;      
      CDVs[i].second |= OBJ_FAILED;
      continue;
    }
    //удаляем конфликтные объекты:
    for(int c=i+1;c<CDVs.size();c++)
      if(!(CDVs[c].second & OBJ_FAILED))
        if(CommDevsEqual(CDVs[i].first,CDVs[c].first))
        {
          PrintKillingMessage(CDVs[i].first->Cfg_D()->Name());
          PrintKillingMessage(CDVs[c].first->Cfg_D()->Name());
          delete CDVs[i].first;
          delete CDVs[c].first;
          CDVs[i].second |= OBJ_FAILED;
          CDVs[c].second |= OBJ_FAILED;
          break;
        }
  }
  //
  for(i=0;i<Devs.size();i++)
  {
    if(Devs[i].second & OBJ_FAILED)
      continue;
    //удаляем конфликтные объекты:
    for(int c=i+1;c<Devs.size();c++)
      if(!(Devs[c].second & OBJ_FAILED))
        if(I7000DevsEqual(Devs[i].first,Devs[c].first))
        {
          PrintKillingMessage(Devs[i].first->Cfg_D()->Name());
          PrintKillingMessage(Devs[c].first->Cfg_D()->Name());
          delete Devs[i].first;
          delete Devs[c].first;
          Devs[i].second |= OBJ_FAILED;
          Devs[c].second |= OBJ_FAILED;
          break;
        }
  }
  //
  for(i=0;i<D_Is.size();i++)
  {
    if(D_Is[i].second & OBJ_FAILED)
      continue;
    //удаляем конфликтные объекты:
    for(int c=i+1;c<D_Is.size();c++)
      if(!(D_Is[c].second & OBJ_FAILED))
        if(I7000InEqual(D_Is[i].first,D_Is[c].first))
        {
          PrintKillingMessage(D_Is[i].first->Cfg_D()->Name());
          PrintKillingMessage(D_Is[c].first->Cfg_D()->Name());
          delete D_Is[i].first;
          delete D_Is[c].first;
          D_Is[i].second |= OBJ_FAILED;
          D_Is[c].second |= OBJ_FAILED;
          break;
        }
  }
  //
  for(i=0;i<D_Os.size();i++)
  {
    if(D_Os[i].second & OBJ_FAILED)
      continue;
    //удаляем конфликтные объекты:
    for(int c=i+1;c<D_Os.size();c++)
      if(!(D_Os[c].second & OBJ_FAILED))
        if(I7000OutEqual(D_Os[i].first,D_Os[c].first))
        {
          PrintKillingMessage(D_Os[i].first->Cfg_D()->Name());
          PrintKillingMessage(D_Os[c].first->Cfg_D()->Name());
          delete D_Os[i].first;
          delete D_Os[c].first;
          D_Os[i].second |= OBJ_FAILED;
          D_Os[c].second |= OBJ_FAILED;
          break;
        }
  }
}
//-------------------------------------------------------------
void I7000_Config::CheckTypeDescription()
{
  type_d *tp;
  TypeDscr *KpTypes[]={
  (TypeDscr *)&COMM_DEV,
  (TypeDscr *)&I7000_TS,
  (TypeDscr *)&I7000_TU,
  (TypeDscr *)&I7041_DEV,
  (TypeDscr *)&I7042_DEV
  };
  dump.Print(D_MSG,"*** Проверка описания типов в текущей конфигурации ***");
  for(int i=0;i<5;i++)
  {
    if(!KpTypes[i]->Init(drv->cfg))
      dump.Print(D_ERR," Ошибка описания полей типа %s!",KpTypes[i]->CfgTypeName());
    else
      dump.Print(D_MSG,"  Описание типа %s: Ok...",KpTypes[i]->CfgTypeName());
  }
  if(COMM_DEV.Flags()!=0 || (I7041_DEV.Flags()!=0 && I7041_DEV.Flags()!=0))
  {
    dump.Print(D_ERR,"  Работа драйверного модуля невозможна!");
  }
  else
  {
    unsigned char add_flag = 0;
    map<unsigned, obj_d *, less<unsigned> >::iterator obj_i;
    //-----------------------------------------------------------------
    //KpCommDev
    tp=(type_d*)drv->cfg->Type(COMM_DEV.CfgId());
    for(obj_i=tp->Objects.begin();obj_i!=tp->Objects.end();obj_i++)
    { 
      I7000_CommDev *comm_dev=new I7000_CommDev(*(*obj_i).second);
      CDVs.push_back(make_pair(comm_dev,add_flag));
      drv->traces.AddTrace((*obj_i).first,COMM_DEV.PortName().CfgFldId());      
      drv->traces.AddTrace((*obj_i).first,COMM_DEV.PortSpeed().CfgFldId());
    }
    //-----------------------------------------------------------------
    //I7041_DEV:
    tp=(type_d*)drv->cfg->Type(I7041_DEV.CfgId());
    for(obj_i=tp->Objects.begin();obj_i!=tp->Objects.end();obj_i++)
    { 
      I7041_Dev *dev=new I7041_Dev(*(*obj_i).second);
      Devs.push_back(make_pair(dev,add_flag));
      drv->traces.AddTrace((*obj_i).first,I7041_DEV.CommDev().CfgFldId());      
      drv->traces.AddTrace((*obj_i).first,I7041_DEV.Address().CfgFldId());
    }
    //-----------------------------------------------------------------
    //I7042_DEV:
    tp=(type_d*)drv->cfg->Type(I7042_DEV.CfgId());
    for(obj_i=tp->Objects.begin();obj_i!=tp->Objects.end();obj_i++)
    { 
      I7042_Dev *dev=new I7042_Dev(*(*obj_i).second);
      Devs.push_back(make_pair(dev,add_flag));
      drv->traces.AddTrace((*obj_i).first,I7042_DEV.CommDev().CfgFldId());      
      drv->traces.AddTrace((*obj_i).first,I7042_DEV.Address().CfgFldId());
    }
    //-----------------------------------------------------------------
    //I7000_TS:
    tp=(type_d*)drv->cfg->Type(I7000_TS.CfgId());
    for(obj_i=tp->Objects.begin();obj_i!=tp->Objects.end();obj_i++)
    { 
      I7000_Ts *dev=new I7000_Ts(*(*obj_i).second);
      D_Is.push_back(make_pair(dev,add_flag));
      drv->traces.AddTrace((*obj_i).first,I7000_TS.Parent().CfgFldId());      
      drv->traces.AddTrace((*obj_i).first,I7000_TS.Num().CfgFldId());
    }
    //-----------------------------------------------------------------
    //I7000_TU:
    tp=(type_d*)drv->cfg->Type(I7000_TU.CfgId());
    for(obj_i=tp->Objects.begin();obj_i!=tp->Objects.end();obj_i++)
    { 
      I7000_Tu *dev=new I7000_Tu(*(*obj_i).second);
      D_Os.push_back(make_pair(dev,add_flag));
      drv->traces.AddTrace((*obj_i).first,I7000_TU.Parent().CfgFldId());      
      drv->traces.AddTrace((*obj_i).first,I7000_TU.Num().CfgFldId());
    }
  }
}
//----------------------------------------------------------------
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%