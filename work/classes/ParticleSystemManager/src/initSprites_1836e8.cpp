#include "class.h"

// ParticleSystemManager::initSprites()
// When the sprite array (+0x18) is non-empty and a camera set (+0xc) is active, this creates the
// shared sprite system on the canvas (+0x04) -- either from an existing texture id (+0x24) or by
// also creating a texture from id (+0x26) -- stores its handle at +0x30, primes the system-wide
// size/UV from the active particle-set's frame rect, then walks every sub-system, binding it to
// the sprite-system handle at its running particle offset.

extern "C" void  PaintCanvas_SpriteSystemCreate(void *canvas, unsigned short count, bool b,
                                                unsigned int *out);
extern "C" void  PaintCanvas_SpriteSystemCreateTex(void *canvas, unsigned short count, bool b,
                                                   unsigned short tex, unsigned int *out);
extern "C" void  PaintCanvas_TextureCreate(unsigned short canvas, unsigned int texId, bool b);
extern "C" void  PaintCanvas_SpriteSystemSetAllSize(unsigned int canvas, short size);
extern "C" void  PaintCanvas_SpriteSystemSetAllUv(void *canvas, unsigned int handle,
                                                 float u, float v0, float w, float v1);
extern "C" short _ips_getParticleCount16(void *sys);

// Active particle-set descriptor (the engine resolves it from a global table).
__attribute__((visibility("hidden"))) extern char *g_activeParticleSet;

struct ParticleSystemManager { void initSprites(); };
void ParticleSystemManager::initSprites()
{
    if (i32(this, 0x18) == 0)
        return;

    u32(this, 0x30) = 0xffffffff;
    if (i32(this, 0x0c) == 0)
        return;

    void *canvas = pp(this, 0x04);
    if (u16(this, 0x24) == 0xffff) {
        if ((short)u16(this, 0x26) != -1) {
            PaintCanvas_SpriteSystemCreate(canvas, (unsigned short)u32(this, 0x34), false,
                                           (unsigned int *)((char *)this + 0x30));
            PaintCanvas_TextureCreate((unsigned short)(unsigned long)pp(this, 0x04),
                                      u16(this, 0x26), (((char)(unsigned long)this + ',') != 0));
        }
    } else {
        PaintCanvas_SpriteSystemCreateTex(canvas, (unsigned short)u32(this, 0x34), false,
                                          u16(this, 0x24),
                                          (unsigned int *)((char *)this + 0x30));
    }

    short offset = 0;
    PaintCanvas_SpriteSystemSetAllSize(u32(this, 0x04), (short)u32(this, 0x30));

    float u = *(float *)(g_activeParticleSet + 0x90);
    float w = *(float *)(g_activeParticleSet + 0x94);
    PaintCanvas_SpriteSystemSetAllUv(pp(this, 0x04), u32(this, 0x30), u, 0.0f, w, 0.0f);

    for (unsigned i = 0; i < u32(this, 0x18); ++i) {
        void *sys = ((void **)u32(this, 0x1c))[i];
        typedef void (*pfn)(void *, unsigned int, short, void *, float, float);
        pfn fn = (pfn)(*(void ***)sys)[0];
        fn(sys, u32(this, 0x30), offset, (void *)fn, u, w);
        offset = offset + _ips_getParticleCount16(((void **)u32(this, 0x1c))[i]);
    }
}
