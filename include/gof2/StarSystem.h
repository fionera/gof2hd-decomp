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

    // ---- methods (converted from free functions) ----
    StarSystem * StarSystem(int mode);
    Vector getLightDirection();
    void initLight();
    void render();
    void render2D();
    void renderSunStreak();
    void scaleSunDuringSupernovaIntro(int amount);
    void switchPlanetForIntro();
    void switchSunForSupernovaExpansion();
    void switchSunForSupernovaIntro();
    void switchSunForSupernovaReversal();
    void updateSupernova(int dt);
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

struct StarSystem {
    Vector field_0x30;                  // +0x30  (light direction)

    ~StarSystem();

    // ---- methods (converted from free functions) ----
    Vector getLightDirection();
    void initLight();
    void render();
    void render2D();
    void renderSunStreak();
    void scaleSunDuringSupernovaIntro(int amount);
    void switchPlanetForIntro();
    void switchSunForSupernovaExpansion();
    void switchSunForSupernovaIntro();
    void switchSunForSupernovaReversal();
    void updateSupernova(int dt);
};
#endif
