/*
 *  venus.h
 */
#ifndef VENUS_H
  #define VENUS_H

#include "common/common.h"

typedef enum { NOTSET, MEMCACHED, SSDP } MirrorType;
typedef enum { UP, DOWN } VictimStatus;

typedef struct {
  MirrorType type;
  char * target_ip;
  char * amp_ip;
  int target_port;
  int amp_port;
  float initialThroughput;
  float incrementThroughput;
  int timeFrequency; //Time of throughput increment frequency in minutes
  int timer; //Duration of attack in minutes 0 == INF
}LhfDraft;

#endif
