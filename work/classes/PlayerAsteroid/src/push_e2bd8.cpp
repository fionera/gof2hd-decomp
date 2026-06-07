#include "class.h"

extern "C" void MatrixSetRotation(Matrix *out, const Matrix *base, float x, float y, float z);
extern "C" Matrix *AEGeometry_getMatrix(AEGeometry *geometry);
extern "C" Matrix Matrix_mul(const Matrix *a, const Matrix *b);
extern "C" void AEGeometry_setMatrix(AEGeometry *geometry, const Matrix *matrix);
extern "C" Vector Vector_scale(const Vector *vector, float scale);
extern "C" void AEGeometry_translate(AEGeometry *geometry, const Vector *delta);

void PlayerAsteroid::push(int delta)
{
    int remaining = F<int>(this, 0x104);
    if (remaining > 0) {
        remaining -= delta;
        F<int>(this, 0x104) = remaining;
        float t = (float)remaining / (float)F<int>(this, 0x108);

        Matrix identity = {{
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f,
        }};
        Matrix rotation;
        MatrixSetRotation(&rotation, &identity, t * F<float>(this, 0x118),
                          t * F<float>(this, 0x11c), t * F<float>(this, 0x120));

        int frameDelta = F<int>(this, 0x134);
        AEGeometry *geometry = F<AEGeometry *>(this, 0x8);
        if (frameDelta > 0) {
            Matrix combined = Matrix_mul(&rotation, AEGeometry_getMatrix(geometry));
            AEGeometry_setMatrix(geometry, &combined);
            frameDelta = F<int>(this, 0x134);
        }

        Vector baseMove = Vector_scale((Vector *)((char *)this + 0x10c), (float)frameDelta);
        float scale = (2.0f - t) * 3.0f * ((float)F<int>(this, 0x108) / 1000.0f);
        Vector move = Vector_scale(&baseMove, scale);
        AEGeometry_translate(geometry, &move);
    }
}
