#include "class.h"

extern int Explosion_paintCanvas;

extern "C" void Matrix_ctor(Matrix *self);
extern "C" void *operator_new(uint32_t size);
extern "C" void operator_delete(void *ptr);
extern "C" void AEGeometry_ctor(AEGeometry *self, uint16_t mesh, int canvas, bool flag);
extern "C" void AEGeometry_addChild(AEGeometry *self, uint32_t child);
extern "C" void *AEGeometry_dtor(AEGeometry *self);
extern "C" uint32_t PaintCanvas_TransformGetTransform32(int canvas, int transform);
extern "C" void Transform_SetAnimationState(uint32_t transform, int state, int value);
extern "C" void Transform_SetAnimationSpeed(uint32_t transform, float speed);
extern "C" void Explosion_setScaling(Explosion *self, float scale);
extern "C" void Explosion_reset(Explosion *self);

static inline __attribute__((always_inline)) AEGeometry *make_geometry(uint16_t mesh, int canvas)
{
    AEGeometry *geometry = (AEGeometry *)operator_new(0xc0);
    AEGeometry_ctor(geometry, mesh, canvas, false);
    return geometry;
}

extern "C" Explosion *_ZN9ExplosionC2Ei(Explosion *self, int type)
{
    Matrix_ctor((Matrix *)B(self, 0x2c));
    F(self, 0x24) = 1.0f;
    I(self, 0x0) = type;
    G(self, 0x4) = 0;
    G(self, 0x8) = 0;
    A(self, 0xc) = 0;

    int *canvas = &Explosion_paintCanvas;
    AEGeometry *geometry;

    switch (type) {
    case 0:
    case 6:
        G(self, 0x4) = make_geometry(0x41b5, *canvas);
        geometry = make_geometry(0x41b4, *canvas);
        AEGeometry_addChild(G(self, 0x4), U(geometry, 0xc));
        operator_delete(AEGeometry_dtor(geometry));
        break;
    default:
        if (type == 13) {
            G(self, 0x4) = make_geometry(0x41a9, *canvas);
            Explosion_setScaling(self, 0.25f);
            Transform_SetAnimationSpeed(
                PaintCanvas_TransformGetTransform32(*canvas, I(G(self, 0x4), 0xc)), 1.0f);
        } else {
            G(self, 0x4) = make_geometry(0x4213, *canvas);
            G(self, 0x8) = make_geometry(0x4211, *canvas);
        }
        break;
    case 3:
        G(self, 0x4) = make_geometry(0x4213, *canvas);
        G(self, 0x8) = make_geometry(0x421d, *canvas);
        break;
    case 4:
        G(self, 0x4) = make_geometry(0x420d, *canvas);
        G(self, 0x8) = make_geometry(0x420c, *canvas);
        break;
    case 5:
        G(self, 0x4) = make_geometry(0x4999, *canvas);
        G(self, 0x8) = make_geometry(0x4998, *canvas);
        break;
    case 7:
        G(self, 0x4) = make_geometry(0x41a5, *canvas);
        break;
    case 8:
        G(self, 0x4) = make_geometry(0x41a6, *canvas);
        break;
    case 9:
        G(self, 0x4) = make_geometry(0x41a7, *canvas);
        break;
    case 10:
        G(self, 0x4) = make_geometry(0x41a8, *canvas);
        break;
    case 11:
        G(self, 0x4) = make_geometry(0x4a34, *canvas);
        G(self, 0x8) = make_geometry(0x4a33, *canvas);
        break;
    case 12:
        G(self, 0x4) = make_geometry(0x4a7e, *canvas);
        break;
    }

    Transform_SetAnimationState(
        PaintCanvas_TransformGetTransform32(*canvas, I(G(self, 0x4), 0xc)), 1, 0);

    if (G(self, 0x8) != 0) {
        Transform_SetAnimationState(
            PaintCanvas_TransformGetTransform32(*canvas, I(G(self, 0x8), 0xc)), 1, 0);
        F((void *)PaintCanvas_TransformGetTransform32(*canvas, I(G(self, 0x8), 0xc)), 0xe0) =
            10000.0f;
    }

    uint64_t primaryDuration =
        *(uint64_t *)B((void *)PaintCanvas_TransformGetTransform32(*canvas, I(G(self, 0x4), 0xc)), 0xf8);
    uint64_t duration = 0;
    if (G(self, 0x8) != 0) {
        uint64_t secondaryDuration =
            *(uint64_t *)B((void *)PaintCanvas_TransformGetTransform32(*canvas, I(G(self, 0x8), 0xc)), 0xf8);
        duration = secondaryDuration < primaryDuration ? primaryDuration : secondaryDuration;
    } else if (primaryDuration != 0) {
        duration = primaryDuration;
    }
    *(uint64_t *)B(self, 0x10) = duration;

    F((void *)PaintCanvas_TransformGetTransform32(*canvas, I(G(self, 0x4), 0xc)), 0xe0) = 10000.0f;
    I(self, 0x28) = -1;
    Explosion_reset(self);
    return self;
}
