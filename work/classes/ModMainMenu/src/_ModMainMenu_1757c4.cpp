#include "class.h"

__attribute__((visibility("hidden"))) extern void *g_ModMainMenu_vtable;

extern "C" void _ZN11ModMainMenu9OnReleaseEv(ModMainMenu *self);

extern "C" ModMainMenu *_ZN11ModMainMenuD2Ev(ModMainMenu *self)
{
    self->f_0 = (char *)g_ModMainMenu_vtable + 8;
    _ZN11ModMainMenu9OnReleaseEv(self);
    return self;
}
