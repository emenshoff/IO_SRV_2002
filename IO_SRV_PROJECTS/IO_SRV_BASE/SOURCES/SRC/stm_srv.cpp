 /*****************************************************************************
                     created by M.E.N.
*****************************************************************************/
#include <stm_srv.h>

#include <stm_packets.h>
#include <xdr_utils.h>
#include <stdlib.h>
#include <string.h>

void KillSocket(int sd)
{
  if(sd!=INVALID_SOCKET)
  {
    shutdown(sd,2);
  #ifndef WIN32
    close(sd);
  #else
    closesocket(sd);
  #endif
  }
}
//------------------------------------------------------------------
StmSrv::StmSrv(EventSupervisor *ep,StmCfg *cf,unsigned port,int max_cln)
 :SrvSock(port),MaxCln(max_cln),EventProcessor(ep),cfg(cf)
{
  srand(time(0));
}
//------------------------------------------------------------------
StmSrv::~StmSrv()
{
  list<SrvClient *>::iterator c_i;
  for(c_i=Clients.begin();c_i!=Clients.end();c_i++)
    if((*c_i))
    {
      delete (*c_i);
    }
  Clients.clear();
}
//------------------------------------------------------------------
void StmSrv::Step()
{
  int sock,step_result;
  list<SrvClient *>::iterator c_i; 
  {
    for(c_i=Clients.begin();c_i!=Clients.end();c_i++)
    {
      step_result = (*c_i)->Step();
      if(step_result == CLN_OK)
      {
      }
      else if(step_result == CLN_FAILED)
      {
        if((*c_i)->name.length())
          dump.Print(D_SRV | D_MSG,"Удален клиент %s(%s)(%d)",(*c_i)->name.c_str(),(*c_i)->ip,(*c_i)->id);
        else
          dump.Print(D_SRV | D_MSG,"Удален клиент (%s)(%d)",(*c_i)->ip,(*c_i)->id);
        delete (*c_i);
        Clients.erase(c_i);
        c_i=Clients.begin();
      }
    }
  }
  if((sock=Accept())!=-1)// проверка нового коннекта
    AddClient(sock);  
}
//------------------------------------------------------------------
void StmSrv::AddClient(int sd)
{  
  linger lng;
  lng.l_linger=0;
  lng.l_onoff=1;
  SrvClient *cln=0;
  struct sockaddr_in addr;
  int addr_len=sizeof(addr);
  static char ip[16];
  if(!getpeername(sd,(struct sockaddr *)&addr,(socklen_t*)&addr_len))
  {
    sprintf(ip,"%s",inet_ntoa(addr.sin_addr));
    dump.Print(D_SRV | D_MSG,"Принят запрос на установление соединения с адреса %s.",ip);
  }  
  if(Clients.size()>=MaxCln)
  {
    dump.Print(D_SRV | D_MSG,"Максимум клиентов достигнут. Запрос на коннект не принят!");
    KillSocket(sd);
    return;
  }
  if(setsockopt(sd,SOL_SOCKET,SO_LINGER,(const char *)&lng,sizeof(lng))<0)
  {
#ifndef WIN32
    dump.Print(D_SRV | D_ERR,"Ошибка установки параметров сокета! ERR=%d",errno);
#else
    dump.Print(D_SRV | D_ERR,"Ошибка установки параметров сокета! ERR=%d",WSAGetLastError());
#endif
    dump.Print(D_SRV | D_MSG,"Запрос на установление соединения с адреса %s отклонен!",ip);
    KillSocket(sd);
    return;
  }
#ifndef WIN32
    if(fcntl(sd, F_SETFL, O_NONBLOCK) < 0)
    {
      dump.Print(D_SRV | D_ERR,"Ошибка установки параметров сокета! ERR=%d",errno);
      dump.Print(D_SRV | D_MSG,"Запрос на установление соединения с адреса %s отклонен!",ip);
      KillSocket(sd);
      return;
    }
#else
    unsigned long opt=1;//FIONBIO;
    if(ioctlsocket(sd,FIONBIO,&opt)<0)
    {
      dump.Print(D_SRV | D_ERR,"Ошибка установки параметров сокета! ERR=%d",WSAGetLastError());
      dump.Print(D_SRV | D_MSG,"Запрос на установление соединения с адреса %s отклонен!",ip);
      KillSocket(sd);
      return;
    }
#endif  
  cln=new SrvClient(supervisor,cfg,Clients.size(),sd);//добавляем  
  cln->srv=this;//затычка
  cln->CurrState=ST_BEGIN;// 
  sprintf(cln->ip,"%s",ip);
  Clients.push_back(cln);
}
//-------------------------------------------------------------
void StmSrv::ProcessEvent(const IoSrvEvent &ev)
{
  list<SrvClient *>::const_iterator c_i;  
  switch(ev.ev_id)
  {
    case IO_SRV_EV_OBJS_CHANGED:case IO_SRV_EV_OBJ_CHANGED:
      for(c_i=Clients.begin();c_i!=Clients.end();c_i++)
        (*c_i)->ProcessEvent(ev);
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
//-----------------------------------------------------------------------------
bool StmSrv::FindDublicate(SrvClient *cln)
{
  list<SrvClient *>::const_iterator c_i;
  for(c_i=Clients.begin();c_i!=Clients.end();c_i++)
    if((*c_i)!=cln)
      if(strcmp((*c_i)->name.c_str(),cln->name.c_str())==0 && strcmp((*c_i)->ip,cln->ip)==0)
        return true;
  return false;
}
//-----------------------------------------------------------------------------
/*DWORD WINAPI SrvProc(LPVOID lpParameter)
{
  return 1;
}
//-------------------------------------------------------------
DWORD WINAPI ClnProc(LPVOID cln)
{
  while(1)
  {
    ((SrvClient *)cln)->Step();
  }
  return 1;
}*/