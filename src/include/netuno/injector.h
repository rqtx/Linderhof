
#ifndef INJECTOR_H
  #define INJECTOR_H

#include <pthread.h>
#include "common/common.h"
#include "netuno/netuno.h"

#define MAXINJECTORS 10

typedef struct {
    Packet *pkt;
    int socket;
    bool freeBucket;
    unsigned int bucketSize;
    unsigned int pktCounter;
    unsigned int pktDroped;
}InjectorNet;

typedef struct {
    pthread_t id;
    InjectorNet net;
}Injector;

/**
 * @brief Start injector.
 *
 * @param p_pkt[in] Packet data.
 * @return Injectors handler struct 
 */
Injector ** StartInjector( Packet *p_pkt );

/**
 * @brief Destroy injector.
 *
 * @param p_injector[in] Packet data to destroy.
 */
void InjectorDestroy( Injector *p_injector );

#endif          /*INJECTOR_H*/
