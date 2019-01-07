/******************************************************************************
	Класс для проверки условий, заданных для ламп щита
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
 bool ConditionIsTrue(const char *);//основная функция
private:
 double GetValue(const char *);//получение значения поля
 int Parse();       //рекурсивный разбор выражения
 int ValidSmb(char);//проверка вхождения символа в множество допустимых
 int CalcStackExp();//если в стеках уже лежит выражение, то вычисляет его, иначе возвратит -1 
 void GetToken();   //выделение лексемы
 void Panic();      //парсер иногда склонен впадать в панику... выплевывая исключение...
 char *Token;       // текущая лексема
 char *Pos;         //текущая позиция
 int TokType;       // тип тек. лексемы
 int Rank;          //суммарный ранг выражения 
 StmCfg *cfg;    //указатель на конфигурацию
 char ErrorMsg[100];//буфер для вывода ошибок
 vector<double> ValStack;//стек значений
 vector<char> OperStack;//стек операций 
};
#endif
