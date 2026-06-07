#include "class.h"

extern "C" void ext_001ab328(void *engine, unsigned short duration);

extern "C" void ApplicationManager_Vibrate(ApplicationManager *self, unsigned short duration)
{
    if (*(bool *)((char *)self + 0xb2)) {
        ext_001ab328(*(void **)((char *)self + 0xa8), duration);
    }
}
