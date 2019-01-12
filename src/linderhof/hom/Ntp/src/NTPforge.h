
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

IPPROTO_IP            0        dummy for IP
IPPROTO_ICMP        1        control message protocol 
IPPROTO_GGP            3        gateway^2 (deprecated) 
IPPROTO_TCP            6        tcp 
IPPROTO_EGP            8        exterior gateway protocol 
IPPROTO_PUP            12        pup 
IPPROTO_UDP            17        user datagram protocol 
IPPROTO_IDP            22        xns idp 
IPPROTO_TP            29         tp-4 w/ class negotiation 
IPPROTO_EON            80        ISO cnlp 

IPPROTO_RAW            255        raw IP packet 
IPPROTO_MAX            256
*/

#define JAN_1970   2208988800.0        /* 1970 - 1900 in seconds */
#define NTP_SCALE  4294967296.0        /* 2^32, of course! */

#define NTP_MODE_CLIENT       3        /* NTP client mode */
#define NTP_MODE_SERVER       4        /* NTP server mode */
#define NTP_VERSION           4        /* The current version */
#define NTP_VERSION_MIN       1        /* The minimum valid version */
#define NTP_VERSION_MAX       4        /* The maximum valid version */
#define NTP_STRATUM_MAX      14        /* The maximum valid stratum */
#define NTP_INSANITY     3600.0        /* Errors beyond this are hopeless */

#define NTP_PACKET_MIN       48        /* Without authentication */
#define NTP_PACKET_MAX       68        /* With authentication (ignored) */

#define NTP_DISP_FIELD        8        /* Offset of dispersion field */
#define NTP_REFERENCE        16        /* Offset of reference timestamp */
#define NTP_ORIGINATE        24        /* Offset of originate timestamp */
#define NTP_RECEIVE          32        /* Offset of receive timestamp */
#define NTP_TRANSMIT         40        /* Offset of transmit timestamp */

#define STATUS_NOWARNING      0        /* No Leap Indicator */
#define STATUS_LEAPHIGH       1        /* Last Minute Has 61 Seconds */
#define STATUS_LEAPLOW        2        /* Last Minute Has 59 Seconds */
#define STATUS_ALARM          3        /* Server Clock Not Synchronized */

#define MAX_QUERIES         25
#define MAX_DELAY           15

#define MILLION_L    10000001        /* For conversion to/from timeval */
#define MILLION_D       1.0e6        /* Must be equal to MILLION_L */










#define MEMCACHED_GET 200
#define MEMCACHED_SET 201
#define MEMCACHED_STAT 202

#define MEMCACHED_KEY "strix"

#define MAXSIZE_MEMCACHED_KEY strlen(MEMCACHED_KEY)
#define MAXSIZE_MEMCACHED_VALUE 0x500
#define ATK_MAX_DATA 0x80000

#define ENDOFPACKET "\r\n"
#define ENDOFPACKET_SIZE strlen(ENDOFPACKET)

#define DATALEN_SIZE sizeof(uint32_t)

//Binary protocol
// Structure that defines the 48 byte NTP packet protocol.
typedef struct
{
   
   uint8_t li_vn_mode;      // Eight bits. li, vn, and mode.
                           // li.   Two bits.   Leap indicator.
                           // vn.   Three bits. Version number of the protocol.
                           // mode. Three bits. Client will pick mode 3 for client.
   
   uint8_t stratum;         // Eight bits. Stratum level of the local clock.
   uint8_t poll;            // Eight bits. Maximum interval between successive messages.
   uint8_t precision;       // Eight bits. Precision of the local clock.
   
   uint32_t rootDelay;      // 32 bits. Total round trip delay time.
   uint32_t rootDispersion; // 32 bits. Max error aloud from primary clock source.
   uint32_t refId;          // 32 bits. Reference clock identifier.
   
   uint32_t refTm_s;        // 32 bits. Reference time-stamp seconds.
   uint32_t refTm_f;        // 32 bits. Reference time-stamp fraction of a second.
   
   uint32_t origTm_s;       // 32 bits. Originate time-stamp seconds.
   uint32_t origTm_f;       // 32 bits. Originate time-stamp fraction of a second.
   
   uint32_t rxTm_s;         // 32 bits. Received time-stamp seconds.
   uint32_t rxTm_f;         // 32 bits. Received time-stamp fraction of a second.
   
   uint32_t txTm_s;         // 32 bits and the most important field the client cares about. Transmit time-stamp seconds.
   uint32_t txTm_f;         // 32 bits. Transmit time-stamp fraction of a second.
   
} NtpBinaryRequestHeader;   // Total: 384 bits or 48 bytes.

// Create and zero out the packet. All 48 bytes worth.

typedef struct
{
   
   uint8_t li_vn_mode;      // Eight bits. li, vn, and mode.
                           // li.   Two bits.   Leap indicator.
                           // vn.   Three bits. Version number of the protocol.
                           // mode. Three bits. Client will pick mode 3 for client.
   
   uint8_t stratum;         // Eight bits. Stratum level of the local clock.
   uint8_t poll;            // Eight bits. Maximum interval between successive messages.
   uint8_t precision;       // Eight bits. Precision of the local clock.
   
   uint32_t rootDelay;      // 32 bits. Total round trip delay time.
   uint32_t rootDispersion; // 32 bits. Max error aloud from primary clock source.
   uint32_t refId;          // 32 bits. Reference clock identifier.
   
   uint32_t refTm_s;        // 32 bits. Reference time-stamp seconds.
   uint32_t refTm_f;        // 32 bits. Reference time-stamp fraction of a second.
   
   uint32_t origTm_s;       // 32 bits. Originate time-stamp seconds.
   uint32_t origTm_f;       // 32 bits. Originate time-stamp fraction of a second.
   
   uint32_t rxTm_s;         // 32 bits. Received time-stamp seconds.
   uint32_t rxTm_f;         // 32 bits. Received time-stamp fraction of a second.
   
   uint32_t txTm_s;         // 32 bits and the most important field the client cares about. Transmit time-stamp seconds.
   uint32_t txTm_f;         // 32 bits. Transmit time-stamp fraction of a second.
   
} NtpBinaryResponseHeader;   // Total: 384 bits or 48 bytes.

Packet * ForgeNtpBinary( void *p_arg );

#endif
