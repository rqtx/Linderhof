
#ifndef PLANNER_H
  #define PLANNER_H

#include "venus.h"

extern void ExecuteMemcachedAttack( void *);

typedef struct {
  int atkId;
  char * target_ip;
  char * amp_ip;
  int target_port;
  int amp_port;
  float initialThroughput;
  float incrementThroughput;
  int timeFrequency;
  int nInjections; 
}LhfDraft;

typedef struct {
  MirrorId mirrorId;
  void * atkData;
  void (*atk_cmd)(void *);
}LhfPlan;

LhfPlan * Planner( LhfDraft draft );

#endif
