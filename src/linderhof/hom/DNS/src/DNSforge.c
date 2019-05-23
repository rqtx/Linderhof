#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "strix.h"
#include "DNSforge.h"

// Packet * ForgeDnsBinary( void *p_arg, char * domain)
// {
// 	Packet *pac = NULL;
// 	char *name, *buf;
//     struct DNS_H *dns = NULL;
//     struct QUESTION *info = NULL;
//     dns = (struct DNS_H *) buf;
//     int packetSize = 0;

//     memalloc( (void *)&pac, sizeof(Packet) );

    
//     dns->id = (unsigned short) htons(getpid());

//     dns->qr = 0; 
//     dns->opcode = 0; 
//     dns->aa = 0; 
//     dns->tc = 0; 
//     dns->rd = 1;
//     dns->ra = 0;
//     dns->z = 0;
//     dns->ad = 0;
//     dns->cd = 0;
//     dns->rcode = 0;

//     dns->q_count = 1; 
//     dns->ans_count = 0;
//     dns->auth_count = 0;
//     dns->add_count = 0;

//     name =(unsigned char*)&buf[sizeof(struct DNS_H)];
//     int lock = 0 , i;
//     strcat((char*)domain,".");
     
//     for(i = 0 ; i < strlen((char*)domain) ; i++) 
//     {
//         if(domain[i]=='.') 
//         {
//             *name++ = i-lock;
//             for(;lock<i;lock++) 
//             {
//                 *name++=domain[lock];
//             }
//             lock++;
//         }
//     }
//     *name++='\0';

//     info =(struct QUESTION*)&buf[sizeof(struct DNS_H) + (strlen((const char*)name) + 1)]; 
//     info->qtype = htons(1); 
//     info->qclass = htons(1);

//     packetSize = sizeof(info);
//     pac->packet_ptr = info;
//     pac->pkt_size = packetSize;

// 	return pac;
// }

Packet * ForgeDNS( void *p_arg ){
	
	Packet *pac = NULL;
	char * dns_packet;
	DNSheader * dns = NULL;
	int packetSize = 0;

	memalloc( (void *)&pac, sizeof(Packet) );

    packetSize = sizeof( DNSheader );
    memalloc( (void *)&dns_packet, packetSize );

    dns = ( DNSheader *) dns_packet;

    dns->id = (unsigned short) htons(getpid());
    dns->rd = 1; 
    dns->tc = 0; 
    dns->aa = 0; 

    dns->opcode = 0; 
    dns->qr = 0; 
    dns->rcode = 0;
    dns->cd = 0;
    dns->ad = 1;
    dns->z = 0;
    dns->ra = 0;
    dns->q_count = htons(1); 
    dns->ans_count = 0;
    dns->auth_count = 0;
    dns->add_count = htons(1);

    //dns->data == 'unb.br';

	pac->packet_ptr = dns_packet;
	pac->pkt_size = packetSize;

	return pac;
}