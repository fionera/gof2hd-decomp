#include "class.h"

extern "C" void AEMath_VectorNormalize(Vector *result, const Vector *value);

extern "C" void Engine_LightSetLightDirection(Engine *self, float x, float y, float z,
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

        Vector input;
        input.x = x;
        input.y = y;
        input.z = z;
        Vector normalized;
        AEMath_VectorNormalize(&normalized, &input);
        *(Vector *)((char *)self + 0x468 + index * 0x0c) = normalized;
        *(uint32_t *)((char *)self + 0x378 + index * 4) = 0;
    }
    if ((uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - (uint32_t)(__UINTPTR_TYPE__)cookie == 0) {
        return;
    }
    __stack_chk_fail();
}
