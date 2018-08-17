/*
 *  error.h
 *
 */

#ifndef ERROR_H
  #define ERROR_H

#include <errno.h>

void error_fatal( char * p_error );
void error_warning( char * p_error );

#endif
