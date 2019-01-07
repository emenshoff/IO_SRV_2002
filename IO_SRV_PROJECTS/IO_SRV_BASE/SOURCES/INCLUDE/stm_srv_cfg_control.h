/*****************************************************************************
       ����� ��� �������� ��������� ������������
    (�������� ��������� �����, ���������� �������������� �������).
    -�� ������ ���������� A.Z.
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
 StmCfg *ConfCreate();//������ ��� ��� �������� �����
private:
 bool LoadType(DataBlock &);    //�������� ������ ���� �� ����� ��� ��������� � ������
 bool LoadCluster(DataBlock &); //�������� �������� �������
 bool LoadGroup(DataBlock &);   //�������� ������ �������������
 void ClearInits();             //������� �������� ��������������� ������
 bool InitValues();             //������������� ����� ��������
 StmCfg *cfg; 
 char *CfgFileName;
//������� 
 map<char *,unsigned,compare_key > AccessClusters;//���� - ��� ��������, ��������-��� id � ����������� �����
 map<unsigned,map<unsigned,BinData *,less<unsigned> >*,less<unsigned> > Inits;//���� - id ����, � ������� ����-id ����, second - ���������������� �������� ����
 map<unsigned,DataBlock *,less<unsigned> > IndividualObjInits;//����. �������� ��� �������� �������� (����-id ������� � �����)
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif