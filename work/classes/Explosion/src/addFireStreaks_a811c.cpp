#include "class.h"

extern int Explosion_paintCanvas;
extern void *Explosion_random;

extern "C" void *operator_new(uint32_t size);
extern "C" void Array_AEGeometryPtr_ctor(Array *self);
extern "C" void ArraySetLength_AEGeometryPtr(int length, Array *self);
extern "C" int AERandom_nextInt(void *self, int bound);
extern "C" void AEGeometry_ctor(AEGeometry *self, uint16_t mesh, int canvas, bool flag);
extern "C" uint32_t PaintCanvas_TransformGetTransform32(int canvas, int transform);
extern "C" void Transform_SetAnimationState(uint32_t transform, int state, int value);
extern "C" void AEGeometry_setRotation3(AEGeometry *self, float x, float y, float z);
extern "C" void AEGeometry_setScaling3(AEGeometry *self, float x, float y, float z);

extern "C" void Explosion_addFireStreaks(Explosion *self)
{
    if (A(self, 0xc) != 0) {
        return;
    }

    Array *streaks = (Array *)operator_new(0xc);
    Array_AEGeometryPtr_ctor(streaks);
    A(self, 0xc) = streaks;

    int length = AERandom_nextInt(Explosion_random, 7) + 3;
    ArraySetLength_AEGeometryPtr(length, A(self, 0xc));

    int *canvas = &Explosion_paintCanvas;
    int (*nextInt)(void *, int) = AERandom_nextInt;

    for (uint32_t i = 0; i < U(A(self, 0xc), 0x0); i++) {
        AEGeometry *geometry = (AEGeometry *)operator_new(0xc0);
        AEGeometry_ctor(geometry, 0x37d4, *canvas, false);
        *(AEGeometry **)((char *)P(A(self, 0xc), 0x4) + i * 4) = geometry;

        geometry = *(AEGeometry **)((char *)P(A(self, 0xc), 0x4) + i * 4);
        Transform_SetAnimationState(
            PaintCanvas_TransformGetTransform32(*canvas, I(geometry, 0xc)), 1, 0);

        geometry = *(AEGeometry **)((char *)P(A(self, 0xc), 0x4) + i * 4);
        F((void *)PaintCanvas_TransformGetTransform32(*canvas, I(geometry, 0xc)), 0xe0) =
            10000.0f;

        geometry = *(AEGeometry **)((char *)P(A(self, 0xc), 0x4) + i * 4);
        float x = (float)nextInt(Explosion_random, 0x168);
        float y = (float)nextInt(Explosion_random, 0x168);
        float z = (float)nextInt(Explosion_random, 0x168);
        AEGeometry_setRotation3(geometry, (x / 180.0f) * 3.1415927f,
                                (y / 180.0f) * 3.1415927f,
                                (z / 180.0f) * 3.1415927f);

        int scaleInt = nextInt(Explosion_random, 0x32) + 0x32;
        float scale = (float)scaleInt / 100.0f;
        geometry = *(AEGeometry **)((char *)P(A(self, 0xc), 0x4) + i * 4);
        AEGeometry_setScaling3(geometry, scale, scale, scale);
    }
}
