#include "class.h"
#include <arm_neon.h>

extern "C" uint8_t g_Engine_useShaders;
extern "C" void esMatrixMultiply(void *out, const void *lhs, const void *rhs);

extern "C" void Engine_SetOrthoMatrix(Engine *self, const uint32_t *projection,
                                       const uint32_t *view, bool multiply)
{
    void * volatile cookie = __stack_chk_guard;
    if (g_Engine_useShaders != 0) {
        for (int i = 0; i < 16; i += 1) {
            *(uint32_t *)((char *)self + 0x384 + i * 4) = projection[i];
        }
        if (multiply) {
            uint32_t local[16];
            __aeabi_memcpy(local, view, 0x40);
            esMatrixMultiply((char *)self + 0x384, local, (char *)self + 0x384);
        }
    }
    if ((uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - (uint32_t)(__UINTPTR_TYPE__)cookie == 0) {
        return;
    }
    __stack_chk_fail();
}
