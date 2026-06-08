#include "class.h"

struct Waypoint;
struct Player;
struct AEGeometry;
struct PlayerFixedObject;
struct PlayerFighter;
struct Globals;
struct LODManager;

__attribute__((visibility("hidden"))) extern int    **g_cs_rng;      // [DAT_000d0468]
__attribute__((visibility("hidden"))) extern int     *g_cs_diffRec;  // [DAT_000d0474]
__attribute__((visibility("hidden"))) extern Globals **g_cs_globalsA;// [DAT_000d0838]
__attribute__((visibility("hidden"))) extern Globals **g_cs_globalsB;// [DAT_000d0cdc]

extern "C" {
int  cs_rand40000(int rng);
int  Status_getCurrentCampaignMission_cs();
int  Status_getLevel_cs();
int  Status_gameWon_cs();
int  Status_inAlienOrbit_cs();
int  Status_hardCoreMode_cs();
void *Level_opnew_cs(unsigned size);
void Player_ctor_cs(Player *p, int hp, int dmg, int a, int b, int c);
void Player_setEmpData_cs(Player *p, int a, int b);
void PlayerFighter_ctor_cs(PlayerFighter *pf, int type, int race, Player *pl, AEGeometry *g,
                           float x, float y, float z, int flag);
void PlayerFixedObject_ctor_cs(PlayerFixedObject *o, int type, int race, Player *pl,
                               AEGeometry *g, float x, float y, float z);
void PlayerFixedObject_setWreckedMeshId_cs(PlayerFixedObject *o, int mesh);
void PlayerFixedObject_setBV_cs(PlayerFixedObject *o, void *bv);
int  Globals_getShipGroup_cs(Globals *g, int type, int race, int flag);
void LODManager_addObject_cs(LODManager *m, AEGeometry *g);
void *Array_int_dtor_cs(void *a);
void operator_delete_cs(void *p);
// Builds the class-appropriate bounding-volume array (the original is a long cascade of SIMD
// BoundingAAB constructions Ghidra could not lift) and returns it; also returns the wreck mesh id.
void *Level_cs_buildBV(int race, int type, int *outWreckMesh);
}

// Level::createShip(race, shipClass, type, wp, hostile, group) — spawns a fighter (class 0) or a
// fixed capital ship (class 1) at the waypoint, scaling its stats to the player level/difficulty.
extern "C" PlayerFixedObject *Level_createShip(Level *thisptr, int race, int shipClass, int type,
                                               Waypoint *wp, int hostile, int group)
{
    char *self = (char *)thisptr;
    int camp = Status_getCurrentCampaignMission_cs();

    int x = 0, y = 0, z = 0;
    if (wp != 0) {
        x = *(int *)((char *)wp + 0x124);
        y = *(int *)((char *)wp + 0x128);
        z = *(int *)((char *)wp + 0x12c);
    }
    int rng = *(int *)g_cs_rng;
    int jx = cs_rand40000(rng);
    int jy = cs_rand40000(rng);
    int jz = cs_rand40000(rng);

    // base hitpoints from player level + campaign ramp.
    int lvl = Status_getLevel_cs();
    if (0x15 <= lvl) lvl = 0x14;
    int ramp = (Status_gameWon_cs() != 0) ? 0xb4 : (camp << 2);
    int hp = ramp + lvl * 0xe + 0x14;
    if (type == 0x33)      hp = (int)((float)hp * 1.0f);
    else if (type == 0x31) hp = (int)((float)hp * 17.0f);
    else if (type == 0x2c) hp = (int)((float)hp * 2.25f);
    if ((unsigned)(camp - 0x31) < 8 && (0x8fU >> ((unsigned)(camp - 0x31) & 0xff) & 1) != 0)
        hp = 0x10e;

    int dmgLvl = Status_getLevel_cs();
    if (0x15 <= dmgLvl) dmgLvl = 0x14;
    int empA = dmgLvl * 5 + 0x28;
    int empB;
    if (shipClass == 1) {
        int mul = (type == 0xe) ? 0x19 : 5;
        empA = empA * 3;
        hp = mul * hp;
        empB = 45000;
    } else {
        empB = 15000;
    }
    hp = (int)((float)hp + (*(float *)(*(int *)g_cs_diffRec + 0x2c) - 0.5f) * (float)hp);
    if (camp == 0x9a) {
        int alien = Status_inAlienOrbit_cs();
        hp = hp << (alien & (race == 9));
    }

    Player *pl = (Player *)Level_opnew_cs(0x114);
    int playerHp = (Status_hardCoreMode_cs() != 0) ? 0x28a : 1000;
    Player_ctor_cs(pl, playerHp, hp, 1, 1, 0);
    float fx = (float)(x + jx - 20000);
    float fy = (float)(y + jy - 20000);
    float fz = (float)(z + jz - 20000);
    Player_setEmpData_cs(pl, empA, empB);

    PlayerFixedObject *obj = 0;
    if (shipClass == 0) {
        PlayerFighter *pf = (PlayerFighter *)Level_opnew_cs(0x2f0);
        PlayerFighter_ctor_cs(pf, type, race, pl, 0, fx, fy, fz, 0);
        obj = (PlayerFixedObject *)pf;
        int gg = Globals_getShipGroup_cs(*g_cs_globalsA, type, race, group);
        (*(void (**)(PlayerFixedObject *, int, int, int))(*(int *)obj + 8))(obj, gg, type, hostile);
        if (*(int *)(self + 0xc0) != 1 && *(int *)(self + 0xc0) != 0x17) {
            AEGeometry *g = *(AEGeometry **)((char *)obj + 0xc);
            if (g == 0) g = *(AEGeometry **)((char *)obj + 8);
            LODManager_addObject_cs(*(LODManager **)self, g);
        }
        if (type == 0x2c || type == 0x31 || type == 0x33) {
            if (type == 0x33) *(char *)((char *)obj + 0x25) = 0;
            if (type != 0x33 && *(void **)((char *)obj + 0x50) != 0) {
                operator_delete_cs(Array_int_dtor_cs(*(void **)((char *)obj + 0x50)));
                *(int *)((char *)obj + 0x50) = 0;
            }
        }
    } else if (shipClass == 1) {
        obj = (PlayerFixedObject *)Level_opnew_cs(0x1bc);
        PlayerFixedObject_ctor_cs(obj, type, race, pl, 0, fx, fy, fz);
        int wreck = 0;
        void *bv = Level_cs_buildBV(race, type, &wreck);
        PlayerFixedObject_setWreckedMeshId_cs(obj, wreck);
        PlayerFixedObject_setBV_cs(obj, bv);
        int gg = Globals_getShipGroup_cs(*g_cs_globalsB, type, race, 0);
        (*(void (**)(PlayerFixedObject *, int, int, int))(*(int *)obj + 8))(obj, gg, type, 0);
        LODManager_addObject_cs(*(LODManager **)self, *(AEGeometry **)((char *)obj + 8));
        *(char *)((char *)obj + 0x40) = 1;
    }

    if (obj != 0)
        (*(void (**)(PlayerFixedObject *, Level *))(*(int *)obj + 0x14))(obj, thisptr);
    return obj;
}
