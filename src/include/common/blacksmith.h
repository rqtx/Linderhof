
#ifndef BLACKSMITH_H
  #define BLACKSMITH_H

Packet * ForgeUDP(char * ip_dest, char * ip_src, int dest_port, Packet * (*f_payload)(void * arg), void *p_arg);

#endif
