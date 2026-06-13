#ifndef GOF2_STARSYSTEM_H
#define GOF2_STARSYSTEM_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
struct StarSystem;
struct AEGeometry;
struct LensFlare;
struct Station;
struct SolarSystem;
struct Status;
struct Engine;
struct PlayerStatic;
struct FileRead;

static inline void *&P(void *p, unsigned off) {
    return *(void **)((char *)p + off);
}

static inline int &I(void *p, unsigned off) {
    return *(int *)((char *)p + off);
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

class StarSystem {
public:
    // ---- members (declared in original 32-bit offset order; natural 64-bit layout) ----
    Vector sunLightColor;                   // +0x00  sun light/diffuse color (r,g,b floats)
    uint8_t supernovaSystem;                // +0x0c  inSupernovaSystem() flag
    uint32_t supernovaSunTexture;           // +0x10  supernova sun billboard texture handle
    Array<uint32_t> *texturesArray;         // +0x14  station/planet texture handles
    void *playerTargets;                    // +0x18  flattened Array<KIPlayer*> (intro camera targets)
    Array<AEGeometry *> *planetsArray;      // +0x1c  sun + planet/station billboard meshes
    Array<Vector> *positionsArray;          // +0x20  per-mesh world positions
    Array<int> *stationIdxArray;            // +0x24  station indices
    uint8_t abstractSystem;                 // +0x28  getSystem()==0 (no real solar system) flag
    LensFlare *lensFlare;                   // +0x2c  sun lens flare
    Vector lightDirection;                  // +0x30  (light direction)
    uint32_t tintColor;                     // +0x3c  lens-flare / system tint color
    AEGeometry *sunStreak;                  // +0x40  sun-streak billboard geometry
    AEGeometry *planetRing;                 // +0x44  planet-ring billboard geometry
    uint32_t planetRingTexture;             // +0x48  planet-ring texture handle
    int planetRingIndex;                    // +0x4c  index of planet that has a ring (-1 = none)
    uint32_t selectedStationSlot;           // +0x50  special/selected station slot index
    uint8_t fogEnabled;                     // +0x54  fog enabled flag
    float planetScale;                      // +0x58  selected-planet base scale
    float planetRingScaleOffset;            // +0x5c  selected-planet scale offset

    StarSystem(int mode);
    ~StarSystem();

    // ---- methods (converted from free functions) ----
    // Container members held as real Array<T> (== std::vector<T>) heap objects:
    //   texturesArray    station/planet texture handles
    //   planetsArray     sun + planet/station billboard meshes
    //   positionsArray   per-mesh world positions
    //   stationIdxArray  station indices
    // playerTargets remains on the flattened {count,data} container ABI because
    // LevelScript.cpp dereferences it directly via P(targets, 4); migrating it would need a
    // matching cross-file edit there.
    void *getPlanetTargets();   // flattened Array<KIPlayer*> (intro camera targets)
    void *getPlanets();         // Array<AEGeometry*>* of planet meshes
    Vector getLightDirection();
    void initLight();
    void render();
    void render2D();
    void renderSunStreak();
    void renderSunStreak_tail(void *geom);
    void scaleSunDuringSupernovaIntro(int amount);
    void switchPlanetForIntro();
    void switchSunForSupernovaExpansion();
    void switchSunForSupernovaIntro();
    void switchSunForSupernovaReversal();
    void updateSupernova(int dt);
};
#endif
