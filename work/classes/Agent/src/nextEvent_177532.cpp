#include "class.h"

extern "C" void Agent_nextEvent(Agent *self)
{
    F<int>(self, 0x54) = F<int>(self, 0x54) + 1;
}
