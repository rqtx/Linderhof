
#include "venus.h"
#include "interface/interface.h"
#include "monitor/crake.h"

#define PARSER_DELIM " "
#define PARSER_INITIALIZE(str) strtok(str, PARSER_DELIM)
#define PARSER_CONTINUE strtok(NULL, PARSER_DELIM)

#define MAXBUFFERSIZE 500
#define MAXARGS  20

static char * askServerIP( )
{
    char *serverIP = NULL;
    memalloc(&serverIP, sizeof(MAXBUFFERSIZE));
    fputs("\nServer IP: ", stdout);
    fgets(serverIP, MAXBUFFERSIZE, stdin);
    return serverIP;
}

static CmdType getCmd( char *p_cmd )
{
    if( !strcmp(p_cmd, "atk") )
    {
        return AttackCmd;
    }

    if( !strcmp(p_cmd, "exit") )
    {
        return ExitCmd;
    }
    
    return UnknownCmd;
}

static char ** parserUserCmd( int *r_args, char *p_buffer )
{
    char *str;
    int nargs = 0;
    char *argStr[MAXARGS];
    char **parsed = NULL;

    for(str = PARSER_INITIALIZE(p_buffer); ((str != NULL) && (nargs < MAXARGS)); str = PARSER_CONTINUE)
    {
        argStr[nargs] = str;
        nargs++;
    }

    memalloc( &parsed, sizeof(char)*nargs );
    for(int i = 0; i < nargs; i++)
    {
        parsed[i] = argStr[i];
    }
    
    *r_args = nargs;
    return parsed;
}

static Packet * getPacketFromInput()
{
    char buffer[MAXBUFFERSIZE];
    char **parsedBuffer = NULL;
    int args = 0;   
    Packet *pkt = NULL;

    while( NULL == pkt )
    {
        CmdType cmd = UnknownCmd;
        
        memset(buffer, 0, MAXBUFFERSIZE);
        fputs("_$: ", stdout);
        fgets(buffer, MAXBUFFERSIZE, stdin);
        buffer[strlen(buffer)-1] = '\0'; //Remove \n from enter

        parsedBuffer = parserUserCmd( &args, buffer );
        cmd = getCmd( parsedBuffer[0]);
        
        if( cmd == UnknownCmd )
        {
            LOG("UnknownCmd\n");
            continue;    
        }

        pkt = CreateCmdPacket(cmd, args, parsedBuffer, NULL);
        if( NULL == pkt )
        {
            LOG("Arg problem\n");
            continue;
        }
    }

    return pkt;
}

static int connectToServer()
{
    int sock = CreateSocket(TCP, true);
    char * serverIP = askServerIP();
    int serverPort = DEFAULT_COMPORT;
    struct sockaddr_in servAddr;

    /* Construct the server address structure */
    memset(&servAddr, 0, sizeof(servAddr));     /* Zero out structure */
    servAddr.sin_family      = AF_INET;             /* Internet address family */
    servAddr.sin_addr.s_addr = inet_addr(serverIP);   /* Server IP address */
    servAddr.sin_port        = htons(serverPort); /* Server port */

    /* Establish the connection to the echo server */
    if (connect(sock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0){
        Efatal( ERROR_NET, "connect() failed\n");
    }

    return sock;
}

void SnowmanShell()
{
    Packet *pac;
    CommandPkt *cmd;
    int sock = connectToServer();

    for(;;)
    {
        pac = getPacketFromInput();
        cmd = (CommandPkt *)pac->packet_ptr;
        
        switch( (int)cmd->type )
        {
            case AttackCmd:
                SendPacket(sock, pac);
                LOG("Packet sent\n");
                MonitorCrake(60);
        }

    }
}

