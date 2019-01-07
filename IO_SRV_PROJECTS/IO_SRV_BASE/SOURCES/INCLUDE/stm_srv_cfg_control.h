/*****************************************************************************
       Класс для создания серверной конфигурации
    (разборка бинарного файла, созданного конфигуратором сервера).
    -на основе исходников A.Z.
                     created by M.E.N.
*****************************************************************************/
#ifndef _srv_cfg_control_
#define _srv_cfg_control_

#include <io_srv.h>
#include <stm_srv_cfg.h>
#include <data_block.h>
#include <map>
using namespace std;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
struct compare_key
{
bool operator()(char *st1,char *st2)const
  {
    return strcmp(st1,st2)<0;
  }
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class ConfCreator
{
public:
 ConfCreator(const char *file_name);
 ~ConfCreator();
 StmCfg *ConfCreate();//вызови это для создания конфы
private:
 bool LoadType(DataBlock &);    //загрузка одного типа со всеми его объектами и полями
 bool LoadCluster(DataBlock &); //загрузка кластера доступа
 bool LoadGroup(DataBlock &);   //загрузка группы пользователей
 void ClearInits();             //очистка словарей вспомогательных данных
 bool InitValues();             //инициализация полей объектов
 StmCfg *cfg; 
 char *CfgFileName;
//словари 
 map<char *,unsigned,compare_key > AccessClusters;//ключ - имя кластера, значение-его id в создаваемой конфе
 map<unsigned,map<unsigned,BinData *,less<unsigned> >*,less<unsigned> > Inits;//ключ - id типа, у второго ключ-id поля, second - инициализирующее значение поля
 map<unsigned,DataBlock *,less<unsigned> > IndividualObjInits;//иниц. значения для отдеьных объектов (ключ-id объекта в конфе)
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif