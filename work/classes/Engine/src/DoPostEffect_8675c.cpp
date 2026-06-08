#include "class.h"

extern "C" uint8_t g_Engine_useShaders;
extern "C" uint8_t g_Engine_postEffectFlag;
extern "C" int g_Engine_postEffectBW;
extern "C" int g_Engine_shaderPostA;
extern "C" int g_Engine_shaderPostB;
extern "C" int g_Engine_shaderPostC;
extern "C" void Engine_SetPostEffect(Engine *self, uint32_t effect, bool enable);

typedef void ShaderPostDraw(ShaderBaseStruct *, void *);
typedef void ShaderPostDrawSwap(ShaderBaseStruct *, void *, void **, Engine *);

extern "C" void Engine_DoPostEffect(Engine *self)
{
    void * volatile cookie = __stack_chk_guard;
    uint32_t flags = *(uint32_t *)((char *)self + 0x410);
    void *current = *(void **)((char *)self + 0x414);
    void *other = *(void **)((char *)self + 0x418);
    if (g_Engine_useShaders != 0) {
        void *slot = other;
        if ((flags & 2) != 0) {
            ShaderBaseStruct *shader =
                *(ShaderBaseStruct **)(*(char **)((char *)self + 0x514) + g_Engine_shaderPostA * 4);
            void **vtable = *(void ***)shader;
            if ((flags & ~2u) == 0) {
                ((ShaderPostDraw *)vtable[0x14 / 4])(shader, current);
                flags = 0;
            } else {
                ((ShaderPostDrawSwap *)vtable[0x1c / 4])(shader, current, &other, self);
                slot = other;
                other = current;
            }
        }
        if ((*(uint8_t *)((char *)self + 0x410) & 1) != 0) {
            ShaderBaseStruct *shader =
                *(ShaderBaseStruct **)(*(char **)((char *)self + 0x514) + g_Engine_shaderPostB * 4);
            void **vtable = *(void ***)shader;
            if ((flags & ~1u) == 0) {
                ((ShaderPostDraw *)vtable[0x14 / 4])(shader, slot);
                flags = 0;
            } else {
                ((ShaderPostDrawSwap *)vtable[0x1c / 4])(shader, slot, &other, self);
                slot = other;
            }
        }
        if ((*(uint8_t *)((char *)self + 0x410) & 4) != 0) {
            ShaderBaseStruct *shader =
                *(ShaderBaseStruct **)(*(char **)((char *)self + 0x514) + g_Engine_shaderPostC * 4);
            void **vtable = *(void ***)shader;
            if ((flags & ~4u) == 0) {
                ((ShaderPostDraw *)vtable[0x18 / 4])(shader, slot);
            } else {
                ((ShaderPostDrawSwap *)vtable[0x20 / 4])(shader, slot, &other, self);
            }
        }
        if (g_Engine_postEffectFlag == 1) {
            Engine_SetPostEffect(self, g_Engine_postEffectBW, false);
        }
    }
    if ((uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - (uint32_t)(__UINTPTR_TYPE__)cookie == 0) {
        return;
    }
    __stack_chk_fail();
}
