#include "engine/render/ParticleSystemSprite.h"
#include "engine/render/ParticleSettingsRef.h"

#define GOF2_ENUM_BlendMode
#include "engine/render/PaintCanvas.h"




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
    this->emitTimer = 0;
    this->field_0x4 = 1;
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
    if (handle == 0xffffffffu)
        return;

    canvas->SetTexture(texture, 0xffffffffu);
    canvas->SetBlendMode(blend);

    float m[15];

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
    if ((int) (this->flags << 24) < 0) {
        this->updateAreaExitParticle(index, dt);
        return;
    }

    const ParticleSettings::SetDefinition &def =
            reinterpret_cast<ParticleSettings &>(ParticleSettingsRef::cur).sets[this->particleSetIds[index]];
    this->particleAges[index] = (int) ((float) this->particleAges[index] + dt);
    if (def.lifetime < this->particleAges[index]) {
        this->particleAges[index] = -1;
        this->canvas->SpriteSystemSetPosition(this->resource, (uint16_t) (this->idOffset + index),
                                              4294967296.0f, 4294967296.0f, 4294967296.0f);
        this->canvas->SpriteSystemSetSize(this->resource, (uint16_t) (this->idOffset + index), 0);
        return;
    }

    this->canvas->SpriteSystemAddSize(this->resource, (uint16_t) (this->idOffset + index),
                                      (short) ((float) def.posBase * dt * 0.001f));

    float alpha;
    float red;
    float green;
    float blue;
    this->interpolateColor(index, alpha, red, green, blue);
    this->canvas->SpriteSystemSetRGBA(this->resource, (uint16_t) (this->idOffset + index),
                                      alpha, red, green, blue);

    if (def.frames != 0) {
        int age = this->particleAges[index] - 1;
        int frame = age * def.frames / def.lifetime;
        int prevFrame = (age - (int) dt) * def.frames / def.lifetime;
        if (prevFrame < 0)
            prevFrame = 0;
        if (frame != prevFrame) {
            float u0 = reinterpret_cast<const float &>(def.uvU0);
            float v0 = reinterpret_cast<const float &>(def.uvV0);
            float du = reinterpret_cast<const float &>(def.uvU1) - u0;
            float dv = reinterpret_cast<const float &>(def.uvV1) - v0;
            float u = u0 + du * (float) frame;
            float row = (float) (int) (u + 0.0009765625f);
            u -= row;
            float v = v0 + dv * row;
            float uv[4] = {u, du + u, v, dv + v};
            float rotated[4];
            float *out = uv;
            if ((this->flags >> 24) & 2)
                out = this->rotateUVs(uv, index, rotated);
            this->canvas->SpriteSystemSetUv(this->resource, (uint16_t) (this->idOffset + index),
                                            out[0], out[2], out[1], out[3]);
        }
    }

    Vector step = this->particleVelocities[index] * dt * 0.001f;
    this->canvas->SpriteSystemAddPosition(this->resource, (uint16_t) (this->idOffset + index),
                                          step.x, step.y, step.z);
}

void ParticleSystemSprite::enable(bool enabled) {
    (void) enabled;
}

void ParticleSystemSprite::updateAreaExitParticle(int index, float dt) {
    char *set = (char *) ParticleSettingsRef::cur + (int) this->particleSetIds[index] * 0xa0;

    Vector step = this->particleVelocities[index] * dt * 0.001f;
    this->canvas->SpriteSystemAddPosition(this->resource, (uint16_t)(this->idOffset + index),
                                          step.x, step.y, step.z);

    Vector pos = {0.0f, 0.0f, 0.0f};
    this->canvas->SpriteSystemGetPosition(this->resource, (uint16_t)(this->idOffset + index), pos);

    Vector d = pos - AbyssEngine::AEMath::MatrixGetPosition(*this->matrix);
    float dist = d.x * d.x + d.y * d.y + d.z * d.z;

    float outer = *(float *) (set + 0x78) * *(float *) (set + 0x78);
    if (dist > outer) {
        this->setAlpha(index, *(uint32_t *) (set + 0x34), 0.0f);

        if (dist > outer * 1.01f) {
            float x = (float) (this->random.nextInt(2000) - 1000) * 0.001f;
            float y = (float) (this->random.nextInt(2000) - 1000) * 0.001f;
            float z = (float) (this->random.nextInt(2000) - 1000) * 0.001f;
            float w = (float) (this->random.nextInt(2000) - 1000) * 0.001f;

            float len = x * x + y * y + z * z + w * w;
            if (len < 1.0f)
                return;

            float a = z * w - x * y;
            float b = y * w + x * z;

            Vector dir;
            dir.x = (b + b) / len;
            dir.y = (a + a) / len;
            dir.z = (x * x + w * w - y * y - z * z) / len;

            dir *= *(float *) (set + 0x78);
            dir += AbyssEngine::AEMath::MatrixGetPosition(*this->matrix);
            this->canvas->SpriteSystemSetPosition(this->resource,
                                                  (uint16_t)(this->idOffset + index),
                                                  dir.x, dir.y, dir.z);

            Vector vel;
            ((uint32_t *) &vel)[0] = *(uint32_t *) (set + 0x58);
            ((uint32_t *) &vel)[1] = *(uint32_t *) (set + 0x5c);
            ((uint32_t *) &vel)[2] = *(uint32_t *) (set + 0x60);
            this->particleVelocities[index] = vel;
        }
        return;
    }

    float alpha;
    float mid = *(float *) (set + 0x7c) * *(float *) (set + 0x7c);
    if (dist > mid) {
        alpha = (outer - dist) / (outer - mid);
    } else {
        float inner = *(float *) (set + 0x80) * *(float *) (set + 0x80);
        if (dist > inner) {
            this->setAlpha(index, *(uint32_t *) (set + 0x34), 1.0f);
            return;
        }

        float core = *(float *) (set + 0x40) * *(float *) (set + 0x40);
        if (dist > core) {
            alpha = (dist - core) / (inner - core);
        } else {
            this->setAlpha(index, *(uint32_t *) (set + 0x34), 0.0f);
            return;
        }
    }
    this->setAlpha(index, *(uint32_t *) (set + 0x34), alpha);
}

void ParticleSystemSprite::setParticle(const Vector &pos, float scale, uint32_t color, float u0,
                                       float v0, float u1, float v1, bool maskedColor, float size0,
                                       float size1, const Vector &velocity) {
    (void) size0;
    (void) size1;
    (void) velocity;

    this->canvas->SpriteSystemSetPosition(this->resource, (uint16_t) (this->currentParticle + this->idOffset),
                                          pos.x, pos.y, pos.z);
    this->canvas->SpriteSystemSetSize(this->resource, (uint16_t) (this->idOffset + this->currentParticle),
                                      (short) scale);
    this->canvas->SpriteSystemSetUv(this->resource, (uint16_t) (this->idOffset + this->currentParticle),
                                    u0, v0, u1, v1);
    if (maskedColor)
        color &= 0xffffff00;
    this->canvas->SpriteSystemSetRGBA(this->resource, (uint16_t) (this->idOffset + this->currentParticle),
                                      (float) (color >> 24) * 0.003921569f,
                                      (float) ((color >> 16) & 0xff) * 0.003921569f,
                                      (float) ((color >> 8) & 0xff) * 0.003921569f,
                                      (float) (color & 0xff) * 0.003921569f);
}

void ParticleSystemSprite::render(PaintCanvas *canvas, uint32_t handle) {
    if (handle == 0xffffffffu)
        return;

    canvas->DrawSpriteSystem(handle, *(const Matrix *) canvas->CameraGetLocal(canvas->CameraGetCurrent()),
                             *(const Matrix *) canvas->CameraGetLocal(canvas->CameraGetCurrent()));
}
