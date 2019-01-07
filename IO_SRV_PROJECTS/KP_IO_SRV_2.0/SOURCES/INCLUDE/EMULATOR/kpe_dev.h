/***********************************************************
               created by M.E.N
***********************************************************/

#ifndef _kpe_dev_h_
#define _kpe_dev_h_

#include <kp_cdv.h>
#include <kpe_afb.h>
#include <kpe_types.h>
#include <io_srv_control_object.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class KpeDev : public KpAbstractDev
{
public:
  KpeDev(obj_d &);
  ~KpeDev();
  virtual unsigned Type()const;
  virtual void Step(); 
  virtual void ProcPacket(const unsigned char *,unsigned len);//��������� ������ ������   
  virtual const char *Parent()const;  
  inline bool Enabled()const;  //������� ���������� ������         
  unsigned short Delay()const;  
private:
  int curr_item;
  long last_out_time;          //����� ��������� ������� �� ������ ��������  
  KpeAfb *Items[16];           //�����. �����
};
//----------------------------------------------------------------
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#endif //kpe_dev