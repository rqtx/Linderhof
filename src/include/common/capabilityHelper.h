/**
 * capabilityHelper.h
 *
 * Helper to Linux capability
 * */

#ifndef CAPABILITYHELPER_H
  #define CAPABILITYHELPER_H

#include <sys/capability.h> //use -lcap to compile

/**
 *  @brief Set Linux Capability
 *
 *  @param p_cap[in] Capability to be set
 * */
void SetCapability( cap_value_t p_cap );

#endif
