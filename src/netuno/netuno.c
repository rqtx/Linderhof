
#include <time.h>
#include <math.h>
#include "venus.h"
#include "netuno/injector.h"
#include "netuno/netuno.h"
#include "netuno/logger.h"

typedef struct { 
    Packet *pkt;
    unsigned int injCells;
    unsigned int bucket;
    unsigned int throughputExpected;
    unsigned int probes;
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
    p_netuno->probes = pkt;
    p_netuno->throughputCurrent = p_netuno->probes * p_netuno->pkt->pkt_size; 
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
    p_netuno->throughputExpected = 0;
}

void StartNetunoInjector( Packet *p_pkt, unsigned int p_inithp, unsigned int p_timer, unsigned int p_inc, char *p_file )
{
    unsigned int masterClock = 0, incPoint = p_inc;
    NetunoInjector netuno;
    netuno.pkt = p_pkt;
    FILE *fpLog = CreateLoggerFile(p_file);

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
        LogInjection( fpLog, netuno.throughputExpected, netuno.throughputCurrent, netuno.probes );
        
        if(p_timer > 0 && masterClock >= p_timer)
        {
            break;
        }

        if( incPoint != 0 && masterClock == incPoint )
        {
            incPoint += p_inc;
            netuno.throughputExpected += 10;

            if( netuno.throughputExpected <= NETUNO_MAXTHP )
            {
                netuno.bucket = ((netuno.throughputExpected * MEGABYTE) / netuno.pkt->pkt_size)/netuno.injCells;
            }
            else
            {
                freeAttack(&netuno);
                incPoint = 0;
            }
        }
    }
    
    for(int i = 0; i < netuno.injCells; i++)
    {
        netuno.injectors[i]->net.bucketSize = 0;
    }

    if(fpLog != NULL)
    {
        fclose(fpLog);
    }

    for(int i = 0; i < netuno.injCells; i++)
    {
        InjectorDestroy( netuno.injectors[i] );
    }
}

