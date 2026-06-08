#include "class.h"

struct Hud;
struct Achievements;
struct Radar { static int hasScanner(); };

// PC-relative singleton holders.
__attribute__((visibility("hidden"))) extern Status      **g_ed_status; // [DAT_000d441c]
__attribute__((visibility("hidden"))) extern Achievements **g_ed_achA;  // [DAT_000d4420]
__attribute__((visibility("hidden"))) extern Achievements **g_ed_achB;  // [DAT_000d4424]
__attribute__((visibility("hidden"))) extern float          g_ed_100;   // DAT_000d4418 == 100.0f

extern "C" {
void Status_incKills(Status *s);
int  Achievements_hasMedal(Achievements *a, int id, int tier);
int  Achievements_getValue(Achievements *a, int id, int tier);
int  PlayerEgo_getHUD(int egoPtr);
int  PlayerEgo_emergencySystemActive(int egoPtr);
void Hud_hudEventMedal(int hud, int id, int value);
int  aeabi_idivmod_ed(int a, int b);
}

// Level::enemyDied — the header declares it nullary, but the target reads a discriminator in
// r2 (whether the kill counts toward the player), so we express it as a C callback that the
// engine invokes with (Level* self, _, bool wasFriendlyFire).
extern "C" void Level_enemyDied(Level *thisptr, int r1, bool r2arg)
{
    char *self = (char *)thisptr;
    int r2 = (int)r2arg; // the recovered "in_r2" discriminator
    (void)r1;

    *(int *)(self + 0x118) = *(int *)(self + 0x118) - 1;  // enemiesLeft--
    *(int *)(self + 0x12c) = *(int *)(self + 0x12c) + 1;  // kills++

    if (r2 != 0) {
        *(int *)(self + 0x20) = *(int *)(self + 0x20) + 1;
        return;
    }

    Status **statusHolder = g_ed_status;
    Status_incKills(*statusHolder);
    *(int *)(self + 0x24) = *(int *)(self + 0x24) + 1;
    if (*(int *)(self + 0xf0) == 0)
        return;

    if (Radar::hasScanner() == 0) {
        Achievements **achA = g_ed_achA;
        if (Achievements_hasMedal(*achA, 0x28, 1) == 0) {
            int st = *(int *)statusHolder;
            int v = *(int *)(st + 0x11c);
            if (*(char *)(st + 0x120) == 0) {
                v = v + 1;
                *(int *)(st + 0x11c) = v;
            }
            int goal = Achievements_getValue(*achA, 0x28, 1);
            int prog = (int)(((float)v / (float)goal) * g_ed_100);
            if (aeabi_idivmod_ed(prog, 10) == 0) {
                int hud = PlayerEgo_getHUD(*(int *)(self + 0xf0));
                int cur = *(int *)(*(int *)statusHolder + 0x11c);
                int g2 = Achievements_getValue(*achA, 0x28, 1);
                Hud_hudEventMedal(hud, 0x28, (int)(((float)cur / (float)g2) * g_ed_100));
            }
            int cur2 = *(int *)(*(int *)statusHolder + 0x11c);
            if (Achievements_getValue(*achA, 0x28, 1) <= cur2)
                *(char *)(*(int *)statusHolder + 0x120) = 1;
        }
    }

    if (*(int *)(self + 0xf0) != 0 &&
        PlayerEgo_emergencySystemActive(*(int *)(self + 0xf0)) != 0) {
        Achievements **achB = g_ed_achB;
        if (Achievements_hasMedal(*achB, 0x2b, 1) == 0) {
            int st = *(int *)statusHolder;
            int v = *(int *)(st + 0x13c) + 1;
            *(int *)(st + 0x13c) = v;
            int goal = Achievements_getValue(*achB, 0x2b, 1);
            if (0 < (int)((float)v / (float)goal)) {
                int hud = PlayerEgo_getHUD(*(int *)(self + 0xf0));
                int cur = *(int *)(*(int *)statusHolder + 0x13c);
                int g2 = Achievements_getValue(*achB, 0x2b, 1);
                Hud_hudEventMedal(hud, 0x2b, (int)(((float)cur / (float)g2) * g_ed_100));
            }
            int cur2 = *(int *)(*(int *)statusHolder + 0x13c);
            if (Achievements_getValue(*achB, 0x2b, 1) <= cur2)
                *(char *)(*(int *)statusHolder + 0x140) = 1;
        }
    }
}
