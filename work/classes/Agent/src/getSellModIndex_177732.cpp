#include "class.h"

extern "C" int Agent_getSellModIndex(Agent *self)
{
    return F<int>(self, 0x94);
}
