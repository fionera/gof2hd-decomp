#include "engine/render/IParticleSystem.h"
#include <new>
#include "engine/core/AERandom.h"

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

static char *ParticleSet_definitions = nullptr;

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

static inline int float_bits(float v) {
    int i;
    __builtin_memcpy(&i, &v, sizeof(i));
    return i;
}

static inline float bits_float(int v) {
    float f;
    __builtin_memcpy(&f, &v, sizeof(f));
    return f;
}

static inline void zero_vec(char *v) {
    *(uint32_t *) (v + 0) = 0;
    *(uint32_t *) (v + 4) = 0;
    *(uint32_t *) (v + 8) = 0;
}

void IParticleSystem::emit(int delta) {
    if (this->emitEnabled == 0 || this->renderEnabled == 0) {
        return;
    }

    int set = this->particleSets[this->particleSetIndex];
    if (set == -1) {
        return;
    }

    uint32_t flags = this->flags;
    if ((flags & 0x80) == 0) {
        if ((flags & 0x100) != 0) {
            return;
        }
    } else if ((flags & 0x100) != 0 || this->particleAges[0] != -1) {
        return;
    }

    char travelDiv[12];
    char baseDelta[12];
    char uv[16];
    char rotated[16];

    Vector matrixPos = AbyssEngine::AEMath::MatrixGetPosition(*this->matrix);
    Vector right = AbyssEngine::AEMath::MatrixGetRight(*this->matrix);
    if (this->mirror != 0) {
        right = -right;
    }
    Vector up = AbyssEngine::AEMath::MatrixGetUp(*this->matrix);
    Vector dir = AbyssEngine::AEMath::MatrixGetDir(*this->matrix);

    char *def = ParticleSet_definitions + (set + set * 4) * 32;
    float speed2 = AbyssEngine::AEMath::VectorDot(this->emitterVelocity, this->emitterVelocity);
    if (speed2 < (float) *(int *) (def + 0x98)) {
        return;
    }

    float fdelta = (float) delta;
    float elapsed = this->emitTimer + fdelta;
    *(Vector *) travelDiv = this->emitterVelocity * elapsed;
    *(Vector *) travelDiv /= 1000.0f;
    float travelLen2 = AbyssEngine::AEMath::VectorDot(*(const Vector *) travelDiv, *(const Vector *) travelDiv);
    int travelBits = float_bits(travelLen2);
    float invGuess = bits_float(0x5f3759df - (travelBits >> 1));
    float invLen = (travelLen2 * -0.5f * invGuess * invGuess + 1.5f) * invGuess;
    float distance = 1.0f / invLen;

    int emitCount;
    if ((flags & 0x10) != 0) {
        float countf = distance / *(float *) (def + 0x2c);
        emitCount = (int) countf;
        this->emitTimer = (elapsed * (countf - (float) emitCount)) / countf;
    } else if ((flags & 0x20) != 0) {
        int cycles = (int) (*(float *) (def + 0x2c) * elapsed * 0.001f);
        this->emitTimer = elapsed + ((float) cycles * -1000.0f) / *(float *) (def + 0x2c);
        emitCount = cycles;
    } else {
        emitCount = *(int *) (def + 0x10);
        if ((flags & 0x40) != 0) {
            this->emitEnabled = 0;
        }
    }

    if (emitCount <= 0) {
        return;
    }

    *(Vector *) baseDelta = matrixPos - *(const Vector *) travelDiv;
    float pathScale = 0.0f;
    if ((this->flags & 0xc0) == 0) {
        int speedBits = float_bits(speed2);
        float y = bits_float(0x5f3759df - (speedBits >> 1));
        pathScale = (speed2 * -0.5f * y * y + 1.5f) * y;
    }

    // Cache UV data
    ((uint32_t *) uv)[0] = *(uint32_t *) (def + 0x88);
    ((uint32_t *) uv)[1] = *(uint32_t *) (def + 0x90);
    ((uint32_t *) uv)[2] = *(uint32_t *) (def + 0x8c);
    ((uint32_t *) uv)[3] = *(uint32_t *) (def + 0x94);

    // Pre-loop: cache pointers to def fields (forces compiler to hoist address computations)
    const int   *volatile pYSpread    = (const int *)   (def + 0x4c);
    const int   *volatile pPosSpread  = (const int *)   (def + 0x48);
    const float *volatile pRandDir    = (const float *) (def + 0x84);
    const float *volatile pFadeFloat  = (const float *) (def + 0x40);
    const float *volatile pVelScale   = (const float *) (def + 0x24);
    const float *volatile pSize1      = (const float *) (def + 0x20);
    const float *volatile pSize0      = (const float *) (def + 0x1c);
    const float *volatile pDirOffset  = (const float *) (def + 0x80);
    const int   *volatile pFadeFrames = (const int *)   (def + 0x3c);
    const uint32_t *volatile pColor0  = (const uint32_t *)(def + 0x34);
    const float *volatile pUpOffset   = (const float *) (def + 0x7c);
    const float *volatile pInterval   = (const float *) (def + 0x2c);
    const float *volatile pLife       = (const float *) (def + 0x14);
    const int   *volatile pRandomLife = (const int *)   (def + 0x18);
    const float *volatile pPosRange   = (const float *) (def + 0x78);
    const int   *volatile pPhaseMode  = (const int *)   (def + 0x30);
    const float *volatile pDirVel     = (const float *) (def + 0x70);
    const float *volatile pUpVel      = (const float *) (def + 0x6c);
    const float *volatile pRightVel   = (const float *) (def + 0x68);
    const float *volatile pVelBaseZ   = (const float *) (def + 0x60);
    const float *volatile pVelBaseY   = (const float *) (def + 0x5c);
    const float *volatile pVelBaseX   = (const float *) (def + 0x58);
    const float *volatile pDrag       = (const float *) (def + 0x64);
    const int   *volatile pVelSpread  = (const int *)   (def + 0x50);
    AbyssEngine::AERandom *volatile pRandom = &this->random;
    uint32_t *volatile uvp = (uint32_t *) uv;

    for (int i = 0; i < emitCount; ++i) {
        char particlePos[12];
        char velocity[12];
        int current = this->currentParticle;
        this->particleSetIds[current] = (int8_t) set;
        this->particleAges[current] = 0;
        if (((this->flags >> 24) & 0x2) != 0) {
            uvp = (uint32_t *) rotateUVs((float *) uv, current, (float *) rotated);
        }

        int velSpread = *pVelSpread;
        if (velSpread == 0) {
            zero_vec(velocity);
        } else {
            int range = velSpread << 1;
            AbyssEngine::AERandom *rnd = pRandom;
            float randX = (float) (rnd->nextInt(range) - velSpread);
            float velBaseX = *pVelBaseX;
            float randY = (float) (rnd->nextInt(range) - velSpread);
            float velBaseY = *pVelBaseY;
            float randZ = (float) (rnd->nextInt(range) - velSpread);
            float velBaseZ = *pVelBaseZ;
            ((float *) velocity)[0] = velBaseX + randX;
            ((float *) velocity)[1] = velBaseY + randY;
            ((float *) velocity)[2] = velBaseZ + randZ;
        }

        Vector &slot = this->particleVelocities[current];
        slot = *(Vector *) (velocity);

        float drag = *pDrag;
        if (drag != 0.0f) {
            *(Vector *) velocity = this->emitterVelocity * drag;
            slot -= *(Vector *) (velocity);
        }
        float rightVel = *pRightVel;
        if (rightVel != 0.0f) {
            *(Vector *) velocity = right * rightVel;
            slot += *(Vector *) (velocity);
        }
        float upVel = *pUpVel;
        if (upVel != 0.0f) {
            *(Vector *) velocity = up * upVel;
            slot += *(Vector *) (velocity);
        }
        float dirVel = *pDirVel;
        if (dirVel != 0.0f) {
            *(Vector *) velocity = dir * dirVel;
            slot += *(Vector *) (velocity);
        }

        float phase;
        if (*pPhaseMode == 1) {
            phase = (float) i;
        } else {
            phase = (float) i + (float) pRandom->nextInt(10000) * 0.0001f;
        }

        zero_vec(particlePos);
        float step = 1.5f;
        {
        char tmp[12];
        char tmp2[12];
        if ((this->flags & 0xc0) == 0) {
            if (distance >= 1.0f) {
                step = ((this->flags & 0x10) != 0)
                           ? *pInterval
                           : distance / (float) emitCount;
                *(Vector *) tmp = *(const Vector *) travelDiv * (phase * step);
                *(Vector *) tmp2 = *(const Vector *) tmp * pathScale;
                *(Vector *) (particlePos) = *(Vector *) (tmp2);
                *(Vector *) tmp2 = *(const Vector *) baseDelta + *(const Vector *) particlePos;
                *(Vector *) (particlePos) = *(Vector *) (tmp2);
            } else {
                *(Vector *) (particlePos) = matrixPos;
                phase = (float) (i + 1);
                emitCount = i + 1;
            }
        } else {
            *(Vector *) (particlePos) = matrixPos;
            phase = 0.0f;
        }

        if ((this->flags & 0x80) != 0) {
            int posRange = (int) *pPosRange;
            int range = posRange << 1;
            AbyssEngine::AERandom *rnd = pRandom;
            ((float *) tmp)[0] = (float) (rnd->nextInt(range) - posRange);
            ((float *) tmp)[1] = (float) (rnd->nextInt(range) - posRange);
            ((float *) tmp)[2] = (float) (rnd->nextInt(range) - posRange);
            *(Vector *) (particlePos) += *(Vector *) (tmp);
        } else {
            float posRange = *pPosRange;
            if (posRange != 0.0f) {
                *(Vector *) tmp = right * posRange;
                *(Vector *) (particlePos) += *(Vector *) (tmp);
            }
            float upOffset = *pUpOffset;
            if (upOffset != 0.0f) {
                *(Vector *) tmp = up * upOffset;
                *(Vector *) (particlePos) += *(Vector *) (tmp);
            }
            float dirOffset = *pDirOffset;
            if (dirOffset != 0.0f) {
                *(Vector *) tmp = dir * dirOffset;
                *(Vector *) (particlePos) += *(Vector *) (tmp);
            }
            float randDir = *pRandDir;
            if (randDir != 0.0f) {
                *(Vector *) tmp = dir * (float) pRandom->nextInt(
                                      (int) randDir);
                *(Vector *) (particlePos) += *(Vector *) (tmp);
            }
            int posSpread = *pPosSpread;
            if (posSpread != 0) {
                AbyssEngine::AERandom *rnd = pRandom;
                ((float *) tmp)[0] = (float) (rnd->nextInt(posSpread << 1) -
                                              posSpread);
                ((float *) tmp)[1] = 0.0f;
                ((float *) tmp)[2] = (float) (rnd->nextInt(posSpread << 1) -
                                              posSpread);
                *(Vector *) (particlePos) += *(Vector *) (tmp);
            }
            int ySpread = *pYSpread;
            if (ySpread != 0) {
                ((float *) particlePos)[1] +=
                        (float) (pRandom->nextInt(ySpread << 1) - ySpread);
            }
        }
        }

        {
        char emitVelocity[12];
        float life = *pLife;
        int randomLife = *pRandomLife;
        if (randomLife != 0) {
            float lifeRand = (float) pRandom->nextInt(randomLife);
            uint32_t *u = uvp;
            int uvp0 = u[0];
            int uvp1 = u[1];
            int uvp2 = u[2];
            int uvp3 = u[3];
            uint32_t color0 = *pColor0;
            int fadeFrames = *pFadeFrames;
            int colorFlag;
            if (fadeFrames > 0) {
                colorFlag = 1;
            } else {
                colorFlag = (*pFadeFloat > 0.0f) ? 1 : 0;
            }

            float size0base = *pSize0;
            AbyssEngine::AERandom *rnd = pRandom;
            const int *lifeBound = pRandomLife;
            life += lifeRand;
            float size0 = size0base + (float) rnd->nextInt(*lifeBound);
            float size1 = *pSize1 + (float) rnd->nextInt(*lifeBound);

            float velScale = *pVelScale;
            if (velScale == 0.0f) {
                zero_vec(emitVelocity);
            } else {
                *(Vector *) emitVelocity = velScale * slot;
            }

            this->setParticle(*(const Vector *) particlePos, life, color0,
                              bits_float(uvp0), bits_float(uvp2), bits_float(uvp1),
                              bits_float(uvp3), colorFlag != 0, size0, size1,
                              *(const Vector *) emitVelocity);
        } else {
            uint32_t *u = uvp;
            int uvp0 = u[0];
            int uvp1 = u[1];
            int uvp2 = u[2];
            int uvp3 = u[3];
            uint32_t color0 = *pColor0;
            int fadeFrames = *pFadeFrames;
            int colorFlag;
            if (fadeFrames > 0) {
                colorFlag = 1;
            } else {
                colorFlag = (*pFadeFloat > 0.0f) ? 1 : 0;
            }

            float size0 = *pSize0;
            float size1 = *pSize1;
            float velScale = *pVelScale;
            if (velScale == 0.0f) {
                zero_vec(emitVelocity);
            } else {
                *(Vector *) emitVelocity = velScale * slot;
            }

            this->setParticle(*(const Vector *) particlePos, life, color0,
                              bits_float(uvp0), bits_float(uvp2), bits_float(uvp1),
                              bits_float(uvp3), colorFlag != 0, size0, size1,
                              *(const Vector *) emitVelocity);
        }
        }

        float endDrag = *pDrag;
        if (endDrag != 0.0f) {
            char tmp[12];
            char tmp2[12];
            *(Vector *) tmp = this->emitterVelocity * endDrag;
            *(Vector *) tmp2 = *(const Vector *) tmp * 2.0f;
            slot += *(Vector *) (tmp2);
        }

        float remaining = pathScale * step * ((float) emitCount - phase) * 1000.0f;
        if (remaining > fdelta) {
            remaining = fdelta;
        }
        this->updateSingle(current, remaining);

        current = this->currentParticle + 1;
        if (this->maxParticles <= current) {
            current = 0;
        }
        this->currentParticle = current;
    }
}

void IParticleSystem::interpolateColor(int index, float &alpha, float &red, float &green, float &blue) {
    int age = this->particleAges[index];
    int setIndex = this->particleSetIds[index];
    char *def = ParticleSet_definitions + (setIndex + setIndex * 4) * 32;

    float t = (float) age / (float) *(int *) (def + 0x28);
    if (t > 1.0f) {
        t = 1.0f;
    }
    float inv = 1.0f - t;
    uint32_t c0 = *(uint32_t *) (def + 0x34);
    uint32_t c1 = *(uint32_t *) (def + 0x38);

    float a0 = (float) (c0 >> 24);
    float r0 = (float) ((c0 >> 16) & 0xff);
    float g0 = (float) ((c0 >> 8) & 0xff);
    float b0 = (float) (c0 & 0xff);
    float a1 = (float) (c1 >> 24);
    float r1 = (float) ((c1 >> 16) & 0xff);
    float g1 = (float) ((c1 >> 8) & 0xff);
    float b1 = (float) (c1 & 0xff);

    const float scale = 0.003921568859368563f;
    alpha = (inv * a0 + t * a1) * scale;
    red = (inv * r0 + t * r1) * scale;
    green = (inv * g0 + t * g1) * scale;
    blue = (inv * b0 + t * b1) * scale;

    int fadeFrames = *(int *) (def + 0x3c);
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

float *IParticleSystem::rotateUVs(float *src, int seed, float *dst) {
    AbyssEngine::AERandom random((long long) seed);
    unsigned value = (unsigned) random.nextInt(40000);
    unsigned inv = ~value;
    ((uint32_t *) dst)[0] = ((uint32_t *) src)[value & 1];
    ((uint32_t *) dst)[1] = ((uint32_t *) src)[inv & 1];
    ((uint32_t *) dst)[2] = *(uint32_t *) ((char *) src + (((value & 2) << 1) | 8));
    ((uint32_t *) dst)[3] = *(uint32_t *) ((char *) src + (((inv & 2) << 1) | 8));
    return dst;
}

IParticleSystem::IParticleSystem(PaintCanvas *canvas, Matrix const *matrix,
                                 Array<ParticleSettings::ParticleSet> const &sets,
                                 bool mirror, bool alphaFade)
    : canvas(canvas), random(), matrix(matrix), emitterVelocity(),
      lastEmitterPosition(), particleSets() {
    this->mirror = mirror;
    this->alphaFade = alphaFade;
    ArraySet(sets, this->particleSets);

    this->currentParticle = 0;
    this->resource = -1;
    this->idOffset = -1;
    this->emitEnabled = 1;
    this->renderEnabled = 1;
    this->updateEnabled = 1;
    this->maxParticles = 0;
    this->flags = 0;

    int count = sets.size();
    const ParticleSettings::ParticleSet *src = sets.data();
    int firstFlags = 0;
    uint32_t maxParticles = 0;
    while (count != 0) {
        int set = *src;
        if (set != -1) {
            char *def = ParticleSet_definitions + (set + set * 4) * 32;
            uint32_t particles = *(uint32_t *) (def + 0x10);
            if ((int) maxParticles <= (int) particles) {
                maxParticles = particles;
            }
            this->maxParticles = maxParticles;
            if (firstFlags == 0) {
                firstFlags = *(int *) (def + 0xc);
                this->flags = firstFlags;
            }
        }
        ++src;
        --count;
    }

    this->emitTimer = 0;
    this->particleSetIndex = 0;
    this->particleAges = new int[maxParticles];
    this->particleSetIds = new int8_t[maxParticles];

    for (int i = 0; i < (int) maxParticles; ++i) {
        this->particleSetIds[i] = (int8_t) 200;
        maxParticles = this->maxParticles;
    }

    this->initialized = 0;
    this->field_0x4 = 1;
    this->emitterVelocityDirty = 1;
}

void IParticleSystem::calcEmitterVelocity(int delta) {
    Vector position = AbyssEngine::AEMath::MatrixGetPosition(*this->matrix);
    Vector diff = position - this->lastEmitterPosition;
    Vector scaled = diff * (1000.0f / (float) delta);
    this->emitterVelocity = scaled;
    this->emitterVelocityDirty = 0;
    this->lastEmitterPosition = position;
}

void IParticleSystem::emitManual(Vector position, int particleSet, Vector const *velocity, float lifetime) {
    if (particleSet != -1) {
        int current = this->currentParticle;
        int set = this->particleSets[particleSet];
        this->particleSetIds[current] = (int8_t) set;
        char *def = ParticleSet_definitions + (set + set * 4) * 32;
        this->particleAges[current] = 0;

        uint32_t uv[4];
        uint32_t rotated[4];
        uv[0] = *(uint32_t *) (def + 0x88);
        uv[1] = *(uint32_t *) (def + 0x90);
        uv[2] = *(uint32_t *) (def + 0x8c);
        uv[3] = *(uint32_t *) (def + 0x94);
        uint32_t *uvp = uv;
        if (((this->flags >> 24) & 0x2) != 0) {
            uvp = (uint32_t *) rotateUVs((float *) uv, current, (float *) rotated);
        }

        char velBuf[12];
        int spread = *(int *) (def + 0x50);
        if (spread == 0) {
            *(uint32_t *) (velBuf + 0) = 0;
            *(uint32_t *) (velBuf + 4) = 0;
            *(uint32_t *) (velBuf + 8) = 0;
        } else {
            int range = spread << 1;
            ((float *) velBuf)[0] = (float) (
                this->random.nextInt(range) - spread);
            ((float *) velBuf)[1] = *(float *) (def + 0x5c) +
                                            (float) (this->random.nextInt(range) -
                                                     spread);
            ((float *) velBuf)[2] = (float) (
                this->random.nextInt(range) - spread);
        }

        Vector &slot = this->particleVelocities[current];
        slot = *(Vector *) (velBuf);

        if (velocity != 0) {
            float drag = *(float *) (def + 0x64);
            if (drag != 0.0f) {
                char tmp[12];
                *(Vector *) tmp = *(const Vector *) velocity * drag;
                slot -= *(Vector *) (tmp);
            }
        }

        int posSpread = *(int *) (def + 0x48);
        if (posSpread != 0) {
            ((float *) velBuf)[0] =
                    (float) (this->random.nextInt(posSpread << 1) - posSpread);
            ((float *) velBuf)[1] = 0.0f;
            ((float *) velBuf)[2] =
                    (float) (this->random.nextInt(posSpread << 1) - posSpread);
            *(Vector *) (&position) += *(Vector *) (velBuf);
        }

        int ySpread = *(int *) (def + 0x4c);
        if (ySpread != 0) {
            position.y += (float) (this->random.nextInt(ySpread << 1) - ySpread);
        }

        if (lifetime < 0.0f) {
            lifetime = *(float *) (def + 0x14);
        }

        int randomLife = *(int *) (def + 0x18);
        if (randomLife != 0) {
            float uv0 = bits_float(uvp[0]);
            float uv1 = bits_float(uvp[1]);
            float uv2 = bits_float(uvp[2]);
            float uv3 = bits_float(uvp[3]);
            float size0base = *(float *) (def + 0x1c);
            float life = lifetime + (float) this->random.nextInt(randomLife);
            float size1base = *(float *) (def + 0x20);
            float size0 = size0base + (float) this->random.nextInt(randomLife);
            float size1 = size1base + (float) this->random.nextInt(randomLife);
            float velocityScale = *(float *) (def + 0x24);
            if (velocityScale == 0.0f) {
                *(uint32_t *) (velBuf + 0) = 0;
                *(uint32_t *) (velBuf + 4) = 0;
                *(uint32_t *) (velBuf + 8) = 0;
            } else {
                *(Vector *) velBuf = velocityScale * slot;
            }

            this->setParticle(position, life, *(uint32_t *) (def + 0x34),
                              uv0, uv2, uv1, uv3, *(int *) (def + 0x3c) > 0,
                              size0, size1, *(const Vector *) velBuf);
        } else {
            float velocityScale = *(float *) (def + 0x24);
            if (velocityScale == 0.0f) {
                *(uint32_t *) (velBuf + 0) = 0;
                *(uint32_t *) (velBuf + 4) = 0;
                *(uint32_t *) (velBuf + 8) = 0;
            } else {
                *(Vector *) velBuf = velocityScale * slot;
            }

            this->setParticle(position, lifetime, *(uint32_t *) (def + 0x34),
                              bits_float(uvp[0]), bits_float(uvp[2]), bits_float(uvp[1]),
                              bits_float(uvp[3]), *(int *) (def + 0x3c) > 0,
                              *(float *) (def + 0x1c), *(float *) (def + 0x20),
                              *(const Vector *) velBuf);
        }

        float drag = *(float *) (def + 0x64);
        if (drag != 0.0f) {
            char tmp[12];
            *(Vector *) tmp = this->emitterVelocity * drag;
            *(Vector *) velBuf = *(const Vector *) tmp * 2.0f;
            slot += *(Vector *) (velBuf);
        }

        current = this->currentParticle + 1;
        if (this->maxParticles <= current) {
            current = 0;
        }
        this->currentParticle = current;
    }
}

void IParticleSystem::resetEmitterVelocity() {
    char value[12] = {};
    this->emitterVelocity = *(Vector *) (value);
    this->emitterVelocityDirty = 1;
    char *matrixValue = value;
    new (matrixValue) Vector(AbyssEngine::AEMath::MatrixGetPosition(*this->matrix));
    this->lastEmitterPosition = *(Vector *) (matrixValue);
    this->field_0x4 = 0;
}
