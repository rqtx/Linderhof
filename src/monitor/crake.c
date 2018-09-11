#include "venus.h"
#include "interface/configr.h"
#include "interface/interface.h"

#define RCVBUFSIZE 2046

int sock = 0;
struct sockaddr_in servAddr;

static void startMonitor()
{
    char buf[RCVBUFSIZE];
    int recv_len;
    int monitorSock = CreateSocket( UDP, true );
    struct sockaddr_in addr_in = {
        .sin_family = AF_INET,
        .sin_port = htons(DEFAULT_COMPORT),
        .sin_addr.s_addr = htonl(INADDR_ANY)
    };
    struct sockaddr_in addr_recv;
    unsigned int slen = sizeof(addr_recv);

    //bind socket to port
    if( bind(monitorSock , (struct sockaddr*)&addr_in, sizeof(addr_in) ) == -1)
    {
        Efatal( ERROR_NET, "bind error" ); 
    }

    //keep listening for data
    while(1)
    {
        printf("Waiting for data...");
        fflush(stdout);
       
        //try to receive some data, this is a blocking call
        if ((recv_len = recvfrom(monitorSock, buf, RCVBUFSIZE, 0, (struct sockaddr *)&addr_recv, &slen)) == -1)
        {
            //die("recvfrom()");
        }
         
        //print details of the client/peer and the data received
        printf("Received packet from %s:%d\n", inet_ntoa(addr_recv.sin_addr), ntohs(addr_recv.sin_port));
        printf("Data: %s\n" , buf);
    }
 
    CloseSocket(monitorSock);
}

static void connectServer( char *p_servIP)
{

    sock = CreateSocket(TCP, true);

    /* Construct the server address structure */
    memset(&servAddr, 0, sizeof(servAddr));     /* Zero out structure */
    servAddr.sin_family      = AF_INET;             /* Internet address family */
    servAddr.sin_addr.s_addr = inet_addr(p_servIP);   /* Server IP address */
    servAddr.sin_port        = htons(DEFAULT_COMPORT); /* Server port */

    /* Establish the connection to the echo server */
    if (connect(sock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0){
        Efatal( ERROR_NET, "connect() failed");
    }
}

void Crake( int argc, char ** argv )
{
    char * serverIP = GetServerIP(); 
    Packet *pkt = CreateCmdAttack( argc, argv );
    connectServer( serverIP );
    SendPacket(sock, pkt); 
    SendPacket(sock, pkt); 
    //startMonitor();
}


