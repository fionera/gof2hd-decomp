#ifndef GOF2_PARTICLESYSTEMMESH_H
#define GOF2_PARTICLESYSTEMMESH_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
struct ParticleSystemMesh;
struct PaintCanvas;                 // global engine type (see fwd.h)

enum BlendMode { BlendMode_dummy = 0 };

// ParticleSettings::ParticleSet — a small enum value used as a per-particle preset index.
struct ParticleSettings {
    enum ParticleSet { ParticleSet_dummy = 0 };
};
using ParticleSet = ParticleSettings::ParticleSet;

// Matrix/Vector are the global 3D-math aliases pulled in by common.h.

void *operator new(__SIZE_TYPE__ size);

struct ParticleSystemMesh {
    void* field_0x0;                    // +0x0  vtable
    int field_0x4;                      // +0x4
    void* field_0x8;                    // +0x8  PaintCanvas*
    uint8_t field_0xc;                  // +0xc
    uint8_t field_0xd;                  // +0xd
    void* field_0x18;                   // +0x18 Matrix*
    uint32_t field_0x34;                // +0x34 flags
    uint8_t field_0x35;                 // +0x35
    uint8_t field_0x36;                 // +0x36
    uint8_t field_0x45;                 // +0x45
    uint32_t field_0x48;                // +0x48 particle count
    uint8_t field_0x4c;                 // +0x4c
    int field_0x50;                     // +0x50 current id
    uint32_t field_0x54;                // +0x54 mesh handle
    uint32_t field_0x58;                // +0x58 first point
    uint8_t field_0x5c;                 // +0x5c
    uint32_t field_0x60;                // +0x60
    void* field_0x64;                   // +0x64 positions buffer
    void* field_0x68;                   // +0x68 ages array
    void* field_0x6c;                   // +0x6c set-id array
    uint32_t field_0x70;                // +0x70 point count
    uint8_t field_0x74;                 // +0x74 wide
    uint32_t field_0x78;                // +0x78
    uint32_t field_0x7c;                // +0x7c
    uint32_t field_0x80;                // +0x80
    uint32_t field_0x84;                // +0x84
    uint32_t field_0x88;                // +0x88
    uint8_t field_0x90;                 // +0x90 new-section flag
    uint32_t field_0x94;                // +0x94
    uint32_t field_0x98;                // +0x98 edge count
    uint32_t field_0x9c;                // +0x9c stride

    ParticleSystemMesh(PaintCanvas *canvas, const Matrix *matrix, const void *sets, bool a, bool b);

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
