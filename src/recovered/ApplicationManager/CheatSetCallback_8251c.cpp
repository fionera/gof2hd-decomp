#include "class.h"

extern "C" void ext_001ab5c8(void *cheats);

extern "C" void ApplicationManager_CheatSetCallback(ApplicationManager *self, void *callback, void *data)
{
    void *cheats = *(void **)((char *)self + 0x30);
    if (cheats != 0) {
        ext_001ab5c8(cheats);
    }
}
