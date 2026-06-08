#include "class.h"

// ParticleSystemSprite::updateSingle(int index, float dt)
// Advances one live particle. Skips the work when the system's active flag (+0x34 bit7) is
// clear. Otherwise it ages the particle (ages array at +0x68, per-particle set index at +0x6c
// selecting a 0xA0-byte ParticleSet), kills it once it passes its lifetime, grows its sprite
// size, interpolates and applies its RGBA, steps the UV flipbook animation when the set has
// frames, and integrates its velocity into the sprite position.

struct ParticleSystemSprite {
    void updateSingle(int index, float dt);
};

extern "C" float VectorSignedToFloat(int v, unsigned char mode);
extern "C" void  PaintCanvas_SpriteSystemSetPosition(unsigned int h, unsigned short id, float x, float y, float z);
extern "C" void  PaintCanvas_SpriteSystemAddSize(unsigned int h, unsigned short id, short d);
extern "C" void  PaintCanvas_SpriteSystemAddPosition(unsigned int h, unsigned short id, float x, float y, float z);
extern "C" void  PaintCanvas_SpriteSystemSetRGBA(unsigned int h, unsigned short id, float r, float g, float b, float a);
extern "C" void  PaintCanvas_SpriteSystemSetUv(unsigned int h, unsigned short id, float u0, float v0, float u1, float v1);
extern "C" void  IParticleSystem_interpolateColor(void *self, int index, float *r, float *g, float *b);
extern "C" float *IParticleSystem_rotateUVs(void *self, float *uv, int index);

// Base address of the ParticleSet table the per-particle byte index multiplies into (0xA0 stride).
__attribute__((visibility("hidden"))) extern char *g_particleSetBase;
// Rounding bias the original adds before converting UV offsets to int.
__attribute__((visibility("hidden"))) extern float g_uvRoundBias;

void ParticleSystemSprite::updateSingle(int index, float dt)
{
    // Active flag: sign bit of (flagByte << 24).
    if ((int)((unsigned int)F<unsigned char>(this, 0x34) << 0x18) < 0)
        return;

    unsigned int handle = F<unsigned int>(this, 0x08);
    unsigned short id = (unsigned short)F<unsigned int>(this, 0x54);

    int *ages = (int *)F<void *>(this, 0x68);
    char *setIdx = (char *)F<void *>(this, 0x6c);
    char *set = g_particleSetBase + (int)setIdx[index] * 0xa0;

    // Age the particle.
    float age = VectorSignedToFloat(ages[index], 0);
    age = (float)(int)(age + dt);
    ages[index] = (int)age;

    int lifetime = *(int *)(set + 0x28);
    if ((int)age > lifetime) {
        ages[index] = -1;
        PaintCanvas_SpriteSystemSetPosition(handle, id, age, 0.0f, 0.0f);
        return;
    }

    // Grow size.
    VectorSignedToFloat(*(int *)(set + 0x44), 0);
    PaintCanvas_SpriteSystemAddSize(handle, id, (short)F<int>(this, 0x58) + (short)index);

    // Color.
    float cr, cg, cb;
    IParticleSystem_interpolateColor(this, index, &cr, &cg, &cb);
    PaintCanvas_SpriteSystemSetRGBA(handle, id, cg, 0.0f, cb, 0.0f);

    // UV flipbook animation (when the set has frames at +0x9c).
    int frames = *(int *)(set + 0x9c);
    if (frames != 0) {
        int span = *(int *)(set + 0x28);
        int aged = ages[index] - 1;
        int cur  = (aged * frames) / span;
        int prevAged = (int)((float)aged - dt);
        int prev = (prevAged * frames) / span;
        if (prev < 0) prev = 0;

        if ((unsigned int)cur != (unsigned int)prev) {
            float fcur = VectorSignedToFloat(cur, 0);
            float du = *(float *)(set + 0x90) - *(float *)(set + 0x88);
            float dv = *(float *)(set + 0x94) - *(float *)(set + 0x8c);
            float u0 = *(float *)(set + 0x88) + du * fcur;
            float frac = VectorSignedToFloat((int)(u0 + g_uvRoundBias), 0);
            u0 = u0 - frac;
            float v0 = *(float *)(set + 0x8c) + dv * frac;
            float uv[4];
            uv[0] = u0;
            uv[1] = du + u0;
            uv[2] = v0;
            uv[3] = dv + v0;

            float *out = uv;
            // UV rotation flag: sign bit of (flagByte2 << 30).
            if ((int)((unsigned int)F<unsigned char>(this, 0x37) << 0x1e) < 0)
                out = IParticleSystem_rotateUVs(this, uv, index);

            PaintCanvas_SpriteSystemSetUv(handle, id, out[1], 0.0f, out[2], 0.0f);
        }
    }

    // Integrate velocity into position.
    float pos[4] = { 0, 0, 0, 0 };
    PaintCanvas_SpriteSystemAddPosition(handle, id, pos[1], 0.0f, pos[2]);
}
