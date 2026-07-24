#include "engine/render/ParticleSystemSprite.h"
#include "engine/render/ParticleSettingsRef.h"

#define GOF2_ENUM_BlendMode
#include "engine/render/PaintCanvas.h"

void ParticleSystem_updateAreaExitParticleImpl(ParticleSystemSprite *self, int index, float dt);

using AbyssEngine::AEMath::VectorSignedToFloat;
using AbyssEngine::AEMath::VectorUnsignedToFloat;

static float g_uvRoundBias = 0.0f;

static float g_colorScale = 0.0f;

ParticleSystemSprite::~ParticleSystemSprite() {
    this->release();
}

ParticleSystemSprite::ParticleSystemSprite(PaintCanvas *canvas, const Matrix *matrix,
                                           const Array<ParticleSettings::ParticleSet> &particleSets,
                                           bool mirror, bool alphaFade)
    : IParticleSystem(canvas, matrix, particleSets, mirror, alphaFade) {
    this->particleVelocities = new Vector[(uint32_t) this->maxParticles]();
    this->cachedPow = AbyssEngine::AEMath::Pow(0.7f, 0.2f);
}

void ParticleSystemSprite::reset() {
    for (int i = 0; i < this->maxParticles; i++) {
        this->canvas->SpriteSystemSetPosition(this->resource, (uint16_t)(this->idOffset + i),
                                              4294967296.0f, 4294967296.0f, 4294967296.0f);
        this->canvas->SpriteSystemSetSize(this->resource, (uint16_t)(this->idOffset + i), 0);
        this->particleAges[i] = -1;
    }
    this->field_0x4 = 1;
    this->emitTimer = 0;
}

void ParticleSystemSprite::init(uint32_t spriteId, uint16_t idOffset) {
    this->resource = spriteId;
    this->idOffset = idOffset;
    this->initialized = 1;

    this->reset();
}

int ParticleSystemSprite::getQuadCount() {
    return this->maxParticles;
}

void ParticleSystemSprite::release() {
    delete[] this->particleVelocities;
    this->particleVelocities = nullptr;
    delete[] this->particleAges;
    this->particleAges = nullptr;
    delete[] this->particleSetIds;
    this->particleSetIds = nullptr;
}

void ParticleSystemSprite::render(PaintCanvas *canvas, uint32_t handle, uint32_t texture, BlendMode blend) {
    char mbuf[60];
    float *m = (float *) mbuf;
    if (handle == 0xffffffffu)
        return;

    canvas->SetTexture(texture, 0xffffffffu);
    canvas->SetBlendMode(blend);

    m[0] = 1.0f;
    m[1] = 0.0f;
    m[2] = 0.0f;
    m[3] = 0.0f;
    m[4] = 0.0f;
    m[5] = 1.0f;
    m[6] = 0.0f;
    m[7] = 0.0f;
    m[8] = 0.0f;
    m[9] = 0.0f;
    m[10] = 1.0f;
    m[11] = 0.0f;
    m[12] = 1.0f;
    m[13] = 1.0f;
    m[14] = 1.0f;
    canvas->SetWorldViewMatrix(*(const AbyssEngine::AEMath::Matrix *) m);
    canvas->DrawSpriteSystem(handle);
}

void ParticleSystemSprite::updateSingle(int index, float dt) {
    if ((int) (this->flags << 24) < 0)
        return;

    PaintCanvas *pc = this->canvas;
    uint32_t handle = this->resource;
    uint16_t id = (uint16_t) this->resource;

    int *ages = this->particleAges;
    int8_t *setIdx = this->particleSetIds;
    char *set = (char *) ParticleSettingsRef::cur + (int) setIdx[index] * 0xa0;

    float age = VectorSignedToFloat(ages[index], 0);
    age = (float) (int) (age + dt);
    ages[index] = (int) age;

    int lifetime = *(int *) (set + 0x28);
    if ((int) age > lifetime) {
        ages[index] = -1;
        pc->SpriteSystemSetPosition(handle, id, age, 0.0f, 0.0f);
        return;
    }

    VectorSignedToFloat(*(int *) (set + 0x44), 0);
    pc->SpriteSystemAddSize(handle, id, (short) this->idOffset + (short) index);

    float ca, cr, cg, cb;
    this->interpolateColor(index, ca, cr, cg, cb);
    pc->SpriteSystemSetRGBA(handle, id, cg, 0.0f, cb, 0.0f);

    int frames = *(int *) (set + 0x9c);
    if (frames != 0) {
        int span = *(int *) (set + 0x28);
        int aged = ages[index] - 1;
        int cur = (aged * frames) / span;
        int prevAged = (int) ((float) aged - dt);
        int prev = (prevAged * frames) / span;
        if (prev < 0) prev = 0;

        if ((uint32_t) cur != (uint32_t) prev) {
            float fcur = VectorSignedToFloat(cur, 0);
            float du = *(float *) (set + 0x90) - *(float *) (set + 0x88);
            float dv = *(float *) (set + 0x94) - *(float *) (set + 0x8c);
            float u0 = *(float *) (set + 0x88) + du * fcur;
            float frac = VectorSignedToFloat((int) (u0 + g_uvRoundBias), 0);
            u0 = u0 - frac;
            float v0 = *(float *) (set + 0x8c) + dv * frac;
            float uv[4] = {u0, du + u0, v0, dv + v0};

            float *out = uv;
            float uvRot[4];

            if (((this->flags >> 24) & 0x2) != 0)
                out = this->rotateUVs(uv, index, uvRot);

            pc->SpriteSystemSetUv(handle, id, out[1], 0.0f, out[2], 0.0f);
        }
    }

    float pos[4] = {0, 0, 0, 0};
    pc->SpriteSystemAddPosition(handle, id, pos[1], 0.0f, pos[2]);
}

void ParticleSystemSprite::setAlpha(int index, uint32_t color, float alpha) {
    float c0 = (float) (color & 0xff);
    float c1 = (float) ((color >> 8) & 0xff);
    float c2 = (float) ((color >> 16) & 0xff);
    float c3 = (float) (color >> 24);

    c0 = c0 * (1.0f / 255.0f);
    c1 = c1 * (1.0f / 255.0f);
    c2 = c2 * (1.0f / 255.0f);
    c3 = c3 * (1.0f / 255.0f);

    if (this->alphaFade == 0) {
        c0 = c0 * alpha;
    } else {
        c1 = c1 * alpha;
        c2 = c2 * alpha;
        c3 = c3 * alpha;
    }

    this->canvas->SpriteSystemSetRGBA(this->resource,
                                      (uint16_t)(this->idOffset + index), c3, c2, c1, c0);
}

void ParticleSystemSprite::enable(bool enabled) {
    (void) enabled;
}

void ParticleSystemSprite::updateAreaExitParticle(int index, float dt) {
    ParticleSystem_updateAreaExitParticleImpl(this, index, dt);
}

void ParticleSystemSprite::setParticle(const Vector &pos, float p2, uint32_t color, float p4,
                                       float p5, float p6, float p7, bool clearColor, float p9,
                                       float p10, const Vector &uv) {
    (void) p2;
    (void) p5;
    (void) p6;
    (void) p7;
    (void) p9;
    (void) p10;

    PaintCanvas *pc = this->canvas;
    uint32_t handle = this->resource;
    uint16_t id = (uint16_t) this->resource;

    pc->SpriteSystemSetPosition(handle, id, pos.y, p4, pos.z);

    short size = (short) this->currentParticle + (short) this->idOffset;
    pc->SpriteSystemSetSize(handle, id, size);

    pc->SpriteSystemSetUv(handle, id, uv.x, uv.y, uv.z, ((const float *) &uv)[3]);

    if (clearColor)
        color &= 0xffffff00;

    float a = VectorUnsignedToFloat(color >> 0x18, 0);
    float b = VectorUnsignedToFloat((color & 0xffff) >> 8, 0);
    float g = VectorUnsignedToFloat(color & 0xff, 0);
    float r = VectorUnsignedToFloat((color & 0xffffff) >> 0x10, 0);
    (void) a;
    (void) b;
    (void) g;

    pc->SpriteSystemSetRGBA(handle, id, r * g_colorScale, g, g_colorScale, b);
}

void ParticleSystemSprite::render(PaintCanvas *canvas, uint32_t handle) {
    if (handle == 0xffffffffu)
        return;

    float *a = (float *) canvas->CameraGetLocal(canvas->CameraGetCurrent());
    Matrix am;
    for (int i = 0; i < 15; ++i)
        am.m[i] = a[i];

    float *b = (float *) canvas->CameraGetLocal(canvas->CameraGetCurrent());
    Matrix bm;
    for (int i = 0; i < 15; ++i)
        bm.m[i] = b[i];

    canvas->DrawSpriteSystem(handle, am, bm);
}
