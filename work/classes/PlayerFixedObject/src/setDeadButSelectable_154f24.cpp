#include "class.h"

// NEAR (80.6%): structure matches, but the final Transform::SetAnimationRangeInTime call is
// emitted as a tail-branch here while the target keeps a frame and passes the 64-bit arg on the
// stack (push {r2,r3,...}; strd; call; pop). Calling-convention detail not reproducible from src.
extern "C" void Player_setHitpoints(void *player, int hp);
extern "C" void Player_setVulnerable(void *player, bool v);
extern "C" void LODManager_removeObject(void *lod, void *geom);
extern "C" void *AEGeometry_dtor(void *geom);
extern "C" void operator_delete(void *p);
extern "C" void *PaintCanvas_TransformGetTransform(void *canvas, int id);
extern "C" void Transform_SetAnimationRangeInTime(void *transform, long long range);
// PaintCanvas singleton holder (pc-rel -> holder; *holder is the canvas).
__attribute__((visibility("hidden"))) extern void **g_pfo_canvas3;

extern "C" void PlayerFixedObject_setDeadButSelectable(PlayerFixedObject *self)
{
    void *player = self->f_4;
    self->f_134 = 0;
    Player_setHitpoints(player, 1);
    Player_setVulnerable(self->f_4, false);
    LODManager_removeObject(*(void **)self->f_54, self->f_8);
    void *geom = self->f_8;
    if (geom != 0) operator_delete(AEGeometry_dtor(geom));
    void **holder = g_pfo_canvas3;
    void *newGeom = self->f_124;
    self->f_8 = newGeom;
    void *t = PaintCanvas_TransformGetTransform(*holder, *(int *)((char *)newGeom + 0xc));
    Transform_SetAnimationRangeInTime(t, *(long long *)((char *)t + 0xf8));
}
