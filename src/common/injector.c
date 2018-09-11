#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <signal.h>
#include "common/injector.h"

#define MAX_INJECTOR_THREADS 5
typedef enum { INJECTOR, MONITOR, TRAFFIC_SHAPING, TIMER, THPTIMER} THREAD_TYPE;

#define SAMPLING 5
#define MEGABYTE 1000000
#define DEFAULT_SIZE 1


#define ListToInjector( p_list ) ( (Injector *)p_list->data )

List *_injectionDestroyList = NULL;
List *_injectionList = NULL;

static unsigned int _nextid = 0;

static struct timespec _onesec = {
               .tv_sec = 1,                     /* 1 seconds */
               .tv_nsec = 0       /* 0 nanoseconds */
          };

static List * getInjectorListById( int p_id )
{
  List *list_aux;

  LIST_FOREACH( _injectionList, list_aux )
  {
    if( ( (Injector *)list_aux->data )->injectorId == p_id ){
      return list_aux; 
    }
  }

  return NULL;
}

static void *_trafficshapingHandler( void *p_id )
{
  int *id = (int *)p_id;
  Injector *inj = ListToInjector(getInjectorListById(*id));
  
  if( NULL == inj ){
    return NULL; //Keep compiler quiet
  }

  while(1)
  {
    inj->bucketSize = (inj->monitor.throughputExpected * MEGABYTE) / inj->pkt->pkt_size;
    nanosleep(&_onesec, NULL);
  }
    
  return NULL; //kepp compiler quiet
}

static void *_throughputMonitorHandler( void *p_id )
{
  float sample = 0;
  int *id = (int *)p_id;
  Injector *inj = ListToInjector(getInjectorListById(*id));
  
  if( NULL == inj ){
    return NULL; //Keep compiler quiet
  }
  
  while(1)
  {
    for(int i = 0; i < SAMPLING; i++)
    { 
      inj->pktCounter = 0;
      nanosleep(&_onesec, NULL);
      sample += inj->pkt->pkt_size * inj->pktCounter;
    }
 
    inj->monitor.throughputCurrent = (sample/SAMPLING)/MEGABYTE;
    sample = 0;
    nanosleep(&_onesec, NULL);
  };

  return NULL; //Keep compiler quiet
}

static void *_injectorHandler( void *p_id )
{ 
    int *id = (int *)p_id;
    Injector *inj = ListToInjector(getInjectorListById(*id));
    if( NULL == inj )
    {
        return NULL; //Keep compiler quiet
    }
    
    inj->socket = CreateSocket( UDP, false );
    while(1)
    {
        while( inj->bucketSize || ( inj->monitor.throughputExpected <= 0 ) )
        {
            pthread_mutex_lock(&inj->lock);
            if( SendPacket(inj->socket, inj->pkt) != 0 )
            {
                CloseSocket(inj->socket);
                inj->threads[INJECTOR].running = false;
                Efatal( ERROR_NET, "Injection ERROR_INJECTOR\n");
            }
            inj->bucketSize--;
            inj->pktCounter++;
            pthread_mutex_unlock(&inj->lock);
        }
  };

  return NULL; //Keep compiler quiet
}

static void *_timerHandler( void *p_id )
{
  int *id = (int *)p_id;
  Injector *inj = ListToInjector(getInjectorListById(*id));
  struct timespec timeWait;

  if( NULL == inj ){
    return NULL; //Keep compiler quiet
  }
  
  timeWait.tv_sec = inj->timer;
  nanosleep(&timeWait, NULL);
  InjectionDestroy(*id, true);

  return NULL; //Keep compiler quiet
}

// Throughput increment
static void * _thpincHandler( void *p_id )
{
  int *id = (int *)p_id;
  Injector *inj = ListToInjector(getInjectorListById(*id));
  struct timespec timeWait;

  if( NULL == inj ){
    return NULL; //Keep compiler quiet
  }
  
  timeWait.tv_sec = inj->incFrequency;

  while(1)
  {
    nanosleep(&timeWait, NULL);
    inj->monitor.throughputExpected += inj->incThroughput;
 
  }

  return NULL; //Keep compiler quiet
}

int injectionCreate(int p_id)
{
  
    Injector *inj = ListToInjector(getInjectorListById(p_id));
    char buffer[100];

    if( NULL == inj )
    {
        return ERROR_INJECTOR;
    }


    pthread_mutex_init(&inj->lock, NULL);
    InjectionPause(p_id);

    if( 0 != pthread_create(&(inj->threads[INJECTOR].id), NULL, _injectorHandler, &p_id) )
    {
        sprintf (buffer, "Cannot create thread: ERROR_INJECTOR %d\n", p_id);
        InjectionDestroy(inj->injectorId, false);
        ELOG( ERROR_INJECTOR, buffer);
    }

    inj->threads[INJECTOR].running = true;

    if( 0 != pthread_create(&(inj->threads[MONITOR].id), NULL, _throughputMonitorHandler, &p_id) )
    {
        sprintf (buffer, "Cannot create thread: ERROR_INJECTOR %d\n", errno);
        InjectionDestroy(inj->injectorId, false);
        ELOG(ERROR_INJECTOR, buffer);
    }

    inj->threads[MONITOR].running = true;
  
    if( inj->monitor.throughputExpected > 0 )
    {
        if( 0 != pthread_create(&(inj->threads[TRAFFIC_SHAPING].id), NULL, _trafficshapingHandler, &p_id) )
        {
            sprintf (buffer, "Cannot create thread: ERROR_INJECTOR %d\n", errno);
            InjectionDestroy(inj->injectorId, false);
            ELOG(ERROR_INJECTOR, buffer);
        }
        inj->threads[TRAFFIC_SHAPING].running = true;
    }


    if( inj->timer > 0)
    {
        if( 0 != pthread_create(&(inj->threads[TIMER].id), NULL, _timerHandler, &p_id) )
        {
            sprintf (buffer, "Cannot create thread: ERROR_INJECTOR %d\n", errno);
            InjectionDestroy(inj->injectorId, false);
            ELOG(ERROR_INJECTOR, buffer);
        } 
        inj->threads[TIMER].running = true;
    }

    if( inj->incFrequency > 0 )
    {
        if( 0 != pthread_create(&(inj->threads[THPTIMER].id), NULL, _thpincHandler, &p_id) )
        {
            sprintf (buffer, "Cannot create thread: ERROR_INJECTOR %d\n", errno);
            InjectionDestroy(inj->injectorId, false);
            ELOG(ERROR_INJECTOR, buffer);
        }
        inj->threads[THPTIMER].running = true;
    }

    sprintf (buffer, "Created Injection %d\n", p_id);
    ELOG(SUCCESS, buffer);
}

int CreateInjection( Packet *p_pkt, float p_thp, unsigned int p_time, unsigned int p_incTime, unsigned int p_incThp )
{

  Injector *newInject;
    
  memalloc( &newInject,  sizeof( Injector ) ); 
  memalloc( &(newInject->threads), sizeof( ThreadStat ) * MAX_INJECTOR_THREADS );

  newInject->injectorId = _nextid++;
  newInject->monitor.throughputExpected = p_thp;
  newInject->monitor.throughputCurrent = 0;
  newInject->bucketSize = 0;
  newInject->pkt = p_pkt;
  newInject->pktCounter = 0;
  newInject->timer = p_time * 60;
  newInject->incFrequency = p_incTime * 60;
  newInject->incThroughput = p_incThp;
  newInject->monitor.status = CREATED;

  InsertCell( &_injectionList, newInject ); 

  injectionCreate( newInject->injectorId );
  
  return newInject->injectorId;

}

int InjectionPause(int p_id)
{
    Injector *inj = ListToInjector(getInjectorListById(p_id));
    char buffer[100];

    if( NULL == inj )
    {
        sprintf (buffer, "Injection does not exist! \n");
        ELOG(ERROR_INJECTOR, buffer);
    }
  
    pthread_mutex_lock(&inj->lock);
    inj->monitor.status = PAUSED;
  
    printf (buffer, "Injection %d paused.\n", p_id);
    ELOG(SUCCESS, buffer);
}

int InjectionResume(int p_id)
{
    Injector *inj = ListToInjector(getInjectorListById(p_id));
    char buffer[100];

    if( NULL == inj || inj->monitor.status == CREATED)
    {
        sprintf (buffer, "Injection does not exist! \n");
        ELOG(SUCCESS, buffer);
    }

    pthread_mutex_unlock(&inj->lock);
    inj->monitor.status = RUNNING;
    printf (buffer, "Injection %d resumed.\n", p_id);
    ELOG(SUCCESS, buffer);
}

int InjectionDestroy(int p_id, bool p_track)
{
    List *cell = getInjectorListById(p_id); 
    Injector *inj = ListToInjector(cell);
    int *id= NULL;
    char buffer[100];

    if( NULL == inj )
    {
        sprintf (buffer, "Injection does not exist! \n");
        ELOG(SUCCESS, buffer);

    }
  
    InjectionPause( p_id );
 
    for(int i = 0; i < MAX_INJECTOR_THREADS; i++)
    {
        if(inj->threads[i].running)
        {
            pthread_cancel(inj->threads[i].id);
        }
    }

    memfree(&(inj->threads));

    pthread_mutex_destroy(&inj->lock);
  
    RemoveCell(&cell);
   
    if(p_track)
    {
        memalloc( &id, sizeof(int) );
        *id = p_id;
        InsertCellLast( &_injectionDestroyList, id);
    }

    printf (buffer, "Injection %d destroyed.\n", p_id);
    ELOG(SUCCESS, buffer);
}

void DestroyAllInjectors()
{
    while( NULL != _injectionList )
    {
        int injID = ListToInjector(_injectionList)->injectorId;
        InjectionDestroy(injID, false);
    }
}

int GetDestroyed()
{
    int id = *(ListToInt(_injectionDestroyList));
    RemoveCell(&_injectionDestroyList);
    return id;
}
