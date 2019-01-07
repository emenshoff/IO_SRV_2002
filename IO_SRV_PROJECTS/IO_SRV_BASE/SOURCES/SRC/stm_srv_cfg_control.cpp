/***************************************************************************
        Прошу прощения за громоздкость сей писанины...
                       M.E.N.
***************************************************************************/

#include <stdio.h>

#ifdef WIN32
# include <windows.h>
#else
# include <fcntl.h>
# include <unistd.h>
# include <sys/stat.h>
# include <sys/mman.h>
#endif

#include <val.h>
#include <io_srv_dump.h>
#include <stm_srv_cfg_control.h>

#define tm_link 1000

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
ConfCreator::ConfCreator(const char *file_name):CfgFileName(0),cfg(0)
{
  if(!file_name)
  {  
    dump.Print("Недопустимое имя файла конфигурации!");
  }
  else
    CfgFileName=strdup(file_name);
}
//----------------------------------------------------------------------------
ConfCreator::~ConfCreator()
{
  free(CfgFileName);
  ClearInits();
}
//----------------------------------------------------------------------------
StmCfg *ConfCreator::ConfCreate()
{
  unsigned i;
  unsigned char ver;
  unsigned long count;
  BinData tmp_bin;
  DataBlock * blk=0,b,b2;
#ifdef WIN32
  HANDLE hFile=0;
  HANDLE hMap=0;
  LPVOID lpvFile=0;
  hFile = ::CreateFile(CfgFileName, GENERIC_READ, FILE_SHARE_READ,
        NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if(hFile==INVALID_HANDLE_VALUE)
    goto fail;
  hMap = ::CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
  if(hMap == NULL)
    goto fail;
  lpvFile = ::MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, 0);
  if(lpvFile == NULL)
    goto fail;
  blk= new DataBlock((char *)lpvFile,BlkUse);
#else
//UNIX
  struct stat sb;
  int fd,f_size;
  void *file_map;
  if((fd=open(CfgFileName,O_RDONLY|O_NONBLOCK))<0)
    goto fail;
  stat(CfgFileName,&sb);
  f_size=sb.st_size;
  if((file_map=mmap(0,f_size,PROT_READ,MAP_SHARED,fd,0))==MAP_FAILED)
    goto fail;
  blk= new DataBlock((char *)file_map,BlkUse);
#endif
  if(!(*blk>>ver))  //Достаем версию..
    goto fail;
  if(!(*blk>>tmp_bin))//имя конфы
    goto fail;
  cfg=new StmCfg(tmp_bin.c_str(),1,ver);
  for(i=0;i<3;i++)//пропускаем ненужное 
    if(!blk->Next()) goto fail;
  if(!(*blk>>tmp_bin)) //путь к конф. файлу для дампа
    goto fail;
  DumpCfgFileName = tmp_bin.c_str();
  if(!(*blk>>tmp_bin)) //путь к файлу дампа
    goto fail;
  DumpFileName = tmp_bin.c_str();
  for(i=0;i<4;i++)//пропускаем ненужное 
    if(!blk->Next()) goto fail;
  if(!(*blk>>count))//Source count
    goto fail;
  for(i=0;i<count;i++)//Sources skipping
    if(!blk->Next()) goto fail;
  if(!(*blk>>count))//Streams count
    goto fail;
  for(i=0;i<count;i++)//Streams
    if(!blk->Next()) goto fail;
  if(!(*blk>>count))//Types count
    goto fail;
  for(i=0;i<count;i++)//добавляем типы
  {
    if(!(*blk>>b))
      goto fail;
    if(!LoadType(b))
      goto fail;
  }
  if(!(*blk>>count))//Clusters count
    goto fail;
  for(i=0;i<count;i++)//добавляем кластеры доступа
  {
    if(!(*blk>>b))
      goto fail;
    if(!LoadCluster(b))
      goto fail;
  }
  if(!(*blk>>count))//Groups count
    goto fail;
  for(i=0;i<count;i++)//добавляем группы юзверей и самих юзверей
  {
    if(!(*blk>>b))
      goto fail;
    if(!LoadGroup(b))
      goto fail;
  }
  delete blk;//Модули и события даже не трогаем...
  blk=0;
#ifdef WIN32
  ::UnmapViewOfFile(lpvFile);
  ::CloseHandle(hMap);
  ::CloseHandle(hFile);
#else
  if(fd>0)
    close(fd);
  if(file_map!=MAP_FAILED)
    munmap(file_map,f_size);
#endif
  if(!InitValues())
    goto fail;
  ClearInits();
  dump.Print("Инициализация конфигурации сервера: Ok...");
  dump.Print("Число объектов в конфигурации сервера: %d",cfg->ObjectsCount());
  return cfg;
  fail:
  {
    dump.Print("Ошибка разбора конфигурации сервера!");
    if(cfg)delete cfg;
    if(blk)delete blk;
#ifdef WIN32
    ::UnmapViewOfFile(lpvFile);
    ::CloseHandle(hMap);
    ::CloseHandle(hFile);
#else
  if(fd>0)
    close(fd);
  if(file_map!=MAP_FAILED)
    munmap(file_map,f_size);
#endif
    ClearInits();
    return 0;
  }
}
//---------------------------------------------------------------------------
bool ConfCreator::LoadType(DataBlock &b)
{
//Обращаю внимание: некоторые переменные применяются для разных целей, и их название может сбить с толку...
  if(!cfg) return false;
  unsigned i,type_id,fld_id,obj_id;
  unsigned char IsInit=0;
  unsigned short base_type;
  unsigned long count;
  BinData tmp_bin,init_data,comment;
  DataBlock init_blk,b2,b3;

  map<unsigned,BinData*,less<unsigned> > *Map = 0;
  map<char *,unsigned,compare_key >::const_iterator cl;
  if((!b.Next())||!(b>>tmp_bin))
    goto fail;
  if((type_id=cfg->AddType(tmp_bin.c_str()))==0)
    goto fail;
  //printf("Добавлен тип %s\n",tmp_bin.c_str());
  for(i=0;i<4;i++)//пропуск ненужного
    if(!b.Next())
      goto fail;
  if(!(b>>count))//число полей
    goto fail;
  //Logdef(Print(DEBUG_ERROR,"Тип %s:",tmp_bin.c_str()))
  Map=new map<unsigned,BinData*,less<unsigned> >();
  for(i=0;i<count;i++)
  {
    if(!(b>>b2))
      goto fail;
    if((!b2.Next())||(!(b2>>tmp_bin))||(!b2.Next())||(!b2.Next())||(!(b2>>base_type))||(!(b2>>init_data)))
      goto fail;
    if(base_type == tm_link)
    {
       base_type=tm_string;
       init_data="";
    }
    fld_id=cfg->AddFld(type_id,base_type,tmp_bin.c_str());
    Map->insert(pair<unsigned,BinData *>(fld_id,new BinData(init_data)));
    //Logdef(Print(DEBUG_ERROR,"Добавлено поле %s баз. тип %u",tmp_bin.c_str(),base_type))
    //printf("Добавлено поле %s баз. тип %u\n",tmp_bin.c_str(),base_type);
  }
  if(i!=0)
    Inits.insert(pair<unsigned,map<unsigned,BinData*,less<unsigned> >*>(type_id,Map));// правда красиво? :)
  else
  {
    delete Map;
    Map = 0;
  }
  b2.Free();
  if(!(b>>count))//число объектов
    goto fail;
  for(i=0;i<count;i++)
  {
    if(!(b>>b2))
      goto fail;
    if(
      (!b2.Next())||(!(b2>>tmp_bin))||(!b2.Next())||(!(b2>>IsInit))||(!(b2>>init_blk))
      ||(!b2.Next())||(!(b2>>init_data))||(!b2.Next())||(!b2.Next())||(!b2.Next())
        )
        goto fail;
    if((cl=AccessClusters.find((char *)init_data.c_str()))!=AccessClusters.end())
      fld_id=(*cl).second;
    else
    {
      AccessClusters.insert(pair<char *,unsigned>(strdup(init_data.c_str()),AccessClusters.size()+1));
      fld_id=AccessClusters.size();
    }
    obj_id=cfg->AddObj(type_id,tmp_bin.c_str(),(unsigned short)fld_id);    
    //printf("Добавлен объект %s\n",tmp_bin.c_str());
    if(IsInit!=0)
      IndividualObjInits.insert(pair<unsigned,DataBlock*>(obj_id,new DataBlock(init_blk.GetData(),BlkCopy)));
  }
//остальное в типе нас не интересует....
  return true;
fail:
  if(Map)
    delete Map;
  return false;
}
//---------------------------------------------------------------------------
bool ConfCreator::LoadCluster(DataBlock &b)
{
  if(!cfg) return false;
  map<char *,unsigned,compare_key >::const_iterator c;
  BinData data;
  if((!b.Next())||(!(b>>data)))return false;
  if((c=AccessClusters.find((char *)data.c_str()))==AccessClusters.end())
    AccessClusters.insert(pair<char *,unsigned>(strdup(data.c_str()),AccessClusters.size()+1));
  return true;
}
//---------------------------------------------------------------------------
bool ConfCreator::LoadGroup(DataBlock &b)
{
  if(!cfg) return false;
  DataBlock b2;
  BinData data,pwd;
  unsigned group_id,i;
  unsigned char r,w,rights;
  unsigned long count;
  map<char *,unsigned,compare_key >::const_iterator c;
  if((!b.Next())||(!(b>>data))||(!b.Next()))
    return false;
  group_id=cfg->AddUsrGroup(data.c_str());
  if(!(b>>b2))
    return false;
  if(!(b2>>count))
    return false;
  for(i=0;i<count;i++)//права (через кластеры доступа)
  {
    if((!b2.Next())||(!(b2>>data))||(!(b2>>r))||(!(b2>>w))||(!b2.Next())||(!b2.Next())) return false;
    if((c=AccessClusters.find((char *)data.c_str()))!=AccessClusters.end())
    {
      rights=0;
      rights |= r; rights |=(w<<1);
      cfg->SetRights(group_id,(*c).second,rights);
    }
  }
  if(!(b>>count)) return false;//число юзверей
  for(i=0;i<count;i++)
  {
    if(!(b>>b2))return false;
    if((!b2.Next())||(!(b2>>data))||(!b2.Next())||(!(b2>>pwd)))return false;
    cfg->AddUser(group_id,data.c_str(),pwd.c_str());
  }
  return true;
}
//---------------------------------------------------------------------------
void ConfCreator::ClearInits()
{
  map<char *,unsigned,compare_key >::iterator c;
  map<unsigned,DataBlock *,less<unsigned> >::iterator x;
  map<unsigned,BinData *,less<unsigned> >::iterator XPEH;
  map<unsigned,map<unsigned,BinData *,less<unsigned> >*,less<unsigned> >::iterator i;
  for(i=Inits.begin();i!=Inits.end();i++)
  {
    for(XPEH=(*i).second->begin();XPEH!=(*i).second->end();XPEH++)
      delete (*XPEH).second;//удаляем BinData
    (*i).second->clear();
    delete (*i).second;//а теперь и хрен...
  }
  for(x=IndividualObjInits.begin();x!=IndividualObjInits.end();x++)
    delete (*x).second;
  for(c=AccessClusters.begin();c!=AccessClusters.end();c++)
    free((*c).first);
  Inits.clear();
  AccessClusters.clear();
  IndividualObjInits.clear();
}
//---------------------------------------------------------------------------
//без оптимизации алгоритма...
bool ConfCreator::InitValues()
{
  obj_d *ob;
  int fld_index = 0;
  const type_d * tp;
  fld_d *fld;
  BinData d1,name;
  unsigned short base_type;
  //DataBlock b;
  unsigned long count;
  map<unsigned,fld_d *,less<unsigned> >::const_iterator fld_i;
  map<unsigned,obj_d *,less<unsigned> >::const_iterator obj;
  map<unsigned,map<unsigned,BinData *,less<unsigned> >*,less<unsigned> >::const_iterator i;
  map<unsigned,BinData *,less<unsigned> >::const_iterator vl;
  map<unsigned,DataBlock *,less<unsigned> >::const_iterator x;
  for(i=Inits.begin();i!=Inits.end();i++)
  {
    if((tp=cfg->Type((*i).first))!=0)
    {
      //dump.Print("Тип %s:*********\n",tp->Name());
      for(obj=tp->Objects.begin();obj!=tp->Objects.end();obj++)
      {
        //dump.Print("   Объект %s:\n",(*obj).second->Name());
       for(vl=(*i).second->begin();vl!=(*i).second->end();vl++)
        if((fld=(fld_d*)(*obj).second->Master()->Fld((*vl).first))!=0)
        {
          //dump.Print("     Поле %s = %s\n",fld->Name(),(*vl).second->c_str());
          switch(fld->Type())
          {
            case tm_byte:
              (*(*obj).second)[(*vl).first] = (unsigned char )atoi((*vl).second->c_str());
             break;
            case tm_char:
              (*(*obj).second)[(*vl).first] = (char )atoi((*vl).second->c_str());
             break;
            case tm_uint:
              (*(*obj).second)[(*vl).first] = (unsigned short )atoi((*vl).second->c_str());
             break;
            case tm_int:
              (*(*obj).second)[(*vl).first] = (short )atoi((*vl).second->c_str());
             break;
            case tm_ulong:
              (*(*obj).second)[(*vl).first] = (unsigned long )atol((*vl).second->c_str());
             break;
            case tm_long:
              (*(*obj).second)[(*vl).first] = (long )atol((*vl).second->c_str());
             break;
            case tm_float:
              (*(*obj).second)[(*vl).first] = (float )atof((*vl).second->c_str());
             break;
            case tm_double : 
              (*(*obj).second)[(*vl).first] = (double )atof((*vl).second->c_str());
             break;
            case tm_time:
              (*(*obj).second)[(*vl).first] = (double )atof((*vl).second->c_str());
             break;
            case tm_string:
              //(*(*obj).second)[(*vl).first] = (const char*)(*vl).second->c_str();
              (*obj).second->SetFldValue((*vl).first,(void*)(*vl).second->c_str(),tm_string);
             break;
            default:
             continue;
          }       
        }
      }
    }
  }
  for(x=IndividualObjInits.begin();x!=IndividualObjInits.end();x++)
  {
    if((ob=(obj_d*)cfg->Obj((*x).first))==0)
      continue;
    //dump.Print(" Объект %s:   ***\n",ob->Name());
    if(!((*(*x).second)>>count))
      return false;
     for(int v=0;v<3;v++)//  :)))
        if(!((*(*x).second).Next()))
          return false;
    for(unsigned long i=0;i<count-1;i++)
    { 
      if(!((*(*x).second)>>name))
        return false;
      //dump.Print("    Поле %s = ",name.c_str());
      if(!((*(*x).second)>>base_type))
        return false;
      if(base_type==tm_link)
        if(!((*(*x).second)>>d1))
          return false;
      if(!((*(*x).second)>>d1))
        return false;

      //dump.Print("%s",d1.c_str());
      if((fld_i=find_if(ob->Master()->Fields.begin(),ob->Master()->Fields.end(),bind2nd(NameKey(),name.c_str())))!=ob->Master()->Fields.end())
      {
         fld_index = (*fld_i).second->Id();
          switch((*fld_i).second->Type())
          {
            case tm_byte:
              (*ob)[fld_index] = (unsigned char )atoi(d1.c_str());
              //dump.Print("(%u)\n",(unsigned char )(*ob)[fld_index]);
             break;
            case tm_char:
              (*ob)[fld_index] = (char )atoi(d1.c_str());
              //dump.Print("(%d)\n",(char )(*ob)[fld_index]);
             break;
            case tm_uint:
              (*ob)[fld_index] = (unsigned short )atoi(d1.c_str());
              //dump.Print("(%u)\n",(unsigned short )(*ob)[fld_index]);
             break;
            case tm_int:
              (*ob)[fld_index] = (short )atoi(d1.c_str());
              //dump.Print("(%d)\n",(short )(*ob)[fld_index]);
             break;
            case tm_ulong:
              (*ob)[fld_index] = (unsigned long )atol(d1.c_str());
              //dump.Print("(%u)\n",(unsigned long )(*ob)[fld_index]);
             break;
            case tm_long:
              (*ob)[fld_index] = (long )atol(d1.c_str());
              //dump.Print("(%d)\n",(long )(*ob)[fld_index]);
             break;
            case tm_float:
              (*ob)[fld_index] = (float )atof(d1.c_str());
              //dump.Print("(%f)\n",(float )(*ob)[fld_index]);
             break;
            case tm_double : 
              (*ob)[fld_index] = (double )atof(d1.c_str());
              //dump.Print("(%f)\n",(double )(*ob)[fld_index]);
             break;
            case tm_time:
              (*ob)[fld_index] = (double )atof(d1.c_str());
              //dump.Print("(%f)\n",(double )(*ob)[fld_index]);
             break;
            case tm_string:
              //(*ob)[fld_index] = d1.c_str();//(const char*)
              ob->SetFldValue(fld_index,(void*)d1.c_str(),tm_string);
              //dump.Print("(%s)\n",(const char* )(*ob)[fld_index]);              
             break;
            default:
             continue;
          }          
      }
      /*if(!strcmp(ob->Name(),"FB1") && !strcmp(name.c_str(),"Enabled"))
      {
        fld_d *fd = (*fld_i).second;
        extern_data = fd->Values[ob->Id()];
      }*/
    }
  }
  return true;
} 
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
