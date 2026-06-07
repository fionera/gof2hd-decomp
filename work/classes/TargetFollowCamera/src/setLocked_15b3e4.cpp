#include "class.h"

// Sets the locked flag at 0x44. When locking, snapshots the current ship matrix:
// copies AEGeometry::getMatrix() into a local, takes its up-vector as the camera up
// (0x20), transforms the cam offset (0x38) by it for the position (0x8), then updates.
// The on-stack Matrix/Vector buffers make the compiler emit the canary automatically.
extern "C" float *AEGeometry_getMatrix(void *geom);
extern "C" void *memcpy(void *, const void *, unsigned);
extern "C" void *MatrixGetUp(Vector *out, const void *m);
extern "C" void *Vector_assign(Vector *dst, const Vector *src);
extern "C" void *MatrixTransformVector(Vector *out, const void *m, const Vector *v);
extern "C" void TFC_update(TargetFollowCamera *self, int n);

extern "C" void TFC_setLocked(TargetFollowCamera *self, bool locked) {
    char *p = (char *)self;
    char mat[60];    // local matrix copy (char buffer -> stack canary)
    Vector up;
    F<uint8_t>(self, 0x44) = locked;
    if (locked) {
        __builtin_memcpy(mat, AEGeometry_getMatrix(F<void *>(self, 0x4)), 0x3c);
        MatrixGetUp(&up, mat);
        Vector_assign((Vector *)(p + 0x20), &up);
        MatrixTransformVector(&up, mat, (const Vector *)(p + 0x38));
        Vector_assign((Vector *)(p + 0x8), &up);
        TFC_update(self, 0x32);
    }
}
