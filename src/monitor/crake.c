#include "venus.h"
#include "interface/configr.h"
#include "interface/interface.h"

#define RCVBUFSIZE 2046

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
