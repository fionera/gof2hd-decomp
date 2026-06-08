#include "class.h"

// TargetFollowCamera::TargetFollowCamera(uint id, AEGeometry* target, Vector camOffset,
//                                        Vector targetOffset)
// Initializes the camera position triplet to the camOffset, snapshots the target matrix to
// derive the initial position, seeds the local + first-person matrices, the shake/zoom state,
// and precomputes the damping-function coefficients for both axes.
//
// camOffset/targetOffset arrive by value (3 floats each); the decompile reads them off the
// incoming arg slots. We model them as Vector by value.

extern "C" void TFC_Matrix_ctor(Matrix *m);
extern "C" void TFC_Vector_assign(Vector *dst, const Vector *src);   // pcVar8
extern "C" void *TFC_AEGeometry_getMatrix(void *geom);
extern "C" void *TFC_memcpy(void *dst, const void *src, unsigned n);
extern "C" void TFC_MatrixTransformVector(Vector *out, const Vector *v);
extern "C" float TFC_VectorLength(const Vector *v);
extern "C" void TFC_aproximateCooefficientsForAproximationOfDampingFunktion(
    TargetFollowCamera *self, float t, double *outB, double *outA, double *outC,
    double *outD, double *outE, double *outF);
extern "C" void TFC___stack_chk_fail();

__attribute__((visibility("hidden"))) extern int **g_TFC_ctor_canary;
__attribute__((visibility("hidden"))) extern double g_TFC_seed0;   // DAT_0016a668 (2 doubles)
__attribute__((visibility("hidden"))) extern double g_TFC_seed1;
__attribute__((visibility("hidden"))) extern double g_TFC_dampX;   // DAT_0016a678
__attribute__((visibility("hidden"))) extern double g_TFC_dampY;   // DAT_0016a670

extern "C" TargetFollowCamera *TFC_ctor(TargetFollowCamera *self, unsigned id, void *target,
                                        Vector camOffset, Vector targetOffset)
{
    int *canary = *g_TFC_ctor_canary;
    int saved = *canary;
    char *p = (char *)self;

    // position triplet := camOffset, mirror into the three tracked frames
    F<float>(self, 0x08) = 0.0f;
    F<float>(self, 0x0c) = camOffset.y;
    F<float>(self, 0x10) = camOffset.z;
    F<int>(self, 0x14) = (int)camOffset.x;     // field_14..0x24 initialized like the disasm zero-fill
    F<int>(self, 0x18) = 0;
    F<int>(self, 0x1c) = (int)camOffset.y;
    F<int>(self, 0x20) = (int)camOffset.z;
    F<int>(self, 0x24) = (int)camOffset.x;
    F<int>(self, 0x28) = 0;
    F<int>(self, 0x2c) = (int)camOffset.y;
    F<int>(self, 0x30) = (int)camOffset.z;
    F<int>(self, 0x34) = (int)camOffset.x;
    F<int>(self, 0x38) = (int)camOffset.y;
    F<int>(self, 0x3c) = (int)camOffset.z;
    F<int>(self, 0x40) = (int)camOffset.x;
    F<int>(self, 0x50) = 0;
    F<int>(self, 0x54) = 0;
    F<int>(self, 0x58) = 0;

    TFC_Matrix_ctor((Matrix *)(p + 0xb4));
    F<int>(self, 0xf4) = 0;
    F<int>(self, 0xf8) = 0;
    F<int>(self, 0xfc) = 0;
    TFC_Matrix_ctor((Matrix *)(p + 0x13c));

    F<unsigned>(self, 0x00) = id;
    F<void *>(self, 0x04) = target;

    TFC_Vector_assign((Vector *)(p + 0x2c), &camOffset);
    TFC_Vector_assign((Vector *)(p + 0x38), &targetOffset);

    Vector zero = {0.0f, 0.0f, 0.0f};
    TFC_Vector_assign((Vector *)(p + 0x08), &zero);
    TFC_Vector_assign((Vector *)(p + 0x14), &zero);

    // initial position from the target's current matrix
    char mat[0x3c];
    TFC_memcpy(mat, TFC_AEGeometry_getMatrix(target), 0x3c);
    Vector v;
    TFC_MatrixTransformVector(&v, (const Vector *)mat);
    TFC_Vector_assign((Vector *)(p + 0x14), &v);
    TFC_MatrixTransformVector(&v, (const Vector *)mat);
    TFC_Vector_assign((Vector *)(p + 0x08), &v);

    Vector up = {0.0f, 1.0f, 0.0f};
    TFC_Vector_assign((Vector *)(p + 0x20), &up);

    F<int>(self, 0x44) = 0x10000;
    F<int>(self, 0x48) = 0;
    F<uint8_t>(self, 0x4c) = 0;
    F<uint8_t>(self, 0xf0) = 0;
    F<uint8_t>(self, 0x100) = 0;
    F<uint8_t>(self, 0x10c) = 1;
    F<float>(self, 0x110) = 0.0f;       // shake amount
    F<int>(self, 0x114) = 5;            // shake frequency

    float zoom = TFC_VectorLength(&targetOffset);

    // seed second-matrix block + damping inputs
    *(double *)(p + 0x128) = g_TFC_seed0;
    *(double *)(p + 0x130) = g_TFC_seed1;
    F<float>(self, 0xb0) = zoom;
    F<uint8_t>(self, 0x138) = 0;

    TFC_aproximateCooefficientsForAproximationOfDampingFunktion(
        (TargetFollowCamera *)(p + 0x80), zoom, (double *)(p + 0x70),
        (double *)(p + 0x60), (double *)(p + 0x68), (double *)(p + 0x78),
        (double *)(p + 0x60), &g_TFC_dampX);
    TFC_aproximateCooefficientsForAproximationOfDampingFunktion(
        (TargetFollowCamera *)(p + 0xa8), *(float *)(p + 0x12c), (double *)(p + 0x98),
        (double *)(p + 0x88), (double *)(p + 0x90), (double *)(p + 0xa0),
        (double *)(p + 0x88), &g_TFC_dampY);

    if (*canary != saved) {
        TFC___stack_chk_fail();
    }
    return self;
}
