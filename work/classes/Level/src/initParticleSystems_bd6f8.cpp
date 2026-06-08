#include "class.h"

struct SolarSystem;
struct KIPlayer;
struct ParticleSystemManager;

__attribute__((visibility("hidden"))) extern int   *g_ips_stack;    // [DAT_000cda44]
__attribute__((visibility("hidden"))) extern unsigned *g_ips_canvas; // [DAT_000cda98]
__attribute__((visibility("hidden"))) extern int   *g_ips_envA;     // [DAT_000cdaa0] env record
__attribute__((visibility("hidden"))) extern int   *g_ips_envB;     // [DAT_000cde3c]/[DAT_000cde38]
__attribute__((visibility("hidden"))) extern int  (*g_ips_addSystem)(int, void *, int, int); // [DAT_000cde40]
__attribute__((visibility("hidden"))) extern void (*g_ips_enableEmit)(int);  // [DAT_000cde44]

extern "C" {
void *Level_opnew_ips(unsigned size);
void  Array_int_ctor_ips(void *a);
void  ArraySetLength_int_ips(unsigned n, void *a);
int   Status_getSystem_ips();
int   SolarSystem_hasPirateBase_ips(SolarSystem *s);
int   SolarSystem_getTextureIndex_ips();
int   SolarSystem_getRace_ips();
int   Status_inAlienOrbit_ips();
int   Status_inSupernovaSystem_ips();
int   Status_getCurrentCampaignMission_ips();
int   Status_getShip_ips();
int   Ship_getIndex_ips();
int   KIPlayer_getType_ips(KIPlayer *k);
int   ParticleSystemManager_addSystem_ips(int mgr, void *ref, int kind, int flag);
void  ParticleSystemManager_init_ips(ParticleSystemManager *m, void *ctx);
void  ParticleSystemManager_enableSystemEmit_ips(int mgr, int sys);
void  PaintCanvas_CameraGetCurrent_ips();
int   PaintCanvas_CameraGetLocal_ips(unsigned c);
int   AEGeometry_getReferenceMatrix_ips();
void  AEGeometry_updateReferenceMatrix_ips();
// Builds the per-asteroid dust descriptor block (huge SIMD struct init Ghidra mangled) and writes
// the resulting system handle into the array at self+0xa8.
void  Level_ips_buildAsteroidDust(Level *self, void *arr);
// Computes the ambient-light / nebula tint for the current orbit and writes it into the engine
// env record (corrupted SIMD packing in the original).
void  Level_ips_applyAmbient(Level *self);
// Registers one of the player-engine particle systems (kind) with a unit reference transform.
int   Level_ips_addPlayerSystem(Level *self, int kind);
}

// Level::initParticleSystems() — wires up all the in-flight particle systems (engine trails,
// explosions, asteroid dust, nebula tint) once the player and scene exist.
void Level::initParticleSystems()
{
    char *self = (char *)this;

    if (*(int *)(self + 0xf0) != 0) {
        // per-asteroid dust systems.
        if (*(int *)(self + 0xa4) != 0) {
            void *arr = Level_opnew_ips(0xc);
            Array_int_ctor_ips(arr);
            *(void **)(self + 0xa8) = arr;
            ArraySetLength_int_ips(**(unsigned **)(self + 0xa4), arr);
            Level_ips_buildAsteroidDust(this, arr);
        }

        // camera-locked sky particle system.
        unsigned canvas = *g_ips_canvas;
        PaintCanvas_CameraGetCurrent_ips();
        int local = PaintCanvas_CameraGetLocal_ips(canvas);
        int sys = ParticleSystemManager_addSystem_ips(*(int *)(self + 0x88), (void *)local, 4, 0);
        *(int *)(self + 0x64) = sys;

        // pirate-base smoke plume attached to the pirate flagship.
        if (Status_getSystem_ips() != 0) {
            SolarSystem *ss = (SolarSystem *)Status_getSystem_ips();
            unsigned *en = *(unsigned **)(self + 0xf8);
            if (SolarSystem_hasPirateBase_ips(ss) != 0 && en != 0) {
                for (unsigned i = 0; i < *en; i = i + 1) {
                    KIPlayer *k = *(KIPlayer **)(en[1] + i * 4);
                    if (k != 0 && KIPlayer_getType_ips(k) == 0x37a3) {
                        AEGeometry_updateReferenceMatrix_ips();
                        int ref = AEGeometry_getReferenceMatrix_ips();
                        ParticleSystemManager_addSystem_ips(*(int *)(self + 0x7c), (void *)ref, 8, 0);
                        break;
                    }
                    en = *(unsigned **)(self + 0xf8);
                }
            }
        }

        PaintCanvas_CameraGetCurrent_ips();
        local = PaintCanvas_CameraGetLocal_ips(canvas);
        sys = ParticleSystemManager_addSystem_ips(*(int *)(self + 0x7c), (void *)local, 7, 0);
        *(int *)(self + 0x284) = sys;

        Level_ips_applyAmbient(this);
    }

    // nebula light direction.
    Level_ips_applyAmbient(this);

    // init the always-present managers.
    if (*(ParticleSystemManager **)(self + 0x80) != 0)
        ParticleSystemManager_init_ips(*(ParticleSystemManager **)(self + 0x80), 0);
    if (*(ParticleSystemManager **)(self + 0x7c) != 0)
        ParticleSystemManager_init_ips(*(ParticleSystemManager **)(self + 0x7c), 0);
    if (*(ParticleSystemManager **)(self + 0x88) != 0)
        ParticleSystemManager_init_ips(*(ParticleSystemManager **)(self + 0x88), 0);
    if (*(ParticleSystemManager **)(self + 0x8c) != 0)
        ParticleSystemManager_init_ips(*(ParticleSystemManager **)(self + 0x8c), 0);
    if (*(ParticleSystemManager **)(self + 0x98) != 0)
        ParticleSystemManager_init_ips(*(ParticleSystemManager **)(self + 0x98), 0);

    // the player-engine systems (slots 0x34..0x5c) all use a unit transform.
    *(int *)(self + 0x38) = Level_ips_addPlayerSystem(this, 10);
    *(int *)(self + 0x3c) = Level_ips_addPlayerSystem(this, 0xb);
    *(int *)(self + 0x48) = Level_ips_addPlayerSystem(this, 0x14);
    *(int *)(self + 0x34) = Level_ips_addPlayerSystem(this, 0x15);
    *(int *)(self + 0x50) = Level_ips_addPlayerSystem(this, 0x16);
    *(int *)(self + 0x54) = Level_ips_addPlayerSystem(this, 0x17);
    if (Status_getCurrentCampaignMission_ips() == 0x50) {
        *(int *)(self + 0x58) = Level_ips_addPlayerSystem(this, 0x18);
        *(int *)(self + 0x5c) = Level_ips_addPlayerSystem(this, 0x18);
    }

    ParticleSystemManager_init_ips(*(ParticleSystemManager **)(self + 0x74), 0);
    ParticleSystemManager_enableSystemEmit_ips(*(int *)(self + 0x74), *(int *)(self + 0x50));
    ParticleSystemManager_enableSystemEmit_ips(*(int *)(self + 0x74), *(int *)(self + 0x54));
    if (Status_getCurrentCampaignMission_ips() == 0x50) {
        ParticleSystemManager_enableSystemEmit_ips(*(int *)(self + 0x74), *(int *)(self + 0x58));
        ParticleSystemManager_enableSystemEmit_ips(*(int *)(self + 0x74), *(int *)(self + 0x5c));
    }

    void (*enableEmit)(int) = g_ips_enableEmit;
    enableEmit(*(int *)(self + 0x9c));
    enableEmit(*(int *)(self + 0x78));
    enableEmit(*(int *)(self + 0x84));
    if (*(ParticleSystemManager **)(self + 0x94) != 0)
        ParticleSystemManager_init_ips(*(ParticleSystemManager **)(self + 0x94), 0);
}
