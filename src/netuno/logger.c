
#include "venus.h"
#include "netuno/netuno.h"
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

void LogInjection( FILE * p_fp, float p_thpExp, float p_thpCur, unsigned int p_probes )
{
    FILE *fp = ( p_fp == NULL ) ? stdout : p_fp; 
    char *curTime = GetCurrentTimeStr();
    fprintf( fp, "\n%s\n ", curTime);
    fprintf( fp, "Thp Expected: %f Mb/s\n", p_thpExp);
    fprintf( fp, "Thp Provided: %f Mb/s\n", p_thpCur/MEGABYTE);
    fprintf( fp, "Probes: %u/s\n", p_probes);
}

void LogAttack( FILE *p_fp, float p_thp )
{
    FILE *fp = ( p_fp == NULL ) ? stdout : p_fp; 
    char *curTime = GetCurrentTimeStr();
    fprintf( fp, "%s %f\n", curTime, p_thp );
}
