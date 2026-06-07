#include "class.h"

extern "C" void *__stack_chk_guard;
extern "C" void __stack_chk_fail(unsigned diff);
extern "C" void AEGeometry_setPosition3(int geom, float x, float y, float z);
extern "C" void AEMath_VectorAssign(void *dst, void *src);
extern "C" void Trail_reset(int trail, int a, int b, int c);
extern "C" int AEGeometry_getMatrix2(int geom);
extern "C" void AEMath_MatrixAssign(void *dst, void *src);

// PlayerFighter::setPosition(float, float, float). r0=self, r1..r3 = x,y,z (raw bits).
extern "C" void PlayerFighter_setPosition3(PlayerFighter *self, int x, int y, int z)
{
    volatile uint32_t stackGuard = (uint32_t)__stack_chk_guard;

    F<int32_t>(self, 0x58) = x;
    F<int32_t>(self, 0x5c) = y;
    F<int32_t>(self, 0x60) = z;

    int stackVec[3];
    AEGeometry_setPosition3(F<int32_t>(self, 8), 0, 0, 0);  // forwards x,y,z via regs
    AEMath_VectorAssign((char *)self + 0x158, stackVec);
    if (F<int32_t>(self, 0x154) != 0) {
        Trail_reset(F<int32_t>(self, 0x154), F<int32_t>(self, 0x158),
                    F<int32_t>(self, 0x15c), F<int32_t>(self, 0x160));
    }
    int m = AEGeometry_getMatrix2(F<int32_t>(self, 8));
    AEMath_MatrixAssign((char *)*(int *)((char *)self + 4) + 4, (void *)m);

    uint32_t diff = (uint32_t)__stack_chk_guard - stackGuard;
    if (diff == 0) {
        return;
    }
    __stack_chk_fail(diff);
}
