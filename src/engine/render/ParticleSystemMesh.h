#ifndef GOF2_PARTICLESYSTEMMESH_H
#define GOF2_PARTICLESYSTEMMESH_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"

// Galaxy on Fire 2 -- ParticleSystemMesh.
// A mesh-backed particle system: each particle is rendered as one or more quads whose
// edges are stored in `positions` and animated per frame. The class itself lives in the
// global namespace; only the cross-class argument types (PaintCanvas, BlendMode) live in
// AbyssEngine.

namespace AbyssEngine {
class PaintCanvas;
enum BlendMode { BlendMode_dummy };
}
using AbyssEngine::PaintCanvas;
using AbyssEngine::BlendMode;

// ParticleSettings::ParticleSet -- a small index value used as a per-particle preset index.
struct ParticleSettings {
    enum ParticleSet { ParticleSet_dummy = 0 };
};
using ParticleSet = ParticleSettings::ParticleSet;

class ParticleSystemMesh {
public:
    void* vtable;                       // +0x0
    uint8_t dirty;                       // +0x4   reset() marks the system live/dirty
    PaintCanvas* canvas;                // +0x8
    uint8_t emitEnabled;                 // +0xc   emit() gate
    uint8_t visible;                     // +0xd   emit() gate
    const Matrix* matrix;               // +0x18
    Vector motion;                       // +0x1c  embedded direction/motion vector (scaled in updateUsualEdges)
    uint32_t flags;                      // +0x34
    uint8_t trailFlags;                  // +0x35  bit7 selects trail-mode particles
    uint8_t edgeFlags;                   // +0x36  bit3 selects the embedded-motion edge source
    uint8_t colorMask;                   // +0x45
    uint32_t particleCount;              // +0x48
    uint8_t flipRight;                   // +0x4c  negate the right basis vector
    int currentId;                       // +0x50
    uint32_t mesh;                       // +0x54
    uint32_t firstPoint;                 // +0x58
    uint8_t initialized;                 // +0x5c  init() completed
    uint32_t emitCounter;                // +0x60
    Vector* positions;                   // +0x64  positions/edge-vector buffer (12-byte elements)
    int* ages;                           // +0x68  per-particle age array
    int8_t* setIds;                      // +0x6c  per-particle set-id array
    uint32_t pointCount;                 // +0x70
    uint8_t wide;                        // +0x74
    uint32_t field_0x78;                 // +0x78
    uint32_t field_0x7c;                 // +0x7c
    uint32_t field_0x80;                 // +0x80
    uint32_t field_0x84;                 // +0x84
    uint32_t field_0x88;                 // +0x88
    uint8_t newSectionStarted;           // +0x90
    uint32_t frameCounter;               // +0x94
    uint32_t edgeCount;                  // +0x98
    uint32_t stride;                     // +0x9c

    ParticleSystemMesh(PaintCanvas *canvas, const Matrix *matrix, const void *sets, bool a, bool b);
    ~ParticleSystemMesh();

    void emit(int id);
    int getPrevId(int id);
    int getQuadCount();
    void incId();
    void reset();
    void startNewSection();
    uint8_t wasNewSectionStarted();
    int init(uint32_t mesh, uint32_t firstPoint);
    void setQuadEdge(const Vector &edge, int point, const Vector &delta);
    void finishCurrentTrailParticle(ParticleSet set, int id, const Vector &first, const Vector &second);
    void setParticle(const Vector &pos, float scale, uint32_t color, float a, float b, float c, float d,
                     bool trail, float e, float f, const Vector &dir);
    void setParticle(const Vector &pos, float scale, uint32_t color, float u0, float u1, float v0, float v1,
                     bool useMaskedColor, float upScale, float dirScale, const Vector &delta, bool finish);
    void updateUsualEdges(int id, int delta);
    void updateTrailEdges(int id, int delta);
    void updateSingleColor(int id);
    void updateSingle(int id, float delta);

    static void render(PaintCanvas *canvas, uint32_t texture);
    static void render(PaintCanvas *canvas, uint32_t mesh, uint32_t texture, BlendMode blend);
    static void emitTrail(int self);
};
#endif
