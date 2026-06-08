#include "class.h"

extern "C" void *__stack_chk_guard;
extern "C" void __stack_chk_fail(unsigned diff);

extern "C" float VectorSignedToFloat(int v, int mode);
extern "C" void AEMath_MatrixSetRotation(void *m, float rx, float ry, float rz);
extern "C" int AEGeometry_getMatrix(int geom);
extern "C" void AEMath_MatrixMul(void *out, void *m);          // operator*(out, m)
extern "C" void AEGeometry_setMatrix(void *geom);
extern "C" float l2f(long long v);                              // __aeabi_l2f
extern "C" void AEGeometry_translate(void *geom);
// Vector scale helper used 3x: out = vec * scalar.
extern "C" void PF_vscale(void *out, void *vec, float scalar);

extern void *const gPush_guard __attribute__((visibility("hidden")));     // DAT_000eff24
extern const float gPush_div __attribute__((visibility("hidden")));       // DAT_000eff20

// PlayerFighter::push(int dt) — self in r0, dt in r1.
extern "C" void PlayerFighter_push(PlayerFighter *self, int dt)
{
    int *guardP = *(int **)gPush_guard;
    volatile int saved = *guardP;

    if (0 < F<int32_t>(self, 0x104)) {
        int remaining = F<int32_t>(self, 0x104) - dt;
        float fr = VectorSignedToFloat(remaining, 0);
        float ftotal = VectorSignedToFloat(F<int32_t>(self, 0x108), 0);
        F<int32_t>(self, 0x104) = remaining;
        float frac = fr / ftotal;

        unsigned char rot[60];
        AEMath_MatrixSetRotation(rot, frac * *(float *)((char *)self + 0x120), 0.0f,
                                 frac * *(float *)((char *)self + 0x11c));

        int lo = F<int32_t>(self, 0x1d0);
        int hi = F<int32_t>(self, 0x1d4);
        if ((int)(unsigned)(lo == 0) <= hi) {
            void *geom = F<void *>(self, 8);
            void *m = (void *)(long)AEGeometry_getMatrix((int)(long)geom);
            AEMath_MatrixMul(rot, m);
            AEGeometry_setMatrix(geom);
            lo = F<int32_t>(self, 0x1d0);
            hi = F<int32_t>(self, 0x1d4);
        }
        float speed = l2f(((long long)hi << 32) | (unsigned)lo);
        void *geom = F<void *>(self, 8);
        float ftotal2 = VectorSignedToFloat(F<int32_t>(self, 0x108), 0);

        unsigned char a[12], b[12], c[60];
        PF_vscale(a, (char *)self + 0x10c, speed);
        PF_vscale(b, a, *(float *)((char *)self + 0x1e8));
        PF_vscale(c, b, (2.0f - frac) * 3.0f * (ftotal2 / gPush_div));
        AEGeometry_translate(geom);
    }

    if (*guardP == saved) {
        return;
    }
    __stack_chk_fail((unsigned)(*guardP - saved));
}
