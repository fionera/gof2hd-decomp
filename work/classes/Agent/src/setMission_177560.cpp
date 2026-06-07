#include "class.h"

struct Mission;

extern "C" void Agent_setMission(Agent *self, Mission *mission)
{
    F<Mission *>(self, 0x8c) = mission;
}
