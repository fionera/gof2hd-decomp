#include "class.h"

extern "C" void Agent_giveRewardAtNextChat(Agent *self, bool v)
{
    F<uint8_t>(self, 0x85) = v;
}
