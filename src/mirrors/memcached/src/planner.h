
#ifndef PLANNER_H
  #define PLANNER_H

#include "venus.h"
#include "manager.h"

AttackPlan * StrixPlanner( LhfDraft *draft, int draftSize );
void DestroyStrixPlan( AttackPlan **plan );

#endif
