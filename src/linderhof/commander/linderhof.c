#include <pthread.h>
#include "venus.h"
#include "common/capabilityHelper.h"
#include "commander/planner.h"
#include "commander/hom.h"
#include "interface/oryx.h"

static void closeLhf()
{
    Elog(SUCCESS, "Closing lhf\n");
    CloseOryxNet();
    memoryclean();
}
static void fatalHandler(int p_arg)
{
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
    return true;    
}

void LinderhofBootstrap()
{
    SetSigHdr(SIGINT,  closeHandler);
    SetSigHdr(SIGQUIT, closeHandler);
    ESetErroAction(fatalHandler);
    SetCapability(CAP_NET_RAW);
    //SetCapability(CAP_NET_ADMIN);
}

void LinderhofNet()
{
    LinderhofBootstrap(); 
    OryxNet();
}

void LinderhofCli( int p_argc, char **p_argv )
{
    LinderhofBootstrap();
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

