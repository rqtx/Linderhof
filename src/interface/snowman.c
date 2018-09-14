
#include "venus.h"
#include "interface/interface.h"
#include "interface/configr.h"

#define PARSER_DELIM " "
#define PARSER_INITIALIZE(str) strtok(str, PARSER_DELIM)
#define PARSER_CONTINUE strtok(NULL, PARSER_DELIM)

#define MAXBUFFERSIZE 500
#define MAXARGS  20
static CmdType getCmd( char *p_cmd )
{
    if( !strcmp(p_cmd, "atk") )
    {
        return AttackCmd;
    }
    
    return UnknownCmd;
}

static int parserUserCmd( char **r_parsed, char *p_buffer )
{
    char *str;
    int nargs = 0;
    char *argStr[MAXARGS];

    for(str = PARSER_INITIALIZE(p_buffer); ((str != NULL) && (nargs < MAXARGS)); str = PARSER_CONTINUE)
    {
        argStr[nargs] = str;
        nargs++;
    }

    memalloc( &r_parsed, sizeof(char)*nargs );
    for(int i = 0; i < nargs; i++)
    {
        r_parsed[i] = argStr[i];
    }

    return nargs;
}

static Packet * getPacketFromInput()
{
    char buffer[MAXBUFFERSIZE];
    int bread = 0;
    char **parsedBuffer = NULL;
    int args = 0;   
    Packet *pkt = NULL;

    while( NULL == pkt )
    {
        CmdType cmd = UnknownCmd;
        
        memset(buffer, 0, MAXBUFFERSIZE);
        
        bread = fread(buffer, sizeof(char), MAXBUFFERSIZE, stdin);
        
        args = parserUserCmd( parsedBuffer, buffer );
        cmd = getCmd( parsedBuffer[0]);
        
        if( cmd == UnknownCmd )
        {
            LOG("UnknownCmd");
            continue;    
        }

        pkt = CreateCmdPacket(cmd, args, parsedBuffer);
    }

    return pkt;
}

static int connectToServer()
{
    int sock = CreateSocket(TCP, true);
    char * serverIP = GetServerIP();
    int serverPort = GetServerPort();
    struct sockaddr_in servAddr;

    /* Construct the server address structure */
    memset(&servAddr, 0, sizeof(servAddr));     /* Zero out structure */
    servAddr.sin_family      = AF_INET;             /* Internet address family */
    servAddr.sin_addr.s_addr = inet_addr(serverIP);   /* Server IP address */
    servAddr.sin_port        = htons(serverPort); /* Server port */


    /* Establish the connection to the echo server */
    if (connect(sock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0){
        Efatal( ERROR_NET, "connect() failed");
    }

    return sock;
}

void SnowmanShell()
{
    Packet *pac;
    //int sock = connectToServer();

    for(;;)
    {
        pac = getPacketFromInput();
        //SendPacket(sock, pac);    
    }
}

