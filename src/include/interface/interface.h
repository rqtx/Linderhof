
#ifndef INTERFACE_H
  #define INTERFACE_H

#include "venus.h"

typedef enum { UnknownCmd = 0, AttackCmd, ExitCmd } CmdType;

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
#define GetClientIP(p_addr) inet_ntoa(p_addr.sin_addr)

Packet * CreateCmdPacket( CmdType p_type, int p_argc, char **p_argv );

#endif
