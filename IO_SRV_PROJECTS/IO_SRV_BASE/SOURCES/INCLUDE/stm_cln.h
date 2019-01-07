/****************************************************************************
                     класс - клиент сервера.
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
#include <srv_sock.h>//только для дефайнов!
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
//----------шапочка пакета-----------------------------------------
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
//--------------текущее состояние клиента---------------
enum{
     ST_BEGIN=0,  //не прислал еще первый пакет
     ST_KEYS,     //ожидание ключей кодирования
     ST_LOGIN    ,//ожидание результатов логина
     ST_SRV_LIST, //ожидание списка серверов
     ST_TYPE_LIST,//-//- списка типов
     ST_OBJ_LIST, //-//- списка объектов
     ST_REQ,      //-//- спорадической информации
     ST_OBJ,      //-//- синхр. знач. объекта
     ST_OBJS,     //-//- объектов
     ST_SET_RESULT,//-//-результата установки поля объекта
     ST_FAILED,
    };
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//------------------ Класс - клиент  сервера-----------
class SrvClient : public EventProcessor
{
  friend class StmSrv;
public:
  SrvClient(EventSupervisor *,StmCfg*,int id,int sock=-1,int MaxBuffSize=MAX_PACKET_SIZE);
  ~SrvClient();
  int Step();          //шаг обработки пакетов и заказов
  int Id()const{return id;}
protected:
  void Fail();
  int ReadStep();      //чтение из сокета и добавление во вх. очередь
  int SendStep();      //запись из вых. очереди в сокет
  void Disconnect();
  void ServiceOrder(unsigned obj_id);
  virtual void ProcessEvent(const IoSrvEvent &ev);
//функции, приведенные ниже, возвращают false, если произошла ошибка работы с XDR или логическая ошибка работы с клиентом
//----------------обработка запросов клиентов (разборка соответствующих пакетов)----------------
  inline void Cl_User(XDRIn &);
  inline void Cl_Pwd(XDRIn &);
  inline void Cl_SrvList();
  inline void Cl_TypeList();
  inline void Cl_ObjList();
  inline void Cl_AddDelReq(XDRIn &,bool del);
  inline void Cl_GetObj(XDRIn &);
  inline void Cl_SetObj(XDRIn &);
//-----------------ответы на запросы клиентов (формирование пакетов)-----------------
  inline void Send_KEYS();
  inline void Send_RET(unsigned short req_id);//второй параметр - код запроса
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
  void AddDelReq(unsigned obj_id,const unsigned mask[],int msk_len,bool del);//функция добавления и удаления запросов спор. инфы  
  bool CheckPasswd(const char *pwd);  
  const char *Key();//генерит рандомный ключ (для шифрования пароля)
  inline bool PutFldValue(unsigned obj_id,unsigned fld_id,XDROut &,bool ps_str);//последний параметр-признак паскальевской строки
  unsigned Group;       //идентификатор группы, к которой относиться юзер
  unsigned cfg_id;      //внутренний идентификатор в конфе
  unsigned short cln_id;//идентификатор, который клиент прислал в пакете
  int id;               //идентификатор сеанса
  int CurrState;        //текущее состояние клиента
  int Sock;             // дескриптор сокета
  int ReadSize;         //нужно:
  int SendSize;
  const int BuffSize;   //размер буферов
  bool Failed;
  string name;           //имя (которое прислал)
  char *key1;           //ключи шифрования
  char *key2;
  char ip[16];          //ip-адрес (для логов)
  list<BinData *> InQueue;// вх. / вых. очереди
  list<BinData *> OutQueue;
  IoSrvEvent event;     //событие
  StmSrv *srv;          //заглушка для удаления одинаковых клиентов
  StmCfg *cfg;          //указатель на конфигурацию сервера
  SrvTracer requests;   //запросы данного клиента  
  time_t last_in_time;  //время прихода последней посылки от клиента
  char *ReadBuff;       //буфер на чтение из сокета
  char *WriteBuff;      //буфер на запись в сокет  
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