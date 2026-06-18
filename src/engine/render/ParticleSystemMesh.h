#ifndef GOF2_PARTICLESYSTEMMESH_H
#define GOF2_PARTICLESYSTEMMESH_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "engine/render/IParticleSystem.h"
#include "engine/render/ParticleSettings.h"   // canonical ParticleSettings::ParticleSet nested type

// Galaxy on Fire 2 -- ParticleSystemMesh.
// A mesh-backed particle system: each particle is rendered as one or more quads whose
// edges are stored in `positions` and animated per frame. It is one of the two concrete
// IParticleSystem renderers; the emit/update/render hooks below are the real C++ overrides
// of the base virtual slots.

// PaintCanvas is AbyssEngine::PaintCanvas (the type IParticleSystem stores as `canvas`).
namespace AbyssEngine {
class PaintCanvas;   // real type lives here (engine/render/PaintCanvas.h); pointer-only use
enum BlendMode { BlendMode_dummy = 0, BlendMode_1 = 1, BlendMode_2 = 2, BlendMode_8 = 8, BlendMode_0x15 = 0x15 };
}
using ::AbyssEngine::PaintCanvas;
using AbyssEngine::BlendMode;

// ParticleSettings::ParticleSet -- a small index value used as a per-particle preset index.
// The canonical definition lives in engine/render/ParticleSettings.h (included above).
using ParticleSet = ParticleSettings::ParticleSet;

// ParticleSystemMesh is a concrete IParticleSystem renderer. The base supplies the vptr and the
// shared emitter state (canvas, matrix, flags, emitEnabled, particle buffers, ...). The members
// below are the mesh-specific extension fields; the emit/update/render hooks are real overrides of
// the IParticleSystem virtual slots.
class ParticleSystemMesh : public IParticleSystem {
public:
    uint8_t dirty;                       // reset() marks the system live/dirty
    uint8_t visible;                     // emit() gate
    Vector motion;                       // embedded direction/motion vector (scaled in updateUsualEdges)
    uint8_t trailFlags;                  // bit7 selects trail-mode particles
    uint8_t edgeFlags;                   // bit3 selects the embedded-motion edge source
    uint8_t colorMask;
    uint32_t particleCount;
    uint8_t flipRight;                   // negate the right basis vector
    int currentId;
    uint32_t mesh;
    uint32_t firstPoint;
    uint8_t initialized;                 // init() completed
    uint32_t emitCounter;
    Vector* positions;                   // positions/edge-vector buffer (12-byte elements)
    int* ages;                           // per-particle age array
    int8_t* setIds;                      // per-particle set-id array
    uint32_t pointCount;
    uint8_t wide;
    uint32_t field_0x78;
    uint32_t field_0x7c;
    uint32_t field_0x80;
    uint32_t field_0x84;
    uint32_t field_0x88;
    uint8_t newSectionStarted;
    uint32_t frameCounter;
    uint32_t edgeCount;
    uint32_t stride;

    ParticleSystemMesh(PaintCanvas *canvas, const Matrix *matrix, const void *sets, bool a, bool b);
    ~ParticleSystemMesh();

    // IParticleSystem virtual overrides (slot order: emit=1, reset=2, getQuadCount=4,
    // updateSingle=5, setParticle=6, init=0).
    void emit(int delta) override;
    int  getQuadCount() override;
    void reset() override;
    int  init(uint32_t mesh, uint16_t firstPoint) override;
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
    // Full 12-argument setter; the 11-arg virtual override forwards into this one.
    void setParticle(const Vector &pos, float scale, uint32_t color, float u0, float u1, float v0, float v1,
                     bool useMaskedColor, float upScale, float dirScale, const Vector &delta, bool finish);
    void updateUsualEdges(int id, int delta);
    void updateTrailEdges(int id, int delta);
    void updateSingleColor(int id);

    static void render(PaintCanvas *canvas, uint32_t texture);
    static void render(PaintCanvas *canvas, uint32_t mesh, uint32_t texture, BlendMode blend);
    static void emitTrail(int self);
};
#endif
