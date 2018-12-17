#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "common/common.h"

#define GetIpHeader(pointer)    ( (struct ip *) (pointer) )
#define GetUdpHeader(pointer)   ( (struct udphdr *) (pointer + IP_HEADER_SIZE) )
#define GetPayload(pointer)     ( pointer + IP_HEADER_SIZE + UDP_HEADER_SIZE )

static uint16_t ip_checksum(const void *buf, size_t hdr_len)
{
  unsigned long sum = 0;
  const uint16_t *ip1;
 
  ip1 = buf;
  while (hdr_len > 1){
    sum += *ip1++;
    if (sum & 0x80000000)
    sum = (sum & 0xFFFF) + (sum >> 16);
    hdr_len -= 2;
  }
 
  while (sum >> 16){
    sum = (sum & 0xFFFF) + (sum >> 16);
  }

  return(~sum);
}

Packet * ForgeUDP(char * ip_dest, char * ip_src, int dest_port, Packet * (*f_payload)(void * arg), void *p_arg)
{
    char * datagram;
    Packet *firstPkt = NULL;
    Packet *lastPkt = NULL;
    struct ip *ip_header = NULL;          // Pointer to the beginning of ip header
    struct udphdr *udp_header = NULL;        // Pointer to the beginning of udp header
    char *payload_ptr = NULL;               // Pointer to the beginning of payload
    
    if(NULL == f_payload) 
    {
        Efatal(ERROR_BLACKSMITH, "Internal error");
    } 
    
    for(Packet *payload = f_payload(p_arg); payload != NULL; payload = payload->next)
    {
        Packet *tmp = CreateEmptyPacket();
        tmp->next = NULL;
        tmp->saddr.sin_family = AF_INET;
        tmp->saddr.sin_port = htons(dest_port);
        tmp->saddr.sin_addr.s_addr = inet_addr(ip_dest);
        
        datagram = NULL;
        memalloc( &datagram, IP_HEADER_SIZE + UDP_HEADER_SIZE + payload->pkt_size );

        ip_header = GetIpHeader(datagram);                                  // Pointer to the beginning of ip header
        udp_header = GetUdpHeader(datagram);   // Pointer to the beginning of udp header
        payload_ptr = GetPayload(datagram);               // Pointer to the beginning of payload

    

        //IP Header
        ip_header->ip_v = 4;
        ip_header->ip_hl = 5;
        ip_header->ip_len = IP_HEADER_SIZE + UDP_HEADER_SIZE + payload->pkt_size;
        ip_header->ip_tos = 0;
        ip_header->ip_off = 0;		    // no fragment = 0
        ip_header->ip_ttl = 64;			    // default value = 64
        ip_header->ip_p = IPPROTO_UDP;
        ip_header->ip_sum = ip_checksum(datagram, ip_header->ip_len);
        inet_pton(AF_INET, ip_src, (struct in_addr *)&ip_header->ip_src);
        inet_pton(AF_INET, ip_dest, (struct in_addr *)&ip_header->ip_dst);

        //UDP header
        udp_header->source = htons(tmp->saddr.sin_port);
        udp_header->dest = htons(dest_port);
        udp_header->len = htons(UDP_HEADER_SIZE + payload->pkt_size);
        udp_header->check = 0;
  
        //Payload
        memcpy(payload_ptr, payload->packet_ptr , payload->pkt_size);
    
        tmp->type = RAW;
        tmp->packet_ptr = datagram;
        tmp->pkt_size = ip_header->ip_len;
        tmp->payload_size = payload->pkt_size;
        strcpy( tmp->ip_dest, ip_dest);
        tmp->dest_port = dest_port;
        
        if( NULL == firstPkt )
        {
            firstPkt = tmp;
            lastPkt = tmp;
        }
        else
        {
            lastPkt->next = tmp;
            lastPkt = tmp;
        }
    }

    return firstPkt;
}

Packet * ForgeTCP(char * ip_dest, int dest_port, Packet * (*f_payload)(void * arg), void *p_arg)
{
    Packet *firstPkt = NULL;
    Packet *lastPkt = NULL;
    char *payload_ptr = NULL;               // Pointer to the beginning of payload
    
    if(NULL == f_payload) 
    {
        Efatal(ERROR_BLACKSMITH, "Internal error");
    } 
    
    for(Packet *payload = f_payload(p_arg); payload != NULL; payload = payload->next)
    {
        Packet *tmp = CreateEmptyPacket();
        tmp->next = NULL;
        tmp->saddr.sin_family = AF_INET;
        tmp->saddr.sin_port = htons(dest_port);
        tmp->saddr.sin_addr.s_addr = inet_addr(ip_dest);
        
        memalloc( &payload_ptr, payload->pkt_size );
        memcpy(payload_ptr, payload->packet_ptr , payload->pkt_size);
    
        tmp->type = TCP;
        tmp->packet_ptr = payload_ptr;
        tmp->pkt_size = payload->pkt_size;
        tmp->payload_size = payload->pkt_size;
        strcpy( tmp->ip_dest, ip_dest);
        tmp->dest_port = dest_port;
        
        if( NULL == firstPkt )
        {
            firstPkt = tmp;
            lastPkt = tmp;
        }
        else
        {
            lastPkt->next = tmp;
            lastPkt = tmp;
        }
    }

    return firstPkt;
}
