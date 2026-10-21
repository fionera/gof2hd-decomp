#include "engine/render/ParticleSystemMesh.h"
#include "engine/render/ParticleSettingsRef.h"
#define GOF2_ENUM_BlendMode
#include "engine/render/PaintCanvas.h"
#include "engine/math/AEMath.h"

int ParticleSystemMesh::getPrevId(int id) {
    if (id == 0)
        id = (int) this->maxParticles;
    return id - 1;
}

void ParticleSystemMesh::setParticle(const Vector &pos, float scale, uint32_t color, float u0, float v0,
                                     float u1, float v1, bool useMaskedColor, float upScale, float dirScale,
                                     const Vector &delta) {
    return setParticle(pos, scale, color, u0, v0, u1, v1, useMaskedColor, upScale, dirScale, delta, false);
}

void ParticleSystemMesh::emit(int id) {
    if (this->emitEnabled == 0 || this->renderEnabled == 0) {
        this->newSectionStarted = 1;
        return;
    }

    uint32_t flags = this->flags;
    if ((flags & 0x80) != 0)
        return;
    if ((int) (flags << 16) < 0)
        return emitTrail(id);
    return IParticleSystem::emit(id);
}

void ParticleSystemMesh::finishCurrentTrailParticle(ParticleSet set, int id, const Vector &first,
                                                    const Vector &second) {
    this->particleSetIds[id] = (int8_t) set;
    this->particleAges[id] = 0;

    uint32_t flags = this->flags;
    uint32_t offset = (this->edgeCount * id * 2) | 1;
    Vector *dst = this->particleVelocities + offset;

    if ((flags & 0x1000) != 0) {
        *dst = first;
        flags = this->flags;
        dst += 2;
    }
    if ((int) (flags << 18) < 0)
        *dst = second;
}

void ParticleSystemMesh::incId() {
    int id = this->currentParticle + 1;
    if (id >= (int) this->maxParticles)
        id = 0;
    this->currentParticle = id;
}

void ParticleSystemMesh::reset() {
    for (int i = 0; i < (int) this->pointCount; i++) {
        this->canvas->MeshSetPoint(this->resource,
                                   (uint16_t)(this->idOffset + i), 0.0f, 0.0f, 0.0f);
    }

    for (int i = 0; i < (int) this->maxParticles; i++)
        this->particleAges[i] = -1;

    this->currentParticle = 0;
    this->frameCounter = 0;
    this->newSectionStarted = 1;
    this->emitTimer = 0;
    this->field_0x4 = 1;
}

ParticleSystemMesh::~ParticleSystemMesh() {
    this->release();
}

void ParticleSystemMesh::release() {
}

void ParticleSystemMesh::render(PaintCanvas *canvas, uint32_t texture) {
    if (texture != 0xffffffff)
        return canvas->DrawTransform(texture, nullptr);
}

void ParticleSystemMesh::startNewSection() {
    this->newSectionStarted = 1;
}

uint8_t ParticleSystemMesh::wasNewSectionStarted() {
    return this->newSectionStarted;
}

int ParticleSystemMesh::getQuadCount() {
    return (int) this->pointCount >> 2;
}

void ParticleSystemMesh::setQuadEdge(const Vector &edge, int point, const Vector &delta) {
    Vector pos = edge - delta;
    this->canvas->MeshSetPoint(this->resource, (uint16_t) point, pos.x, pos.y, pos.z);

    pos = edge + delta;

    uint8_t wide = this->wide;
    int next = point + 1;
    PaintCanvas *canvas = this->canvas;
    uint32_t mesh = this->resource;
    if (wide == 0) {
        canvas->MeshSetPoint(mesh, (uint16_t) next, pos.x, pos.y, pos.z);
    } else {
        canvas->MeshSetPoint(mesh, (uint16_t) next, edge.x, edge.y, edge.z);
        this->canvas->MeshSetPoint(this->resource, (uint16_t)(point + 5), edge.x, edge.y, edge.z);
        this->canvas->MeshSetPoint(this->resource, (uint16_t)(point + 4), pos.x, pos.y, pos.z);
    }
}

ParticleSystemMesh::ParticleSystemMesh(PaintCanvas *canvas, const Matrix *matrix, const Array<ParticleSet> &sets,
                                       bool a, bool b)
    : IParticleSystem(canvas, matrix, sets, a, b) {
    uint32_t flags = this->flags;
    int particleCount = this->maxParticles;
    this->field_0x80 = 0;
    this->field_0x84 = 0;
    this->field_0x88 = 0;

    this->edgeCount = 0;
    if ((flags & 0x1000) != 0)
        this->edgeCount++;
    if ((flags & 0x2000) != 0)
        this->edgeCount++;
    if ((flags & 0x4000) != 0)
        this->edgeCount++;
    this->wide = (flags >> 16) & 1;
    this->stride = this->edgeCount << this->wide;
    uint32_t edgeCount = this->edgeCount;
    uint32_t stride = this->stride;
    uint32_t quads = particleCount * stride;
    this->pointCount = quads << 2;

    if ((flags & 0x8000) != 0) {
        if (sets.size() != 0) {
            int set = (int) sets.data()[0];
            if (set != -1) {
                const ParticleSettings::SetDefinition &def =
                        ParticleSettingsRef::cur.sets[set];
                if (def.colorFlag > 0.0f)
                    this->pointCount = (quads << 2) + (stride << 2);
            }
        }
        this->particleVelocities = new Vector[particleCount * edgeCount * 2];
    } else {
        this->particleVelocities = new Vector[particleCount];
    }
    this->field_0x78 = 0;
    this->field_0x7c = 0;
}

void ParticleSystemMesh::setParticle(const Vector &pos, float scale, uint32_t color, float u0, float v0, float u1,
                                     float v1, bool useMaskedColor, float upScale, float dirScale,
                                     const Vector &delta, bool finish) {
    Vector right = AbyssEngine::AEMath::MatrixGetRight(*this->matrix) * scale;
    if (this->mirror != 0)
        right = -right;
    Vector up = AbyssEngine::AEMath::MatrixGetUp(*this->matrix) * (upScale != 0.0f ? upScale : scale);
    Vector dir = AbyssEngine::AEMath::MatrixGetDir(*this->matrix) * (dirScale != 0.0f ? dirScale : scale);

    if ((this->flags & 0x20000) != 0) {
        Vector diagUp = (up - right) * 0.70710677f;
        Vector diagRight = (right + up) * 0.70710677f;
        right = diagRight;
        up = diagUp;
    }

    int point = (int) this->idOffset + (int) this->stride * this->currentParticle * 4;
    if ((this->flags & 0x1000) != 0) {
        setQuadEdge((pos - dir) - delta, point, right);
        setQuadEdge((pos + dir) + delta, point + 2, right);
        point += this->wide != 0 ? 8 : 4;
    }
    if ((this->flags & 0x2000) != 0) {
        setQuadEdge((pos - dir) + delta, point, up);
        setQuadEdge((pos + dir) - delta, point + 2, up);
        point += this->wide != 0 ? 8 : 4;
    }
    if ((this->flags & 0x4000) != 0) {
        setQuadEdge(pos + up, point, right);
        setQuadEdge(pos - up, point + 2, right);
    }

    uint32_t frontColor = color;
    if (useMaskedColor && !finish)
        frontColor = color & (this->alphaFade == 0 ? 0xffffff00u : 0xffu);
    uint32_t backColor = color;
    if (finish)
        backColor = color & (this->alphaFade == 0 ? 0xffffff00u : 0xffu);

    int base = (int) this->idOffset + (int) this->stride * this->currentParticle * 4;
    for (int i = 0; i < (int) this->stride; i++) {
        this->canvas->MeshSetColor(this->resource, (uint16_t) base, backColor);
        this->canvas->MeshSetColor(this->resource, (uint16_t) (base + 1), backColor);
        this->canvas->MeshSetColor(this->resource, (uint16_t) (base + 2), frontColor);
        this->canvas->MeshSetColor(this->resource, (uint16_t) (base + 3), frontColor);
        this->canvas->MeshSetUv(this->resource, (uint16_t) base, u0, v0);
        this->canvas->MeshSetUv(this->resource, (uint16_t) (base + 1), u1, v0);
        this->canvas->MeshSetUv(this->resource, (uint16_t) (base + 2), u0, v1);
        this->canvas->MeshSetUv(this->resource, (uint16_t) (base + 3), u1, v1);
        base += 4;
    }
}

void ParticleSystemMesh::init(uint32_t mesh, uint16_t firstPoint) {
    this->resource = mesh;
    this->idOffset = firstPoint;

    for (int i = 0; i < (int) this->pointCount; i += 4) {
        this->canvas->MeshSetUv(this->resource, (uint16_t)(this->idOffset + i), 0.0f, 0.0f);
        this->canvas->MeshSetUv(this->resource, (uint16_t)(this->idOffset + i + 1), 1.0f, 0.0f);
        this->canvas->MeshSetUv(this->resource, (uint16_t)(this->idOffset + i + 2), 0.0f, 1.0f);
        this->canvas->MeshSetUv(this->resource, (uint16_t)(this->idOffset + i + 3), 1.0f, 1.0f);
        this->canvas->MeshSetColor(this->resource, (uint16_t)(this->idOffset + i), (uint32_t)0);
        this->canvas->MeshSetColor(this->resource, (uint16_t)(this->idOffset + i + 1), (uint32_t)0);
        this->canvas->MeshSetColor(this->resource, (uint16_t)(this->idOffset + i + 2), (uint32_t)0);
        this->canvas->MeshSetColor(this->resource, (uint16_t)(this->idOffset + i + 3), (uint32_t)0);
    }

    int point = (int) this->idOffset;
    for (int tri = 0; tri < ((int) this->pointCount >> 1); tri += 2) {
        this->canvas->MeshSetTriangle(this->resource,
                                      (uint16_t)(tri + (this->idOffset >> 1)), (uint16_t)(point + 2),
                                      (uint16_t)(point + 1), (uint16_t) point);
        this->canvas->MeshSetTriangle(this->resource,
                                      (uint16_t)(tri + (this->idOffset >> 1) + 1), (uint16_t)(point + 1),
                                      (uint16_t)(point + 2), (uint16_t)(point + 3));
        point += 4;
    }

    this->initialized = 1;

    this->reset();
}

void ParticleSystemMesh::updateUsualEdges(int id, int delta) {
    int start = (int) this->idOffset;
    int stride = (int) this->stride;
    Vector move = {0.0f, 0.0f, 0.0f};
    if ((int) (this->flags << 12) < 0)
        move = this->emitterVelocity * ((float) delta * 0.001f * this->particleVelocities[id].y);
    else
        move = this->particleVelocities[id] * ((float) delta * 0.001f);

    int point = start + id * 4 * stride;
    for (int i = 0; i < (int) this->stride * 4; i++)
        this->canvas->MeshTranslatePoint(this->resource, (uint16_t) (point + i), move.x, move.y, move.z);
}

void ParticleSystemMesh::updateSingleColor(int id) {
    float a;
    float r;
    float g;
    float b;

    int stride = (int) this->stride;
    int start = (int) this->idOffset;
    if ((int) (this->flags << 16) < 0) {
        if (this->particleAges[(id == 0 ? (int) this->maxParticles : id) - 1] == -1) {
            int set = this->particleSetIds[id];
            uint32_t color = ParticleSettingsRef::cur.sets[set].color1;
            uint32_t mask = this->alphaFade == 0 ? 0xffffff00u : 0xffu;
            color &= mask;
            a = (float) (color >> 24) * 0.0039215689f;
            r = (float) ((color >> 16) & 0xff) * 0.0039215689f;
            g = (float) ((color >> 8) & 0xff) * 0.0039215689f;
            b = (float) (color & 0xff) * 0.0039215689f;
        } else {
            this->interpolateColor(id, a, r, g, b);
        }
    } else {
        this->interpolateColor(id, a, r, g, b);
    }

    int point = start + id * 4 * stride;
    for (int i = 0; i < (int) this->stride; i++) {
        this->canvas->MeshSetColor(this->resource, (uint16_t)(point + i * 4 + 2), a, r, g, b);
        this->canvas->MeshSetColor(this->resource, (uint16_t)(point + i * 4 + 3), a, r, g, b);
    }

    if ((int) (this->flags << 16) < 0) {
        int next = (id == (int) this->maxParticles - 1) ? 0 : id + 1;
        if (this->particleAges[next] == -1)
            return;
        point = (int) this->idOffset + (int) this->stride * next * 4;
        for (int i = 0; i < (int) this->stride; i++, point += 4) {
            this->canvas->MeshSetColor(this->resource, (uint16_t) point, a, r, g, b);
            this->canvas->MeshSetColor(this->resource, (uint16_t)(point + 1), a, r, g, b);
        }
    } else {
        point = start + id * 4 * stride;
        for (int i = 0; i < (int) this->stride; i++, point += 4) {
            this->canvas->MeshSetColor(this->resource, (uint16_t) point, a, r, g, b);
            this->canvas->MeshSetColor(this->resource, (uint16_t)(point + 1), a, r, g, b);
        }
    }
}

void ParticleSystemMesh::render(PaintCanvas *canvas, uint32_t mesh, uint32_t texture, BlendMode blend) {
    if (mesh == 0xffffffff)
        return;
    canvas->SetTexture(texture, 0xffffffff);
    canvas->SetBlendMode(blend);
    uint32_t current = canvas->CameraGetCurrent();
    float *local = canvas->CameraGetLocal(current);
    return canvas->DrawTransform(mesh, (const Matrix*)local);
}

void ParticleSystemMesh::emitTrail(int delta) {
    int set = (int) this->particleSets.data()[this->particleSetIndex];
    const ParticleSettings::SetDefinition &def = ParticleSettingsRef::cur.sets[set];

    Vector rightScaled = AbyssEngine::AEMath::MatrixGetRight(*this->matrix) * (this->mirror == 0 ? 1.0f : -1.0f);
    Vector up = AbyssEngine::AEMath::MatrixGetUp(*this->matrix);
    Vector dir = AbyssEngine::AEMath::MatrixGetDir(*this->matrix);

    Vector edgeRight = (this->flags & 0x20000) == 0
                       ? rightScaled * (def.lifeBase)
                       : (rightScaled + up) * (def.lifeBase * 0.70710677f);
    Vector edgeUp = (this->flags & 0x20000) == 0
                    ? up * (def.lifeBase)
                    : (up - rightScaled) * (def.lifeBase * 0.70710677f);
    Vector edgeDir = dir * (def.lifeBase);

    Vector pos = AbyssEngine::AEMath::MatrixTransformVector(
            *this->matrix, Vector((float) (int) this->field_0x78, 0.0f, (float) (int) this->field_0x7c));
    pos = pos + def.posRight * rightScaled
              + def.posUp * up
              + def.posDir * dir;

    if (def.colorFlag > 0.0f) {
        int saved = this->currentParticle;
        int prev = getPrevId(this->currentParticle);
        bool finish = this->particleAges[prev] == -1;
        pos -= def.colorFlag * 0.5f * dir;
        float u0 = def.uvU0;
        float v0 = def.uvV0;
        float u1 = def.uvU1;
        float v1 = v0 + (def.uvV1 - v0) * 0.05f;
        this->currentParticle = this->maxParticles;
        setParticle(pos, def.lifeBase, def.color0, u0, v0, u1, v1, true, 0.0f,
                    def.colorFlag * 0.5f, Vector(), finish);
        this->currentParticle = saved;
        pos -= def.colorFlag * 0.5f * dir;
    }

    if (this->newSectionStarted == 0) {
        int point = (int) this->idOffset + (int) this->stride * this->currentParticle * 4;
        uint32_t flags = this->flags;
        if ((flags & 0x1000) != 0) {
            setQuadEdge(pos, point + 2, edgeRight);
            point += this->wide == 0 ? 4 : 8;
            flags = this->flags;
        }
        if ((int) (flags << 18) < 0) {
            setQuadEdge(pos, point + 2, edgeUp);
            point += this->wide == 0 ? 4 : 8;
            flags = this->flags;
        }
        if ((int) (flags << 17) < 0) {
            setQuadEdge(pos, point + 2, edgeDir);
            flags = this->flags;
        }
        if ((int) (flags << 11) < 0) {
            float t = (float) (int) (this->frameCounter + delta)
                      / def.flLifetime;
            float u0 = def.uvU0;
            float u1 = def.uvU1;
            float v = def.uvV0
                      + (t < 1.0f ? t : 1.0f)
                        * (def.uvV1
                           - def.uvV0);
            int base = (int) this->idOffset + (int) this->stride * this->currentParticle * 4;
            for (int i = 0; i < (int) this->stride; i++, base += 4) {
                this->canvas->MeshSetUv(this->resource, (uint16_t) (base + 2), u0, v);
                this->canvas->MeshSetUv(this->resource, (uint16_t) (base + 3), u1, v);
            }
        }
    }

    this->frameCounter += delta;
    if (def.flLifetime < (float) (int) this->frameCounter) {
        Vector diff = *(Vector *) &this->field_0x80 - pos;
        if (AbyssEngine::AEMath::VectorDot(diff, diff) > 6000.0f)
            this->newSectionStarted = 1;
    }

    if (this->newSectionStarted != 0) {
        this->frameCounter = 0;
        int prev = getPrevId(this->currentParticle);
        int prevAge = this->particleAges[prev];
        float s = (float) def.posBase;
        finishCurrentTrailParticle((ParticleSet) set, this->currentParticle, rightScaled * s, up * s);
        incId();

        int point = (int) this->idOffset + (int) this->stride * this->currentParticle * 4;
        Vector *vel = this->particleVelocities + this->currentParticle * (int) this->edgeCount * 2;
        uint32_t flags = this->flags;
        if ((flags & 0x1000) != 0) {
            setQuadEdge(pos, point, edgeRight);
            setQuadEdge(pos, point + 2, edgeRight);
            vel[0] = rightScaled * s;
            vel[1] = Vector();
            point += this->wide == 0 ? 4 : 8;
            vel += 2;
            flags = this->flags;
        }
        if ((int) (flags << 18) < 0) {
            setQuadEdge(pos, point, edgeUp);
            setQuadEdge(pos, point + 2, edgeUp);
            vel[0] = up * s;
            vel[1] = Vector();
        }
        *(Vector *) &this->field_0x80 = pos;

        uint32_t frontColor = def.color0;
        uint32_t backColor = frontColor;
        if (prevAge == -1) {
            uint32_t mask = this->alphaFade == 0 ? 0xffffff00u : 0xffu;
            backColor = frontColor & mask;
        }

        int base = (int) this->idOffset + (int) this->stride * this->currentParticle * 4;
        for (int i = 0; i < (int) this->stride; i++, base += 4) {
            this->canvas->MeshSetColor(this->resource, (uint16_t) base, backColor);
            this->canvas->MeshSetColor(this->resource, (uint16_t) (base + 1), backColor);
            this->canvas->MeshSetColor(this->resource, (uint16_t) (base + 2), frontColor);
            this->canvas->MeshSetColor(this->resource, (uint16_t) (base + 3), frontColor);
            this->canvas->MeshSetUv(this->resource, (uint16_t) base,
                                    def.uvU0,
                                    def.uvV0);
            this->canvas->MeshSetUv(this->resource, (uint16_t) (base + 1),
                                    def.uvU1,
                                    def.uvV0);
            this->canvas->MeshSetUv(this->resource, (uint16_t) (base + 2),
                                    def.uvU0,
                                    def.uvV1);
            this->canvas->MeshSetUv(this->resource, (uint16_t) (base + 3),
                                    def.uvU1,
                                    def.uvV1);
        }

        this->particleAges[this->currentParticle] = -2;
        this->newSectionStarted = 0;
    }
}

void ParticleSystemMesh::updateSingle(int id, float delta) {
    int set = this->particleSetIds[id];
    const ParticleSettings::SetDefinition &def = ParticleSettingsRef::cur.sets[set];
    if ((int) (this->flags << 16) < 0) {
        updateTrailEdges(id, (int) delta);
        if (this->particleAges[id] == -2) {
            if (this->newSectionStarted == 0)
                return;
            Vector right = AbyssEngine::AEMath::MatrixGetRight(*this->matrix) * (this->mirror != 0 ? -1.0f : 1.0f);
            Vector up = AbyssEngine::AEMath::MatrixGetUp(*this->matrix);
            float size = (float) def.posBase;
            finishCurrentTrailParticle((ParticleSet) set, id, right * size, up * size);
        }
    } else {
        updateUsualEdges(id, (int) delta);
    }

    this->particleAges[id] = (int) ((float) this->particleAges[id] + delta);
    updateSingleColor(id);

    if (this->particleAges[id] > def.lifetime) {
        int stride = (int) this->stride;
        int start = (int) this->idOffset;
        this->particleAges[id] = -1;
        int point = start + stride * id * 4;
        for (int i = 0; i < (int) this->stride * 4; i++)
            this->canvas->MeshSetPoint(this->resource, (uint16_t) (point + i), 0.0f, 0.0f, 0.0f);
    }
}

void ParticleSystemMesh::updateTrailEdges(int id, int delta) {
    float scale = (float) delta * 0.001f;
    int point = (int) this->idOffset + id * (int) this->stride * 4;
    Vector *edge = this->particleVelocities + id * (int) this->edgeCount * 2;

    for (int i = 0; i < (int) this->edgeCount; i++) {
        Vector move = *edge++ * scale;
        this->canvas->MeshTranslatePoint(this->resource, (uint16_t) point, -move.x, -move.y, -move.z);
        this->canvas->MeshTranslatePoint(this->resource, (uint16_t) (point + (this->wide != 0 ? 4 : 1)),
                                         move.x, move.y, move.z);

        if (this->particleAges[id] != -2 || (int) (this->flags << 16) >= 0) {
            Vector move2 = *edge++ * scale;
            this->canvas->MeshTranslatePoint(this->resource, (uint16_t) (point + 2), -move2.x, -move2.y, -move2.z);
            this->canvas->MeshTranslatePoint(this->resource, (uint16_t) (point + 2 + (this->wide != 0 ? 4 : 1)),
                                             move2.x, move2.y, move2.z);
            point += this->wide != 0 ? 8 : 4;
        }
    }
}
