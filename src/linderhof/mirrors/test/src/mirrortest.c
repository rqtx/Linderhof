#include "venus.h"
#include "injector/injector.h"
#include "common/blacksmith.h"

int ExecuteTestMirror( void *p_draft )
{
  int injectorId;
  LhfDraft *plan = (LhfDraft *)p_draft;
  Packet *pac = ForgeUDP( plan->amp_ip, plan->target_ip, plan->amp_port, NULL, "MirrorTest" );

  injectorId = CreateInjection( *pac, plan->initialThroughput, plan->timer, plan->timeFrequency, plan->incrementThroughput);
  InjectionResume( injectorId );

  return SUCCESS;
}
