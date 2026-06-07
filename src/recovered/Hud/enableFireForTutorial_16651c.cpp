#include "class.h"

extern "C" void Hud_enableFireForTutorial(Hud *self, bool value)
{
    UC(self, 0x4a5) = value;
}
