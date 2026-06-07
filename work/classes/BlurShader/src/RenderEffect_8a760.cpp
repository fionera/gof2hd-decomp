#include "class.h"

typedef void FBOEffect(FBOContainer *, Engine *, void *, uint32_t, uint32_t, uint32_t, uint32_t,
                       uint32_t);

extern "C" void BlurShader_RenderEffect(FBOContainer *fbo, Engine *engine, uint32_t amount,
                                        uint32_t x, uint32_t y, uint32_t z, uint32_t w)
{
    void *volatile cookie = __stack_chk_guard;
    uint32_t zero = 0;
    void **vtable = *(void ***)fbo;
    ((FBOEffect *)vtable[0x20 / 4])(fbo, engine, &zero, amount, x, y, z, w);
    uint32_t guardDelta =
        stack_guard_delta(*(volatile uint32_t *)&cookie, (uint32_t)__stack_chk_guard);
    if (guardDelta == 0) {
        return;
    }
    __stack_chk_fail(guardDelta);
}
