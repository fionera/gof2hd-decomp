#include "class.h"

extern "C" void Agent_setImageParts(Agent *self, int *parts)
{
    F<int *>(self, 0x88) = parts;
}
