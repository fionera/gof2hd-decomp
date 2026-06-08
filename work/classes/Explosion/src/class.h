#ifndef WORK_CLASSES_EXPLOSION_SRC_CLASS_H
#define WORK_CLASSES_EXPLOSION_SRC_CLASS_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

struct Explosion {
    // @portable-fields
    unsigned char _pad_0[4];
    void* f_4; // 0x4
    void* f_8; // 0x8
};
struct AEGeometry;
struct Array;
struct PaintCanvas;
struct TargetFollowCamera;

struct Vector {
    union {
        struct {
            float x;
            float y;
            float z;
        };
        char bytes[12];
    };
};

struct Matrix {
    char m[60];
};

static inline char *B(void *p, int off) { return (char *)p + off; }
static inline int &I(void *p, int off) { return *(int *)((char *)p + off); }
static inline uint32_t &U(void *p, int off) { return *(uint32_t *)((char *)p + off); }
static inline uint8_t &UC(void *p, int off) { return *(uint8_t *)((char *)p + off); }
static inline float &F(void *p, int off) { return *(float *)((char *)p + off); }
static inline void *&P(void *p, int off) { return *(void **)((char *)p + off); }
static inline uint64_t &U64(void *p, int off) { return *(uint64_t *)((char *)p + off); }

static inline AEGeometry *&G(void *p, int off) { return *(AEGeometry **)((char *)p + off); }
static inline Array *&A(void *p, int off) { return *(Array **)((char *)p + off); }

#endif
