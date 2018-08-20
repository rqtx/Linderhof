#include "common/common.h"


typedef struct memoryData{
  void *memory;
  int size;
  struct memoryData *prev;
  struct memoryData *next;
} MemoryData;

int memoryUsed = 0;
MemoryData *memoryAllocad = NULL;

static void createMemoryData( void *p_memory, size_t p_size )
{
  MemoryData *new = malloc( sizeof(MemoryData) );
  
  new->memory = p_memory;
  new->size = p_size;
  
  if( NULL == memoryAllocad )
  {
    new->next = NULL; 
    new->prev = NULL;
  }
  else{
    new->next = memoryAllocad;
    new->prev = memoryAllocad->prev;
    memoryAllocad->prev = new;
  }

  memoryAllocad = new;

  memoryUsed += p_size + sizeof(MemoryData);
}

static void releaseMemory( MemoryData *p_data )
{
  MemoryData *prev, *next;
 
  if( NULL == p_data ){
    return;
  }

  prev = p_data->prev;
  next = p_data->next;

  if( NULL != prev )
  {
    prev->next = next;
  }

  if( NULL != next )
  {
    next->prev = prev;
  }

  free(p_data->memory);
  free(p_data);

  return;
}
void memoryalloc( void ** p_ptr, size_t p_size, const char * const p_func )
{
  if( NULL == p_ptr )
  {
    return;
  }
  
  *p_ptr = malloc( p_size );
        
  if( NULL == *p_ptr )
  {
    char error[50];
    snprintf(error, 50, "[%s] out of memory", p_func);
    error_fatal(error);
  }

  memset(*p_ptr, 0, p_size);

  createMemoryData( *p_ptr, p_size );
  
}

void memoryfree(void ** p_ptr)
{

  for( MemoryData *aux = memoryAllocad; aux != NULL; aux = aux->next )
  {
    if(aux->memory == *p_ptr)
    {
      releaseMemory( aux ); 
      *p_ptr = NULL;
    }    
  }
}

void memoryclean()
{
  for( MemoryData *aux = memoryAllocad; aux != NULL; aux = aux->next )
  {
    releaseMemory(aux); 
  }
  memoryAllocad = NULL;
}
