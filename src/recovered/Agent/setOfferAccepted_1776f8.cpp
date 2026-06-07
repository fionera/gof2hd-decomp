#include "class.h"

extern "C" void Agent_setOfferAccepted(Agent *self, bool v)
{
    F<uint8_t>(self, 0x84) = v;
}
