#include "class.h"

struct Mission;
struct Station;

// PC-relative singletons / data slots.
__attribute__((visibility("hidden"))) extern int   *g_uaa_stack;     // [DAT_000d5f40]
__attribute__((visibility("hidden"))) extern int  **g_uaa_rngHolder; // [DAT_000d5f4c]

extern "C" {
int  Status_getMission_uaa();
int  Mission_isCampaignMission_uaa(Mission *m);
int  Status_getCurrentCampaignMission_uaa();
int  KIPlayer_isDead_uaa();
int  Player_isActive_uaa();
int  Status_inAlienOrbit_uaa();
int  Status_getStation_uaa();
int  Station_isAttackedByAliens_uaa(Station *s);
// Computes a randomized spawn offset for an alien attacker relative to either the player or the
// station origin, then commits it to the KIPlayer at +0x48 (vtable setPosition). The original is
// pure SIMD float math that Ghidra could not lift cleanly; delegate to the engine helper.
void Level_uaa_placeAlien(int self, int *kiPlayer, int alienInOrbit);
}

// Level::updateAlienAttackers(int dt) — periodically (re)spawns the alien attacker wave.
extern "C" void Level_updateAlienAttackers(Level *thisptr, int dt)
{
    char *self = (char *)thisptr;
    *(int *)(self + 0x170) = *(int *)(self + 0x170) + dt;

    Mission *m = (Mission *)Status_getMission_uaa();
    bool blocked = (m != 0) && Mission_isCampaignMission_uaa(m) != 0 &&
                   (Status_getCurrentCampaignMission_uaa() == 0x28 ||
                    Status_getCurrentCampaignMission_uaa() == 0x93 ||
                    Status_getCurrentCampaignMission_uaa() == 0x9a);
    if (blocked)
        return;

    int elapsed = *(int *)(self + 0x170);
    int period = (Status_getCurrentCampaignMission_uaa() == 0x29) ? 10000 : 45000;
    if (elapsed <= period)
        return;

    unsigned *enemies = *(unsigned **)(self + 0xf8);
    *(int *)(self + 0x170) = 0;
    if (enemies == 0)
        return;

    for (unsigned i = 0; i < *enemies; i = i + 1) {
        int *ki = *(int **)(enemies[1] + i * 4);
        // race 9 == alien; only respawn ones that are dead and inactive.
        if (ki[10] == 9 && KIPlayer_isDead_uaa() != 0 && Player_isActive_uaa() == 0) {
            (*(void (**)(int *))(*ki + 0x18))(ki);   // revive
            int inOrbit = (Status_inAlienOrbit_uaa() != 0);
            if (!inOrbit) {
                Station *st = (Station *)Status_getStation_uaa();
                if (Station_isAttackedByAliens_uaa(st) == 0)
                    inOrbit = 1; // fall back to player-relative placement
            }
            Level_uaa_placeAlien((int)self, ki, inOrbit);
        }
        enemies = *(unsigned **)(self + 0xf8);
    }
}
