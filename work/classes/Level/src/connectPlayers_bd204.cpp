#include "class.h"

struct Mission;
struct Station;
struct Player;
struct ApplicationManager;

__attribute__((visibility("hidden"))) extern ApplicationManager **g_cp_appMgr; // [DAT_000cd530]

extern "C" {
int  ApplicationManager_GetCurrentApplicationModule_cp(ApplicationManager *m);
void *Level_opnew_cp(unsigned size);
void ArrayPlayer_ctor_cp(void *a);
void *ArrayPlayer_dtor_cp(void *a);
void ArraySetLength_Player_cp(unsigned n, void *a);
void operator_delete_cp(void *p);
void Player_setEnemies_cp(Player *p, void *arr);
void Player_addEnemies_cp(Player *p, void *arr);
void Player_setEnemy_cp(Player *p, Player *e);
int  Player_isAlwaysFriend_cp();
int  Status_getCurrentCampaignMission_cp();
int  Status_inAlienOrbit_cp();
int  Status_getStation_cp();
int  Station_getIndex_cp(Station *s);
int  Station_stationHasHiddenBlueprint_cp(Station *s, int flag);
int  Status_getMission_cp();
int  Mission_getType_cp();
int  Mission_isEmpty_cp();
int  Mission_isCampaignMission_cp(Mission *m);
int  KIPlayer_isWingMan_cp();
int  PlayerFixedObject_getDockingType_cp();
}

// Level::connectPlayers() — wires up each ship's enemy list from the friend/enemy/neutral arrays,
// honouring a thicket of campaign-mission-specific exceptions.
void Level::connectPlayers()
{
    char *self = (char *)this;

    if (ApplicationManager_GetCurrentApplicationModule_cp(*g_cp_appMgr) == 5)
        return;

    // player's enemy list from the enemy array.
    if (*(int *)(self + 0xf8) != 0 && *(int *)(self + 0xf0) != 0) {
        void *arr = Level_opnew_cp(0xc);
        ArrayPlayer_ctor_cp(arr);
        ArraySetLength_Player_cp(**(unsigned **)(self + 0xf8), arr);
        int n = *(int *)arr;
        for (int j = 0; j != n; j = j + 1)
            *(int *)(*(int *)((char *)arr + 4) + j * 4) =
                *(int *)(*(int *)(*(int *)(*(int *)(self + 0xf8) + 4) + j * 4) + 4);
        Player_setEnemies_cp((Player *)**(int **)(self + 0xf0), arr);
        operator_delete_cp(ArrayPlayer_dtor_cp(arr));
    }
    // add asteroids/landmarks arrays as additional enemies.
    if (*(int *)(self + 0xfc) != 0 && *(int *)(self + 0xf0) != 0) {
        void *arr = Level_opnew_cp(0xc);
        ArrayPlayer_ctor_cp(arr);
        ArraySetLength_Player_cp(**(unsigned **)(self + 0xfc), arr);
        int n = *(int *)arr;
        for (int j = 0; j != n; j = j + 1)
            *(int *)(*(int *)((char *)arr + 4) + j * 4) =
                *(int *)(*(int *)(*(int *)(*(int *)(self + 0xfc) + 4) + j * 4) + 4);
        Player_addEnemies_cp((Player *)**(int **)(self + 0xf0), arr);
        operator_delete_cp(ArrayPlayer_dtor_cp(arr));
    }
    if (*(int *)(self + 0xf4) != 0 && *(int *)(self + 0xf0) != 0) {
        void *arr = Level_opnew_cp(0xc);
        ArrayPlayer_ctor_cp(arr);
        ArraySetLength_Player_cp(**(unsigned **)(self + 0xf4), arr);
        int n = *(int *)arr;
        for (int j = 0; j != n; j = j + 1)
            *(int *)(*(int *)((char *)arr + 4) + j * 4) =
                *(int *)(*(int *)(*(int *)(*(int *)(self + 0xf4) + 4) + j * 4) + 4);
        Player_addEnemies_cp((Player *)**(int **)(self + 0xf0), arr);
        operator_delete_cp(ArrayPlayer_dtor_cp(arr));
    }

    if (*(int *)(self + 0xf8) == 0)
        return;

    int camp = Status_getCurrentCampaignMission_cp();
    for (unsigned i = 0; i < **(unsigned **)(self + 0xf8); i = i + 1) {
        int e = *(int *)((*(unsigned **)(self + 0xf8))[1] + i * 4);
        int eFaction = *(int *)(e + 0x28);
        int wmAll = KIPlayer_isWingMan_cp();
        unsigned count = 0;
        bool notM24 = camp != 0x24;
        bool notFirst = i != 0;

        // first pass: count how many ships become this ship's enemies.
        for (unsigned k = 0; k < **(unsigned **)(self + 0xf8); k = k + 1) {
            int o = *(int *)((*(unsigned **)(self + 0xf8))[1] + k * 4);
            if (o != e && (((~wmAll) & (*(int *)(o + 0x28) == eFaction)) == 0)) {
                bool consider;
                if (notM24 || notFirst) {
                    consider = true;
                    if (camp == 0x9a) {
                        int alien = Status_inAlienOrbit_cp();
                        if (k == 8 && alien != 0 && *(int *)(e + 0x28) == 8)
                            consider = false;
                    } else if (camp == 0x40 && !(i != 0 && e != 0 && k != 0)) {
                        consider = false;
                    }
                } else {
                    consider = KIPlayer_isWingMan_cp() == 0;
                }
                if (consider) {
                    bool skip = false;
                    if (*(char *)(e + 0x40) != 0 && PlayerFixedObject_getDockingType_cp() == 3) {
                        Station *st = (Station *)Status_getStation_cp();
                        if (Station_stationHasHiddenBlueprint_cp(st, 1) != 0)
                            skip = true;
                    }
                    if (!skip)
                        count = count + 1;
                }
            }
        }

        void *arr = Level_opnew_cp(0xc);
        ArrayPlayer_ctor_cp(arr);
        if (*(int *)(self + 0xf0) != 0)
            count = count + 1;
        ArraySetLength_Player_cp(count, arr);

        Mission *m = (Mission *)Status_getMission_cp();
        int mtype = Mission_getType_cp();
        bool branchA = !(((i & 1) == 0 && mtype == 0xc)) &&
                       Mission_getType_cp() != 2 && Mission_getType_cp() != 9;

        bool jumpAlwaysFriend = false;
        if (branchA) {
            // mission-type-specific carve-outs that fall into the "always friend" path.
            int tmp;
            if (Mission_isCampaignMission_cp(m) != 0) {
                tmp = Status_getCurrentCampaignMission_cp();
                if (tmp == 0x10 && *(int *)(e + 0x28) == 9) jumpAlwaysFriend = true;
            }
            if (!jumpAlwaysFriend && Mission_isCampaignMission_cp(m) != 0) {
                tmp = Status_getCurrentCampaignMission_cp();
                if (tmp == 0x18 && *(int *)(e + 0x28) == 9) jumpAlwaysFriend = true;
            }
            if (!jumpAlwaysFriend && Mission_isCampaignMission_cp(m) != 0) {
                tmp = Status_getCurrentCampaignMission_cp();
                if (tmp == 0x1c && *(int *)(e + 0x28) == 9) jumpAlwaysFriend = true;
            }
            if (!jumpAlwaysFriend) {
                if (Mission_isCampaignMission_cp(m) != 0) Status_getCurrentCampaignMission_cp();
                if (Mission_isCampaignMission_cp(m) != 0) Status_getCurrentCampaignMission_cp();

                int *dst = (int *)((char *)arr + 4);
                int slot = 0;
                int ego = *(int *)(self + 0xf0);
                if (ego != 0) {
                    *(int *)(*dst) = *(int *)ego;
                    slot = 1;
                }
                for (unsigned k = 0; k < **(unsigned **)(self + 0xf8); k = k + 1) {
                    int o = *(int *)((*(unsigned **)(self + 0xf8))[1] + k * 4);
                    if (o != e && (((~wmAll) & (*(int *)(o + 0x28) == eFaction)) == 0)) {
                        bool consider;
                        if (notM24 || notFirst) {
                            consider = true;
                            if (camp == 0x9a) {
                                int alien = Status_inAlienOrbit_cp();
                                if (k == 8 && alien != 0 && *(int *)(e + 0x28) == 8)
                                    consider = false;
                            } else if (camp == 0x40 && !(i != 0 && e != 0 && k != 0)) {
                                consider = false;
                            }
                        } else {
                            consider = KIPlayer_isWingMan_cp() == 0;
                        }
                        if (consider) {
                            bool skip = false;
                            if (*(char *)(e + 0x40) != 0 &&
                                PlayerFixedObject_getDockingType_cp() == 3) {
                                Station *st = (Station *)Status_getStation_cp();
                                if (Station_stationHasHiddenBlueprint_cp(st, 1) != 0)
                                    skip = true;
                            }
                            if (!skip) {
                                *(int *)(*dst + slot * 4) =
                                    *(int *)(*(int *)(*(int *)(self + 0xf8) + 4) + k * 4) + 4
                                        ? *(int *)(*(int *)(*(int *)(self + 0xf8) + 4) + k * 4) + 4
                                        : 0;
                                slot = slot + 1;
                            }
                        }
                    }
                }
            }
        }

        if (!branchA || jumpAlwaysFriend) {
            if (Player_isAlwaysFriend_cp() == 0) {
                int slot = 0;
                for (unsigned k = 0; k < **(unsigned **)(self + 0xf8); k = k + 1) {
                    int o = *(int *)((*(unsigned **)(self + 0xf8))[1] + k * 4);
                    if (o != e && *(char *)(o + 0x3c) == 0 &&
                        (((~wmAll) & (*(int *)(o + 0x28) == eFaction)) == 0)) {
                        bool add;
                        if (notM24 || notFirst) {
                            add = !(camp == 0x40 && !(i != 0 && e != 0 && k != 0));
                        } else {
                            add = KIPlayer_isWingMan_cp() == 0;
                        }
                        if (add) {
                            int src = *(int *)(*(int *)(*(int *)(self + 0xf8) + 4) + k * 4);
                            *(int *)(*(int *)((char *)arr + 4) + slot * 4) = *(int *)(src + 4);
                            slot = slot + 1;
                        }
                    }
                }
            } else {
                operator_delete_cp(ArrayPlayer_dtor_cp(arr));
                arr = Level_opnew_cp(0xc);
                ArrayPlayer_ctor_cp(arr);
                ArraySetLength_Player_cp(1, arr);
            }
            *(int *)(*(int *)((char *)arr + 4) + *(int *)arr * 4 - 4) = **(int **)(self + 0xf0);
        }

        Player_addEnemies_cp(*(Player **)(e + 4), arr);
        operator_delete_cp(ArrayPlayer_dtor_cp(arr));

        Status_getMission_cp();
        if (eFaction == 10 && Mission_isEmpty_cp() != 0)
            Player_setEnemy_cp(*(Player **)(e + 4), (Player *)**(int **)(self + 0xf0));
    }
}
