#ifndef WORK_CLASSES_PLAYERASTEROID_SRC_CLASS_H
#define WORK_CLASSES_PLAYERASTEROID_SRC_CLASS_H

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef int int32_t;

namespace AbyssEngine {

struct String {
    void *text;
    uint32_t field_04;
    uint32_t size;

    String(const char *text, bool copy);
};

namespace AEMath {
struct Vector {
    float x;
    float y;
    float z;
};

struct Matrix {
    float m[16];
};
} // namespace AEMath

} // namespace AbyssEngine

using String = AbyssEngine::String;
using Vector = AbyssEngine::AEMath::Vector;
using Matrix = AbyssEngine::AEMath::Matrix;

struct AEGeometry;
struct Player;
struct Explosion;
struct TargetFollowCamera;
struct ArrayInt;

template <class T>
static inline T &F(void *self, uint32_t off)
{
    return *(T *)((char *)self + off);
}

template <class T>
static inline T const &F(const void *self, uint32_t off)
{
    return *(const T *)((const char *)self + off);
}

struct PlayerAsteroid {
    // @portable-fields
    void* f_0; // 0x0
    void* f_4; // 0x4
    void* f_8; // 0x8
    int f_c; // 0xc
    unsigned char _pad_10[44];
    int f_3c; // 0x3c
    unsigned char _pad_40[12];
    int f_4c; // 0x4c
    void* f_50; // 0x50
    unsigned char _pad_54[36];
    void* f_78; // 0x78
    unsigned char _pad_7c[12];
    int f_88; // 0x88
    unsigned char _pad_8c[84];
    int f_e0; // 0xe0
    unsigned char _pad_e4[17];
    int f_f5; // 0xf5
    unsigned char _pad_f9[11];
    int f_104; // 0x104
    int f_108; // 0x108
    unsigned char _pad_10c[12];
    int f_118; // 0x118
    int f_11c; // 0x11c
    int f_120; // 0x120
    int f_124; // 0x124
    int f_128; // 0x128
    void* f_12c; // 0x12c
    unsigned char _pad_130[4];
    int f_134; // 0x134
    int f_138; // 0x138
    int f_13c; // 0x13c
    int f_140; // 0x140
    int f_144; // 0x144
    int f_148; // 0x148
    int f_14c; // 0x14c
    int f_150; // 0x150
    unsigned char _pad_154[4];
    int f_158; // 0x158
    int f_15c; // 0x15c
    int f_160; // 0x160
    int f_164; // 0x164
    int f_168; // 0x168
    int f_16c; // 0x16c

    PlayerAsteroid(int playerId, AEGeometry *geometry, int explosionType, int asteroidIndex,
                   const Vector &position, float scaling, int quality);

    void setAsteroidIndex(int asteroidIndex);
    Vector getPosition();
    void setPosition(const Vector &position);
    void translate(const Vector &delta);
    uint8_t isMinable();
    int getQuality();
    float getScaling();
    int getQualityFrameIndex();
    String getQualityString();
    void setAsteroidCenter(Vector center);
    void setRotationEnabled(bool enabled);
    void initPush(const Vector &target, int duration);
    void push(int delta);
    void update(int delta);
    void render();
    void outerCollide(float x, float y, float z);
    void outerCollide(Vector value);
    Vector getProjectionVector(const Vector &value);
    bool collide(float x, float y, float z);
};

extern "C" void *operator_new(uint32_t size);
extern "C" void operator_delete(void *ptr);

#endif
