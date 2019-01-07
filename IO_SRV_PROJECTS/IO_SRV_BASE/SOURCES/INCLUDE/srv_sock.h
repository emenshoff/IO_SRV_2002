/**********************************************************************************
         Класс для взаимодействия с клиентами через сокет - low level      
                           created by M.E.N.
**********************************************************************************/
#ifndef _srv_sock_h
#define _srv_sock_h
#ifndef WIN32
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>
  #include <netdb.h>
  #include <errno.h>
  #include <sys/time.h>
  #include <time.h>
  #include <unistd.h>
  #include <fcntl.h>
#else
  #include<windows.h>
  #define socklen_t int
#endif

#include <string.h>

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#ifndef WIN32
# define INVALID_SOCKET -1
# define SOCKET_ERROR -1
#endif                                                                    
//------------------------------------------------------------------------
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class SrvSock
{
public:
  SrvSock(unsigned port);
  virtual ~SrvSock();
  bool Ok(){return (ListenSock!=INVALID_SOCKET);}
  virtual void Disconnect();
  int Accept();
protected:
  int ListenSock;//дескриптор сокета
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif