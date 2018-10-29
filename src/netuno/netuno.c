
#include <time.h>
#include <math.h>
#include "venus.h"
#include "netuno/injector.h"
#include "netuno/netuno.h"
#include "netuno/logger.h"

#define MEGABYTE 1048576

typedef struct { 
    Packet *pkt;
    unsigned int injCells;
    unsigned int bucket;
    unsigned int throughputExpected;
    float throughputCurrent;
    Injector **injectors;
}NetunoInjector;


static void getInjetorThp( NetunoInjector * p_netuno )
{
    float pkt = 0;

    for(int i = 0; i < p_netuno->injCells; i++)
    {
        pkt += p_netuno->injectors[i]->net.pktCounter;
        p_netuno->injectors[i]->net.pktCounter = 0;
    }
    p_netuno->throughputCurrent = pkt * p_netuno->pkt->pkt_size; 
}

static void resetBucket( NetunoInjector *p_netuno )
{
    for(int i = 0; i < p_netuno->injCells; i++)
    {
        p_netuno->injectors[i]->net.bucketSize = p_netuno->bucket; 
    }
}

static void freeAttack( NetunoInjector *p_netuno )
{
    for(int i = 0; i < p_netuno->injCells; i++)
    {
        p_netuno->injectors[i]->net.freeBucket = true; 
    }
}

void StartNetunoInjector( Packet *p_pkt, unsigned int p_inithp, unsigned int p_timer )
{
    unsigned int masterClock = 0;
    NetunoInjector netuno;
    netuno.pkt = p_pkt;
    FILE *fpLog = CreateLoggerFile(ATK_LOGGER);

    netuno.injectors = StartInjector( netuno.pkt );
    netuno.injCells = MAXINJECTORS;
    
    if(p_inithp <= 0 || p_inithp > NETUNO_MAXTHP)
    {
        freeAttack(&netuno);
        netuno.throughputExpected = 0;
    }
    else
    {
        netuno.throughputExpected = p_inithp;
        netuno.bucket = ((netuno.throughputExpected * MEGABYTE) / netuno.pkt->pkt_size)/netuno.injCells;
    }

    while(1)
    {
        SleepOneSec();
        masterClock++;
        getInjetorThp(&netuno);

        if( netuno.throughputExpected )
        {
            resetBucket( &netuno );
        }
        //LogInjection( fpLog, netuno.throughputExpected, netuno.throughputCurrent);
        LogInjection( NULL, netuno.throughputExpected, netuno.throughputCurrent);
        
        if(p_timer > 0 && masterClock >= p_timer)
        {
            break;
        }
    }

    fclose(fpLog);
    for(int i = 0; i <= netuno.injCells; i++)
    {
        InjectorDestroy( netuno.injectors[i] );
    }
}

