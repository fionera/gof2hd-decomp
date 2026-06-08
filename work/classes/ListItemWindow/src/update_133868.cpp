#include "class.h"

// Callees (resolved blx targets).
extern "C" void  ScrollTouchWindow_update(void *self);                  // 0x75b98
extern "C" int   Ship_getIndex(void *ship);                            // 0x719c8
extern "C" uint32_t PaintCanvas_TransformGetLocal(uint32_t tf);        // 0x720c4
extern "C" void  MatrixSetRotation(void *m, float x, float y, float z); // 0x72094
extern "C" void  MatrixSetScaling(void *m, float x, float y, float z);  // 0x6f814
extern "C" void  AEGeometry_setRotation(void *geo, float x, float y, float z); // 0x73054
extern "C" int   VectorSignedToFloat_i(int v);                          // s32->f32 helper

// Hidden PC-relative roots.
__attribute__((visibility("hidden"))) extern uint32_t *g_liw_u_tf;     // 0x143904 (preview transform handle)
__attribute__((visibility("hidden"))) extern const float *g_liw_u_angleTable; // 0x143... per-ship base angle
extern const float g_liw_u_angleScale;                                 // 0x1439b8

struct ListItemWindow { void update(int frameTime); };

// ListItemWindow::update(int frameTime)
//   Advances the scroll window, applies inertial spin damping to the rotating
//   3D ship preview, and pushes the resulting rotation onto its geometry.
void ListItemWindow::update(int /*frameTime*/)
{
    ScrollTouchWindow_update(pp(this, 0x18));

    if (u8(this, 0x54) == 0)
        return;

    if (u8(this, 0x138) == 0) {
        float spin = f32(this, 0x12c) * f32(this, 0x130);
        float mag = spin > 0.0f ? spin : -spin;
        f32(this, 0x130) = spin;
        if (mag > 1.0f) {
            float angle = (float)i32(this, 0x11c);
            i32(this, 0x11c) = (int)(spin + angle);
        }
    }

    Ship_getIndex(pp(this, 0x14) ? *(void **)((char *)pp(this, 0x14) + 0xc) : 0);
    int idx = Ship_getIndex(*(void **)((char *)pp(this, 0x14) + 0xc));

    float baseAngle = f32(this, 0x114);
    float angle = (float)i32(this, 0x11c) / g_liw_u_angleScale;
    f32(this, 0x118) = angle;

    uint32_t tf = *g_liw_u_tf;
    uint32_t loc = PaintCanvas_TransformGetLocal(tf);
    MatrixSetRotation((void *)loc, angle, 0.0f, 0.0f);
    loc = PaintCanvas_TransformGetLocal(tf);
    float tableAngle = g_liw_u_angleTable[idx] + baseAngle;
    MatrixSetScaling((void *)loc, tableAngle, tableAngle, tableAngle);

    if (i32(this, 0x90) != -1) {
        loc = PaintCanvas_TransformGetLocal(tf);
        MatrixSetRotation((void *)loc, angle, 0.0f, 0.0f);
        loc = PaintCanvas_TransformGetLocal(tf);
        MatrixSetScaling((void *)loc, tableAngle, tableAngle, tableAngle);
    }

    AEGeometry_setRotation(pp(this, 0x10), tableAngle, tableAngle, tableAngle);
}
