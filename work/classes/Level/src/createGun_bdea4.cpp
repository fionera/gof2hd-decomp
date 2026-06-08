#include "class.h"

struct Gun;
struct ObjectGun;

// Index->resource lookup tables (each DAT_ is a base into a const id array).
__attribute__((visibility("hidden"))) extern int *g_cg_beamTable;   // [DAT_000ce2b4]
__attribute__((visibility("hidden"))) extern int  g_cg_rocketFx;    // DAT_000ce2b8
__attribute__((visibility("hidden"))) extern int  g_cg_objFx;       // DAT_000ce2bc
__attribute__((visibility("hidden"))) extern int *g_cg_objTable;    // [DAT_000ce2d4]
__attribute__((visibility("hidden"))) extern int *g_cg_rocketTable; // [DAT_000ce2c8]
__attribute__((visibility("hidden"))) extern int **g_cg_rocketSnd;  // [DAT_000ce2cc]
__attribute__((visibility("hidden"))) extern int **g_cg_itemTableA; // [DAT_000ce2c0]
__attribute__((visibility("hidden"))) extern int *g_cg_bombTable;   // [DAT_000ce2c4]
__attribute__((visibility("hidden"))) extern int *g_cg_bombSnd;     // [DAT_000ce64c]
__attribute__((visibility("hidden"))) extern int  g_cg_mineFx;      // DAT_000ce644
__attribute__((visibility("hidden"))) extern int *g_cg_objTable8;   // [DAT_000ce648]
__attribute__((visibility("hidden"))) extern int *g_cg_mineTable;   // [DAT_000ce2d8]
__attribute__((visibility("hidden"))) extern int *g_cg_mineSnd;     // [DAT_000ce654]
__attribute__((visibility("hidden"))) extern int *g_cg_objTable23;  // [DAT_000ce660]
__attribute__((visibility("hidden"))) extern int *g_cg_sentryTable; // [DAT_000ce65c]
__attribute__((visibility("hidden"))) extern int *g_cg_bombTable2a; // [DAT_000ce664]
__attribute__((visibility("hidden"))) extern int **g_cg_snd29;      // [DAT_000ce668]
__attribute__((visibility("hidden"))) extern int **g_cg_snd2a;      // [DAT_000ce66c]
__attribute__((visibility("hidden"))) extern int **g_cg_snd2b;      // [DAT_000ce670]
__attribute__((visibility("hidden"))) extern int **g_cg_snd2c;      // [DAT_000ce674]
__attribute__((visibility("hidden"))) extern int **g_cg_snd2d;      // [DAT_000ce678]
__attribute__((visibility("hidden"))) extern int **g_cg_snd2e;      // [DAT_000ce67c]

extern "C" {
void *Level_opnew_cg(unsigned size);
void Gun_ctor_cg(Gun *g, int a, int b, int c, int d, int e, int f, int g7, int h, int i,
                 int j, int k, int l, int m);
void Gun_setIndex_cg(Gun *g, int idx);
void Gun_setPlayerGun_cg(Gun *g, int flag);
void Gun_setErrorMagnitudePercentage_cg(Gun *g, int v);
void Gun_setLevel_cg(Gun *g, Level *self);
void BeamGun_ctor_cg(ObjectGun *o, int a, Gun *g, int idx, Level *self);
void RocketGun_ctor_cg(ObjectGun *o, int a, Gun *g, int res, int b, int c, int d, int e,
                       Level *self);
void ObjectGun_ctor_cg(ObjectGun *o, int a, Gun *g, int res, int d, Level *self);
void BombGun_ctor_cg(ObjectGun *o, Gun *g, unsigned res, int c, int d, int e, Level *self);
void MineGun_ctor_cg(ObjectGun *o, Gun *g, int res, int c, int d, Level *self);
void SentryGun_ctor_cg(ObjectGun *o, Gun *g, int res, int c, int d, Level *self);
int  Item_getAttribute_cg(int item);
void Globals_addSoundResourceToList_cg(int snd);
void ArrayAGun_ctor_cg(void *a);
void ArrayAdd_AGun_cg(ObjectGun *o, void *a);
}

// Level::createGun(idx, owner, kind, hp, dmg, rate, cool, color) — factory for the player's and
// AI ships' weapons; dispatches on `kind` to the right Gun subclass and registers the sound.
extern "C" Gun *Level_createGun(Level *thisptr, int idx, int owner, int kind, int hp, int dmg,
                                int rate, int cool, int color)
{
    char *self = (char *)thisptr;
    ObjectGun *obj = 0;
    Gun *gun = 0;

    switch (kind) {
    case 0:
    case 1:
    case 3: {
        int res = ((int *)g_cg_beamTable)[idx];
        if (res < 0) {
            gun = (Gun *)Level_opnew_cg(0x114);
            Gun_ctor_cg(gun, owner, dmg, 1, hp, cool, rate, color, 0, 0, 0, 0, 0, 0);
            Gun_setIndex_cg(gun, idx);
            *(int *)((char *)gun + 0x5c) = kind;
            Gun_setPlayerGun_cg(gun, 1);
            obj = (ObjectGun *)Level_opnew_cg(0x24);
            BeamGun_ctor_cg(obj, owner, gun, idx, thisptr);
        } else {
            int barrels = ((unsigned)(idx - 9) < 3 || idx == 0xe4) ? 1 : 0x14;
            gun = (Gun *)Level_opnew_cg(0x114);
            if (kind == 3) {
                Gun_ctor_cg(gun, owner, dmg, barrels, hp, cool, rate, color, 0, 0, g_cg_rocketFx,
                            0, 0, 0);
                Gun_setIndex_cg(gun, idx);
                *(int *)((char *)gun + 0x5c) = 3;
                Gun_setPlayerGun_cg(gun, 1);
                Gun_setErrorMagnitudePercentage_cg(gun, 0x14);
                obj = (ObjectGun *)Level_opnew_cg(0xe8);
                RocketGun_ctor_cg(obj, owner, gun, res, 0, 0, 0, 1, thisptr);
            } else {
                Gun_ctor_cg(gun, owner, dmg, barrels, hp, cool, rate, color, 0, 0, 0, 0, 0, 0);
                Gun_setIndex_cg(gun, idx);
                *(int *)((char *)gun + 0x5c) = kind;
                Gun_setPlayerGun_cg(gun, 1);
                obj = (ObjectGun *)Level_opnew_cg(0xb0);
                ObjectGun_ctor_cg(obj, owner, gun, res, 1000, thisptr);
            }
        }
        break;
    }
    case 2:
    case 0x19:
        gun = (Gun *)Level_opnew_cg(0x114);
        Gun_ctor_cg(gun, owner, dmg, 0x19, hp, cool, rate, color, 0, 0, g_cg_objFx, 0, 0, 0);
        Gun_setIndex_cg(gun, idx);
        *(int *)((char *)gun + 0x5c) = kind;
        Gun_setPlayerGun_cg(gun, 1);
        obj = (ObjectGun *)Level_opnew_cg(0xb0);
        ObjectGun_ctor_cg(obj, owner, gun, ((int *)g_cg_objTable)[idx], 1000, thisptr);
        Gun_setErrorMagnitudePercentage_cg(gun, 2);
        break;
    case 4:
    case 5:
    case 0x28: {
        gun = (Gun *)Level_opnew_cg(0x114);
        int barrels = (kind == 0x28) ? (idx - 0xd3) : 5;
        Gun_ctor_cg(gun, owner, dmg, barrels, hp, cool, rate, color, 0, 0, 0, 0, 0, 0);
        Gun_setIndex_cg(gun, idx);
        *(int *)((char *)gun + 0x5c) = kind;
        Gun_setPlayerGun_cg(gun, 1);
        obj = (ObjectGun *)Level_opnew_cg(0xe8);
        RocketGun_ctor_cg(obj, owner, gun, ((int *)g_cg_rocketTable)[idx], 0, 0, kind,
                          (kind == 0x28 || kind == 5) ? 1 : 0, thisptr);
        Globals_addSoundResourceToList_cg(**g_cg_rocketSnd);
        break;
    }
    case 6:
    case 7:
    case 0x22: {
        gun = (Gun *)Level_opnew_cg(0x114);
        Gun_ctor_cg(gun, owner, dmg, 1, hp, cool, rate, color, 0, 0, g_cg_objFx, 0, 0, 0);
        Gun_setIndex_cg(gun, idx);
        *(int *)((char *)gun + 0x5c) = kind;
        Gun_setPlayerGun_cg(gun, 1);
        int attr = Item_getAttribute_cg(*(int *)(*(int *)(*g_cg_itemTableA + 4) + idx * 4));
        obj = (ObjectGun *)Level_opnew_cg(300);
        BombGun_ctor_cg(obj, gun, ((unsigned *)g_cg_bombTable)[idx], 1, kind, attr == 1 ? 1 : 0,
                        thisptr);
        Globals_addSoundResourceToList_cg(*g_cg_bombSnd);
        break;
    }
    case 8:
    case 0x23: {
        int fx = (idx == 0xb5 || idx != 0x30) ? g_cg_mineFx : g_cg_rocketFx;
        int extra = (idx == 0x30 && idx != 0xb5) ? (g_cg_rocketFx - 0xf60000) : 0;
        if (kind == 0x23) { fx = (idx == 0xb5 || idx != 0x30) ? g_cg_mineFx : g_cg_rocketFx; }
        gun = (Gun *)Level_opnew_cg(0x114);
        Gun_ctor_cg(gun, owner, dmg, 0xf, hp, cool, rate, color, extra, 0, fx, 0, 0, 0);
        Gun_setIndex_cg(gun, idx);
        *(int *)((char *)gun + 0x5c) = kind;
        Gun_setPlayerGun_cg(gun, 1);
        if ((idx == 0x30 || idx == 0xe0 || idx == 0xb5)) {
            *(char *)((char *)gun + 0xa4) = 1;
            if (idx == 0xe0) *(char *)((char *)gun + 0xa5) = 1;
        }
        obj = (ObjectGun *)Level_opnew_cg(0xb0);
        int *tbl = (kind == 0x23) ? (int *)g_cg_objTable23 : (int *)g_cg_objTable8;
        ObjectGun_ctor_cg(obj, owner, gun, tbl[idx], 1000, thisptr);
        break;
    }
    case 0xb: {
        gun = (Gun *)Level_opnew_cg(0x114);
        Gun_ctor_cg(gun, owner, dmg, 10, hp, cool, rate, 0x40000000, 0, 0, 0, 0, 0, 0);
        Gun_setIndex_cg(gun, idx);
        *(int *)((char *)gun + 0x5c) = 0xb;
        Gun_setPlayerGun_cg(gun, 1);
        obj = (ObjectGun *)Level_opnew_cg(0xd4);
        MineGun_ctor_cg(obj, gun, ((int *)g_cg_mineTable)[idx], 1, 0xb, thisptr);
        Globals_addSoundResourceToList_cg(*g_cg_mineSnd);
        break;
    }
    case 0x27: {
        gun = (Gun *)Level_opnew_cg(0x114);
        Gun_ctor_cg(gun, owner, 0, 3, hp, cool, rate, 0x40000000, 0, 0, 0, 0, 0, 0);
        Gun_setIndex_cg(gun, idx);
        *(int *)((char *)gun + 0x5c) = 0x27;
        Gun_setPlayerGun_cg(gun, 1);
        obj = (ObjectGun *)Level_opnew_cg(0xb4);
        SentryGun_ctor_cg(obj, gun, ((int *)g_cg_sentryTable)[idx], 1, 0x27, thisptr);
        Globals_addSoundResourceToList_cg(*g_cg_mineSnd);
        break;
    }
    case 0x2a: {
        gun = (Gun *)Level_opnew_cg(0x114);
        Gun_ctor_cg(gun, owner, dmg, 1, hp, 1, rate, color, 0, 0, 0, 0, 0, 0);
        Gun_setIndex_cg(gun, idx);
        *(int *)((char *)gun + 0x5c) = 0x2a;
        Gun_setPlayerGun_cg(gun, 1);
        obj = (ObjectGun *)Level_opnew_cg(300);
        BombGun_ctor_cg(obj, gun, ((unsigned *)g_cg_bombTable2a)[idx], 1, 0x2a, 0, thisptr);
        Globals_addSoundResourceToList_cg(*g_cg_bombSnd);
        break;
    }
    default:
        gun = 0;
        break;
    }

    // index-keyed extra sound resources (fallthrough chain in the original).
    switch (idx) {
    case 0x29: Globals_addSoundResourceToList_cg(**g_cg_snd29);
    case 0x2a: Globals_addSoundResourceToList_cg(**g_cg_snd2a);
    case 0x2b: Globals_addSoundResourceToList_cg(**g_cg_snd2b);
    case 0x2c: Globals_addSoundResourceToList_cg(**g_cg_snd2c);
    case 0x2d: Globals_addSoundResourceToList_cg(**g_cg_snd2d);
    case 0x2e: Globals_addSoundResourceToList_cg(**g_cg_snd2e);
    default: break;
    }

    Gun_setLevel_cg(gun, thisptr);
    void *guns = *(void **)(self + 0xe4);
    if (guns == 0) {
        guns = Level_opnew_cg(0xc);
        ArrayAGun_ctor_cg(guns);
        *(void **)(self + 0xe4) = guns;
    }
    ArrayAdd_AGun_cg(obj, guns);
    return gun;
}
