#include "class.h"

extern "C" uint8_t g_Engine_useShaders;
extern "C" void glMatrixMode(unsigned int mode);
extern "C" void MatrixGetGL(const Matrix *matrix, float *out);
extern "C" void glLoadMatrixf(const float *matrix);

extern "C" void Engine_SetUVMatrix(Engine *self, const uint32_t *matrix)
{
    if (g_Engine_useShaders == 0) {
        glMatrixMode(0x1702);
        MatrixGetGL((const Matrix *)matrix, (float *)((char *)self + 0x428));
        glLoadMatrixf((float *)((char *)self + 0x428));
        return glMatrixMode(0x1700);
    }

    uint32_t m0 = matrix[0];
    uint32_t m1 = matrix[1];
    uint32_t m2 = matrix[2];
    uint32_t m3 = matrix[3];
    uint32_t m4 = matrix[4];
    uint32_t m5 = matrix[5];
    uint32_t m6 = matrix[6];
    uint32_t m7 = matrix[7];
    uint32_t m8 = matrix[8];
    uint32_t m9 = matrix[9];
    uint32_t m10 = matrix[10];
    uint32_t m11 = matrix[11];

    *(uint32_t *)((char *)self + 0x1c4) = m0;
    *(uint32_t *)((char *)self + 0x1c8) = m4;
    *(uint32_t *)((char *)self + 0x1cc) = m8;
    *(uint32_t *)((char *)self + 0x1d0) = 0;
    *(uint32_t *)((char *)self + 0x1d4) = m1;
    *(uint32_t *)((char *)self + 0x1d8) = m5;
    *(uint32_t *)((char *)self + 0x1dc) = m9;
    *(uint32_t *)((char *)self + 0x1e0) = 0;
    *(uint32_t *)((char *)self + 0x1e4) = m2;
    *(uint32_t *)((char *)self + 0x1e8) = m6;
    *(uint32_t *)((char *)self + 0x1ec) = m10;
    *(uint32_t *)((char *)self + 0x1f0) = 0;
    *(uint32_t *)((char *)self + 0x1f4) = m3;
    *(uint32_t *)((char *)self + 0x1f8) = m7;
    *(uint32_t *)((char *)self + 0x1fc) = m11;
    *(uint32_t *)((char *)self + 0x200) = 0x3f800000;
}
