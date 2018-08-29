
#ifndef INJECTOR_H
  #define INJECTOR_H

#include <pthread.h>
#include "common/common.h"

typedef enum { CREATED, RUNNING, PAUSED }InjetorStatus;

typedef struct { 
  InjetorStatus status;
  float throughputExpected; //Mb/s (MAX == 0);
  float throughputCurrent;
}InjectionMonitor;

typedef struct {
  pthread_t id;
  bool running;  
}ThreadStat;

typedef struct {
  int injectorId;
  InjectionMonitor monitor;
  ThreadStat *threads;
  unsigned int bucketSize;
  Packet *pkt;
  int socket;
  unsigned int pktCounter; 
  unsigned int incFrequency; //Time of throughput increment frequency in seconds
  float incThroughput;
  time_t timer; //Duration of attack in seconds 0 == INF
  pthread_mutex_t lock;
}Injector;


/**
 * @brief Create injection.
 *
 * @param p_pkt[in] Packet pointer to inject.
 * @param p_thp[in] Initial throughput
 * @param p_time[in] Injection time of operation (Minutes)
 * @param p_incTime[in] Injection increment throughput time (Minutes)
 * @param p_incThp[in] Injection throughput increment size
 * @return Injector Id
 */
int CreateInjection( Packet *p_pkt, float p_thp, unsigned int p_time, unsigned int p_incTime, unsigned int p_incThp );

/**
 * @brief Resume injection.
 *
 * @param id[in]  Injection id
 * @return 1 if SUCCESS 0 if ERROR
 */
int InjectionResume(int p_id);
int InjectionPause(int p_id);
int InjectionDestroy(int p_id);

#endif          /*INJECTOR_H*/
