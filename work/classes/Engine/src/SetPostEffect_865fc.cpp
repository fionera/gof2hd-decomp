#include "class.h"

extern "C" void String_ctor_char(String *self, const char *text, bool copy);
extern "C" void String_dtor(String *self);
extern "C" void FBOContainer_ctor(FBOContainer *self, Engine *engine, String *name);
extern "C" void FBOContainer_Create(FBOContainer *self, int width, int height, bool depth, bool color);

extern "C" int g_Engine_postEffectBlur;
extern "C" int g_Engine_postEffectBW;
extern "C" uint8_t g_Engine_postEffectFlag;
extern "C" int g_Engine_postEffectCounter;
extern "C" int g_Engine_postEffectPending;

extern "C" void Engine_SetPostEffect(Engine *self, uint32_t effect, bool enable)
{
    void * volatile cookie = __stack_chk_guard;
    if (*(void **)((char *)self + 0x414) == 0 && enable) {
        FBOContainer *fbo = (FBOContainer *)operator new(0x38);
        char nameStorage[sizeof(String)];
        String *name = (String *)nameStorage;
        String_ctor_char(name, "posteffect", false);
        FBOContainer_ctor(fbo, self, name);
        *(FBOContainer **)((char *)self + 0x414) = fbo;
        String_dtor(name);
        int width;
        int height;
        if (*(int *)(**(char ***)((char *)self + 0x30) + 0x30) == 2) {
            width = *(int *)((char *)self + 0x368);
            height = *(int *)((char *)self + 0x36c);
        } else {
            width = *(int *)((char *)self + 0x36c);
            height = *(int *)((char *)self + 0x368);
        }
        FBOContainer_Create(*(FBOContainer **)((char *)self + 0x414), width, height, false, true);
    }

    uint32_t flags = *(uint32_t *)((char *)self + 0x410);
    if (effect == (uint32_t)g_Engine_postEffectBW) {
        if (enable) {
            if (g_Engine_postEffectCounter > 0) {
                g_Engine_postEffectCounter -= 1;
            } else {
                flags |= 4;
            }
        } else if (g_Engine_postEffectFlag == 1) {
            flags &= ~4u;
            g_Engine_postEffectFlag = 0;
            g_Engine_postEffectPending = 1;
        } else {
            g_Engine_postEffectFlag = g_Engine_postEffectCounter < 1;
        }
    } else if (effect == (uint32_t)g_Engine_postEffectBlur) {
        g_Engine_postEffectFlag = enable;
        flags = enable ? (flags | 2) : (flags & ~2u);
    } else if (effect == 0x1400000) {
        flags = enable ? (flags | 1) : (flags & ~1u);
    }
    *(uint32_t *)((char *)self + 0x410) = flags;
    if ((uint32_t)__stack_chk_guard - (uint32_t)cookie == 0) {
        return;
    }
    __stack_chk_fail();
}
