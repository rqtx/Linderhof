
#ifndef HOM_H
  #define HOM_H

#include "venus.h"

typedef struct {
  MirrorType type;
  void * atkData;
  int (*atk_cmd)(void *);
}LhfPlan;

typedef struct MirrorData{
  pthread_t id;
  LhfPlan *plan;
}Mirror;

void HallOfMirrors( LhfPlan *p_plan );

#endif
