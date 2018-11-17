#include "venus.h"
#include "common/blacksmith.h"
#include "netuno/netuno.h"
#include "netuno/logger.h"
#include "common/pthreadHelper.h"


int main()
{
    Packet *pkt = ForgeUDP("10.0.0.1", "10.0.0.1", 0, NULL, "injector test");
    
    StartNetunoInjector( pkt, 0, 0); 
}
