/****************************************************************************
                     ����� - ������ �������.
                            by M.E.N.
*****************************************************************************/
#ifndef _srv_cln_h
#define _srv_cln_h
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#include <map>
#include <list>
#include <algorithm>
#include <bin_data.h>
#include <stm_srv_cfg.h>
#include <xdr_utils.h>
#include <stm_packets.h>
#include <io_srv_modules.h>
#include <io_srv_event_processor.h>
#include <srv_sock.h>//������ ��� ��������!
#include <srv_tracer.h>

extern "C" {
  char *crypt(const char *str, const char *key);
}

using namespace std;
//-----------------------------------------------------------------
#define STM_USERVERS	11
//-----------------------------------------------------------------
#ifdef WIN32
 const bool IsWin32=true;
#else
 const bool IsWin32=false;
#endif
//----------������� ������-----------------------------------------
typedef struct {
  unsigned char    stm_vers;
  unsigned char    stm_type;
  unsigned short   stm_length;
} STM_BLOCK;
//-----------------------------------------------------------------
const int blk_size=sizeof(STM_BLOCK);
//-----------------------------------------------------------------
#define MAX_PACKET_SIZE 0x7fff
#define CLN_OK          1
#define CLN_FAILED      -1
//--------------������� ��������� �������---------------
enum{
     ST_BEGIN=0,  //�� ������� ��� ������ �����
     ST_KEYS,     //�������� ������ �����������
     ST_LOGIN    ,//�������� ����������� ������
     ST_SRV_LIST, //�������� ������ ��������
     ST_TYPE_LIST,//-//- ������ �����
     ST_OBJ_LIST, //-//- ������ ��������
     ST_REQ,      //-//- ������������� ����������
     ST_OBJ,      //-//- �����. ����. �������
     ST_OBJS,     //-//- ��������
     ST_SET_RESULT,//-//-���������� ��������� ���� �������
     ST_FAILED,
    };
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//------------------ ����� - ������  �������-----------
class SrvClient : public EventProcessor
{
  friend class StmSrv;
public:
  SrvClient(EventSupervisor *,StmCfg*,int id,int sock=-1,int MaxBuffSize=MAX_PACKET_SIZE);
  ~SrvClient();
  int Step();          //��� ��������� ������� � �������
  int Id()const{return id;}
protected:
  void Fail();
  int ReadStep();      //������ �� ������ � ���������� �� ��. �������
  int SendStep();      //������ �� ���. ������� � �����
  void Disconnect();
  void ServiceOrder(unsigned obj_id);
  virtual void ProcessEvent(const IoSrvEvent &ev);
//�������, ����������� ����, ���������� false, ���� ��������� ������ ������ � XDR ��� ���������� ������ ������ � ��������
//----------------��������� �������� �������� (�������� ��������������� �������)----------------
  inline void Cl_User(XDRIn &);
  inline void Cl_Pwd(XDRIn &);
  inline void Cl_SrvList();
  inline void Cl_TypeList();
  inline void Cl_ObjList();
  inline void Cl_AddDelReq(XDRIn &,bool del);
  inline void Cl_GetObj(XDRIn &);
  inline void Cl_SetObj(XDRIn &);
//-----------------������ �� ������� �������� (������������ �������)-----------------
  inline void Send_KEYS();
  inline void Send_RET(unsigned short req_id);//������ �������� - ��� �������
  inline void Send_VER();
  inline void Send_SrvList();
  inline void Send_TypesList();
  inline void Send_ObjList();
  inline void Send_FldVal(XDROut &);
  inline void Send_ObjVal(XDROut &);
  inline void Send_Object(XDROut &,unsigned);  
  inline void Send_AnswSetObj(unsigned req_id,bool sucsess);
  inline void GetCurrTime(struct timeval &);
//-------------------------------------------------------------
  void AddDelReq(unsigned obj_id,const unsigned mask[],int msk_len,bool del);//������� ���������� � �������� �������� ����. ����  
  bool CheckPasswd(const char *pwd);  
  const char *Key();//������� ��������� ���� (��� ���������� ������)
  inline bool PutFldValue(unsigned obj_id,unsigned fld_id,XDROut &,bool ps_str);//��������� ��������-������� ������������� ������
  unsigned Group;       //������������� ������, � ������� ���������� ����
  unsigned cfg_id;      //���������� ������������� � �����
  unsigned short cln_id;//�������������, ������� ������ ������� � ������
  int id;               //������������� ������
  int CurrState;        //������� ��������� �������
  int Sock;             // ���������� ������
  int ReadSize;         //�����:
  int SendSize;
  const int BuffSize;   //������ �������
  bool Failed;
  string name;           //��� (������� �������)
  char *key1;           //����� ����������
  char *key2;
  char ip[16];          //ip-����� (��� �����)
  list<BinData *> InQueue;// ��. / ���. �������
  list<BinData *> OutQueue;
  IoSrvEvent event;     //�������
  StmSrv *srv;          //�������� ��� �������� ���������� ��������
  StmCfg *cfg;          //��������� �� ������������ �������
  SrvTracer requests;   //������� ������� �������  
  time_t last_in_time;  //����� ������� ��������� ������� �� �������
  char *ReadBuff;       //����� �� ������ �� ������
  char *WriteBuff;      //����� �� ������ � �����  
#ifdef MULTI_THREADED
# ifdef WIN32
    HANDLE thread_handle;
# else
    int thread_handle;
# endif
#endif
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif