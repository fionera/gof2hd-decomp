#include "class.h"

extern "C" void Engine_SetEyePosition(Engine *self, float x, float y, float z)
{
    void * volatile cookie = __stack_chk_guard;
    Vector value;
    value.x = x;
    value.y = y;
    value.z = z;
    *(Vector *)((char *)self + 0x3fc) = value;
    if ((uint32_t)__stack_chk_guard - (uint32_t)cookie == 0) {
        return;
    }
    __stack_chk_fail();
}
