
#ifndef CONFIGR_H
  #define CONFIGR_H

#include "venus.h"

typedef struct {
  char * serverIP;
  unsigned int port;
  float initialThp;
  float incThp;
  unsigned int timeFreq;
  unsigned int timer;
}ConfigOpt;

char * GetServerIP();
int GetServerPort();
void SetDraftConfig( LhfDraft *p_draft );

#endif
