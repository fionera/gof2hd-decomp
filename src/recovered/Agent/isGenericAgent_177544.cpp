#include "class.h"

extern "C" bool Agent_isGenericAgent(Agent *self)
{
    return F<int>(self, 0x58) == 1;
}
