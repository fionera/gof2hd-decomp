#include "class.h"

extern "C" void Vector_assign(Vector *dst, const Vector *src);   // 0x6eb3c

extern "C" uint64_t Engine_SetEyePosition(Engine *self, uint32_t x, uint32_t y, uint32_t z)
{
    void * volatile cookie = __stack_chk_guard;
    uint32_t buf[3];
    buf[0] = x;
    buf[1] = y;
    buf[2] = z;
    Vector_assign((Vector *)((char *)self + 0x3fc), (const Vector *)buf);
    uint32_t saved = (uint32_t)cookie;
    uint32_t current = (uint32_t)*(void *volatile *)&__stack_chk_guard;
    uint32_t guardDelta = current - saved;
    if (guardDelta == 0) {
        return (uint64_t)buf[0] | ((uint64_t)buf[1] << 32);
    }
    __stack_chk_fail(guardDelta);
}
