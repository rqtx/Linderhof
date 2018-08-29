#include <pthread.h>

#include "venus.h"
#include "commander/hom.h" 
/*
 * Hall of Mirrors
 * Responsible for controlling attacks
 */

Mirror *mirrors = NULL;

void  HallOfMirrors( LhfPlan *p_plan )
{
  p_plan->atk_cmd(p_plan->atkData);
}
