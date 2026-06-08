#include "class.h"

struct Station;
struct Galaxy;
struct SolarSystem;
struct AEGeometry;
struct PlayerGasCloud;
struct ParticleSystemManager;

__attribute__((visibility("hidden"))) extern int    *g_cgc_stack;   // [DAT_000bffc0]
__attribute__((visibility("hidden"))) extern Galaxy **g_cgc_galaxy; // [DAT_000bffc4]
__attribute__((visibility("hidden"))) extern int    *g_cgc_rng;     // [DAT_000bffcc]
__attribute__((visibility("hidden"))) extern void  **g_cgc_canvas;  // [DAT_000bffd4]

extern "C" {
int  Status_getStation_cgc();
int  Galaxy_getPlasmaProbabilities_cgc(Galaxy *g, Station *st);
int  Status_getShip_cgc();
int  Ship_getFirstEquipmentOfSort_cgc(int ship);
int  Status_inAlienOrbit_cgc();
int  Status_getSystem_cgc();
int  SolarSystem_getIndex_cgc();
int  SolarSystem_getRoutes_cgc();
int  Status_getCurrentCampaignMission_cgc();
int  Station_getIndex_cgc(Station *s);
int  AERandom_nextInt_cgc(int rng);
void *Level_opnew_cgc(unsigned size);
void ArrayGasCloud_ctor_cgc(void *a);
void ArraySetLength_GasCloud_cgc(int n, void *a);
void AEGeometry_ctor_cgc(AEGeometry *g, int mesh, void *canvas, int flag);
void PlayerGasCloud_ctor_cgc(PlayerGasCloud *c, int kind, ParticleSystemManager *psm,
                             AEGeometry *geo, Vector *pos);
// Generates a random spawn position for cloud index `i` that is far enough from the player and
// from the origin (the original is a SIMD reject-sampling loop Ghidra could not lift). `boss`
// pins the very first cloud to a fixed spot.
void Level_cgc_randomPos(int rng, int boss, unsigned i, Vector *out);
}

// Level::createGasClouds() — populates the orbit with plasma/gas clouds when the player carries a
// collector and the system permits it.
void Level::createGasClouds()
{
    char *self = (char *)this;

    Galaxy *gal = *g_cgc_galaxy;
    Station *st = (Station *)Status_getStation_cgc();
    int *prob = (int *)Galaxy_getPlasmaProbabilities_cgc(gal, st);

    int ship = Status_getShip_cgc();
    if (Ship_getFirstEquipmentOfSort_cgc(ship) == 0 || Status_inAlienOrbit_cgc() != 0)
        return;

    Status_getSystem_cgc();
    if (SolarSystem_getIndex_cgc() != 10 && *prob == 0xcc) {
        Status_getSystem_cgc();
        if (SolarSystem_getRoutes_cgc() == 0)
            return;
    }

    void *arr = Level_opnew_cgc(0xc);
    ArrayGasCloud_ctor_cgc(arr);
    *(void **)(self + 0xf4) = arr;

    bool boss = false;
    if (Status_getCurrentCampaignMission_cgc() == 0x8e) {
        Station *s2 = (Station *)Status_getStation_cgc();
        boss = Station_getIndex_cgc(s2) == 0x4f;
    }

    int rng = *g_cgc_rng;
    int roll = AERandom_nextInt_cgc(rng);
    // count = base + (prob[1]/denom) * (roll+4); base 3 for the boss fight.
    float countF = (float)(boss ? 3.0f : 0.0f) + ((float)prob[1] / 1.0f) * (float)(roll + 4);
    int count = (countF > 0.0f) ? (int)countF : 0;
    ArraySetLength_GasCloud_cgc(count, *(void **)(self + 0xf4));

    void *canvas = *g_cgc_canvas;
    for (unsigned i = 0; i < **(unsigned **)(self + 0xf4); i = i + 1) {
        int kind = *prob;
        Vector pos;
        Level_cgc_randomPos(rng, boss ? 1 : 0, i, &pos);

        AEGeometry *geo = (AEGeometry *)Level_opnew_cgc(0xc0);
        AEGeometry_ctor_cgc(geo, 0x37d1, canvas, 0);
        PlayerGasCloud *cloud = (PlayerGasCloud *)Level_opnew_cgc(0x16c);
        PlayerGasCloud_ctor_cgc(cloud, kind,
                                *(ParticleSystemManager **)(self + 0x94), geo, &pos);
        *(PlayerGasCloud **)(*(int *)(*(int *)(self + 0xf4) + 4) + i * 4) = cloud;
        char *c = *(char **)(*(int *)(*(int *)(self + 0xf4) + 4) + i * 4);
        (*(void (**)())(*(int *)c + 0x14))();
    }
}
