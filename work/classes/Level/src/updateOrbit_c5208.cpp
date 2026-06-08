#include "class.h"

struct SolarSystem;
struct KIPlayer;

// PC-relative cookie slot.
__attribute__((visibility("hidden"))) extern int *g_uo_stack; // [DAT_000d5588]

extern "C" {
int  Status_getSystem_uo();
int  SolarSystem_getRace_uo();
int  SolarSystem_getSecurityLevel_uo(SolarSystem *s);
void Level_alarmAllFriends_uo(Level *self, int race, int message);
void Level_createRadioMessage_uo(Level *self, int type);
int  KIPlayer_isJumper_uo(KIPlayer *k);
int  KIPlayer_isDead_uo();
int  KIPlayer_isWingMan_uo();
int  Player_isActive_uo();
int  Status_inAlienOrbit_uo();
// Revives + repositions an enemy to a randomized far offset from the player (corrupted SIMD math).
void Level_uo_spawnFar(Level *self, int *kiPlayer);
}

// Level::updateOrbit(int dt) — non-mission ambient traffic / reinforcement spawner.
extern "C" void Level_updateOrbit(Level *thisptr, int dt)
{
    char *self = (char *)thisptr;

    int t178 = *(int *)(self + 0x178) + dt;
    *(int *)(self + 0x174) = *(int *)(self + 0x174) + dt;
    *(int *)(self + 0x178) = t178;

    // delayed "friends turned hostile" alarm.
    if (*(char *)(self + 0x18a) != 0) {
        if (Status_getSystem_uo() != 0 && *(int *)(*(int *)(self + 0xf0) + 0x18) != 0) {
            Status_getSystem_uo();
            int race = SolarSystem_getRace_uo();
            Level_alarmAllFriends_uo(thisptr, race, 0);
            Status_getSystem_uo();
            SolarSystem_getRace_uo();
            Level_createRadioMessage_uo(thisptr, 2);
            *(char *)(self + 0x18a) = 0;
        }
        t178 = *(int *)(self + 0x178);
    }

    // jumper reinforcement every 10s.
    if (10000 < t178) {
        unsigned *en = *(unsigned **)(self + 0xf8);
        *(int *)(self + 0x178) = 0;
        if (en != 0) {
            for (unsigned i = 0; i < *en; i = i + 1) {
                KIPlayer *ki = *(KIPlayer **)(en[1] + i * 4);
                if (KIPlayer_isJumper_uo(ki) != 0 && KIPlayer_isDead_uo() != 0 &&
                    Player_isActive_uo() == 0 && *(char *)((char *)ki + 0x42) == 0) {
                    (*(void (**)(KIPlayer *))(*(int *)ki + 0x18))(ki);
                    (*(void (**)(KIPlayer *, int, int, int))(*(int *)ki + 0x48))(ki, 0, 0, 0);
                    break;
                }
                en = *(unsigned **)(self + 0xf8);
            }
        }
    }

    // major reinforcement sweep every 45s.
    if (45000 < *(int *)(self + 0x174)) {
        int hostileAlive = 0;
        unsigned *en = *(unsigned **)(self + 0xf8);
        *(int *)(self + 0x174) = 0;
        if (0 < *(int *)(self + 0x16c)) {
            if (en == 0)
                return;
            for (unsigned i = 0; i < *en; i = i + 1) {
                if ((*en - (unsigned)*(int *)(self + 0x16c)) <= i &&
                    KIPlayer_isWingMan_uo() == 0 && KIPlayer_isDead_uo() != 0) {
                    hostileAlive = hostileAlive + (Player_isActive_uo() ^ 1);
                }
                en = *(unsigned **)(self + 0xf8);
            }
        }
        if (en != 0) {
            int spawned = 0;
            for (unsigned i = 0; i < *en; i = i + 1) {
                int *ki = *(int **)(en[1] + i * 4);
                // re-activate friendly slots that died.
                if (0 < *(int *)(self + 0x160) && (int)i < *(int *)(self + 0x160) &&
                    KIPlayer_isDead_uo() != 0 && Player_isActive_uo() == 0 &&
                    *(char *)((char *)ki + 0x42) == 0) {
                    (*(void (**)(int *))(*ki + 0x18))(ki);
                    (*(void (**)(int *, int, int, int))(*ki + 0x48))(ki, 0, 0, 0);
                }
                // spawn enemy reinforcements subject to security-level caps.
                if (1 < hostileAlive && *(int *)(self + 0x184) < 2 &&
                    0 < *(int *)(self + 0x16c) &&
                    (unsigned)(**(int **)(self + 0xf8) - *(int *)(self + 0x16c)) <= i) {
                    int race = ki[10];
                    bool secZero = false, secOne = false;
                    if (Status_inAlienOrbit_uo() == 0) {
                        SolarSystem *ss = (SolarSystem *)Status_getSystem_uo();
                        secZero = SolarSystem_getSecurityLevel_uo(ss) == 0;
                    }
                    if (Status_inAlienOrbit_uo() == 0) {
                        SolarSystem *ss = (SolarSystem *)Status_getSystem_uo();
                        secOne = SolarSystem_getSecurityLevel_uo(ss) == 1;
                    }
                    if (KIPlayer_isDead_uo() != 0 && Player_isActive_uo() == 0 &&
                        *(char *)((char *)ki + 0x42) == 0) {
                        bool doSpawn;
                        if (race != 9 && !secZero) {
                            doSpawn = secOne && *(int *)(self + 0x17c) <= 2;
                        } else {
                            doSpawn = true;
                        }
                        if (doSpawn) {
                            *(int *)(self + 0x17c) = *(int *)(self + 0x17c) + 1;
                            Level_uo_spawnFar(thisptr, ki);
                            spawned = 1;
                        }
                    }
                }
                en = *(unsigned **)(self + 0xf8);
            }
            if (spawned & 1)
                *(int *)(self + 0x184) = *(int *)(self + 0x184) + 1;
        }
    }
}
