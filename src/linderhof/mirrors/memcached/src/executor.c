#include "manager.h"
#include "executor.h"
#include "common/common.h"
#include "common/injector.h"


#define FAIL -1
#define STRIKE 0

int ExecuteAttack( AttackPlan * atkData )
{
  int sock = CreateSocket( UDP, false );

  if( SendPacket(sock, atkData->setPacket) < 0 ){
    CloseSocket(sock);
    return FAIL;
  }
   
  atkData->injectorId = CreateInjection( atkData->getPacket, atkData->initialThroughput, atkData->timer, atkData->incFrequency, atkData->incThroughput );
  InjectionResume( atkData->injectorId );  
  
  CloseSocket(sock);
  return STRIKE;
}

