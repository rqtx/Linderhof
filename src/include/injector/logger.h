
#ifndef LOGGER_H
  #define LOGGER_H

FILE * CreateLoggerFile();
void LogInjection( FILE * p_fp, float p_thpExp, float p_thpCur );
void LogAttack( FILE *p_fp, float p_thp );

#endif
