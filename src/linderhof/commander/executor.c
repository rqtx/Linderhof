/*
 * Hall of Mirrors
 * Responsible for controlling attacks
 */


#include "venus.h"
#include "commander/executor.h" 

#ifdef ORYXNET

#include <pthread.h>
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

void MirrorQueueAdd( LhfPlan *p_plan )
{
    LhfPlan *newPlan = NULL;
    memalloc( &newPlan, sizeof(LhfPlan) );
    memcpy( newPlan, p_plan, sizeof(LhfPlan) );

    InsertCellLast( &plansQueue, newPlan);
}

#endif

int execMirror( LhfPlan *p_plan )
{
    int id = -1;
    Mirror *mirror;
 
    if( (id = p_plan->atk_cmd(p_plan->atkData)) < 0 )
    {
        memfree( &mirror );
        return ERROR_MIRROR; 
    }
    
    return SUCCESS;
}

void  HallOfMirrors( LhfPlan *p_plan )
{
    if( NULL != p_plan )
    {
        execMirror( p_plan );
    }
}

