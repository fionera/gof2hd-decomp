#include "class.h"

extern "C" char g_perfPending;
extern "C" uint64_t g_perfCounter;
extern "C" uint64_t g_perfLimit;
extern "C" char g_perfExpired;

extern "C" void ApplicationManager_SetCurrentApplicationModule(ApplicationManager *self, unsigned int id)
{
    char *pending = &g_perfPending;
    if (*pending != 0) {
        uint64_t *counter = &g_perfCounter;
        uint64_t limit = g_perfLimit;
        uint64_t value = *counter + 1;
        *counter = value;
        if ((long long)(value - limit) >= 0) {
            *(volatile char *)&g_perfExpired = 1;
            *(volatile char *)pending = 0;
        }
    }

    unsigned int count = *(unsigned int *)((char *)self + 0x50);
    unsigned int index = 0;
    while (index < count) {
        if (*(unsigned int *)(*(char **)((char *)self + 0x54) + index * 4) == id) {
            void *module = *(void **)(*(char **)((char *)self + 0x48) + index * 4);
            int current = *(int *)((char *)self + 0x18);
            *(unsigned int *)((char *)self + 0x3c) = current != 0;
            *(unsigned int *)((char *)self + 0x5c) = id;
            *(void **)((char *)self + 0x60) = module;
            return;
        }
        ++index;
    }
}
