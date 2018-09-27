
#include "strix.h"
#include "planner.h"
#include "executor.h"
#include "common/common.h"

#define NOSIGNAL  0
#define SIGNAL_STOP 1
#define SIGNAL_ERROR 2

#define INVALID_DRAFT 1;

int  ExecuteMemcachedMirror( void *draft )
{
    AttackPlan *plan;
    plan = StrixPlanner( draft );
    return ExecuteAttack(plan);
}


