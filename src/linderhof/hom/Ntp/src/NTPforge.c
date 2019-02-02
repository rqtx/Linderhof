#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "strix.h"
#include "NTPforge.h"

Packet * ForgeNtpBinary( void *p_arg )
{
 // int *opcode = (int *)p_arg;
 Packet *pac = NULL;
 char * ntp_packet;
 NtpBinaryRequestHeader * ntp_header = NULL;
 int packetSize = 0;

 memalloc( (void *)&pac, sizeof(Packet) );

     packetSize = sizeof( NtpBinaryRequestHeader );
     memalloc( (void *)&ntp_packet, packetSize );

     ntp_header = ( NtpBinaryRequestHeader *) ntp_packet;

     // magic occurs
    ntp_header->rm_vn_mode=0x17;       //Sets the response bit to 0, More bit to 0, Version field to 2, Mode field to 7
    ntp_header->auth_seq=0x00;    /* key, sequence number */
    ntp_header->implementation=0x03;   //Sets the implementation to 3
    ntp_header->request=0x2a;

   //  // rm_vn_mode;     /* response, more, version, mode */
   // ntp_header->auth_seq=0x00;    /* key, sequence number */
   // ntp_header->err_nitems=0x0000;    /* error code/number of data items */
   // ntp_header->mbz_itemsize=0x0000;    /* item size */
   // ntp_header->tstamp=0x00000000;         /* time stamp, for authentication */
   // ntp_header->keyid=0x0000;        /* encryption key */

 pac->packet_ptr = ntp_packet;
 pac->pkt_size = packetSize;

 return pac;
}