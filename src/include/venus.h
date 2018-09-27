/*
 *  venus.h
 */
#ifndef VENUS_H
  #define VENUS_H

#include "common/common.h"

#define DEFAULT_COMPORT 2056

typedef enum { NOTSET, TEST, MEMCACHED, SSDP } MirrorType;
typedef enum { UP, DOWN } VictimStatus;

typedef struct {
  MirrorType type;
  char target_ip[16];
  char amp_ip[16];
  int target_port;
  int amp_port;
  float initialThroughput;
  bool incrementThroughput;
  int timer; //Duration of attack in minutes 0 == INF
}LhfDraft;

#endif
