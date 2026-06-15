#ifndef GOF2_STARSYSTEM_H
#define GOF2_STARSYSTEM_H
#include "gof2/common.h"

// Galaxy on Fire 2 -- the renderable star system shown in the cockpit/intro views.
// Owns the sun, planet and station billboard geometries, their world positions and
// texture handles, the sun lens-flare and the supernova animation state.

class AEGeometry;
class LensFlare;

class StarSystem {
public:
    Vector sunLightColor;                   // sun light/diffuse color (r,g,b)
    uint8_t supernovaSystem;                // inSupernovaSystem() flag
    uint32_t supernovaSunTexture;           // supernova sun billboard texture handle
    Array<uint32_t> *texturesArray;         // station/planet texture handles
    void *playerTargets;                    // flattened Array<KIPlayer*> (intro camera targets)
    Array<AEGeometry *> *planetsArray;      // sun + planet/station billboard meshes
    Array<Vector> *positionsArray;          // per-mesh world positions
    Array<int> *stationIdxArray;            // station indices
    uint8_t abstractSystem;                 // getSystem()==0 (no real solar system) flag
    LensFlare *lensFlare;                   // sun lens flare
    Vector lightDirection;
    uint32_t tintColor;                     // lens-flare / system tint color
    AEGeometry *sunStreak;                  // sun-streak billboard geometry
    AEGeometry *planetRing;                 // planet-ring billboard geometry
    uint32_t planetRingTexture;             // planet-ring texture handle
    int planetRingIndex;                    // index of planet that has a ring (-1 = none)
    uint32_t selectedStationSlot;           // special/selected station slot index
    uint8_t fogEnabled;                     // fog enabled flag
    float planetScale;                      // selected-planet base scale
    float planetRingScaleOffset;            // selected-planet scale offset

    StarSystem(int mode);
    ~StarSystem();

    // playerTargets is a flattened {count,data} Array<KIPlayer*> container because
    // LevelScript.cpp dereferences it directly; getPlanets() returns the typed array.
    void *getPlanetTargets();
    void *getPlanets();
    Vector getLightDirection();
    void initLight();
    void render();
    void render2D();
    void renderSunStreak();
    void renderSunStreak_tail(AEGeometry *geom);
    void scaleSunDuringSupernovaIntro(int amount);
    void switchPlanetForIntro();
    void switchSunForSupernovaExpansion();
    void switchSunForSupernovaIntro();
    void switchSunForSupernovaReversal();
    void updateSupernova(int dt);
};
#endif
