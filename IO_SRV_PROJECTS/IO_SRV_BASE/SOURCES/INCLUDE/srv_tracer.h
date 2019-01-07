/**********************************************************************************
      ����� �������� �� ���������� ����� ��������  
                     created by M.E.N.
**********************************************************************************/
#ifndef _srv_tracer_h
#define _srv_tracer_h

#include <stm_srv_cfg.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class SrvTracer
{           
public:
 SrvTracer(StmCfg *conf);
 ~SrvTracer();
 void ClearTraces();                      //������� ��� ������������
 void AddTrace(unsigned obj,unsigned fld);//�������� ������ �� ������������ �������� ����
 void DelTrace(unsigned obj,unsigned fld);//������� ���� ������������ 
 const unsigned *GetMask(unsigned)const;  //�������� ����� ������� ����� �������
private:
 StmCfg*cfg;
//���� - ������������� ������� � �����, ������� - ����� ���������� �����
 map<unsigned, unsigned *,less<unsigned> > Traces;
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif