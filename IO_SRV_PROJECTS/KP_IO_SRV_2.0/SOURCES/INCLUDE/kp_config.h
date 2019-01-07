/********************************************************************	
        Осуществляет конфигурирование драйверного модуля.
                     created by M.E.N.
********************************************************************/
#ifndef _kp_config_h
#define _kp_config_h

#include <stdio.h>
#include <kp_dev.h>
#include <kp_control.h>
#include <kp_functional.h>

using namespace std;
//second пары содержит флаги, отражающие текущее состояние конфигурирования объекта.
typedef vector<pair<ControlObject *,unsigned char> > ObjVectorType;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class KpConfig
{
public:
  KpConfig(KpControl *ctl);
  ~KpConfig();
  bool Init();
private:
  bool Create();                  //создание структур данных, необходимых для функционирования дрв.
  void ClearAll(bool must_die);   //очистка векторов
  void CheckForErrors();          //проверка на конфликты    
  void CheckTypesDescription();   //проверка правильности описания типов и их полей в конфе. (с занасением текущих их id)
  void PrintKillingMessage(const char *);     //вывод сообщения об удалении объекта
  void Clear(ObjVectorType &vk,bool must_die);//очистка вектора без проверки флагов объектов
  void Clear(ObjVectorType &vk,unsigned char flag,bool must_die);//---/-- с проверкой флагов...
  void CheckForErrors(ObjVectorType &vk);
  //временные хранилища создаваемых объектов:
  ObjVectorType TSs;
  ObjVectorType TITs;
  ObjVectorType TITis;
  ObjVectorType TIIs;
  ObjVectorType TUs;
  ObjVectorType LGHs;
  ObjVectorType FBs;
  ObjVectorType KPs;
  ObjVectorType CNLs;
  ObjVectorType CDVs;  
  KpControl *kp_ctl;  
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif