#include "class.h"
#include <arm_neon.h>

// Base of the global ParticleSet array, reached through PC-relative pointers. The
// target loads it twice (one literal for the "flags" view, one for the value view).
__attribute__((visibility("hidden"))) extern char *g_psm_base;
__attribute__((visibility("hidden"))) extern char *g_psm_base2;

// ParticleSettings::multiplyAll(float scale): scale lifetimes and colour components of
// every ParticleSet by `scale` (or its reciprocal, depending on per-set flag bits).
// NOTE: best-effort. The colour clamps lower to a chain of vcvt.u32.f32 / conditional
// 0xff saturation whose exact FP register scheduling under -Oz does not reproduce the
// original byte-for-byte; the arithmetic is faithful to the decompiled form.
extern "C" void ParticleSettings_multiplyAll(float scale) {
    char *flags = g_psm_base;
    char *vals = g_psm_base2;
    float recip = 1.0f / ((scale + 1.0f) * 0.5f);
    for (int i = 0; i != 0x1e00; i += 0xa0) {
        char *f = flags + i;
        unsigned fl = *(unsigned *)(f + 0xc);
        float lifeScale, lifeBase;
        if (fl & 0x20) {
            lifeBase = *(float *)(vals + i + 0x2c);
            lifeScale = scale;
        } else if ((int)(fl << 0x1b) < 0) {
            lifeBase = *(float *)(vals + i + 0x2c);
            lifeScale = 1.0f / scale;
        } else {
            continue;
        }
        char *v = vals + i;
        unsigned c34 = *(unsigned *)(v + 0x34);
        unsigned c38 = *(unsigned *)(v + 0x38);
        float comp10 = (float)*(int *)(v + 0x10);
        float r34 = recip * (float)(int)(c34 & 0xff);
        float r38 = recip * (float)(int)(c38 & 0xff);
        *(float *)(f + 0x2c) = lifeBase * lifeScale;
        unsigned u34 = (0.0f < r34) ? (unsigned)r34 : 0;
        unsigned u38 = (0.0f < r38) ? (unsigned)r38 : 0;
        if (u34 > 0xfe) u34 = 0xff;
        if (u38 > 0xfe) u38 = 0xff;
        *(unsigned *)(f + 0x34) = u34 | (c34 & 0xffffff00);
        *(unsigned *)(f + 0x38) = u38 | (c38 & 0xffffff00);
        *(int *)(f + 0x10) = (int)(comp10 * scale);
    }
}
