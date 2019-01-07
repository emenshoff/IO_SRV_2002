/******************************************************************** 
       ���������� ���������� ��������� ������ ������           
                 created by M.E.N.
*********************************************************************/
#ifndef _srv_h
#define _srv_h
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#include <list>
#include <stm_cln.h>
#include <srv_sock.h>
#include <stm_srv_const.h>
#include <io_srv_dump.h>
#include <io_srv_modules.h>
#include <io_srv_event_processor.h>

using namespace std;
//-----------------------------------------------------------------
class StmSrv : public SrvSock, public EventProcessor
{
public:
  StmSrv(EventSupervisor *,StmCfg *,unsigned port,int max_cln=MAX_CLIENTS);//���� ������ �������� ����� -1, �� ����������� �� �������� �� ���������
  virtual ~StmSrv();
  virtual void Step();
  bool Ok(){return (SrvSock::Ok() && cfg!=0);}
  bool FindDublicate(SrvClient *cln);
  virtual void ProcessEvent(const IoSrvEvent &);//���������� �������� �b ��������� �������
protected:
  void AddClient(int sd);
  list<SrvClient *> Clients;//�������  
  int MaxCln;               //������������ ����� �������� 
  StmCfg *cfg;
};
//-------------------------------------------------------------
struct IfClnZero : unary_function<SrvClient *,bool>
{
  bool operator()(SrvClient *p)
  {
    return (p==0);
  }
};
//-------------------------------------------------------------
/*
#ifdef MULTI_THREADED
# ifdef WIN32
    extern DWORD WINAPI SrvProc(LPVOID lpParameter);
    extern DWORD WINAPI ClnProc(LPVOID cln);
# else
# endif
#endif
*/
//-------------------------------------------------------------
#endif