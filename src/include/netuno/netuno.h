
#ifndef NETUNO_H
  #define NETUNO_H

#define NETUNO_MAXTHP 50

typedef enum { FULL, INCREMENT }InjectorType;

void StartNetunoInjector( Packet *p_pkt, unsigned int p_inithp, unsigned int p_timer, InjectorType p_type );

#endif
