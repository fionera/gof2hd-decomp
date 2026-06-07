#include "class.h"

__attribute__((visibility("hidden"))) extern int *g_ModMainMenu_suspendObj;

extern "C" void ModMainMenu_suspendTail(int obj);

extern "C" void _ZN11ModMainMenu9OnSuspendEv(ModMainMenu *self)
{
    int obj = *g_ModMainMenu_suspendObj;
    if (obj != 0)
        ModMainMenu_suspendTail(obj);
}
