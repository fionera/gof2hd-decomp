#ifndef WORK_CLASSES_AEGEOMETRY_SRC_CLASS_H
#define WORK_CLASSES_AEGEOMETRY_SRC_CLASS_H

// Galaxy on Fire 2 -- AbyssEngine AEGeometry (Android libgof2hdaa.so, armv7 Thumb).
// Qualified target names are top-level: "AEGeometry::isVisible()" (no AbyssEngine:: on the class).
// Field offsets recovered per-method from the target disassembly; accessed via byte-offset casts.

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;
typedef unsigned long long uint64_t;

void *operator new(uint32_t size);
void *operator new[](uint32_t size);
void operator delete(void *ptr) noexcept;
void operator delete[](void *ptr) noexcept;
inline void *operator new(uint32_t, void *p) noexcept { return p; }

extern "C" void *__stack_chk_guard;
extern "C" __attribute__((noreturn)) void __stack_chk_fail(...);
extern "C" void *__aeabi_memcpy(void *dst, const void *src, uint32_t n);

struct AEGeometry {
    int f_0; // 0x0
    void* f_4; // 0x4
    unsigned char _pad_8[4];
    void* f_c; // 0xc
    int f_10; // 0x10
    int f_14; // 0x14
    void* f_18; // 0x18
    int f_1c; // 0x1c
    int f_20; // 0x20
    int f_24; // 0x24
    int f_28; // 0x28
    void* f_2c; // 0x2c
    int f_30; // 0x30
    int f_34; // 0x34
    int f_38; // 0x38
    int f_3c; // 0x3c
    int f_40; // 0x40
    int f_44; // 0x44
    uint8_t f_48; // 0x48
    uint8_t f_49; // 0x49
    unsigned char _pad_4a[2];
    int f_4c; // 0x4c
    int f_50; // 0x50
    void* f_54; // 0x54
    void* f_58; // 0x58
    void* f_5c; // 0x5c
    void* f_60; // 0x60
    void* f_64; // 0x64
    unsigned char _pad_68[16];
    int f_78; // 0x78
    int f_7c; // 0x7c
    int f_80; // 0x80
};

namespace AbyssEngine {
namespace AEMath {

struct Vector {
    float x, y, z;
};

struct Matrix {
    float m[15];
    Matrix();                                  // 0x0006f118
    Matrix &operator=(const Matrix &o);        // 0x0006f148
};

// AEMath free functions (resolved blx targets in the target).
void VectorNormalize(void *out, const Vector *v);        // 0x0006ec80
Vector operator-(const Vector &a, const Vector &b);      // 0x0006ec38
Vector MatrixGetUp(const Matrix &m);                     // 0x0006f4d8
Vector MatrixGetRight(const Matrix &m);                  // 0x0006f4e4
Vector MatrixGetPosition(const Matrix &m);               // 0x0006f16c

} // namespace AEMath

struct PaintCanvas {
    // Static transform/mesh ops; first arg is the canvas pointer.
    static uint32_t TransformGetLocal(uint32_t canvas, uint32_t tf);     // 0x000720c4
    static void TransformCreate(PaintCanvas *canvas, uint32_t *out);     // 0x000720ac
    static void TransformSetLocal(PaintCanvas *canvas, uint32_t tf, AEMath::Matrix *m); // 0x000721c0
    static void TransformAddChild(PaintCanvas *canvas, uint32_t tf, uint32_t child);    // 0x000720d0
};

} // namespace AbyssEngine

// Field accessors --------------------------------------------------------------
static inline int32_t &i32(void *self, uint32_t off) { return *(int32_t *)((char *)self + off); }
static inline uint32_t &u32(void *self, uint32_t off) { return *(uint32_t *)((char *)self + off); }
static inline uint8_t &u8(void *self, uint32_t off) { return *(uint8_t *)((char *)self + off); }
static inline float &f32(void *self, uint32_t off) { return *(float *)((char *)self + off); }
static inline void *&pp(void *self, uint32_t off) { return *(void **)((char *)self + off); }

#endif
