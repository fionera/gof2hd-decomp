#include "class.h"

struct Level;
extern "C" void KIPlayer_setLevel(PlayerFighter *self, Level *lvl);
// Three function pointers fetched once from hidden PC-relative globals (each a ptr-to-fnptr).
extern void *const gSL_f1 __attribute__((visibility("hidden")));
extern void *const gSL_f2 __attribute__((visibility("hidden")));
extern void *const gSL_f3 __attribute__((visibility("hidden")));

typedef int (*F1)(int geom);
typedef int (*F2)(int base, int v, int kind, int z);
typedef void (*F3)(int base, int v, int z);

extern "C" void PlayerFighter_setLevel(PlayerFighter *self, Level *lvl)
{
    KIPlayer_setLevel(self, lvl);
    F1 f1 = (F1)gSL_f1;
    F2 f2 = (F2)gSL_f2;
    F3 f3 = (F3)gSL_f3;
    int base = F<int32_t>(self, 0x54);

    int v;
    v = f2(*(int *)(base + 0x74), f1(F<int32_t>(self, 8)), 9, 0);
    F<int32_t>(self, 0x1a0) = v;
    f3(*(int *)(F<int32_t>(self, 0x54) + 0x74), v, 0);

    v = f2(*(int *)(F<int32_t>(self, 0x54) + 0x78), f1(F<int32_t>(self, 8)), 0xf, 0);
    F<int32_t>(self, 0x80) = v;
    f3(*(int *)(F<int32_t>(self, 0x54) + 0x78), v, 0);

    v = f2(*(int *)(F<int32_t>(self, 0x54) + 0x84), f1(F<int32_t>(self, 8)), 0x2a, 0);
    F<int32_t>(self, 0x84) = v;
    f3(*(int *)(F<int32_t>(self, 0x54) + 0x84), v, 0);

    v = f2(*(int *)(F<int32_t>(self, 0x54) + 0x8c), f1(F<int32_t>(self, 8)), 0x11, 0);
    F<int32_t>(self, 0x134) = v;
    f3(*(int *)(F<int32_t>(self, 0x54) + 0x8c), v, 0);

    v = f2(*(int *)(F<int32_t>(self, 0x54) + 0x8c), f1(F<int32_t>(self, 8)), 0x12, 0);
    F<int32_t>(self, 0x138) = v;
    return f3(*(int *)(F<int32_t>(self, 0x54) + 0x8c), v, 0);
}
