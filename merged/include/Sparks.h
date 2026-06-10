#ifndef WORK_CLASSES_SPARKS_SRC_CLASS_H
#define WORK_CLASSES_SPARKS_SRC_CLASS_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef short int16_t;

struct Sparks;

namespace AbyssEngine {
namespace AEMath {
struct Vector {
    float x;
    float y;
    float z;
};
struct Matrix {
    char bytes[0x3c];
};
}
}

template <class T>
static inline T &F(void *p, unsigned off) {
    return *(T *)((char *)p + off);
}

#endif
