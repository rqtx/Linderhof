#ifndef PACKETFURNACE_H
#define PACKETFURNACE_H

#include "common/netio.h"

//Binary protocol

typedef struct {
   unsigned char rm_vn_mode;     /* response, more, version, mode */
   unsigned char auth_seq;    /* key, sequence number */
   unsigned char implementation;    /* implementation number */
   unsigned char request;        /* request number */
   unsigned short err_nitems;    /* error code/number of data items */
   unsigned short  mbz_itemsize;    /* item size */
   char data[40];          /* data area [32 prev](176 byte max) */
   unsigned long tstamp;         /* time stamp, for authentication */
   unsigned int keyid;        /* encryption key */
   char mac[8];      /* (optional) 8 byte auth code */
}NtpBinaryRequestHeader;

typedef struct {
   unsigned char rm_vn_mode;     /* response, more, version, mode */
   unsigned char auth_seq;    /* key, sequence number */
   unsigned char implementation;    /* implementation number */
   unsigned char request;        /* request number */
   unsigned short err_nitems;    /* error code/number of data items */
   unsigned short  mbz_itemsize;    /* item size */
   char data[40];          /* data area [32 prev](176 byte max) */
   unsigned long tstamp;         /* time stamp, for authentication */
   unsigned int keyid;        /* encryption key */
   char mac[8];      /* (optional) 8 byte auth code */
}NtpBinaryResponseHeader;

Packet * ForgeNtpBinary( void *p_arg );

#endif
