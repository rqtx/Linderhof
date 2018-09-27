#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <signal.h>
#include <math.h> 
#include "netuno/injector.h"
#include "netuno/logger.h"

#define THPINCREMENT 1

typedef enum { INJECTOR, MONITOR, TRAFFIC_SHAPING } THREAD_TYPE;

#define SAMPLING 60
#define MEGABYTE 1000000
#define DEFAULT_SIZE 1

static struct timespec _onesec = {
               .tv_sec = 1,                     /* 1 seconds */
               .tv_nsec = 0       /* 0 nanoseconds */
          };

static void *_trafficshapingHandler( void *p_arg )
{
    Injector *injector = (Injector *)p_arg; 
   
    LOG("Starting traffic shaping\n");
    while(1)
    {
        injector->net.bucketSize = ceil( (injector->monitor.throughputExpected * MEGABYTE) / injector->net.pkt->payload_size );
        nanosleep(&_onesec, NULL);
    }
    return NULL; //kepp compiler quiet
}

static void *_throughputMonitorHandler( void *p_arg )
{
    float sample = 0;
    Injector *injector = (Injector *)p_arg; 

    LOG("Starting throughput monitor\n");
    while(1)
    {
        for(int i = 0; i < SAMPLING; i++)
        { 
            injector->net.pktCounter = 0;
            nanosleep(&_onesec, NULL);
            sample += injector->net.pkt->payload_size * injector->net.pktCounter;
        }
 
        injector->monitor.throughputCurrent = (sample/SAMPLING)/MEGABYTE;
        sample = 0;
        injector->net.pktDroped = 0;
        nanosleep(&_onesec, NULL);
    }

    return NULL; //Keep compiler quiet
}

static void *_injectorHandler( void *p_arg )
{
    Injector *injector = (Injector *)p_arg;

    LOG("Starting injector\n");
    while(1)
    {
        while( injector->net.bucketSize )
        {
            if( 0 == SendPacket(injector->net.pkt) )
            {
                //Packet was sent
                injector->net.bucketSize--;
                injector->net.pktCounter++;
            }
            else
            {
                injector->net.pktDroped++;
            }
        }
  };

  return NULL; //Keep compiler quiet
}

void injectorBootstrap( Injector *injector )
{

    if( 0 != pthread_create(&(injector->auxThreads[INJECTOR].id), NULL, _injectorHandler, injector) )
    {
        Efatal( ERROR_INJECTOR, "Cannot create thread: ERROR_INJECTOR\n");
    }
    injector->auxThreads[INJECTOR].running = true;

    if( 0 != pthread_create(&(injector->auxThreads[MONITOR].id), NULL, _throughputMonitorHandler, injector) )
    {
        Efatal(ERROR_INJECTOR, "Cannot create thread: ERROR_INJECTOR\n");
    }
    injector->auxThreads[MONITOR].running = true;
  
    if( 0 != pthread_create(&(injector->auxThreads[TRAFFIC_SHAPING].id), NULL, _trafficshapingHandler, injector) )
    {
        Efatal(ERROR_INJECTOR, "Cannot create thread: ERROR_INJECTOR\n");
    }
    injector->auxThreads[TRAFFIC_SHAPING].running = true;
}

Injector * StartInjector( Packet *p_pkt, int p_inithp)
{
    Injector *injector = NULL;
    memalloc(&injector, sizeof(Injector));

    injector->monitor.throughputExpected = p_inithp;
    injector->monitor.throughputCurrent = 0;
    injector->net.pkt = p_pkt;
    injectorBootstrap(injector); 
    return injector;
}

void InjectorDestroy( Injector *injector )
{
    for(int j = 0; j < INJECTOR_AUXTHREADS; j++)
    {
        if(injector->auxThreads[j].running)
        {
            pthread_cancel(injector->auxThreads[j].id);
        }
    }
    memfree(&injector);
    LOG("Injector destroyed.\n");
}
