#include "class.h"

using AbyssEngine::AEMath::Vector;

extern "C" void *AEGeometry_getMatrix(AEGeometry *self);
extern "C" void *AEGeometry_getReferenceMatrix(AEGeometry *self);
extern "C" void AEGeometry_rotate(AEGeometry *self, float x, float y, float z);
extern "C" void Player_getPosition(Vector *out, Player *self);
extern "C" void MatrixGetDir(Vector *out, const void *matrix);
extern "C" void VectorNormalize(Vector *out, const Vector *v);
extern "C" void Vector_scale(Vector *out, const Vector *v, float value);
extern "C" void Vector_add(Vector *out, const Vector *a, const Vector *b);
extern "C" Vector *Vector_assign(Vector *dst, const Vector *src);
extern "C" void Matrix_assign(void *dst, const void *src);
extern "C" void Matrix_mul(void *out, const void *a, const void *b);
extern "C" void MatrixInverseTransformVector(Vector *out, const void *matrix, const Vector *v);
extern "C" void Player_shoot(Player *self, int weapon, int amount, int amountHigh, int flags,
                             const void *matrix);
extern "C" uint32_t PaintCanvas_TransformGetTransform(uint32_t canvas, uint32_t transform);
extern "C" void Transform_Update(uint32_t transform, int amount, int amountHigh, int flags);

extern uint32_t *gPlayerTurretCanvas_rotation __attribute__((visibility("hidden")));

struct PlayerTurret {
    void handleRotation(int delta, AEGeometry *mainGeometry, AEGeometry *turretGeometry);
};

void PlayerTurret::handleRotation(int delta, AEGeometry *mainGeometry, AEGeometry *turretGeometry)
{
    char matrixBytes[60];
    char positionBytes[12];
    char dirBytes[12];
    char normalBytes[12];
    char scaledBytes[12];
    char sumBytes[12];
    char tmpMatrixA[60];
    char tmpMatrixB[60];

    __aeabi_memcpy(matrixBytes, B(P(this, 0x14c), 0x4), 0x3c);
    Player_getPosition((Vector *)positionBytes, (Player *)P(this, 0x14c));
    MatrixGetDir((Vector *)dirBytes, matrixBytes);
    VectorNormalize((Vector *)normalBytes, (Vector *)dirBytes);
    Vector_scale((Vector *)scaledBytes, (Vector *)normalBytes, 3000.0f);
    Vector_add((Vector *)sumBytes, (Vector *)positionBytes, (Vector *)scaledBytes);
    Vector_assign((Vector *)B(this, 0x9c), (Vector *)sumBytes);

    if (UC(this, 0x3f) == 0) {
        void *base = AEGeometry_getMatrix(TP<AEGeometry>(this, 0x8));
        void *turret = AEGeometry_getMatrix(turretGeometry);
        Matrix_mul(tmpMatrixA, base, turret);
        void *main = AEGeometry_getMatrix(mainGeometry);
        Matrix_mul(tmpMatrixB, tmpMatrixA, main);
        Matrix_assign(matrixBytes, tmpMatrixB);
    } else {
        Matrix_assign(matrixBytes, AEGeometry_getMatrix(TP<AEGeometry>(this, 0x8)));
    }

    MatrixInverseTransformVector((Vector *)positionBytes, matrixBytes, (Vector *)B(this, 0x9c));
    VectorNormalize((Vector *)sumBytes, (Vector *)positionBytes);
    Vector *normal = (Vector *)sumBytes;

    bool ready = false;
    float yaw = 0.0f;
    if (normal->x > 0.0f) {
        yaw = (float)delta;
        AEGeometry_rotate(turretGeometry, 0.0f, yaw * 0.001f * 0.25f, 0.0f);
    } else if (normal->x < -0.05f) {
        yaw = (float)-delta;
        AEGeometry_rotate(turretGeometry, 0.0f, yaw * 0.001f * 0.25f, 0.0f);
    } else {
        ready = true;
    }

    if (normal->y > 0.0f) {
        if (UC(this, 0x3f) == 0 && I(this, 0x134) < 100) {
            P(this, 0x150) = P(this, 0x14c);
            I(this, 0x130) = I(this, 0x130) + delta;
            return;
        }
        float step = (float)I(this, 0x124);
        float next = (float)I(this, 0x134) - step;
        I(this, 0x134) = (int)next;
        AEGeometry_rotate(mainGeometry, next, 0.0f, step * 0.001f * 0.25f);
        P(this, 0x150) = 0;
        return;
    }
    if (normal->y < -0.05f) {
        if (UC(this, 0x3f) == 0 && I(this, 0x134) > 99) {
            P(this, 0x150) = P(this, 0x14c);
            I(this, 0x130) = I(this, 0x130) + delta;
            return;
        }
        float step = (float)I(this, 0x124);
        float next = (float)I(this, 0x134) + step;
        I(this, 0x134) = (int)next;
        AEGeometry_rotate(mainGeometry, next, 0.0f, step * 0.001f * -0.25f);
        P(this, 0x150) = 0;
        return;
    }

    if (ready) {
        Player_shoot(TP<Player>(this, 0x4), 0, delta, delta >> 31, 0, matrixBytes);
        uint32_t transform = PaintCanvas_TransformGetTransform(*gPlayerTurretCanvas_rotation,
                                                               U(turretGeometry, 0xc));
        Transform_Update(transform, delta, delta >> 31, 0);
    }
}
