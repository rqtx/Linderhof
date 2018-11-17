
/*
 *  packetforge.h
 *          
 */

#ifndef PACKETFURNACE_H
  #define PACKETFURNACE_H

#include "common/netio.h"

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

#define ENDOFPACKET "\r\n"
#define ENDOFPACKET_SIZE strlen(ENDOFPACKET)

#define DATALEN_SIZE sizeof(uint32_t)

#define CMDGET_MAXSIZE 3 + SPACE_SIZE + MAXSIZE_MEMCACHED_KEYSTR + ENDOFPACKET_SIZE
//Binary protocol
typedef struct {
  uint8_t magic;            //Magic number identifying the package
  uint8_t opcode;           //Command code
  uint16_t key_length;      //Key length
  uint8_t extras_length;    //Extras length
  uint8_t data_type;        //Data type
  uint16_t vbucket_id;      //vbucket id
  uint32_t tot_length;      //Total body length
  uint32_t opaque;          //Opaque
  uint64_t cas;             //CAS
}BinaryRequestHeader;

typedef struct {
  uint32_t flags;
  uint32_t expiration;
}BinarySetExtra;

//Text Protocol
typedef struct {
    uint16_t requestID;
    uint16_t seqNumber;
    uint16_t nDatagrams;
    uint16_t reserved;
}TextProtocolHeader;

Packet * ForgeMemcachedBinary( void *p_arg );
Packet * ForgeMemcachedText(void *p_arg);

#endif
