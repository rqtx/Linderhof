/*
 * Hall of Mirrors
 * Responsible for controlling attacks
 */

#include <pthread.h>

#include "venus.h"
#include "commander/hom.h" 

#define GetPlanFromList( p_list ) (LhfPlan *)p_list->data;
List *plansQueue = NULL;
List *mirrorList = NULL;

Mirror * createMirror()
{
    Mirror *mirror = NULL;
    memalloc( &mirror, sizeof(Mirror) );
    return mirror;
}

LhfPlan * getNextPlan()
{
    List *first = plansQueue;
    LhfPlan * plan;
    
    if( NULL == first )
    {
        return NULL;
    }

    plan = GetPlanFromList(first);
    plansQueue = plansQueue->next;
    RemoveCell( &first );

    return plan;

}

int execMirror( LhfPlan p_plan )
{
    int id = -1;
    Mirror *mirror;
     
    if( (id = p_plan.atk_cmd(p_plan.atkData)) < 0 )
    {
        memfree( &mirror );
        return ERROR_MIRROR; 
    }
    
    mirror = createMirror();
    mirror->id = id;
    memalloc( &mirror->plan, sizeof(LhfPlan) );
    mirror->plan->type = p_plan.type;
    mirror->plan->atk_cmd = p_plan.atk_cmd;
    mirror->plan->atkData = p_plan.atkData;
    InsertCell( &mirrorList, (void *)mirror );
    return SUCCESS;
}

void  HallOfMirrors( LhfPlan *p_plan )
{
    if( NULL != p_plan )
    {
        execMirror( *p_plan );
    }
}

void MirrorQueueAdd( LhfPlan *p_plan )
{
    LhfPlan *newPlan = NULL;
    memalloc( &newPlan, sizeof(LhfPlan) );
    memcpy( newPlan, p_plan, sizeof(LhfPlan) );

    InsertCellLast( &plansQueue, newPlan);
}
