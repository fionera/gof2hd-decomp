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

static inline int32_t &i32(void *self, uint32_t off) { return *(int32_t *)((char *)self + off); }
static inline uint32_t &u32(void *self, uint32_t off) { return *(uint32_t *)((char *)self + off); }
static inline void *&ptr(void *self, uint32_t off) { return *(void **)((char *)self + off); }

struct Trail { void* _opaque; };  // no offset accesses observed
#endif
