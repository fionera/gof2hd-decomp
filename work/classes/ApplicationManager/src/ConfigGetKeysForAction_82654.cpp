#include "class.h"

extern "C" void Array_StringPtr_ctor(void *array);
extern "C" void ArrayAdd_StringPtr(String *string, void *array);

extern "C" void *ApplicationManager_ConfigGetKeysForAction(ApplicationManager *self, long long action)
{
    int low = (int)action;
    int high = (int)(action >> 32);
    unsigned int byteOffset = 0;
    void *result = 0;
    unsigned int index = 0;
    goto check;

loop:
    {
        char *actions = *(char **)((char *)self + 0x8c);
        int actionLow = *(int *)(actions + byteOffset);
        int actionHigh = *(int *)(actions + byteOffset + 4);
        int mismatch = (actionLow ^ low) | (actionHigh ^ high);
        if (mismatch == 0) {
            if (result == 0) {
                result = operator new(0xc);
                Array_StringPtr_ctor(result);
            }
            String *string = (String *)operator new(0xc);
            actions = *(char **)((char *)self + 0x8c);
            char *keys = *(char **)((char *)self + 0x10);
            unsigned int keyIndex = *(unsigned int *)(actions + byteOffset + 8);
            new (string) String(*(String *)(keys + keyIndex * 0x10 + 4), false);
            ArrayAdd_StringPtr(string, result);
        }
        byteOffset += 0x10;
        index += 2;
    }
check:
    if (index < *(unsigned int *)((char *)self + 0x88)) {
        goto loop;
    }
    return result;
}
