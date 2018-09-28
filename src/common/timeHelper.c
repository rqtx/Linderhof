#include <time.h>

static struct timespec _onesec = {
               .tv_sec = 1,                     /* 1 seconds */
               .tv_nsec = 0       /* 0 nanoseconds */
          };

static struct timespec _onemin = {
               .tv_sec = 60,                     /* 1 seconds */
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
