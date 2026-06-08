#ifndef WORK_CLASSES_LODMESHMERGER_SRC_CLASS_H
#define WORK_CLASSES_LODMESHMERGER_SRC_CLASS_H

// Galaxy on Fire 2 -- LodMeshMerger class (Android libgof2hdaa.so, armv7 Thumb).
// Top-level class. Its API uses AbyssEngine::{Mesh, PaintCanvas, AEMath::Matrix}.
// Field offsets recovered per-method from the target disassembly; accessed via
// byte-offset casts from `this`.
//
// Field layout (this):
//   0x00 int   rows
//   0x04 u16   flags
//   0x06 u8    initFlag
//   0x08 Array<Mesh*> {len@0x08, data@0x0c}
//   0x14 PaintCanvas* canvas
//   0x18 u32   meshId
//   0x1c u32   transformId
//   0x20 void* outMesh
//   0x24 void* slots (transformed-mesh ptr table)
//   0x28 Matrix* matrices (0x3c each)
//   0x2c int8*  lods
//   0x30 u8*    enabled
//   0x34 u8*    visible
//   0x38 int    cols
//   0x3c u8     dirty

typedef unsigned char uint8_t;
typedef signed char int8_t;
typedef unsigned short uint16_t;
typedef short int16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef int int32_t;

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

struct LodMeshMerger {
    // @portable-fields
    int f_0; // 0x0
    uint16_t f_4; // 0x4
    uint8_t f_6; // 0x6
    unsigned char _pad_7[5];
    void* f_c; // 0xc
    unsigned char _pad_10[4];
    void* f_14; // 0x14
    int f_18; // 0x18
    int f_1c; // 0x1c
    void* f_20; // 0x20
    void* f_24; // 0x24
    void* f_28; // 0x28
    void* f_2c; // 0x2c
    void* f_30; // 0x30
    void* f_34; // 0x34
    int f_38; // 0x38
    uint8_t f_3c; // 0x3c

    LodMeshMerger(int rows, int cols, PaintCanvas *canvas, uint16_t flags);
    ~LodMeshMerger();

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
