#include "class.h"

extern "C" void ext_001ab5d8(void *cheats);

extern "C" void ApplicationManager_CheatAddCode(ApplicationManager *self, void *code, int value)
{
    void *cheats = *(void **)((char *)self + 0x30);
    if (cheats != 0) {
        ext_001ab5d8(cheats);
    }
}
