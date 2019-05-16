#ifndef STRIX_DEFINES_H
 #define STRIX_DEFINES_H

#include "venus.h"

#define DNS_DEFAULT_PORT 53
#define THROUGHPUT_DEFAULT 0

typedef struct {
 Packet * getPacket;
 Packet * setPacket; 
 LhfDraft *draft;
}AttackPlan;

#endif