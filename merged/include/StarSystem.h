#ifndef WORK_CLASSES_STARSYSTEM_SRC_CLASS_H
#define WORK_CLASSES_STARSYSTEM_SRC_CLASS_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

struct StarSystem;
struct AEGeometry;
struct LensFlare;
struct Array;
struct Station;
struct SolarSystem;
struct Status;
struct Engine;
struct PlayerStatic;
struct FileRead;

struct Vector {
    float x;
    float y;
    float z;
};

struct Matrix {
    char data[0x3c];
};

template <class T>
static inline T &F(void *p, unsigned off) {
    return *(T *)((char *)p + off);
}

static inline void *&P(void *p, unsigned off) {
    return *(void **)((char *)p + off);
}

static inline int &I(void *p, unsigned off) {
    return *(int *)((char *)p + off);
}

static inline uint32_t &U(void *p, unsigned off) {
    return *(uint32_t *)((char *)p + off);
}

static inline float &FL(void *p, unsigned off) {
    return *(float *)((char *)p + off);
}

static inline uint8_t &B(void *p, unsigned off) {
    return *(uint8_t *)((char *)p + off);
}

static inline uint32_t array_len(void *array) {
    return U(array, 0);
}

static inline void *array_data(void *array) {
    return P(array, 4);
}

#endif
