#include "class.h"

extern "C" uint8_t g_Engine_useShaders;
extern "C" void AEMath_MatrixInverseRotateVector(Vector *out, const Matrix *matrix, const Vector *value);
extern "C" void AEMath_VectorNormalize(Vector *result, const Vector *value);
extern "C" void AEMath_MatrixInverseTransformVector(Vector *out, const Matrix *matrix, const Vector *value);
extern "C" void Engine_ShaderUpdate(Engine *self);

extern "C" void Engine_SetModelMatrix(Engine *self, const uint32_t *matrix)
{
    void * volatile cookie = __stack_chk_guard;
    if (g_Engine_useShaders != 0) {
        *(uint32_t *)((char *)self + 0x204) = matrix[0];
        *(uint32_t *)((char *)self + 0x208) = matrix[4];
        *(uint32_t *)((char *)self + 0x20c) = matrix[8];
        *(uint32_t *)((char *)self + 0x210) = matrix[1];
        *(uint32_t *)((char *)self + 0x214) = matrix[5];
        *(uint32_t *)((char *)self + 0x218) = matrix[9];
        *(uint32_t *)((char *)self + 0x21c) = matrix[2];
        *(uint32_t *)((char *)self + 0x220) = matrix[6];
        *(uint32_t *)((char *)self + 0x224) = matrix[10];
        uint32_t gl[16] = {
            matrix[0], matrix[4], matrix[8], 0,
            matrix[1], matrix[5], matrix[9], 0,
            matrix[2], matrix[6], matrix[10], 0,
            matrix[3], matrix[7], matrix[11], 0x3f800000,
        };
        __aeabi_memcpy((char *)self + 0x144, gl, 0x40);
        Vector tmp;
        if (*(float *)((char *)self + 0x378) == 0.0f) {
            AEMath_MatrixInverseRotateVector(&tmp, (const Matrix *)matrix,
                                             (const Vector *)((char *)self + 0x468));
            AEMath_VectorNormalize(&tmp, &tmp);
            *(Vector *)((char *)self + 0x330) = tmp;
        } else {
            *(Vector *)((char *)self + 0x330) = *(Vector *)((char *)self + 0x468);
        }
        if (*(int *)((char *)self + 0x32c) > 1) {
            if (*(float *)((char *)self + 0x37c) == 0.0f) {
                AEMath_MatrixInverseRotateVector(&tmp, (const Matrix *)matrix,
                                                 (const Vector *)((char *)self + 0x474));
                AEMath_VectorNormalize(&tmp, &tmp);
                *(Vector *)((char *)self + 0x33c) = tmp;
            } else {
                *(Vector *)((char *)self + 0x33c) = *(Vector *)((char *)self + 0x474);
            }
        }
        Engine_ShaderUpdate(self);
        AEMath_MatrixInverseTransformVector(&tmp, (const Matrix *)matrix,
                                            (const Vector *)((char *)self + 0x3fc));
        *(Vector *)((char *)self + 0x34c) = tmp;
        *(float *)((char *)self + 0x34c) /= *(float *)(matrix + 12);
        *(float *)((char *)self + 0x350) /= *(float *)(matrix + 13);
        *(float *)((char *)self + 0x354) /= *(float *)(matrix + 14);
    }
    if ((uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - (uint32_t)(__UINTPTR_TYPE__)cookie == 0) {
        return;
    }
    __stack_chk_fail();
}
