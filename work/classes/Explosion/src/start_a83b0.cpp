#include "class.h"

extern int Explosion_paintCanvas;
extern void *Explosion_random;

extern "C" void AEGeometry_setPosition(AEGeometry *self, const Vector *position);
extern "C" void AEGeometry_setDirection(AEGeometry *self, const Vector *direction, const Vector *up);
extern "C" uint32_t PaintCanvas_TransformGetTransform32(int canvas, int transform);
extern "C" int AERandom_nextInt(void *self, int bound);
extern "C" void MatrixSetRotation(Matrix *out, Matrix *base, int zero1, int zero2, float angle);
extern "C" void Explosion_setScaling(Explosion *self, float scale);
extern "C" void Explosion_playSound(Explosion *self, Vector *position);

extern "C" void Explosion_start(Explosion *self, const Vector *position, const Vector *direction)
{
    AEGeometry_setPosition(G(self, 0x4), position);

    int *canvas = &Explosion_paintCanvas;
    UC((void *)PaintCanvas_TransformGetTransform32(*canvas, I(G(self, 0x4), 0xc)), 0xed) = 1;

    int lodTransform = I(G(self, 0x4), 0x14);
    if (lodTransform != -1) {
        UC((void *)PaintCanvas_TransformGetTransform32(*canvas, lodTransform), 0xed) = 1;
    }

    AEGeometry *secondary = G(self, 0x8);
    if (secondary != 0) {
        AEGeometry_setPosition(secondary, position);
        UC((void *)PaintCanvas_TransformGetTransform32(*canvas, I(G(self, 0x8), 0xc)), 0xed) = 1;
    }

    int type = I(self, 0x0);
    if ((uint32_t)(type - 8) < 3) {
        Matrix rotation;
        float angle = (float)AERandom_nextInt(Explosion_random, 0xc45) / 1000.0f;
        MatrixSetRotation(&rotation, (Matrix *)B(self, 0x2c), 0, 0, angle);

        float scale = 0.6f + (float)AERandom_nextInt(Explosion_random, 0x28) * 0.01f;
        Explosion_setScaling(self, scale);
    } else if (type == 0xb) {
        Vector up;
        up.x = 0.0f;
        up.y = 1.0f;
        up.z = 0.0f;
        AEGeometry_setDirection(G(self, 0x4), direction, &up);
        up.x = 0.0f;
        up.y = 1.0f;
        up.z = 0.0f;
        AEGeometry_setDirection(G(self, 0x8), direction, &up);
    }

    Array *streaks = A(self, 0xc);
    if (streaks != 0) {
        for (uint32_t i = 0; i < U(streaks, 0x0); i++) {
            AEGeometry *geometry = *(AEGeometry **)((char *)streaks->f_4 + i * 4);
            AEGeometry_setPosition(geometry, position);
            UC((void *)PaintCanvas_TransformGetTransform32(*canvas, I(geometry, 0xc)), 0xed) = 1;
            streaks = A(self, 0xc);
        }
    }

    UC(self, 0x20) = 1;
    Vector soundPosition = *position;
    Explosion_playSound(self, &soundPosition);
}
