/***********************************************************
               created by M.E.N
***********************************************************/

#include <io_srv_dump.h>
#include <io_srv_drv_types.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#define WRONG_TYPE_DESCR    0x01    //тип описан неверно
#define OBJECTS_COUNT_ZERO  0x02    //число объектов типа равно 0
#define MUST_KILL           0x04    //все объекты данного типа не используются
#define TYPE_NOT_FOUND      0x08    //тип не найден
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
FldDscr::FldDscr(const char *n,unsigned char tp)
  :type(tp)
{
  name=strdup(n);
  cfg_fld_id=0;
}
//-------------------------------------------------------------------
FldDscr::~FldDscr()
{
  free(name);
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
TypeDscr::TypeDscr(const char *n)
{
  name=strdup(n);
  flags=0;
}
//-------------------------------------------------------------------
TypeDscr::~TypeDscr()
{
  vector<FldDscr *>::iterator i;
  for(i=fields.begin();i!=fields.end();i++)
    delete (*i);
  fields.clear();
  free(name);
}
//-------------------------------------------------------------------
bool TypeDscr::Init(StmCfg * cfg)
{
  type_d *tp;
  obj_d *obj=0;
  fld_d *fld;
  unsigned tp_id;
  vector<FldDscr *>::iterator i;
  if(((tp_id=cfg->TypeId(name))==0)
    ||((tp=(type_d *)cfg->Type(tp_id))==0))
  {
    dump.Print(D_ERR,"  В конфигурации не обнаружен тип %s!",name);
    flags |= TYPE_NOT_FOUND;
    return false;
  }
  cfg_id=tp_id;
  if(!tp->ObjCount())
  {
    dump.Print(D_MSG,"  Количество объектов типа %s равно 0!",name);
    flags |= OBJECTS_COUNT_ZERO;
    obj=0;
  }
  else
    obj=(*tp->Objects.begin()).second;
  for(i=fields.begin();i!=fields.end();i++)
  {
    fld=(fld_d *)tp->Fld(cfg->FldId(tp->Id(),(*i)->CfgFldName()));
    if(fld!=0)
    {      
      if(fld->Type()==(*i)->Type())
      {
        //dump.Print(D_MSG,"%s.%s = %d ",tp->Name(),(*i)->CfgFldName(),fld->Id());
        (*i)->cfg_fld_id=fld->Id();
        continue;
      }      
      else
        dump.Print(D_ERR,"  У типа %s поле с именем %s имеет недопустимый тип!",name,(*i)->CfgFldName());
    }
    dump.Print(D_ERR,"  У типа %s не описано поле с именем %s!",name,(*i)->CfgFldName());
    flags |= WRONG_TYPE_DESCR;
    return false;
  }
  return true;
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%