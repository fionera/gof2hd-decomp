#include "class.h"

// Tail-call thunk; arg is this->0x18 when this->0x58 == 1.
extern "C" void HangarWindow_render3D_thunk(void *arg);

extern "C" void HangarWindow_render3D(HangarWindow *self)
{
    if (self->f_58 == 1) {
        return HangarWindow_render3D_thunk(self->f_18);
    }
}
