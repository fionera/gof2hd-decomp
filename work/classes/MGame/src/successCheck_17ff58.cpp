#include "class.h"

struct PlayerEgo;
struct Level;
struct DialogueWindow;
struct Mission;
struct Station;
struct StarSystem;
struct Route;
struct PlayerFixedObject;
struct KIPlayer;
struct Player;

extern "C" void *Status_getCampaignMission();                            // 0x7285c
extern "C" int Mission_getType(void *m);                                 // 0x72874
extern "C" Mission *Status_getMission();                                 // 0x72850
extern "C" int Status_missionCompleted(int status, int a, long long b);  // 0x... missionCompleted
extern "C" int Level_checkObjective(int level);                          // 0x... checkObjective
extern "C" int Mission_isCampaignMission(Mission *m);                    // 0x7372c
extern "C" void Status_incMissionCount(void *status);
extern "C" int Status_getCurrentCampaignMission();                       // 0x71770
extern "C" int DialogueWindow_hasSuccessDialogue(int cm);
extern "C" void Status_nextCampaignMission(int status);
extern "C" void Level_removeObjectives(Level *l);
extern "C" void Status_setMission(void *status);
extern "C" void *operator_new(unsigned sz);                              // 0x6eb24
extern "C" void operator_delete(void *p);                                // 0x6eb48
extern "C" void DialogueWindow_ctor(DialogueWindow *w);                  // 0x75544
extern "C" int DialogueWindow_hasLevel(DialogueWindow *w);               // 0x78658
extern "C" void DialogueWindow_setLevel(DialogueWindow *w, Level *l);    // 0x78664
extern "C" void DialogueWindow_set(DialogueWindow *w, int m, int a);     // 0x75550
extern "C" void MGame_pauseSounds(MGame *self);                          // 0x7822c
extern "C" Station *Status_getStation();                                 // 0x71c14
extern "C" int Station_getIndex(Station *s);                             // 0x71824
extern "C" int Level_getEnemies(Level *l);                              // 0x... getEnemies
extern "C" int KIPlayer_isDead(KIPlayer *k);
extern "C" void Player_setHitpoints(Player *p, int hp);
extern "C" void Player_removeAllGuns(Player *p);
extern "C" void PlayerFixedObject_setMoving(PlayerFixedObject *o, int v);
extern "C" StarSystem *Level_getStarSystem(Level *l);                   // 0x727c0
extern "C" void StarSystem_getPlanets(StarSystem *ss);
extern "C" void Route_ctor(Route *r, int *pts, int n);
extern "C" void *Route_clone(Route *r);
extern "C" void KIPlayer_setRoute(KIPlayer *k, Route *r);
extern "C" void *Route_dtor(Route *r);
// Corrupt follow-up-mission setup block (227..291) kept as one helper.
extern "C" void MGame_buildMissionFollowup(MGame *self);

__attribute__((visibility("hidden"))) extern int *g_scGuard;     // @0x18ff6e (stack guard [0])
__attribute__((visibility("hidden"))) extern int *g_scStatus;    // @0x18ffa8 (*puVar18)
__attribute__((visibility("hidden"))) extern uint8_t **g_scFlag; // @0x1904aa
extern "C" __attribute__((noreturn)) void __stack_chk_fail(...);

// Bind a DialogueWindow to the current level (duplicated block at 0x1019020c/0x190020).
static void bindDlg(MGame *self) {
    if (F<int>(self, 0x8c) == 0) {
        DialogueWindow *w = (DialogueWindow *)operator_new(0x74);
        DialogueWindow_ctor(w);
        Level *lvl = F<Level *>(self, 0x78);
        F<int>(self, 0x8c) = (int)w;
        if (lvl != 0) DialogueWindow_setLevel(w, lvl);
    } else if (DialogueWindow_hasLevel(F<DialogueWindow *>(self, 0x8c)) == 0) {
        Level *lvl = F<Level *>(self, 0x78);
        if (lvl != 0) DialogueWindow_setLevel(F<DialogueWindow *>(self, 0x8c), lvl);
    }
}

// MGame::successCheck(): detect mission completion and run its outcome.
extern "C" void MGame_successCheck(MGame *self) {
    int *guard = g_scGuard;
    int g0 = *guard;

    bool timed = !(F<int>(self, 0x4c) < (int)(F<unsigned>(self, 0x48) < 0x1389));
    if (timed || F<uint8_t>(self, 0xdc) != 0) {
        if (Mission_getType(Status_getCampaignMission()) != 0xaa) goto done;
    }

    {
        int *status = g_scStatus;
        int mc = Status_missionCompleted(*status, 0, 0);
        int obj = Level_checkObjective(F<int>(self, 0x78));
        if (mc == 0 && obj == 0) goto done;
    }

    if (Mission_getType(Status_getMission()) == 5) goto deliverFollowup;
    if (Mission_getType(Status_getMission()) == 3) goto deliverFollowup;

    {
        int *status = g_scStatus;
        if (Mission_isCampaignMission(Status_getMission()) == 0)
            Status_incMissionCount(status);

        if (Mission_isCampaignMission(Status_getMission()) != 0) {
            bool hasSuccess = false;
            if (Mission_isCampaignMission(Status_getMission()) != 0) {
                int cm = Status_getCurrentCampaignMission();
                if (DialogueWindow_hasSuccessDialogue(cm) != 0)
                    hasSuccess = true;
            }
            if (!hasSuccess) {
                int cm = Status_getCurrentCampaignMission();
                if (cm > 0x2d && Mission_isCampaignMission(Status_getMission()) != 0) {
                    int cm2 = Status_getCurrentCampaignMission();
                    if (DialogueWindow_hasSuccessDialogue(cm2) == 0) {
                        Status_nextCampaignMission(*status);
                        Level_removeObjectives(F<Level *>(self, 0x78));
                        Status_setMission(status);
                    }
                }
                goto done;
            }
            // fallthrough: has success dialogue -> show it.
            bindDlg(self);
            DialogueWindow_set(F<DialogueWindow *>(self, 0x8c),
                               (int)Status_getMission(), 1);
            F<uint16_t>(self, 0x5d) = 0x101;
            MGame_pauseSounds(self);

            int cm = Status_getCurrentCampaignMission();
            if (Mission_isCampaignMission(Status_getMission()) != 0 && cm == 0x26) {
                int enemies = Level_getEnemies(F<Level *>(self, 0x78));
                unsigned n = *(unsigned *)enemies;
                for (unsigned i = 0; i < n; i++) {
                    int e = *(int *)(*(int *)((char *)enemies + 4) + i * 4);
                    if (*(uint8_t *)((char *)e + 0x40) != 0 &&
                        KIPlayer_isDead((KIPlayer *)e) == 0)
                        Player_setHitpoints(*(Player **)((char *)e + 4),
                                            *(int *)((char *)e + 4));
                }
            } else if (Mission_isCampaignMission(Status_getMission()) != 0 && cm == 0x38) {
                StarSystem *ss = Level_getStarSystem(F<Level *>(self, 0x78));
                StarSystem_getPlanets(ss);
                int pts[3] = {0, 0, 0};
                Route *route = (Route *)operator_new(0x18);
                Route_ctor(route, pts, 3);
                int enemies = Level_getEnemies(F<Level *>(self, 0x78));
                unsigned n = *(unsigned *)enemies;
                for (unsigned i = 0; i < n; i++) {
                    KIPlayer *k = *(KIPlayer **)(*(int *)((char *)enemies + 4) + i * 4);
                    if (*(int *)((char *)k + 0x28) == 1) {
                        Route *rc = (Route *)Route_clone(route);
                        KIPlayer_setRoute(k, rc);
                    }
                }
                operator_delete(Route_dtor(route));
            } else if (Mission_isCampaignMission(Status_getMission()) != 0 && cm == 0x3f) {
                int enemies = Level_getEnemies(F<Level *>(self, 0x78));
                unsigned n = *(unsigned *)enemies;
                for (unsigned i = 0; i < n; i++) {
                    int e = *(int *)(*(int *)((char *)enemies + 4) + i * 4);
                    if (*(int *)((char *)e + 0x28) == 8)
                        Player_removeAllGuns(*(Player **)((char *)e + 4));
                }
            } else if (Mission_isCampaignMission(Status_getMission()) != 0 && cm == 0x49) {
                int enemies = Level_getEnemies(F<Level *>(self, 0x78));
                unsigned n = *(unsigned *)enemies;
                for (unsigned i = 0; i < n; i++) {
                    PlayerFixedObject *o = *(PlayerFixedObject **)(*(int *)((char *)enemies + 4) + i * 4);
                    if (*(uint8_t *)((char *)o + 0x40) != 0 &&
                        KIPlayer_isDead((KIPlayer *)o) == 0) {
                        Player_setHitpoints(*(Player **)((char *)o + 4),
                                            *(int *)((char *)o + 4));
                        PlayerFixedObject_setMoving(o, 1);
                    }
                }
            } else {
                if (Station_getIndex(Status_getStation()) == 0x70 &&
                    Status_getCurrentCampaignMission() == 0x8f)
                    **g_scFlag = 1;
            }
            goto done;
        }
        // Non-campaign success-dialogue path.
        bindDlg(self);
        int m = Status_getCurrentCampaignMission() == 0 ? (int)Status_getMission()
                                                         : Status_getCurrentCampaignMission();
        DialogueWindow_set(F<DialogueWindow *>(self, 0x8c), m, 1);
        F<uint16_t>(self, 0x5d) = 0x101;
        MGame_pauseSounds(self);
        goto done;
    }

deliverFollowup:
    bindDlg(self);
    MGame_buildMissionFollowup(self);

done:
    if (*guard != g0)
        __stack_chk_fail();
}
