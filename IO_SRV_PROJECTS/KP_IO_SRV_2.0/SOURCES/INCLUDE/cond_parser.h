/******************************************************************************
	����� ��� �������� �������, �������� ��� ���� ����
                    class by M.E.N.
*******************************************************************************/
#ifndef _cond_parser
#define _cond_parser


#include <stm_srv_cfg.h>

#include <stm_types.h>
#include <vector>
//#include <val.h>

using namespace std;
//-----------------------------------------------------------------
enum TOKTYPE {NONE=0, OPER, NUM, VAR, R_DEL, L_DEL, END};
//-----------------------------------------------------------------
class CondParser
{
public:
 CondParser(StmCfg *cfg);
 ~CondParser();
 bool ConditionIsTrue(const char *);//�������� �������
private:
 double GetValue(const char *);//��������� �������� ����
 int Parse();       //����������� ������ ���������
 int ValidSmb(char);//�������� ��������� ������� � ��������� ����������
 int CalcStackExp();//���� � ������ ��� ����� ���������, �� ��������� ���, ����� ��������� -1 
 void GetToken();   //��������� �������
 void Panic();      //������ ������ ������� ������� � ������... ���������� ����������...
 char *Token;       // ������� �������
 char *Pos;         //������� �������
 int TokType;       // ��� ���. �������
 int Rank;          //��������� ���� ��������� 
 StmCfg *cfg;    //��������� �� ������������
 char ErrorMsg[100];//����� ��� ������ ������
 vector<double> ValStack;//���� ��������
 vector<char> OperStack;//���� �������� 
};
#endif
