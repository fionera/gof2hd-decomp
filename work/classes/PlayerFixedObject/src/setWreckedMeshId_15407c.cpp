#include "class.h"

extern "C" void *operator_new(uint32_t);
extern "C" void AEGeometry_ctor(void *geom, uint16_t meshId, void *canvas, bool b);
extern "C" void *PaintCanvas_TransformGetTransform(void *canvas, int id);
extern "C" int Globals_getWreckCollision(void *globals, int kind, void *geom);

// PaintCanvas singleton holder (pc-rel deref -> holder; canvas object is *holder).
__attribute__((visibility("hidden"))) extern void **g_pfo_canvas2;
// Globals singleton: pc-rel deref -> holder, **holder is the Globals object.
__attribute__((visibility("hidden"))) extern void ***g_pfo_globals;

extern "C" void PlayerFixedObject_setWreckedMeshId(PlayerFixedObject *self, int meshId)
{
    self->f_184 = (uint16_t)meshId;
    void *geom = operator_new(0xc0);
    void **holder = g_pfo_canvas2;
    AEGeometry_ctor(geom, (uint16_t)meshId, *holder, true);
    self->f_124 = geom;
    void *t = PaintCanvas_TransformGetTransform(*holder, *(int *)((char *)geom + 0xc));
    *(int *)((char *)t + 0xe0) = 0x48f42400; // 500000.0f far-clip constant (raw bits)

    int kind = self->f_ac;
    int sel;
    if (kind == 0xd) {
        sel = 4;
    } else if (kind == 0xe) {
        sel = 0;
    } else if (kind == 0xf) {
        if (self->f_28 == 3) sel = 1;
        else sel = (self->f_28 == 2) ? 2 : 3;
    } else if (kind == 0x37a3) {
        sel = 5;
    } else {
        sel = self->f_194;
        if (sel < 0) return;
        self->f_12c = Globals_getWreckCollision(**g_pfo_globals, sel, self->f_124);
        return;
    }
    self->f_194 = sel;
    self->f_12c = Globals_getWreckCollision(**g_pfo_globals, sel, self->f_124);
}
