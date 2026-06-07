#include "class.h"

extern "C" float g_Engine_texEnv;
extern "C" uint8_t g_Engine_useShaders;
extern "C" uint8_t g_Engine_texEnvDirty;
extern "C" void glActiveTexture(unsigned int texture);
extern "C" void glTexEnvf(unsigned int target, unsigned int pname, float value);
extern "C" void glBindTexture(unsigned int target, unsigned int texture);

extern "C" void Engine_SetTextureSlot(Engine *self, uint32_t textureIndex, uint32_t slot)
{
    void *manager = **(void ***)((char *)self + 0x30);
    uint32_t count = *(uint32_t *)((char *)manager + 0x10);
    if (count == 0 || slot >= 8 || textureIndex > count - 1) {
        return;
    }
    uint32_t *bound = (uint32_t *)((char *)self + 0x7c + slot * 4);
    void *textureEntry = *(void **)(*(char **)((char *)manager + 0x14) + textureIndex * 4);
    uint32_t texture = **(uint32_t **)(&textureEntry);
    if (*bound == texture) {
        return;
    }
    glActiveTexture(slot + 0x84c0);
    float env = *(float *)((char *)textureEntry + 0x10);
    if (g_Engine_texEnv != env) {
        g_Engine_texEnv = env;
        if (g_Engine_useShaders == 0) {
            glTexEnvf(0x8500, 0x8501, env);
            textureEntry = *(void **)(*(char **)((char *)manager + 0x14) + textureIndex * 4);
        } else if (g_Engine_texEnvDirty != 0) {
            g_Engine_texEnvDirty = 0;
        }
    }
    glBindTexture(*(uint8_t *)((char *)textureEntry + 0x14) == 0 ? 0xde1 : 0x8513, texture);
    *bound = texture;
}
