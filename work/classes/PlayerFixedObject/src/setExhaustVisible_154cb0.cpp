#include "class.h"

extern "C" void *PaintCanvas_TransformGetTransform(void *canvas);
extern "C" void Transform_setExhaustVisible(void *transform, bool v);

// PaintCanvas singleton holder (single pc-rel deref -> holder; object is *holder).
__attribute__((visibility("hidden"))) extern void **g_pfo_canvas;

// NEAR: target keeps the frame up-front (no shrink-wrap, bool saved to r4 before the
// field checks). clang shrink-wraps here (checks before push, bx lr early-exit).
extern "C" void PlayerFixedObject_setExhaustVisible(PlayerFixedObject *self, bool v)
{
    void *geom = self->f_8;
    if (geom != 0 && *(int *)((char *)geom + 0x14) != -1) {
        void **holder = g_pfo_canvas;
        return Transform_setExhaustVisible(PaintCanvas_TransformGetTransform(*holder), v);
    }
}
