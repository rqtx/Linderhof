
#ifndef MANAGER_H
  #define MANAGER_H

#include "common/netio.h"

int StrixManager( void * draft, int draftSize);
void * CreateDefaultDraft( char* target_ip, char* amp_ip );
int *GetInjectorsId();

#endif
