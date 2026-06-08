#include "class.h"

extern int Explosion_paintCanvas;
extern void *Explosion_random;

extern "C" void AEGeometry_setScaling3(AEGeometry *self, float x, float y, float z);
extern "C" int AERandom_nextInt(void *self, int bound);
extern "C" uint32_t PaintCanvas_TransformGetTransform32(int canvas, int transform);
extern "C" void Transform_SetAnimationSpeed(uint32_t transform, float speed);

extern "C" void Explosion_setScaling(Explosion *self, float scale)
{
    F(self, 0x24) = scale;
    AEGeometry_setScaling3(G(self, 0x4), scale, scale, scale);

    AEGeometry *secondary = G(self, 0x8);
    if (secondary != 0) {
        AEGeometry_setScaling3(secondary, scale, scale, scale);
    }

    float speed = 1.0f;
    if (scale < 1.0f) {
        speed = (1.0f - scale) * 3.0f + 1.0f;
    }

    int type = I(self, 0x0);
    if (type == 0xb) {
        speed = speed * 0.5f;
    }
    if ((uint32_t)(type - 8) < 3) {
        speed = 0.7f + (float)AERandom_nextInt(Explosion_random, 0x3c) * 0.01f;
    }

    int *canvas = &Explosion_paintCanvas;
    Transform_SetAnimationSpeed(
        PaintCanvas_TransformGetTransform32(*canvas, I(G(self, 0x4), 0xc)), speed);

    secondary = G(self, 0x8);
    if (secondary != 0) {
        Transform_SetAnimationSpeed(
            PaintCanvas_TransformGetTransform32(*canvas, I(secondary, 0xc)), speed);
    }

    Array *streaks = A(self, 0xc);
    if (streaks != 0) {
        for (uint32_t i = 0; i < U(streaks, 0x0); i++) {
            AEGeometry *geometry = *(AEGeometry **)((char *)P(streaks, 0x4) + i * 4);
            Transform_SetAnimationSpeed(
                PaintCanvas_TransformGetTransform32(*canvas, I(geometry, 0xc)), speed);
            streaks = A(self, 0xc);
        }
    }

    *(long long *)B(self, 0x10) = (long long)((float)*(long long *)B(self, 0x10) / speed);
}
