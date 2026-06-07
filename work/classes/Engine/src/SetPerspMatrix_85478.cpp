#include "class.h"

extern "C" uint8_t g_Engine_useShaders;

extern "C" void Engine_SetPerspMatrix(Engine *self, const uint32_t *matrix)
{
    if (g_Engine_useShaders == 0) {
        return;
    }
    *(uint32_t *)((char *)self + 0x384) = matrix[0];
    *(uint32_t *)((char *)self + 0x388) = matrix[1];
    *(uint32_t *)((char *)self + 0x38c) = matrix[2];
    *(uint32_t *)((char *)self + 0x390) = matrix[3];
    *(uint32_t *)((char *)self + 0x394) = matrix[4];
    *(uint32_t *)((char *)self + 0x398) = matrix[5];
    *(uint32_t *)((char *)self + 0x39c) = matrix[6];
    *(uint32_t *)((char *)self + 0x3a0) = matrix[7];
    *(uint32_t *)((char *)self + 0x3a4) = matrix[8];
    *(uint32_t *)((char *)self + 0x3a8) = matrix[9];
    *(uint32_t *)((char *)self + 0x3ac) = matrix[10];
    *(uint32_t *)((char *)self + 0x3b0) = matrix[11];
    *(uint32_t *)((char *)self + 0x3b4) = matrix[12];
    *(uint32_t *)((char *)self + 0x3b8) = matrix[13];
    *(uint32_t *)((char *)self + 0x3bc) = matrix[14];
    *(uint32_t *)((char *)self + 0x3c0) = matrix[15];
}
