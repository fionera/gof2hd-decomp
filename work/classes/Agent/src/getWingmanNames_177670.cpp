#include "class.h"

extern "C" void *Agent_getWingmanNames(Agent *self)
{
    return F<void *>(self, 0x90);
}
