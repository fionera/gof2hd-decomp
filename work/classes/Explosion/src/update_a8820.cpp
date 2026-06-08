#include "class.h"

extern int Explosion_paintCanvas;

extern "C" long long PaintCanvas_TransformGetTransform(int canvas, int transform);
extern "C" uint32_t PaintCanvas_TransformGetTransform32(int canvas, int transform);
extern "C" void Transform_Update(long long transform, long long elapsed, int zero);
extern "C" void Transform_Update32(uint32_t transform, uint32_t high, long long elapsed, uint32_t zero);
extern "C" void AEGeometry_setPosition(AEGeometry *self, const Vector *position);
extern "C" void Explosion_reset_tail(Explosion *self);

extern "C" void Explosion_update_vector(Explosion *self, int dt, const Vector *position)
{
    if (UC(self, 0x20) == 0) {
        return;
    }

    int *canvas = &Explosion_paintCanvas;
    long long delta = (long long)dt;
    Transform_Update(PaintCanvas_TransformGetTransform(*canvas, I(G(self, 0x4), 0xc)), delta, 0);

    int lodTransform = I(G(self, 0x4), 0x14);
    if (lodTransform != -1) {
        Transform_Update(PaintCanvas_TransformGetTransform(*canvas, lodTransform), delta, 0);
    }

    AEGeometry *secondary = G(self, 0x8);
    if (secondary != 0) {
        Transform_Update32(PaintCanvas_TransformGetTransform32(*canvas, I(secondary, 0xc)), 0, delta, 0);
    }

    Array *streaks = A(self, 0xc);
    if (streaks != 0) {
        for (uint32_t i = 0; i < U(streaks, 0x0); i++) {
            AEGeometry *geometry = *(AEGeometry **)((char *)P(streaks, 0x4) + i * 4);
            AEGeometry_setPosition(geometry, position);
            Transform_Update(PaintCanvas_TransformGetTransform(*canvas, I(geometry, 0xc)), delta, 0);
            streaks = A(self, 0xc);
        }
    }

    long long elapsed = *(long long *)B(self, 0x18) + delta;
    *(long long *)B(self, 0x18) = elapsed;
    if (*(long long *)B(self, 0x10) < elapsed) {
        return Explosion_reset_tail(self);
    }
}
