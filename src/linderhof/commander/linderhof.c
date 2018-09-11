#include <pthread.h>
#include "venus.h"
#include "common/injector.h"
#include "commander/planner.h"
#include "commander/hom.h"
#include "interface/oryx.h"

static void closeLhf()
{
    Elog(SUCCESS, "Closing lhf\n");
    CloseOryxNet();
    DestroyAllInjectors();
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
    exit(SUCCESS);
}

static bool validateDraft( LhfDraft p_draft )
{
    return true;    
}

void LinderhofMaster()
{
    

    SetSigHdr(SIGINT,  closeHandler);
    SetSigHdr(SIGQUIT, closeHandler);
    ESetErroAction(fatalHandler);
    
    OryxNet();
}

int AddMirrorAttack( LhfDraft p_draft )
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

