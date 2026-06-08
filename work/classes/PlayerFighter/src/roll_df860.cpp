#include "class.h"

extern "C" void *__stack_chk_guard;
extern "C" void __stack_chk_fail(unsigned diff);

extern "C" int AEGeometry_getMatrix(int geom);
extern "C" void AEMath_MatrixIdentity(void *out, void *m);
extern "C" void AEMath_MatrixAssign(void *dst, void *src);
extern "C" void AEMath_MatrixSetRotation(void *m, float rx, float ry, float rz);
extern "C" float VectorSignedToFloat(int v, int mode);

extern void *const gRoll_guard __attribute__((visibility("hidden")));   // DAT_000efa4c
extern const float gRoll_threshold __attribute__((visibility("hidden"))); // DAT_000efa14
extern const float gRoll_f18 __attribute__((visibility("hidden")));
extern const float gRoll_f1c __attribute__((visibility("hidden")));
extern const float gRoll_f20 __attribute__((visibility("hidden")));
extern const float gRoll_f24 __attribute__((visibility("hidden")));
extern const float gRoll_f28 __attribute__((visibility("hidden")));
extern const float gRoll_f2c __attribute__((visibility("hidden")));
extern const float gRoll_f30 __attribute__((visibility("hidden")));
extern const float gRoll_f34 __attribute__((visibility("hidden")));
extern const float gRoll_f38 __attribute__((visibility("hidden")));
extern const float gRoll_f3c __attribute__((visibility("hidden")));
extern const float gRoll_f40 __attribute__((visibility("hidden")));
extern const float gRoll_f44 __attribute__((visibility("hidden")));
extern const float gRoll_f48 __attribute__((visibility("hidden")));

// PlayerFighter::roll(int angle) — self in r0, angle in r1.
extern "C" void PlayerFighter_roll(PlayerFighter *self, int angle)
{
    int *guardP = *(int **)gRoll_guard;
    volatile int saved = *guardP;

    if (F<signed char>(self, 0x100) == 0) {
        goto done;
    }

    {
        int m = AEGeometry_getMatrix(F<int32_t>(self, 8));
        float fwdX = *(float *)(m + 0x10);
        float fwdY = *(float *)(m + 0x14);
        float absX = (0.0f < fwdX) ? fwdX : -fwdX;
        if (0x3b < angle) angle = 0x3c;

        // Snap-back to identity when banked back near level.
        if (fwdY >= 0.0f && absX < gRoll_threshold) {
            unsigned char tmp[60];
            AEMath_MatrixIdentity(tmp, (char *)self + 0x258);
            AEMath_MatrixAssign((char *)self + 0x258, tmp);
            F<signed char>(self, 0x100) = 0;
            F<short>(self, 0x254) = 0;
            goto done;
        }

        float bank;    // fVar11
        float roll;    // fVar10 (z rotation)
        if (F<signed char>(self, 0x254) != 0) {
            roll = (fwdY == fwdY) ? gRoll_f20 : gRoll_f1c;   // NaN(fVar10) selection
            bank = (fwdX < 0.0f) ? roll : gRoll_f18;
        } else {
            bank = 0.0f;
            roll = 0.0f;
            if (fwdX > 0.0f && F<signed char>(self, 0x255) == 1) {
                F<signed char>(self, 0x254) = 1;
                bank = gRoll_f48;
            } else if (fwdX < 0.0f && F<signed char>(self, 0x255) == 2) {
                F<signed char>(self, 0x254) = 1;
                bank = gRoll_f44;
            } else if (fwdX >= 0.0f) {
                if (fwdY >= 0.0f) {
                    bank = absX;
                    const float *p = (gRoll_f38 < fwdX) ? &gRoll_f40 : &gRoll_f3c;
                    bank = *p;
                } else {
                    bank = gRoll_f24;
                }
            } else {
                bank = gRoll_f28;
                if (fwdY >= 0.0f) {
                    const float *p = &gRoll_f30;
                    if (fwdX != gRoll_f2c && fwdX < gRoll_f2c) p = &gRoll_f34;
                    bank = *p;
                }
            }
        }

        if (fwdX != 0.0f) {
            F<signed char>(self, 0x255) = (fwdX < 0.0f) ? 1 : 2;
        }
        float fa = VectorSignedToFloat(angle, 0);
        F<signed char>(self, 0x100) = 1;
        unsigned char tmp[60];
        AEMath_MatrixSetRotation(tmp, bank * fa, 0.0f, roll);
        AEMath_MatrixAssign((char *)self + 0x258, tmp);
    }

done:
    if (*guardP == saved) {
        return;
    }
    __stack_chk_fail((unsigned)(*guardP - saved));
}
