#include "class.h"

extern "C" void *__stack_chk_guard;
extern "C" void __stack_chk_fail(unsigned diff);
extern "C" void KIPlayer_reset(PlayerFighter *self);
extern "C" void AEMath_VectorAssign(void *dst, void *src);
// hidden PC-relative function-pointer global (Vector zero-set helper), used 3x.
extern void *const gReset_vfn __attribute__((visibility("hidden")));

typedef void (*VFn)(void *dst, void *zeroVec);

__attribute__((minsize)) extern "C" void PlayerFighter_reset(PlayerFighter *self)
{
    volatile uint32_t stackGuard = (uint32_t)__stack_chk_guard;

    KIPlayer_reset(self);
    F<uint8_t>(self, 0x4c) = 1;

    int v[3];
    v[0] = F<int32_t>(self, 0x58);
    v[1] = F<int32_t>(self, 0x5c);
    v[2] = F<int32_t>(self, 0x60);
    AEMath_VectorAssign((char *)self + 0x158, v);
    AEMath_VectorAssign((char *)self + 0x2c, v);

    F<int32_t>(self, 0x1d0) = 0;
    F<int32_t>(self, 0x1d4) = 0;
    F<int32_t>(self, 0x38) = 0;
    F<uint8_t>(self, 0x12e) = 0;
    F<int32_t>(self, 0x148) = 0;
    F<uint8_t>(self, 0x12c) = 0;
    F<int32_t>(self, 0x1b8) = 0;
    F<int32_t>(self, 0x1c0) = 0;
    F<int32_t>(self, 0x1c4) = 0;
    F<int32_t>(self, 0x1c8) = 0;
    if (F<int32_t>(self, 0x88) != 5) {
        F<int32_t>(self, 0x88) = 0;
    }

    VFn vfn = (VFn)gReset_vfn;
    int z[3] = {0, 0, 0};
    vfn((char *)self + 0x90, z);
    vfn((char *)self + 0x164, z);
    vfn((char *)self + 0x170, z);

    F<uint8_t>(self, 0xd0) = 0;
    F<uint16_t>(self, 0x68) = 0;
    F<uint8_t>(self, 0x6a) = 0;
    F<uint8_t>(self, 0x4c) = 1;
    F<int32_t>(self, 0x140) = 0;
    F<uint8_t>(self, 0x1fc) = 0;
    F<uint8_t>(self, 0x13c) = 0;
    F<uint8_t>(self, 0x2d0) = 0;
    F<int32_t>(self, 0x2c8) = 0;
    F<int32_t>(self, 0x2cc) = 0;
    F<int32_t>(self, 0x2d4) = 0;
    F<uint8_t>(self, 0x2d8) = 1;
    F<uint8_t>(self, 0x2e4) = 0;

    uint32_t diff = (uint32_t)__stack_chk_guard - stackGuard;
    if (diff == 0) {
        return;
    }
    __stack_chk_fail(diff);
}
