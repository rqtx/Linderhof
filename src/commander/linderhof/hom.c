#include "venus.h"
#include "commander/planner.h" 
/*
 * Hall of Mirrors
 * Responsible for controlling attacks
 */

void HallOfMirrors( LhfPlan *p_plan )
{
  p_plan->atk_cmd( p_plan->atkData );
}

