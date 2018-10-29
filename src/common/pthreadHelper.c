#include <sched.h>
#include "common/error.h"
#include "common/pthreadHelper.h"

 pthread_t CreateThread( int p_level, void *(*p_func)(void *), void *p_arg )
{
    pthread_attr_t custom_sched_attr;
    int schedLevel;
    pthread_t id;
    struct sched_param param;
    
    if( LVL_FIFO == p_level )
    {
        schedLevel = SCHED_FIFO;
        param.sched_priority = MAX_PRIOLVL(schedLevel);
    }
    else
    {
        schedLevel = SCHED_OTHER;
        if( LVL_HIGH == p_level )
        {
            param.sched_priority = MAX_PRIOLVL(schedLevel);
        }
        else if( LVL_LOW == p_level )
        {
            param.sched_priority = LOW_PRIOLVL(schedLevel);
        }
        else
        {
            param.sched_priority = MID_PRIOLVL(schedLevel);
        }
    }

    pthread_attr_init(&custom_sched_attr);
    pthread_attr_setinheritsched(&custom_sched_attr, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setschedpolicy(&custom_sched_attr, schedLevel);
    pthread_attr_setschedparam(&custom_sched_attr, &param);

    if( 0 != pthread_create(&id, &custom_sched_attr, p_func, p_arg) )
    {
        return ERROR_THREAD;
    }
    return id;
}

void  SetThreadLevel( pthread_t p_pid, int p_level)
{
    struct sched_param param;
    int schedLevel;
    pthread_attr_t custom_sched_attr;

    if( LVL_FIFO == p_level )
    {
        schedLevel = SCHED_FIFO;
        param.sched_priority = MAX_PRIOLVL(schedLevel);
    }
    else
    {
        schedLevel = SCHED_OTHER;
        if( LVL_HIGH == p_level )
        {
            param.sched_priority = MAX_PRIOLVL(schedLevel);
        }
        else if( LVL_LOW == p_level )
        {
            param.sched_priority = LOW_PRIOLVL(schedLevel);
        }
        else
        {
            param.sched_priority = MID_PRIOLVL(schedLevel);
        }
    }
    
    pthread_attr_init(&custom_sched_attr);
    pthread_attr_setinheritsched(&custom_sched_attr, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setschedpolicy(&custom_sched_attr, schedLevel);
    pthread_attr_setschedparam(&custom_sched_attr, &param);
}

void  SetThreadCore(pthread_t p_pid, int p_core)
{
    cpu_set_t cpuset;

    CPU_ZERO(&cpuset);
    CPU_SET(p_core, &cpuset);

    if (pthread_setaffinity_np(p_pid, sizeof(cpu_set_t), &cpuset) != 0) {
        Efatal(ERROR_THREAD, "pthread_setaffinity_np");
    }
}
