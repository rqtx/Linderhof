#include "manager.h"
#include "executor.h"
#include "common/common.h"
#include "common/injector.h"


#define FAIL -1
#define STRIKE 0

int ExecuteAttack( AttackData * atkData )
{
  if( !send_data(atkData->setPacket->packet_ptr, atkData->setPacket->pkt_size, (struct sockaddr *)atkData->setPacket->saddr) ){
    return FAIL;
  }
   
  atkData->injectorId = CreateInjection( atkData->getPacket, atkData->initialThroughput, atkData->timer, atkData->incFrequency, atkData->incThroughput );
  InjectionResume( atkData->injectorId );  
  
  return STRIKE;
}

void DestroyAttack( AttackData * atkData )
{
  if( NULL == atkData ){
    return;
  }

  InjectionDestroy(atkData->injectorId);
}

