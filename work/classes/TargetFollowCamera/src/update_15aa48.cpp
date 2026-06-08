#include "class.h"

// TargetFollowCamera::update(int dt)
// Advances the chase camera one frame. Depending on the camera mode flags it either:
//   * snaps to a fixed matrix (+0x138 set),
//   * follows the target with a damped spring (the polynomial coefficients precomputed in the
//     ctor at +0x60.. and +0x88..),
//   * runs a first-person matrix, or
//   * locks/positions directly,
// then applies rumble (+0x47) and screen-shake (+0x110), builds the look-at matrix, rolls it
// by +0x130 and pushes it to the active PaintCanvas camera.
//
// The decompile's `in_fpscr` bit-mangling is just clang's lowering of float comparisons; we
// translate those back to ordinary comparisons. `dt` is the int parameter (r1).

extern "C" void *TFC_u_AEGeometry_getMatrix(void *geom);
extern "C" void *TFC_u_memcpy(void *dst, const void *src, unsigned n);
extern "C" void TFC_u_MatrixGetUp(Vector *out, const Matrix *m);
extern "C" void TFC_u_MatrixGetDir(Vector *out, const Matrix *m);
extern "C" void TFC_u_MatrixGetPosition(Vector *out, const Matrix *m);
extern "C" void TFC_u_MatrixGetLookAt(Matrix *out, const Vector *pos, const Vector *at,
                                      const Vector *up);
extern "C" void TFC_u_MatrixIdentity(Matrix *out, const Matrix *m);
extern "C" void TFC_u_MatrixSetRotation(Matrix *out, const Matrix *base, float x, float y, float z,
                                        int order);
extern "C" void TFC_u_MatrixSetRotation3(Matrix *out, float x, float y, float z);
extern "C" void TFC_u_MatrixMul_assign(Matrix *lhs, const Matrix *rhs);   // operator*=
extern "C" void TFC_u_MatrixMul(Matrix *lhs, const Matrix *rhs);          // operator*
extern "C" void TFC_u_MatrixTransformVector(Vector *out, const Vector *m);
extern "C" void TFC_u_Matrix_assign(Matrix *dst, const Matrix *src);
extern "C" void TFC_u_Vector_assign(Vector *dst, const Vector *src);      // pcVar16
extern "C" void TFC_u_Vector_addassign(Vector *dst, const Vector *src);
extern "C" void TFC_u_Vector_mulassign(Vector *dst, float s);
extern "C" void TFC_u_Vector_add(Vector *out, const Vector *a);
extern "C" void TFC_u_Vector_sub(Vector *out, const Vector *a);
extern "C" float TFC_u_VectorLength(const Vector *v);
extern "C" int TFC_u_rand(void *rng, int bound);
extern "C" void TFC_u_CameraSetLocal(unsigned camera, const Matrix *m);
extern "C" void TFC_u___stack_chk_fail();

__attribute__((visibility("hidden"))) extern int **g_TFC_u_canary;
__attribute__((visibility("hidden"))) extern void **g_TFC_u_rng;        // rumble/shake rng holder
__attribute__((visibility("hidden"))) extern unsigned *g_TFC_u_camera;  // active camera holder
__attribute__((visibility("hidden"))) extern double g_TFC_u_seed0;      // DAT_0016b268
__attribute__((visibility("hidden"))) extern double g_TFC_u_seed1;      // DAT_0016b270
__attribute__((visibility("hidden"))) extern float g_TFC_u_shakeMax;    // DAT_0016b278
__attribute__((visibility("hidden"))) extern float g_TFC_u_rumbleScale; // DAT_0016b020
__attribute__((visibility("hidden"))) extern float g_TFC_u_shakeScale;  // DAT_0016b02c

extern "C" void TFC_update(TargetFollowCamera *self, int dt)
{
    int *canary = *g_TFC_u_canary;
    int saved = *canary;
    char *p = (char *)self;

    if (F<char>(self, 0x138) != 0) {
        // fixed-matrix path
        Matrix lm;
        Vector pos;
        TFC_u_MatrixGetPosition(&pos, &lm);
        F<float>(self, 0x08) = pos.x;
        F<float>(self, 0x0c) = pos.y;
        F<float>(self, 0x10) = pos.z;
        TFC_u_Vector_assign((Vector *)(p + 8), &pos);
        TFC_u_CameraSetLocal(*g_TFC_u_camera, *(Matrix **)p);
        if (F<int>(self, 0x04) != 0) {
            char mat[0x3c];
            TFC_u_memcpy(mat, TFC_u_AEGeometry_getMatrix(F<void *>(self, 0x4)), 0x3c);
            Vector up, posv;
            TFC_u_MatrixGetUp(&up, (const Matrix *)mat);
            TFC_u_Vector_assign((Vector *)(p + 0x20), &up);
            TFC_u_MatrixGetPosition(&posv, (const Matrix *)mat);
            TFC_u_Vector_assign((Vector *)(p + 0x14), &posv);
        }
        if (*canary != saved)
            TFC_u___stack_chk_fail();
        return;
    }

    if (dt > 0 && F<char>(self, 0x46) != 0) {
        char mat[0x3c];
        TFC_u_memcpy(mat, TFC_u_AEGeometry_getMatrix(F<void *>(self, 0x4)), 0x3c);

        if (F<char>(self, 0x45) == 0) {           // not look-at cam
            if (F<char>(self, 0x44) == 0) {       // not locked
                if (F<char>(self, 0xf0) == 0) {   // damped follow
                    Vector savedPos14 = *(Vector *)(p + 0x14);
                    Vector savedPos08 = *(Vector *)(p + 0x08);

                    Vector up;
                    TFC_u_MatrixGetUp(&up, (const Matrix *)mat);
                    TFC_u_Vector_assign((Vector *)(p + 0x20), &up);

                    if (F<char>(self, 0x4c) != 0) {        // rotate-around-target
                        Matrix rot;
                        TFC_u_MatrixSetRotation(&rot, (const Matrix *)mat,
                                                F<float>(self, 0x50), F<float>(self, 0x54),
                                                F<float>(self, 0x58), 2);
                        TFC_u_MatrixMul_assign((Matrix *)mat, &rot);
                        float curLen = TFC_u_VectorLength((Vector *)(p + 0x38));
                        if (curLen != F<float>(self, 0xb0))
                            TFC_u_Vector_mulassign((Vector *)(p + 0x38),
                                                   F<float>(self, 0xb0) / curLen);
                    }

                    double dd = (double)dt;
                    double d2 = dd * dd;
                    double d3 = d2 * dd;
                    double d4 = d3 * dd;
                    double inv = 1.0 / (float)dt;

                    Vector dir;
                    TFC_u_MatrixTransformVector(&dir, (const Vector *)mat);
                    TFC_u_Vector_assign((Vector *)(p + 8), &dir);
                    TFC_u_MatrixTransformVector(&dir, (const Vector *)mat);
                    TFC_u_Vector_assign((Vector *)(p + 0x14), &dir);

                    Vector diff = *(Vector *)(p + 0x14);
                    TFC_u_Vector_sub(&diff, (const Vector *)(p + 8));
                    // position-axis spring: const at +0x80, coefficient doubles at +0x60..
                    double c0 = *(double *)(p + 0x80);
                    double *cc = (double *)(p + 0x60);
                    float kA = (float)((c0 + cc[1] * d2 + cc[0] * dd + cc[2] * d3 + cc[3] * d4) * inv);
                    TFC_u_Vector_mulassign(&diff, kA);
                    TFC_u_Vector_add(&diff, &savedPos14);
                    TFC_u_Vector_assign((Vector *)(p + 0x14), &diff);

                    Vector diff2 = *(Vector *)(p + 0x14);
                    TFC_u_Vector_sub(&diff2, (const Vector *)(p + 8));
                    double c0b = *(double *)(p + 0xa8);
                    double *ccb = (double *)(p + 0x88);
                    float kB = (float)((c0b + ccb[1] * d2 + ccb[0] * dd +
                                        *(double *)(p + 0x98) * d3 + *(double *)(p + 0xa0) * d4) * inv);
                    TFC_u_Vector_mulassign(&diff2, kB);

                    if (F<char>(self, 0x100) != 0) {
                        float l = TFC_u_VectorLength(&diff2) + F<float>(self, 0x104);
                        F<float>(self, 0x104) = l;
                        F<uint8_t>(self, 0x100) = (l < g_TFC_u_shakeMax) ? 1 : 0;
                    }

                    Vector finalPos = diff2;
                    TFC_u_Vector_add(&finalPos, &savedPos08);
                    TFC_u_Vector_assign((Vector *)(p + 0x14), &finalPos);
                } else {                           // identity-rotation follow
                    Matrix id;
                    TFC_u_MatrixIdentity(&id, (const Matrix *)mat);
                    TFC_u_Matrix_assign((Matrix *)mat, &id);
                    Matrix rot;
                    TFC_u_MatrixSetRotation3(&rot, 0.0f, 0.0f, 0.0f);
                    TFC_u_MatrixMul(&rot, (const Matrix *)(p + 0xb4));
                    TFC_u_Matrix_assign((Matrix *)mat, &rot);

                    Vector cur = *(Vector *)(p + 8);
                    Vector np;
                    TFC_u_MatrixTransformVector(&np, (const Vector *)mat);
                    TFC_u_Vector_assign((Vector *)(p + 8), &np);

                    if (F<float>(self, 0x108) == 0.0f ||
                        F<float>(self, 0x104) < F<float>(self, 0x108) * 1.5f) {
                        Vector d = np;
                        TFC_u_Vector_sub(&d, (const Vector *)(p + 8));
                        if (F<float>(self, 0x108) == 0.0f)
                            F<float>(self, 0x108) = TFC_u_VectorLength(&d);
                        double dd = (double)dt;
                        double d2 = dd * dd;
                        double d3 = d2 * dd;
                        double d4 = d3 * dd;
                        double inv = 1.0 / (float)dt;
                        double c0 = *(double *)(p + 0xa8);
                        float k = (float)((c0 + *(double *)(p + 0x90) * d3 +
                                           *(double *)(p + 0x88) * d4 +
                                           *(double *)(p + 0x98) * d2 +
                                           *(double *)(p + 0xa0) * dd) * inv);
                        TFC_u_Vector_mulassign(&d, k);
                        float l = TFC_u_VectorLength(&d) + F<float>(self, 0x104);
                        float thr = F<float>(self, 0x108) * 0.75f;
                        F<float>(self, 0x104) = l;
                        F<uint8_t>(self, 0x100) = (l >= thr) ? 1 : 0;
                        TFC_u_Vector_add(&cur, (const Vector *)(p + 8));
                        TFC_u_Vector_assign((Vector *)(p + 8), &cur);
                    }

                    Vector up, dir2;
                    TFC_u_MatrixGetUp(&up, (const Matrix *)mat);
                    TFC_u_Vector_assign((Vector *)(p + 0x20), &up);
                    TFC_u_MatrixGetDir(&dir2, (const Matrix *)mat);
                    Vector tv;
                    TFC_u_MatrixTransformVector(&tv, (const Vector *)mat);
                    TFC_u_Vector_assign((Vector *)(p + 0x14), &tv);
                }
            } else {                               // locked: snap to position
                F<uint8_t>(self, 0x100) = 0;
                Vector pos;
                TFC_u_MatrixGetPosition(&pos, (const Matrix *)mat);
                TFC_u_Vector_assign((Vector *)(p + 0x14), &pos);
                F<float>(self, 0x14) = F<float>(self, 0x14) - F<float>(self, 0x08);
                F<float>(self, 0x18) = F<float>(self, 0x18) - F<float>(self, 0x0c);
                F<float>(self, 0x1c) = F<float>(self, 0x1c) - F<float>(self, 0x10);
            }
        } else {                                   // first-person / look-at cam
            Matrix fp;
            if (F<char>(self, 0x10c) == 0) {
                TFC_u_MatrixIdentity(&fp, (const Matrix *)mat);
            } else {
                TFC_u_memcpy(&fp, mat, 0x3c);
            }
            Vector up, pos;
            TFC_u_MatrixGetUp(&up, &fp);
            TFC_u_Vector_assign((Vector *)(p + 0x20), &up);
            TFC_u_MatrixGetPosition(&pos, &fp);
            TFC_u_Vector_assign((Vector *)(p + 0x14), &pos);
            F<uint8_t>(self, 0x100) = 0;
        }

        // first-person matrix sync
        if (F<char>(self, 0xf0) != 0) {
            Matrix fpm;
            Vector v;
            TFC_u_memcpy(&fpm, (char *)self + 0xb4, 0x3c);
            TFC_u_MatrixGetPosition(&v, &fpm);
            TFC_u_Vector_assign((Vector *)(p + 8), &v);
            TFC_u_MatrixGetUp(&v, &fpm);
            TFC_u_Vector_assign((Vector *)(p + 0x20), &v);
            TFC_u_MatrixGetDir(&v, &fpm);
            TFC_u_Vector_sub(&v, (const Vector *)(p + 8));
            TFC_u_Vector_assign((Vector *)(p + 0x14), &v);
        }

        // rumble
        if (F<char>(self, 0x47) != 0) {
            int t = F<int>(self, 0x48) - dt;
            F<int>(self, 0x48) = t;
            if (t < 1)
                F<uint8_t>(self, 0x47) = 0;
            float scale = (F<char>(self, 0xf0) == 0) ? 1.0f : g_TFC_u_rumbleScale;
            void *rng = *g_TFC_u_rng;
            int amt = F<int>(self, 0x120);
            float r = (float)(TFC_u_rand(rng, amt << 1) - amt);
            F<float>(self, 0x08) = F<float>(self, 0x08) + scale * r;
            amt = F<int>(self, 0x120);
            r = (float)(TFC_u_rand(rng, amt << 1) - amt);
            F<float>(self, 0x0c) = F<float>(self, 0x0c) + scale * r;
            amt = F<int>(self, 0x120);
            r = (float)(TFC_u_rand(rng, amt << 1) - amt);
            F<float>(self, 0x10) = F<float>(self, 0x10) + scale * r;
        }

        // screen-shake
        float shake = F<float>(self, 0x110);
        if (shake > 0.0f) {
            int freq = F<int>(self, 0x114);
            float scale = (F<char>(self, 0xf0) == 0) ? 1.0f : g_TFC_u_shakeScale;
            void *rng = *g_TFC_u_rng;
            int b = freq << 1;
            float r = (float)(TFC_u_rand(rng, b) - freq);
            F<float>(self, 0x14) = F<float>(self, 0x14) + scale * shake * r;
            r = (float)(TFC_u_rand(rng, b) - freq);
            F<float>(self, 0x18) = F<float>(self, 0x18) + scale * shake * r;
            r = (float)(TFC_u_rand(rng, b) - freq);
            F<float>(self, 0x1c) = F<float>(self, 0x1c) + scale * shake * r;
        }

        // build look-at + roll, push to camera, store local matrix
        Matrix look;
        TFC_u_MatrixGetLookAt(&look, (const Vector *)(p + 8), (const Vector *)(p + 0x14),
                              (const Vector *)(p + 0x20));
        Matrix lm;
        TFC_u_Matrix_assign(&lm, &look);
        Matrix roll;
        TFC_u_MatrixSetRotation3(&roll, F<float>(self, 0x130), 0.0f, 0.0f);
        TFC_u_MatrixMul_assign(&lm, &roll);
        TFC_u_CameraSetLocal(*g_TFC_u_camera, *(Matrix **)p);
        TFC_u_Matrix_assign((Matrix *)(p + 0x13c), &lm);
    }

    if (*canary != saved)
        TFC_u___stack_chk_fail();
}
