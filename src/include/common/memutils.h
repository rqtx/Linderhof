/*
 *  memutils.h
 *
 *  Memory manager
 */
#ifndef MEMUTILS_H
  #define MEMUTILS_H

#define memalloc(pointer, size) memoryalloc( (void **)pointer, size, __func__)
#define memfree(pointer) memoryfree( (void **)pointer)

void memoryalloc( void ** p_ptr, size_t p_size, const char * const p_func );

void memoryfree(void ** p_ptr);

/**
 *  @brief Release all alocated memory
 * */
void memoryclean();

#endif
