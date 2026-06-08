#include "class.h"

extern int Explosion_paintCanvas;

extern "C" uint32_t PaintCanvas_TransformGetTransform32(int canvas, int transform);
extern "C" long long PaintCanvas_TransformGetTransform(int canvas, int transform);
extern "C" void Transform_SetAnimationState(uint32_t transform, int state, int value);
extern "C" void Transform_SetAnimationRangeInTime(long long transform, long long end, long long start);

extern "C" void Explosion_reset(Explosion *self)
{
    int *canvas = &Explosion_paintCanvas;

    Transform_SetAnimationState(
        PaintCanvas_TransformGetTransform32(*canvas, I(G(self, 0x4), 0xc)), 3, 0);
    Transform_SetAnimationState(
        PaintCanvas_TransformGetTransform32(*canvas, I(G(self, 0x4), 0xc)), 1, 0);

    int lodTransform = I(G(self, 0x4), 0x14);
    if (lodTransform != -1) {
        Transform_SetAnimationState(PaintCanvas_TransformGetTransform32(*canvas, lodTransform), 3, 0);
        Transform_SetAnimationState(
            PaintCanvas_TransformGetTransform32(*canvas, I(G(self, 0x4), 0x14)), 1, 0);
    }

    AEGeometry *secondary = G(self, 0x8);
    if (secondary != 0) {
        Transform_SetAnimationState(
            PaintCanvas_TransformGetTransform32(*canvas, I(secondary, 0xc)), 3, 0);
        Transform_SetAnimationState(
            PaintCanvas_TransformGetTransform32(*canvas, I(G(self, 0x8), 0xc)), 1, 0);
    }

    if (I(self, 0x0) == 6) {
        Transform_SetAnimationRangeInTime(
            PaintCanvas_TransformGetTransform(*canvas, I(G(self, 0x4), 0xc)),
            0x8fcLL, 10000000LL);
        Transform_SetAnimationRangeInTime(
            PaintCanvas_TransformGetTransform(*canvas, I(G(self, 0x8), 0xc)),
            0x8fcLL, 10000000LL);
    }

    Array *streaks = A(self, 0xc);
    if (streaks != 0) {
        for (uint32_t i = 0; i < U(streaks, 0x0); i++) {
            AEGeometry *geometry = *(AEGeometry **)((char *)streaks->f_4 + i * 4);
            Transform_SetAnimationState(
                PaintCanvas_TransformGetTransform32(*canvas, I(geometry, 0xc)), 3, 0);
            Transform_SetAnimationState(
                PaintCanvas_TransformGetTransform32(*canvas, I(geometry, 0xc)), 1, 0);
            streaks = A(self, 0xc);
        }
    }

    *(long long *)B(self, 0x18) = 0;
    UC(self, 0x20) = 0;
}
