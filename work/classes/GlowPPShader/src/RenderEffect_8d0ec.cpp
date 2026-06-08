#include "class.h"

typedef void RenderEffectFn(GlowPPShader *, FBOContainer *, FBOContainer **, Engine *);

static inline uint32_t stack_guard_diff(uint32_t saved, uint32_t current)
{
    return current - saved;
}

extern "C" void GlowPPShader_RenderEffect_simple(GlowPPShader *self, FBOContainer *source, Engine *engine)
{
    void * volatile cookie = __stack_chk_guard;
    FBOContainer *target = 0;
    void **vtable = *(void ***)self;
    ((RenderEffectFn *)vtable[0x1c / 4])(self, source, &target, engine);
    uint32_t saved = (uint32_t)(__UINTPTR_TYPE__)cookie;
    uint32_t diff = stack_guard_diff(saved, (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard);
    if (diff == 0) {
        return;
    }
    __stack_chk_fail(diff);
}
