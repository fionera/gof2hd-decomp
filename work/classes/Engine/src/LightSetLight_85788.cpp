#include "class.h"

extern "C" uint8_t g_Engine_useShaders;
extern "C" void glLightfv(unsigned int light, unsigned int pname, const void *params);

extern "C" void Engine_LightSetLight(Engine *self, unsigned int light)
{
    void * volatile cookie = __stack_chk_guard;
    uint32_t values[4] = {0, 0, 0, 0};
    unsigned int index = light - 0x4000;
    if (index < 8) {
        int count = light - 0x3fff;
        int current = *(int *)((char *)self + 0x32c);
        if (current > count) {
            count = current;
        }
        *(int *)((char *)self + 0x32c) = count;

        char *vector = (char *)self + 0x468 + index * 0x0c;
        values[0] = *(uint32_t *)(vector + 0x0);
        values[1] = *(uint32_t *)(vector + 0x4);
        values[2] = *(uint32_t *)(vector + 0x8);
        values[3] = *(uint32_t *)((char *)self + 0x378 + index * 4);
        if (g_Engine_useShaders == 0) {
            glLightfv(light, 0x1203, values);
        }
    }
    if ((uint32_t)__stack_chk_guard - (uint32_t)cookie == 0) {
        return;
    }
    __stack_chk_fail();
}
