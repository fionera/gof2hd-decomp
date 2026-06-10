#ifndef WORK_CLASSES_ROCKETGUN_SRC_CLASS_H
#define WORK_CLASSES_ROCKETGUN_SRC_CLASS_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

struct RocketGun;
struct Gun;
struct Level;
struct Radar;

template <class T>
struct Array {
    uint32_t length;
    T *data;
    uint32_t capacity;
};

namespace AbyssEngine {
namespace AEMath {
struct Vector {
    float x;
    float y;
    float z;
};
struct Matrix {
    char m[0x3c];
};
}
}

typedef AbyssEngine::AEMath::Vector Vector;
typedef AbyssEngine::AEMath::Matrix Matrix;

template <class T>
static inline T &F(void *self, unsigned off) {
    return *(T *)((char *)self + off);
}

typedef int v4i __attribute__((__vector_size__(16), __aligned__(4)));

#endif
