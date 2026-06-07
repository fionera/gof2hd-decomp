#include "class.h"

typedef void ModuleCallback(void *);

extern "C" void Engine_Suspend(void *engine);

extern "C" void ApplicationManager_Suspend(ApplicationManager *self)
{
    unsigned int state = *(unsigned int *)((char *)self + 0x3c);
    if (state - 3 < 2) {
        return;
    }

    void *module = *(void **)((char *)self + 0x18);
    if (module != 0) {
        ModuleCallback **vtable = *(ModuleCallback ***)module;
        vtable[0x3c / 4](module);
        void *engine = *(void **)((char *)self + 0xa8);
        if (engine != 0) {
            Engine_Suspend(engine);
        }
        int oldState = *(int *)((char *)self + 0x3c);
        *(int *)((char *)self + 0x3c) = 3;
        *(int *)((char *)self + 0x40) = oldState;
    }
}
