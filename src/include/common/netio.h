
#ifndef NETIO_H
  #define NETIO_H

#include <netinet/in.h> //struct sockaddr_in
#include "common/common.h"

typedef struct  PacketData {
  void *packet_ptr;
  size_t pkt_size;
  char *ip_dest;
  int dest_port;
  struct sockaddr_in *saddr;
}Packet;

int create_socket( void );

void close_socket( int fd);

bool send_packet(int socket, const void *buf, size_t len, struct sockaddr *saddr);

bool send_data(const void *buf, size_t len, struct sockaddr *saddr);

bool is_valid_ipv4(char * ip_str);

void release_packet( Packet *p_pkt );
#endif      //NETIO_H
