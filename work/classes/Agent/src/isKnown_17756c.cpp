#include "class.h"

extern "C" bool Agent_isKnown(Agent *self)
{
    return F<int>(self, 0x54) > 0;
}
