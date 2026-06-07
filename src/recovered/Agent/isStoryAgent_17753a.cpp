#include "class.h"

extern "C" bool Agent_isStoryAgent(Agent *self)
{
    return F<int>(self, 0x58) == 0;
}
