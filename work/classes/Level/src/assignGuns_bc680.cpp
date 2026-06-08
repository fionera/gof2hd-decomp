#include "class.h"

struct Wanted;
struct Mission;
struct Gun;
struct ObjectGun;
struct RocketGun;
struct PlayerTurret;
struct KIPlayer;
struct Player;

__attribute__((visibility("hidden"))) extern int    *g_ag_stack;     // [DAT_000cca28]
__attribute__((visibility("hidden"))) extern Status **g_ag_status;   // [DAT_000cca38]
__attribute__((visibility("hidden"))) extern int    *g_ag_diffRec;   // [DAT_000cca44] difficulty record
__attribute__((visibility("hidden"))) extern int   **g_ag_shipTbl;   // [DAT_000cce38]
__attribute__((visibility("hidden"))) extern int   **g_ag_itemTblA;  // [DAT_000cd1f0]
__attribute__((visibility("hidden"))) extern int    *g_ag_weaponDmg; // [DAT_000cd1f4]
__attribute__((visibility("hidden"))) extern int   **g_ag_statusB;   // [DAT_000cce40]
__attribute__((visibility("hidden"))) extern int   **g_ag_alienCnt;  // [DAT_000cce44]
__attribute__((visibility("hidden"))) extern int   **g_ag_snd;       // [DAT_000cd1f8]
__attribute__((visibility("hidden"))) extern int   **g_ag_itemTblB;  // [DAT_000cd1fc]
__attribute__((visibility("hidden"))) extern int   **g_ag_snd2;      // [DAT_000cd200]
__attribute__((visibility("hidden"))) extern float   g_ag_perLevel;  // DAT_000cca3c

extern "C" {
void  ArrayReleaseClasses_AGun_ag(void *a);
void *ArrayAGun_dtor_ag(void *a);
void  operator_delete_ag(void *p);
void *Level_opnew_ag(unsigned size);
void  ArrayAGun_ctor_ag(void *a);
void  ArraySetLength_AGun_ag(unsigned n, void *a);
void  ArrayAdd_AGun_ag(ObjectGun *o, void *a);
int   Status_getLevel_ag();
int   Status_getCurrentCampaignMission_ag();
int   Status_getMission_ag();
int   Status_gameWon_ag();
int   Status_getWantedInCurrentOrbit_ag(Status *s);
int   Mission_getType_ag();
int   Mission_isCampaignMission_ag(Mission *m);
int   KIPlayer_isWingMan_ag();
int   KIPlayer_isEnemy_ag(KIPlayer *k);
int   KIPlayer_getType_ag(KIPlayer *k);
void  KIPlayer_addGun_ag(Gun *target, int gun);
int   Player_isAlwaysFriend_ag();
int   Player_isAlwaysEnemy_ag();
void  Player_setPlayShootSound_ag(Player *p, int flag, int v);
int   Player_getMaxHitpoints_ag();
void  Player_setMaxHitpoints_ag(Player *p, int hp);
int   PlayerTurret_getHost_ag(PlayerTurret *t);
int   Wanted_getWeapon_ag(Wanted *w);
int   Item_getAttribute_ag(int item);
void  Gun_ctor_ag(Gun *g, int a, int b, int c, int d, int e, int f, int g7, int h, int i,
                  int j, int k, int l, int m);
void  Gun_setFriendGun_ag(Gun *g, int flag);
void  Gun_setLevel_ag(Gun *g, Level *self);
void  Gun_setIndex_ag(Gun *g, int idx);
void  RocketGun_ctor_ag(RocketGun *r, int a, Gun *g, int res, int b, int c, int d, int e,
                        Level *self);
void  ObjectGun_ctor_ag(ObjectGun *o, int a, Gun *g, int res, int d, Level *self);
void  Globals_addSoundResourceToList_ag(int snd);
}

// Level::assignGuns() — equips every active ship in the level with weapons scaled to the player
// level and difficulty, with a large set of campaign/wanted/ship-type special cases.
void Level::assignGuns()
{
    char *self = (char *)this;

    if (*(void **)(self + 0xe8) != 0) {
        ArrayReleaseClasses_AGun_ag(*(void **)(self + 0xe8));
        if (*(void **)(self + 0xe8) != 0)
            operator_delete_ag(ArrayAGun_dtor_ag(*(void **)(self + 0xe8)));
        *(int *)(self + 0xe8) = 0;
    }
    Status **status = g_ag_status;
    *(int *)(self + 0xe8) = 0;

    // base weapon "power" from player level (clamped to 20).
    float lvlPower = (float)(Status_getLevel_ag() - 2) * g_ag_perLevel;
    if (lvlPower >= 20.0f) lvlPower = 20.0f;
    if (lvlPower < 0.0f) lvlPower = (float)(Status_getLevel_ag() - 2) * g_ag_perLevel;

    float diffScale = *(float *)(*(int *)g_ag_diffRec + 0x2c);
    int camp = Status_getCurrentCampaignMission_ag();
    int basePower = (int)(lvlPower + lvlPower * (diffScale - 0.5f));
    Wanted *wanted = (Wanted *)Status_getWantedInCurrentOrbit_ag(*status);
    unsigned *list = *(unsigned **)(self + 0xf8);
    if (0x15 < basePower) basePower = 0x16;
    if (list == 0)
        return;

    // count gun slots needed.
    unsigned slots = 0;
    for (unsigned i = 0; i < *list; i = i + 1) {
        int e = *(int *)(list[1] + i * 4);
        if (e != 0 && *(char *)(e + 0x25) != 0) {
            int add = (KIPlayer_isWingMan_ag() != 0) ? 2 : 1;
            list = *(unsigned **)(self + 0xf8);
            slots = slots + add;
        }
    }

    void *guns = Level_opnew_ag(0xc);
    ArrayAGun_ctor_ag(guns);
    *(void **)(self + 0xe8) = guns;
    ArraySetLength_AGun_ag(slots, guns);

    int baseDmg = (basePower == 0) ? 3 : (basePower + 2);
    if (camp == 4) baseDmg = 1;

    int outIdx = 0;
    for (unsigned i = 0; i < **(unsigned **)(self + 0xf8); i = i + 1) {
        int e = *(int *)((*(unsigned **)(self + 0xf8))[1] + i * 4);
        if (e == 0)
            continue;
        if (*(char *)(e + 0x25) == 0)
            goto wingmanExtra;

        {
            if (*(int *)(self + 0xc0) == 2)
                Player_setPlayShootSound_ag(*(Player **)(e + 4), 0, 2);

            int color = 0x41800000;
            int dmg;
            Status_getMission_ag();
            if (Mission_getType_ag() == 6 && Player_isAlwaysFriend_ag() == 0) {
                color = 0x41e00000;
                dmg = Status_getLevel_ag() + baseDmg;
            } else {
                Status_getMission_ag();
                dmg = baseDmg;
                if (Mission_getType_ag() == 0xc && Player_isAlwaysFriend_ag() != 0) {
                    color = 0x41e00000;
                    dmg = Status_getLevel_ag() + baseDmg;
                }
            }

            // alien ships ramp the damage harder.
            if (KIPlayer_isWingMan_ag() == 0 && Player_isAlwaysFriend_ag() == 0 &&
                *(int *)(*(int *)(*(int *)(self + 0xf8) + 4) + i * 4) &&
                *(int *)(*(int *)(*(int *)(*(int *)(self + 0xf8) + 4) + i * 4) + 0x28) == 9) {
                if (camp != 0x10) dmg = (int)((float)dmg * 1.0f);
                else dmg = dmg + dmg;
            } else {
                int hard = ((0x8fU >> ((unsigned)(camp - 0x31) & 0xff) & 1) != 0 &&
                            (unsigned)(camp - 0x31) <= 7) ? 5 : dmg;
                dmg = hard;
            }

            if (camp == 0x46 && KIPlayer_isWingMan_ag() == 0)
                dmg = (int)((float)dmg * 2.5f);

            if (Status_getMission_ag() != 0) {
                Status_getMission_ag();
                if (Mission_getType_ag() == 0xb7) dmg = 1;
            }

            Gun *gun = (Gun *)Level_opnew_ag(0x114);
            int won = Status_gameWon_ag();
            int rampMis = (won != 0) ? 0x2d : Status_getCurrentCampaignMission_ag();
            Gun_ctor_ag(gun, 0, dmg, 4, -1, 3000, rampMis * -2 + 600, color, 0, 0, 0, 0, 0, 0);
            Gun_setFriendGun_ag(gun, 1);
            Gun_setLevel_ag(gun, this);
            Gun_setIndex_ag(gun, 0);
            *(int *)((char *)gun + 0x5c) = 0;

            int res;
            switch (*(int *)(*(int *)(*(int *)(self + 0xf8) + 4) + i * 4) + 0x28
                        ? *(int *)(*(int *)(*(int *)(*(int *)(self + 0xf8) + 4) + i * 4) + 0x28)
                        : 0) {
            case 0:  *(int *)((char *)gun + 0x5c) = 0; Gun_setIndex_ag(gun, 0);    res = 0x1a62; break;
            case 1:  Gun_setIndex_ag(gun, 3);    res = 0x1a68; break;
            case 2:  *(int *)((char *)gun + 0x5c) = 0; Gun_setIndex_ag(gun, 7);    res = 0x1a6c; break;
            case 3:  Gun_setIndex_ag(gun, 0x19); res = 0x1a92; break;
            case 9:  Gun_setIndex_ag(gun, 5);    res = 0x1a6a; break;
            case 10: Gun_setIndex_ag(gun, 0xe5); res = 0x4a93;
                     *(int *)((char *)gun + 0x60) = (int)((float)*(int *)((char *)gun + 0x60) * 1.0f);
                     break;
            default: *(int *)((char *)gun + 0x5c) = 1; Gun_setIndex_ag(gun, 0x13); res = 0x1a8b; break;
            }

            int camp2 = Status_getCurrentCampaignMission_ag();
            PlayerTurret *turret = *(PlayerTurret **)(*(int *)(*(int *)(self + 0xf8) + 4) + i * 4);
            if (*(char *)((char *)turret + 0x3e) != 0) {
                int host = PlayerTurret_getHost_ag(turret);
                if (host != 0 && (*(int *)(host + 0x7c) == 0x2d || *(int *)(host + 0x7c) == 0x33)) {
                    *(int *)((char *)gun + 0x5c) = 2; Gun_setIndex_ag(gun, 0x16); res = 0x1a8e;
                } else {
                    KIPlayer *k = *(KIPlayer **)(*(int *)(*(int *)(self + 0xf8) + 4) + i * 4);
                    if (*(char *)((char *)k + 0x3f) == 0) {
                        *(int *)((char *)gun + 0x5c) = 1;
                        if (*(int *)((char *)k + 0x28) == 1) { Gun_setIndex_ag(gun, 0xf);  res = 0x1a87; }
                        else                                 { Gun_setIndex_ag(gun, 0x14); res = 0x1a8c; }
                    } else {
                        int kt = KIPlayer_getType_ag(k);
                        if (kt == 0x49c1)      { *(int *)((char *)gun + 0x5c) = 1; Gun_setIndex_ag(gun, 0x14); res = 0x1a8d; }
                        else if (kt == 0x49c0) { *(int *)((char *)gun + 0x5c) = 0; Gun_setIndex_ag(gun, 2);    res = 0x1a64; }
                        else                   { *(int *)((char *)gun + 0x5c) = 1; Gun_setIndex_ag(gun, 0xe);  res = 0x1a86; }
                        *(char *)((char *)gun + 0xa8) = 1;
                        // base stats from the ship-stat table (corrupted SIMD in original).
                        if (camp2 == 0x9e && kt == 0x49c2 && Player_isAlwaysEnemy_ag() != 0) {
                            Player *pp = *(Player **)(*(int *)(*(int *)(*(int *)(self + 0xf8) + 4) + i * 4) + 4);
                            int mhp = Player_getMaxHitpoints_ag();
                            Player_setMaxHitpoints_ag(pp, (int)((float)mhp * 5.0f));
                        }
                    }
                }
            }

            // wanted/ship special weapon overrides.
            int kt2 = Status_getCurrentCampaignMission_ag();
            int host2 = *(int *)(*(int *)(*(int *)(self + 0xf8) + 4) + i * 4);
            if (Status_getMission_ag() != 0) {
                Mission *mm = (Mission *)Status_getMission_ag();
                if (Mission_isCampaignMission_ag(mm) != 0) {
                    if (**g_ag_statusB == Status_getCurrentCampaignMission_ag() &&
                        2 < **g_ag_alienCnt &&
                        KIPlayer_isEnemy_ag(*(KIPlayer **)(*(int *)(*(int *)(self + 0xf8) + 4) + i * 4)) != 0)
                        *(int *)((char *)gun + 0x60) = (int)((float)*(int *)((char *)gun + 0x60) * 1.0f);
                }
            }
            if (wanted != 0 && *(char *)(host2 + 0x42) != 0) {
                int w = Wanted_getWeapon_ag(wanted);
                Gun_setIndex_ag(gun, w);
                int attr = Item_getAttribute_ag(*(int *)(*(int *)(*g_ag_itemTblA + 4) + w * 4));
                res = ((int *)g_ag_weaponDmg)[w];
                *(int *)((char *)gun + 0x5c) = attr;
                *(int *)((char *)gun + 0x60) = *(int *)((char *)gun + 0x60) << 2;
            }

            // wrap the Gun in a Rocket/Object gun and store it.
            int sc = *(int *)((char *)gun + 0x5c);
            if (sc == 0x28 || sc == 5) {
                RocketGun *r = (RocketGun *)Level_opnew_ag(0xe8);
                RocketGun_ctor_ag(r, *(int *)((char *)gun + 0x58), gun, res, 0, 0, sc,
                                  sc == 5 ? 1 : 0, this);
                *(RocketGun **)(*(int *)(*(int *)(self + 0xe8) + 4) + outIdx * 4) = r;
                *(int *)((char *)gun + 0x50) = 0x41000000;
                *(int *)((char *)gun + 0x44) = 10000;
                *(int *)((char *)gun + 0x48) = 3000;
                *(int *)((char *)gun + 0x60) = *(int *)((char *)gun + 0x60) << 2;
            } else {
                ObjectGun *o = (ObjectGun *)Level_opnew_ag(0xb0);
                ObjectGun_ctor_ag(o, 0, gun, res, 0x2711, this);
                *(ObjectGun **)(*(int *)(*(int *)(self + 0xe8) + 4) + outIdx * 4) = o;
            }
            KIPlayer_addGun_ag(*(Gun **)(*(int *)(*(int *)(self + 0xf8) + 4) + i * 4), (int)gun);
            Globals_addSoundResourceToList_ag(**g_ag_snd);
            outIdx = outIdx + 1;
            (void)kt2;
        }

wingmanExtra:
        if (KIPlayer_isWingMan_ag() != 0 &&
            *(char *)(*(int *)(*(int *)(self + 0xf8) + 4) + i * 4) + 0x25 != 0) {
            Gun *gun = (Gun *)Level_opnew_ag(0x114);
            Gun_ctor_ag(gun, 0x12, 0, 4, -1, 3000, 400, 0x41800000, 0, 0, 0, 0, 0, 0);
            Gun_setFriendGun_ag(gun, 1);
            Gun_setLevel_ag(gun, this);
            *(int *)((char *)gun + 0x58) = 0x12;
            *(int *)((char *)gun + 0x5c) = 1;
            ObjectGun *o = (ObjectGun *)Level_opnew_ag(0xb0);
            ObjectGun_ctor_ag(o, 0x12, gun, 0x1a8a, 0x2711, this);
            *(ObjectGun **)(*(int *)(*(int *)(self + 0xe8) + 4) + outIdx * 4) = o;
            Gun_setIndex_ag(gun, 0x12);
            int attr = Item_getAttribute_ag(*(int *)(*(int *)(*g_ag_itemTblB + 4) + 0x48));
            *(int *)((char *)gun + 100) = attr;
            KIPlayer_addGun_ag(*(Gun **)(*(int *)(*(int *)(self + 0xf8) + 4) + i * 4), (int)gun);
            Globals_addSoundResourceToList_ag(**g_ag_snd2);
            outIdx = outIdx + 1;
        }
    }
}
