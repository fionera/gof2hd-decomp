#include "class.h"

struct Mission;

extern "C" Mission *Agent_getMission(Agent *self)
{
    return F<Mission *>(self, 0x8c);
}
