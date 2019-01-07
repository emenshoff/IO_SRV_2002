/***********************************************************
    ¬ывод сообщений и трассировка объектов.
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
// «атычки (некрасиво, однако нужно)
extern time_t IO_StartupTime; //врем€ старта (ре-) сервера ввода-вывода
extern time_t IO_InitDevTime; //врем€, необходимое дл€ инициализации данных
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#define D_ERR       0x01        //вывод ошибок
#define D_WARN      0x02        //вывод предупреждений
#define D_MSG       0x04        //вывод сообщений
#define D_PORT      0x08        //уровень порта
#define D_CNL       0x10        //канальный уровень
#define D_DEV       0x20        //уровень устройства
#define D_AFB       0x40        //уровень модул€ устройства
#define D_SENSOR    0x80        //уровень датчика
#define D_SRV       0x100       //уровень сервера вв-выв.
#define D_CLN       0x200       //уровень клиента сервера
#define D_STM_PROTO 0x400       //уровень протокола сервера
#define D_TRACE     0x800       //разрешение трассировки объектов
#define D_TIME      0x1000      //вывод времени в сообщени€х
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
  //дл€ парсера cfg-файла
  unsigned GetObjId(const char*);
  void GetToken(const char*,int);//выделение текущей лексемы
  char *smb;                      //
  int curr_key;                   //текущий ключ (маска)
  unsigned char token_type;       //тип текущей лексемы
  unsigned char state;            //текущее состо€ние кон. автомата
  //типы лексем файла конфигурации:
  enum{KEY,OPER,L_BR,R_BR,BIN_VAL,STR_VAL,NONE}; 
  //состо€ни€ конечного автомата:
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