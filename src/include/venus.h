/*
 *  venus.h
 */
#ifndef VENUS_H
  #define VENUS_H

#include "common/common.h"

#define DEFAULT_COMPORT 2056
#define IPSIZE 16
#define LOGFILESIZE 20

typedef enum { NOTSET, TEST, MEMCACHED_GETSET, MEMCACHED_STATS, SSDP } MirrorType;

typedef struct {
  MirrorType type;                  //Type of mirror to use
  char target_ip[IPSIZE];           //Target IP
  char amp_ip[IPSIZE];              //Amplifier IP
  uint32_t target_port;             //Target port
  uint32_t amp_port;                //Amplifier port
  float throughput;                 //Attack throughput
  int timer;                        //Duration of attack in minutes
  char logfile[LOGFILESIZE];        //Log file name
  uint32_t incAttack;               //Enable increment atttack if > 0
}LhfDraft;

#endif
