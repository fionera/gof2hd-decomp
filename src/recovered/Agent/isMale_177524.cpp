#include "class.h"

extern "C" uint8_t Agent_isMale(Agent *self)
{
    return F<uint8_t>(self, 0x50);
}
