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
    void *g78 = self->f_78;
    if (g78 != 0 && self->f_1b8 == 0) {
        AEGeometry_render(g78);
    }
    int iVar1 = self->f_88;
    if (iVar1 == 5) {
LAB_538:
        if (self->f_1b8 != 0) return;
        return render_thunk_state5(self->f_8);
    }
    if (iVar1 == 4) {
        if (self->f_1b8 == 0) AEGeometry_render(self->f_124);
        iVar1 = self->f_18c;
        if (iVar1 == 0) return;
    } else {
        if (iVar1 != 3) {
            if (Player_isActive(self->f_4) == 0) return;
            goto LAB_538;
        }
        if (self->f_1b8 == 0) AEGeometry_render(self->f_124);
        iVar1 = self->f_18c;
    }
    return render_thunk_other(iVar1);
}
