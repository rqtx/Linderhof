#include "venus.h"
#include "commander/planner.h"
#include "commander/hom.h"
#include "mirrors/memcached.h"

#define ERROR -1
#define ATKDEFAULTPORT -1

bool running = true;

static void _fatalHandler()
{
  memoryclean();
  exit(ERROR);
}

static void _closeHandler()
{
 running = false; 
}

LhfDraft * CreateEmptyDraft()
{
  LhfDraft *emp = NULL;
  
  memalloc( &emp, sizeof(LhfDraft) );
  emp->type = NOTSET;
  emp->target_ip = NULL;
  emp->amp_ip = NULL;
  emp->target_port = ATKDEFAULTPORT;
  emp->amp_port = ATKDEFAULTPORT;
  emp->initialThroughput = 1;
  emp->incrementThroughput = 1;
  emp->timeFrequency = 1;
  emp->timer = 60;

  return emp;
}



void LhfStartController( LhfDraft *p_draft)
{
  
  LhfPlan *plan = NULL;

  signal(SIGINT,  _closeHandler);
  signal(SIGABRT, _fatalHandler);
  signal(SIGTERM, _fatalHandler);
  
  plan = Planner( p_draft );

  HallOfMirrors( plan );

  while(running)
  {
  
  }
}
