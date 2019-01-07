/*****************************************************************************
                     created by M.E.N.
*****************************************************************************/
#include <stm_srv_cfg.h>
#include <io_srv_dump.h>
//--------------------------------------------------------------------------------
Dscr::Dscr(const char *Name,unsigned Id):id(Id)
{
  name=strdup(Name);
}
//--------------------------------------------------------------------------------
Dscr::~Dscr()
{
  if(name)
    free(name);
}
//%%%%%%%%%%%%%%%%%%%%%% fld_d %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
fld_d::fld_d(const char *Name,unsigned Id,unsigned char Type)
  :Dscr(Name,Id), type(Type)
{
 
}
//--------------------------------------------------------------------------------
fld_d::~fld_d()
{
  map<unsigned,void *,less<unsigned> >::iterator i;
  for(i=Values.begin();i!=Values.end();i++)
    free ((*i).second);
  Values.clear();
}
//--------------------------------------------------------------------------------
int fld_d::Len()const
{
  return GetLenForType((unsigned)type);
}
//%%%%%%%%%%%%%%%%%%%%%% obj_d %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
obj_d::obj_d(const char *Name,unsigned Id,unsigned short a_cluster,type_d *mr)
  :Dscr(Name,Id),master(mr),AccessCluster(a_cluster)
{
  int mask_len=(master->FldCount()+31)/32;
  mask_of_fld_changes=(unsigned *)calloc(mask_len,sizeof(unsigned));
}
//--------------------------------------------------------------------------------
obj_d::~obj_d()
{
  free(mask_of_fld_changes);
}
//--------------------------------------------------------------------------------
void obj_d::SetFldChangeBit(unsigned fld)const
{
  if(master->FldCount() >= fld)
  {
    int index=fld/32;
    mask_of_fld_changes[index] |= (1<<(fld-32*index-1)); 
  }
}
//--------------------------------------------------------------------------------
void obj_d::ClearFldChangeBit(unsigned fld)const
{
  if(master->FldCount() >= fld)
  {
    int index=fld/32;
    mask_of_fld_changes[index] ^= (1<<(fld-32*index-1)); 
  }
}
//--------------------------------------------------------------------------------
void obj_d::ClearFldChangesMask()const
{
  int mask_len=(master->FldCount()+31)/32;
  for(int i=0;i<mask_len;i++)
    mask_of_fld_changes[i]=0;
}
//--------------------------------------------------------------------------------
void obj_d::SetFldValue(unsigned fld,void *val,unsigned char fld_type)
{
  master->SetFldValue(id,fld,val,fld_type);  
}
//--------------------------------------------------------------------------------
const void *obj_d::GetFldValue(unsigned fld,unsigned char &fld_type)const
{
  return master->GetFldValue(id,fld,fld_type);
}
//--------------------------------------------------------------------------------
obj_d::FldValPxy obj_d::operator[](unsigned fld)
{
  map<unsigned,void *,less<unsigned> >::iterator d;
  map<unsigned, fld_d *, less<unsigned> >::const_iterator f_d;
  if((f_d=master->Fields.find(fld))!=master->Fields.end())
    if((d=(*f_d).second->Values.find(id))!=(*f_d).second->Values.end())
    {
      //dump.Print(D_MSG,"%s.%s.%s = %u",master->Name(),Name(),(*f_d).second->Name(),*(unsigned char*)(*d).second);
      return obj_d::FldValPxy((*d).second,(*f_d).second->Type());
    }
  return obj_d::FldValPxy(0,0);
}
//--------------------------------------------------------------------------------
obj_d::FldValPxy::operator char()const
{
  if(type==tm_char)
    return *(char*)data;
  return 0;
}
//--------------------------------------------------------------------------------
obj_d::FldValPxy::operator unsigned char()const
{
  if(type==tm_byte)
   return *(unsigned char *)data;
  return 0;
}
//--------------------------------------------------------------------------------
obj_d::FldValPxy::operator short()const
{
  if(type==tm_int)
    return *(short *)data;
  return 0;
}
//--------------------------------------------------------------------------------
obj_d::FldValPxy::operator unsigned short()const
{
  if(type==tm_uint)
    return *(unsigned short *)data;
  return 0;
}
//--------------------------------------------------------------------------------
obj_d::FldValPxy::operator int()const
{
  if(type==tm_long)
    return *(long *)data;
  return 0;
}
//--------------------------------------------------------------------------------
obj_d::FldValPxy::operator unsigned()const
{
  if(type==tm_ulong)
   return *(unsigned long *)data;
  return 0;
}
//--------------------------------------------------------------------------------
obj_d::FldValPxy::operator long()const
{
  if(type==tm_long)
    return *(long *)data;
  return 0;
}
//--------------------------------------------------------------------------------
obj_d::FldValPxy::operator unsigned long()const
{
  if(type==tm_ulong)
    return *(unsigned long *)data;
  return 0;
}
//--------------------------------------------------------------------------------
obj_d::FldValPxy::operator float()const
{
  if(type==tm_float)
    return *(float *)data;
  return 0;
}
//--------------------------------------------------------------------------------
obj_d::FldValPxy::operator double()const
{
  if(type==tm_double || type==tm_time)
    return *(double *)data;
  return 0;
}
//--------------------------------------------------------------------------------
obj_d::FldValPxy::operator const char*()const
{
  if(type==tm_string)
    return (const char*)data;
  return "";
}
//--------------------------------------------------------------------------------
obj_d::FldValPxy::operator timeval()const
{
  static struct timeval tv;
  if(type==tm_time)
  {
    double sec = *(double*)data / 1e6;
    static struct timeval tv;
    tv.tv_sec =  (long)sec;
    tv.tv_usec = (long)(*(double*)data - sec);    
  }
  else
  {
    tv.tv_sec =0;
    tv.tv_usec =0;
  }
  return tv;
}
//--------------------------------------------------------------------------------
void obj_d::FldValPxy::operator=(char val)
{
  if(type==tm_char)
    *(char *)data=val;
}
//--------------------------------------------------------------------------------
void obj_d::FldValPxy::operator=(unsigned char val)
{
  if(type==tm_byte)
   *(unsigned char *)data=val;
}
//--------------------------------------------------------------------------------
void obj_d::FldValPxy::operator=(short val)
{
  if(type==tm_int)
     *(short *)data=val;     
}
//--------------------------------------------------------------------------------
void obj_d::FldValPxy::operator=(unsigned short val)
{
   if(type==tm_uint)
     *(unsigned short *)data=val;     
}
//--------------------------------------------------------------------------------
void obj_d::FldValPxy::operator=(int val)
{
   if(type==tm_long)
     *(long *)data=val;     
}
//--------------------------------------------------------------------------------
void obj_d::FldValPxy::operator=(unsigned val)
{
   if(type==tm_ulong)
     *(unsigned long *)data=val;
}
//--------------------------------------------------------------------------------
void obj_d::FldValPxy::operator=(long val)
{
  if(type==tm_long)
     *(long *)data=val;     
}
//--------------------------------------------------------------------------------
void obj_d::FldValPxy::operator=(unsigned long val)
{
  if(type==tm_ulong)
     *(unsigned long *)data=val;     
}
//--------------------------------------------------------------------------------
void obj_d::FldValPxy::operator=(float val)
{
   if(type==tm_float)
     *(float *)data=val;     
}
//--------------------------------------------------------------------------------
void obj_d::FldValPxy::operator=(double val)
{
  if(type==tm_double || type==tm_time)
   *(double *)data=val;
}
//--------------------------------------------------------------------------------
void obj_d::FldValPxy::operator=(const char* val)
{
   if(type==tm_string)
   {
     free(data);
     data=strdup(val);
   }
}
//--------------------------------------------------------------------------------
void obj_d::FldValPxy::operator=(timeval tv)
{
  if(type==tm_time)
    *(double*)data = (double)(tv.tv_sec * 1e6 + tv.tv_usec);
}
//%%%%%%%%%%%%%%%%%%%%%%% type_d %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
type_d::type_d(const char *Name,unsigned Id,StmCfg *msr)
  :Dscr(Name,Id),master(msr)
{
}
//--------------------------------------------------------------------------------
type_d::~type_d()
{
  map<unsigned, obj_d *, less<unsigned> >::iterator o_i;
  map<unsigned, fld_d *, less<unsigned> >::iterator f_i;
  for(o_i=Objects.begin();o_i!=Objects.end();o_i++)
    if((*o_i).second!=0)
      delete ((*o_i).second);
  Objects.clear();
  for(f_i=Fields.begin();f_i!=Fields.end();f_i++)
    if((*f_i).second!=0)
      delete ((*f_i).second);
  Fields.erase(Fields.begin(),Fields.end());
}
//--------------------------------------------------------------------------------
const fld_d *type_d::Fld(unsigned i)const
{
  map<unsigned, fld_d *, less<unsigned> >::const_iterator f;
  if((f=Fields.find(i))!=Fields.end())
    return (*f).second;
  return 0;
}
//--------------------------------------------------------------------------------
const obj_d *type_d::Obj(unsigned obj_id)const
{
  map<unsigned, obj_d *, less<unsigned> >::const_iterator i;
  if((i=Objects.find(obj_id))!=Objects.end())
    return (*i).second;
  return 0;
}
//--------------------------------------------------------------------------------
const void *type_d::GetFldValue(unsigned obj,unsigned fld,unsigned char &fld_type)const
{
  map<unsigned,void *,less<unsigned> >::const_iterator d;
  map<unsigned, fld_d *, less<unsigned> >::const_iterator f_d;
  if((f_d=Fields.find(fld))!=Fields.end())
    if((d=(*f_d).second->Values.find(obj))!=(*f_d).second->Values.end())
    {
      fld_type = (*f_d).second->Type();
      return (*d).second;
    }
  return 0;
}
//--------------------------------------------------------------------------------
void type_d::SetFldValue(unsigned obj,unsigned fld,void *val,unsigned char fld_type)
{  
  void *data = NULL;
  map<unsigned,void *,less<unsigned> >::iterator d;
  map<unsigned, fld_d *, less<unsigned> >::iterator f_d;
  if((f_d=Fields.find(fld))!=Fields.end())
  {
      if(fld_type != (*f_d).second->Type())
        return;
      if((d=(*f_d).second->Values.find(obj))!=(*f_d).second->Values.end())
      {
        data = (*d).second;
        switch((*f_d).second->Type())
        {
           case tm_char:
             *(char *)data=*(char *)val;
             break;
           case tm_byte:
             *(unsigned char *)data=*(unsigned char *)val;
             break;
           case tm_int:
             *(unsigned short *)data=*(unsigned short *)val;
             break;
           case tm_uint:
             *(short *)data=*(short *)val;
             break;
           case tm_long:
             *(long *)data=*(long *)val;
             break;
           case tm_ulong:
             *(unsigned long *)data=*(unsigned long *)val;
             break;
           case tm_float:
             *(float *)data=*(float *)val;
             break;
           case tm_time: case tm_double:
             *(double *)data=*(double *)val;
             break;
           case tm_string:
             free((*d).second);
             (*d).second = strdup((char*)val);
             break;
           default:
             break;
        }
      }
  }
}
//--------------------------------------------------------------------------------
unsigned type_d::AddFld(const char *name,unsigned Type)
{
  if(!name||(Type<tm_byte||Type>tm_hi))
    return 0;
  map<unsigned, obj_d *, less<unsigned> >::const_iterator i;  
  fld_d *f=new fld_d(name,Fields.size()+1,(unsigned char)Type);    
  void *data=0;
  switch((unsigned char)Type)
  {
    case tm_byte:
      data=calloc(1,sizeof(unsigned char));
     break;
    case tm_char:
      data=calloc(1,sizeof(char));
     break;
    case tm_uint:
      data=calloc(1,sizeof(unsigned short));
     break;
    case tm_int:
      data=calloc(1,sizeof(short));
     break;
    case tm_ulong:
      data=calloc(1,sizeof(unsigned long));
     break;
    case tm_long:
      data=calloc(1,sizeof(long));
     break;
    case tm_float:
      data=calloc(1,sizeof(float));
     break;
    case tm_double :
      data=calloc(1,sizeof(double));
     break;
    case tm_time:
     data=calloc(1,sizeof(double));
     break;
    case tm_string:
     data=malloc(2);
     strcpy((char*)data,"");
     break;
    default:
     return 0;
  }
  for(i=Objects.begin();i!=Objects.end();i++)
  //добавили в вектор нового поля место под значения полей всех объектов этого типа
  {
    f->Values.insert(pair<unsigned,void  *>((*i).first,data));
  }
  Fields.insert(pair<unsigned,fld_d *>(Fields.size()+1,f));
  return Fields.size();
}
//--------------------------------------------------------------------------------
unsigned type_d::AddObj(unsigned Id,const char *Name,unsigned short a_cl)
{
  if(!Name||!Fields.size())
    return 0;
  map<unsigned, fld_d *, less<unsigned> >::iterator f;
  obj_d *obj=new obj_d(Name,Id,a_cl,this);
  Objects.insert(pair<unsigned,obj_d *>(Id,obj));
  void *data=0;
  for(f=Fields.begin();f!=Fields.end();f++)
  {
    switch((*f).second->Type())
    {
      case tm_byte:
        data=calloc(1,sizeof(unsigned char));
       break;
      case tm_char:
        data=calloc(1,sizeof(char));
       break;
      case tm_uint:
        data=calloc(1,sizeof(unsigned short));
       break;
      case tm_int:
        data=calloc(1,sizeof(short));
       break;
      case tm_ulong:
        data=calloc(1,sizeof(unsigned long));
       break;
      case tm_long:
        data=calloc(1,sizeof(long));
       break;
      case tm_float:
        data=calloc(1,sizeof(float));
       break;
      case tm_double :
        data=calloc(1,sizeof(double));
       break;
      case tm_time:
       data=calloc(1,sizeof(double));
       break;
      case tm_string:
       data=malloc(2);
       strcpy((char*)data,"");
       break;
      default:
       return 0;
    }//в каждое поле добавим место под его значения для нового объекта
    (*f).second->Values.insert(pair<unsigned,void *>(Id,data));
  }
  return Id;
}
//%%%%%%%%%%%%%%%%%%%%%%%% User %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
User::User(unsigned i_d,const char *name,const char *pwd):Dscr(name,i_d)
{
  if(pwd)
  {
    passwd=strdup(pwd);
  }
  else
    passwd=0;
}
//-------------------------------------------------------------------------------
User::~User()
{
  if(passwd)
    free(passwd);
}
//%%%%%%%%%%%%%%%%%%%%%%%% Group %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
UsrGroup::~UsrGroup()
{
  map<unsigned ,User *,less<unsigned> >::iterator u;
  for(u=Users.begin();u!=Users.end();u++)
    if((*u).second)
      delete (*u).second;
  Users.clear();
  AccessClusters.clear();
}
//-------------------------------------------------------------------------------
int UsrGroup::Rights(unsigned cl)
{
  map<unsigned, char,less<unsigned> >::iterator c;
  if((c=AccessClusters.find(cl))!=AccessClusters.end())
    return (*c).second;
  return NOT_ACCESSABLE;
}
//%%%%%%%%%%%%%%%%%%%%%%%%  StmCfg  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
StmCfg::StmCfg(const char *name,unsigned id,unsigned ver)
  :Dscr(name,id),ObjCount(0),TypeCount(0),UserCount(0),version(ver)
{
}
//--------------------------------------------------------------------------------
StmCfg::~StmCfg()
{
  map<unsigned, UsrGroup *,less<unsigned> >::iterator g;
  map<unsigned, type_d *, less<unsigned> >::iterator t;
  for(t=Types.begin();t!=Types.end();t++)
    if((*t).second)
      delete (*t).second;
  for(g=Groups.begin();g!=Groups.end();g++)
    if((*g).second)
      delete (*g).second;
  Types.clear();
  Groups.clear();
}
//--------------------------------------------------------------------------------
unsigned StmCfg::AddType(const char *name)
{
  if(!name)
    return 0;
  map<unsigned, UsrGroup *,less<unsigned> >::iterator g;
  type_d *type=new type_d(name,Types.size()+1,this);
  Types.insert(pair<unsigned,type_d *>(Types.size()+1,type));
  TypeCount++;
  return Types.size();//это и есть идентификатор типа
}
//--------------------------------------------------------------------------------
unsigned StmCfg::AddFld(unsigned type_id,unsigned fld_type,const char *name)
{
  unsigned fld;
  map<unsigned,type_d *,less<unsigned> >::iterator tp;
  map<unsigned,obj_d *,less<unsigned> >::iterator obj;
  if((tp=Types.find(type_id))!=Types.end())
  {
    if(name)
    {
      fld=(*tp).second->AddFld(name,fld_type);
      return fld;
    }
  }
  return 0;
}
//--------------------------------------------------------------------------------
unsigned StmCfg::AddObj(unsigned type,const char *name,unsigned short a_cl)
{
  map<unsigned,type_d *,less<unsigned> >::iterator tp;
  if((tp=Types.find(type))!=Types.end())
    if(name)
      return (*tp).second->AddObj(++ObjCount,name,a_cl);
  return 0;
}
//--------------------------------------------------------------------------------
unsigned StmCfg::AddUsrGroup(const char *name)
{
  pair<map<unsigned, UsrGroup *,less<unsigned> >::iterator,bool>p=Groups.insert(pair<unsigned,UsrGroup *>(Groups.size()+1,new UsrGroup(name,Groups.size()+1)));
  if(p.second)
    return (*p.first).first;
  return 0;
}
//--------------------------------------------------------------------------------
unsigned StmCfg::AddUser(unsigned gr,const char *name,const char *pwd)
{
  map<unsigned, UsrGroup *,less<unsigned> >::iterator g;
  if((g=Groups.find(gr))!=Groups.end())
  {
    pair<map<unsigned, User*,less<unsigned> >::iterator,bool>p=(*g).second->Users.insert(pair<unsigned, User *>(UserCount+1,new User(UserCount+1,name,pwd)));
      if(p.second)
        return ++UserCount;
  }
  return 0;
}
//--------------------------------------------------------------------------------
void StmCfg::SetRights(unsigned gr,unsigned cl,char rts)
{
  map<unsigned, char,less<unsigned> >::iterator t;
  map<unsigned, UsrGroup *,less<unsigned> >::iterator g;
  if((g=Groups.find(gr))!=Groups.end())
  {
    if((t=(*g).second->AccessClusters.find(cl))!=(*g).second->AccessClusters.end())
      (*t).second=rts;
    else
      (*g).second->AccessClusters.insert(pair<unsigned,char>(cl,rts));
  }
}
//--------------------------------------------------------------------------------
const type_d *StmCfg::Type(unsigned id)
{
  
  map<unsigned,type_d *,less<unsigned> >::const_iterator tp;
  if((tp=Types.find(id))!=Types.end())
  {
    
    return (*tp).second;
  }
  
  return 0;
}
//--------------------------------------------------------------------------------
const obj_d *StmCfg::Obj(unsigned id)
{
  obj_d *obj;
  map<unsigned,type_d *,less<unsigned> >::const_iterator tp;
  
  for(tp=Types.begin();tp!=Types.end();tp++)
    if((obj=(obj_d *)(*tp).second->Obj(id))!=0)
    {
      
      return obj;
    }
  
  return 0;
}
//--------------------------------------------------------------------------------
const char *StmCfg::UserPasswd(unsigned usr)const
{
  map<unsigned, User *,less<unsigned> >::const_iterator u;
  map<unsigned, UsrGroup *,less<unsigned> >::const_iterator g;
  
  for(g=Groups.begin();g!=Groups.end();g++)
    if((u=(*g).second->Users.find(usr))!=(*g).second->Users.end())
    {
      
      return (*u).second->Passwd();
    }
  
  return "";
}
//--------------------------------------------------------------------------------
const char *StmCfg::UserName(unsigned usr)const
{
  map<unsigned, User *,less<unsigned> >::const_iterator u;
  map<unsigned, UsrGroup *,less<unsigned> >::const_iterator g;
  
  for(g=Groups.begin();g!=Groups.end();g++)
    if((u=(*g).second->Users.find(usr))!=(*g).second->Users.end())
    {
      
      return (*u).second->Name();
    }
  
  return "";
}
//--------------------------------------------------------------------------------
unsigned StmCfg::UserId(const char *name)
{
  map<unsigned, User *,less<unsigned> >::iterator u;
  map<unsigned, UsrGroup *,less<unsigned> >::iterator g;
  
  for(g=Groups.begin();g!=Groups.end();g++)
    if((u=find_if((*g).second->Users.begin(),(*g).second->Users.end(),bind2nd(NameKey(),name)))!=(*g).second->Users.end())
    {
      
      return (*u).second->Id();
    }
    
  return 0;
}
//--------------------------------------------------------------------------------
unsigned StmCfg::Group(unsigned usr)
{
  map<unsigned, User *,less<unsigned> >::const_iterator u;
  map<unsigned, UsrGroup *,less<unsigned> >::const_iterator g;
  
  for(g=Groups.begin();g!=Groups.end();g++)
    if((u=(*g).second->Users.find(usr))!=(*g).second->Users.end())
    {
      
      return (*g).first;
    }
  
  return 0;
}
//--------------------------------------------------------------------------------
bool StmCfg::GroupCanObject(unsigned gr,unsigned obj,bool read)
{
  const obj_d *ob;
  map<unsigned, char,less<unsigned> >::iterator i;
  map<unsigned, UsrGroup *,less<unsigned> >::const_iterator g;
  
  if((g=Groups.find(gr))!=Groups.end())
  {
    ob=Obj(obj);
    if(!ob)
    {
      
      return false;    
    }
    if((i=(*g).second->AccessClusters.find(ob->Cluster()))!=(*g).second->AccessClusters.end())
    {
      if(read)
      {
        if((*i).second & 0x1)
        {
          
          return true;
        }
      }
      else
      {
        char x=(*i).second;
        if((*i).second & 0x2)
        {
          
          return true;
        }
      }
    }
  }
  
  return false;
}
//--------------------------------------------------------------------------------
unsigned StmCfg::TypeId(const char *TypeName)
{
  map<unsigned,type_d *,less<unsigned> >::const_iterator tp;
  
  if((tp=find_if(Types.begin(),Types.end(),bind2nd(NameKey(),TypeName)))!=Types.end())
  {
    
    return (*tp).first;
  }
  
  return 0;
}
//--------------------------------------------------------------------------------
unsigned StmCfg::ObjId(unsigned t_id,const char *ObjName)
{
  map<unsigned,obj_d *,less<unsigned> >::const_iterator obj;
  map<unsigned,type_d *,less<unsigned> >::const_iterator tp;
  
  if((tp=Types.find(t_id))!=Types.end())
    if((obj=find_if((*tp).second->Objects.begin(),(*tp).second->Objects.end(),bind2nd(NameKey(),ObjName)))!=(*tp).second->Objects.end())
    {
      
      return (*obj).first;
    }
  
  return 0;

}
//--------------------------------------------------------------------------------
unsigned StmCfg::FldId(unsigned t_id,const char *FldName)
{  
  map<unsigned,type_d *,less<unsigned> >::const_iterator tp;
  map<unsigned,fld_d *,less<unsigned> >::const_iterator fld,f;    
  
  if((tp=Types.find(t_id))!=Types.end())
    if((fld=find_if((*tp).second->Fields.begin(),(*tp).second->Fields.end(),bind2nd(NameKey(),FldName)))!=(*tp).second->Fields.end())
    {      
      return (unsigned)(*fld).first;
    }
  
  return 0;
}
//--------------------------------------------------------------------------------
