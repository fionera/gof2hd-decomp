#ifndef WORK_CLASSES_OBJECTGUN_SRC_CLASS_H
#define WORK_CLASSES_OBJECTGUN_SRC_CLASS_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef long long int64_t;

struct AEGeometry;
struct Array;
struct Explosion;
struct Gun;
struct Level;
struct ObjectGun;
struct PaintCanvas;
struct Player;
struct TargetFollowCamera;

struct Vector {
    float x;
    float y;
    float z;
};

struct Matrix {
    uint32_t words[15];
};

struct Vec4 {
    float x;
    float y;
    float z;
    float w;
};

struct MeshId {
    uint16_t id;
    uint16_t pad;
};

struct ObjectGun {
    // @portable-fields
    void* f_0; // 0x0
    void* f_4; // 0x4
    void* f_8; // 0x8
    void* f_c; // 0xc
    int f_10; // 0x10
    int f_14; // 0x14
    void* f_18; // 0x18
    int f_1c; // 0x1c
    int f_20; // 0x20
    void* f_24; // 0x24
    int f_28; // 0x28
    void* f_2c; // 0x2c
    void* f_30; // 0x30
    int f_34; // 0x34
    unsigned char _pad_38[4];
    void* f_3c; // 0x3c
    void* f_40; // 0x40
    int f_44; // 0x44
    int f_48; // 0x48
    int f_4c; // 0x4c
    int f_50; // 0x50
    int f_54; // 0x54
    int f_58; // 0x58
    int f_5c; // 0x5c
    int f_60; // 0x60
    int f_64; // 0x64
    int f_68; // 0x68
    int f_6c; // 0x6c
    int f_70; // 0x70
    int f_74; // 0x74
    int f_78; // 0x78
    int f_7c; // 0x7c
    int f_80; // 0x80
    int f_84; // 0x84
    int f_88; // 0x88
    int f_8c; // 0x8c
    unsigned char _pad_90[4];
    int f_94; // 0x94
    int f_98; // 0x98
    int f_9c; // 0x9c
    unsigned char _pad_a0[4];
    int f_a4; // 0xa4
    int f_a8; // 0xa8

    ObjectGun(int owner, Gun *gun, int mesh, uint32_t flags, Level *level);
    ~ObjectGun();

    static void setEnemies(Array *enemies);
    void setScaling(int x, int y);
    void replaceGun(int mesh);
    void update(int dt);
    void render();
};

template <class T>
static inline T &F(void *self, unsigned off)
{
    return *(T *)((char *)self + off);
}

template <class T>
static inline const T &F(const void *self, unsigned off)
{
    return *(const T *)((const char *)self + off);
}

#endif
