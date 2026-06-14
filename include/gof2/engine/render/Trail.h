#ifndef GOF2_TRAIL_H
#define GOF2_TRAIL_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
void *operator new(__SIZE_TYPE__ size);
void operator delete(void *ptr) noexcept;

namespace AbyssEngine {
namespace AEMath {

Vector MatrixTransformVector(const Matrix &matrix, const Vector &vector);
Matrix MatrixSetTranslation(Matrix &matrix, float x, float y, float z);

} // namespace AEMath

struct PaintCanvas {
    void MeshCreate(uint16_t vertices, uint16_t triangles, signed char flags, uint16_t texture, uint32_t &out);
    void MeshSetTriangle(uint32_t mesh, uint16_t triangle, uint16_t a, uint16_t b, uint16_t c);
    void MeshSetUv(uint32_t mesh, uint16_t vertex, float u, float v);
    float MeshSetPoint(uint32_t mesh, uint16_t vertex, float x, float y, float z);
    void TransformCreate(uint32_t *out);
    uint32_t TransformGetLocal(uint32_t transform);
    void TransformAddMeshId(uint32_t transform, uint32_t mesh);
};

} // namespace AbyssEngine

extern "C" void Trail_renderTransform(AbyssEngine::PaintCanvas *canvas, uint32_t transform, int mode);
extern "C" void Trail_transformSetColor(AbyssEngine::PaintCanvas *canvas, uint32_t transform, uint32_t color);

__attribute__((visibility("hidden"))) extern AbyssEngine::PaintCanvas **gTrailCanvasRender;
__attribute__((visibility("hidden"))) extern AbyssEngine::PaintCanvas **gTrailCanvasType1;
__attribute__((visibility("hidden"))) extern AbyssEngine::PaintCanvas **gTrailCanvasType2;
__attribute__((visibility("hidden"))) extern AbyssEngine::PaintCanvas **gTrailCanvasType3;
__attribute__((visibility("hidden"))) extern AbyssEngine::PaintCanvas **gTrailCanvasTypeDefault;
__attribute__((visibility("hidden"))) extern AbyssEngine::PaintCanvas **gTrailCanvasType5;
__attribute__((visibility("hidden"))) extern AbyssEngine::PaintCanvas **gTrailCanvasType8;
__attribute__((visibility("hidden"))) extern AbyssEngine::PaintCanvas **gTrailCanvasCtor;
__attribute__((visibility("hidden"))) extern AbyssEngine::PaintCanvas **gTrailCanvasUpdate;
__attribute__((visibility("hidden"))) extern AbyssEngine::PaintCanvas **gTrailCanvasSetWidth;

static inline void *&ptr(void *self, uint32_t off) { return *(void **)((char *)self + off); }

// Trail: named layout recovered from the decompiled bodies in src/Trail.cpp and cross-checked
// against Ghidra (Trail @ 0016bd58, ~Trail @ 0016bf8c, changeType @ 0016bef4, setWidth @ 0016c184).
// Trailing comments are the original 32-bit field offsets, kept for cross-reference.
class Trail {
public:
    uint32_t field_0x00;     // +0x00  zeroed in ctor, otherwise unused
    uint32_t field_0x04;     // +0x04  zeroed in ctor, otherwise unused
    uint32_t field_0x08;     // +0x08  zeroed in ctor, otherwise unused
    int width;               // +0x0c  half-width of the trail strip
    uint32_t meshId;         // +0x10  PaintCanvas mesh id
    uint32_t transformId;    // +0x14  PaintCanvas transform id
    int *points;             // +0x18  absolute vertex points buffer (x,y,z triples)
    int *relativePoints;     // +0x1c  vertex points relative to the head buffer
    int pointCount;          // +0x20  number of ints in the point buffers
    int segments;            // +0x24  number of trail segments

    Trail(int type, int segments);
    ~Trail();

    void update(const AbyssEngine::AEMath::Vector &a, const AbyssEngine::AEMath::Vector &b);
    void update(float ax, float ay, float az, float bx, float by, float bz);
    void update(const AbyssEngine::AEMath::Matrix &a, const AbyssEngine::AEMath::Matrix &b,
                const AbyssEngine::AEMath::Vector &v);

    void render();
    void translate(const AbyssEngine::AEMath::Vector &delta);
    void setWidth(int width);
    void changeType(int type);
    void reset(AbyssEngine::AEMath::Vector value);
};
#endif
