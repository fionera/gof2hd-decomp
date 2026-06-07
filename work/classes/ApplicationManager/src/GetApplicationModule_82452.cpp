#include "class.h"

extern "C" void *ApplicationManager_GetApplicationModule(ApplicationManager *self, unsigned int id)
{
    unsigned int count = *(unsigned int *)((char *)self + 0x50);
    unsigned int index = 0;
    goto check;

advance:
    ++index;
check:
    if (index >= count) {
        return 0;
    }
    if (*(int *)(*(char **)((char *)self + 0x54) + index * 4) != (int)id) {
        goto advance;
    }
    return *(void **)(*(char **)((char *)self + 0x48) + index * 4);
}
