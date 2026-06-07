#include "class.h"
#include <arm_neon.h>

extern "C" uint8_t g_Engine_useShaders;
extern "C" void glMatrixMode(unsigned int mode);
extern "C" void glLoadIdentity();
extern "C" void glScalef(float x, float y, float z);

extern "C" void Engine_ResetUVMatrix(Engine *self)
{
    if (g_Engine_useShaders != 0) {
        uint32_t one = 0x3f800000;
        uint32x4_t zero = vdupq_n_u32(0);
        *(volatile uint32_t *)((char *)self + 0x1c4) = one;
        *(volatile uint32_t *)((char *)self + 0x1d8) = one;
        *(volatile uint32_t *)((char *)self + 0x1ec) = one;
        *(volatile uint32_t *)((char *)self + 0x200) = one;
        vst1q_u32((uint32_t *)((char *)self + 0x1c8), zero);
        vst1q_u32((uint32_t *)((char *)self + 0x1dc), zero);
        vst1q_u32((uint32_t *)((char *)self + 0x1f0), zero);
        return;
    }

    glMatrixMode(0x1702);
    glLoadIdentity();
    glScalef(1.0f, -1.0f, 1.0f);
    return glMatrixMode(0x1700);
}
