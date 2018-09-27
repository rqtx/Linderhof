#include "venus.h"
#include "netuno/netuno.h"
#include "common/blacksmith.h"

int ExecuteTestMirror( void *p_draft )
{
  LhfDraft *plan = (LhfDraft *)p_draft;
  Packet *pac = ForgeUDP( plan->amp_ip, plan->target_ip, plan->amp_port, NULL, "MirrorTest" );

  StartNetunoInjector( pac, plan->initialThroughput, plan->timer, plan->incrementThroughput);

  return SUCCESS;
}
