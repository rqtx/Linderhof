
#ifndef INTERFACE_H
  #define INTERFACE_H

#include "venus.h"

typedef enum { Attack } CmdType;

typedef struct {
    CmdType type;
    unsigned int dataSize;
    LhfDraft data;
}CommandPkt;
#define COMMANDPKT_HEADERSIZE ( sizeof(CmdType) + sizeof(int) )

typedef struct {
    int socket;
    struct sockaddr_in addr;
}ClientAddr;

Packet * CreateCmdAttack( int argc, char ** argv );

#endif
