
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
 * @brief Start injector.
 *
 * @param p_pkt[in] Packet data.
 * @param p_thp[in] Initial throughput
 * @return Injector handler struct 
 */
Injector * StartInjector( Packet *p_pkt, int p_inithp );

/**
 * @brief Destroy injector.
 *
 * @param p_injector[in] Packet data to destroy.
 */
void InjectorDestroy( Injector *p_injector );

#endif          /*INJECTOR_H*/
