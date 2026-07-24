#ifndef GOF2_PARTICLESYSTEMMESH_H
#define GOF2_PARTICLESYSTEMMESH_H
#include "engine/core/Array.h"
#include "engine/render/RenderEnums.h"
#include "../core/AEString.h"
#include "engine/render/IParticleSystem.h"
#include "engine/render/ParticleSettings.h"

#include "engine/math/Matrix.h"
#include "engine/math/Vector.h"

namespace AbyssEngine {
}

namespace AbyssEngine {
    class PaintCanvas;
}

using ::AbyssEngine::PaintCanvas;
using AbyssEngine::BlendMode;

using ParticleSet = ParticleSettings::ParticleSet;

class ParticleSystemMesh : public IParticleSystem {
public:
    uint32_t pointCount;
    uint8_t wide;
    uint32_t field_0x78;
    uint32_t field_0x7c;
    uint32_t field_0x80;
    uint32_t field_0x84;
    uint32_t field_0x88;
    uint32_t field_0x8c;
    uint8_t newSectionStarted;
    uint32_t frameCounter;
    uint32_t edgeCount;
    uint32_t stride;

    ParticleSystemMesh(PaintCanvas *canvas, const Matrix *matrix, const Array<ParticleSet> &sets, bool a, bool b);

    ~ParticleSystemMesh();

    void emit(int delta) override;

    int getQuadCount() override;

    void reset() override;

    void release() override;

    void init(uint32_t mesh, uint16_t firstPoint) override;

    void setParticle(const Vector &pos, float scale, uint32_t color, float u0, float u1, float v0,
                     float v1, bool useMaskedColor, float upScale, float dirScale,
                     const Vector &delta) override;

    void updateSingle(int id, float delta) override;

    int getPrevId(int id);

    void incId();

    void startNewSection();

    uint8_t wasNewSectionStarted();

    void setQuadEdge(const Vector &edge, int point, const Vector &delta);

    void finishCurrentTrailParticle(ParticleSet set, int id, const Vector &first, const Vector &second);

    void setParticle(const Vector &pos, float scale, uint32_t color, float u0, float u1, float v0, float v1,
                     bool useMaskedColor, float upScale, float dirScale, const Vector &delta, bool finish);

    void updateUsualEdges(int id, int delta);

    void updateTrailEdges(int id, int delta);

    void updateSingleColor(int id);

    static void render(PaintCanvas *canvas, uint32_t texture);

    static void render(PaintCanvas *canvas, uint32_t mesh, uint32_t texture, BlendMode blend);

    static void emitTrail(int self);
};

#if __SIZEOF_POINTER__ == 4
static_assert(__builtin_offsetof(ParticleSystemMesh, pointCount) == 0x70, "ParticleSystemMesh::pointCount offset");
static_assert(__builtin_offsetof(ParticleSystemMesh, wide) == 0x74, "ParticleSystemMesh::wide offset");
static_assert(__builtin_offsetof(ParticleSystemMesh, field_0x78) == 0x78, "ParticleSystemMesh::field_0x78 offset");
static_assert(__builtin_offsetof(ParticleSystemMesh, field_0x8c) == 0x8c, "ParticleSystemMesh::field_0x8c offset");
static_assert(__builtin_offsetof(ParticleSystemMesh, newSectionStarted) == 0x90, "ParticleSystemMesh::newSectionStarted offset");
static_assert(__builtin_offsetof(ParticleSystemMesh, frameCounter) == 0x94, "ParticleSystemMesh::frameCounter offset");
static_assert(__builtin_offsetof(ParticleSystemMesh, edgeCount) == 0x98, "ParticleSystemMesh::edgeCount offset");
static_assert(__builtin_offsetof(ParticleSystemMesh, stride) == 0x9c, "ParticleSystemMesh::stride offset");
static_assert(sizeof(ParticleSystemMesh) == 0xa0, "ParticleSystemMesh size");
#endif
#endif
