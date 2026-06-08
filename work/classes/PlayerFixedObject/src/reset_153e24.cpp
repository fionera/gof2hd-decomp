#include "class.h"
typedef unsigned long long uint64_t;

extern "C" void KIPlayer_reset(PlayerFixedObject *self);
extern "C" void *Vector_assign(Vector *dst, Vector *src);

typedef void (*SetPosFn)(PlayerFixedObject *, float, float, float);

// PC-relative pointer-to-function-pointer; *holder is a Vector::operator= style routine
// invoked as fn(Vector*, Vector*).
typedef void (*VecAssignFn)(void *dst, void *src);
__attribute__((visibility("hidden"))) extern VecAssignFn *g_pfo_vecAssignZero;

extern "C" void PlayerFixedObject_reset(PlayerFixedObject *self)
{
    KIPlayer_reset(self);

    // vtable slot 0x48 -> setPosition(this->0x58, 0x5c, 0x60)
    SetPosFn setPos = *(SetPosFn *)(*(char **)self + 0x48);
    setPos(self, F<float>(self, 0x58), F<float>(self, 0x5c), F<float>(self, 0x60));

    F<int32_t>(self, 0x168) = 0;

    // Vector copy: 0x58 -> 0x138
    {
        char buf[12];
        *(uint64_t *)buf = *(uint64_t *)((char *)self + 0x58);
        *(uint32_t *)(buf + 8) = F<uint32_t>(self, 0x60);
        Vector_assign((Vector *)((char *)self + 0x138), (Vector *)buf);
    }
    // Vector copy: 0x138 -> 0x2c
    {
        char buf[12];
        *(uint64_t *)buf = *(uint64_t *)((char *)self + 0x138);
        *(uint32_t *)(buf + 8) = F<uint32_t>(self, 0x140);
        Vector_assign((Vector *)((char *)self + 0x2c), (Vector *)buf);
    }

    F<int32_t>(self, 0x130) = 0;
    if (F<int32_t>(self, 0x88) != 5)
        F<int32_t>(self, 0x88) = 0;

    VecAssignFn fn = *g_pfo_vecAssignZero;
    char zero[12];
    *(uint32_t *)(zero + 0) = 0;
    *(uint32_t *)(zero + 4) = 0;
    *(uint32_t *)(zero + 8) = 0;
    fn((char *)self + 0x90, zero);
    *(uint32_t *)(zero + 0) = 0;
    *(uint32_t *)(zero + 4) = 0;
    *(uint32_t *)(zero + 8) = 0;
    fn((char *)self + 0x144, zero);
    *(uint32_t *)(zero + 0) = 0;
    *(uint32_t *)(zero + 4) = 0;
    *(uint32_t *)(zero + 8) = 0;
    fn((char *)self + 0x150, zero);
}
