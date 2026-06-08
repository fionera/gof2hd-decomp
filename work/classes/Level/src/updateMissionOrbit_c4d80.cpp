#include "class.h"

struct Mission;

// PC-relative cookie slot.
__attribute__((visibility("hidden"))) extern int *g_umo_stack; // [DAT_000d50dc]

extern "C" {
int  Status_getMission_umo();
int  Mission_isEmpty_umo();
int  Mission_getType_umo();
int  KIPlayer_isDead_umo();
int  Player_isActive_umo();
// Revives the KIPlayer then teleports it to a freshly randomized offset from the player's
// position (the original is SIMD float math Ghidra could not lift). Two profiles are used:
//   profile 0 — generic far-field wave, profile 1 — boss-fight (type 0xb7/0xf) variant.
void Level_umo_spawnAt(Level *self, int *kiPlayer, int profile);
}

// Level::updateMissionOrbit(int dt) — drives mission-specific enemy respawn timing.
extern "C" void Level_updateMissionOrbit(Level *thisptr, int dt)
{
    char *self = (char *)thisptr;

    // --- phase A: periodic far wave (only when a "boss present" flag at 0x288 is set) ---
    if (*(char *)(self + 0x288) != 0) {
        Status_getMission_umo();
        if (Mission_isEmpty_umo() == 0) {
            int t = *(int *)(self + 0x174);
            *(int *)(self + 0x174) = t + dt;
            if (0x57e4 < t + dt) {
                *(int *)(self + 0x174) = 0;
                int aliveCore = 0;
                for (int j = 0; j != 4; j = j + 1)
                    aliveCore = aliveCore + (KIPlayer_isDead_umo() ^ 1);
                if (aliveCore != 0) {
                    unsigned *en = *(unsigned **)(self + 0xf8);
                    for (unsigned i = 4; i < *en; i = i + 1) {
                        int *ki = *(int **)(en[1] + i * 4);
                        if (KIPlayer_isDead_umo() != 0 && Player_isActive_umo() == 0)
                            Level_umo_spawnAt(thisptr, ki, 0);
                    }
                }
            }
        }
    }

    // --- phase B: type-0xb7 boss escort wave ---
    if (Status_getMission_umo() != 0) {
        Status_getMission_umo();
        if (Mission_getType_umo() == 0xb7) {
            int t = *(int *)(self + 0x174);
            *(int *)(self + 0x174) = t + dt;
            if (0x1d4c < t + dt) {
                *(int *)(self + 0x174) = 0;
                unsigned *en = *(unsigned **)(self + 0xf8);
                for (unsigned i = 0; i < *en; i = i + 1) {
                    int *ki = *(int **)(en[1] + i * 4);
                    if (KIPlayer_isDead_umo() != 0 && Player_isActive_umo() == 0 &&
                        ki[0x1f] != 0x33) {
                        Level_umo_spawnAt(thisptr, ki, 1);
                        ki[0x14] = 0;
                    }
                }
            }
        }
    }

    // --- phase C: type-0xf wave (skips the last enemy, the leader) ---
    Status_getMission_umo();
    if (Mission_isEmpty_umo() == 0) {
        Status_getMission_umo();
        if (Mission_getType_umo() == 0xf) {
            int t = *(int *)(self + 0x174);
            *(int *)(self + 0x174) = t + dt;
            if (50000 < t + dt) {
                *(int *)(self + 0x174) = 0;
                unsigned *en = *(unsigned **)(self + 0xf8);
                unsigned count = *en;
                // require at least one of the non-leader enemies to still be alive.
                bool anyAlive = false;
                for (unsigned i = 0; i + 1 < count; i = i + 1) {
                    if (KIPlayer_isDead_umo() == 0) { anyAlive = true; break; }
                }
                if (anyAlive) {
                    for (unsigned i = 0; i + 1 < count; i = i + 1) {
                        int *ki = *(int **)(en[1] + i * 4);
                        if (KIPlayer_isDead_umo() != 0 && Player_isActive_umo() == 0)
                            Level_umo_spawnAt(thisptr, ki, 0);
                        en = *(unsigned **)(self + 0xf8);
                        count = *en;
                    }
                }
            }
        }
    }
}
