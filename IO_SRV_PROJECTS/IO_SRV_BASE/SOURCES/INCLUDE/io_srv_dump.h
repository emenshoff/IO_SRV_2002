/***********************************************************
    ����� ��������� � ����������� ��������.
               created by M.E.N
***********************************************************/

#ifndef _dump_h_
#define _dump_h_

#include <time.h>
#include <stdio.h>
#include <def_crit_sec.h>
//#include <win2koi_koi2win.h>
#include <io_srv_modules.h>
#include <io_srv_event_processor.h>
#include <map>
#include <string>

using namespace std;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
extern string DumpFileName;
extern string DumpCfgFileName;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// ������� (���������, ������ �����)
extern time_t IO_StartupTime; //����� ������ (��-) ������� �����-������
extern time_t IO_InitDevTime; //�����, ����������� ��� ������������� ������
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#define D_ERR       0x01        //����� ������
#define D_WARN      0x02        //����� ��������������
#define D_MSG       0x04        //����� ���������
#define D_PORT      0x08        //������� �����
#define D_CNL       0x10        //��������� �������
#define D_DEV       0x20        //������� ����������
#define D_AFB       0x40        //������� ������ ����������
#define D_SENSOR    0x80        //������� �������
#define D_SRV       0x100       //������� ������� ��-���.
#define D_CLN       0x200       //������� ������� �������
#define D_STM_PROTO 0x400       //������� ��������� �������
#define D_TRACE     0x800       //���������� ����������� ��������
#define D_TIME      0x1000      //����� ������� � ����������
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class Dump : public EventProcessor
{
public:
  Dump();
  ~Dump();
  bool Ok()const{return ready;}  
  bool ResetDump(const char *dmp_name);
  bool ResetTraces(StmCfg *conf,const char *cfg_name,const char *dmp_name);
  int Flags()const{return flags;}  
  void Print(const char *fmt,...);
  void Print(int flag,const char *fmt,...);
  void ObjMsg(unsigned obj_id,int flag,const char *fmt,...);
  virtual void ProcessEvent(const IoSrvEvent &event);
protected:
  void Shutdown();
  void UpdateCfg();
  void DumpObject(unsigned);
  int flags;
  time_t cfg_file_time;
  time_t cfg_file_mtime;
  char buff[0xfff];
  char *cfg_file_name;
  char *dmp_file_name;
  map<unsigned ,unsigned char,less<unsigned> > traces;
  FILE *dump_file;
  unsigned long line;
  StmCfg *cfg;
  //��� ������� cfg-�����
  unsigned GetObjId(const char*);
  void GetToken(const char*,int);//��������� ������� �������
  char *smb;                      //
  int curr_key;                   //������� ���� (�����)
  unsigned char token_type;       //��� ������� �������
  unsigned char state;            //������� ��������� ���. ��������
  //���� ������ ����� ������������:
  enum{KEY,OPER,L_BR,R_BR,BIN_VAL,STR_VAL,NONE}; 
  //��������� ��������� ��������:
  enum{GET_STR_KEY,GET_BIN_KEY,END,ERR,NEXT};
private:
  Dump(Dump&):EventProcessor(0){}
  void operator=(Dump&){}
  bool ready;
  static int obj_count;
  CRIT_SEC_DESCRIPT(CrtSec);
};
//----------------------------------------------------------------
extern Dump dump;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif //dump