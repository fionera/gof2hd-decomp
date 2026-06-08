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
