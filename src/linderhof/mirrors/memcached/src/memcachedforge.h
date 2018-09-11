
/*
 *  packetforge.h
 *          
 */

#ifndef PACKETFURNACE_H
  #define PACKETFURNACE_H

#include "common/netio.h"

#define GetIpHeader(X)  ( (struct iphdr *) (X) )
#define GetUdpHeader(X) ( (struct udphdr *) (X + sizeof(struct iphdr)) )

/* Standard well-defined IP protocols.  
 *        netinet/in.h
 
 Protocols RFC 790
 
IPPROTO_IP		    0		dummy for IP
IPPROTO_ICMP		1	    control message protocol 
IPPROTO_GGP		    3		gateway^2 (deprecated) 
IPPROTO_TCP		    6		tcp 
IPPROTO_EGP		    8		exterior gateway protocol 
IPPROTO_PUP		    12		pup 
IPPROTO_UDP		    17		user datagram protocol 
IPPROTO_IDP		    22		xns idp 
IPPROTO_TP		    29 		tp-4 w/ class negotiation 
IPPROTO_EON		    80		ISO cnlp 

IPPROTO_RAW		    255		raw IP packet 
IPPROTO_MAX		    256
*/
#define MEMCACHED_GET 200
#define MEMCACHED_SET 201
#define MEMCACHED_STAT 202


#define MEMCACHED_KEY 0xABADF00D
#define MEMCACHED_KEYSTR "strix"

#define MAXSIZE_MEMCACHED_KEY 0x0004
#define MAXSIZE_MEMCACHED_KEYSTR 5
#define MAXSIZE_MEMCACHED_VALUE 0x500 //0x100000

typedef struct MemcachedSetCmdData{
  char cmd[4];
  char key[MAXSIZE_MEMCACHED_KEYSTR];
  uint16_t flags;
  uint32_t exptime; //This number of seconds may not exceed 60*60*24*30 (numberof seconds in 30 days)
  uint32_t dataLen;
  char* data;
  uint64_t cas;
}MemcachedSetCmd;

typedef struct MemcachedRequestHeaderData {
  uint8_t magic;            //Magic number identifying the package
  uint8_t opcode;           //Command code
  uint16_t key_length;      //Key length
  uint8_t extras_length;    //Extras length
  uint8_t data_type;        //Data type
  uint16_t vbucket_id;      //vbucket id
  uint32_t tot_length;      //Total body length
  uint32_t opaque;          //Opaque
  uint64_t cas;             //CAS
}MemcachedRequestHeader;

typedef struct MemcachedSetExtraData {
  uint32_t flags;
  uint32_t expiration;
}MemcachedSetExtra;

Packet * ForgeMemcachedUDP( char *ip_dest, char *ip_src, int dest_port, int src_port, int opcode );
Packet * ForgeMemcachedPacket( void *p_arg );
Packet * ForgeMemcachedCommand(void *p_arg);

#endif
