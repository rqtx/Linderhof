/**
 *  netio.h
 *
 *  Network API
 * */
#ifndef NETIO_H
  #define NETIO_H

#include <arpa/inet.h>  //strcut sockaddr
#include <netinet/in.h> //struct sockaddr_in
#include "common/common.h"

#define IP_HEADER_SIZE  sizeof(struct ip)
#define UDP_HEADER_SIZE sizeof(struct udphdr)

#define BLOCK true
#define NO_BLOCK false
#define  SOCKADDR_SIZE sizeof(struct sockaddr)

typedef enum { EMPTY = 1, RAW, UDP, TCP, LHF } NetType;

typedef struct Packet{
  NetType type;
  void *packet_ptr;
  size_t pkt_size;
  size_t payload_size;
  char ip_dest[16];
  int dest_port;
  struct sockaddr_in saddr;
  struct Packet *next;
}Packet;

/**
 *  @brief Create a BLOCK or NO_BLOCK socket
 *  @param p_type[in] Socket type (Enum NetType)
 *  @param p_blk[in] set BLOCK or NO_BLOCK socket mode
 * */
int CreateSocket( NetType p_type, bool p_blk);

/**
 *  @brief Close socket
 * */
void CloseSocket( int fd);

/**
 *  @brief Set socket to BLOCK or NO_BLOCK mode
 *
 *  @param p_fd[in] Socket file descriptor
 *  @param blocking BLOCK or NO_BLOCK socket mode
 * */
int BlockSocket(int p_fd, bool blocking);

/**
 *  @brief Set socket flags
 *
 *  @param p_socket[in] Socket file descriptor
 *  @param p_flags Flags to be set
 * */
int SetSocketFlag( int p_socket, int p_flags );

/**
 *  @brief Create an empty struct Packet
 *
 * */
Packet * CreateEmptyPacket( );

/**
 *  @brief Relese packet memory
 *
 *  @param p_pkt[in] Packet to be released
 * */
void ReleasePacket( Packet *p_pkt );

/**
 *  @brief Set Packet.data as descibred on Packet.saddr
 *
 *  @param p_pkt[in] Packet to be sent
 * */
int SendPacket( int p_socket, Packet *p_pkt );

int BindPort( int p_socket, struct sockaddr_in saddr );

/**
 *  @brief Verify if string is a valid IPv4
 *
 *  @param p_str[in] String to be checked
 * */
bool is_valid_ipv4(char * ip_str);

/**
 * @brief Set maximum socket send buffer in bytes
 *
 * @param p_str[in] File descriptor 
 * @param p_str[in] Minimal buffer size 
 */
int Setup_sendbuffer ( int p_fd, uint32_t p_n );

void ConnectTCP(int p_socket, Packet *p_pkt);

#endif      //NETIO_H
