#ifndef WORK_CLASSES_RADAR_SRC_CLASS_H
#define WORK_CLASSES_RADAR_SRC_CLASS_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

struct Level;
struct Hud;
struct Player;
struct Station;

namespace AbyssEngine {
struct String {
    ~String();
};

namespace AEMath {
struct Matrix;

struct Vector {
    union {
        struct {
            float x;
            float y;
            float z;
        };
        float v[3];
    };

};

Vector MatrixTransformVector(Matrix const &, Vector const &);
}

struct PaintCanvas {
    int GetScreenPosition(AEMath::Vector const &, AEMath::Vector &);
};
}

template <class T>
struct Array {
    uint32_t length;
    T *data;
    uint32_t cap;

    ~Array();
};

struct KIPlayer {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8();
    virtual void v9();
    virtual AbyssEngine::AEMath::Vector getPosition();
};

struct SolarSystem {
    Array<Station *> *getStations();
};

struct Status {
    SolarSystem *getSystem();
};

struct Globals {
    float sqrt(float);
};

extern Status *gStatus;
extern Globals *gGlobals;
extern AbyssEngine::PaintCanvas *gPaintCanvas;

void operator delete(void *) noexcept;

struct Radar {
    // @portable-fields
    void* f_0; // 0x0
    void* f_4; // 0x4
    int f_8; // 0x8
    void* f_c; // 0xc
    int f_10; // 0x10
    int f_14; // 0x14
    int f_18; // 0x18
    unsigned char _pad_1c[8];
    void* f_24; // 0x24
    unsigned char _pad_28[16];
    void* f_38; // 0x38
    int f_3c; // 0x3c
    int f_40; // 0x40
    unsigned char _pad_44[4];
    int f_48; // 0x48
    int f_4c; // 0x4c
    int f_50; // 0x50
    int f_54; // 0x54
    int f_58; // 0x58
    unsigned char _pad_5c[21];
    int f_71; // 0x71
    unsigned char _pad_75[43];
    int f_a0; // 0xa0
    int f_a4; // 0xa4
    int f_a8; // 0xa8
    int f_ac; // 0xac
    unsigned char _pad_b0[76];
    int f_fc; // 0xfc
    int f_100; // 0x100
    unsigned char _pad_104[8];
    int f_10c; // 0x10c
    int f_110; // 0x110
    int f_114; // 0x114
    int f_118; // 0x118
    int f_11c; // 0x11c
    int f_120; // 0x120
    unsigned char _pad_124[8];
    int f_12c; // 0x12c
    int f_130; // 0x130
    unsigned char _pad_134[32];
    int f_154; // 0x154
    int f_158; // 0x158
    int f_15c; // 0x15c
    int f_160; // 0x160
    int f_164; // 0x164
    int f_168; // 0x168
    int f_16c; // 0x16c
    int f_170; // 0x170
    int f_174; // 0x174
    int f_178; // 0x178
    int f_17c; // 0x17c
    int f_180; // 0x180
    unsigned char _pad_184[4];
    void* f_188; // 0x188
    void* f_18c; // 0x18c
    unsigned char _pad_190[24];
    int f_1a8; // 0x1a8
    unsigned char _pad_1ac[8];
    int f_1b4; // 0x1b4
    int f_1b8; // 0x1b8
    int f_1bc; // 0x1bc
    unsigned char _pad_1c0[4];
    int f_1c4; // 0x1c4
    unsigned char _pad_1c8[68];
    int f_20c; // 0x20c
    unsigned char _pad_210[8];
    int f_218; // 0x218
    int f_21c; // 0x21c
    int f_220; // 0x220
    int f_224; // 0x224
    int f_228; // 0x228
    int f_22c; // 0x22c
    int f_230; // 0x230

    Radar(Level *);
    ~Radar();

    void unlockAsteroid();
    uint8_t isPlasmaInRange();
    uint8_t hasScanner();
    int getTurretScopeWidth();
    bool stationLocked();
    int getPlanetDockIndex();
    void update(AbyssEngine::AEMath::Vector);
    void update(KIPlayer *);
    AbyssEngine::AEMath::Vector elipsoidIntersect(int, int, AbyssEngine::AEMath::Vector);
    void calcDistance(float, float, float, float, float, float);
    void drawCurrentLock(Hud *);
    long long draw(Player *, Hud *, int);
};

template <class T>
static inline T &F(void *p, unsigned off)
{
    return *(T *)((char *)p + off);
}

#endif
