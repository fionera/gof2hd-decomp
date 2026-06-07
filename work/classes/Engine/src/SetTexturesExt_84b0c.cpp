#include "class.h"

extern "C" void Engine_SetTextureSlot(Engine *self, uint32_t textureIndex, uint32_t slot);
extern "C" void glActiveTexture(unsigned int texture);

extern "C" void Engine_SetTexturesExt(Engine *self, uint32_t first, uint32_t second,
                                       uint32_t third, ...)
{
    void * volatile cookie = __stack_chk_guard;
    void *manager = **(void ***)((char *)self + 0x30);
    if (*(uint32_t *)((char *)manager + 0x10) != 0) {
        uint32_t values[3] = {first, second, third};
        uint32_t slot = 0;
        uint32_t *p = values;
        while (*p != 0xffffffff) {
            Engine_SetTextureSlot(self, *p, slot);
            slot += 1;
            p += 1;
        }
        for (uint32_t i = slot; i < 0x14; i += 1) {
            *(uint32_t *)((char *)self + 0x7c + i * 4) = 0xffffffff;
        }
        glActiveTexture(0x84c0);
    }
    if ((uint32_t)__stack_chk_guard - (uint32_t)cookie == 0) {
        return;
    }
    __stack_chk_fail();
}
