#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "strix.h"
#include "NTPforge.h"

#define SET_CMD "set %s 0 0 %d\r\n%s\r\n"
#define GET_CMD "get %s\r\n"

typedef enum {
   PROTOCOL_BINARY_REQ = 0x80,
   PROTOCOL_BINARY_RES = 0x81
} protocol_binary_magic;

Packet * ForgeNtpBinary( void *p_arg )
{
 int *opcode = (int *)p_arg;
 Packet *pac = NULL;
 char * ntp_packet;
 NtpBinaryRequestHeader * ntp_header = NULL;
 // uint32_t * key = NULL;
 int packetSize = 0;

 memalloc( (void *)&pac, sizeof(Packet) );

 switch( *opcode ){
   case NTP :
     packetSize = sizeof( NtpBinaryRequestHeader );
     memalloc( (void *)&ntp_packet, packetSize );

     ntp_header = ( NtpBinaryRequestHeader *) ntp_packet;

     // key = ( uint32_t * ) (ntp_packet + sizeof( NtpBinaryRequestHeader ));

     ntp_header->li_vn_mode = 0x00;
     ntp_header->stratum = 0x00;
     ntp_header->poll = 0x00;
     ntp_header->precision = 0x00;
     ntp_header->rootDelay = 0x00;
     ntp_header->rootDispersion = 0x00;
     ntp_header->refId = 0x00;
     ntp_header->refTm_s = 0x00;
     ntp_header->refTm_f = 0x00;
     ntp_header->origTm_s = 0x00;
     ntp_header->origTm_f = 0x00;
     ntp_header->rxTm_s = 0x00;
     ntp_header->rxTm_f = 0x00;
     ntp_header->txTm_s = 0x00;
     ntp_header->txTm_f = 0x00;

     // *key =  ntohl(MEMCACHED_KEY);
     break;

 }

 pac->packet_ptr = ntp_packet;
 pac->pkt_size = packetSize;

 return pac;
}


Packet * ForgeNtpText(void *p_arg)
{
    int *opcode = (int*) p_arg;
    char *dataBuffer = NULL;
    char *cmd = NULL;
    Packet *pac = NULL;
    TextProtocolHeader *header = NULL;
    // char *payload = NULL;

    memalloc( (void *)&pac, sizeof(Packet) );

    switch( *opcode ){

        case NTP : 
        default :
            pac->pkt_size = sizeof(TextProtocolHeader) + MAXSIZE_MEMCACHED_KEY + strlen(GET_CMD);
            memalloc( (void *)&dataBuffer, pac->pkt_size );
            header = (TextProtocolHeader *)dataBuffer;
            cmd = dataBuffer + sizeof(TextProtocolHeader);

            header->requestID = 0;
            header->seqNumber = 0;
            header->nDatagrams = htons(0x01);
            header->reserved = 0;

            sprintf(cmd, GET_CMD, MEMCACHED_KEY);
            pac->packet_ptr = dataBuffer;
            break;
  }

  return pac;
}