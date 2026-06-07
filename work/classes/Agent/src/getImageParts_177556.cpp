#include "class.h"

extern "C" int *Agent_getImageParts(Agent *self)
{
    return F<int *>(self, 0x88);
}
