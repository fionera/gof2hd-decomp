#include "class.h"

extern "C" void AEGeometry_render(void *geom);
extern "C" int Player_isActive(void *player);
// Tail-call thunks selected by object state.
extern "C" void render_thunk_state5(void *geom);   // DAT_001abdd4 thunk, arg = this->0x8
extern "C" void render_thunk_other(int meshId);     // DAT_001ac2b4 thunk, arg = this->0x18c

// NEAR: clang reorders the state comparisons (3 before 5) and duplicates the shared
// state5 block instead of sharing it via the original goto; structure differs.
extern "C" void PlayerFixedObject_render(PlayerFixedObject *self)
{
    void *g78 = F<void *>(self, 0x78);
    if (g78 != 0 && F<uint8_t>(self, 0x1b8) == 0) {
        AEGeometry_render(g78);
    }
    int iVar1 = F<int>(self, 0x88);
    if (iVar1 == 5) {
LAB_538:
        if (F<uint8_t>(self, 0x1b8) != 0) return;
        return render_thunk_state5(F<void *>(self, 0x8));
    }
    if (iVar1 == 4) {
        if (F<uint8_t>(self, 0x1b8) == 0) AEGeometry_render(F<void *>(self, 0x124));
        iVar1 = F<int>(self, 0x18c);
        if (iVar1 == 0) return;
    } else {
        if (iVar1 != 3) {
            if (Player_isActive(F<void *>(self, 0x4)) == 0) return;
            goto LAB_538;
        }
        if (F<uint8_t>(self, 0x1b8) == 0) AEGeometry_render(F<void *>(self, 0x124));
        iVar1 = F<int>(self, 0x18c);
    }
    return render_thunk_other(iVar1);
}
