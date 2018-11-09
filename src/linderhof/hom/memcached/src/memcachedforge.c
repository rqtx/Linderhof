#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "strix.h"
#include "memcachedforge.h"

#define SET_CMD "set %s 0 0 %d\r\n%s\r\n"
#define GET_CMD "get %s\r\n"

typedef enum {
    PROTOCOL_BINARY_REQ = 0x80,
    PROTOCOL_BINARY_RES = 0x81
} protocol_binary_magic;

Packet * ForgeMemcachedBinary( void *p_arg )
{
  int *opcode = (int *)p_arg;
  Packet *pac = NULL;
  char * mem_packet;
  BinaryRequestHeader * mem_header = NULL; 
  BinarySetExtra * extra = NULL;
  uint32_t * key = NULL;
  int packetSize = 0;

  memalloc( (void *)&pac, sizeof(Packet) );

  switch( *opcode ){
    case MEMCACHED_GET :
      packetSize = sizeof( BinaryRequestHeader ) + MAXSIZE_MEMCACHED_KEY;
      memalloc( (void *)&mem_packet, packetSize );
  
      mem_header = ( BinaryRequestHeader *) mem_packet;

      key = ( uint32_t * ) (mem_packet + sizeof( BinaryRequestHeader ));

      mem_header->magic = PROTOCOL_BINARY_REQ;
      mem_header->opcode = 0x00;
      mem_header->key_length = ntohs(MAXSIZE_MEMCACHED_KEY);
      mem_header->extras_length = 0x00;
      mem_header->data_type = 0x00;
      mem_header->vbucket_id = 0x0000;
      mem_header->tot_length = ntohl(MAXSIZE_MEMCACHED_KEY);
      mem_header->opaque = 0x00000000;
      mem_header->cas = 0x0000000000000000;

      *key =  ntohl(MEMCACHED_KEY);
      break;

    case MEMCACHED_SET :
      packetSize = sizeof( BinaryRequestHeader ) + sizeof( BinarySetExtra ) + MAXSIZE_MEMCACHED_KEY + MAXSIZE_MEMCACHED_VALUE;
      memalloc( (void *)&mem_packet, packetSize );
  
      mem_header = ( BinaryRequestHeader *) mem_packet;
      extra = ( BinarySetExtra * ) (mem_packet + sizeof( BinaryRequestHeader ));

      key = ( uint32_t * ) (mem_packet + sizeof( BinaryRequestHeader ) + sizeof( BinarySetExtra ));
      char * value = (mem_packet + sizeof( BinaryRequestHeader ) + sizeof( BinarySetExtra ) + MAXSIZE_MEMCACHED_KEY);

      mem_header->magic = PROTOCOL_BINARY_REQ ;
      mem_header->opcode = 0x01;
      mem_header->key_length = ntohs(MAXSIZE_MEMCACHED_KEY);
      mem_header->extras_length = sizeof( BinarySetExtra );
      mem_header->data_type = 0x00;
      mem_header->vbucket_id = 0x0000;
      mem_header->tot_length = ntohl(sizeof( BinarySetExtra ) + MAXSIZE_MEMCACHED_KEY + MAXSIZE_MEMCACHED_VALUE);
      mem_header->opaque = 0x00000000;
      mem_header->cas = 0x0000000000000000;

      extra->flags = ntohl(0xdeadbeef);
      extra->expiration = ntohl(0x151800);
      
      *key =  ntohl(MEMCACHED_KEY);
      memset(value, '-', MAXSIZE_MEMCACHED_VALUE);
      break;

    case MEMCACHED_STAT :
    default :
        packetSize = sizeof( BinaryRequestHeader );
        memalloc( (void *)&mem_packet, packetSize );
  
        mem_header = ( BinaryRequestHeader *) mem_packet;

        mem_header->magic = PROTOCOL_BINARY_REQ;
        mem_header->opcode = 0x10;
        mem_header->key_length = 0x0000;
        mem_header->extras_length = 0x00;
        mem_header->data_type = 0x00;
        mem_header->vbucket_id = 0x0000;
        mem_header->tot_length = 0x00000000;
        mem_header->opaque = 0x00000000;
        mem_header->cas = 0x0000000000000000;

        break;
  }
  
  pac->packet_ptr = mem_packet;
  pac->pkt_size = packetSize;

  return pac;
}

Packet * ForgeMemcachedText(void *p_arg)
{
    int *opcode = (int*) p_arg;
    char *dataBuffer = NULL;
    char *cmd = NULL;
    Packet *pac = NULL;
    TextProtocolHeader *header = NULL;
    char *payload = NULL;

    memalloc( (void *)&pac, sizeof(Packet) );

    switch( *opcode ){
        case MEMCACHED_GET :
            pac->pkt_size = sizeof(TextProtocolHeader) + MAXSIZE_MEMCACHED_KEY + strlen(GET_CMD);
            memalloc( (void *)&dataBuffer, pac->pkt_size );
            header = (TextProtocolHeader *)dataBuffer;
            cmd = dataBuffer + sizeof(TextProtocolHeader);

            header->requestID = 0;
            header->seqNumber = 0;
            header->nDatagrams = htons(0x01);
            header->reserved = 0;

            sprintf(cmd, GET_CMD, MEMCACHED_KEYSTR);
            pac->packet_ptr = dataBuffer;
            break;

        case MEMCACHED_SET :
            pac->pkt_size = sizeof(TextProtocolHeader) + MAXSIZE_MEMCACHED_KEY + DATALEN_SIZE + MAXSIZE_MEMCACHED_VALUE + strlen(SET_CMD);
            memalloc(&dataBuffer, pac->pkt_size);
            header = (TextProtocolHeader *)dataBuffer;
            char *cmd = dataBuffer + sizeof(TextProtocolHeader);
            char *data_block = NULL;

            header->requestID = 0;
            header->seqNumber = 0;
            header->nDatagrams = htons(0x01);
            header->reserved = 0;
            
            memalloc(&data_block, MAXSIZE_MEMCACHED_VALUE);
            memset(data_block, '-', MAXSIZE_MEMCACHED_VALUE);
            sprintf(cmd, SET_CMD, MEMCACHED_KEYSTR, MAXSIZE_MEMCACHED_VALUE, data_block);
            pac->packet_ptr = dataBuffer;
            memfree(&data_block);
            break;

        case MEMCACHED_STAT :
        default :
            pac->pkt_size = sizeof(TextProtocolHeader) + strlen("stats\r\n");
            memalloc(&cmd, pac->pkt_size);
            header = (TextProtocolHeader *)cmd;
            payload = cmd + sizeof(TextProtocolHeader);

            header->requestID = 0;
            header->seqNumber = 0;
            header->nDatagrams = htons(0x01);
            header->reserved = 0;

            memcpy(payload, "stats\r\n", strlen("stats\r\n"));

            pac->packet_ptr = cmd;
            break;
  }

  return pac;
}
  
