#ifndef WORK_CLASSES_TRAIL_SRC_CLASS_H
#define WORK_CLASSES_TRAIL_SRC_CLASS_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;
typedef unsigned long long uint64_t;

void *operator new(__SIZE_TYPE__ size);
void operator delete(void *ptr) noexcept;

namespace AbyssEngine {
namespace AEMath {

struct Vector {
    union {
        struct {
            float x;
            float y;
            float z;
        };
        float v[3];
        char bytes[12];
    };
};

struct Matrix {
    float m[15];
};

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

struct Trail {
    Trail(int type, int segments);
    ~Trail();

    void changeType(int type);
    void render();
    void reset(AbyssEngine::AEMath::Vector value);
    void setWidth(int width);
    void translate(const AbyssEngine::AEMath::Vector &delta);
    void update(float ax, float ay, float az, float bx, float by, float bz);
    void update(const AbyssEngine::AEMath::Vector &a, const AbyssEngine::AEMath::Vector &b);
    void update(const AbyssEngine::AEMath::Matrix &a,
                const AbyssEngine::AEMath::Matrix &b,
                const AbyssEngine::AEMath::Vector &v);
};

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

#endif
