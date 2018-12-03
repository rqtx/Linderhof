
#include "venus.h"
#include "netuno/netuno.h"
#include "netuno/logger.h"

#include <math.h>
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

void LogInjection( FILE * p_fp, unsigned int p_level, float p_thpCur, unsigned int p_probes )
{
    FILE *fp = ( p_fp == NULL ) ? stdout : p_fp; 
    char *curTime = GetCurrentTimeStr();
    fprintf( fp, "\n%s\n", curTime);
    fprintf( fp, "Current level:    %u\n", p_level);
    fprintf( fp, "Probes expected:  %.0lf/s\n", ProbesByLevel(p_level));
    fprintf( fp, "Probes provided:  %u/s\n", p_probes);
    //fprintf( fp, "Throughput:       %f Mb/s\n", p_thpCur/MEGABYTE);
}
