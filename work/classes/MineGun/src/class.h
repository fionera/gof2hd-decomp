#ifndef WORK_CLASSES_MINEGUN_SRC_CLASS_H
#define WORK_CLASSES_MINEGUN_SRC_CLASS_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

struct Gun;
struct Level;
struct PlayerEgo;
struct Explosion;
struct AEGeometry;
struct TargetFollowCamera;

namespace AbyssEngine {
namespace AEMath {
struct Vector {
    float x;
    float y;
    float z;
};
}
}

typedef AbyssEngine::AEMath::Vector Vector;

template <class T>
struct Array {
    uint32_t length;
    T *data;
    uint32_t capacity;
};

struct MineGun {
    // @portable-fields
    void* f_0; // 0x0
    unsigned char _pad_4[4];
    void* f_8; // 0x8
    unsigned char _pad_c[36];
    void* f_30; // 0x30
    unsigned char _pad_34[12];
    void* f_40; // 0x40
    unsigned char _pad_44[108];
    void* f_b0; // 0xb0
    void* f_b4; // 0xb4
    void* f_b8; // 0xb8
    void* f_bc; // 0xbc

    MineGun(Gun *gun, int param_2, int param_3, int param_4, Level *level);
    ~MineGun();
    void setPlayer(PlayerEgo *player);
    void render();
    void update(int delta);
};

template <class T>
static inline T &F(void *self, unsigned off) {
    return *(T *)((char *)self + off);
}

static inline void *&P(void *self, unsigned off) { return *(void **)((char *)self + off); }
static inline int32_t &I(void *self, unsigned off) { return *(int32_t *)((char *)self + off); }
static inline uint32_t &U(void *self, unsigned off) { return *(uint32_t *)((char *)self + off); }
static inline uint8_t &UC(void *self, unsigned off) { return *(uint8_t *)((char *)self + off); }
static inline float &FL(void *self, unsigned off) { return *(float *)((char *)self + off); }

typedef int v4i __attribute__((__vector_size__(16), __aligned__(4)));

#endif
