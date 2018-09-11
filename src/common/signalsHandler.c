
#include "common/common.h"

int SetSigHdr( int p_sig, void p_func(int sig) )
{
    struct sigaction act;
    
    memset (&act, '\0', sizeof(act));
    
    act.sa_handler = p_func;
    
    if (sigaction(p_sig, &act, NULL) < 0) 
    {
         ELOG(ERROR_SIGNAL, "sigaction error");
    }

    return SUCCESS;
}
