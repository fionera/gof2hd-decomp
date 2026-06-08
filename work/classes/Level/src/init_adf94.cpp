#include "class.h"

struct Route;
struct Objective;
struct LODManager;
struct ParticleSystemManager;
struct Mission;
struct SolarSystem;
struct StarSystem;

__attribute__((visibility("hidden"))) extern int   *g_init_stack;     // [DAT_000be25c]
__attribute__((visibility("hidden"))) extern int  **g_init_canvas;    // [DAT_000be260]
__attribute__((visibility("hidden"))) extern int  **g_init_statusA;   // [DAT_000be264]
__attribute__((visibility("hidden"))) extern char **g_init_flagStack; // [DAT_000be268]
__attribute__((visibility("hidden"))) extern int  **g_init_missionDef;// [DAT_000be5a4]
__attribute__((visibility("hidden"))) extern int  **g_init_settings;  // [DAT_000be5ac]
__attribute__((visibility("hidden"))) extern char **g_init_bmFlag;    // [DAT_000be5b0]

extern "C" {
void  operator_delete_init(void *p);
void *Route_dtor_init(Route *r);
void *Objective_dtor_init(Objective *o);
void *Level_opnew_init(unsigned size);
void  LODManager_ctor_init(LODManager *m);
void  ParticleSystemManager_ctor_init(ParticleSystemManager *m, int canvas, int a, int b,
                                      int c, int d, int e);
int   Status_inAlienOrbit_init();
int   Status_getSystem_init();
int   SolarSystem_getTextureIndex_init();
int   Status_getCurrentCampaignMission_init();
int   Status_getStationStack_init(int status);
int   Status_getMission_init();
void  Status_setMission_init(Mission *m);
int   Status_gameWon_init();
int   Status_inBlackMarketSystem_init();
int   Mission_isEmpty_init();
int   Mission_isCampaignMission_init(Mission *m);
int   Station_getIndex_init(void *s);
int   SolarSystem_currentOrbitHasWarpGate_init();
int   SolarSystem_getStations_init(SolarSystem *s);
void  Level_createSpace_init(Level *self);
void  Level_createPlayer_init(Level *self);
void  Level_createAsteroids_init(Level *self);
void  Level_createGasClouds_init(Level *self);
void  Level_createMission_init(Level *self);
void  Level_createCampaignMission_init(Level *self);
void  Level_createScene_init(Level *self);
void  Level_createStaticObjects_init(Level *self);
void  Level_createSentryGuns_init(Level *self);
void  Level_createFighterTurrets_init(Level *self);
void  Level_createWingmen_init(Level *self);
void  Level_assignGuns_init(Level *self);
void  Level_connectPlayers_init(Level *self);
int   KIPlayer_isWingMan_init();
void  PlayerEgo_setRoute_init(int route);
// Places the player at the appropriate spawn (station/warpgate/planet/origin); all the candidate
// positions involve SIMD vector math the decompiler corrupted, so delegate the whole choice.
void  Level_init_placePlayer(Level *self, int statusA, int stationStack);
}

// Level::init() — staged level setup; runs over up to two ticks (the counter at self+0x134),
// then builds space, mission, scene, objects, guns and wires up the players.
extern "C" int Level_init(Level *thisptr)
{
    char *self = (char *)thisptr;
    int **statusA = 0;

    int stage = *(int *)(self + 0x134);
    if (stage == 0) {
        // --- first tick: tear down stale routes/objectives and build particle systems ---
        *(char *)(self + 0x68) = 0;
        *(char *)(self + 0x1b0) = 0;
        *(short *)(self + 0x189) = 0;
        if (*(Route **)(self + 0x108) != 0)
            operator_delete_init(Route_dtor_init(*(Route **)(self + 0x108)));
        *(int *)(self + 0x108) = 0;
        if (*(Route **)(self + 0x110) != 0)
            operator_delete_init(Route_dtor_init(*(Route **)(self + 0x110)));
        *(int *)(self + 0x110) = 0;
        if (*(Route **)(self + 0x10c) != 0)
            operator_delete_init(Route_dtor_init(*(Route **)(self + 0x10c)));
        *(int *)(self + 0x10c) = 0;
        if (*(Objective **)(self + 0x2c) != 0)
            operator_delete_init(Objective_dtor_init(*(Objective **)(self + 0x2c)));
        *(int *)(self + 0x2c) = 0;
        if (*(Objective **)(self + 0x28) != 0)
            operator_delete_init(Objective_dtor_init(*(Objective **)(self + 0x28)));
        *(int *)(self + 0x28) = 0;

        LODManager *lod = (LODManager *)Level_opnew_init(0x14);
        LODManager_ctor_init(lod);
        *(LODManager **)self = lod;

        int canvas = **g_init_canvas;
        statusA = g_init_statusA;

        bool dustVariant = false;
        if (Status_inAlienOrbit_init() == 0) {
            Status_getSystem_init();
            dustVariant = SolarSystem_getTextureIndex_init() == 0xc;
        }

        // the engine builds ten particle-system managers (engine trails, explosions, dust...).
        ParticleSystemManager *psm;
        psm = (ParticleSystemManager *)Level_opnew_init(100);
        ParticleSystemManager_ctor_init(psm, canvas, 1, 0x4e85, 0, 0xffff, 0);
        *(ParticleSystemManager **)(self + 0x78) = psm;
        psm = (ParticleSystemManager *)Level_opnew_init(100);
        ParticleSystemManager_ctor_init(psm, canvas, 1, 0x6a72, 0, 0xffff, 0);
        *(ParticleSystemManager **)(self + 0x84) = psm;
        psm = (ParticleSystemManager *)Level_opnew_init(100);
        ParticleSystemManager_ctor_init(psm, canvas, 1, 0x4e83, 1, 0xffff, 0);
        *(ParticleSystemManager **)(self + 0x74) = psm;
        psm = (ParticleSystemManager *)Level_opnew_init(100);
        ParticleSystemManager_ctor_init(psm, canvas, 1, 0x4e7a, 1, 0x4e7a, 1);
        *(ParticleSystemManager **)(self + 0x80) = psm;
        psm = (ParticleSystemManager *)Level_opnew_init(100);
        ParticleSystemManager_ctor_init(psm, canvas, 1, 0x5e20, 1, 0x5e20, 1);
        *(ParticleSystemManager **)(self + 0x98) = psm;
        psm = (ParticleSystemManager *)Level_opnew_init(100);
        ParticleSystemManager_ctor_init(psm, canvas, 1, dustVariant ? 0x4ea9 : 0x4e7f, 1, 0, 0);
        *(ParticleSystemManager **)(self + 0x7c) = psm;
        psm = (ParticleSystemManager *)Level_opnew_init(100);
        ParticleSystemManager_ctor_init(psm, canvas, 1, 0x4e7c, 0, 0x4e7c, 0);
        *(ParticleSystemManager **)(self + 0x88) = psm;
        psm = (ParticleSystemManager *)Level_opnew_init(100);
        ParticleSystemManager_ctor_init(psm, canvas, 1, 0x6a7c, 1, 0x6a7c, 1);
        *(ParticleSystemManager **)(self + 0x8c) = psm;
        psm = (ParticleSystemManager *)Level_opnew_init(100);
        ParticleSystemManager_ctor_init(psm, canvas, 1, 0x6ab9, 1, 0xffff, 0);
        *(ParticleSystemManager **)(self + 0x9c) = psm;
        psm = (ParticleSystemManager *)Level_opnew_init(100);
        ParticleSystemManager_ctor_init(psm, canvas, 1, 0x6aaf, 1, 0xffff, 0);
        *(ParticleSystemManager **)(self + 0x94) = psm;

        Level_createSpace_init(thisptr);

        if (*(int *)(self + 0xc0) == 3) {
            Level_createPlayer_init(thisptr);
            int stk = 0;
            if (**g_init_flagStack != 0 && *(int *)(*(int *)(self + 0xf0) + 8) != 0)
                stk = Status_getStationStack_init((int)*statusA);
            Level_init_placePlayer(thisptr, (int)*statusA, stk);
        }
        stage = *(int *)(self + 0x134);
    }

    if (stage != 1) {
        *(int *)(self + 0x134) = stage + 1;
        return 0;
    }

    if (*(int *)(self + 0xc0) != 4 && *(int *)(self + 0xc0) != 0x17) {
        Level_createAsteroids_init(thisptr);
        Level_createGasClouds_init(thisptr);
    }

    Mission *m = (Mission *)Status_getMission_init();
    if (m == 0)
        Status_setMission_init((Mission *)**g_init_missionDef);

    int mode = *(int *)(self + 0xc0);
    if (mode == 3) {
        bool campaign = Mission_isEmpty_init() == 0 && Mission_isCampaignMission_init(m) != 0;
        bool madeScene = false;
        if (campaign) {
            if (*(int *)(self + 0xc0) != 3) { madeScene = true; }
            else {
                int won = Status_gameWon_init();
                int *settings = *g_init_settings;
                bool skip = won != 0 && *(char *)(settings + 0x37) == 0 &&
                            *(char *)(settings + 0x35) == 0;
                if (skip) {
                    Level_createMission_init(thisptr);
                    if (**g_init_bmFlag != 0 && Status_inBlackMarketSystem_init() != 0)
                        Level_init_placePlayer(thisptr, (int)*statusA, 0);
                } else if (*(int *)(self + 0xc0) != 3) {
                    madeScene = true;
                } else {
                    Status_getMission_init();
                    if (Mission_isEmpty_init() == 0) {
                        Mission *mm = (Mission *)Status_getMission_init();
                        if (Mission_isCampaignMission_init(mm) != 0)
                            Level_createCampaignMission_init(thisptr);
                    }
                }
            }
        } else {
            Level_createMission_init(thisptr);
            if (**g_init_bmFlag != 0 && Status_inBlackMarketSystem_init() != 0)
                Level_init_placePlayer(thisptr, (int)*statusA, 0);
        }
        if (madeScene) {
            Level_createScene_init(thisptr);
            *(int *)(self + 0xc0) = mode;
        }
    } else {
        Level_createScene_init(thisptr);
        *(int *)(self + 0xc0) = mode;
    }

    Level_createStaticObjects_init(thisptr);
    mode = *(int *)(self + 0xc0);
    if (mode != 0x17 && mode != 4 &&
        (mode != 2 || Status_getCurrentCampaignMission_init() != 0x2b)) {
        Level_createSentryGuns_init(thisptr);
        Level_createFighterTurrets_init(thisptr);
        Level_createWingmen_init(thisptr);
    }
    Level_assignGuns_init(thisptr);
    if (*(int *)(self + 0xc0) != 3)
        *(int *)(self + 0xc0) = 3;
    Level_connectPlayers_init(thisptr);
    if (*(Route **)(self + 0xf0) != 0)
        PlayerEgo_setRoute_init(*(int *)(self + 0xf0));

    // recompute enemiesLeft.
    unsigned *list = *(unsigned **)(self + 0xf8);
    int initial = 0;
    int enemies = 0;
    if (list != 0) {
        for (unsigned i = 0; i < *list; i = i + 1) {
            int e = *(int *)(list[1] + i * 4);
            if (*(char *)(e + 0x41) == 0 && *(char *)(e + 0x71) == 0 && *(char *)(e + 0x3f) == 0) {
                int wm = KIPlayer_isWingMan_init();
                list = *(unsigned **)(self + 0xf8);
                if (wm == 0) {
                    e = *(int *)(list[1] + i * 4);
                    if (*(char *)(e + 0x44) == 0 && *(char *)(e + 0x3c) == 0)
                        enemies = enemies + (*(unsigned char *)(e + 0x3d) ^ 1);
                }
            }
        }
        if (list != 0)
            enemies = enemies - *(int *)(self + 0x120);
    }
    *(int *)(self + 0x128) = 0;
    *(int *)(self + 0x118) = enemies;
    if (*(unsigned **)(self + 0xfc) != 0)
        initial = **(unsigned **)(self + 0xfc);
    *(int *)(self + 0x184) = 0;
    *(char *)(self + 0x188) = 0;
    *(int *)(self + 0x1c) = 0;
    *(int *)(self + 0x128) = initial;
    *(int *)(self + 0x12c) = 0;
    *(char *)(self + 0x13c) = 0;
    *(char *)(self + 0x70) = 1;
    return 1;
}
