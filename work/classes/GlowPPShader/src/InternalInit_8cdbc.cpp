#include "class.h"

extern "C" void *operator_new_0(uint32_t size);
extern "C" void *operator_new_1(uint32_t size);
extern "C" void *operator_new_2(uint32_t size);
extern "C" void *operator_new_3(uint32_t size);
extern "C" void String_ctor_char_0(String *self, const char *text, bool copy);
extern "C" void String_ctor_char_1(String *self, const char *text, bool copy);
extern "C" void String_ctor_char_2(String *self, const char *text, bool copy);
extern "C" void String_ctor_char_3(String *self, const char *text, bool copy);
extern "C" void FBOContainer_ctor_0(FBOContainer *self, Engine *engine, String *name);
extern "C" void FBOContainer_ctor_1(FBOContainer *self, Engine *engine, String *name);
extern "C" void FBOContainer_ctor_2(FBOContainer *self, Engine *engine, String *name);
extern "C" void FBOContainer_ctor_3(FBOContainer *self, Engine *engine, String *name);
extern "C" void String_dtor_0(String *self);
extern "C" void String_dtor_1(String *self);
extern "C" void String_dtor_2(String *self);
extern "C" void String_dtor_3(String *self);
extern "C" void FBOContainer_Create_0(FBOContainer *self, uint32_t width, uint32_t height, bool depth, bool color);
extern "C" void FBOContainer_Create_1(FBOContainer *self, uint32_t width, uint32_t height, bool depth, bool color);
extern "C" void FBOContainer_Create_2(FBOContainer *self, uint32_t width, uint32_t height, bool depth, bool color);
extern "C" void FBOContainer_Create_3(FBOContainer *self, uint32_t width, uint32_t height, bool depth, bool color);

extern "C" void GlowPPShader_InternalInit(GlowPPShader *self, Engine *engine)
{
    void * volatile cookie = __stack_chk_guard;
    String name0;
    String name1;
    String name2;
    String name3;

    FBOContainer *fbo = (FBOContainer *)operator_new_0(0x38);
    String_ctor_char_0(&name0, "GlowPPShader0", false);
    FBOContainer_ctor_0(fbo, engine, &name0);
    *(FBOContainer **)((char *)self + 0x3c) = fbo;
    String_dtor_0(&name0);
    FBOContainer_Create_0(*(FBOContainer **)((char *)self + 0x3c), 0x200, 0x200, true, false);

    fbo = (FBOContainer *)operator_new_1(0x38);
    String_ctor_char_1(&name1, "GlowPPShader1", false);
    FBOContainer_ctor_1(fbo, engine, &name1);
    *(FBOContainer **)((char *)self + 0x58) = fbo;
    String_dtor_1(&name1);
    FBOContainer_Create_1(*(FBOContainer **)((char *)self + 0x58), 0x200, 0x200, true, false);

    fbo = (FBOContainer *)operator_new_2(0x38);
    String_ctor_char_2(&name2, "GlowPPShader2", false);
    FBOContainer_ctor_2(fbo, engine, &name2);
    *(FBOContainer **)((char *)self + 0x74) = fbo;
    String_dtor_2(&name2);
    FBOContainer_Create_2(*(FBOContainer **)((char *)self + 0x74), 0x200, 0x200, true, false);

    fbo = (FBOContainer *)operator_new_3(0x38);
    String_ctor_char_3(&name3, "GlowPPShader3", false);
    FBOContainer_ctor_3(fbo, engine, &name3);
    *(FBOContainer **)((char *)self + 0xa0) = fbo;
    String_dtor_3(&name3);
    FBOContainer_Create_3(*(FBOContainer **)((char *)self + 0xa0), 0x200, 0x200, true, false);

    uint32_t diff = (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - (uint32_t)(__UINTPTR_TYPE__)cookie;
    if (diff == 0) {
        return;
    }
    __stack_chk_fail(diff);
}
