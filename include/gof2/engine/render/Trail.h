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

// Trail: fields accessed via i32/u32/ptr offset helpers (deterministic field_0xNN layout).
// Method signatures recovered from the decompiled bodies in src/Trail.cpp and cross-checked
// against Ghidra (Trail @ 0016bd58, ~Trail @ 0016bf8c, changeType @ 0016bef4, setWidth @ 0016c184).
class Trail {
public:
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
