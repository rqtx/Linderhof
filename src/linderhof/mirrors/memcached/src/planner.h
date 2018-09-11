
#ifndef PLANNER_H
  #define PLANNER_H

#include "venus.h"
#include "manager.h"

AttackPlan * StrixPlanner( LhfDraft *draft );
void DestroyStrixPlan( AttackPlan *plan );

#endif
