
#ifndef NETIO_H
  #define NETIO_H

#include <arpa/inet.h>  //strcut sockaddr
#include <netinet/in.h> //struct sockaddr_in
#include "common/common.h"

#define BLOCK true
#define NO_BLOCK false

typedef enum { Empty = 1, LHF, UDP, TCP } NetType;

typedef struct  PacketData {
  NetType type;
  void *packet_ptr;
  size_t pkt_size;
  char ip_dest[16];
  int dest_port;
  struct sockaddr_in saddr;
}Packet;

int CreateSocket( NetType p_type, bool p_blk);
void CloseSocket( int fd);
int SetSocketFlag( int p_socket, int p_flags );
int BlockSocket(int p_fd, bool blocking);

Packet * CreateEmptyPacket( );
void ReleasePacket( Packet *p_pkt );
int SendPacket( int p_socket, Packet *p_pkt );


bool is_valid_ipv4(char * ip_str);
#endif      //NETIO_H
