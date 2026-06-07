#include "class.h"

// KIPlayer::KIPlayer(this, type, -1, Player*, AEGeometry*, float, float, float, 0) -> this
extern "C" void *KIPlayer_ctor(void *self, int type, int minusOne, Player *owner,
                               AEGeometry *geom, float a, float b, float c, int zero); // 0x75790

// Vtable base, accessed PC-relative; the live vtable pointer is (vtbl base) + 8.
__attribute__((visibility("hidden"))) extern int g_PlayerJunk_vtbl;

// PlayerJunk::PlayerJunk(int, Player*, AEGeometry*, float, float, float)
extern "C" void _ZN10PlayerJunkC1EiP6PlayerP10AEGeometryfff(
        void *self, int p1, Player *p2, AEGeometry *p3, float p4, float p5, float p6) {
    void *base = KIPlayer_ctor(self, p1, -1, p2, p3, p4, p5, p6, 0);
    F<uint8_t>(base, 0x3d) = 1;
    F<int>(base, 0x0) = g_PlayerJunk_vtbl + 8;
}
