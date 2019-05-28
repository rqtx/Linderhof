#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "strix.h"
#include "DNSforge.h"

char dominio[] = "unb.br";

Packet * ForgeDNS(void *p_arg ){
	
    Packet *pac = NULL;
    DNSheader * dns = NULL;
    QUESTION * ques = NULL;
    char * dns_packet = NULL;
    unsigned char * strDomain;
    unsigned char * name;
    int packetSize = 0;

    strDomain = (unsigned char *) &dominio;

    memalloc( (void *)&pac, sizeof(Packet) );
    packetSize = sizeof( DNSheader ) + strlen((const char*)strDomain) + 1 + sizeof(QUESTION);
    memalloc( (void *)&dns_packet, packetSize );
    
    dns = ( DNSheader *) dns_packet;
    name = (unsigned char *)(dns_packet + sizeof(DNSheader));
    ques = ( QUESTION * )(dns_packet + sizeof(DNSheader) + (strlen((const char*)name) + 1)); 

    dns->id = (unsigned short) htons(getpid());
    dns->qr = 0; //This is a query
    dns->opcode = 0; //This is a standard query
    dns->aa = 0; //Not Authoritative
    dns->tc = 0; //This message is not truncated
    dns->rd = 1; //Recursion Desired
    dns->ra = 0; //Recursion not available! hey we dont have it (lol)
    dns->z = 0;
    dns->ad = 1;
    dns->cd = 0;
    dns->rcode = 0;

    dns->q_count = htons(1); //we have only 1 question
    dns->ans_count = 0;
    dns->auth_count = 0;
    dns->add_count = htons(1);

    ChangetoDnsNameFormat(name , strDomain);

    ques->qtype = htons(255);
    ques->qclass = htons(1);

	pac->packet_ptr = dns_packet;
	pac->pkt_size = packetSize;

	return pac;
}

void ChangetoDnsNameFormat(unsigned char* name, unsigned char * strDomain){
    int lock = 0 , i;
    strcat((char*)strDomain,".");

    for(i = 0 ; i < strlen((char*)strDomain) ; i++){
        if(strDomain[i]=='.'){
            *name++ = i-lock;
            for(;lock<i;lock++){
                *name++=strDomain[lock];
            }
            lock++; //or lock=i+1;
        }
    }
    *name++='\0';
}