/***********************************************************
      Вывод сообщений и трассировка объектов.
    !!!Версия только для гранитовского сервера ввода-вывода!!!
               created by M.E.N
***********************************************************/

#include <ctype.h>
#include <stdio.h>
#include <io_srv_dump.h>
#include <stm_srv_cfg.h>
#include <string>
#include <stdarg.h>
#ifndef WIN32
# include <sys/stat.h>
#endif
//#include <win2koi_koi2win.h>
extern "C"{
extern char * Win2Koi( char * str );
extern char * Koi2Win( char * str );
}

using namespace std;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
string DumpFileName;
string DumpCfgFileName;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
time_t IO_StartupTime; 
time_t IO_InitDevTime; 
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#define MAX_DUMP_FILE_SIZE  0xfffff
#define UPDATE_TIME         10
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
typedef map<unsigned,unsigned char,less<unsigned> > map_type ;
//----------------------------------------------------------------
struct FlagNames
{
  char * str;
  int flag;
};
//----------------------------------------------------------------
const FlagNames flags_names[13]=
{
    {"ERROR" ,     D_ERR},
    {"WARNING" ,   D_WARN},
    {"MESSAGE" ,   D_MSG},
    {"PORT" ,      D_PORT},
    {"CHANNEL" ,   D_CNL},
    {"DEV" ,        D_DEV},
    {"AFB" ,       D_AFB},
    {"SENSOR" ,    D_SENSOR},
    {"SRV" ,       D_SRV},
    {"CLN" ,       D_CLN},
    {"STM_PROTO" , D_STM_PROTO},
    {"TRACE" ,     D_TRACE},
    {"TIME" ,      D_TIME}
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Dump dump;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
int Dump::obj_count = 0;
//----------------------------------------------------------------
Dump::Dump()
:ready(false),flags(0),dump_file(NULL),line(0), EventProcessor(0),cfg(0)
{
  cfg_file_name = 0;
  dmp_file_name = 0;
  if(obj_count)
    return;
  else
    obj_count++;
  //flags = 0xffffffff;
  CRIT_SEC_CREATE(CrtSec)
}
//----------------------------------------------------------------
Dump::~Dump()
{ 
  Shutdown();
  CRIT_SEC_FREE(CrtSec)
}
//----------------------------------------------------------------
void Dump::Shutdown()
{
  if(cfg_file_name)
    free(cfg_file_name);
  if(dmp_file_name)
    free(dmp_file_name);
  traces.clear();
  if(dump_file!=NULL)
  {
   #ifndef WIN32    
    fprintf(dump_file,"*********\tпЛПОЮБОЙЕ ТБВПФЩ\t**********\n");
   #else
    fprintf(dump_file,"*********\tОкончание работы\t**********\n");
   #endif
    fclose(dump_file);
    dump_file = NULL;
  }
  ready=false;
  line = 0;
}
//----------------------------------------------------------------
bool Dump::ResetDump(const char *dmp_name)
{
  CRIT_SEC_BEGIN(CrtSec)
  if(dump_file)
    fclose(dump_file);
  dump_file=fopen(dmp_name,"a");//6/2/2 "w"
  if(!dump_file)
  {
    puts("dump file create error!\n");    
    CRIT_SEC_END(CrtSec)
    return false;
  }
  dmp_file_name = strdup(dmp_name);    
  #ifdef WIN32
  fprintf(dump_file,"*********\tНачало работы\t**********\n");
  #else
  fprintf(dump_file,"*********\tоБЮБМП ТБВПФЩ\t**********\n");
  #endif
  return true;
}
//----------------------------------------------------------------
bool Dump::ResetTraces(StmCfg *conf,const char *cfg_name,const char *dmp_name)
{
  cfg = conf;
  FILE *cfg_fl;
  CRIT_SEC_BEGIN(CrtSec)
  if(!dump_file)
    if(!ResetDump(dmp_name))
    {
      CRIT_SEC_END(CrtSec)
      return (ready=false);  
    }
  cfg_fl=fopen(cfg_name,"rb");
  if(!cfg_fl)
  {
    puts("dump cfg file open error!\n");
    CRIT_SEC_END(CrtSec)
    return (ready=false);  
  }
  else
    fclose(cfg_fl);
  cfg_file_time = time(0);
  cfg_file_name = strdup(cfg_name);
  CRIT_SEC_END(CrtSec)
  flags = 0;
  UpdateCfg();
  return (ready=true);
}
//----------------------------------------------------------------
void Dump::Print(const char *fmt,...)
{   
  CRIT_SEC_BEGIN(CrtSec) 
  if(!dump_file)
    return;
  struct stat dump_file_status;  
  if(stat(dmp_file_name,&dump_file_status)==0)
    if(dump_file_status.st_size >= MAX_DUMP_FILE_SIZE)
    {//обрезка файла
      fclose(dump_file);
      dump_file=fopen(dmp_file_name,"w");
      if(!dump_file)
      {
        ready=false;
        CRIT_SEC_END(CrtSec)
        return;
      }
    }
    fprintf(dump_file,"%d ",++line);
    if(flags & D_TIME)
    {
      time_t curr_time = time(0);
      struct tm *curr_time_tm = localtime(&curr_time);
      fprintf(dump_file,"[%d.%d.%d %d:%d:%d] ",
        curr_time_tm->tm_mday,
        curr_time_tm->tm_mon+1,
        curr_time_tm->tm_year-100+2000,
        curr_time_tm->tm_hour,
        curr_time_tm->tm_min,
        curr_time_tm->tm_sec);
    }    
    //sprintf(buff,"%s",fmt);
    va_list args;
    va_start(args,fmt);
    vsprintf(buff,fmt,args);
    va_end(args);    
    #ifndef WIN32
    Win2Koi(buff);
    #endif
    fprintf(dump_file,"%s\n",buff); 
    fflush(dump_file);
  CRIT_SEC_END(CrtSec)
}
//----------------------------------------------------------------
void Dump::ObjMsg(unsigned obj_id,int flag,const char *fmt,...)
{  
  if(!ready)
    return;
  if(obj_id)
  {
    map_type::iterator i;
    if(flags & D_TRACE)
    {
      if((i=traces.find(obj_id))==traces.end())
        return;
    }
    else
      return;
  }
  if(!ready)
    return;  
  if(!dump_file)
    return;
  CRIT_SEC_BEGIN(CrtSec)    
  if((time(0) - cfg_file_time) > UPDATE_TIME)
    UpdateCfg();
  struct stat dump_file_status;  
  if(stat(dmp_file_name,&dump_file_status)==0)
    if(dump_file_status.st_size >= MAX_DUMP_FILE_SIZE)
    {//обрезка файла
      fclose(dump_file);
      dump_file=fopen(dmp_file_name,"w");
      if(!dump_file)
      {
        ready=false;
        CRIT_SEC_END(CrtSec)
        return;
      }
    }
  if((flags & flag) == flag)
  {    
    fprintf(dump_file,"%d ",++line);
    if(flags & D_TIME)
    {
      time_t curr_time = time(0);
      struct tm *curr_time_tm = localtime(&curr_time);
      fprintf(dump_file,"[%d.%d.%d %d:%d:%d] ",
        curr_time_tm->tm_mday,
        curr_time_tm->tm_mon+1,
        curr_time_tm->tm_year-100+2000,
        curr_time_tm->tm_hour,
        curr_time_tm->tm_min,
        curr_time_tm->tm_sec);
    }    
    //sprintf(buff,"%s",fmt);
    va_list args;
    va_start(args,fmt);
    vsprintf(buff,fmt,args);
    va_end(args);    
    #ifndef WIN32
    Win2Koi(buff);
    #endif
    fprintf(dump_file,"%s\n",buff); 
    fflush(dump_file);
  }    
  CRIT_SEC_END(CrtSec)
}
//----------------------------------------------------------------
void Dump::Print(int flag,const char *fmt,...)
{
  if(!ready)
    return;  
  if(!dump_file)
    return;
  CRIT_SEC_BEGIN(CrtSec)    
  if((time(0) - cfg_file_time) > UPDATE_TIME)
    UpdateCfg();
  struct stat dump_file_status;  
  if(stat(dmp_file_name,&dump_file_status)==0)
    if(dump_file_status.st_size >= MAX_DUMP_FILE_SIZE)
    {//обрезка файла
      fclose(dump_file);
      dump_file=fopen(dmp_file_name,"w");
      if(!dump_file)
      {
        ready=false;
        CRIT_SEC_END(CrtSec)
        return;
      }
    }
  if((flags & flag) == flag)
  {    
    fprintf(dump_file,"%d ",++line);
    if(flags & D_TIME)
    {
      time_t curr_time = time(0);
      struct tm *curr_time_tm = localtime(&curr_time);
      fprintf(dump_file,"[%d.%d.%d %d:%d:%d] ",
        curr_time_tm->tm_mday,
        curr_time_tm->tm_mon+1,
        curr_time_tm->tm_year-100+2000,
        curr_time_tm->tm_hour,
        curr_time_tm->tm_min,
        curr_time_tm->tm_sec);
    }    
    //sprintf(buff,"%s",fmt);
    va_list args;
    va_start(args,fmt);
    vsprintf(buff,fmt,args);
    va_end(args);    
    #ifndef WIN32
    Win2Koi(buff);
    #endif
    fprintf(dump_file,"%s\n",buff); 
    fflush(dump_file);
  }    
  CRIT_SEC_END(CrtSec)
}
//----------------------------------------------------------------
void Dump::ProcessEvent(const IoSrvEvent &event)
{
  if(!ready)
    return;
  if((time(0) - cfg_file_time) > UPDATE_TIME)
    UpdateCfg();
  map_type::const_iterator c;
  map_type::iterator i;
  switch(event.ev_id)
  {
    case IO_SRV_EV_OBJS_CHANGED:	//изменение группы объеков
      /*if(flags & D_TRACE)
      {
        map_type *mp;
        mp = (map_type*)event.data;
        for(c=mp->begin();c!=mp->end();c++)
          if((i=traces.find((*c).first))!=traces.end())
            DumpObject((*c).first);
      }*/
      break;
    case IO_SRV_EV_OBJ_CHANGED:		//изменение объекта
      if(flags & D_TRACE)
       if((i=traces.find(*(unsigned*)event.data))!=traces.end())
         DumpObject(*(unsigned*)event.data);
      break;
    //case IO_SRV_EV_SHUTDOWN : case IO_SRV_EV_HALT:
      //ready = false;
    default:
      return;
  }  
}
//----------------------------------------------------------------
void Dump::UpdateCfg()
{  
  
  int readed;
  char *parse_buff=0;
  struct stat file_status;
  if(stat(cfg_file_name,&file_status))
    return;  
  if(file_status.st_mtime == cfg_file_mtime)
    return;
  cfg_file_mtime = file_status.st_mtime;
  cfg_file_time = time(0);
  FILE *f = fopen(cfg_file_name,"rb");
  if(!f)
    goto fail;
  parse_buff = (char*)malloc(file_status.st_size);
  if(!parse_buff)
    goto fail;
  readed = fread(parse_buff,file_status.st_size,1,f);
  fclose(f);
  if(!readed )
    goto fail;
  flags = 0;
  traces.clear(); //не будем мудрить :)
  smb = parse_buff;
  state = NEXT;
  token_type = NONE;  
  //puts("XPEH");
  while(state != END)
  {    
    GetToken(parse_buff,file_status.st_size);
    if(state == ERR)
      goto fail;
    if(state == END)
      break;
    switch(token_type)
    {      
      case KEY:
        if(state != NEXT)
          goto fail;
        curr_key = 0;
        if(!strcmp("TRACE_OBJECT",buff))
        {
          state = GET_STR_KEY;
          break;
        }
        else
        {
          for(int i=0;i<13;i++)
            if(!strcmp(flags_names[i].str,buff))
            {
              curr_key |= flags_names[i].flag;
              break;
            }
        }
        if(curr_key)
        {
          state = GET_BIN_KEY;
          break;
        } 
        else
          goto fail;
        break;
      case OPER:
        if(state !=  GET_BIN_KEY)
          goto fail;
        break;
      case L_BR:
        if(state != GET_STR_KEY)
          goto fail;
        break;
      case R_BR:
        if(state != GET_STR_KEY)
          goto fail;
        else
        {
#ifndef WIN32
          Koi2Win(buff);
#endif
          unsigned id = GetObjId(buff);
          if(id)
          {
            map<unsigned,unsigned char,less<unsigned> >::iterator i;
            if((i=traces.find(id))==traces.end())
              traces.insert(pair<unsigned,unsigned char>(id,0));
          }
          state = NEXT;
        }
        break;
      case BIN_VAL:
        if(state !=  GET_BIN_KEY)
          goto fail;
        else
        {
          long val = atol(buff);
          if(val != 0)
            flags |= curr_key;
          state = NEXT;
        }
        break;
      case STR_VAL:
        if(state != GET_STR_KEY)
          goto fail;
        break;
    }
  }
  free(parse_buff);
  return;
fail:
  free(parse_buff);
  puts("dump_cfg file error!\n");
}
//----------------------------------------------------------------
void Dump::GetToken(const char *parse_buff,int max)
{  
#ifdef WIN32
  static const char *SYMBOLS=
    "0123456789_ВЧЮЪДЕГЯКЛМНОПРТФХЖИЦЙЗАЩЬЫЭСШБУвчюъдегяклмнопртфхжицйзащьыэсшбуqwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM";   
#else
  static const char *SYMBOLS=
    "0123456789_чюаядезслмнопртфжицйгкъбэшщьуывхЧЮАЯДЕЗСЛМНОПРТФЖИЦЙГКЪБЭШЩЬУЫВХqwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM";
#endif
  for(;smb-parse_buff < max;)
  { 
    //..........пропуск пробелов............
    if(isspace(*smb))
    {
      ++smb;
      continue;
    }
    //.........пропуск комментария '#'
    if(*smb == '#')
      for(;(smb-parse_buff < max) && *smb != '\n';smb++){}
    //.........пропуск комментария { }..... (как в паскале)
    else if(*smb == '{')
      for(;(smb-parse_buff < max) && *smb != '}';smb++){}
    //..........оператор = .................
    else if(*smb == '=')
    {
      ++smb;
      token_type = OPER;
      return;
    }
    //..........левая скобка.................
    else if(*smb == '<')
    {
      ++smb;
      token_type = L_BR;
      return;
    }
    //..........правая скобка................
    else if(*smb == '>')
    {
      ++smb;
      token_type = R_BR;
      return;
    }
    //..........значение.....................
    else if(isdigit(*smb))
    {
      int len,bf_len=sizeof(buff);
      for(len=0;max > smb-parse_buff && isdigit(*smb) && len<bf_len;smb++,len++)
        buff[len] = *smb;
      buff[len] = '\0';
      token_type = BIN_VAL;
      return;
    }
    //..........ключ или строковое значение..
    else if(isalpha(*smb))
    {
      int p_cnt,len,bf_len=sizeof(buff);
      for(len=0,p_cnt=0;max > smb-parse_buff && (strchr(SYMBOLS, *smb) || *smb=='.') && len<bf_len;smb++,len++)
      {
        buff[len] = *smb;
        if(*smb=='.')p_cnt++;
      }
      buff[len] = '\0';
      if(p_cnt==1)
      {        
        token_type = STR_VAL;
        return;
      }
      else if(p_cnt == 0)
      {
        token_type = KEY;
        return;
      }
      else
      {
        state = ERR;
        return;
      }
    }
    else
    {
      state = ERR;
      return;
    }
  }
  state = END;
}
//----------------------------------------------------------------
unsigned Dump::GetObjId(const char *ptr)
{
  int len;
  char type_name[256];
  char obj_name[256];
  for(len=0;*ptr && *ptr!='.' && len<256;type_name[len] = *ptr++,len++){}
  ++ptr;
  type_name[len] = '\0';
  for(len=0;*ptr && len<256;obj_name[len] = *ptr++,len++){}
  obj_name[len] = '\0';
  return cfg->ObjId(cfg->TypeId(type_name),obj_name);
}
//----------------------------------------------------------------
void Dump::DumpObject(unsigned obj_id)
{
  CRIT_SEC_BEGIN(CrtSec)  
  unsigned char tp;
  const obj_d *obj = cfg->Obj(obj_id);
  if(!obj)
    return;
  int fld_count = obj->Master()->FldCount();
  char *name;
#ifndef WIN32
  name = strdup(obj->Name());
  Win2Koi(name);
#else
  name = (char*)obj->Name();
#endif
  fprintf(dump_file,"%d Dump: %s:\n",++line,name);
  for(int i=1;i<=fld_count;i++)
  {
    const void *val = obj->GetFldValue(i,tp);
    fprintf(dump_file,"  %s : ",obj->Master()->Fld(i)->Name());
    switch(obj->Master()->Fld(i)->Type())
    {
      case tm_byte:
        fprintf(dump_file,"%u\n",*(unsigned char*)val);
        break;
      case tm_char:
        fprintf(dump_file,"%d\n",*(char*)val);
        break;
      case tm_uint:
        fprintf(dump_file,"%u\n",*(unsigned short*)val);
        break;
      case tm_int:
        fprintf(dump_file,"%d\n",*(short*)val);
        break;
      case tm_ulong:
        fprintf(dump_file,"%u\n",*(unsigned long*)val);
        break;
      case tm_long:
        fprintf(dump_file,"%d\n",*(long*)val);
        break;
      case tm_float:
        fprintf(dump_file,"%f\n",*(float*)val);
        break;
      case tm_double:
        fprintf(dump_file,"%f\n",*(double*)val);
        break;
      case tm_time:
        {
          double value = *(double*)val,sec; 
          struct timeval tv;
          sec =value / 1e6;
          tv.tv_sec =  (long)sec;
          tv.tv_usec = (long)(value - sec);
          tm *tt = localtime(&tv.tv_sec);
          sprintf(buff,"%02u/%02u/%02u %02u:%02u:%02u.%06u",
           tt->tm_mday, tt->tm_mon+1, tt->tm_year % 100,
           tt->tm_hour, tt->tm_min, tt->tm_sec, tv.tv_usec);
          fprintf(dump_file,"%s\n",buff);
        }
        break;
      case tm_string:
        {
         #ifndef WIN32
          char *str = strdup((const char*)val);
          fprintf(dump_file,"%s\n",Win2Koi(str));
          free(str);
         #else
          fprintf(dump_file,"%s\n",(const char*)val);
         #endif
        }
        break;
    }
    fflush(dump_file);
  }
  CRIT_SEC_END(CrtSec)
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%