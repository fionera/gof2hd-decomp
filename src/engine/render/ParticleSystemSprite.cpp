#include "gof2/engine/render/ParticleSystemSprite.h"
#include "gof2/game/core/PaintCanvasClass.h"
#include "gof2/externs.h"
#include "gof2/platform/libc.h"

// ParticleSystemSprite derives (in the shipped binary) from IParticleSystem. In this port the
// class is kept flat with named members, so the few base-subobject operations are reached through
// typed helper symbols rather than a base-class cast.
extern "C" uint32_t g_ParticleSystemSprite_vtable[];
extern "C" void _pss_base_ctor(ParticleSystemSprite *self, PaintCanvas *canvas, const Matrix *matrix,
                               const void *sets, bool mirror, bool alphaFade);
extern "C" void _pss_base_dtor(ParticleSystemSprite *self);
extern "C" void _pss_interpolateColor(ParticleSystemSprite *self, int index, float *alpha, float *red,
                                      float *green, float *blue);
extern "C" float *_pss_rotateUVs(ParticleSystemSprite *self, float *src, int index, float *dst);
extern "C" void ParticleSystem_updateAreaExitParticleImpl(ParticleSystemSprite *self, int index, float dt);

// Fixed-point helpers the engine emitted as explicit calls.
extern "C" float VectorSignedToFloat(int v, uint8_t mode);
extern "C" float VectorUnsignedToFloat(uint32_t v, uint8_t mode);

// Base address of the ParticleSet table the per-particle byte index multiplies into (0xA0 stride).
extern "C" char *g_particleSetBase;
// Rounding bias the original adds before converting UV offsets to int.
extern "C" float g_uvRoundBias;
// Colour normalization scale (1/255).
extern "C" float g_colorScale;

// Re-installs the derived vtable, frees the per-particle sprite scratch arrays via release(), then
// chains into the IParticleSystem base destructor to tear down the emitter/particle-set state.
ParticleSystemSprite::~ParticleSystemSprite()
{
    this->vtable = g_ParticleSystemSprite_vtable + 2;
    this->release();
    _pss_base_dtor(this);
}

// Chains to the IParticleSystem base ctor, installs the derived vtable, allocates the per-particle
// sprite scratch array (particleCount * 12 bytes) and zero-fills it, then caches a precomputed Pow
// value.
ParticleSystemSprite::ParticleSystemSprite(PaintCanvas *canvas, const Matrix *matrix,
                                           const void *particleSets, bool mirror, bool alphaFade)
{
    _pss_base_ctor(this, canvas, matrix, particleSets, mirror, alphaFade);

    uint32_t count = (uint32_t)this->particleCount;

    this->vtable = g_ParticleSystemSprite_vtable + 2;

    char *arr = new char[count * 0xc];
    if (count != 0)
        memset(arr, 0, (size_t)count * 0xc);
    this->spriteData = arr;

    this->cachedPow = AbyssEngine::AEMath::Pow(0.0f, 0.0f);
}

void ParticleSystemSprite::reset()
{
    PaintCanvas *pc = (PaintCanvas *)g_PaintCanvas;
    for (int i = 0; i < this->particleCount; i++) {
        pc->SpriteSystemSetPosition(this->canvasHandle, (uint16_t)(this->idOffset + i),
                                    4294967296.0f, 4294967296.0f, 4294967296.0f);
        pc->SpriteSystemSetSize(this->canvasHandle, (uint16_t)(this->idOffset + i), 0);
        this->ages[i] = -1;
    }
    this->field_0x60 = 0;
    this->field_0x4 = 1;
}

int ParticleSystemSprite::init(uint32_t spriteId, uint16_t idOffset)
{
    void **vt = (void **)this->vtable;
    this->spriteId = spriteId;
    this->idOffset = idOffset;
    this->field_0x5c = 1;
    typedef int (*pfn)(ParticleSystemSprite *);
    return ((pfn)vt[2])(this);
}

void ParticleSystemSprite::release()
{
    delete[] (char *)this->spriteData;
    this->spriteData = nullptr;
    delete[] this->ages;
    this->ages = nullptr;
    delete[] this->setIndices;
    this->setIndices = nullptr;
}

// ParticleSystemSprite::render(PaintCanvas*, handle, texture, BlendMode)
// Draws the sprite system with an identity world transform: binds the texture and blend mode,
// builds an identity affine matrix on the stack, pushes it as the world-view matrix, and issues
// the sprite-system draw. A handle of 0xffffffff means "no system" and is skipped.
void ParticleSystemSprite::render(PaintCanvas *canvas, uint32_t handle, uint32_t texture, BlendMode blend)
{
    if (handle == 0xffffffffu)
        return;

    canvas->SetTexture(texture, 0);
    canvas->SetBlendMode(blend);

    // Identity affine transform (3x4 rows + trailing vector).
    float m[16] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
    };
    canvas->SetWorldViewMatrix(*(const AbyssEngine::AEMath::Matrix *)m);
    canvas->DrawSpriteSystem(handle);
}

// ParticleSystemSprite::updateSingle(index, dt)
// Advances one live particle. Skips the work when the system's active flag is clear. Otherwise it
// ages the particle, kills it once it passes its lifetime, grows its sprite size, interpolates and
// applies its RGBA, steps the UV flipbook animation when the set has frames, and integrates its
// velocity into the sprite position.
void ParticleSystemSprite::updateSingle(int index, float dt)
{
    // Active flag: sign bit of (flags << 24).
    if ((int)((uint32_t)this->flags << 0x18) < 0)
        return;

    PaintCanvas *pc = (PaintCanvas *)g_PaintCanvas;
    uint32_t handle = this->canvasHandle;
    uint16_t id = (uint16_t)this->spriteId;

    int *ages = this->ages;
    int8_t *setIdx = this->setIndices;
    char *set = g_particleSetBase + (int)setIdx[index] * 0xa0;

    // Age the particle.
    float age = VectorSignedToFloat(ages[index], 0);
    age = (float)(int)(age + dt);
    ages[index] = (int)age;

    int lifetime = *(int *)(set + 0x28);
    if ((int)age > lifetime) {
        ages[index] = -1;
        pc->SpriteSystemSetPosition(handle, id, age, 0.0f, 0.0f);
        return;
    }

    // Grow size.
    VectorSignedToFloat(*(int *)(set + 0x44), 0);
    pc->SpriteSystemAddSize(handle, id, (short)this->idOffset + (short)index);

    // Colour.
    float ca, cr, cg, cb;
    _pss_interpolateColor(this, index, &ca, &cr, &cg, &cb);
    pc->SpriteSystemSetRGBA(handle, id, cg, 0.0f, cb, 0.0f);

    // UV flipbook animation (when the set has frames at +0x9c).
    int frames = *(int *)(set + 0x9c);
    if (frames != 0) {
        int span = *(int *)(set + 0x28);
        int aged = ages[index] - 1;
        int cur  = (aged * frames) / span;
        int prevAged = (int)((float)aged - dt);
        int prev = (prevAged * frames) / span;
        if (prev < 0) prev = 0;

        if ((uint32_t)cur != (uint32_t)prev) {
            float fcur = VectorSignedToFloat(cur, 0);
            float du = *(float *)(set + 0x90) - *(float *)(set + 0x88);
            float dv = *(float *)(set + 0x94) - *(float *)(set + 0x8c);
            float u0 = *(float *)(set + 0x88) + du * fcur;
            float frac = VectorSignedToFloat((int)(u0 + g_uvRoundBias), 0);
            u0 = u0 - frac;
            float v0 = *(float *)(set + 0x8c) + dv * frac;
            float uv[4] = { u0, du + u0, v0, dv + v0 };

            float *out = uv;
            float uvRot[4];
            // UV rotation flag: sign bit of (flags2 << 30).
            if ((int)((uint32_t)this->flags2 << 0x1e) < 0)
                out = _pss_rotateUVs(this, uv, index, uvRot);

            pc->SpriteSystemSetUv(handle, id, out[1], 0.0f, out[2], 0.0f);
        }
    }

    // Integrate velocity into position.
    float pos[4] = { 0, 0, 0, 0 };
    pc->SpriteSystemAddPosition(handle, id, pos[1], 0.0f, pos[2]);
}

void ParticleSystemSprite::setAlpha(int index, uint32_t color, float alpha)
{
    float c0 = (float)(color & 0xff);
    float c1 = (float)((color >> 8) & 0xff);
    float c2 = (float)((color >> 16) & 0xff);
    float c3 = (float)(color >> 24);

    c0 = c0 * (1.0f / 255.0f);
    c1 = c1 * (1.0f / 255.0f);
    c2 = c2 * (1.0f / 255.0f);
    c3 = c3 * (1.0f / 255.0f);

    if (this->cAlphaChannelMode == 0) {
        c0 = c0 * alpha;
    } else {
        c1 = c1 * alpha;
        c2 = c2 * alpha;
        c3 = c3 * alpha;
    }

    ((PaintCanvas *)g_PaintCanvas)->SpriteSystemSetRGBA(this->canvasHandle,
                                    (uint16_t)(this->idOffset + index), c3, c2, c1, c0);
}

// The target is a 12-byte veneer that does not lift cleanly; functionally it is a thin forwarder
// that tail-calls the shared area-exit particle update helper.
void ParticleSystemSprite::updateAreaExitParticle(int index, float dt)
{
    ParticleSystem_updateAreaExitParticleImpl(this, index, dt);
}

// ParticleSystemSprite::setParticle(pos, _, color, _, ..., clearColor, _, _, uv)
// Pushes one particle's state into the PaintCanvas sprite system: position (from `pos`), size
// (base size + per-system offset), UV rectangle, and RGBA. When `clearColor` is set the packed
// colour's low byte is masked off first.
void ParticleSystemSprite::setParticle(const Vector &pos, float p2, uint32_t color, float p4,
                                       float p5, float p6, float p7, bool clearColor, float p9,
                                       float p10, const Vector &uv)
{
    (void)p2; (void)p5; (void)p6; (void)p7; (void)p9; (void)p10;

    PaintCanvas *pc = (PaintCanvas *)g_PaintCanvas;
    uint32_t handle = this->canvasHandle;
    uint16_t id = (uint16_t)this->spriteId;

    pc->SpriteSystemSetPosition(handle, id, pos.y, p4, pos.z);

    short size = (short)this->baseSize + (short)this->idOffset;
    pc->SpriteSystemSetSize(handle, id, size);

    pc->SpriteSystemSetUv(handle, id, uv.x, uv.y, uv.z, ((const float *)&uv)[3]);

    if (clearColor)
        color &= 0xffffff00;

    float a = VectorUnsignedToFloat(color >> 0x18, 0);
    float b = VectorUnsignedToFloat((color & 0xffff) >> 8, 0);
    float g = VectorUnsignedToFloat(color & 0xff, 0);
    float r = VectorUnsignedToFloat((color & 0xffffff) >> 0x10, 0);
    (void)a; (void)b; (void)g;

    pc->SpriteSystemSetRGBA(handle, id, r * g_colorScale, g, g_colorScale, b);
}

// ParticleSystemSprite::render(PaintCanvas*, handle)
// Camera-relative draw: snapshots the current camera's local matrix twice (the engine swaps the
// active camera between the two queries) and forwards both 15-float matrices to the sprite-system
// draw entry. A handle of 0xffffffff means "no system" and is skipped.
void ParticleSystemSprite::render(PaintCanvas *canvas, uint32_t handle)
{
    if (handle == 0xffffffffu)
        return;

    float *a = (float *)canvas->CameraGetLocal(canvas->CameraGetCurrent());
    float am[15] = { a[0], a[1], a[2],  a[3],  a[4],  a[5],  a[6], a[7],
                     a[8], a[9], a[10], a[11], a[12], a[13], a[14] };

    float *b = (float *)canvas->CameraGetLocal(canvas->CameraGetCurrent());

    canvas->DrawSpriteSystem(handle, am, b);
}
