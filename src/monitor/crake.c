#include "venus.h"
#include "interface/configr.h"
#include "interface/interface.h"
#include "monitor/crake.h"
#include "injector/logger.h"

#include <time.h>
#include <pthread.h>

#define RCVBUFSIZE 2046
#define SAMPLING 60
#define MEGABYTE 1000000

unsigned int bufferCounter = 0;

static struct timespec _onesec = {
               .tv_sec = 1,                     /* 1 seconds */
               .tv_nsec = 0       /* 0 nanoseconds */
          };

static void *throughputMonitorHandler( void *p_arg )
{
    float sample = 0;
    FILE *fp = NULL;    

      
    while(1)
    {

        for(int i = 0; i < SAMPLING; i++)
        { 
            bufferCounter = 0;
            nanosleep(&_onesec, NULL);
            sample += bufferCounter;
        }
 
        if(fp == NULL)
        {
            fp = stdout;
        }

        LogAttack(fp, (sample/SAMPLING)/MEGABYTE);
        sample = 0;
        nanosleep(&_onesec, NULL);
    }

  return NULL; //Keep compiler quiet
}

static void *reciverHandler( void *p_arg )
{
    char buf[RCVBUFSIZE];
    int recv_len;
    int monitorSock = CreateSocket( UDP, BLOCK );
    int monitorPort = CRAKE_DEFAULT_PORT;

    struct sockaddr_in addr_in = {
        .sin_family = AF_INET,
        .sin_port = htons(monitorPort),
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
        //try to receive some data, this is a blocking call
        if ((recv_len = recvfrom(monitorSock, buf, RCVBUFSIZE, 0, (struct sockaddr *)&addr_recv, &slen)) != -1)
        {
            bufferCounter += recv_len;
        }
         
        //print details of the client/peer and the data received
        //printf("Received packet from %s:%d\n", inet_ntoa(addr_recv.sin_addr), ntohs(addr_recv.sin_port));
        //printf("Data: %s\n" , buf);
    }
 
    CloseSocket(monitorSock);

    return NULL; //Keep compiler quiet
}

void StartMonitor()
{
    pthread_t monitorID;

    if( 0 != pthread_create(&monitorID, NULL, throughputMonitorHandler, NULL) )
    {
        Elog( ERROR_INJECTOR, "Monitor error");
    }

    reciverHandler( NULL );
}
