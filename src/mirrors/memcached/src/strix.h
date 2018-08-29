/*
 *  strix_defines.h
 *    
 */

#ifndef STRIX_DEFINES_H
  #define STRIX_DEFINES_H

#define MEMCACHED_DEFAULT_PORT 11211
#define THROUGHPUT_DEFAULT 0

#include "venus.h"

typedef LhfDraft AttackDraft;

void * ExecuteMemcachedMirror( void *draft );

#endif
