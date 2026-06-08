#include "class.h"

extern "C" uint8_t g_Engine_useShaders;
extern "C" void esMatrixMultiply(void *out, const void *lhs, const void *rhs);
extern "C" void MatrixGetGL(const Matrix *matrix, float *out);
extern "C" void glLoadMatrixf(const float *matrix);

extern "C" void Engine_SetWorldViewMatrix(Engine *self, const uint32_t *matrix)
{
    void * volatile cookie = __stack_chk_guard;
    if (g_Engine_useShaders != 0) {
        uint32_t gl[16] = {
            matrix[0], matrix[4], matrix[8], 0,
            matrix[1], matrix[5], matrix[9], 0,
            matrix[2], matrix[6], matrix[10], 0,
            matrix[3], matrix[7], matrix[11], 0x3f800000,
        };
        __aeabi_memcpy((char *)self + 0x184, gl, 0x40);
        esMatrixMultiply((char *)self + 0x104, gl, (char *)self + 0x384);
    } else {
        MatrixGetGL((const Matrix *)matrix, (float *)((char *)self + 0x428));
        uint32_t saved0 = (uint32_t)cookie;
        uint32_t current0 = (uint32_t)*(void *volatile *)&__stack_chk_guard;
        uint32_t delta0 = current0 - saved0;
        if (delta0 == 0) {
            return glLoadMatrixf((float *)((char *)self + 0x428));
        }
        __stack_chk_fail(delta0);
    }
    uint32_t saved1 = (uint32_t)cookie;
    uint32_t current1 = (uint32_t)*(void *volatile *)&__stack_chk_guard;
    uint32_t delta1 = current1 - saved1;
    if (delta1 == 0) {
        return;
    }
    __stack_chk_fail(delta1);
}
