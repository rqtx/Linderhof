
#ifndef CONFIGR_H
  #define CONFIGR_H

#include "venus.h"

typedef struct {
  char * serverIP;
  unsigned int port;
  float initialThp;
  bool incThp;
  unsigned int timer;
}ConfigOpt;

char * GetServerIP();
int GetServerPort();
void SetDraftConfig( LhfDraft *p_draft );

#endif
