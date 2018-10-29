
#ifndef PTHREADHELPER_H
  #define PTHREADHELPER_H
#include <pthread.h>

#define LVL_FIFO    0
#define LVL_HIGH    1
#define LVL_MID     2
#define LVL_LOW     3 

#define MAX_PRIOLVL( sched ) sched_get_priority_max( sched )
#define LOW_PRIOLVL( sched ) sched_get_priority_min( sched )
#define MID_PRIOLVL( sched ) (MAX_PRIOLVL(sched) + LOW_PRIOLVL(sched) )/2

pthread_t CreateThread( int p_level, void *(*p_func)(void *), void *p_arg );
void SetThreadCore( pthread_t p_pid, int p_core );
void SetThreadLevel( pthread_t p_id, int p_level );

#endif
