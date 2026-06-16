#ifndef GOF2_PARTICLESYSTEMMESH_H
#define GOF2_PARTICLESYSTEMMESH_H
#include "common.h"

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
    void* vtable;
    int field_0x4;
    PaintCanvas* canvas;
    uint8_t field_0xc;
    uint8_t field_0xd;
    const Matrix* matrix;
    Vector field_0x1c;                  // embedded direction/motion vector (scaled in updateUsualEdges)
    uint32_t flags;
    uint8_t field_0x35;
    uint8_t field_0x36;
    uint8_t colorMask;
    uint32_t particleCount;
    uint8_t field_0x4c;
    int currentId;
    uint32_t mesh;
    uint32_t firstPoint;
    uint8_t field_0x5c;
    uint32_t field_0x60;
    Vector* positions;                  // positions/edge-vector buffer (12-byte elements)
    int* ages;                          // per-particle age array
    int8_t* setIds;                     // per-particle set-id array
    uint32_t pointCount;
    uint8_t wide;
    uint32_t field_0x78;
    uint32_t field_0x7c;
    uint32_t field_0x80;
    uint32_t field_0x84;
    uint32_t field_0x88;
    uint8_t newSectionStarted;
    uint32_t field_0x94;
    uint32_t edgeCount;
    uint32_t stride;

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
