
#ifndef PLANNER_H
  #define PLANNER_H

#include "venus.h"
#include "hom.h"

extern void *ExecuteMemcachedAttack( void *);

LhfPlan * Planner( LhfDraft *draft );

#endif
