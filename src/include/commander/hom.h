
#ifndef HOM_H
  #define HOM_H

#include "venus.h"

typedef struct {
  MirrorType type;
  void * atkData;
  void *(*atk_cmd)(void *);
}LhfPlan;

typedef struct MirrorStatusData{
  MirrorType type;
  float throughput;
  float amplification;
  VictimStatus vtcStatus;
  struct MirrorStatusData *next;
}MirrorStatus;

typedef struct MirrorData{
  pthread_t id;
  LhfPlan *plan;
  struct MirrorData *next;
}Mirror;

void HallOfMirrors( LhfPlan *p_plan );

#endif
