#include "class.h"

extern "C" void MatrixSetRotation(Matrix *out, const Matrix *base, float x, float y, float z);
extern "C" Matrix *AEGeometry_getMatrix(AEGeometry *geometry);
extern "C" Matrix Matrix_mul(const Matrix *a, const Matrix *b);
extern "C" void AEGeometry_setMatrix(AEGeometry *geometry, const Matrix *matrix);
extern "C" Vector Vector_scale(const Vector *vector, float scale);
extern "C" void AEGeometry_translate(AEGeometry *geometry, const Vector *delta);

void PlayerAsteroid::push(int delta)
{
    int remaining = this->f_104;
    if (remaining > 0) {
        remaining -= delta;
        this->f_104 = remaining;
        float t = (float)remaining / (float)this->f_108;

        Matrix identity = {{
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f,
        }};
        Matrix rotation;
        MatrixSetRotation(&rotation, &identity, t * this->f_118,
                          t * this->f_11c, t * this->f_120);

        int frameDelta = this->f_134;
        AEGeometry *geometry = this->f_8;
        if (frameDelta > 0) {
            Matrix combined = Matrix_mul(&rotation, AEGeometry_getMatrix(geometry));
            AEGeometry_setMatrix(geometry, &combined);
            frameDelta = this->f_134;
        }

        Vector baseMove = Vector_scale((Vector *)((char *)this + 0x10c), (float)frameDelta);
        float scale = (2.0f - t) * 3.0f * ((float)this->f_108 / 1000.0f);
        Vector move = Vector_scale(&baseMove, scale);
        AEGeometry_translate(geometry, &move);
    }
}
