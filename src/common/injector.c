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

#define ERROR 0
#define SUCCESS 1

#define ListToInjector( p_list ) ( (Injector *)p_list->data )

List *_injectionList = NULL;
static unsigned int _nextid = 0;

static struct timespec _onesec = {
               .tv_sec = 1,                     /* 1 seconds */
               .tv_nsec = 0       /* 0 nanoseconds */
          };

static List * _getInjectorListById( int p_id )
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
  Injector *inj = ListToInjector(_getInjectorListById(*id));
  
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
  Injector *inj = ListToInjector(_getInjectorListById(*id));
  
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
  Injector *inj = ListToInjector(_getInjectorListById(*id));
  
  if( NULL == inj ){
    return NULL; //Keep compiler quiet
  }

  while(1)
  {
    while( inj->bucketSize || ( inj->monitor.throughputExpected <= 0 ) )
    {
      pthread_mutex_lock(&inj->lock);
      send_packet(inj->socket, inj->pkt->packet_ptr, inj->pkt->pkt_size, (struct sockaddr *)inj->pkt->saddr);
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
  Injector *inj = ListToInjector(_getInjectorListById(*id));
  struct timespec timeWait;

  if( NULL == inj ){
    return NULL; //Keep compiler quiet
  }
  
  timeWait.tv_sec = inj->timer;
  nanosleep(&timeWait, NULL);
  InjectionDestroy(*id);

  return NULL; //Keep compiler quiet
}

// Throughput increment
static void * _thpincHandler( void *p_id )
{
  int *id = (int *)p_id;
  Injector *inj = ListToInjector(_getInjectorListById(*id));
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
  
  Injector *inj = ListToInjector(_getInjectorListById(p_id));
  
  if( NULL == inj ){
    return ERROR;
  }


  pthread_mutex_init(&inj->lock, NULL);
  InjectionPause(p_id);

  if( 0 != pthread_create(&(inj->threads[INJECTOR].id), NULL, _injectorHandler, &p_id) )
  {
    char buffer[50];
    sprintf (buffer, "Cannot create thread: ERROR %d\n", errno);
    InjectionDestroy(inj->injectorId);
    return ERROR;
  }

  inj->threads[INJECTOR].running = true;

  if( 0 != pthread_create(&(inj->threads[MONITOR].id), NULL, _throughputMonitorHandler, &p_id) )
  {
    char buffer[50];
    sprintf (buffer, "Cannot create thread: ERROR %d\n", errno);
    InjectionDestroy(inj->injectorId);
    return ERROR;
  }

  inj->threads[MONITOR].running = true;
  
  if( inj->monitor.throughputExpected > 0 )
  {
    if( 0 != pthread_create(&(inj->threads[TRAFFIC_SHAPING].id), NULL, _trafficshapingHandler, &p_id) )
    {
      char buffer[50];
      sprintf (buffer, "Cannot create thread: ERROR %d\n", errno);
      InjectionDestroy(inj->injectorId);
      return ERROR;
    }
    inj->threads[TRAFFIC_SHAPING].running = true;
  }


  if( inj->timer > 0)
  {
    if( 0 != pthread_create(&(inj->threads[TIMER].id), NULL, _timerHandler, &p_id) )
    {
      char buffer[50];
      sprintf (buffer, "Cannot create thread: ERROR %d\n", errno);
      InjectionDestroy(inj->injectorId);
      return ERROR;
    }
    
    inj->threads[TIMER].running = true;
  }


  if( inj->incFrequency > 0 )
  {
    if( 0 != pthread_create(&(inj->threads[THPTIMER].id), NULL, _thpincHandler, &p_id) )
    {
      char buffer[50];
      sprintf (buffer, "Cannot create thread: ERROR %d\n", errno);
      InjectionDestroy(inj->injectorId);
      return ERROR;
    }

    inj->threads[THPTIMER].running = true;
  }
  
  return SUCCESS;
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
  newInject->socket = create_socket();
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
  Injector *inj = ListToInjector(_getInjectorListById(p_id));
  
  if( NULL == inj ){
    return ERROR;
  }
  
  pthread_mutex_lock(&inj->lock);
  inj->monitor.status = PAUSED;
  
  return SUCCESS;
}

int InjectionResume(int p_id)
{
  Injector *inj = ListToInjector(_getInjectorListById(p_id));
  
  if( NULL == inj || inj->monitor.status == CREATED){
    return ERROR;
  }

  pthread_mutex_unlock(&inj->lock);
  inj->monitor.status = RUNNING;
  return SUCCESS;
}

int InjectionDestroy(int p_id)
{
  List *cell = _getInjectorListById(p_id); 
  Injector *inj = ListToInjector(cell);
  
  if( NULL == inj ){
    return ERROR;
  }
  
  InjectionPause( p_id );

  //The pkt pointer was allocated by mirror config, so we own it, and it's a good ideia free it here.  
  release_packet( inj->pkt );

  memfree(inj->threads);

  for(int i = 0; i < MAX_INJECTOR_THREADS; i++){
    if(inj->threads[i].running){
      pthread_cancel(inj->threads[i].id);
    }
  }

  pthread_mutex_destroy(&inj->lock);
  
  RemoveCell(cell);

  return SUCCESS;
}

int InjectionNewThroughput(int p_id, int p_newThroughput)
{
  Injector *inj = ListToInjector(_getInjectorListById(p_id));
  
  if( NULL == inj ){
    return ERROR;
  }

    return SUCCESS;
}

