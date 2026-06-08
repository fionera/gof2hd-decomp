#include "class.h"

// AbyssEngine::CameraIsPointinViewFrustum(Vector const&, Matrix*, Camera*)
// Test whether a world-space point lies inside the camera's view frustum. The point is
// transformed into camera/local space (optionally pre-multiplied by an extra matrix), then
// projected onto the camera forward/up/right axes and range-checked against the per-camera
// half-extents.
//
// Camera frustum fields (byte offsets): +0x04 near, +0x08 far, +0x0c local matrix (0x3c),
// +0x48 horizontal slope, +0x50 vertical slope.
namespace AbyssEngine {

extern "C" char *g_Camera_frustumEnabledFlag; // **(DAT + 0x81466)

extern "C" {
void *__aeabi_memcpy(void *dst, const void *src, size_t_ n);
// Engine math helpers (referenced by C-name; real symbols are AbyssEngine::AEMath::*).
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

int CameraIsPointinViewFrustum(Vector *point, Matrix *extra, Camera *cam)
{
    if (*g_Camera_frustumEnabledFlag == 0)
        return 1;

    char *c = (char *)cam;
    Matrix local;       // camera-local matrix (0x3c bytes used)
    Matrix transformed; // normalized-axis scratch
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
    AE_VectorSub(&pos, point);
    AE_MatrixGetDir(&dir);
    AE_VectorSub(&axis, &dir);
    AE_VectorNormalize(&transformed, &axis);
    float fwd = AE_VectorDot(&pos, &transformed);

    if (fwd > f32(c, 0x8) || fwd < f32(c, 0x4))
        return 0;

    AE_MatrixGetUp(&axis);
    AE_VectorNormalize(&transformed, &axis);
    float up = AE_VectorDot(&pos, &transformed);

    float hLimit = fwd * f32(c, 0x48);
    if (up > hLimit || up < -hLimit)
        return 0;

    AE_MatrixGetRight(&axis);
    AE_VectorNormalize(&transformed, &axis);
    float right = AE_VectorDot(&pos, &transformed);

    float vLimit = hLimit * f32(c, 0x50);
    return (right <= vLimit && right >= -vLimit) ? 1 : 0;
}

} // namespace AbyssEngine
