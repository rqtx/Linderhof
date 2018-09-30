
#ifndef NETUNO_H
  #define NETUNO_H

#define NETUNO_MAXTHP 50

/**
 * FULL - static throughput attack
 * INCREMENT - increment attack starting in 1 Mb/s
 * */
typedef enum { FULL, INCREMENT }InjectorType;

/**
 * @brief Start Netuno injector.
 *
 * @param p_pkt[in] Packet data to inject.
 * @param p_thp[in] Initial throughput. (Just used on FULL attack)
 * @param p_timer[in] Attack execution time.
 * @param p_type[in] Attack type (INCREMENT or FULL).
 * @return Injector handler strcut 
 */
void StartNetunoInjector( Packet *p_pkt, unsigned int p_inithp, unsigned int p_timer, InjectorType p_type );

#endif
