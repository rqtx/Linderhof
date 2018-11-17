/*
 *  error.h
 *  Error handler
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
#define ERROR_MEMCACHED -400
#define ERROR_ORYX      -500
#define ERROR_CLI       -600
#define ERROR_PLANNER   -700 

/**
 *  @brief Display error message
 *
 *  @param p_errorCode[in] Error code
 *  @param p_msg[in] Message to display
 * */
int Elog( int p_errorCode, char * p_msg );
#define ELOG( p_errorCode, p_msg ) return Elog( p_errorCode, p_msg )
#define LOG(p_msg) Elog( SUCCESS, p_msg )

/**
 * @brief Set error funcion handler on error fatal
 *
 * @param p_func[in] Function that will be used on fatal error
 * */
void ESetErroAction( void p_func( int ) );

/**
 *  @brief Fire fatal error
 *
 *  @param p_errorCode[in] Error code
 *  @param p_error[in] Error message
 * */
void Efatal( int p_errorCode, char * p_error );

#endif
