#include "common/common.h"
#include "common/capabilityHelper.h"

//effective capabilities for the caller
void SetCapability( cap_value_t p_cap )
{
    cap_t caps;
    caps = cap_get_proc();
    if (caps == NULL)
    {
        /* handle error */;
    }

    if (cap_set_flag(caps, CAP_EFFECTIVE, 2, &p_cap, CAP_SET) == -1)
    {
        /* handle error */;
    }

    if (cap_set_proc(caps) == -1)
    {
        /* handle error */;
    }

    if (cap_free(caps) == -1)
    {
        /* handle error */;
    }
}
