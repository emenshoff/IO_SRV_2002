#include <kp_types.h>
#include <kp_config.h>
#include <kp_functional.h>
#include <kp_events_cash.h>
#include <io_srv_dump.h>

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
KpConfig::KpConfig(KpControl *c)
:kp_ctl(c)
{
  EventsCash.Update();
}
//-----------------------------------------------------------------
KpConfig::~KpConfig()
{
  ClearAll(true);
}
//-----------------------------------------------------------------
bool KpConfig::Init()
{ 
  CheckTypesDescription();
  //обработка результатов проверки описания типов:
  //проверка на ошибки и конфликты:
  //kp_ctl->Clear();
  //kp_ctl->tracer.ClearTraces();
  CheckForErrors();
  return Create();
}
//-----------------------------------------------------------------
bool KpConfig::Create()
{
  char *ts1,*ts2;
  char *name;
  unsigned char tp;
  time_t init_time_count = 0; //затычка
  int cdv_cnt=0,cnl_cnt=0,kp_cnt=0,fb_ts_cnt=0,
      fb_tit_cnt=0,fb_tii_cnt=0,
      fb_tu_cnt=0,fb_lgh_cnt=0,ts_cnt=0,
      tit_cnt=0,titi_cnt=0,tii_cnt=0,tu_cnt=0,lgh_cnt=0;
  //определение соответствия ТС-ов субблокам:
  dump.Print(D_MSG,"*** Определение логических связей между объектами: ***");
  {
    ObjVectorType::iterator cdv_i;
    //инициализация портов    
    for(cdv_i=CDVs.begin();cdv_i!=CDVs.end();cdv_i++)
    {
      if(IfPortInited()(*cdv_i))
      {
        ObjVectorType::iterator cnl_i;
        //определяем соответствие каналов портам        
        for(cnl_i=CNLs.begin();cnl_i!=CNLs.end();cnl_i++)
         if(CnlParentNameKey()(*cnl_i,(*cdv_i).first->Cfg_D()->Name()))
        {
          unsigned cnl_num = static_cast<KpCnl*>((*cnl_i).first)->Num();
          dump.Print(D_MSG,"  %s  ->  %s [%u]",(*cnl_i).first->Cfg_D()->Name(),(*cdv_i).first->Cfg_D()->Name(),cnl_num);
          ObjVectorType::iterator kp_i;
          //определение соответствия КП каналам:
          for(kp_i=KPs.begin();kp_i!=KPs.end();kp_i++)
           if(KpCnlNameKey()(*kp_i,(*cnl_i).first->Cfg_D()->Name()))
          {
            init_time_count = static_cast<KP*>((*kp_i).first)->ResponceTimeoutVal();
            if(IO_InitDevTime < init_time_count)
              IO_InitDevTime = init_time_count;
            dump.Print(D_MSG,"  %s  ->  %s[%d]",(*kp_i).first->Cfg_D()->Name(),(*cnl_i).first->Cfg_D()->Name(),static_cast<KP*>((*kp_i).first)->Address());
            ObjVectorType::iterator fb_i;
            //определение соответствия субблоков КП:
            for(fb_i=FBs.begin();fb_i!=FBs.end();fb_i++)
             if(FbParentNameKey()(*fb_i,(*kp_i).first->Cfg_D()->Name()))
            {
              dump.Print(D_MSG,"  %s  ->  %s [%d]",(*fb_i).first->Cfg_D()->Name(),(*kp_i).first->Cfg_D()->Name(),static_cast<AFB*>((*fb_i).first)->Place());
              ObjVectorType::iterator sensor_i;              
              //определяем тип субблока
              switch((*fb_i).first->Type())
              {
                case FB_TS:
                  //определение соответствия тс-ов субблокам:
                  for(sensor_i=TSs.begin();sensor_i!=TSs.end();sensor_i++)
                   if(TsParentNameKey()(*sensor_i,(*fb_i).first->Cfg_D()->Name()))
                  {
                     unsigned num = ((TSCtl*)(*sensor_i).first)->Num();
                    dump.Print(D_MSG,"  %s  ->  %s [%u]",(*sensor_i).first->Cfg_D()->Name(),(*fb_i).first->Cfg_D()->Name(),num);
                    ts_cnt++;
                    (*sensor_i).second |= ASSIGNED;
                    ((FbTs*)(*fb_i).first)->Items[num-1]=(TSCtl*)(*sensor_i).first;
                  } 
                  fb_ts_cnt++;
                  break;
                case FB_TIT:
                  //определение соответствия ТИТ субблокам:
                  for(sensor_i=TITs.begin();sensor_i!=TITs.end();sensor_i++)
                   if(TitParentNameKey()(*sensor_i,(*fb_i).first->Cfg_D()->Name()))
                  {  
                    unsigned num = ((TITCtl*)(*sensor_i).first)->Num();
                    ((FbTit*)(*fb_i).first)->Items[num-1]=(TITCtl*)(*sensor_i).first;
                    ObjVectorType::iterator itg_i;
                    //определение соответствия интегриуемых ТИТ-ов текущим ТИТ-ам:
                    name=(char*)(const char*)(*sensor_i).first->Cfg_D()->GetFldValue(KP_TIT.TITid_Name().CfgFldId(),tp);
                    if((itg_i=find_if(TITis.begin(),TITis.end(),bind2nd(CtlObjNameKey(),(const char*)name)))!=TITis.end())
                    {                     
                      ((TITCtl*)(*sensor_i).first)->titi_d=(TITiCtl*)(*itg_i).first;
                      kp_ctl->ItgTits.push_back((TITiCtl*)(*itg_i).first);
                      (*itg_i).second  |= ASSIGNED;
                      titi_cnt++;
                      dump.Print(D_MSG,"  %s  ->  %s",(*itg_i).first->Cfg_D()->Name(),(*sensor_i).first->Cfg_D()->Name());
                    }
                    else
                    {
                      dump.Print(D_ERR,"  Не найден объект с именем %s! Вычисление интегрируемых ТИ для %s невозможно!",(const char*)name,(*sensor_i).first->Cfg_D()->Name());
                      ((TITCtl*)(*sensor_i).first)->titi_d=0;
                    }
                    tit_cnt++;
                    (*sensor_i).second |= ASSIGNED;
                    dump.Print(D_MSG,"  %s  ->  %s [%u]",(*sensor_i).first->Cfg_D()->Name(),(*fb_i).first->Cfg_D()->Name(),num);
                  }
                  fb_tit_cnt++;
                  break;
                case FB_TII:
                  //определение соответствия интегр. ТИТ субблокам:
                  for(sensor_i=TIIs.begin();sensor_i!=TIIs.end();sensor_i++)
                   if(TiiParentNameKey()(*sensor_i,(*fb_i).first->Cfg_D()->Name()))
                  {
                     unsigned num = ((TIICtl*)(*sensor_i).first)->Num();
                    ((FbTii*)(*fb_i).first)->Items[num-1]=(TIICtl*)(*sensor_i).first;
                    (*sensor_i).second |= ASSIGNED;
                    tii_cnt++;
                    dump.Print(D_MSG,"  %s  ->  %s[%d]",(*sensor_i).first->Cfg_D()->Name(),(*fb_i).first->Cfg_D()->Name(),num);
                  }
                  fb_tii_cnt++;
                  break;
                case FB_TU:
                  //определение соответствия ТУ субблокам:
                  for(sensor_i=TUs.begin();sensor_i!=TUs.end();sensor_i++)
                   if(TuParentNameKey()(*sensor_i,(*fb_i).first->Cfg_D()->Name()))
                  {
                    ObjVectorType::iterator ctl_ts;
                    //ищем ТС контроля телеуправления
                    name=(char*)(*sensor_i).first->Cfg_D()->GetFldValue(KP_TU.TU_Confirm_TS().CfgFldId(),tp);
                    ts1=(char*)name;
                    if((ctl_ts=find_if(TSs.begin(),TSs.end(),bind2nd(CtlObjNameKey(),ts1)))!=TSs.end())
                    {
                      static_cast<TUCtl*>((*sensor_i).first)->ts_ctl=const_cast<obj_d*>((*ctl_ts).first->Cfg_D());
                      dump.Print(D_MSG,"  %s <-> %s (тс контроля ту)",(*sensor_i).first->Cfg_D()->Name(),(*ctl_ts).first->Cfg_D()->Name());
                      //ищем тс разрешения телеуправления:
                      name=(char*)(*sensor_i).first->Cfg_D()->GetFldValue(KP_TU.TU_Enable_TS().CfgFldId(),tp);
                      ts2=(char*)name;
                      if((ctl_ts=find_if(TSs.begin(),TSs.end(),bind2nd(CtlObjNameKey(),ts2)))!=TSs.end())
                      {
                        unsigned num = static_cast<TUCtl*>((*sensor_i).first)->Num();
                        static_cast<TUCtl*>((*sensor_i).first)->ts_net_ctl=const_cast<obj_d*>((*ctl_ts).first->Cfg_D());
                        dump.Print(D_MSG,"  %s <-> %s (тс разрешения ту)",(*sensor_i).first->Cfg_D()->Name(),(*ctl_ts).first->Cfg_D()->Name());
                        static_cast<FbTu*>((*fb_i).first)->Items[num-1]=static_cast<TUCtl*>((*sensor_i).first);
                        (*sensor_i).second |= ASSIGNED;
                        tu_cnt++;
                        dump.Print(D_MSG,"  %s  ->  %s [%u]",(*sensor_i).first->Cfg_D()->Name(),(*fb_i).first->Cfg_D()->Name(),num);
                      }
                      else
                      {
                        dump.Print(D_ERR,"  Не найден ТС разрешения телеуправления %s объекта %s!",ts2,(*sensor_i).first->Cfg_D()->Name());
                        (*sensor_i).second |= OBJ_FAILED;
                      }
                    }
                    else
                    {
                      dump.Print(D_ERR,"  Не найден ТС контроля телеуправления %s объекта %s!",ts1,(*sensor_i).first->Cfg_D()->Name());
                      (*sensor_i).second |= OBJ_FAILED;
                    }
                  }
                  fb_tu_cnt++;
                  break;
                case FB_LIGHT:
                  //определение соответствия ламп субблокам:
                  for(sensor_i=LGHs.begin();sensor_i!=LGHs.end();sensor_i++)
                   if(LghParentNameKey()(*sensor_i,(*fb_i).first->Cfg_D()->Name()))
                  {
                    unsigned num=((LightCtl*)(*sensor_i).first)->Num();
                    if((static_cast<FbLight*>((*fb_i).first)->MaxItems() < num))
                    {
                      dump.Print(D_ERR,"  Объект %s имеет недопустимый номер для данного типа субблока: %d!",(*sensor_i).first->Cfg_D()->Name(),num);
                      (*sensor_i).second |= OBJ_FAILED;
                    }
                    else
                    {
                      static_cast<FbLight*>((*fb_i).first)->Items[num-1]=static_cast<LightCtl*>((*sensor_i).first);
                      (*sensor_i).second |= ASSIGNED;
                      lgh_cnt++;
                      dump.Print(D_MSG,"  %s  ->  %s [%u]",(*sensor_i).first->Cfg_D()->Name(),(*fb_i).first->Cfg_D()->Name(),num);
                    }
                  }
                  fb_lgh_cnt++;
                  break;
                default:
                  break;
              }
              //добавление субблока в КП:
              unsigned place = static_cast<AFB*>((*fb_i).first)->Place();
              static_cast<AFB*>((*fb_i).first)->parent=static_cast<KP*>((*kp_i).first);
              static_cast<KP*>((*kp_i).first)->Items[place == 16 ? 0 : place]=static_cast<AFB*>((*fb_i).first);
              (*fb_i).second |= ASSIGNED;
            }
            //запись номера канала КП:            
            static_cast<KP*>((*kp_i).first)->cnl=static_cast<KpCnl*>((*cnl_i).first);
            static_cast<KpCnl*>((*cnl_i).first)->parent = static_cast<KpCommDev*>((*cdv_i).first);
            static_cast<KpCnl*>((*cnl_i).first)->Items[static_cast<KP*>((*kp_i).first)->Address()]=static_cast<KP*>((*kp_i).first);
            (*kp_i).second |= ASSIGNED;
            kp_cnt++;
          }
          //добавили канал в порт:
          static_cast<KpCommDev*>((*cdv_i).first)->channels[cnl_num]=static_cast<KpCnl*>((*cnl_i).first);
          (*cnl_i).second |= ASSIGNED;
          cnl_cnt++;
        }
        static_cast<KpCommDev*>((*cdv_i).first)->InitChannels();
        kp_ctl->Ports.push_back(static_cast<KpCommDev*>((*cdv_i).first));
        (*cdv_i).second |= ASSIGNED;
        cdv_cnt++;
      }
    }
  }
  unsigned char killing_flag=(0 | ASSIGNED);  //объект считаю готовым к работе, если установлен флаг ASSIGNED
  //удаляем из векторов объекты, логическая связь которых с другими определена (деструкторы не вызываем!)
  //данные объекты будут удалены только при вызове деструкторов объектов - родителей, либо специальным образом, при реконфигурации 
  Clear(CDVs,killing_flag,false);
  Clear(CNLs,killing_flag,false);
  Clear(KPs,killing_flag,false);
  Clear(FBs,killing_flag,false);
  Clear(TSs,killing_flag,false);
  Clear(TITs,killing_flag,false);
  Clear(TITis,killing_flag,false);
  Clear(TIIs,killing_flag,false);
  Clear(TUs,killing_flag,false);
  Clear(LGHs,killing_flag,false);
  //удаляем то, что осталось (объекты с ошибками и неопределенным назначением) (с вызовом деструкторов)
  ClearAll(true);
  dump.Print(D_MSG,"********************************************************");
  dump.Print(D_MSG,"  Количество портов %d",cdv_cnt);
  dump.Print(D_MSG,"  Количество каналов %d",cnl_cnt);
  dump.Print(D_MSG,"  Количество КП %d",kp_cnt);
  dump.Print(D_MSG,"  Количество ТC %d",ts_cnt);
  dump.Print(D_MSG,"  Количество ТИТ %d",tit_cnt);
  dump.Print(D_MSG,"  Количество ТИИ %d",tii_cnt);
  dump.Print(D_MSG,"  Количество интегрируемых ТИТ %d",titi_cnt);
  dump.Print(D_MSG,"  Количество объектов ТУ %d",tu_cnt);
  dump.Print(D_MSG,"  Количество ламп щита %d",lgh_cnt);
  if(cnl_cnt && cdv_cnt && kp_cnt)
  {     
    dump.Print(D_MSG,"Разбор конфигурации: Ok...");
    dump.Print(D_MSG,"********************************************************");
    return true;    
  }
  dump.Print(D_ERR,"Работа драйверного модуля невозможна: в конфигурации нет объектов типа %s, либо %s, либо %s  либо они неверно описаны!",COMM_DEV.CfgTypeName(),KP_DEV.CfgTypeName(),KP_CHANNEL.CfgTypeName());
  /*kp_ctl->Clear();
  kp_ctl->tracer.ClearTraces();*/
  dump.Print(D_MSG,"********************************************************");
  return false;  
}
//-----------------------------------------------------------------
void KpConfig::PrintKillingMessage(const char *name)
{
  dump.Print(D_WARN," Объект %s удален!",name);
}
//-----------------------------------------------------------------
void KpConfig::Clear(ObjVectorType &vk,bool must_die)
{
  ObjVectorType::iterator obj_i;
  for(obj_i=vk.begin();obj_i!=vk.end();obj_i++)
  {
    if(must_die)
    {
      PrintKillingMessage((*obj_i).first->Cfg_D()->Name());
      delete (*obj_i).first;
    }
  }
  vk.clear();
}
//-----------------------------------------------------------------
void KpConfig::Clear(ObjVectorType &vk,unsigned char flag,bool must_die)
{
  ObjVectorType tmp;
  ObjVectorType::iterator obj_i;
  for(obj_i=vk.begin();obj_i!=vk.end();obj_i++)
  {
    if(IfFlagIsSet()(*obj_i,flag))
    {
      if(must_die)
      {
        PrintKillingMessage((*obj_i).first->Cfg_D()->Name());
        delete (*obj_i).first;
        (*obj_i).first=0;
      }
    }
    else
      tmp.push_back(*obj_i);
  }
  vk.clear();
  copy(tmp.begin(),tmp.end(),inserter(vk,vk.begin()));
  tmp.clear();
}
//-----------------------------------------------------------------
void KpConfig::CheckForErrors(ObjVectorType &Objects)
{
  for(int i=0;i<Objects.size();i++)
  {
    if(!(Objects[i].second & OBJ_FAILED))
    {
      //удаляем объекты с ошибками:
      if(ChkCtlObjectForWrong()(Objects[i]))
      {
        Objects[i].second |= OBJ_FAILED;
        continue;
      }
      //удаляем конфликтные объекты:
      for(int c=i+1;c<Objects.size();c++)
       if(!(Objects[c].second & OBJ_FAILED))
        if(ChkForCtlObjectEqual()(Objects[i],Objects[c]))
        {
          Objects[i].second |= OBJ_FAILED;
          Objects[c].second |= OBJ_FAILED;
        }
    }
  }
  Clear(Objects,OBJ_FAILED,true);
}
//-----------------------------------------------------------------
void KpConfig::CheckForErrors()
{
  dump.Print(D_MSG,"*** Проверка объектов на ошибки и конфликты ***");
  CheckForErrors(CDVs);
  CheckForErrors(CNLs);
  CheckForErrors(KPs);
  CheckForErrors(FBs);
  CheckForErrors(TSs);
  CheckForErrors(TITs);
  CheckForErrors(TITis);
  CheckForErrors(TIIs);
  CheckForErrors(TUs);
  CheckForErrors(LGHs);  
}
//-----------------------------------------------------------------
void KpConfig::ClearAll(bool must_die)
{
  Clear(CDVs,must_die);
  Clear(CNLs,must_die);
  Clear(KPs,must_die);
  Clear(FBs,must_die);
  Clear(TSs,must_die);
  Clear(TITs,must_die);
  Clear(TITis,must_die);
  Clear(TIIs,must_die);
  Clear(TUs,must_die);
  Clear(LGHs,must_die);  
}
//-----------------------------------------------------------------
//решение проблемы "в лоб" :)
void KpConfig::CheckTypesDescription()
{
  type_d *tp;
  TypeDscr *KpTypes[]={
  (TypeDscr *)&COMM_DEV,
  (TypeDscr *)&KP_CHANNEL,
  (TypeDscr *)&KP_DEV,
  (TypeDscr *)&KP_AFB_TS,
  (TypeDscr *)&KP_AFB_TIT,
  (TypeDscr *)&KP_AFB_TII,
  (TypeDscr *)&KP_AFB_TU,
  (TypeDscr *)&KP_AFB_LIGHT,
  (TypeDscr *)&KP_TS,
  (TypeDscr *)&KP_TIT,
  (TypeDscr *)&KP_TII,
  (TypeDscr *)&KP_TITi,
  (TypeDscr *)&KP_LIGHT,
  (TypeDscr *)&KP_TU
  };
  unsigned char add_flag=0;
  dump.Print(D_MSG,"*** Проверка описания типов в текущей конфигурации ***");
  map<unsigned, obj_d *, less<unsigned> >::iterator obj_i;
  for(int i=0;i<14;i++)
  {
    if(!KpTypes[i]->Init(kp_ctl->cfg))
      dump.Print(D_ERR," Ошибка описания полей типа %s!",KpTypes[i]->CfgTypeName());
    else
      dump.Print(D_MSG,"  Описание типа %s: Ok...",KpTypes[i]->CfgTypeName());
  }
  //-----------------------------------------------------------------
  //
  if(COMM_DEV.Flags()!=0 || KP_DEV.Flags()!=0 || KP_CHANNEL.Flags()!=0)
  {
    dump.Print(D_ERR,"  Работа драйверного модуля невозможна!");
  }
  else
  {
    //-----------------------------------------------------------------
    //KpCommDev
    tp=(type_d*)kp_ctl->cfg->Type(COMM_DEV.CfgId());
    for(obj_i=tp->Objects.begin();obj_i!=tp->Objects.end();obj_i++)
    { 
      KpCommDev *comm_dev=new KpCommDev(*(*obj_i).second);
      CDVs.push_back(pair<ControlObject *,unsigned char>(comm_dev,add_flag));
      kp_ctl->tracer.AddTrace((*obj_i).first,COMM_DEV.PortName().CfgFldId());      
      kp_ctl->tracer.AddTrace((*obj_i).first,COMM_DEV.PortSpeed().CfgFldId());
    }
    //-----------------------------------------------------------------
    //Channels
    tp=(type_d*)kp_ctl->cfg->Type(KP_CHANNEL.CfgId());
    for(obj_i=tp->Objects.begin();obj_i!=tp->Objects.end();obj_i++)
    { 
      KpCnl *cnl=new KpCnl(*(*obj_i).second);
      CNLs.push_back(pair<ControlObject *,unsigned char>(cnl,add_flag));
      kp_ctl->tracer.AddTrace((*obj_i).first,KP_CHANNEL.CommDev().CfgFldId());
      kp_ctl->tracer.AddTrace((*obj_i).first,KP_CHANNEL.KpSpeed().CfgFldId());
      kp_ctl->tracer.AddTrace((*obj_i).first,KP_CHANNEL.Number().CfgFldId());
    }
    //-----------------------------------------------------------------
    //KP
    tp=(type_d*)kp_ctl->cfg->Type(KP_DEV.CfgId());
    for(obj_i=tp->Objects.begin();obj_i!=tp->Objects.end();obj_i++)
    {
      KP *kp_dv=new KP(*(*obj_i).second);
      KPs.push_back(pair<ControlObject *,unsigned char>(kp_dv,add_flag));
      kp_ctl->tracer.AddTrace((*obj_i).first,KP_DEV.CnlName().CfgFldId());
      kp_ctl->tracer.AddTrace((*obj_i).first,KP_DEV.Address().CfgFldId()); 
    }
  }
  //-----------------------------------------------------------------
  //
  if(KP_AFB_TS.Flags()!=0 || KP_TS.Flags()!=0)
  {
    dump.Print(D_ERR,"  Обработка телесигналов невозможна!");
    dump.Print(D_ERR,"  Обработка телеуправления невозможна!");
  }
  else
  {
    //-----------------------------------------------------------------
    //FbTs
    tp=(type_d*)kp_ctl->cfg->Type(KP_AFB_TS.CfgId());
    for(obj_i=tp->Objects.begin();obj_i!=tp->Objects.end();obj_i++)
    {
      FbTs *fb=new FbTs(*(*obj_i).second);
      FBs.push_back(pair<ControlObject *,unsigned char>(fb,add_flag));
      kp_ctl->tracer.AddTrace((*obj_i).first,KP_AFB_TS.Parent().CfgFldId());
      kp_ctl->tracer.AddTrace((*obj_i).first,KP_AFB_TS.Place().CfgFldId());
    }
    //-----------------------------------------------------------------
    //TSCtl
    tp=(type_d*)kp_ctl->cfg->Type(KP_TS.CfgId());
    for(obj_i=tp->Objects.begin();obj_i!=tp->Objects.end();obj_i++)
    {
      TSCtl *fb=new TSCtl(*(*obj_i).second);
      TSs.push_back(pair<ControlObject *,unsigned char>(fb,add_flag));
      kp_ctl->tracer.AddTrace((*obj_i).first,KP_TS.Num().CfgFldId());
      kp_ctl->tracer.AddTrace((*obj_i).first,KP_TS.Parent().CfgFldId());
    }
    if(KP_AFB_TU.Flags()!=0 || KP_TU.Flags()!=0)
    {
      dump.Print(D_ERR,"  Обработка телеуправления невозможна!");
    }
    else
    {
      //-----------------------------------------------------------------
      //FbTu
      tp=(type_d*)kp_ctl->cfg->Type(KP_AFB_TU.CfgId());
      //if(tp)
      for(obj_i=tp->Objects.begin();obj_i!=tp->Objects.end();obj_i++)
      { 
        FbTu *fb=new FbTu(*(*obj_i).second);
        FBs.push_back(pair<ControlObject *,unsigned char>(fb,add_flag));
        kp_ctl->tracer.AddTrace((*obj_i).first,KP_AFB_TU.Place().CfgFldId());
        kp_ctl->tracer.AddTrace((*obj_i).first,KP_AFB_TU.Parent().CfgFldId());
        kp_ctl->tracer.AddTrace((*obj_i).first,KP_AFB_TU.Num().CfgFldId());
        kp_ctl->tracer.AddTrace((*obj_i).first,KP_AFB_TU.Num().CfgFldId());
      }
      //-----------------------------------------------------------------
      //TU
      tp=(type_d*)kp_ctl->cfg->Type(KP_TU.CfgId());
      //if(tp)
      for(obj_i=tp->Objects.begin();obj_i!=tp->Objects.end();obj_i++)
      {
        TUCtl *fb=new TUCtl(*(*obj_i).second);
        TUs.push_back(pair<ControlObject *,unsigned char>(fb,add_flag));
        kp_ctl->tracer.AddTrace((*obj_i).first,KP_TU.Num().CfgFldId());
        kp_ctl->tracer.AddTrace((*obj_i).first,KP_TU.Parent().CfgFldId());
        kp_ctl->tracer.AddTrace((*obj_i).first,KP_TU.GroupNum().CfgFldId());
        kp_ctl->tracer.AddTrace((*obj_i).first,KP_TU.TU_Enable_TS().CfgFldId());
        kp_ctl->tracer.AddTrace((*obj_i).first,KP_TU.TU_Confirm_TS().CfgFldId());
      }
    }
  }
  //-----------------------------------------------------------------
  //
  if(KP_AFB_TIT.Flags()!=0 || KP_TIT.Flags()!=0)
  {
    dump.Print(D_ERR,"  Обработка телеизмерений текущих невозможна!");
    dump.Print(D_ERR,"  Обработка телеизмерений интегрируемых невозможна!");
  }
  else
  {
    //-----------------------------------------------------------------
    //FB_TIT
    tp=(type_d*)kp_ctl->cfg->Type(KP_AFB_TIT.CfgId());
    for(obj_i=tp->Objects.begin();obj_i!=tp->Objects.end();obj_i++)
    {
      FbTit *fb=new FbTit(*(*obj_i).second);
      FBs.push_back(pair<ControlObject *,unsigned char>(fb,add_flag));
      kp_ctl->tracer.AddTrace((*obj_i).first,KP_AFB_TIT.Place().CfgFldId());
      kp_ctl->tracer.AddTrace((*obj_i).first,KP_AFB_TIT.Parent().CfgFldId());    
    }
    //-----------------------------------------------------------------
    //TIT
    tp=(type_d*)kp_ctl->cfg->Type(KP_TIT.CfgId());
    for(obj_i=tp->Objects.begin();obj_i!=tp->Objects.end();obj_i++)
    {
      TITCtl *fb=new TITCtl(*(*obj_i).second);
      TITs.push_back(pair<ControlObject *,unsigned char>(fb,add_flag));
      kp_ctl->tracer.AddTrace((*obj_i).first,KP_TIT.Num().CfgFldId());
      kp_ctl->tracer.AddTrace((*obj_i).first,KP_TIT.Parent().CfgFldId());
      kp_ctl->tracer.AddTrace((*obj_i).first,KP_TIT.TITid_Name().CfgFldId());
    }
    //-----------------------------------------------------------------
    //TITi
    if(KP_TITi.Flags()!=0)
    {
      dump.Print(D_ERR,"  Обработка телеизмерений интегрируемых невозможна!");
    }
    else
    {
      tp=(type_d*)kp_ctl->cfg->Type(KP_TITi.CfgId());
      for(obj_i=tp->Objects.begin();obj_i!=tp->Objects.end();obj_i++)
      {
        TITiCtl *fb=new TITiCtl(*(*obj_i).second);
        TITis.push_back(pair<ControlObject *,unsigned char>(fb,add_flag));
      }
    }
  }
  //-----------------------------------------------------------------
  //
  if(KP_AFB_TII.Flags()!=0 || KP_TII.Flags()!=0)
  {
    dump.Print(D_ERR,"  Обработка интегральных телеизмерений невозможна!");
  }
  else
  {
    //-----------------------------------------------------------------
    //FB_TII
    tp=(type_d*)kp_ctl->cfg->Type(KP_AFB_TII.CfgId());
    for(obj_i=tp->Objects.begin();obj_i!=tp->Objects.end();obj_i++)
    {
      FbTii *fb=new FbTii(*(*obj_i).second);
      FBs.push_back(pair<ControlObject *,unsigned char>(fb,add_flag));
      kp_ctl->tracer.AddTrace((*obj_i).first,KP_AFB_TII.Place().CfgFldId());
      kp_ctl->tracer.AddTrace((*obj_i).first,KP_AFB_TII.Parent().CfgFldId());    
    }
    //-----------------------------------------------------------------
    //TII
    tp=(type_d*)kp_ctl->cfg->Type(KP_TII.CfgId());
    for(obj_i=tp->Objects.begin();obj_i!=tp->Objects.end();obj_i++)
    {
      TIICtl *fb=new TIICtl(*(*obj_i).second);
      TIIs.push_back(pair<ControlObject *,unsigned char>(fb,add_flag));
      kp_ctl->tracer.AddTrace((*obj_i).first,KP_TII.Num().CfgFldId());
      kp_ctl->tracer.AddTrace((*obj_i).first,KP_TII.Parent().CfgFldId());
    }
  }
  //-----------------------------------------------------------------
  //
  if(KP_AFB_LIGHT.Flags()!=0 || KP_LIGHT.Flags()!=0)
  {
    dump.Print(D_ERR,"  Вывод на щит невозможен!");
  }
  else
  {
    //-----------------------------------------------------------------
    //FB_LGH
    tp=(type_d*)kp_ctl->cfg->Type(KP_AFB_LIGHT.CfgId());
    for(obj_i=tp->Objects.begin();obj_i!=tp->Objects.end();obj_i++)
    {
      unsigned char type,tp;
      type=*(unsigned char*)(*obj_i).second->GetFldValue(KP_AFB_LIGHT.Type().CfgFldId(),tp);
      if((type != 32) && (type != 64))
      {
        dump.Print(D_ERR,"  Неверный тип субблока %s: %d. Данный объект не отслеживаю!",(*obj_i).second->Name(),type);
        continue;
      }
      FbLight *fb=new FbLight(*(*obj_i).second);
      FBs.push_back(pair<ControlObject *,unsigned char>(fb,add_flag));
      kp_ctl->tracer.AddTrace((*obj_i).first,KP_AFB_LIGHT.Type().CfgFldId());
      kp_ctl->tracer.AddTrace((*obj_i).first,KP_AFB_LIGHT.Place().CfgFldId());
      kp_ctl->tracer.AddTrace((*obj_i).first,KP_AFB_LIGHT.Parent().CfgFldId());
    }
    //-----------------------------------------------------------------
    //LGH
    tp=(type_d*)kp_ctl->cfg->Type(KP_LIGHT.CfgId());
    for(obj_i=tp->Objects.begin();obj_i!=tp->Objects.end();obj_i++)
    {
      LightCtl *fb=new LightCtl(*(*obj_i).second,kp_ctl->parser);
      LGHs.push_back(pair<ControlObject *,unsigned char>(fb,add_flag));
      kp_ctl->tracer.AddTrace((*obj_i).first,KP_LIGHT.Num().CfgFldId());
      kp_ctl->tracer.AddTrace((*obj_i).first,KP_LIGHT.Parent().CfgFldId());
      //kp_ctl->tracer.AddTrace((*obj_i).first,KP_LIGHT.Condition().CfgFldId());
    }
  }
}
//-----------------------------------------------------------------
