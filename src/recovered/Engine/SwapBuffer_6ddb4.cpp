#include "class.h"

extern "C" void Engine_SwapBuffer(Engine *self)
{
    uint32_t index = 0;
    uint32_t zero = 0;
    while (index < *(uint32_t *)((char *)self + 0x3d8)) {
        *(uint32_t *)(*(char **)((char *)self + 0x3dc) + index * 4) = zero;
        index += 1;
    }
}
