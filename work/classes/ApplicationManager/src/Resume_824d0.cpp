#include "class.h"

typedef void ModuleCallback(void *);

extern "C" void Engine_Resume(void *engine);

extern "C" void ApplicationManager_Resume(ApplicationManager *self)
{
    if (*(int *)((char *)self + 0x3c) != 3) {
        return;
    }

    void *module = *(void **)((char *)self + 0x18);
    if (module != 0) {
        ModuleCallback **vtable = *(ModuleCallback ***)module;
        vtable[0x40 / 4](module);
        void *engine = *(void **)((char *)self + 0xa8);
        if (engine != 0) {
            Engine_Resume(engine);
        }
        *(uint32_t *)((char *)self + 0xa0) = 0;
        *(uint32_t *)((char *)self + 0xa4) = 0;
        *(uint32_t *)((char *)self + 0x80) = 0;
        *(uint32_t *)((char *)self + 0x84) = 0;
        *(int *)((char *)self + 0x3c) = 4;
    }
}
