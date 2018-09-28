
#ifndef CONFIGR_H
  #define CONFIGR_H

#include "venus.h"

typedef struct {
  char * serverIP;
  unsigned int port;
}ConfigOpt;

void SetServerIP( char * p_ip );
char * GetServerIP();
int GetServerPort();
char * AskServerIP();

#endif
