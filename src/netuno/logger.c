
#include "venus.h"
#include "netuno/logger.h"

#include <time.h>

FILE * CreateLoggerFile( char *p_file )
{
    FILE *fp = NULL;
    if( p_file != NULL )
    {
        fp = fopen(p_file, "w");
    }
    return fp;
}

void LogInjection( FILE * p_fp, float p_thpExp, float p_thpCur )
{
    FILE *fp = ( p_fp == NULL ) ? stdout : p_fp; 
    char *curTime = GetCurrentTimeStr();
    fprintf( fp, "%s %f %f\n ", curTime, p_thpExp ,p_thpCur);
}

void LogAttack( FILE *p_fp, float p_thp )
{
    FILE *fp = ( p_fp == NULL ) ? stdout : p_fp; 
    char *curTime = GetCurrentTimeStr();
    fprintf( fp, "%s %f\n", curTime, p_thp );
}
