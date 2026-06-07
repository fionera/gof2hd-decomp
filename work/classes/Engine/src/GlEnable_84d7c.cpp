#include "class.h"

extern "C" uint8_t g_Engine_useShaders;
extern "C" void glEnable(unsigned int cap);
extern "C" void glDisable(unsigned int cap);

extern "C" void Engine_GlEnable(Engine *self, unsigned int cap, bool enable)
{
    if (g_Engine_useShaders == 0) {
        unsigned int glCap = cap == 0x1000000 ? 0xbc0 : cap;
        if (enable) {
            return glEnable(glCap);
        }
        return glDisable(glCap);
    }

    uint32_t bit = 0;
    if (cap == 0xde1) {
        bit = 1;
    } else if (cap == 0x1000000) {
        bit = 2;
    } else if (cap >= 0x1100000 && cap <= 0x1100024) {
        static const uint32_t bits[] = {
            4, 8, 0x10, 0x20, 0x80, 0x40, 0x100, 0x200, 0x400, 0x1000,
            0, 0, 0, 0, 0, 0, 0x2000, 0x4000, 0x8000, 0x10000, 0x20000,
            0x40000, 0x80000, 0x100000, 0x200000, 0x400000, 0, 0, 0, 0, 0,
            0, 0x800000, 0x1000000, 0x2000000, 0x4000000, 0x8000000,
        };
        bit = bits[cap - 0x1100000];
    }
    if (bit == 0) {
        return;
    }
    uint32_t flags = *(uint32_t *)((char *)self + 0x420);
    flags = enable ? (flags | bit) : (flags & ~bit);
    *(uint32_t *)((char *)self + 0x420) = flags;
}
