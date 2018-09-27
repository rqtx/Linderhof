
#include "venus.h"
#include "netuno/logger.h"

#include <time.h>

char * getCurrentTimeStr()
{
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    return asctime (timeinfo);
}

FILE * CreateLoggerFile( LoggerType p_type )
{
    FILE *fp = NULL;

    switch( (int)p_type )
    {
        case ATK_LOGGER:
            fp = fopen("atklogger.txt", "w");
            break;
        case MONITOR_LOGGER:
            fp = fopen("monitorlogger.txt", "w");
            break;
    }
    return fp;
}

void LogInjection( FILE * p_fp, float p_thpExp, float p_thpCur )
{
    FILE *fp = ( p_fp == NULL ) ? stdout : p_fp; 
    char *curTime = getCurrentTimeStr();
    fprintf( fp, "%s %f %f\n ", curTime, p_thpExp ,p_thpCur);
}

void LogAttack( FILE *p_fp, float p_thp )
{
    FILE *fp = ( p_fp == NULL ) ? stdout : p_fp; 
    char *curTime = getCurrentTimeStr();
    fprintf( fp, "%s %f\n", curTime, p_thp );
}
