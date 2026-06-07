#include "class.h"

// ModStation::showMapWindow()
extern "C" void ModStation_showMapWindow(ModStation *self)
{
    UC(self, 0x64) = 0;
    UC(self, 0x67) = 1;
}
