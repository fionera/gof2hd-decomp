#include "class.h"

// SolarSystem::setVisible(bool) — strb.w r1,[r0,#0x44]; bx lr
extern "C" void SolarSystem_setVisible(SolarSystem *self, bool v)
{
    self->f_44 = v;
}
