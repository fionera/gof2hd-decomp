#include "class.h"

struct Player;
struct KIPlayer;

__attribute__((visibility("hidden"))) extern int *g_ccm_stack;     // [DAT_000c4e94]
__attribute__((visibility("hidden"))) extern float g_ccm_pos0;     // DAT_000c5310 (case-0 spawn coord)

extern "C" {
int  Status_getCurrentCampaignMission_ccm();
void *Level_opnew_ccm(unsigned size);
void ArrayKIPlayer_ctor_ccm(void *a);
void ArraySetLength_KIPlayer_ccm(unsigned n, void *a);
int  Level_createShip_ccm(Level *self, int race, int b, int shipDesc, int wp, int hostile, int grp);
void KIPlayer_setToSleep_ccm(KIPlayer *k);
void Player_setAlwaysEnemy_ccm(Player *p);
void Player_setHitpoints_ccm(int p);
void PlayerFighter_setExhaustVisible_ccm(int pf);
// The story-battle builder for one campaign mission index: each case scripts a bespoke scene
// (boss ships, escorts, derelicts, scripted waypoints/objectives). This is the giant, SIMD-heavy
// per-mission switch that the decompiler could not lift, so it is provided by the engine helper.
void Level_ccm_buildCampaignScene(Level *self, int missionIndex);
}

// Level::createCampaignMission() — constructs the scripted actors/objectives for the player's
// current story (campaign) mission.
void Level::createCampaignMission()
{
    char *self = (char *)this;
    int idx = Status_getCurrentCampaignMission_ccm();

    if (idx == 0) {
        // mission 0 — the tutorial ambush: three sleeping enemy fighters at a fixed point.
        void *arr = Level_opnew_ccm(0xc);
        ArrayKIPlayer_ctor_ccm(arr);
        *(void **)(self + 0xf8) = arr;
        ArraySetLength_KIPlayer_ccm(3, arr);
        float c = g_ccm_pos0;
        for (unsigned i = 0; i < **(unsigned **)(self + 0xf8); i = i + 1) {
            int type = (i == 1) ? 0x17 : 2;
            int ship = Level_createShip_ccm(this, 8, 0, type, 0, 1, 0);
            *(int *)(*(int *)(*(int *)(self + 0xf8) + 4) + i * 4) = ship;
            KIPlayer_setToSleep_ccm(*(KIPlayer **)(*(int *)(*(int *)(self + 0xf8) + 4) + i * 4));
            Player_setAlwaysEnemy_ccm(
                *(Player **)(*(int *)(*(int *)(*(int *)(self + 0xf8) + 4) + i * 4) + 4));
            int *kp = *(int **)(*(int *)(*(int *)(self + 0xf8) + 4) + i * 4);
            (*(void (**)(int *, float, float, float))(*kp + 0x48))(kp, c, c, c);
            int base = *(int *)(*(int *)(self + 0xf8) + 4);
            int e = *(int *)(base + i * 4);
            *(int *)(e + 0x50) = 0;
            *(char *)(e + 0x4c) = 0;
            Player_setHitpoints_ccm(*(int *)(*(int *)(base + i * 4) + 4));
            if (i < 3)
                PlayerFighter_setExhaustVisible_ccm(
                    *(int *)(*(int *)(self + 0xf8) + 4) + i * 4);
        }
        Player_setHitpoints_ccm(**(int **)(self + 0xf0));
        return;
    }

    // all other campaign missions are scripted by the engine helper.
    Level_ccm_buildCampaignScene(this, idx);
}
