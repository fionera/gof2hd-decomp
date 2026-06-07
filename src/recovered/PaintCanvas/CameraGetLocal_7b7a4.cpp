#include "class.h"

extern "C" __attribute__((disable_tail_calls)) void MatrixIdentity(void *result, void *matrix);

void *CameraGetLocal(AbyssEngine::PaintCanvas *self, unsigned int index)
{
    void *result;
    if (index < *(unsigned int *)((char *)self + 0x164)) {
        result = (char *)(*(void ***)((char *)self + 0x168))[index] + 0xc;
    } else {
        char tmp[60];
        result = (char *)self + 0xf8;
        MatrixIdentity(tmp, result);
    }
    return result;
}
