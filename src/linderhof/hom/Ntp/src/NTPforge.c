#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "strix.h"
#include "NTPforge.h"

//#define SET_CMD "set %s 0 0 %d\r\n%s\r\n"
//#define GET_CMD "get %s\r\n"
//
//typedef enum {
//    PROTOCOL_BINARY_REQ = 0x80,
//    PROTOCOL_BINARY_RES = 0x81
//} protocol_binary_magic;
//
//Packet * ForgeNtpBinary( void *p_arg )
//{
//  int *opcode = (int *)p_arg;
//  Packet *pac = NULL;
//  char * mem_packet;
//  BinaryRequestHeader * mem_header = NULL;
//  BinarySetExtra * extra = NULL;
//  uint32_t * key = NULL;
//  int packetSize = 0;
//
//  memalloc( (void *)&pac, sizeof(Packet) );
//
//  switch( *opcode ){
//    case MEMCACHED_GET :
//      packetSize = sizeof( BinaryRequestHeader ) + MAXSIZE_MEMCACHED_KEY;
//      memalloc( (void *)&mem_packet, packetSize );
//
//      mem_header = ( BinaryRequestHeader *) mem_packet;
//
//      key = ( uint32_t * ) (mem_packet + sizeof( BinaryRequestHeader ));
//
//      mem_header->magic = PROTOCOL_BINARY_REQ;
//      mem_header->opcode = 0x00;
//      mem_header->key_length = ntohs(MAXSIZE_MEMCACHED_KEY);
//      mem_header->extras_length = 0x00;
//      mem_header->data_type = 0x00;
//      mem_header->vbucket_id = 0x0000;
//      mem_header->tot_length = ntohl(MAXSIZE_MEMCACHED_KEY);
//      mem_header->opaque = 0x00000000;
//      mem_header->cas = 0x0000000000000000;
//
//      *key =  ntohl(MEMCACHED_KEY);
//      break;
//
//    case MEMCACHED_SET :
//      packetSize = sizeof( BinaryRequestHeader ) + sizeof( BinarySetExtra ) + MAXSIZE_MEMCACHED_KEY + MAXSIZE_MEMCACHED_VALUE;
//      memalloc( (void *)&mem_packet, packetSize );
//
//      mem_header = ( BinaryRequestHeader *) mem_packet;
//      extra = ( BinarySetExtra * ) (mem_packet + sizeof( BinaryRequestHeader ));
//
//      key = ( uint32_t * ) (mem_packet + sizeof( BinaryRequestHeader ) + sizeof( BinarySetExtra ));
//      char * value = (mem_packet + sizeof( BinaryRequestHeader ) + sizeof( BinarySetExtra ) + MAXSIZE_MEMCACHED_KEY);
//
//      mem_header->magic = PROTOCOL_BINARY_REQ ;
//      mem_header->opcode = 0x01;
//      mem_header->key_length = ntohs(MAXSIZE_MEMCACHED_KEY);
//      mem_header->extras_length = sizeof( BinarySetExtra );
//      mem_header->data_type = 0x00;
//      mem_header->vbucket_id = 0x0000;
//      mem_header->tot_length = ntohl(sizeof( BinarySetExtra ) + MAXSIZE_MEMCACHED_KEY + MAXSIZE_MEMCACHED_VALUE);
//      mem_header->opaque = 0x00000000;
//      mem_header->cas = 0x0000000000000000;
//
//      extra->flags = ntohl(0xdeadbeef);
//      extra->expiration = ntohl(0x151800);
//
//      *key =  ntohl(MEMCACHED_KEY);
//      memset(value, '-', MAXSIZE_MEMCACHED_VALUE);
//      break;
//  }
//
//  pac->packet_ptr = mem_packet;
//  pac->pkt_size = packetSize;
//
//  return pac;
//}
