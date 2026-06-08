#include "class.h"

// SolarSystem::isVisible() — ldrb.w r0,[r0,#0x44]; bx lr
extern "C" uint8_t SolarSystem_isVisible(SolarSystem *self)
{
    return self->f_44;
}
