/*
 *  venus.h
 */
#ifndef VENUS_H
  #define VENUS_H

#include "common/common.h"

#define DEFAULT_COMPORT 2056

typedef enum { NOTSET, TEST, MEMCACHED_GETSET, MEMCACHED_STATS, SSDP } MirrorType;

typedef struct {
  MirrorType type;          //Type of mirror to use
  char target_ip[16];       //Target IP
  char amp_ip[16];          //Amplifier IP
  int target_port;          //Target port
  int amp_port;             //Amplifier port
  float initialThroughput;  //Just used on FULL type
  bool typeThroughput;      //INCREMENT or FULL
  int timer;                //Duration of attack in minutes
}LhfDraft;

#endif
