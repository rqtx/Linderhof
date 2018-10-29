/*
 *  strix_defines.h
 *    
 */

#ifndef STRIX_DEFINES_H
  #define STRIX_DEFINES_H

#include "venus.h"

#define MEMCACHED_DEFAULT_PORT 11211
#define THROUGHPUT_DEFAULT 0

#define MEMCACHED_DRAFT_ERROR -10

typedef enum { STAT, GETSET } MemcachedAtk;

typedef struct {
    Packet * getPacket;
    Packet * setPacket; 
    LhfDraft *draft;
}AttackPlan;

#endif
