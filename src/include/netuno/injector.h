
#ifndef INJECTOR_H
  #define INJECTOR_H

#include <pthread.h>
#include "common/common.h"
#include "netuno/netuno.h"

#define INJECTOR_AUXTHREADS 3
#define MAX_INJECTORCELL_THP 5

typedef struct { 
    float throughputExpected;
    float throughputCurrent;
}InjectorMonitor;

typedef struct {
    Packet *pkt;
    unsigned int bucketSize;
    unsigned int pktCounter;
    unsigned int pktDroped;
}InjectorNet;

typedef struct {
    pthread_t id;
    bool running; 
}ThreadStat;

typedef struct {
    InjectorMonitor monitor;
    InjectorNet net;
    ThreadStat auxThreads[INJECTOR_AUXTHREADS];
}Injector;


/**
 * @brief Create injection.
 *
 * @param p_pkt[in] Packet struct.
 * @param p_thp[in] Initial throughput
 * @param p_time[in] Injection time of operation (Minutes)
 * @param p_incTime[in] Injection increment throughput time (Minutes)
 * @param p_incThp[in] Injection throughput increment size
 * @return Injector Id
 */
Injector * StartInjector( Packet *p_pkt, int p_inithp );
void InjectorDestroy( Injector *injector );

#endif          /*INJECTOR_H*/
