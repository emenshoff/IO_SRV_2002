/*****************************************************************************
                     created by M.E.N.
*****************************************************************************/
#include <srv_sock.h>
#include <io_srv_dump.h>
//--------------------------------------------------------------------------
SrvSock::SrvSock(unsigned port)
 :ListenSock(INVALID_SOCKET)
{
#ifdef WIN32
  WORD wVer;
  WSADATA wsaDat;
  wVer=MAKEWORD(1,1);
  if(WSAStartup(wVer,&wsaDat)!=0)
  {
    dump.Print(D_SRV | D_ERR,"Ошибка инициализации сокета WSAGetLastError=%d",WSAGetLastError());
    ListenSock=INVALID_SOCKET;
    return;
  }
#endif
  struct linger lng;
  struct sockaddr_in addr;
  lng.l_linger=0;
  lng.l_onoff=1;
  memset(&addr,0,sizeof(addr));
  addr.sin_family=AF_INET;
  addr.sin_port=htons((unsigned short)port);
  addr.sin_addr.s_addr=INADDR_ANY;
  if((ListenSock=socket(AF_INET,SOCK_STREAM,IPPROTO_IP))==INVALID_SOCKET)
  {
#ifndef WIN32
    dump.Print(D_SRV | D_ERR,"Ошибка создания сокета! ERR=%d",errno);
#else
    dump.Print(D_SRV | D_ERR,"Ошибка создания сокета! ERR=%d",WSAGetLastError());
#endif
    return;
  }
  if(bind(ListenSock,(struct sockaddr *)&addr,sizeof(addr))==SOCKET_ERROR)
  {
#ifndef WIN32
    dump.Print(D_SRV | D_ERR,"Ошибка bind! ERR=%d",errno);
#else
    dump.Print(D_SRV | D_ERR,"Ошибка bind! ERR=%d",WSAGetLastError());    
#endif
	  Disconnect();
    return;
  } 
  if(listen(ListenSock,5)==SOCKET_ERROR)
  {
#ifndef WIN32
    dump.Print(D_SRV | D_ERR,"Ошибка listen! ERR=%d",errno);
#else
    dump.Print(D_SRV | D_ERR,"Ошибка listen! ERR=%d",WSAGetLastError());
#endif
    //затычка 13.04.02
    for(int i=0;i<15;i++)
    {
      #ifndef WIN32
      usleep(1000000);
      #else
      Sleep(1000);
      #endif
      if(listen(ListenSock,5)!=SOCKET_ERROR)
        goto listen_ok;
    }
    Disconnect();
    return;
  }
listen_ok:
  if(setsockopt(ListenSock,SOL_SOCKET,SO_LINGER,(const char *)&lng,sizeof(lng))<0)
  {
#ifndef WIN32
    dump.Print(D_SRV | D_ERR,"Ошибка установки параметров сокета! ERR=%d",errno);
#else
    dump.Print(D_SRV | D_ERR,"Ошибка установки параметров сокета! ERR=%d",WSAGetLastError());
#endif  
    Disconnect();
    return;
  }
#ifndef WIN32
  if(fcntl(ListenSock, F_SETFL, O_NONBLOCK) < 0)
  {
    dump.Print(D_SRV | D_ERR,"Ошибка установки параметров сокета! errno=%d",errno);
    Disconnect();
    return;
  }
#else
  unsigned long opt=FIONBIO;
  if(ioctlsocket(ListenSock,FIONBIO,&opt)<0)
  {
    dump.Print(D_SRV | D_ERR,"Ошибка установки параметров сокета! ERR=%d",WSAGetLastError());
    Disconnect();
    return;
  }
#endif
  dump.Print(D_SRV | D_MSG,"Сокет открыт. Сервер работает на порту %d",port);
}
//--------------------------------------------------------------------------
SrvSock::~SrvSock()
{
  if(ListenSock!=INVALID_SOCKET)
    Disconnect();
}
//--------------------------------------------------------------------------
void SrvSock::Disconnect()
{
  if(ListenSock!=INVALID_SOCKET)
  {
    shutdown(ListenSock,2);
#ifndef WIN32
    close(ListenSock);  
#else
    closesocket(ListenSock);
    WSACleanup();  
#endif
    ListenSock=INVALID_SOCKET;
  }
}
//--------------------------------------------------------------------------
int SrvSock::Accept()
{
  struct sockaddr_in addr;
  int len=sizeof(addr);
  if(!Ok())
    return -1;  
  return accept(ListenSock,(sockaddr *)&addr,(socklen_t *)&len);
}
//--------------------------------------------------------------------------
