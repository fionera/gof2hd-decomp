#include "class.h"

extern "C" uint8_t g_Engine_useShaders;
extern "C" void glLightfv(unsigned int light, unsigned int pname, const void *params);
extern "C" void glMaterialfv(unsigned int face, unsigned int pname, const void *params);
extern "C" void glMaterialf(unsigned int face, unsigned int pname, float value);

extern "C" void Engine_ResetLightParam(Engine *self)
{
    void * volatile cookie = __stack_chk_guard;
    *(uint32_t *)((char *)self + 0x488) = 0x3f800000;
    *(uint32_t *)((char *)self + 0x32c) = 1;
    *(float *)((char *)self + 0x298) = 0.8f;
    *(float *)((char *)self + 0x29c) = 0.8f;
    *(float *)((char *)self + 0x2a0) = 0.8f;
    *(uint32_t *)((char *)self + 0x2a4) = 0x3f800000;
    *(float *)((char *)self + 0x2a8) = 0.2f;
    *(float *)((char *)self + 0x2ac) = 0.2f;
    *(float *)((char *)self + 0x2b0) = 0.2f;
    *(uint32_t *)((char *)self + 0x2b4) = 0x3f800000;
    *(uint64_t *)((char *)self + 0x2b8) = 0;
    *(uint64_t *)((char *)self + 0x2c0) = 0x3f800000ULL;
    *(uint32_t *)((char *)self + 0x2c8) = 0;
    *(uint64_t *)((char *)self + 0x268) = 0;
    *(uint32_t *)((char *)self + 0x270) = 0;
    *(uint32_t *)((char *)self + 0x274) = 0x3f800000;

    Vector up;
    up.x = 0.0f;
    up.y = 1.0f;
    up.z = 0.0f;
    *(Vector *)((char *)self + 0x468) = up;
    *(Vector *)((char *)self + 0x474) = up;
    *(uint32_t *)((char *)self + 0x378) = 0;
    *(uint32_t *)((char *)self + 0x37c) = 0;

    if (g_Engine_useShaders == 0) {
        glLightfv(0x4000, 0x1200, (char *)self + 0x268);
        glLightfv(0x4000, 0x1201, (char *)self + 0x228);
        glLightfv(0x4000, 0x1202, (char *)self + 0x248);
        glMaterialfv(0x408, 0x1200, (char *)self + 0x2a8);
        glMaterialfv(0x408, 0x1201, (char *)self + 0x298);
        glMaterialfv(0x408, 0x1202, (char *)self + 0x2b8);
        glMaterialf(0x408, 0x1601, *(float *)((char *)self + 0x2c8));
    }
    if ((uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - (uint32_t)(__UINTPTR_TYPE__)cookie == 0) {
        return;
    }
    __stack_chk_fail();
}
