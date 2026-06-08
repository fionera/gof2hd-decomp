#include "class.h"

struct Mission;
struct Player;
struct KIPlayer;
struct String;
struct Globals;

__attribute__((visibility("hidden"))) extern int     *g_cwm_stack;   // [DAT_000cc670]
__attribute__((visibility("hidden"))) extern int    **g_cwm_statusB; // [DAT_000cc674]
__attribute__((visibility("hidden"))) extern int    **g_cwm_seedSrc; // [DAT_000cc678]
__attribute__((visibility("hidden"))) extern Globals **g_cwm_globals; // [DAT_000cc67c]

extern "C" {
int  Status_inSupernovaSystem_cwm();
int  Status_getCurrentCampaignMission_cwm();
int  Status_getWingmen_cwm();
void *Level_opnew_cwm(unsigned size);
void ArrayKIPlayer_ctor_cwm(void *a);
void ArraySetLength_KIPlayer_cwm(unsigned n, void *a);
void AERandom_setSeed_cwm(int seed);
void AERandom_reset_cwm();
int  Globals_getRandomEnemyFighter_cwm(Globals *g, int race);
int  Level_createShip_cwm(Level *self, int a, int b, int shipDesc, int wp, int flagA, int flagB);
void KIPlayer_setWingman_cwm(KIPlayer *k, int flag, unsigned slot);
void Player_setAlwaysFriend_cwm(Player *p, int flag);
void Player_setHitpoints_cwm(int p);
void String_assign_cwm(String *dst, String *src);
int  Status_getMission_cwm();
int  Mission_getType_cwm();
void ArrayAdd_KIPlayer_cwm(KIPlayer *k, void *a);
// Positions wingman `i` relative to the player's geometry (right/forward offsets) and sets its
// heading. The original is SIMD vector math Ghidra could not lift cleanly.
void Level_cwm_placeWingman(Level *self, int *kiSlot, unsigned i);
}

// Level::createWingmen() — spawns the player's hired escort fighters in formation.
void Level::createWingmen()
{
    char *self = (char *)this;
    int **statusB = g_cwm_statusB;

    if (Status_inSupernovaSystem_cwm() != 0 ||
        Status_getCurrentCampaignMission_cwm() == 0x9e ||
        Status_getWingmen_cwm() == 0 ||
        *(int *)(self + 0xf0) == 0)
        return;

    void *arr = Level_opnew_cwm(0xc);
    ArrayKIPlayer_ctor_cwm(arr);
    unsigned *wm = (unsigned *)Status_getWingmen_cwm();
    ArraySetLength_KIPlayer_cwm(*wm, arr);

    unsigned n = *(unsigned *)arr;
    for (unsigned i = 0; i < n; i = i + 1) {
        int seed = **g_cwm_seedSrc;
        Status_getWingmen_cwm();
        AERandom_setSeed_cwm(seed);
        int fighter = Globals_getRandomEnemyFighter_cwm(*g_cwm_globals, *(int *)(*statusB + 0x2c));
        int ship = Level_createShip_cwm(this, 5, 0, fighter, 0, 1, 0);
        *(int *)(*(int *)((char *)arr + 4) + i * 4) = ship;

        int *slot = (int *)(*(int *)((char *)arr + 4) + i * 4);
        Level_cwm_placeWingman(this, slot, i);

        KIPlayer_setWingman_cwm(*(KIPlayer **)(*(int *)((char *)arr + 4) + i * 4), 1, i);
        Player_setAlwaysFriend_cwm(*(Player **)(*(int *)(*(int *)((char *)arr + 4) + i * 4) + 4), 1);
        Player_setHitpoints_cwm(*(int *)(*(int *)(*(int *)((char *)arr + 4) + i * 4) + 4));

        int wmList = Status_getWingmen_cwm();
        String_assign_cwm((String *)(*(int *)(*(int *)((char *)arr + 4) + i * 4) + 0x18),
                          *(String **)(*(int *)(wmList + 4) + i * 4));
        *(int *)(*(int *)(*(int *)((char *)arr + 4) + i * 4) + 0x28) = *(int *)(*statusB + 0x2c);

        Status_getMission_cwm();
        if (Mission_getType_cwm() == 0xc)
            *(char *)(*(int *)(*(int *)((char *)arr + 4) + i * 4) + 0x25) = 0;
    }

    if (*(int *)(self + 0xf8) == 0) {
        *(void **)(self + 0xf8) = arr;
    } else {
        for (unsigned i = 0; i < n; i = i + 1) {
            ArrayAdd_KIPlayer_cwm(*(KIPlayer **)(*(int *)((char *)arr + 4) + i * 4),
                                  *(void **)(self + 0xf8));
            n = *(unsigned *)arr;
        }
    }
    AERandom_reset_cwm();
}
