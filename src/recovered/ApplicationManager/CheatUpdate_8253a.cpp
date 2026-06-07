#include "class.h"

extern "C" void ext_001ab5e8(void *cheats, unsigned short key);

extern "C" void ApplicationManager_CheatUpdate(ApplicationManager *self, unsigned short key)
{
    if (*(bool *)((char *)self + 0x34)) {
        void *cheats = *(void **)((char *)self + 0x30);
        if (cheats != 0) {
            ext_001ab5e8(cheats, key);
        }
    }
}
