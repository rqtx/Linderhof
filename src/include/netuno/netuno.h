
#ifndef NETUNO_H
  #define NETUNO_H

#define NETUNO_MAXTHP 100

/**
 * @brief Start Netuno injector.
 *
 * @param p_pkt[in] Packet data to inject.
 * @param p_thp[in] Initial throughput. (Just used on FULL attack)
 * @param p_timer[in] Attack execution time.
 * @return Injector handler strcut 
 */
void StartNetunoInjector( Packet *p_pkt, unsigned int p_inithp, unsigned int p_timer );

#endif
