#include "class.h"

extern "C" void Engine_LightSetLightPosition(Engine *self, float x, float y, float z,
                                              unsigned int light)
{
    void * volatile cookie = __stack_chk_guard;
    unsigned int index = light - 0x4000;
    if (index < 8) {
        int count = light - 0x3fff;
        int current = *(int *)((char *)self + 0x32c);
        if (current > count) {
            count = current;
        }
        *(int *)((char *)self + 0x32c) = count;

        Vector value;
        value.x = x;
        value.y = y;
        value.z = z;
        *(Vector *)((char *)self + 0x468 + index * 0x0c) = value;
        *(uint32_t *)((char *)self + 0x378 + index * 4) = 0x3f800000;
    }
    if ((uint32_t)__stack_chk_guard - (uint32_t)cookie == 0) {
        return;
    }
    __stack_chk_fail();
}
