/*
 *  error.h
 *
 */

#ifndef ERROR_H
  #define ERROR_H

#include <errno.h>

#define SUCCESS 0
#define ERROR_FATAL     -1

#define ERROR_MEMORY    -10
#define ERROR_NET       -20
#define ERROR_FILE      -30
#define ERROR_THREAD    -40
#define ERROR_SIGNAL    -50

#define ERROR_DRAFT     -100
#define ERROR_MIRROR    -200
#define ERROR_INJECTOR  -300

void Efatal( int p_errorCode, char * p_error );

int Elog( int p_errorCode, char * p_msg );
#define ELOG( p_errorCode, p_msg ) return Elog( p_errorCode, p_msg )
#define LOG(p_msg) Elog( SUCCESS, p_msg )

void plog( char *p_msg );
void ESetErroAction( void p_func( int ) );

#endif
