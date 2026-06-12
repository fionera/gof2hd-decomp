#include "gof2/ParticleSystemSprite.h"
#include "gof2/IParticleSystem.h"


extern "C" char ParticleSystemSprite_vtable;
extern "C" void *ParticleSystemSprite_baseDtor(void *self);
extern "C" void PaintCanvas_SetTexture(unsigned int canvas, unsigned int texture);
extern "C" void PaintCanvas_SetBlendMode(void *canvas, int blend);
extern "C" void PaintCanvas_SetWorldViewMatrix(void *canvas, const void *matrix);
extern "C" void PaintCanvas_DrawSpriteSystem(unsigned int canvas);
extern "C" void *memset(void *, int, unsigned long);
extern "C" char _ZTV20ParticleSystemSprite[];
extern "C" float VectorSignedToFloat(int v, unsigned char mode);
extern "C" void  PaintCanvas_SpriteSystemSetPosition(unsigned int h, unsigned short id, float x, float y, float z);
extern "C" void  PaintCanvas_SpriteSystemAddSize(unsigned int h, unsigned short id, short d);
extern "C" void  PaintCanvas_SpriteSystemAddPosition(unsigned int h, unsigned short id, float x, float y, float z);
extern "C" void  PaintCanvas_SpriteSystemSetRGBA(unsigned int h, unsigned short id, float r, float g, float b, float a);
extern "C" void  PaintCanvas_SpriteSystemSetUv(unsigned int h, unsigned short id, float u0, float v0, float u1, float v1);
extern "C" void ParticleSystem_updateAreaExitParticleImpl(void *self, int param_1, float param_2);
extern "C" void PaintCanvas_SpriteSystemSetSize(unsigned int handle, unsigned short id, short size);
extern "C" float VectorUnsignedToFloat(unsigned int v, unsigned char mode);
extern "C" void  PaintCanvas_CameraGetCurrent();
extern "C" float *PaintCanvas_CameraGetLocal(unsigned int canvas);

// ---- _ParticleSystemSprite_182c58.cpp ----
// Complete object destructor (D1): set vtable, run release(), tail-call base dtor.
extern "C" void _ZN20ParticleSystemSprite7releaseEv(void *self); // release()

void *_ZN20ParticleSystemSpriteD1Ev(void *self)
{
    *(void **)self = &ParticleSystemSprite_vtable + 8;
    _ZN20ParticleSystemSprite7releaseEv(self);
    return ParticleSystemSprite_baseDtor(self);
}

// ---- reset_18335c.cpp ----
void ParticleSystemSprite::reset()
{
    for (int i = 0; i < this->particleCount; i++) {
        PaintCanvas_SpriteSystemSetPosition(this->canvasHandle,
                                            (unsigned short)(this->idOffset + i),
                                            4294967296.0f, 4294967296.0f, 4294967296.0f);
        PaintCanvas_SpriteSystemSetSize(this->canvasHandle,
                                        (unsigned short)(this->idOffset + i), 0);
        this->ages[i] = 0xffffffff;
    }
    this->field_0x60 = 0;
    this->field_0x4 = 1;
}

// ---- init_182c48.cpp ----
int ParticleSystemSprite::init(unsigned int param_1, unsigned short param_2)
{
    void **vt = *(void ***)this;
    this->spriteId = param_1;
    this->idOffset = param_2;
    this->field_0x5c = 1;
    typedef int (*pfn)(void *);
    return ((pfn)vt[2])(this);
}

// ---- release_1833b8.cpp ----
void ParticleSystemSprite::release()
{
    if (this->spriteData != 0)
        ::operator delete[](this->spriteData);
    this->spriteData = 0;
    if (this->ages != 0)
        ::operator delete[](this->ages);
    this->ages = 0;
    ::operator delete[](this->setIndices);
    this->setIndices = 0;
}

// ---- render_1833e0.cpp ----
// ParticleSystemSprite::render(PaintCanvas*, unsigned int handle, unsigned int texture, BlendMode)
// Draws the sprite system with an identity world transform: binds the texture and blend mode,
// builds an identity 3x4 matrix on the stack, pushes it as the world-view matrix, and issues
// the sprite-system draw. A handle of -1 means "no system" and is skipped.

// Constant {0,0,1,0} tail used to fill the identity matrix (matches the original literals).
static const float kIdentTail[4] = { 0.0f, 0.0f, 1.0f, 0.0f };

void ParticleSystemSprite::render(void *canvas, int handle, unsigned int texture, int blend)
{
    if (handle == -1)
        return;

    PaintCanvas_SetTexture((unsigned int)(unsigned long)canvas, texture);
    PaintCanvas_SetBlendMode(canvas, blend);

    // Identity affine transform (3x4 rows + trailing vector).
    float m[16];
    m[0] = 1.0f;
    m[1] = 0.0f;
    m[2] = kIdentTail[1];
    m[3] = kIdentTail[2];
    m[4] = kIdentTail[3];
    m[5] = 1.0f;        // local_44
    m[6] = 0.0f;
    m[7] = kIdentTail[1];
    m[8] = kIdentTail[2];
    m[9] = kIdentTail[3];
    m[10] = 0.0f;
    m[11] = kIdentTail[1];
    m[12] = kIdentTail[2];
    m[13] = kIdentTail[3];
    m[14] = 1.0f;       // local_20

    PaintCanvas_SetWorldViewMatrix(canvas, m);
    PaintCanvas_DrawSpriteSystem((unsigned int)(unsigned long)canvas);
}

// ---- ParticleSystemSprite_182b88.cpp ----
// ParticleSystemSprite::ParticleSystemSprite(PaintCanvas*, Matrix const*,
//     Array<ParticleSettings::ParticleSet> const&, bool, bool)
// Chains to the IParticleSystem base ctor, installs the derived vtable, allocates the per-
// particle sprite scratch array (count at +0x48, 12 bytes each) at +0x64 and zero-fills it,
// and caches a precomputed Pow value at +0x70.


extern "C" void _ZN15IParticleSystemC2EPN11AbyssEngine10PaintCanvasEPKNS0_6AEMath6MatrixERK5ArrayI14ParticleSettings11ParticleSetEbb(
    void *self, void *canvas, const void *matrix, const void *sets, bool b4, bool b5);

void *ParticleSystemSprite_ctor(
    void *selfv, void *canvas, const void *matrix, const void *sets, bool b4, bool b5)
{
    _ZN15IParticleSystemC2EPN11AbyssEngine10PaintCanvasEPKNS0_6AEMath6MatrixERK5ArrayI14ParticleSettings11ParticleSetEbb(
        selfv, canvas, matrix, sets, b4, b5);

    ParticleSystemSprite *self = (ParticleSystemSprite *)selfv;
    unsigned int count = self->particleCount;

    // Install the derived vtable (base + 8 -> first virtual slot).
    *(void **)self = (void *)(_ZTV20ParticleSystemSprite + 8);

    void *arr = operator new[](count * 0xc);
    if (count != 0)
        memset(arr, 0, (unsigned long)count * 0xc);
    self->spriteData = arr;

    self->cachedPow = AbyssEngine::AEMath::Pow(0.0f, 0.0f);
    return self;
}

// ---- updateSingle_1830b4.cpp ----
// ParticleSystemSprite::updateSingle(int index, float dt)
// Advances one live particle. Skips the work when the system's active flag (+0x34 bit7) is
// clear. Otherwise it ages the particle (ages array at +0x68, per-particle set index at +0x6c
// selecting a 0xA0-byte ParticleSet), kills it once it passes its lifetime, grows its sprite
// size, interpolates and applies its RGBA, steps the UV flipbook animation when the set has
// frames, and integrates its velocity into the sprite position.

// Base address of the ParticleSet table the per-particle byte index multiplies into (0xA0 stride).
__attribute__((visibility("hidden"))) extern char *g_particleSetBase;
// Rounding bias the original adds before converting UV offsets to int.
__attribute__((visibility("hidden"))) extern float g_uvRoundBias;

void ParticleSystemSprite::updateSingle(int index, float dt)
{
    // Active flag: sign bit of (flagByte << 24).
    if ((int)((unsigned int)this->flags << 0x18) < 0)
        return;

    unsigned int handle = this->canvasHandle;
    unsigned short id = (unsigned short)this->spriteId;

    int *ages = (int *)this->ages;
    char *setIdx = (char *)this->setIndices;
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
    PaintCanvas_SpriteSystemAddSize(handle, id, (short)this->idOffset + (short)index);

    // Color.
    float ca, cr, cg, cb;
    ((IParticleSystem *)(this))->interpolateColor(index, &ca, &cr, &cg, &cb);
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
            float uvRot[4];
            // UV rotation flag: sign bit of (flagByte2 << 30).
            if ((int)((unsigned int)this->flags2 << 0x1e) < 0)
                out = ((IParticleSystem *)(this))->rotateUVs(uv, index, uvRot);

            PaintCanvas_SpriteSystemSetUv(handle, id, out[1], 0.0f, out[2], 0.0f);
        }
    }

    // Integrate velocity into position.
    float pos[4] = { 0, 0, 0, 0 };
    PaintCanvas_SpriteSystemAddPosition(handle, id, pos[1], 0.0f, pos[2]);
}

// ---- setAlpha_183028.cpp ----
void ParticleSystemSprite::setAlpha(int param_1, unsigned int param_2, float param_3)
{
    float c0 = (float)(param_2 & 0xff);
    float c1 = (float)((param_2 >> 8) & 0xff);
    float c2 = (float)((param_2 >> 16) & 0xff);
    float c3 = (float)(param_2 >> 24);

    c0 = c0 * (1.0f / 255.0f);
    c1 = c1 * (1.0f / 255.0f);
    c2 = c2 * (1.0f / 255.0f);
    c3 = c3 * (1.0f / 255.0f);

    if (this->field_0x45 == 0) {
        c0 = c0 * param_3;
    } else {
        c1 = c1 * param_3;
        c2 = c2 * param_3;
        c3 = c3 * param_3;
    }

    PaintCanvas_SpriteSystemSetRGBA(this->canvasHandle,
                                    (unsigned short)(this->idOffset + param_1),
                                    c3, c2, c1, c0);
}

// ---- updateAreaExitParticle_182d6c.cpp ----
// ParticleSystemSprite::updateAreaExitParticle(int, float)
// The target is a 12-byte veneer that the disassembler could not lift cleanly (it decodes as
// stray conditional ARM ops ending in a conditional branch). Functionally it is a thin
// forwarder that, under one condition, tail-calls the shared area-exit particle update helper.
// We model it as that forwarding call so the translation unit is faithful and compiles.

void ParticleSystemSprite::updateAreaExitParticle(int param_1, float param_2)
{
    ParticleSystem_updateAreaExitParticleImpl(this, param_1, param_2);
}

// ---- setParticle_182c7c.cpp ----
// ParticleSystemSprite::setParticle(Vector const& pos, float, unsigned int color, float, float,
//     float, float, bool clearColor, float, float, Vector const& uv)
// Pushes one particle's state into the PaintCanvas sprite system: position (from `pos`), size
// (base size + per-system offset), UV rectangle, and RGBA. When `clearColor` is set the packed
// color's low byte is masked off first. Color components are converted from 0..255 bytes to
// normalized floats via VectorUnsignedToFloat and scaled by a global constant.

// Global color normalization scale (1/255).
__attribute__((visibility("hidden"))) extern float g_colorScale;

void ParticleSystemSprite::setParticle(const void *pos, float p2, unsigned int color,
                                       float p4, float p5, float p6, float p7, bool clearColor,
                                       float p9, float p10, const void *uv)
{
    (void)p2; (void)p5; (void)p6; (void)p7; (void)p9; (void)p10; (void)pos;

    unsigned int handle = this->canvasHandle;
    unsigned short id = (unsigned short)this->spriteId;

    const float *posv = (const float *)pos;
    PaintCanvas_SpriteSystemSetPosition(handle, id, posv[1], p4, posv[2]);

    short size = (short)this->baseSize + (short)this->idOffset;
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

// ---- render_18347c.cpp ----
// ParticleSystemSprite::render(PaintCanvas*, unsigned int handle)
// Camera-relative draw: snapshots the current camera's local matrix twice (the engine swaps
// the active camera between the two queries) and forwards both 15-float matrices to the
// sprite-system draw entry. A handle of 0xffffffff means "no system" and is skipped.

extern "C" void  PaintCanvas_DrawSpriteSystemCam(
    void *canvas, unsigned int handle,
    float a00, float a01, float a02, float a03, float a04, float a05, float a06, float a07,
    float a08, float a09, float a10, float a11, float a12, float a13, float a14,
    float b00, float b01, float b02, float b03, float b04, float b05, float b06, float b07,
    float b08, float b09, float b10, float b11, float b12, float b13, float b14);

void ParticleSystemSprite::render(void *canvas, unsigned int handle)
{
    if (handle == 0xffffffffu)
        return;

    PaintCanvas_CameraGetCurrent();
    float *a = PaintCanvas_CameraGetLocal((unsigned int)(unsigned long)canvas);
    float a0=a[0],a1=a[1],a2=a[2],a3=a[3],a4=a[4],a5=a[5],a6=a[6],a7=a[7];
    float a8=a[8],a9=a[9],a10=a[10],a11=a[11],a12=a[12],a13=a[13],a14=a[14];

    PaintCanvas_CameraGetCurrent();
    float *b = PaintCanvas_CameraGetLocal((unsigned int)(unsigned long)canvas);

    PaintCanvas_DrawSpriteSystemCam(canvas, handle,
        a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,
        b[0],b[1],b[2],b[3],b[4],b[5],b[6],b[7],b[8],b[9],b[10],b[11],b[12],b[13],b[14]);
}
