#include "class.h"

void *operator new(__SIZE_TYPE__ size);

extern "C" void FBOContainer_ctor(FBOContainer *self, Engine *engine, String *name);
extern "C" void FBOContainer_Create(FBOContainer *self, unsigned int width, unsigned int height,
                                    bool depth, bool stencil);

namespace AbyssEngine {

void BloomShader::InternalInit(Engine *engine)
{
    void *volatile cookie = __stack_chk_guard;

    FBOContainer *fbo = (FBOContainer *)operator new(0x38);
    String luma("BloomShader fboLuma", false);
    FBOContainer_ctor(fbo, engine, &luma);
    *(FBOContainer **)((char *)this + 0x34) = fbo;
    luma.~String();
    FBOContainer_Create(*(FBOContainer **)((char *)this + 0x34), 0x100, 0x100, true, false);

    fbo = (FBOContainer *)operator new(0x38);
    String blurH("BloomShader fboBlurH", false);
    FBOContainer_ctor(fbo, engine, &blurH);
    *(FBOContainer **)((char *)this + 0x50) = fbo;
    blurH.~String();
    FBOContainer_Create(*(FBOContainer **)((char *)this + 0x50), 0x100, 0x100, true, false);

    fbo = (FBOContainer *)operator new(0x38);
    String blurV("BloomShader fboBlurV", false);
    FBOContainer_ctor(fbo, engine, &blurV);
    *(FBOContainer **)((char *)this + 0x6c) = fbo;
    blurV.~String();
    FBOContainer_Create(*(FBOContainer **)((char *)this + 0x6c), 0x100, 0x100, true, false);

    fbo = (FBOContainer *)operator new(0x38);
    String black("BloomShader fboBlack", false);
    FBOContainer_ctor(fbo, engine, &black);
    *(FBOContainer **)((char *)this + 0x70) = fbo;
    black.~String();
    FBOContainer_Create(*(FBOContainer **)((char *)this + 0x70), 0x100, 0x100, true, false);

    unsigned int guardDelta = (unsigned int)__stack_chk_guard - (unsigned int)cookie;
    if (guardDelta == 0) {
        return;
    }
    __stack_chk_fail(guardDelta);
}

} // namespace AbyssEngine
