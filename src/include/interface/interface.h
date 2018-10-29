
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

/**
 *  @brief Create a Command packet
 *  @param p_type[in] Command type
 *  @param p_argc[in] Number of arguments passed to command
 *  @param p_argv[in] Command arguments
 *  @param p_argv[in] Packet destination, NULL if not necessary
 * */
Packet * CreateCmdPacket( CmdType p_type, int p_argc, char **p_argv, char *p_destip );

/**
 * @brief Set a LhfDraft with default data
 *
 * @param p_draft[in] LhfDraft pointer
 * */
void SetDraftDefaultData( LhfDraft *p_draft );

#endif
