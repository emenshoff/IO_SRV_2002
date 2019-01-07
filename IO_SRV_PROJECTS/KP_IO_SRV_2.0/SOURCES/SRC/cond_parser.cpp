// Не готово!
#include <cond_parser.h>
#include <ctype.h>
#include <math.h>
//#include <string.h>
#include <stdio.h>


/*
  в стек помещаем вместо:
  !=  a
  <=  b
  >=  c


  Ранги лексем:
              -число  1
              -поле   1
              '('     0
              ')'     0
              '!'     0
              '<'     -1
              '>'     -1
              '='     -1
              '<='    -1
              '>='    -1
              '='     -1
              '&'     -1
              '|'     -1

*/
//-----------------------------------------------------------------
CondParser::CondParser(StmCfg *cf):Token(0), TokType(NONE), cfg(cf){}
//-----------------------------------------------------------------
CondParser::~CondParser()
{
  ValStack.clear();
  OperStack.clear();
  free(Token);
}
//-----------------------------------------------------------------
bool CondParser::ConditionIsTrue(const char *exp)
{  
  if(!exp)
  {
    return false;
  }  
  if(!cfg)
  {
    Panic();
    return false;
  }
  int len=strlen(exp);
  if(!len)
  {
    Panic();
    return false;
  }
  Rank=0;
  ValStack.clear();
  OperStack.clear();
  free(Token);
  Token=(char*)calloc(len,1);//на всякий случай, беру по максимуму
  Pos=(char *)exp;
  try
  {
    return Parse() == 1;
  }
  catch (char *ex)
  {
    ValStack.clear();
    OperStack.clear();
    throw ex;
  }
  return 0;
}
//-----------------------------------------------------------------
int CondParser::Parse()
{
  double val;
  int res;
  GetToken();
  switch(TokType)
  {
    case NUM:
      val=atof(Token);
      if(OperStack.size()&&OperStack.back()=='!')
      {
        OperStack.pop_back();
        ValStack.push_back(!val);
      }
      else
        ValStack.push_back(val);
      Rank++;
      return Parse();
      break;
    case VAR:
      val=GetValue(Token);
      if(OperStack.size()&&OperStack.back()=='!')
      {
        OperStack.pop_back();
        ValStack.push_back(!val);
      }
      else
        ValStack.push_back(val);
      Rank++;
      return Parse();
      break; 
    case L_DEL:
      OperStack.push_back('(');
      return Parse();
      break;
    case OPER:
      res=CalcStackExp();
      if(res==-1)//выражение не полное
      {
         if(Token[0]=='!')
         {
           if(Token[1]=='=')
             Rank--;             
         }
         else
           Rank--;
      }
      else
        ValStack.push_back((double)res);
      switch(Token[0])
      {
        case '!':
          if(Token[1]=='=')
          {  OperStack.push_back('a'); break; }
          else
            OperStack.push_back(Token[0]);
          break;
        case '&' : case '|' : case '=' :
          OperStack.push_back(Token[0]);
          break;
        case '<':
          if(Token[1]=='=')
          { OperStack.push_back('b'); }
          else 
            OperStack.push_back(Token[0]);
          break;
        case '>':
          if(Token[1]=='=')
          { OperStack.push_back('c'); }
          else 
            OperStack.push_back(Token[0]);
          break;
      }
      return Parse();
    case R_DEL:
      res=CalcStackExp();
      if(res==-1)
        Rank--;
      else
        ValStack.push_back((double)res);
      if(Rank>1||Rank<0||!OperStack.size())
        Panic();
      else
      {
        if(ValStack.size())
        {
          if(OperStack.back()!='(')
            Panic();
          else
            OperStack.pop_back();
        }
      }
      return Parse();         
    case END:
      res=CalcStackExp();
      if(res==-1)
        Rank--;
      else
        ValStack.push_back((double)res);
      if(Rank>1 || Rank<0 || ValStack.size()!=1)
        Panic();
      val=ValStack.back();
      ValStack.pop_back();
      if(OperStack.size()!=0)
      {        
        if(OperStack.back()!='!')
        { OperStack.pop_back();  Panic(); }
        else
        {
          OperStack.pop_back();
          if(val==0)
            return 1;
          return 0;
        }
      }
      else
      { 
        if(val!=0)
          return 1;
        return 0;
      }
      break;
   default:
     Panic();
     break;
  }
  return 0;
}
//-----------------------------------------------------------------
double CondParser::GetValue(const char *str)
{
  obj_d *id;   //описатель объекта
  double ret;  
  void *data;
  unsigned x,i=0,tp,obj,fld;
  unsigned char fld_type;
  static char ErrorMessage[0xff];
  char Obj[0xff],Type[0xff],Fld[0xff];
  for(x=0;str[i]!='.' && x<0xff;x++)
    Type[x]=str[i++];
  i++;
  for(x=0;str[i]!='.' && x<0xff;x++)
    Obj[x]=str[i++];
  i++;
  for(x=0;str[i]!='\0' && x<0xff;x++)
    Fld[x]=str[i++];
  if((tp=(cfg)->TypeId(Type))==0)
  {
    sprintf(ErrorMessage,"Ошибка! В конфигурации сервера отсутствует тип с именем %s!",Type);
    throw ErrorMessage;
  }
  else if((obj=(cfg)->ObjId(tp,Obj))==0)
  {
    sprintf(ErrorMessage,"Ошибка! В конфигурации сервера отсутствует объект с именем %s, или он не принадлежит типу %s!",Obj,Type);
    throw ErrorMessage;
  }
  else if((fld=(cfg)->FldId(tp,Fld))==0) 
  {
    sprintf(ErrorMessage,"Ошибка! У объекта %s отсутствует поле с именем %s!",Obj,Fld);
    throw ErrorMessage;
  }
  else if((id=(obj_d*)(cfg)->Obj(obj))==0)
  {
    sprintf(ErrorMessage,"Ошибка! В конфигурации сервера отсутствует объект с именем %s, или он не принадлежит типу %s!",Obj,Type);
    throw ErrorMessage;
  }
  data = (void*)id->GetFldValue(fld,fld_type);
  switch(fld_type)//все значения привожу к самому толстому типу....
  {
    case tm_byte:
      ret = (double)*(unsigned char*)data;
      //Logdef(Print(DEBUG_STEP,"Type %s Object %s Field %s data %u",Type,Obj,Fld,(unsigned char)*data))
      //Logdef(Print(DEBUG_STEP,"Type %s Object %s Field %s data %f",Type,Obj,Fld,(double)(unsigned char)*data))
     break;
    case tm_char:      
      ret = (double)*(char*)data;
      //Logdef(Print(DEBUG_STEP,"Type %s Object %s Field %s data %d",Type,Obj,Fld,(char)*data))
      //Logdef(Print(DEBUG_STEP,"Type %s Object %s Field %s data %f",Type,Obj,Fld,(double)(char)*data))
     break;
    case tm_uint:      
      ret = (double)*(unsigned short*)data;
      //Logdef(Print(DEBUG_STEP,"Type %s Object %s Field %s data %u",Type,Obj,Fld,(unsigned short)*data))
      //Logdef(Print(DEBUG_STEP,"Type %s Object %s Field %s data %f",Type,Obj,Fld,(double)(unsigned short)*data))
     break;
    case tm_int:      
      ret = (double)*(short*)data;
      //Logdef(Print(DEBUG_STEP,"Type %s Object %s Field %s data %d",Type,Obj,Fld,(short)*data))
      //Logdef(Print(DEBUG_STEP,"Type %s Object %s Field %s data %f",Type,Obj,Fld,(double)(short)*data))
     break;
    case tm_ulong:      
      ret = (double)*(unsigned long*)data;
     // Logdef(Print(DEBUG_STEP,"Type %s Object %s Field %s data %u",Type,Obj,Fld,(unsigned long)*data))
      //Logdef(Print(DEBUG_STEP,"Type %s Object %s Field %s data %f",Type,Obj,Fld,(double)(unsigned long)*data))
     break;
    case tm_long:      
      ret = (double)*(long*)data;
     // Logdef(Print(DEBUG_STEP,"Type %s Object %s Field %s data %d",Type,Obj,Fld,(long)*data))
      //Logdef(Print(DEBUG_STEP,"Type %s Object %s Field %s data %f",Type,Obj,Fld,(double)(long)*data))
     break;
    case tm_float:      
      ret = (double)*(float*)data;
      //Logdef(Print(DEBUG_STEP,"Type %s Object %s Field %s data %f",Type,Obj,Fld,(float)*data))
      //Logdef(Print(DEBUG_STEP,"Type %s Object %s Field %s data %f",Type,Obj,Fld,(double)(float)*data))
     break;
    case tm_double: case tm_time:
      ret=*(double*)data;
      //Logdef(Print(DEBUG_STEP,"Type %s Object %s Field %s data %f",Type,Obj,Fld,(double)*data)))
     break;
    default:
      sprintf(ErrorMessage,"Ошибка! Поле объекта %s имеет недопустимый тип!",Fld);
      throw ErrorMessage;
     break;
  }
  return ret;
}
//-----------------------------------------------------------------
int CondParser::CalcStackExp()
{
  char oper;
  double val1,val2;  
  if(OperStack.size())
    if(OperStack.back()!='('&&OperStack.back()!='!')
      if(ValStack.size()>=2)
      {
        val2=ValStack.back();
        ValStack.pop_back();
        val1=ValStack.back();
        ValStack.pop_back();
        oper=OperStack.back();
        OperStack.pop_back();
        switch (oper)
        {
          case '<':
            return (int)(val1 < val2);
          case '>':
            return (int)(val1 > val2);
          case '=':
            return (int)(val1 == val2);
          case 'a':
            return (int)(val1 != val2);
          case 'b':
            return (int)(val1 <= val2);
          case 'c':
            return (int)(val1 >= val2);
          case '&':
            return (int)(val1 && val2);
          case '|':
            return (int)(val1 || val2);
        }
      }
  return -1;
}
//-----------------------------------------------------------------
void CondParser::GetToken()
{
  int p_cnt,x;
  if(Token[0])
    memset(Token,'\0',strlen(Token));
  if(*Pos=='\0')
  { TokType=END; return; }
  while(isspace(*Pos))
    Pos++;
//.......................operators..............
  if(strchr("<>=!|&",*Pos))
  {
    TokType=OPER;
    Token[0]=*Pos;
    if(strchr("|&=",*Pos))
    {  Pos++; return; }
    if(strchr("<>!",*Pos))
    {
      if(*(++Pos)!='=')
        return;
      Token[1]=*Pos;
      Pos++;
      return;
    }
  }
//.......................delimiters.................
  else 
    if(strchr("()",*Pos))
    {
      Token[0]=*(Pos++);
      if(Token[0]=='(')
      { TokType=L_DEL; return; }
      TokType=R_DEL;
      return;
    }
//.......................vars...................
  else
    if(ValidSmb(*Pos)&&!isdigit(*Pos))
    {
      Token[0]=*Pos;
      for(x=0,p_cnt=0;ValidSmb(*Pos)||*Pos=='.';++Pos,++x)
      {
        Token[x]=*Pos;
        if(*Pos=='.')p_cnt++;
      }
      if(p_cnt==2)
      { TokType=VAR; return; }
    }
//......................numbers section.......................
  else
    if(isdigit(*Pos))
    {
      Token[0]=*Pos;      
      for(p_cnt=0,x=0;isdigit(*Pos)||*Pos=='.';++Pos,++x)
      {
        Token[x]=*Pos;
        if(*Pos=='.')p_cnt++;
      }
      if(p_cnt<2)
      { TokType=NUM; return; } 
    }
  Panic();
}
//-----------------------------------------------------------------
int CondParser::ValidSmb(char ch)
{
#ifdef WIN32
  static const char *SYMBOLS=
    "0123456789_ВЧЮЪДЕГЯКЛМНОПРТФХЖИЦЙЗАЩЬЫЭСШБУвчюъдегяклмнопртфхжицйзащьыэсшбуqwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM";   
#else
  static const char *SYMBOLS=
    "0123456789_чюаядезслмнопртфжицйгкъбэшщьуывхЧЮАЯДЕЗСЛМНОПРТФЖИЦЙГКЪБЭШЩЬУЫВХqwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM";
#endif
  if(strchr(SYMBOLS, ch))
    return 1;
  return 0;
}
//-----------------------------------------------------------------
void CondParser::Panic()
{  
  throw "Неверное выражение!";// исключение должно быть перехвачено!
}
//------------------------------------------------------------------------