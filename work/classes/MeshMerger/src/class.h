#ifndef WORK_CLASSES_MESHMERGER_SRC_CLASS_H
#define WORK_CLASSES_MESHMERGER_SRC_CLASS_H

// Galaxy on Fire 2 -- MeshMerger class (Android libgof2hdaa.so, armv7 Thumb).
// Top-level class. Its API uses AbyssEngine::{Mesh, PaintCanvas, AEMath::Matrix}.
// Field offsets recovered per-method from the target disassembly; accessed via
// byte-offset casts from `this`.

typedef unsigned char uint8_t;
typedef signed char int8_t;
typedef unsigned short uint16_t;
typedef short int16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef int int32_t;

inline void *operator new(uint32_t, void *p) noexcept { return p; }

namespace AbyssEngine {

struct Mesh;
struct PaintCanvas;

namespace AEMath {
struct Vector {
    float x, y, z;
};
// AEMath::Matrix is a 15-float (0x3c byte) object with an engine ctor/operator=.
struct Matrix {
    float m[15];
    Matrix();                            // 0x0006f118
    Matrix &operator=(const Matrix &o);  // 0x0006f148
};
} // namespace AEMath

} // namespace AbyssEngine

using Mesh = AbyssEngine::Mesh;
using PaintCanvas = AbyssEngine::PaintCanvas;
using Matrix = AbyssEngine::AEMath::Matrix;
using Vector = AbyssEngine::AEMath::Vector;

// Engine container: { size, data, cap }.
template <class T>
struct Array {
    uint32_t size;
    T *data;
    uint32_t cap;
};

struct MeshMerger {
    MeshMerger(int rows, int cols, PaintCanvas *canvas, uint16_t flags);
    MeshMerger(const Array<uint16_t> &meshIds, Array<Matrix> transforms,
               PaintCanvas *canvas, uint16_t flags);
    ~MeshMerger();

    void render();
    void setMatrix(int index, const Matrix &m);
    void setEnabled(int index, bool enabled);
    void setLod(int index, signed char lod);
    void setMesh(int index, signed char lod, uint16_t meshId);
    int init();
    void update();
    static void *transformMesh(Mesh *mesh, const Matrix &m);
};

// Field accessors via byte offset.
static inline int32_t &i32(void *self, uint32_t off) { return *(int32_t *)((char *)self + off); }
static inline uint32_t &u32(void *self, uint32_t off) { return *(uint32_t *)((char *)self + off); }
static inline uint16_t &u16(void *self, uint32_t off) { return *(uint16_t *)((char *)self + off); }
static inline int16_t &i16(void *self, uint32_t off) { return *(int16_t *)((char *)self + off); }
static inline uint8_t &u8(void *self, uint32_t off) { return *(uint8_t *)((char *)self + off); }
static inline int8_t &i8(void *self, uint32_t off) { return *(int8_t *)((char *)self + off); }
static inline float &f32(void *self, uint32_t off) { return *(float *)((char *)self + off); }
static inline void *&pp(void *self, uint32_t off) { return *(void **)((char *)self + off); }

#endif
