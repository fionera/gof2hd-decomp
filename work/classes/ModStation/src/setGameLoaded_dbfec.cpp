#include "class.h"

// ModStation::setGameLoaded()
extern "C" void ModStation_setGameLoaded(ModStation *self)
{
    UC(self, 0xb0) = 1;
    UC(self, 0xb3) = 1;
}
