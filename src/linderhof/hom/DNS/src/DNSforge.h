#ifndef PACKETFURNACE_H
  #define PACKETFURNACE_H

#include "common/netio.h"

//char domain[] = "unb.br";

typedef struct {
    unsigned short id; 
 
    unsigned char qr; 
    unsigned char opcode;
    unsigned char aa; 
    unsigned char tc; 
    unsigned char rd; 
    unsigned char ra; 
    unsigned char z; 
    unsigned char ad; 
    unsigned char cd; 
    unsigned char rcode;
  
    unsigned short q_count; 
    unsigned short ans_count;
    unsigned short auth_count;
    unsigned short add_count; 
}DNSheader;

struct QUESTION{
    unsigned short qtype;
    unsigned short qclass;
};


Packet * ForgeDNS( void *p_arg );

#endif