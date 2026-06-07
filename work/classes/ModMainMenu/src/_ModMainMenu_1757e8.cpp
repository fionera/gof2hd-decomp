#include "class.h"

extern "C" ModMainMenu *_ZN11ModMainMenuD2Ev(ModMainMenu *self);
extern "C" void ModMainMenu_deleteTail(ModMainMenu *self);

extern "C" void _ZN11ModMainMenuD0Ev(ModMainMenu *self)
{
    ModMainMenu_deleteTail(_ZN11ModMainMenuD2Ev(self));
}
