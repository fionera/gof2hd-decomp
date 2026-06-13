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
    Vector lightDirection;                  // +0x30  (light direction)

    StarSystem(int mode);
    ~StarSystem();

    // ---- methods (converted from free functions) ----
    // Container members held at fixed byte offsets (real Array<T> == std::vector<T> heap objects):
    //   +0x14  Array<uint>*        station/planet texture handles
    //   +0x1c  Array<AEGeometry*>* sun + planet/station billboard meshes
    //   +0x20  Array<Vector>*      per-mesh world positions
    //   +0x24  Array<int>*         station indices
    // +0x18 (getPlanetTargets) remains on the flattened {count,data} container ABI because
    // LevelScript.cpp dereferences it directly via P(targets, 4); migrating it would need a
    // matching cross-file edit there.
    void *getPlanetTargets();   // +0x18  flattened Array<KIPlayer*> (intro camera targets)
    void *getPlanets();         // +0x1c  Array<AEGeometry*>* of planet meshes
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
