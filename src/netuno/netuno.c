
#include <time.h>
#include "venus.h"
#include "netuno/injector.h"
#include "netuno/netuno.h"
#include "netuno/logger.h"

#define FREQUENCY 5
#define MAX_INJECTORCELlS NETUNO_MAXTHP / MAX_INJECTORCELL_THP
#define getNumberOfCells(thp) ( (thp / MAX_INJECTORCELL_THP) <= 0 ) ? 1 : (thp / MAX_INJECTORCELL_THP)

typedef struct { 
    InjectorType type;
    InjectorMonitor monitor;
    Packet *pkt;
    unsigned int injCells;
    unsigned int lastActCell;
    Injector **injectors;
}NetunoInjector;

static Injector ** fullAttack( Packet *p_pkt, int p_inithp, int p_injCells )
{
    Injector **injectors = NULL;

    memalloc( &injectors, sizeof(Injector)*p_injCells );
    
    for(int i = 0; i < p_injCells; i++)
    {
        injectors[i] = StartInjector( p_pkt, MAX_INJECTORCELL_THP);
    }
    return injectors;
}

static Injector ** incAttack( Packet *p_pkt, int p_injCells )
{
    Injector **injectors = NULL;

    memalloc( &injectors, sizeof(Injector)*p_injCells ); 
    injectors[0] = StartInjector( p_pkt, 0);
    return injectors;
}

static void getInjetorThp( NetunoInjector * p_inj )
{
    p_inj->monitor.throughputCurrent = 0;
    p_inj->monitor.throughputExpected = 0;

    for(int i = 0; i <= p_inj->lastActCell; i++)
    {
        p_inj->monitor.throughputCurrent += p_inj->injectors[i]->monitor.throughputCurrent;
        p_inj->monitor.throughputExpected += p_inj->injectors[i]->monitor.throughputExpected;
    }
}

static void incrementInjector( NetunoInjector *p_inj )
{
    if( p_inj->injectors[p_inj->lastActCell]->monitor.throughputExpected < MAX_INJECTORCELL_THP )
    {
        p_inj->injectors[p_inj->lastActCell]->monitor.throughputExpected++;
    }
    else
    {
        if( (p_inj->lastActCell+1) < p_inj->injCells )
        {
            p_inj->lastActCell++;
            p_inj->injectors[p_inj->lastActCell] = StartInjector( p_inj->pkt, 1);
        }
    }
}

void StartNetunoInjector( Packet *p_pkt, unsigned int p_inithp, unsigned int p_timer, InjectorType p_type )
{
    unsigned int masterClock = 0;
    unsigned int incClock = 0;
    unsigned int thp = (p_inithp <= 0 || p_inithp > NETUNO_MAXTHP) ? NETUNO_MAXTHP : p_inithp;
    NetunoInjector netuno;
    netuno.pkt = p_pkt;

#ifdef ORYXNET
    FILE *fpLog = CreateLoggerFile(ATK_LOGGER);
#else
    FILE *fpLog = stdout;
#endif

    LOG("Netuno\n");

    switch( p_type )
    {
        case FULL:
            netuno.type = FULL;
            netuno.injCells = getNumberOfCells(thp);
            netuno.lastActCell = netuno.injCells-1;
            netuno.injectors = fullAttack( netuno.pkt, thp, netuno.injCells);
            break;
        case INCREMENT:
        default:
            netuno.type = INCREMENT;
            netuno.injCells = getNumberOfCells(NETUNO_MAXTHP);
            netuno.injectors = incAttack( netuno.pkt, netuno.injCells);
            netuno.lastActCell = 0;
            break;
        
    }

    while(1)
    {
        getInjetorThp(&netuno);
        if( netuno.type == INCREMENT && netuno.monitor.throughputExpected < NETUNO_MAXTHP && incClock >= FREQUENCY )
        {
            incrementInjector(&netuno);      
            incClock = 0;
        }

        LogInjection( fpLog, netuno.monitor.throughputExpected, netuno.monitor.throughputCurrent);
        SleepOneMinute();
        masterClock++;
        incClock++;

        if(p_timer > 0 && masterClock >= p_timer)
        {
            break;
        }
    }

    fclose(fpLog);

    for(int i = 0; i <= netuno.lastActCell; i++)
    {
        InjectorDestroy( netuno.injectors[i] );
    }
}

