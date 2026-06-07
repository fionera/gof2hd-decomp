#include "class.h"

extern "C" void glActiveTexture(unsigned int texture);
extern "C" void glBindTexture(unsigned int target, unsigned int texture);

extern "C" void Engine_SetFrameBufferTexture(Engine *self, int slot0, int slot1)
{
    char *firstBase = (char *)self + slot0 * 4;
    int firstValue = *(int *)(firstBase + 0x48c);
    if (firstValue != -1) {
        int *first = (int *)(firstBase + 0x48c);
        glActiveTexture(0x84c0);
        glBindTexture(0xde1, *first);
    }

    if (slot1 == -1) {
        return;
    }
    char *secondBase = (char *)self + slot1 * 4;
    int secondValue = *(int *)(secondBase + 0x48c);
    if (secondValue == -1) {
        return;
    }
    int *second = (int *)(secondBase + 0x48c);
    glActiveTexture(0x84c1);
    return glBindTexture(0xde1, *second);
}
