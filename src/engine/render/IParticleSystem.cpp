#include "engine/render/IParticleSystem.h"
#include <new>
#include <cstring>
#include "engine/core/AERandom.h"
#include "engine/render/ParticleSettingsRef.h"

namespace AbyssEngine {
}

namespace AbyssEngine {
    namespace AEMath {
        Vector operator-(const Vector &);

        Vector operator-(const Vector &, const Vector &);

        Vector operator+(const Vector &, const Vector &);

        Vector operator*(const Vector &, float);

        Vector operator*(float, const Vector &);

        float VectorDot(const Vector &, const Vector &);
    }
}



void IParticleSystem::enableUpdate(bool enabled) {
    this->updateEnabled = enabled;
}

int IParticleSystem::getParticleCount() {
    return this->maxParticles;
}

void IParticleSystem::setParticleSet(ParticleSettings::ParticleSet set) {
    if (!this->particleSets.empty() && this->particleSets[0] == set) {
        this->particleSetIndex = 0;
    }
}

void IParticleSystem::enableEmit(bool enabled) {
    if (enabled && this->emitEnabled == 0) {
        this->emitTimer = 0;
    }
    this->emitEnabled = enabled;
}

void IParticleSystem::update(int delta) {
    if (this->updateEnabled != 0) {
        float fdelta = (float) delta;
        for (int i = 0; i < this->maxParticles; ++i) {
            if (this->particleAges[i] != -1) {
                this->updateSingle(i, fdelta);
            }
        }
    }
}

void IParticleSystem::setParticleSetIndex(uint8_t index) {
    this->particleSetIndex = index;
}

void IParticleSystem::setMatrix(Matrix const *matrix) {
    this->matrix = matrix;
    this->field_0x4 = 0;
    this->emitterVelocityDirty = 1;
}

void IParticleSystem::enableRender(bool enabled) {
    if (!enabled && this->renderEnabled != 0) {
        this->reset();
    }
    this->renderEnabled = enabled;
}

namespace {
    inline Vector makeVector(float x, float y, float z) {
        Vector v = {x, y, z};
        return v;
    }

    // Quake-style fast reciprocal square root (one Newton step).
    inline float invSqrt(float x) {
        int bits;
        memcpy(&bits, &x, sizeof(bits));
        bits = 0x5f3759df - (bits >> 1);
        float y;
        memcpy(&y, &bits, sizeof(y));
        return (x * -0.5f * y * y + 1.5f) * y;
    }
}

void IParticleSystem::emit(int delta) {
    if (this->emitEnabled == 0 || this->renderEnabled == 0)
        return;
    int set = this->particleSets[this->particleSetIndex];
    if (set == -1)
        return;
    if ((this->flags & 0x80) != 0) {
        if ((this->flags & 0x100) != 0 || this->particleAges[0] != -1)
            return;
    } else if ((this->flags & 0x100) != 0) {
        return;
    }

    Vector position = AbyssEngine::AEMath::MatrixGetPosition(*this->matrix);
    Vector right = AbyssEngine::AEMath::MatrixGetRight(*this->matrix);
    if (this->mirror != 0)
        right = -right;
    Vector up = AbyssEngine::AEMath::MatrixGetUp(*this->matrix);
    Vector dir = AbyssEngine::AEMath::MatrixGetDir(*this->matrix);

    const ParticleSettings::SetDefinition &def = reinterpret_cast<ParticleSettings &>(ParticleSettingsRef::cur).sets[set];
    float speedSq = AbyssEngine::AEMath::VectorDot(this->emitterVelocity, this->emitterVelocity);
    if (speedSq < (float) def.speedThreshold)
        return;

    float fdelta = (float) delta;
    Vector travel = this->emitterVelocity * (this->emitTimer + fdelta) / 1000.0f;
    float invTravel = invSqrt(AbyssEngine::AEMath::VectorDot(travel, travel));
    float distance = 1.0f / invTravel;

    int count;
    if ((this->flags & 0x10) != 0) {
        float n = distance / def.flLifetime;
        count = (int) n;
        this->emitTimer = (this->emitTimer + fdelta) * (n - (float) count) / n;
    } else if ((this->flags & 0x20) != 0) {
        float elapsed = this->emitTimer + fdelta;
        count = (int) (def.flLifetime * elapsed * 0.001f);
        this->emitTimer = elapsed + (float) count * -1000.0f / def.flLifetime;
    } else {
        count = def.count;
        if ((this->flags & 0x40) != 0)
            this->emitEnabled = 0;
    }
    if (count < 1)
        return;

    Vector start = position - travel;
    float invSpeed = 0.0f;
    if ((this->flags & 0xc0) == 0)
        invSpeed = invSqrt(speedSq);

    float uv[4];
    float rotated[4];
    uv[0] = def.uvU0;
    uv[1] = def.uvU1;
    uv[2] = def.uvV0;
    uv[3] = def.uvV1;
    float *uvp = uv;

    float step = 1.5f;
    for (int i = 0; i < count; i++) {
        this->particleSetIds[this->currentParticle] = (int8_t) set;
        this->particleAges[this->currentParticle] = 0;
        if (((this->flags >> 24) & 0x2) != 0)
            uvp = rotateUVs(uv, this->currentParticle, rotated);

        int spread = def.velSpread;
        if (spread != 0) {
            float x = (float) (this->random.nextInt(spread * 2) - spread) + def.velBaseX;
            float y = (float) (this->random.nextInt(spread * 2) - spread) + def.velBaseY;
            float z = (float) (this->random.nextInt(spread * 2) - spread) + def.velBaseZ;
            this->particleVelocities[this->currentParticle] = makeVector(x, y, z);
        } else {
            this->particleVelocities[this->currentParticle] = makeVector(0.0f, 0.0f, 0.0f);
        }
        if (def.drag != 0.0f)
            this->particleVelocities[this->currentParticle] -= this->emitterVelocity * def.drag;
        if (def.velRight != 0.0f)
            this->particleVelocities[this->currentParticle] += right * def.velRight;
        if (def.velUp != 0.0f)
            this->particleVelocities[this->currentParticle] += up * def.velUp;
        if (def.velDir != 0.0f)
            this->particleVelocities[this->currentParticle] += dir * def.velDir;

        float phase;
        if (def.oneShot == 1)
            phase = (float) (i + 1);
        else
            phase = (float) i + (float) this->random.nextInt(10000) * 0.0001f;

        Vector pos = {0.0f, 0.0f, 0.0f};
        if ((this->flags & 0xc0) != 0) {
            pos = position;
            phase = 0.0f;
        } else if (distance < 1.0f) {
            pos = position;
            count = i + 1;
            phase = (float) (i + 1);
        } else {
            step = (this->flags & 0x10) != 0 ? def.flLifetime
                                              : distance / (float) count;
            pos = travel * (phase * step) * invTravel;
            pos = start + pos;
        }

        if ((this->flags & 0x80) != 0) {
            int range = (int) def.posRight;
            float x = (float) (this->random.nextInt(range * 2) - range);
            float y = (float) (this->random.nextInt(range * 2) - range);
            float z = (float) (this->random.nextInt(range * 2) - range);
            pos += makeVector(x, y, z);
        } else {
            if (def.posRight != 0.0f) {
                pos += right * def.posRight;
            }
            if (def.posUp != 0.0f) {
                pos += up * def.posUp;
            }
            if (def.posDir != 0.0f) {
                pos += dir * def.posDir;
            }
            if (def.posDirRandom != 0.0f) {
                pos += dir * (float) this->random.nextInt((int) def.posDirRandom);
            }
            if (def.posSpread != 0) {
                int range = def.posSpread;
                float x = (float) (this->random.nextInt(range * 2) - range);
                float z = (float) (this->random.nextInt(range * 2) - range);
                pos += Vector{x, 0.0f, z};
            }
            if (def.ySpread != 0)
                pos.y += (float) (this->random.nextInt(def.ySpread * 2) - def.ySpread);
        }

        float life = def.lifeBase;
        if (def.lifeRandom != 0) {
            float extra = (float) this->random.nextInt(def.lifeRandom);
            float uA = uvp[0];
            float uB = uvp[1];
            float vA = uvp[2];
            float vB = uvp[3];
            uint32_t color = def.color0;
            bool masked = def.fadeFrames > 0 || def.colorFlag > 0.0f;
            life += extra;
            float size0 = def.startSize + (float) this->random.nextInt(def.lifeRandom);
            float size1 = def.endSize + (float) this->random.nextInt(def.lifeRandom);
            Vector vel = def.velocityFromSlot != 0.0f
                         ? def.velocityFromSlot * this->particleVelocities[this->currentParticle]
                         : makeVector(0.0f, 0.0f, 0.0f);
            this->setParticle(pos, life, color, uA, vA, uB, vB, masked, size0, size1, vel);
        } else {
            float uA = uvp[0];
            float uB = uvp[1];
            float vA = uvp[2];
            float vB = uvp[3];
            uint32_t color = def.color0;
            bool masked = def.fadeFrames > 0 || def.colorFlag > 0.0f;
            float size1 = def.endSize;
            float size0 = def.startSize;
            Vector vel = def.velocityFromSlot != 0.0f
                         ? def.velocityFromSlot * this->particleVelocities[this->currentParticle]
                         : makeVector(0.0f, 0.0f, 0.0f);
            this->setParticle(pos, life, color, uA, vA, uB, vB, masked, size0, size1, vel);
        }

        if (def.drag != 0.0f) {
            this->particleVelocities[this->currentParticle] +=
                    this->emitterVelocity * def.drag * 2.0f;
        }

        float remaining = invSpeed * (step * ((float) count - phase) * 1000.0f);
        if (remaining > fdelta)
            remaining = fdelta;
        this->updateSingle(this->currentParticle, remaining);

        int next = this->currentParticle + 1;
        if (next >= this->maxParticles)
            next = 0;
        this->currentParticle = next;
    }
}

void IParticleSystem::interpolateColor(int index, float &alpha, float &red, float &green, float &blue) {
    int age = this->particleAges[index];
    const ParticleSettings::SetDefinition &def =
            reinterpret_cast<ParticleSettings &>(ParticleSettingsRef::cur).sets[this->particleSetIds[index]];

    float t = (float) age / (float) def.lifetime;
    t = t > 1.0f ? 1.0f : t;
    float inv = 1.0f - t;
    uint32_t c0 = def.color0;
    uint32_t c1 = def.color1;
    int fadeFrames = def.fadeFrames;
    alpha = (t * (float) (c1 >> 24) + inv * (float) (c0 >> 24)) * 0.003921569f;
    red = (t * (float) ((c1 >> 16) & 0xff) + inv * (float) ((c0 >> 16) & 0xff)) * 0.003921569f;
    green = (t * (float) ((c1 >> 8) & 0xff) + inv * (float) ((c0 >> 8) & 0xff)) * 0.003921569f;
    blue = (t * (float) (c1 & 0xff) + inv * (float) (c0 & 0xff)) * 0.003921569f;

    if (age < fadeFrames) {
        float fade = (float) age / (float) fadeFrames;
        if (this->alphaFade != 0) {
            alpha *= fade;
            red *= fade;
            green *= fade;
        } else {
            blue *= fade;
        }
    }
}

IParticleSystem::IParticleSystem(PaintCanvas *canvas, Matrix const *matrix,
                                 Array<ParticleSettings::ParticleSet> const &sets,
                                 bool mirror, bool alphaFade)
    : canvas(canvas), random(), matrix(matrix), emitterVelocity(),
      lastEmitterPosition(), particleSets() {
    this->alphaFade = alphaFade;
    this->mirror = mirror;
    ArraySet(sets, this->particleSets);

    this->currentParticle = 0;
    this->resource = -1;
    this->idOffset = -1;
    this->emitEnabled = 1;
    this->renderEnabled = 1;
    this->updateEnabled = 1;
    this->maxParticles = 0;
    this->flags = 0;

    for (uint32_t i = 0; i < sets.size(); i++) {
        int set = sets[i];
        if (set != -1) {
            const ParticleSettings::SetDefinition &def =
                    reinterpret_cast<ParticleSettings &>(ParticleSettingsRef::cur).sets[set];
            this->maxParticles = this->maxParticles > def.count ? this->maxParticles : def.count;
            if (this->flags == 0) {
                this->flags = def.flags;
            }
        }
    }

    this->particleSetIndex = 0;
    this->emitTimer = 0;
    this->particleAges = new int[(uint32_t) this->maxParticles];
    this->particleSetIds = new int8_t[this->maxParticles];

    for (int i = 0; i < this->maxParticles; ++i) {
        this->particleSetIds[i] = (int8_t) 200;
    }

    this->initialized = 0;
    this->field_0x4 = 1;
    this->emitterVelocityDirty = 1;
}

void IParticleSystem::calcEmitterVelocity(int delta) {
    Vector position = AbyssEngine::AEMath::MatrixGetPosition(*this->matrix);
    Vector scaled = (position - this->lastEmitterPosition) * (1000.0f / (float) delta);
    this->emitterVelocity = scaled;
    this->emitterVelocityDirty = 0;
    this->lastEmitterPosition = position;
}

void IParticleSystem::emitManual(Vector position, int particleSet, Vector const *velocity, float lifetime) {
    if (particleSet == -1)
        return;

    int set = this->particleSets[particleSet];
    this->particleSetIds[this->currentParticle] = (int8_t) set;
    const ParticleSettings::SetDefinition &def = reinterpret_cast<ParticleSettings &>(ParticleSettingsRef::cur).sets[set];
    this->particleAges[this->currentParticle] = 0;

    float uv[4];
    float rotated[4];
    uv[0] = def.uvU0;
    uv[1] = def.uvU1;
    uv[2] = def.uvV0;
    uv[3] = def.uvV1;
    float *uvp = uv;
    if (((this->flags >> 24) & 0x2) != 0)
        uvp = rotateUVs(uv, this->currentParticle, rotated);

    int spread = def.velSpread;
    Vector tmp = spread != 0
                 ? Vector{(float) (this->random.nextInt(spread * 2) - spread),
                          def.velBaseY +
                          (float) (this->random.nextInt(spread * 2) - spread),
                          (float) (this->random.nextInt(spread * 2) - spread)}
                 : Vector{0.0f, 0.0f, 0.0f};
    this->particleVelocities[this->currentParticle] = tmp;

    if (velocity != nullptr && def.drag != 0.0f) {
        tmp = *velocity * def.drag;
        this->particleVelocities[this->currentParticle] -= tmp;
    }

    if (def.posSpread != 0) {
        int spread = def.posSpread;
        tmp.x = (float) (this->random.nextInt(spread * 2) - spread);
        tmp.y = 0.0f;
        tmp.z = (float) (this->random.nextInt(spread * 2) - spread);
        position += tmp;
    }

    if (def.ySpread != 0)
        position.y += (float) (this->random.nextInt(def.ySpread * 2) - def.ySpread);

    if (!(lifetime >= 0.0f))
        lifetime = def.lifeBase;

    if (def.lifeRandom != 0) {
        float life = lifetime + (float) this->random.nextInt(def.lifeRandom);
        float uA = uvp[0];
        float uB = uvp[1];
        float vA = uvp[2];
        float vB = uvp[3];
        uint32_t color = def.color0;
        int fadeFrames = def.fadeFrames;
        float size0 = def.startSize + (float) this->random.nextInt(def.lifeRandom);
        float size1 = def.endSize + (float) this->random.nextInt(def.lifeRandom);
        Vector vel = def.velocityFromSlot != 0.0f
                     ? def.velocityFromSlot * this->particleVelocities[this->currentParticle]
                     : Vector{0.0f, 0.0f, 0.0f};
        this->setParticle(position, life, color, uA, vA, uB, vB, fadeFrames > 0, size0, size1, vel);
    } else {
        float velocityFromSlot = def.velocityFromSlot;
        float size0 = def.startSize;
        float size1 = def.endSize;
        int fadeFrames = def.fadeFrames;
        uint32_t color = def.color0;
        float uA = uvp[0];
        float uB = uvp[1];
        float vA = uvp[2];
        float vB = uvp[3];
        Vector vel = velocityFromSlot != 0.0f
                     ? velocityFromSlot * this->particleVelocities[this->currentParticle]
                     : Vector{0.0f, 0.0f, 0.0f};
        this->setParticle(position, lifetime, color, uA, vA, uB, vB, fadeFrames > 0, size0, size1, vel);
    }

    if (def.drag != 0.0f) {
        tmp = this->emitterVelocity * def.drag * 2.0f;
        this->particleVelocities[this->currentParticle] += tmp;
    }

    int next = this->currentParticle + 1;
    if (next >= this->maxParticles)
        next = 0;
    this->currentParticle = next;
}

void IParticleSystem::resetEmitterVelocity() {
    {
        Vector value = {};
        this->emitterVelocity = value;
    }
    this->emitterVelocityDirty = 1;
    {
        Vector value = AbyssEngine::AEMath::MatrixGetPosition(*this->matrix);
        this->lastEmitterPosition = value;
    }
    this->field_0x4 = 0;
}
