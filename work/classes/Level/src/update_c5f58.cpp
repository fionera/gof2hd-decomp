#include "class.h"

struct Station;
struct Ship;
struct Item;
struct Player;
struct Hud;
struct LODManager;
struct ParticleSystemManager;

// PC-relative singleton / data holders.
__attribute__((visibility("hidden"))) extern int   *g_up_stack;     // [DAT_000d6238] stack-guard cookie addr
__attribute__((visibility("hidden"))) extern int  **g_up_statusA;   // [DAT_000d624c]
__attribute__((visibility("hidden"))) extern float  g_up_eqMax;     // DAT_000d6230
__attribute__((visibility("hidden"))) extern float *g_up_clampLo;   // [DAT_000d6244]
__attribute__((visibility("hidden"))) extern float *g_up_clampHi;   // [DAT_000d6248]
__attribute__((visibility("hidden"))) extern float *g_up_clampZ;    // [DAT_000d623c]
__attribute__((visibility("hidden"))) extern float *g_up_clampW;    // [DAT_000d6240]

extern "C" {
int  Status_getMission_up();
int  Mission_isEmpty_up();
void Level_updateMissionOrbit_call(Level *self, unsigned dt);
void Level_updateOrbit_call(Level *self, unsigned dt);
int  Status_getStation_up();
int  Station_isAttackedByAliens_up(Station *s);
int  Status_inAlienOrbit_up();
void Level_updateAlienAttackers_call(Level *self);
int  Station_getIndex_up(Station *s);
int  Status_getCurrentCampaignMission_up();
float Status_getGammaRayDamagePerSecond_up(int a, int b);
int  Status_getShip_up();
int  Ship_getFirstEquipmentOfSort_up(int ship);
int  Item_getAttribute_up(int item);
int  Player_getGammaHP_up();
void Player_damageGamma_up(Player *p, float dmg);
int  PlayerEgo_getHUD_up(int ego);
void Hud_hudEvent_up(int hud, int code, int ego);
void PlayerEgo_getPosition_up(void *dst, int ego);
void ParticleSystemManager_update_up(int mgr, unsigned dt);
void LODManager_update_up(LODManager *m, unsigned dt);
}

// Level::update(long long time, bool param_2) [+ a third stack bool] — engine per-frame tick.
extern "C" void Level_update(Level *thisptr, long long /*time*/, unsigned dtArg,
                             int stackFlag)
{
    char *self = (char *)thisptr;
    unsigned dt = dtArg;

    // screen-flash fade.
    if (*(char *)(self + 0x158) != 0) {
        unsigned remaining = *(unsigned *)(self + 0x150) - dt;
        *(unsigned *)(self + 0x150) = remaining;
        if (0x7fffffff < remaining)
            *(char *)(self + 0x158) = 0;
        // colour scaled toward 0 by remaining/duration, clamped — handled by engine helper.
        float frac = (float)remaining / (float)*(int *)(self + 0x154);
        float *col = (float *)(self + 0x140);
        float lo = *g_up_clampLo, hi = *g_up_clampHi, z = *g_up_clampZ, w = *g_up_clampW;
        float scaled[4] = { col[0] * frac, col[1] * frac, col[2] * frac, col[3] * frac };
        float bounds[4] = { lo, hi, z, w };
        for (int k = 0; k < 4; k = k + 1)
            col[k] = bounds[k] > scaled[k] ? bounds[k] : scaled[k];
    }

    int **statusA = g_up_statusA;

    // orbit update: mission orbit if an active non-empty mission, else free orbit.
    bool didMission = false;
    if (Status_getMission_up() != 0) {
        Status_getMission_up();
        if (Mission_isEmpty_up() == 0) {
            Level_updateMissionOrbit_call(thisptr, dt);
            didMission = true;
        }
    }
    if (!didMission)
        Level_updateOrbit_call(thisptr, dt);

    Station *st = (Station *)Status_getStation_up();
    if (Station_isAttackedByAliens_up(st) != 0 || Status_inAlienOrbit_up() != 0)
        Level_updateAlienAttackers_call(thisptr);

    // tick player guns then enemy guns via their vtable update slot (+0x10).
    unsigned *guns = *(unsigned **)(self + 0xe4);
    if (guns != 0) {
        for (unsigned i = 0; i < *guns; i = i + 1) {
            int *g = *(int **)(guns[1] + i * 4);
            (*(void (**)(int *, unsigned))(*g + 0x10))(g, dt);
            guns = *(unsigned **)(self + 0xe4);
        }
    }
    guns = *(unsigned **)(self + 0xe8);
    if (guns != 0) {
        for (unsigned i = 0; i < *guns; i = i + 1) {
            int *g = *(int **)(guns[1] + i * 4);
            (*(void (**)(int *, unsigned))(*g + 0x10))(g, dt);
            guns = *(unsigned **)(self + 0xe8);
        }
    }

    // gamma-ray damage from supernova / hostile stations.
    int aPtr = *(int *)statusA;
    Station *st2 = (Station *)Status_getStation_up();
    int idx = Station_getIndex_up(st2);
    Status_getCurrentCampaignMission_up();
    float gamma = Status_getGammaRayDamagePerSecond_up(aPtr, idx);
    int ego = *(int *)(self + 0xf0);
    if (gamma > 0.0f && ego != 0) {
        int ship = Status_getShip_up();
        int eq = Ship_getFirstEquipmentOfSort_up(ship);
        float factor = gamma;
        if (eq != 0) {
            int attr = Item_getAttribute_up(eq);
            if (attr > 0)
                factor = (g_up_eqMax - (float)attr) / g_up_eqMax;
        }
        int hpBefore = Player_getGammaHP_up();
        Player_damageGamma_up((Player *)*(int *)(self + 0xf0), factor);
        if (hpBefore > 0xe && Player_getGammaHP_up() < 0xf) {
            int hud = PlayerEgo_getHUD_up(*(int *)(self + 0xf0));
            Hud_hudEvent_up(hud, 0x2c, *(int *)(self + 0xf0));
        }
        ego = *(int *)(self + 0xf0);
    }

    // tick the in-flight particle-system managers (skybox, engine trails, etc).
    if (ego != 0) {
        char dummy[16];
        if (*(int *)(self + 0x80) != 0) {
            PlayerEgo_getPosition_up(dummy, ego);
            *(int *)(self + 0xb4) = *(int *)dummy;
            ParticleSystemManager_update_up(*(int *)(self + 0x80), dt);
        }
        if (*(int *)(self + 0x74) != 0) ParticleSystemManager_update_up(*(int *)(self + 0x74), dt);
        if (*(int *)(self + 0x78) != 0) ParticleSystemManager_update_up(*(int *)(self + 0x78), dt);
        if (*(int *)(self + 0x88) != 0) {
            PlayerEgo_getPosition_up(dummy, ego);
            *(int *)(self + 0xb4) = *(int *)dummy;
            ParticleSystemManager_update_up(*(int *)(self + 0x88), dt);
        }
        if (*(int *)(self + 0x7c) != 0) ParticleSystemManager_update_up(*(int *)(self + 0x7c), dt);
        if (*(int *)(self + 0x84) != 0) ParticleSystemManager_update_up(*(int *)(self + 0x84), dt);
        if (*(int *)(self + 0x8c) != 0) ParticleSystemManager_update_up(*(int *)(self + 0x8c), dt);
        if (*(int *)(self + 0x98) != 0) ParticleSystemManager_update_up(*(int *)(self + 0x98), dt);
        if (*(int *)(self + 0x94) != 0) ParticleSystemManager_update_up(*(int *)(self + 0x94), dt);
        if (*(int *)(self + 0x9c) != 0) ParticleSystemManager_update_up(*(int *)(self + 0x9c), dt);
    }

    if (stackFlag == 0)
        LODManager_update_up(*(LODManager **)self, dt);
}
