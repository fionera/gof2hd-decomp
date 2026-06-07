#include "class.h"

// Base of the global ParticleSet array, reached through a PC-relative pointer. The
// target materializes the base twice (separate literals) — once for the two source
// entries and once for the destination — so we model it as two aliasing globals.
__attribute__((visibility("hidden"))) extern char *g_ps_base;
__attribute__((visibility("hidden"))) extern char *g_ps_base_dst;

// ParticleSettings::Interpolate(ParticleSet a, ParticleSet b, float t, ParticleSet out):
// out = b*t + (1-t)*a, field by field. Indices select 0xa0-byte ParticleSet entries.
// Field +0x28 is stored as an int (count) but blended in float space; +0x14/+0x20/+0x70
// are plain floats. Operands are written (1-t)*a + b*t so the b*t product seeds the
// fused multiply-add, matching the target's vmul/vmla pairing.
extern "C" void ParticleSettings_Interpolate(int a, int b, float t, int out) {
    char *base = g_ps_base;
    char *pb = base + b * 0xa0;
    char *pa = base + a * 0xa0;
    float omt = 1.0f - t;
    float cb = (float)*(int *)(pb + 0x28);
    float ca = (float)*(int *)(pa + 0x28);
    char *po = g_ps_base_dst + out * 0xa0;
    float a14 = *(float *)(pa + 0x14);
    *(int *)(po + 0x28) = (int)(omt * ca + cb * t);
    float a20 = *(float *)(pa + 0x20);
    *(float *)(po + 0x14) = omt * a14 + *(float *)(pb + 0x14) * t;
    *(float *)(po + 0x20) = omt * a20 + *(float *)(pb + 0x20) * t;
    *(float *)(po + 0x70) = omt * *(float *)(pa + 0x70) + *(float *)(pb + 0x70) * t;
}
