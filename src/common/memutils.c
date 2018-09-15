#include "common/common.h"


typedef struct memoryData{
  void *memory;
  int size;
  struct memoryData *next;
} MemoryData;

int _memoryUsed = 0;
MemoryData *_memoryAllocad = NULL;

static void createMemoryData( void *p_memory, size_t p_size )
{
    MemoryData *new = malloc( sizeof(MemoryData) );
  
    new->memory = p_memory;
    new->size = p_size;
    new->next = _memoryAllocad;
    _memoryUsed += p_size + sizeof(MemoryData);
}

static void releaseMemory( MemoryData *p_data )
{
 
  if( NULL == p_data )
  {
    return;
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
    Efatal( ERROR_MEMORY, error);
  }

  memset(*p_ptr, 0, p_size);

  createMemoryData( *p_ptr, p_size );
  
}

void memoryfree(void ** p_ptr)
{
    MemoryData *auxData = NULL;
    MemoryData *prev = NULL;

    if( NULL == p_ptr  || NULL == *p_ptr )
    {
        return;
    }

    for(auxData = _memoryAllocad; auxData != NULL; auxData = auxData->next )
    {
        if(auxData->memory == *p_ptr)
        {
            if( NULL != prev )
            {
                prev->next = auxData->next;
            }
            else
            {
                _memoryAllocad = auxData->next;
            }
            releaseMemory( auxData->memory );
            *p_ptr = NULL;
            return;
        }    
        prev = auxData;
    }
}

void memoryclean()
{
    while( _memoryAllocad != NULL )
    {
        releaseMemory( _memoryAllocad );
    }
    _memoryAllocad = NULL;
}
