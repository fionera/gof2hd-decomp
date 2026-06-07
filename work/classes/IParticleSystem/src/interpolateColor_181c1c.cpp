#include "class.h"

__attribute__((visibility("hidden"))) extern char *ParticleSet_definitions;

void IParticleSystem::interpolateColor(int index, float *alpha, float *red, float *green, float *blue)
{
    int age = ((int *)P(this, 0x68))[index];
    int setIndex = ((int8_t *)P(this, 0x6c))[index];
    char *def = ParticleSet_definitions + (setIndex + setIndex * 4) * 32;

    float t = (float)age / (float)*(int *)(def + 0x28);
    if (t > 1.0f) {
        t = 1.0f;
    }
    float inv = 1.0f - t;
    uint32_t c0 = *(uint32_t *)(def + 0x34);
    uint32_t c1 = *(uint32_t *)(def + 0x38);

    float a0 = (float)(c0 >> 24);
    float r0 = (float)((c0 >> 16) & 0xff);
    float g0 = (float)((c0 >> 8) & 0xff);
    float b0 = (float)(c0 & 0xff);
    float a1 = (float)(c1 >> 24);
    float r1 = (float)((c1 >> 16) & 0xff);
    float g1 = (float)((c1 >> 8) & 0xff);
    float b1 = (float)(c1 & 0xff);

    const float scale = 0.003921568859368563f;
    *alpha = (inv * a0 + t * a1) * scale;
    *red = (inv * r0 + t * r1) * scale;
    *green = (inv * g0 + t * g1) * scale;
    *blue = (inv * b0 + t * b1) * scale;

    int fadeFrames = *(int *)(def + 0x3c);
    if (age < fadeFrames) {
        float fade = (float)age / (float)fadeFrames;
        if (U8(this, 0x45) != 0) {
            *alpha *= fade;
            *red *= fade;
            *green *= fade;
        } else {
            *blue *= fade;
        }
    }
}
