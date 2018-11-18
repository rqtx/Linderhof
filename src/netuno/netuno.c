
#include <time.h>
#include <math.h>
#include "venus.h"
#include "netuno/injector.h"
#include "netuno/netuno.h"
#include "netuno/logger.h"

#define GetBucketByLevel(level) pow(10,level)/MAXINJECTORS

typedef struct { 
    Packet *pkt;
    unsigned int injCells;
    unsigned int bucket;
    unsigned int level;
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
    p_netuno->level = 0;
}

void StartNetunoInjector( Packet *p_pkt, unsigned int p_level, unsigned int p_timer, unsigned int p_inc, char *p_file )
{
    unsigned int masterClock = 0, incPoint = p_inc;
    NetunoInjector netuno;
    netuno.pkt = p_pkt;
    FILE *fpLog = CreateLoggerFile(p_file);

    netuno.injectors = StartInjector( netuno.pkt );
    netuno.injCells = MAXINJECTORS;
    
    if(p_level == 0 || p_level > NETUNO_MAXLEVEL)
    {
        freeAttack(&netuno);
    }
    else
    {
        netuno.level = p_level;
        netuno.bucket = GetBucketByLevel(netuno.level);
    }

    while(1)
    {
        SleepOneSec();
        masterClock++;
        getInjetorThp(&netuno);

        if( netuno.level )
        {
            resetBucket( &netuno );
        }
        LogInjection( fpLog, netuno.level, netuno.throughputCurrent, netuno.probes );
        
        if(p_timer > 0 && masterClock >= p_timer)
        {
            break;
        }

        if( incPoint != 0 && masterClock == incPoint )
        {
            incPoint += p_inc;
            netuno.level++;

            if( netuno.level <= NETUNO_MAXLEVEL )
            {
                netuno.bucket = GetBucketByLevel(netuno.level);
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

