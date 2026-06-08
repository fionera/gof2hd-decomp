#include "class.h"

extern int Explosion_paintCanvas;

extern "C" void MatrixGetPosition(Vector *out, const Matrix *matrix);
extern "C" void AEGeometry_setPosition(AEGeometry *self, const Vector *position);
extern "C" void AEGeometry_setMatrix(AEGeometry *self, const Matrix *matrix);
extern "C" uint32_t PaintCanvas_TransformGetTransform32(int canvas, int transform);
extern "C" void Explosion_playSound(Explosion *self, Vector *position);

extern "C" void Explosion_start_matrix(Explosion *self, const Matrix *matrix)
{
    char positionStorage[12];
    Vector *position = (Vector *)positionStorage;

    int type = I(self, 0x0);
    if (type == 6 || type == 0) {
        MatrixGetPosition(position, matrix);
        AEGeometry_setPosition(G(self, 0x4), position);
        MatrixGetPosition(position, matrix);
        AEGeometry_setPosition(G(self, 0x8), position);
    } else {
        AEGeometry_setMatrix(G(self, 0x4), matrix);
        AEGeometry *secondary = G(self, 0x8);
        if (secondary != 0) {
            AEGeometry_setMatrix(secondary, matrix);
        }
    }

    int *canvas = &Explosion_paintCanvas;
    Array *streaks = A(self, 0xc);
    if (streaks != 0) {
        for (uint32_t i = 0; i < U(streaks, 0x0); i++) {
            AEGeometry *geometry = *(AEGeometry **)((char *)P(streaks, 0x4) + i * 4);
            MatrixGetPosition(position, matrix);
            AEGeometry_setPosition(geometry, position);
            UC((void *)PaintCanvas_TransformGetTransform32(*canvas, I(geometry, 0xc)), 0xed) = 1;
            streaks = A(self, 0xc);
        }
    }

    UC((void *)PaintCanvas_TransformGetTransform32(*canvas, I(G(self, 0x4), 0xc)), 0xed) = 1;
    int lodTransform = I(G(self, 0x4), 0x14);
    if (lodTransform != -1) {
        UC((void *)PaintCanvas_TransformGetTransform32(*canvas, lodTransform), 0xed) = 1;
    }
    AEGeometry *secondary = G(self, 0x8);
    if (secondary != 0) {
        UC((void *)PaintCanvas_TransformGetTransform32(*canvas, I(secondary, 0xc)), 0xed) = 1;
    }

    UC(self, 0x20) = 1;
    MatrixGetPosition(position, matrix);
    Explosion_playSound(self, position);
}
