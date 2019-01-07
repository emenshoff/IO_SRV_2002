/*****************************************************************************
                     created by M.E.N.
*****************************************************************************/
#include <val.h>
#include <stm_cln.h>
#include <stdlib.h>
#include <stm_srv.h>
#include <time_utils.h>
#include <io_srv_dump.h>
#include <io_srv_event_supervisor.h>
extern "C"{
extern char * Win2Koi( char * str );
extern char * Koi2Win( char * str );
}
//-------------------------------------------------------------
//const int pr_flags = ;
//const int cln_flags = ;
//-------------------------------------------------------------
typedef map<unsigned,unsigned char,less<unsigned> > map_type;
//-------------------------------------------------------------
SrvClient::SrvClient(EventSupervisor *svr,StmCfg*conf,int i_d,int sock,int b_size)
 :Sock(sock),Failed(false),cfg(conf),
 name(""),key1(""),key2(""),cln_id(0),
 cfg_id(0),Group(0),ReadSize(0),id(i_d),
 requests(conf), SendSize(0),BuffSize(b_size),
 EventProcessor(svr)
{ 
  last_in_time = time(0);
  ReadBuff=(char *)malloc(BuffSize);  
  WriteBuff=(char *)malloc(BuffSize); 
  event.ev_id = IO_SRV_EV_OBJ_CHANGED;
}
//-------------------------------------------------------------
SrvClient::~SrvClient()
{
  list<BinData *>::iterator i;  
  for(i=InQueue.begin();i!=InQueue.end();i++)
    if((*i))
      delete (*i);
  for(i=OutQueue.begin();i!=OutQueue.end();i++)
    if((*i))
      delete (*i);
  InQueue.clear();
  OutQueue.clear();
  free(ReadBuff);
  free(WriteBuff);
  Disconnect();
}
//-------------------------------------------------------------
void SrvClient::Fail()
{  
  Failed=true;
  Disconnect();
}
//-------------------------------------------------------------
void SrvClient::Disconnect()
{
  if(Sock!=INVALID_SOCKET)
  {
    shutdown(Sock,2);
#ifndef WIN32
    close(Sock);
#else
    closesocket(Sock);
#endif    
  }
  Sock = INVALID_SOCKET;
}
//-------------------------------------------------------------
int SrvClient::Step()
{ 
  if(Sock == INVALID_SOCKET || Failed)
    return CLN_FAILED;
  if(time(0) - last_in_time >= 30)
  {
    dump.Print(D_MSG | D_CLN,"Клиент \"%s \"(%s)(%d): таймаут (30 сек.) ожидания пакетов от клиента!",name.c_str(),ip,id);
    Fail();
    return CLN_FAILED;
  }
  XDRIn Pack;  
  int ret;
  fd_set rcv_set;
  fd_set snd_set;
  fd_set err_set;
  struct timeval tvl;
  unsigned long Key;
  unsigned short Usr;
  list<BinData *>::iterator d_i;
  tvl.tv_sec=0;
  tvl.tv_usec=0;
  FD_ZERO(&rcv_set);
  FD_ZERO(&snd_set);
  FD_ZERO(&err_set);
  FD_SET(Sock,&rcv_set);
  FD_SET(Sock,&snd_set);
  FD_SET(Sock,&err_set);
  ret=select(Sock+1,&rcv_set,&snd_set,0,&tvl);
  if(FD_ISSET(Sock,&err_set)&&ret==SOCKET_ERROR)
  {
    dump.Print(D_MSG | D_CLN | D_STM_PROTO,"Клиент \"%s \"(%s)(%d):select error",name.c_str(),ip,id);
  }
  if(FD_ISSET(Sock,&rcv_set))
  {
    ret = ReadStep();
    if(ret == CLN_FAILED)
      return ret;
  }
  try
  {
    for(d_i=InQueue.begin();d_i!=InQueue.end();d_i++)
    {
      Pack.Set((*d_i)->Data(), (*d_i)->Len());
      Key=Pack.getUInt();       // Извлекаем код(имя) пакета
      Usr=(unsigned short)(Key%0x10000);
      switch(Key/0x10000)       // По коду анализируем пакет
      {
        case C_USER:
          dump.Print(D_MSG | D_CLN | D_STM_PROTO,"Клиент \"%s \"(%s)(%d)->C_USER",name.c_str(),ip,id);
          cln_id=Usr;
          Cl_User(Pack);
          break;
        case C_PASSW:
          dump.Print(D_MSG | D_CLN | D_STM_PROTO,"Клиент \"%s \"(%s)(%d)->C_PASSW",name.c_str(),ip,id);
          Cl_Pwd(Pack);
          break;
        case C_SRVLIST:
          dump.Print(D_MSG | D_CLN | D_STM_PROTO,"Клиент \"%s \"(%s)(%d)->C_SRVLIST",name.c_str(),ip,id);
          Cl_SrvList();
          break;
        case C_TYPELIST:
          dump.Print(D_MSG | D_CLN | D_STM_PROTO,"Клиент \"%s \"(%s)(%d)->C_TYPELIST:",name.c_str(),ip,id);
          Cl_TypeList();          
          break;
        case C_OBJLIST:
          dump.Print(D_MSG | D_CLN | D_STM_PROTO,"Клиент \"%s \"(%s)(%d)->C_OBJLIST:",name.c_str(),ip,id);
          Cl_ObjList();
          break;
        case C_ADDREQ:
          dump.Print(D_MSG | D_CLN | D_STM_PROTO,"Клиент \"%s \"(%s)(%d)->C_ADDREQ",name.c_str(),ip,id);
          Cl_AddDelReq(Pack,false);
          break;
        case C_DELREQ:
          dump.Print(D_MSG | D_CLN | D_STM_PROTO,"Клиент \"%s \"(%s)(%d)->C_DELREQ",name.c_str(),ip,id);
          Cl_AddDelReq(Pack,true);
          break;
        case C_GETOBJ:
          dump.Print(D_MSG | D_CLN | D_STM_PROTO,"Клиент \"%s \"(%s)(%d)->C_GETOBJ",name.c_str(),ip,id);
          Cl_GetObj(Pack);          
          break;
        case C_GETOBJS:
          break;
        case C_SETOBJ:
          dump.Print(D_MSG | D_CLN | D_STM_PROTO,"Клиент \"%s \"(%s)(%d)->C_SETOBJ",name.c_str(),ip,id);
          Cl_SetObj(Pack);
          break;
        default:
          dump.Print(D_ERR | D_CLN | D_STM_PROTO,"Клиент \"%s \"(%s)(%d)->Неизвестный тип пакета!",name.c_str(),ip,id);
          {Fail(); Pack.Free(); return CLN_FAILED;}
          break;
      }
      Pack.Free();
      delete (*d_i);
    }
  }
  catch(XDR_EXCEPT ex)
  {
    if(ex==XDR_PACK_ERROR)
    {
      dump.Print(D_ERR | D_CLN | D_STM_PROTO,"Ошибка упаковки XDR!");
    }
    else
    {
      dump.Print(D_ERR | D_CLN | D_STM_PROTO,"Ошибка распаковки XDR!");
    }
    {Fail(); Pack.Free(); return CLN_FAILED;}
  }
  InQueue.clear();
  if(Sock == INVALID_SOCKET || Failed)//22.04.02
    return CLN_FAILED;
  if(FD_ISSET(Sock,&snd_set))
  {
    ret = SendStep();
    if(ret == CLN_FAILED)
      return ret;
  }
  return CLN_OK;
}

//-------------------------------------------------------------------------------
int SrvClient::ReadStep()
{
  int sz;
  BinData *Bin;
  STM_BLOCK * blk;
  if(Sock == INVALID_SOCKET || Failed)//22.04.02
    return CLN_FAILED;
  sz=recv(Sock,ReadBuff+ReadSize,BuffSize-ReadSize,0);
  if(sz==SOCKET_ERROR || sz==0)
  {
#ifdef WIN32
    dump.Print(D_MSG | D_CLN,"Потеря связи с клиентом %s(%s)(%d)! error code %d",name.c_str(),ip,id,WSAGetLastError());
#else
    if(errno == EAGAIN || errno == EINTR)
      return CLN_OK;
    dump.Print(D_MSG | D_CLN,"Потеря связи с клиентом %s(%s)(%d)! error code %d(%s)",name.c_str(),ip,id,errno,strerror(errno));
#endif
    Fail();
    return CLN_FAILED;
  }  
  last_in_time = time(0);//
  ReadSize+=sz;
  //dump.Print(D_MSG,"клиент %s(%s)(%d): прочитано из сокета %d ,байт. Во входном буфере %d байт.",name.c_str(),ip,id,sz,ReadSize);
  if(ReadSize<blk_size)//обрезок дочитаем потом...
    return CLN_OK;
  blk=(STM_BLOCK *)(ReadBuff);//шапка
  sz=ntohs(blk->stm_length)+blk_size;
  if(ReadSize<sz)//остальное дочитаем потом
    return CLN_OK;
  for(; (ReadSize>=sz); )
  {
    {
      Bin=new BinData(ReadBuff+blk_size,sz-blk_size);//вычитываем пакет и добавляем его во вх. очередь
      InQueue.push_back(Bin);      
      //last_in_time = time(0);//
    }
    if(ReadSize>sz)//
       memmove(ReadBuff,ReadBuff+sz,ReadSize-sz);//сдвигаем данные в буфере на размер прочитанного пакета
    ReadSize-=sz;
    if(ReadSize>blk_size)//если шапка "полная", то повторяем, иначе - до прихода всей шапки
    {
      blk=(STM_BLOCK *)(ReadBuff);
      sz=ntohs(blk->stm_length)+blk_size;
    }
    else
      break;
  }
  return CLN_OK;
  //dump.Print(D_MSG,"Клиент %s: размер вх. очереди: %d",name.c_str(),InQueue.size());  
}
//-------------------------------------------------------------------------------
int SrvClient::SendStep()//выблевывает всю очередь в сокет
{
  int sz;
  BinData *Bin;
  STM_BLOCK * blk;
  list<BinData *>::iterator d_i;
  if(Sock == INVALID_SOCKET || Failed)//22.04.02
    return CLN_FAILED;
  if(OutQueue.size() > 1000)
  {
    Fail();
    dump.Print(D_MSG,"клиент %s(%s)(%d):Переполнение выходной очереди (1000 пакетов).",name.c_str(),ip,id);
    return CLN_FAILED;
  }
  //dump.Print(D_MSG,"Клиент %s: размер вых. очереди: %d",name.c_str(),OutQueue.size());
  for(d_i=OutQueue.begin();d_i!=OutQueue.end();d_i++)  
  {
    Bin=(*d_i);
    if(SendSize+Bin->Len()+blk_size>=BuffSize)
    {
      dump.Print(D_MSG,"клиент %s(%s)(%d):Переполнение выходного буфера",name.c_str(),ip,id);
      break;
    }
    blk=(STM_BLOCK *)(WriteBuff+SendSize);
    blk->stm_vers   = STM_USERVERS;
    blk->stm_type   = 1;
    blk->stm_length = htons(Bin->Len());
    memcpy(WriteBuff+SendSize+blk_size, Bin->Data(), Bin->Len());
    SendSize+=Bin->Len()+blk_size;
    delete (*d_i);
    OutQueue.erase(d_i);
    d_i=OutQueue.begin();
  }
  //OutQueue.erase(OutQueue.begin(),OutQueue.end());
  if(SendSize>0)
  {
    sz=send (Sock, WriteBuff, SendSize,0);
    if(sz==SOCKET_ERROR || !sz)
    {
#ifdef WIN32
      dump.Print(D_MSG | D_CLN,"Потеря связи с клиентом %s(%s)(%d)! error code %d",name.c_str(),ip,id,WSAGetLastError());
#else
      if(errno == EAGAIN || errno == EINTR)
        return CLN_OK;
      dump.Print(D_MSG | D_CLN,"Потеря связи с клиентом %s(%s)(%d)! error code %d(%s)",name.c_str(),ip,id,errno,strerror(errno));
#endif
      Fail();
      return CLN_FAILED;
    }    
    if(sz<SendSize)
    {
      memmove(WriteBuff,WriteBuff+sz,SendSize-sz);
      SendSize-=sz;
    }
    else
      SendSize=0;
    //dump.Print(D_MSG,"клиент %s(%s)(%d): записано в сокет %d ,байт. Во выходном буфере %d байт.",name.c_str(),ip,id,sz,SendSize);
  }
  return CLN_OK;
}
//------------------------------------------------------------------
void SrvClient::ServiceOrder(unsigned obj_id)
{
  const unsigned *fld_req_mask=requests.GetMask(obj_id);
  if(fld_req_mask==0) //ничего не заказано
    return ;
  XDROut out_pack;
  int len,i;
  unsigned mask_elem,fld_id=1;
  struct timeval tv;    
  bool changed = false;  
  const obj_d *obj=cfg->Obj(obj_id);       //получили указатель на объект        
  const unsigned *changed_mask=obj->FldChangesMask();
  len=(obj->Master()->FldCount()+31)/32; 
  for(i=0;i<len;i++)
    if((fld_req_mask[i] & changed_mask[i])!=0)  //а изменились ли заказанные поля?
      changed=true;
  if(!changed)
    return;
  map<unsigned,void *,less<unsigned> >::iterator f_i;
  GetCurrTime(tv);
  try
  {
    dump.Print(D_MSG | D_CLN | D_STM_PROTO,"Отсылка клиенту \"%s\" (%s)(%d) объекта %s...",name.c_str(),ip,id,obj->Name());
    out_pack.putUInt(S_OBJVAL*0x10000+cln_id);
    out_pack.putUInt(obj_id);
    out_pack.putUInt(tv.tv_sec);      //кладем время среза
    out_pack.putUInt(tv.tv_usec);
    for(i=0;i<len;i++)
      out_pack.putUInt(fld_req_mask[i] & changed_mask[i]);// кладем маску...
    for(i=0;i<len;i++)
    {
      if(!fld_req_mask[i])
        continue;
      mask_elem=fld_req_mask[i] & changed_mask[i];
      for(int c=0;c<32;c++,fld_id++)
        if(mask_elem & (1<<c))
          PutFldValue(obj_id,fld_id,out_pack,true);
    }
    Send_ObjVal(out_pack);
  }
  catch(XDR_EXCEPT &)
  {
    dump.Print(D_ERR | D_CLN | D_STM_PROTO,"Ошибка упаковки XDR!");
  } 
}
//------------------------------------------------------------------
void SrvClient::ProcessEvent(const IoSrvEvent &ev)
{    
  switch(ev.ev_id)
  {
    case IO_SRV_EV_OBJS_CHANGED:
      /*{
        map_type *mp;
        map_type::iterator i;
        mp = (map_type*)ev.data;
        for(i = mp->begin();i!=mp->end();i++)
          ServiceOrder((*i).first);
         SendStep(); //немедленно отсылаем...
      }*/
      break;
    case IO_SRV_EV_OBJ_CHANGED:
      ServiceOrder(*(unsigned*)ev.data);
      //dump.Print(D_MSG,"Клиент %s: размер вых. очереди: %d",name.c_str(),OutQueue.size());
      SendStep(); //немедленно отсылаем...
      break;
    case IO_SRV_EV_SHUTDOWN:
      break;
    case IO_SRV_EV_HALT:
      break;
    case IO_SRV_EV_SUSPEND:
      break;
    case IO_SRV_EV_RESUME:
      break;
    default:
      return;
  }    
}
//----------------обработка запросов клиентов----------------
void SrvClient::Cl_User(XDRIn &Pack)
{
  char tmp[256];
  if(CurrState==ST_BEGIN)
  {
    Pack.getString(tmp,256,true);
    name=tmp;
/*
    dump.Print(D_MSG | D_CLN,"1 User %s connected...",tmp);
    char *ptr = &tmp[0];
    for(;*ptr;ptr++)
      dump.Print(D_MSG | D_CLN," %X ",(unsigned char)*ptr);
    dump.Print(D_MSG | D_CLN,"2 User %s connected...",name.c_str());
*/
    if((cfg_id=cfg->UserId(name.c_str()))!=0)
    {
      if(srv->FindDublicate(this))
      {
        dump.Print(D_MSG | D_CLN,"Ошибка авторизации клиента %s(%d) : клиент с именем \"%s\" с адреса [%s] уже есть!",ip,id,name.c_str(),ip);
        Fail();
        return;
      }
      if((Group=cfg->Group(cfg_id))!=0)
      {
        CurrState=ST_KEYS;
        Send_KEYS();
        return;
      }
    }
    dump.Print(D_MSG | D_CLN,"Ошибка авторизации клиента %s(%d) : в текущей конфигурации нет пользователей с именем %s!",ip,id,name.c_str());
    Fail();
  }
  else
  {
    dump.Print(D_ERR | D_CLN,"Недопустимый порядок запросов от клиента %s (%s)(%d)!",name.c_str(),ip,id);
    Fail();
  }
}
//------------------------------------------------------------------
void SrvClient::Cl_Pwd(XDRIn &Pack)
{
  char tmp[256];
  if(CurrState!=ST_KEYS)
  {
    dump.Print(D_ERR | D_CLN,"Недопустимый порядок запросов от клиента %s (%s)(%d)!",name.c_str(),ip,id);
    Fail();
    return;
  }
  CurrState=ST_LOGIN;
  Pack.getString(tmp,256,true);
  if(CheckPasswd(tmp))
  {
    Send_VER();
    dump.Print(D_MSG | D_CLN,"Клиент \"%s\" (%s)(%d) : Авторизация: Ok...",name.c_str(),ip,id);
    Failed=false;
    return;
  }
  Fail();
  Send_RET(C_PASSW);
  dump.Print(D_ERR | D_CLN,"Ошибка авторизации клиента %s (%s)(%d)! Пароль не верен!",name.c_str(),ip,id);
}
//------------------------------------------------------------------
void SrvClient::Cl_SrvList()
{
  //CurrState=ST_SRV_LIST;
  Send_SrvList();
}
//------------------------------------------------------------------
void SrvClient::Cl_TypeList()
{
  CurrState=ST_TYPE_LIST;
  Send_TypesList();
}
//------------------------------------------------------------------
void SrvClient::Cl_ObjList()
{
  CurrState=ST_OBJ_LIST;
  Send_ObjList();
}
//------------------------------------------------------------------
void SrvClient::Cl_AddDelReq(XDRIn &pack,bool del)
{
  XDROut out_pack;
  int len,i;
  unsigned obj_id,fld_id;
  const obj_d *obj;
  struct timeval tv;
  unsigned *flds;
 try
 {
  obj_id=pack.getUInt();
  if((obj=cfg->Obj(obj_id))==0)
  {
    dump.Print(D_ERR | D_CLN,"В конфигурации сервера отсутствует объект с идентификатором %u! Запрос клиента %s (%s)(%d) отклонен!",obj_id,name.c_str(),ip,id);
    return;
  }
  if(!cfg->GroupCanObject(Group,obj_id,true))
  {
    dump.Print(D_ERR | D_CLN,"Клиент \"%s\" (%s)(%d) не имеет права на чтение объекта %s типа %s!",name.c_str(),ip,id,obj->Name(),obj->Master()->Name());
    return;
  }  
  len=(obj->Master()->FldCount()+31)/32;
  flds = (unsigned *)calloc(len,sizeof(unsigned));   
  for(i=0;i<len;i++)
    flds[i]=pack.getUInt();
  if(del)
  {
    AddDelReq(obj_id,flds,len,true);
  }
  else//шлем текущие значения...
  {    
    GetCurrTime(tv);
    AddDelReq(obj_id,flds,len,false);
    out_pack.putUInt(S_OBJVAL*0x10000+cln_id);
    out_pack.putUInt(obj_id);
    out_pack.putUInt(tv.tv_sec);
    out_pack.putUInt(tv.tv_usec);
    for(i=0;i<len;i++)
      out_pack.putUInt(flds[i]);//кладем маску    
    for(i=0;i<len;i++)//кладем поля
      for(int x=0;x<32;x++)
        if((flds[i]>>x)&1)
        {
          fld_id=32*i+x+1;
          if(!PutFldValue(obj_id,fld_id,out_pack,true))
          {
            free(flds);          //освобождаем меморю...
            out_pack.Free();
            return;
          }
        }    
    OutQueue.push_back(new BinData((const void *)out_pack.Buff(),out_pack.PacketLen()));
  }
  free(flds);          //освобождаем меморю...
 }
 catch(XDR_EXCEPT ex)
 {
   out_pack.Free();   
   free(flds);
   throw ex;
 }  
}
//------------------------------------------------------------------
void SrvClient::Cl_GetObj(XDRIn &pack)
{
  XDROut out_pack;
  unsigned obj_id,fld_id,req_id;  
  unsigned code=S_OBJECT*0x10000+cln_id;
  struct timeval tv;
  unsigned *flds;
  const obj_d *obj;
  int len;
try
 {
  GetCurrTime(tv);
  req_id=pack.getUInt();
  out_pack.putUInt(code);
  out_pack.putUInt(req_id);//идентификатор запроса
  out_pack.putUInt(tv.tv_sec);//время среза
  out_pack.putUInt(tv.tv_usec);
  obj_id=pack.getUInt();
  if((obj=cfg->Obj(obj_id))!=0)
  {
    if(!cfg->GroupCanObject(Group,obj_id,true))
    {
      dump.Print(D_ERR | D_CLN,"Клиент \"%s\" (%s)(%d) не имеет права на чтение объекта %s типа %s!",name.c_str(),ip,id,obj->Name(),obj->Master()->Name());
      return ;
    }
    dump.Print(D_MSG | D_CLN | D_STM_PROTO,"Отсылка клиенту %s (%s)(%d) объекта %s...",name.c_str(),ip,id,obj->Name());
    len=(obj->Master()->FldCount()+31)/32;
    flds = (unsigned *)calloc(len,sizeof(unsigned));
    for(int c=0;c<len;c++)
    {
      flds[c]=pack.getUInt();
      out_pack.putUInt(flds[c]);//упаковываем маску
    }
    for(int i=0;i<len;i++)
    {
      for(int x=0;x<32;x++)
      {
        if((flds[i]>>x)&1)
        {
          fld_id=32*i+x+1;
          PutFldValue(obj_id,fld_id,out_pack,true);
        }
      }
    }
    free(flds);
    Send_Object(out_pack,req_id);
    return;
  }
  dump.Print(D_ERR | D_CLN,"В конфигурации отсутствует объект с идентификатором %u. Запрос клиента %s (%s)(%d) отклонен...",obj_id,name.c_str(),ip,id);
 }
 catch(XDR_EXCEPT ex)
 {
   free(flds);
   out_pack.Free();   
   throw ex;
 } 
}
//------------------------------------------------------------------
void SrvClient::Cl_SetObj(XDRIn &pack)
{
  obj_d *obj;
  fld_d *fld;  
  int len;
  unsigned fld_index,fld_count;
  unsigned req_id;//идентификатор запроса
  unsigned obj_id;
  unsigned *flds;
  try
  {
    req_id=pack.getUInt();
    obj_id=pack.getUInt();
    if((obj=(obj_d*)cfg->Obj(obj_id))!=0)
    {
      if(!cfg->GroupCanObject(Group,obj_id,false))
      {        
        dump.Print(D_ERR | D_CLN,"Клиент \"%s\" (%s)(%d) не имеет прав на запись: (%s).(%s)!",name.c_str(),ip,id,obj->Master()->Name(),obj->Name());
        Send_AnswSetObj(req_id,false);
        return ;
      }
      fld_count=obj->Master()->FldCount();
      len=(fld_count+31)/32;
      flds = (unsigned *)calloc(len,sizeof(unsigned));
      for(int i=0;i<len;i++)
          flds[i]=pack.getUInt();
      for(int c=0;c<fld_count;c++)
      {
        for(int i=0;i<32;i++)
        {
          if((flds[c]>>i)&1)
          {
           fld_index=32*c+i+1;
           if((fld=(fld_d*)obj->Master()->Fld(fld_index))!=0)
            switch(fld->Type())//толстый свитч
            {
              case tm_byte:
                {
                  unsigned char val = pack.getUChar();
                  obj->SetFldValue(fld_index,(void*)&val,tm_byte);
                  dump.Print(D_MSG | D_CLN | D_STM_PROTO,"Клиент \"%s\" (%s)(%d): запись: (%s.%s.%s) = %u",name.c_str(),ip,id,obj->Master()->Name(),obj->Name(),obj->Master()->Fld(fld_index)->Name(),val);
                }
                break;
              case tm_char:
                {
                  char val = pack.getChar();
                  obj->SetFldValue(fld_index,(void*)&val,tm_char);
                  dump.Print(D_MSG | D_CLN | D_STM_PROTO,"Клиент \"%s\" (%s)(%d): запись: (%s.%s.%s) =  %d",name.c_str(),ip,id,obj->Master()->Name(),obj->Name(),obj->Master()->Fld(fld_index)->Name(),val);
                }
                break;
              case tm_uint:
                {
                  unsigned short val = pack.getUShort();
                  obj->SetFldValue(fld_index,(void*)&val,tm_uint);
                  dump.Print(D_MSG | D_CLN | D_STM_PROTO,"Клиент \"%s \"(%s)(%u): запись: (%s.%s.%s) =  %u",name.c_str(),ip,id,obj->Master()->Name(),obj->Name(),obj->Master()->Fld(fld_index)->Name(),val);
                }
                break;
              case tm_int:
                {
                  short val = pack.getShort();
                  obj->SetFldValue(fld_index,(void*)&val,tm_int);
                  dump.Print(D_MSG | D_CLN | D_STM_PROTO,"Клиент \"%s \"(%s)(%d): запись: (%s.%s.%s) =  %d",name.c_str(),ip,id,obj->Master()->Name(),obj->Name(),obj->Master()->Fld(fld_index)->Name(),val);
                }
                break;
              case tm_ulong:
                {
                  unsigned long val = pack.getULong();
                  obj->SetFldValue(fld_index,(void*)&val,tm_ulong);
                  dump.Print(D_MSG | D_CLN | D_STM_PROTO,"Клиент \"%s \"(%s)(%d): запись: (%s.%s.%s) =  %u",name.c_str(),ip,id,obj->Master()->Name(),obj->Name(),obj->Master()->Fld(fld_index)->Name(),val);
                }
                break;
              case tm_long:
                {
                  long val = pack.getULong();
                  obj->SetFldValue(fld_index,(void*)&val,tm_long);
                  dump.Print(D_MSG | D_CLN | D_STM_PROTO,"Клиент \"%s \"(%s)(%d): запись: (%s.%s.%s) =  %d",name.c_str(),ip,id,obj->Master()->Name(),obj->Name(),obj->Master()->Fld(fld_index)->Name(),val);
                }
                break;
              case tm_float:
                {
                  float val = pack.getFloat();
                  obj->SetFldValue(fld_index,(void*)&val,tm_float);
                  dump.Print(D_MSG | D_CLN | D_STM_PROTO,"Клиент \"%s \"(%s)(%d): запись: (%s.%s.%s) =  %f",name.c_str(),ip,id,obj->Master()->Name(),obj->Name(),obj->Master()->Fld(fld_index)->Name(),val);
                }
                break;
              case tm_double:
                {
                  double val = pack.getDouble();
                  obj->SetFldValue(fld_index,(void*)&val,tm_double);
                  dump.Print(D_MSG | D_CLN | D_STM_PROTO,"Клиент \"%s \"(%s)(%d): запись: (%s.%s.%s) =  %f",name.c_str(),ip,id,obj->Master()->Name(),obj->Name(),obj->Master()->Fld(fld_index)->Name(),val);
                }
                break;
              case tm_time:
                {
                  struct timeval tv;
                  tv.tv_sec = (long)pack.getULong();
                  tv.tv_usec= (long)pack.getULong();
                  double val = (double)(tv.tv_sec*1e6+tv.tv_usec);
                  obj->SetFldValue(fld_index,(void*)&val,tm_time);
                  dump.Print(D_MSG | D_CLN | D_STM_PROTO,"Клиент \"%s \"(%s)(%d): запись: (%s.%s.%s) =  %u : %u",name.c_str(),ip,id,obj->Master()->Name(),obj->Name(),obj->Master()->Fld(fld_index)->Name(),tv.tv_sec,tv.tv_usec);
                }
                break;                
              case tm_string:
                static char str[256];//last change
                pack.getString(str,256,true);                
                obj->SetFldValue(fld_index,(void*)(str),tm_string);
                dump.Print(D_MSG | D_CLN | D_STM_PROTO,"Клиент \"%s \"(%s)(%d): запись: (%s.%s.%s) =  %s",name.c_str(),ip,id,obj->Master()->Name(),obj->Name(),obj->Master()->Fld(fld_index)->Name(),str);
                break;
            }
           obj->SetFldChangeBit(fld_index);
          }
        }
      }
    }
    unsigned obj_ident = obj->Id();
    free(flds);    
    event.data = (void*)&obj_ident;
    supervisor->ProcessEvent(this,event);
    obj->ClearFldChangesMask();
    Send_AnswSetObj(req_id,true);
  }
  catch(XDR_EXCEPT ex)
  {
    free(flds);
    throw ex;
  }
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//-----------------ответы на запросы клиентов-----------------
void SrvClient::Send_KEYS()//!!
{
  XDROut Pack;
  unsigned long code,ret;
  try
  {
    if((cfg_id=cfg->UserId(name.c_str()))!=0)
    {
      key1=strdup(Key());
      key2=strdup(Key());
      code=S_KEYS*0x10000+cln_id;
      Pack.putULong(code);
      Pack.putString(key1,false);
      Pack.putString(key2,false);
      OutQueue.push_back(new BinData((const void *)Pack.Buff(),Pack.PacketLen()));
      return ;
    }
    else
    {
      dump.Print(D_ERR | D_CLN,"Ошибка авторизации клиента %s (%s)(%d) : в текущей конфигурации нет пользователя с именем %s!", name.c_str(),ip,id,name.c_str());
      ret=C_USER;
      Send_RET(ret);
    }
  }
  catch(XDR_EXCEPT ex)
  {
    Pack.Free();
    throw ex;
  }
}
//------------------------------------------------------------------
void SrvClient::Send_RET(unsigned short req_id)
{
  XDROut Pack;
  unsigned code=S_RET*0x10000+cln_id;
  unsigned ret=req_id*0x10000+1;
  try
  {
    Pack.putUInt(code);
    Pack.putUInt(ret);
    OutQueue.push_back(new BinData((const void *)Pack.Buff(),Pack.PacketLen()));
  }
  catch(XDR_EXCEPT ex)
  {
    Pack.Free();
    throw ex;
  }
}
//------------------------------------------------------------------
void SrvClient::Send_VER()
{
  XDROut Pack;
  try
  {
    Pack.putUInt(S_VERSION*0x10000+cln_id);
    Pack.putUInt((unsigned)STM_USERVERS);
    OutQueue.push_back(new BinData((const void *)Pack.Buff(),Pack.PacketLen()));
  }
  catch(XDR_EXCEPT ex)
  {
    Pack.Free();
    throw ex;
  }
}
//------------------------------------------------------------------
void SrvClient::Send_SrvList()
{
  XDROut Pack;
  try
  {
    Pack.putUInt(S_SRVLIST*0x10000+cln_id);
    Pack.putUInt(1);
    Pack.putString(cfg->Name(),true);
    OutQueue.push_back(new BinData((const void *)Pack.Buff(),Pack.PacketLen()));
  }
  catch(XDR_EXCEPT ex)
  {
    Pack.Free();
    throw ex;
  }
}
//------------------------------------------------------------------
void SrvClient::Send_TypesList()
{  
  map<unsigned, type_d *, less<unsigned> >::const_iterator t_i,t;
  map<unsigned, fld_d *, less<unsigned> >::const_iterator f_i;
  try
  {    
    if(cfg->TypesCount()==1)//достаточно одного пакета
    {
      XDROut pack;
      pack.putUInt(S_TYPELIST*0x10000+cln_id);
      pack.putUInt(cfg->TypesCount()+cfg->TypesCount()*0x10000);
      for(t_i=cfg->Types.begin();t_i!=cfg->Types.end();t_i++)
      {
        pack.putString((*t_i).second->Name(),true);
        pack.putUInt((*t_i).second->FldCount());
        for(f_i=(*t_i).second->Fields.begin();f_i!=(*t_i).second->Fields.end();f_i++)
        {
          pack.putString((*f_i).second->Name(),true);
          pack.putUInt((unsigned)(*f_i).second->Type());
          dump.Print(D_MSG | D_CLN | D_STM_PROTO,"  SRV->Клиент \"%s\" (%s)(%d) : %s.%s",name.c_str(),ip,id,(*t_i).second->Name(),(*f_i).second->Name());
        }
      }
      OutQueue.push_back(new BinData((const void *)pack.Buff(),pack.PacketLen()));
      return;
    }
    else//с повторными пакетами-шлю по одному описанию типа в каждом пакете (так надежней и без извратов)
    {
      for(t_i=cfg->Types.begin();t_i!=cfg->Types.end();t_i++)
      {
        XDROut pack;
        t=cfg->Types.end();
        pack.putUInt(S_TYPELIST*0x10000+cln_id);
        if(t_i==(--t))//последний пакет
          pack.putUInt(1+cfg->TypesCount()*0x10000);
        else
          pack.putUInt((1+cfg->TypesCount()*0x10000)+(0x100));
        pack.putString((*t_i).second->Name(),true);
        pack.putUInt((*t_i).second->FldCount());
        for(f_i=(*t_i).second->Fields.begin();f_i!=(*t_i).second->Fields.end();f_i++)
        {
          pack.putString((*f_i).second->Name(),true);
          pack.putUInt((unsigned)(*f_i).second->Type());
          dump.Print(D_MSG | D_CLN | D_STM_PROTO,"  SRV->Клиент \"%s\" (%s)(%d) : %s.%s",name.c_str(),ip,id,(*t_i).second->Name(),(*f_i).second->Name());
        }
        OutQueue.push_back(new BinData((const void *)pack.Buff(),pack.PacketLen()));
      }
    }
  }
  catch(XDR_EXCEPT ex)
  {
    throw ex;
  }  
}
//------------------------------------------------------------------
//согласно моим расчетам, в одном пакете должно отправляться не более 126 описаний объектов
void SrvClient::Send_ObjList()
{
  bool first=true;
  unsigned objs_sended,obj_full_count=0;
  map<unsigned, obj_d *, less<unsigned> >::const_iterator obj;
  map<unsigned, type_d *, less<unsigned> >::const_iterator type;
  try
  {
    for(type=cfg->Types.begin();type!=cfg->Types.end();type++)
    {
      if((*type).second->ObjCount()<126)//все объекты данного типа помещаются в одном пакете
      {
        XDROut pack;
        if(first)        
        {
          first=false;
          pack.putUInt(S_OBJLIST1*0x10000+cln_id);
          pack.putUInt(cfg->ObjectsCount());
        }
        else
          pack.putUInt(S_OBJLIST2*0x10000+cln_id);
        if(++type==cfg->Types.end())
          pack.putUInt((*(--type)).second->ObjCount());
        else
          pack.putUInt((*(--type)).second->ObjCount()+0x10000);
        pack.putString(cfg->Name(),true);
        pack.putString((*type).second->Name(),true);
        for(obj=(*type).second->Objects.begin();obj!=(*type).second->Objects.end();obj++)
        {
//          if(cfg->GroupCanObject(Group,(*type).first,true))
            pack.putString((*obj).second->Name(),true);
            //
//          else
//            pack.putString("");
            dump.Print(D_MSG | D_CLN | D_STM_PROTO,"  SRV->Клиент \"%s\" (%s)(%d) объект %s",name.c_str(),ip,id,(*obj).second->Name());
        }
        OutQueue.push_back(new BinData((const void *)pack.Buff(),pack.PacketLen()));
        SendStep();//отправляю сразу
      }
      else
      {
        int cicles=(*type).second->ObjCount()/126;
        if((*type).second->ObjCount()%126!=0)cicles++;
        objs_sended=0;//счетчик уже отправленных объектов этого типа
        obj=(*type).second->Objects.begin();
        for(int i=0;i<cicles;i++)//все объекты типа отсылаются кусками не более чем по 126 за раз
        {
          XDROut pack;
          if(first)        
          {
            first=false;
            pack.putUInt(S_OBJLIST1*0x10000+cln_id);
            pack.putUInt(cfg->ObjectsCount());
          }
          else
            pack.putUInt(S_OBJLIST2*0x10000+cln_id);
          if(++type==cfg->Types.end())
          {            
            type--;
            if(i+1==cicles)//последний пакет вообще
            {
              pack.putUInt((*type).second->ObjCount()-objs_sended);
            }
            else
              pack.putUInt(126+0x10000);//последуют еще пакеты
          }
          else
          {
            type--;
            if(i+1==cicles)//последний пакет для этого типа
              pack.putUInt((*type).second->ObjCount()-objs_sended);
            else
              pack.putUInt(126+0x10000);//последуют еще пакеты         
          }
          pack.putString(cfg->Name(),true);
          pack.putString((*type).second->Name(),true);
          for(int c=0;(c<126)&&(obj!=(*type).second->Objects.end());c++,obj++)
          {
//            if(cfg->GroupCanObject(Group,(*type).first,true))
              pack.putString((*obj).second->Name(),true);
//            else
//              pack.putString("");
            dump.Print(D_MSG | D_CLN | D_STM_PROTO,"  SRV->Клиент \"%s\" (%s)(%d) объект %s",name.c_str(),ip,id,(*obj).second->Name());
            objs_sended++;
          }
          obj_full_count+=objs_sended;
          OutQueue.push_back(new BinData((const void *)pack.Buff(),pack.PacketLen()));
          SendStep();//отправляю сразу
        }
      }
    }
  }  
  catch(XDR_EXCEPT ex)
  {
    throw ex;
  }
}
//------------------------------------------------------------------
void SrvClient::Send_FldVal(XDROut &Pack)
{
  OutQueue.push_back(new BinData((const void *)Pack.Buff(),Pack.PacketLen()));
}
//------------------------------------------------------------------
void SrvClient::Send_ObjVal(XDROut &Pack)
{
  OutQueue.push_back(new BinData((const void *)Pack.Buff(),Pack.PacketLen()));
}
//------------------------------------------------------------------
void SrvClient::Send_Object(XDROut &Pack,unsigned req_id)
{
  XDROut EndPack;
  OutQueue.push_back(new BinData((const void *)Pack.Buff(),Pack.PacketLen()));
  try
  {
    EndPack.putUInt(S_OBJEND*0x10000+cln_id);
    EndPack.putUInt(req_id);
  }
  catch(XDR_EXCEPT ex)
  {
    Pack.Free();
    throw ex;
  }
  OutQueue.push_back(new BinData((const void *)EndPack.Buff(),EndPack.PacketLen()));
}
//------------------------------------------------------------------
void SrvClient::Send_AnswSetObj(unsigned req_id,bool success)
{
  XDROut Pack;
  try
  {
    Pack.putUInt(S_SETOBJ*0x10000+cln_id);
    Pack.putUInt(req_id);
    if(success)
      Pack.putUInt(0);
    else
      Pack.putUInt(1);
    OutQueue.push_back(new BinData((const void *)Pack.Buff(),Pack.PacketLen()));
  }
  catch(XDR_EXCEPT ex)
  {
    Pack.Free();
    throw ex;
  }
}
//------------------------------------------------------------------
void SrvClient::AddDelReq(unsigned obj_id,const unsigned mask[],int msk_len,bool del)
{
  if(!mask)
    return;  
  bool founded=false;
  const obj_d *object=cfg->Obj(obj_id);
  if(object==0)
    return;  
  int fld_index;
  unsigned seg;
  for(int i=0;i<(msk_len+31)/32;i++)
  {
    seg=mask[i];
    for(int x=0;x<32;x++)
    {
      if((seg>>x)&1)
      {
        fld_index=32*i+x+1;
        if(object->Master()->Fld(fld_index)!=0)//если у данного типа есть такое поле...
        {
            if(del)//удаление
            {
              dump.Print(D_MSG | D_CLN | D_STM_PROTO,"  Клиент \"%s \"(%s)(%d) : Удален запрос на объект %s поле %s",name.c_str(),ip,id,object->Name(),object->Master()->Fld(fld_index)->Name());
              requests.DelTrace(obj_id,fld_index);
            }
            else//добавление
            {
              dump.Print(D_MSG | D_CLN | D_STM_PROTO,"  Клиент \"%s \"(%s)(%d) : Добавлен запрос на объект %s поле %s",name.c_str(),ip,id,object->Name(),object->Master()->Fld(fld_index)->Name());
              requests.AddTrace(obj_id,fld_index);
            }
        }
      }
    }
  }
}
//------------------------------------------------------------------
bool SrvClient::PutFldValue(unsigned obj_id,unsigned fld_id,XDROut &out_pack,bool ps_str)
{  
  unsigned char f_type;
  const obj_d *obj=cfg->Obj(obj_id);
  if(!obj)
    return false;
  const fld_d *fld=obj->Master()->Fld(fld_id);
  if(!fld)
    return false;
  try
  {
    const void *val = obj->GetFldValue(fld_id,f_type);
    switch(fld->Type())
    {
      case tm_byte:
        out_pack.putUChar(*(unsigned char*)val);
       break;
      case tm_char:
        out_pack.putChar(*(char*)val);
       break;
      case tm_uint:
        out_pack.putUShort(*(unsigned short*)val);
       break;
      case tm_int:
        out_pack.putShort(*(short*)val);
       break;
      case tm_ulong:
        out_pack.putULong(*(unsigned long*)val);
       break;
      case tm_long:
        out_pack.putLong(*(long*)val);
       break;
      case tm_float:
        out_pack.putFloat(*(float*)val);
       break;
      case tm_double:
        out_pack.putDouble(*(double*)val);
       break;
      case tm_time:
        {
          //
          unsigned long ul;
          double time_value=*(double*)val;
          ul=(unsigned long)(time_value/1e6);
          out_pack.putULong(ul);
          ul=(unsigned long)(time_value-(double)ul*1e6);
          out_pack.putULong(ul);
        }
       break;
      case tm_string:
        if(ps_str)//строка в паскалевском формате (для некот. посылок)-ИЗВРАТ! 
        {
          char str[256];
          const char *data = (const char *)val;
          str[0]=(unsigned char)strlen(data);
          strcpy(str+1,data);
          Win2Koi(str+1);
          out_pack.putString(str,false);
        }
        else
        {
          char *str = (char*)(const char *)val;
          out_pack.putString(str,true);
        }
       break;
    }
  }
  catch(XDR_EXCEPT ex)
  {
    out_pack.Free();
    throw ex;
  }
  return true;
}
//------------------------------------------------------------------
bool SrvClient::CheckPasswd(const char *pwd)
{
  char *str=crypt(cfg->UserPasswd(cfg_id),key1);
  str=crypt(str,key2);
  return (!strcmp(str,pwd));
}
//------------------------------------------------------------------
const char *SrvClient::Key()
{  
  static
  char ss[] = "0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[]"
              "abcdefghijklmnopqrstuvwxyz{|}";
  static char key[3];
  key[0] = ss[(rand() % (sizeof(ss) - 1))];
  key[1] = ss[(rand() % (sizeof(ss) - 1))];
  key[2] = 0;
  return key;
}
//------------------------------------------------------------------
void SrvClient::GetCurrTime(struct timeval &tv)
{
#ifdef WIN32
  SYSTEMTIME st;
  GetSystemTime(&st);
  tv.tv_sec=time(0) + _timezone;
  tv.tv_usec=st.wMilliseconds*1000;
#else
  gettimeofday(&tv,NULL);
#endif
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%