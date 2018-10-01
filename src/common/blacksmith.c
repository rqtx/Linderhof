#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "common/common.h"

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
  
    int payload_size = 0;
    char * datagram;
    Packet *pac = CreateEmptyPacket();
    Packet *payloadPkg = NULL;
    void *payload = NULL;
    struct iphdr *ip_header = NULL;          // Pointer to the beginning of ip header
    struct udphdr *udp_header = NULL;        // Pointer to the beginning of udp header
    char *payload_ptr = NULL;               // Pointer to the beginning of payload
    
    if(NULL != f_payload)
    {
        payloadPkg = f_payload(p_arg);
        payload_size = payloadPkg->pkt_size;
        payload = payloadPkg->packet_ptr;
    }
    else
    {
        payload_size = strlen(p_arg);
        payload = p_arg;
    }
    
    pac->netSock = CreateSocket( RAW, NO_BLOCK );
    pac->saddr.sin_family = AF_INET;
    pac->saddr.sin_port = 0;
    pac->saddr.sin_addr.s_addr = INADDR_ANY;
    SetPacketPort(pac);

    memalloc( &datagram, sizeof(struct iphdr) + sizeof(struct udphdr) + payload_size );

    ip_header = (struct iphdr *)datagram;                                  // Pointer to the beginning of ip header
    udp_header = (struct udphdr *)(datagram + sizeof(struct iphdr));   // Pointer to the beginning of udp header
    payload_ptr = (datagram + sizeof(struct iphdr) + sizeof(struct udphdr));               // Pointer to the beginning of payload

    

    //IP Header
    ip_header->version = 4;
    ip_header->ihl = 5;
    ip_header->tot_len = sizeof (struct iphdr) + sizeof (struct udphdr) + payload_size;
    ip_header->tos = 0;
    ip_header->frag_off = 0;		    // no fragment = 0
    ip_header->ttl = 64;			    // default value = 64
    ip_header->protocol = IPPROTO_UDP;
    ip_header->check = ip_checksum(datagram, ip_header->tot_len);
    inet_pton(AF_INET, ip_dest, (struct in_addr *)&ip_header->daddr);
    inet_pton(AF_INET, ip_src, (struct in_addr *)&ip_header->saddr);

    //UDP header
    udp_header->source = htons(pac->saddr.sin_port);
    udp_header->dest = htons(dest_port);
    udp_header->len = htons(sizeof(struct udphdr) + payload_size);
    udp_header->check = 0;
  
    //Payload
    memcpy(payload_ptr, payload , payload_size);
    
    pac->type = RAW;
    pac->packet_ptr = datagram;
    pac->pkt_size = ip_header->tot_len;
    pac->payload_size = payload_size;
    strcpy( pac->ip_dest, ip_dest);
    pac->dest_port = dest_port;
    

    return pac;
}
