
#ifndef MANAGER_H
  #define MANAGER_H

#include "common/netio.h"

typedef struct {
  pthread_t executorId;
  pthread_t injectorId;
  Packet * getPacket;
  Packet * setPacket; 
  float initialThroughput;
  int incThroughput;
  int incFrequency;          
  int timer;             
}AttackPlan;

int StrixManager( void * draft, int draftSize);
void * CreateDefaultDraft( char* target_ip, char* amp_ip );
int *GetInjectorsId();

#endif
