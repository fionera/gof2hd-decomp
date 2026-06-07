#include "class.h"

extern "C" void Engine_SetTextureSlot(Engine *self, uint32_t textureIndex, uint32_t slot);
extern "C" void Engine_GlEnable(Engine *self, unsigned int cap, bool enable);
extern "C" void glActiveTexture(unsigned int texture);
extern "C" void glDisable(unsigned int cap);
extern "C" void glBindTexture(unsigned int target, unsigned int texture);
extern "C" uint8_t g_Engine_useShaders;

extern "C" void Engine_SetTextures(Engine *self, uint32_t first, uint32_t second)
{
    void *manager = **(void ***)((char *)self + 0x30);
    uint32_t count = *(uint32_t *)((char *)manager + 0x10);
    if (count == 0 || first > count - 1) {
        return;
    }
    Engine_SetTextureSlot(self, first, 0);
    if (second > count - 1) {
        if (*(int *)((char *)self + 0x80) != -1) {
            if (g_Engine_useShaders == 0) {
                glActiveTexture(0x84c1);
                glDisable(0xde1);
                glActiveTexture(0x84c0);
            }
            *(int *)((char *)self + 0x80) = -1;
        }
        return;
    }
    uint32_t texture = **(uint32_t **)(*(char **)((char *)manager + 0x14) + second * 4);
    if (*(uint32_t *)((char *)self + 0x80) != texture) {
        glActiveTexture(0x84c1);
        Engine_GlEnable(self, 0xde1, true);
        unsigned int target = (g_Engine_useShaders != 0 &&
                               (*(uint32_t *)((char *)self + 0x420) & 0x80008) != 0)
                                  ? 0x8513
                                  : 0xde1;
        glBindTexture(target, texture);
        *(uint32_t *)((char *)self + 0x80) = texture;
    }
}
