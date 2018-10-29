#include <time.h>

static struct timespec _onesec = {
               .tv_sec = 1,                     /* 1 seconds */
               .tv_nsec = 0       /* 0 nanoseconds */
          };

static struct timespec _onemin = {
               .tv_sec = 60,                     /* 60 seconds */
               .tv_nsec = 0       /* 0 nanoseconds */
          };

char * GetCurrentTimeStr()
{
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    return asctime (timeinfo);
}

void SleepOneSec()
{
    nanosleep(&_onesec, NULL);
}

void SleepOneMinute()
{
    nanosleep(&_onemin, NULL);
}

void SleepSec( unsigned int p_sec )
{
    struct timespec timesec;
    timesec.tv_sec = p_sec;
    nanosleep(&timesec, NULL);
}
