#include "class.h"

// ParticleSystemSprite::setParticle(Vector const& pos, float, unsigned int color, float, float,
//     float, float, bool clearColor, float, float, Vector const& uv)
// Pushes one particle's state into the PaintCanvas sprite system: position (from `pos`), size
// (base size + per-system offset), UV rectangle, and RGBA. When `clearColor` is set the packed
// color's low byte is masked off first. Color components are converted from 0..255 bytes to
// normalized floats via VectorUnsignedToFloat and scaled by a global constant.

struct ParticleSystemSprite {
    void setParticle(const void *pos, float p2, unsigned int color, float p4, float p5,
                     float p6, float p7, bool clearColor, float p9, float p10, const void *uv);
};

extern "C" void PaintCanvas_SpriteSystemSetPosition(unsigned int handle, unsigned short id,
                                                    float x, float y, float z);
extern "C" void PaintCanvas_SpriteSystemSetSize(unsigned int handle, unsigned short id, short size);
extern "C" void PaintCanvas_SpriteSystemSetUv(unsigned int handle, unsigned short id,
                                              float u0, float v0, float u1, float v1);
extern "C" void PaintCanvas_SpriteSystemSetRGBA(unsigned int handle, unsigned short id,
                                                float r, float g, float b, float a);
extern "C" float VectorUnsignedToFloat(unsigned int v, unsigned char mode);

// Global color normalization scale (1/255).
__attribute__((visibility("hidden"))) extern float g_colorScale;

void ParticleSystemSprite::setParticle(const void *pos, float p2, unsigned int color,
                                       float p4, float p5, float p6, float p7, bool clearColor,
                                       float p9, float p10, const void *uv)
{
    (void)p2; (void)p5; (void)p6; (void)p7; (void)p9; (void)p10; (void)pos;

    unsigned int handle = F<unsigned int>(this, 0x08);
    unsigned short id = (unsigned short)F<unsigned int>(this, 0x54);

    const float *posv = (const float *)pos;
    PaintCanvas_SpriteSystemSetPosition(handle, id, posv[1], p4, posv[2]);

    short size = (short)F<int>(this, 0x50) + (short)F<int>(this, 0x58);
    PaintCanvas_SpriteSystemSetSize(handle, id, size);

    const float *uvv = (const float *)uv;
    PaintCanvas_SpriteSystemSetUv(handle, id, uvv[0], uvv[1], uvv[2], uvv[3]);

    if (clearColor)
        color &= 0xffffff00;

    float a = VectorUnsignedToFloat(color >> 0x18, 0);
    float b = VectorUnsignedToFloat((color & 0xffff) >> 8, 0);
    float g = VectorUnsignedToFloat(color & 0xff, 0);
    float r = VectorUnsignedToFloat((color & 0xffffff) >> 0x10, 0);
    (void)a; (void)b; (void)g;

    PaintCanvas_SpriteSystemSetRGBA(handle, id, r * g_colorScale, g, g_colorScale, b);
}
