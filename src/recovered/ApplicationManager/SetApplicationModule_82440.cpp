#include "class.h"

extern "C" void ApplicationManager_SetApplicationModule(ApplicationManager *self, void *module)
{
    int current = *(int *)((char *)self + 0x18);
    *(void **)((char *)self + 0x60) = module;
    *(unsigned int *)((char *)self + 0x3c) = current != 0;
}
