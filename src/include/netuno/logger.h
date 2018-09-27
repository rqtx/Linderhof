
#ifndef LOGGER_H
  #define LOGGER_H

typedef enum {ATK_LOGGER, MONITOR_LOGGER} LoggerType;
FILE * CreateLoggerFile( LoggerType p_type );
void LogInjection( FILE * p_fp, float p_thpExp, float p_thpCur );
void LogAttack( FILE *p_fp, float p_thp );

#endif
