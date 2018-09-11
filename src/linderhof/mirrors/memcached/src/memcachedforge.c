#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "strix.h"
#include "memcachedforge.h"


Packet * ForgeMemcachedPacket( void *p_arg )
{
  int *opcode = (int *)p_arg;
  Packet *pac = NULL;
  char * mem_packet;
  MemcachedRequestHeader * mem_header = NULL; 
  MemcachedSetExtra * extra = NULL;
  uint32_t * key = NULL;
  int packetSize = 0;

  memalloc( (void *)&pac, sizeof(Packet) );

  switch( *opcode ){
    case MEMCACHED_GET :
      packetSize = sizeof( MemcachedRequestHeader ) + MAXSIZE_MEMCACHED_KEY;
      memalloc( (void *)&mem_packet, packetSize );
  
      mem_header = ( MemcachedRequestHeader *) mem_packet;

      key = ( uint32_t * ) (mem_packet + sizeof( MemcachedRequestHeader ));

      mem_header->magic = 0x80;
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
      packetSize = sizeof( MemcachedRequestHeader ) + sizeof( MemcachedSetExtra ) + MAXSIZE_MEMCACHED_KEY + MAXSIZE_MEMCACHED_VALUE;
      memalloc( (void *)&mem_packet, packetSize );
  
      mem_header = ( MemcachedRequestHeader *) mem_packet;
      extra = ( MemcachedSetExtra * ) (mem_packet + sizeof( MemcachedRequestHeader ));

      key = ( uint32_t * ) (mem_packet + sizeof( MemcachedRequestHeader ) + sizeof( MemcachedSetExtra ));
      char * value = (mem_packet + sizeof( MemcachedRequestHeader ) + sizeof( MemcachedSetExtra ) + MAXSIZE_MEMCACHED_KEY);

      mem_header->magic = 0x80;
      mem_header->opcode = 0x01;
      mem_header->key_length = ntohs(MAXSIZE_MEMCACHED_KEY);
      mem_header->extras_length = sizeof( MemcachedSetExtra );
      mem_header->data_type = 0x00;
      mem_header->vbucket_id = 0x0000;
      mem_header->tot_length = ntohl(sizeof( MemcachedSetExtra ) + MAXSIZE_MEMCACHED_KEY + MAXSIZE_MEMCACHED_VALUE);
      mem_header->opaque = 0x00000000;
      mem_header->cas = 0x0000000000000000;

      extra->flags = ntohl(0xdeadbeef);
      extra->expiration = ntohl(0x151800);
      
      *key =  ntohl(MEMCACHED_KEY);
      memset(value, '-', MAXSIZE_MEMCACHED_VALUE);

      break;
  }
  
  pac->packet_ptr = mem_packet;
  pac->pkt_size = packetSize;

  return pac;
}

Packet * ForgeMemcachedCommand(void *p_arg)
{
  int *opcode = (int*) p_arg;
  char *cmd = NULL;
  Packet *pac = NULL;
  MemcachedSetCmd *cmdSet = NULL;

  memalloc( (void *)&pac, sizeof(Packet) );

  switch( *opcode ){
    case MEMCACHED_GET :
      
      memalloc( (void *)&cmd, MAXSIZE_MEMCACHED_KEY + 50 );
      pac->pkt_size = sprintf(cmd, "get %s\r\nEND\r\n", MEMCACHED_KEYSTR);
      pac->packet_ptr = cmd;

      break;

    case MEMCACHED_SET :
      
      memalloc( (void *)&cmdSet, sizeof(MemcachedSetCmd) );
      memalloc( (void *)&cmdSet->data, MAXSIZE_MEMCACHED_VALUE );

      strcpy(cmdSet->cmd, "set");
      strcpy(cmdSet->key, MEMCACHED_KEYSTR);
      cmdSet->flags = 0;
      cmdSet->exptime = 60*60*24;
      cmdSet->dataLen = MAXSIZE_MEMCACHED_VALUE;
      memset(cmdSet->data, '-', MAXSIZE_MEMCACHED_VALUE);
      cmdSet->cas = 0;

      memalloc( (void *)&cmd, sizeof(MemcachedSetCmd) + MAXSIZE_MEMCACHED_VALUE + 50 );
      pac->pkt_size = sprintf(cmd, "%s %s %hu %u %u\r\n%s\r\n", cmdSet->cmd, cmdSet->key, cmdSet->flags, cmdSet->exptime, cmdSet->dataLen, cmdSet->data);
      pac->packet_ptr = cmd;
      
      free(cmdSet->data);
      free(cmdSet);

      break;

    case MEMCACHED_STAT :
    default :
      pac->pkt_size = sprintf(pac->packet_ptr, " stats\r\n");
      break;
  }

  return pac;
}
  
