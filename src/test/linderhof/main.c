#include "venus.h"
#include "interface/interface.h"
#include "commander/linderhof.h"

#define IP "10.0.0.1"
int main()
{
    LhfDraft *draft = NULL;
    memalloc( &draft , sizeof(LhfDraft) );
    SetDraftDefaultData(draft);
    memcpy(draft->target_ip, IP, strlen(IP)); 
    memcpy(draft->amp_ip, IP, strlen(IP));
    draft->amp_port = 0;
    draft->target_port = 0;
    draft->type = TEST;
    draft->throughput = 0;
    draft->timer = 60;
    StartMirrorAttack( *draft );    
}
