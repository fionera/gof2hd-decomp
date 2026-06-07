#include "class.h"

typedef void KeyCallback(void *, void *, unsigned int, unsigned int, unsigned int, unsigned int);

extern "C" void ApplicationManager_OnKeyPress(ApplicationManager *self, int key)
{
    *(uint32_t *)((char *)self + 0x98) = 0;
    *(int *)((char *)self + 0x08) = key;
    *(int *)((char *)self + 0x0c) = key >> 31;
    *(uint32_t *)((char *)self + 0x9c) = 0;

    unsigned int keyLow = 0;
    unsigned int keyHigh = 0;
    unsigned int actionLow = 0;
    unsigned int actionHigh = 0;
    unsigned int keyIndex = 0;
    int *mapping = *(int **)((char *)self + 0x10);
    while (keyIndex <= 0x3f) {
        if (*mapping == key) {
            int highIndex = (int)keyIndex - 0x20;
            keyLow = 1u << keyIndex;
            if (highIndex >= 0) {
                keyLow = 0;
            }
            keyHigh = 1u >> (0x20 - keyIndex);
            if (highIndex >= 0) {
                keyHigh = 1u << highIndex;
            }
            *(uint64_t *)((char *)self + 0x80) =
                *(uint64_t *)((char *)self + 0x80) | (((uint64_t)keyHigh << 32) | keyLow);

            unsigned int offset = 0;
            for (unsigned int i = 0; i < *(unsigned int *)((char *)self + 0x88); i += 2) {
                char *entry = *(char **)((char *)self + 0x8c) + offset;
                if (*(unsigned int *)(entry + 8) == keyIndex && *(int *)(entry + 0x0c) == 0) {
                    actionLow |= *(uint32_t *)entry;
                    actionHigh |= *(uint32_t *)(entry + 4);
                    *(uint32_t *)((char *)self + 0x98) = actionLow;
                    *(uint32_t *)((char *)self + 0x9c) = actionHigh;
                    *(uint32_t *)((char *)self + 0xa0) |= actionLow;
                    *(uint32_t *)((char *)self + 0xa4) |= actionHigh;
                }
                offset += 0x10;
            }
            break;
        }
        mapping += 4;
        ++keyIndex;
    }

    void *module = *(void **)((char *)self + 0x18);
    if (module != 0 && *(int *)((char *)self + 0x3c) == 5) {
        void **vtable = *(void ***)module;
        ((KeyCallback *)vtable[0x10 / 4])(module, module, keyLow, keyHigh, actionLow, actionHigh);
    }
}
