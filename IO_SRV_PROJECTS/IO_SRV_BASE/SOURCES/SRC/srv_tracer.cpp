#include <srv_tracer.h>

#include <stdlib.h>
//-------------------------------------------------------------
SrvTracer::SrvTracer(StmCfg*cf):cfg(cf){}
//--------------------------------------------------------------------
SrvTracer::~SrvTracer()
{
  ClearTraces();
}
//--------------------------------------------------------------------
void SrvTracer::ClearTraces()
{
  map<unsigned, unsigned *,less<unsigned> >::iterator ob;
  for(ob=Traces.begin();ob!=Traces.end();ob++)
    free((*ob).second);
  Traces.clear();
}
//--------------------------------------------------------------------
void SrvTracer::AddTrace(unsigned obj,unsigned fld)
{
  map<unsigned, unsigned *,less<unsigned> >::iterator ob;
  const obj_d *object=cfg->Obj(obj);
  if(!object)
    return;
  if(object->Master()->FldCount() < fld)
      return;
  int index=fld/32;
  if((ob=Traces.find(obj))!=Traces.end())
  {
    (*ob).second[index] |= (1<<(fld-32*index-1)); 
  }
  else
  {
    int mask_len=(object->Master()->FldCount()+31)/32;
    unsigned *new_mask=(unsigned *)calloc(mask_len,sizeof(unsigned));
    new_mask[index] |= (1<<(fld-32*index-1)); 
    Traces.insert(pair<unsigned, unsigned *>(obj,new_mask));
  }
}
//--------------------------------------------------------------------
void SrvTracer::DelTrace(unsigned obj,unsigned fld)
{
  map<unsigned, unsigned *,less<unsigned> >::iterator ob;  
  if((ob=Traces.find(obj))!=Traces.end())
  {
    const obj_d *object=cfg->Obj(obj);
    if(object->Master()->FldCount() < fld)
      return;
    int index=fld/32;
    (*ob).second[index] ^= (1<<(fld-32*index-1));
    if((*ob).second == 0)   //если маска вообще пуста
    {
      free((*ob).second);
      Traces.erase(ob);
    }
  }
}
//--------------------------------------------------------------------
const unsigned *SrvTracer::GetMask(unsigned obj)const
{
  map<unsigned, unsigned *,less<unsigned> >::const_iterator ob;  
  if((ob=Traces.find(obj))!=Traces.end())
    return (*ob).second;
  return 0;
}
//--------------------------------------------------------------------