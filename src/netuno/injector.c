#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include "venus.h"
#include "netuno/injector.h"
#include "netuno/logger.h"
#include "common/pthreadHelper.h"

#define THPINCREMENT 1

#define SAMPLING 10
#define MEGABYTE 1000000
#define DEFAULT_SIZE 1

static void *_injectorHandler( void *p_arg )
{
    Injector *injector = (Injector *)p_arg;

    while(1)
    {
        while( injector->net.bucketSize || injector->net.freeBucket )
        {
            if( ERROR_NET != SendPacket(injector->net.socket, injector->net.pkt) )
            {
                //Packet was sent
                injector->net.pktCounter++;
                injector->net.bucketSize--;
            }
            else
            {
                injector->net.pktDroped++;
            }
        }
    };
    return NULL; //Keep compiler quiet
}

void injectorBootstrap( Injector *injector )
{
    if( ERROR_THREAD == (injector->id = CreateThread(LVL_HIGH, _injectorHandler, injector)) )
    {
        Efatal( ERROR_INJECTOR, "Cannot create thread: ERROR_INJECTOR\n");
    }

}

Injector ** StartInjector( Packet *p_pkt )
{
    Injector **injector = NULL;
    int n = 1;

    memalloc(&injector, sizeof(Injector)*MAXINJECTORS);

    for(int i = 0; i < MAXINJECTORS; i++)
    {
        Injector *inj = NULL;

        memalloc(&inj, sizeof(Injector));

        inj->net.pkt = p_pkt;
        inj->net.socket = CreateSocket( RAW, BLOCK );
        SetSocketFlag(inj->net.socket, SO_BROADCAST);
        SetSocketFlag(inj->net.socket, SO_PRIORITY); 
        Setup_sendbuffer( inj->net.socket, n );
        
        inj->net.freeBucket = false;
        inj->net.bucketSize = 0;
        inj->net.pktCounter = 0;
        inj->net.pktDroped = 0;

        injectorBootstrap(inj);
        injector[i] = inj;
    }
    return injector;
}

void InjectorDestroy( Injector *p_injector )
{
    pthread_cancel(p_injector->id);
    memfree(&p_injector);
}
