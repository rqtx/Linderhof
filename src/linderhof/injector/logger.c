
#include "venus.h"
#include "injector/injector.h"

#include <time.h>

char * getCurrentTimeStr()
{
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    return asctime (timeinfo);
}

FILE * CreateLoggerFile()
{
    FILE * fp;
    
    fp = fopen("logger.txt", "w");
    return fp;
}

void LogInjection( FILE * p_fp, float p_thpExp, float p_thpCur )
{
    char *curTime = getCurrentTimeStr();
    fprintf( p_fp, "%s %f Mb/s %f Mb/s\n ", curTime, p_thpExp ,p_thpCur);
}

void LogAttack( FILE *p_fp, float p_thp )
{
    char *curTime = getCurrentTimeStr();
    fprintf( p_fp, "%s %f Mb/s\n", curTime, p_thp );
}
