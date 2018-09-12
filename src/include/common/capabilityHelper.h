#ifndef CAPABILITYHELPER_H
  #define CAPABILITYHELPER_H

//use -lcap to compile
#include <sys/capability.h>
void SetCapability( cap_value_t p_cap );

#endif
