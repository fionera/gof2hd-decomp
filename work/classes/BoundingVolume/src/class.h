#ifndef WORK_CLASSES_BOUNDINGVOLUME_SRC_CLASS_H
#define WORK_CLASSES_BOUNDINGVOLUME_SRC_CLASS_H

// Galaxy on Fire 2 -- BoundingVolume class (Android libgof2hdaa.so, armv7 Thumb).
// Top-level class. Uses AbyssEngine::AEMath::Vector and Array<BoundingVolume*>.
// Field offsets recovered per-method from the target disassembly; accessed via
// byte-offset casts from `this`.
//
// Field layout (this):
//   0x00 void*   vtable
//   0x04 Array<BoundingVolume*>* children
//   0x08 Vector  center  (x@0x08, y@0x0c, z@0x10)
//   0x14 Vector  extents (x@0x14, y@0x18, z@0x1c)

typedef unsigned char uint8_t;
typedef signed char int8_t;
typedef unsigned short uint16_t;
typedef short int16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef int int32_t;

namespace AbyssEngine {
namespace AEMath {
struct Vector {
    float x, y, z;
};
} // namespace AEMath
} // namespace AbyssEngine

using Vector = AbyssEngine::AEMath::Vector;

struct BoundingVolume;

// Array<T> layout: length@0x0, data@0x4.
template <class T>
struct Array {
    uint32_t length;
    T *data;
};

struct BoundingVolume {
    BoundingVolume(float, float, float, float, float, float);
    ~BoundingVolume();
    static void getCollisionNormal(const Vector &out);
    void setVolume(BoundingVolume *src);
    int collide(float, float, float);
    Vector getProjectionVector(const Vector &v);
    void update(float, float, float);
    void staticProjectCollisionOnSurface(const Vector &v, Array<BoundingVolume *> *vols);
};

// Field accessors via byte offset.
static inline int32_t &i32(void *self, uint32_t off) { return *(int32_t *)((char *)self + off); }
static inline uint32_t &u32(void *self, uint32_t off) { return *(uint32_t *)((char *)self + off); }
static inline uint16_t &u16(void *self, uint32_t off) { return *(uint16_t *)((char *)self + off); }
static inline uint8_t &u8(void *self, uint32_t off) { return *(uint8_t *)((char *)self + off); }
static inline float &f32(void *self, uint32_t off) { return *(float *)((char *)self + off); }
static inline void *&pp(void *self, uint32_t off) { return *(void **)((char *)self + off); }

#endif
