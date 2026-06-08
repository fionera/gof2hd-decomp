#include "class.h"

// AbyssEngine::CameraIsSphereinViewFrustum(Vector const&, float radius, Matrix*, Camera*)
// Test whether a world-space sphere (center, radius) overlaps the camera view frustum. The
// center is transformed into camera space (optionally pre-multiplied by an extra matrix) and
// the forward/right/up projections are range-checked, with the sphere radius widening each
// near/far/side bound.
//
// Camera fields (byte offsets): +0x04 near, +0x08 far, +0x0c local matrix (0x3c),
// +0x48 fwd-h slope, +0x50 fwd-v slope, +0x54 right radius scale, +0x58 up radius scale.
namespace AbyssEngine {

extern "C" char *g_Camera_frustumEnabledFlag; // **(DAT + 0x815f2)

extern "C" {
void *__aeabi_memcpy(void *dst, const void *src, size_t_ n);
void AE_Matrix_mulAssign(void *self, const void *rhs);
void AE_Vector_assign(void *self, const void *rhs);
void AE_MatrixInverseTransformVector(void *out, const void *in);
void AE_MatrixGetPosition(void *m);
void AE_MatrixGetDir(void *m);
void AE_MatrixGetUp(void *m);
void AE_MatrixGetRight(void *m);
void AE_VectorSub(void *self, const void *rhs);
void AE_VectorNormalize(void *out, const void *in);
float AE_VectorDot(const void *a, const void *b);
}

int CameraIsSphereinViewFrustum(Vector *center, float radius, Matrix *extra, Camera *cam)
{
    // Ghidra aliases the camera-data pointer through r3; cam is the frustum data here.
    char *c = (char *)cam;

    if (!(radius != 0.0f && *g_Camera_frustumEnabledFlag != 0))
        return 1;

    Matrix local;
    Matrix transformed;
    Vector pos, dir, axis;
    Vector camPoint = {0.0f, 0.0f, 0.0f};

    Matrix *srcMatrix = (Matrix *)(c + 0xc);
    Matrix *dstMatrix;
    if (extra == 0) {
        dstMatrix = &local;
    } else {
        __aeabi_memcpy(&local, srcMatrix, 0x3c);
        AE_Matrix_mulAssign(&local, extra);
        dstMatrix = &transformed;
        srcMatrix = &local;
    }

    AE_MatrixInverseTransformVector(dstMatrix, srcMatrix);
    AE_Vector_assign(&camPoint, dstMatrix);

    AE_MatrixGetPosition(&pos);
    AE_VectorSub(&pos, center);
    AE_MatrixGetDir(&dir);
    AE_VectorSub(&axis, &dir);
    AE_VectorNormalize(&transformed, &axis);
    float fwd = AE_VectorDot(&pos, &transformed);

    // Near/far range padded by the radius.
    if (fwd > f32(c, 0x8) + radius)
        return 0;
    if (fwd < f32(c, 0x4) - radius)
        return 0;

    AE_MatrixGetRight(&axis);
    AE_VectorNormalize(&transformed, &axis);
    float right = AE_VectorDot(&pos, &transformed);

    float rightPad = f32(c, 0x54) * radius;
    float rightLimit = fwd * f32(c, 0x48) * f32(c, 0x50);
    if (right > rightLimit + rightPad)
        return 0;
    if (right < -rightLimit - rightPad)
        return 0;

    AE_MatrixGetUp(&axis);
    AE_VectorNormalize(&transformed, &axis);
    float up = AE_VectorDot(&pos, &transformed);

    float upPad = f32(c, 0x58) * radius;
    float upLimit = fwd * f32(c, 0x48);
    if (up > upLimit + upPad)
        return 0;
    if (up < -upLimit - upPad)
        return 0;

    return 1;
}

} // namespace AbyssEngine
