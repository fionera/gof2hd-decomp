#include "class.h"

struct PlayerEgo;
struct Level;
struct DialogueWindow;
struct TargetFollowCamera;
struct Mission;
struct Objective;
struct RecordHandler;

extern "C" int PlayerEgo_getHitpoints(PlayerEgo *p);                      // 0x724e4
extern "C" int Level_checkGameOver(int level, int arg);                  // 0x78688
extern "C" int DialogueWindow_hasLevel(DialogueWindow *w);               // 0x78658
extern "C" int PlayerEgo_tryToStartEmergencySystem(PlayerEgo *p);        // 0x78694
extern "C" int PlayerEgo_isInWormhole(PlayerEgo *p);                     // 0x76c84
extern "C" void *GameText_getText(int id);                               // 0x72f70
extern "C" void String_assign(void *dst, const void *src);               // 0x6f2b0
extern "C" void *operator_new(unsigned sz);                              // 0x6eb24
extern "C" void DialogueWindow_ctor(DialogueWindow *w);                  // 0x75544
extern "C" void DialogueWindow_setLevel(DialogueWindow *w, Level *l);    // 0x78664
extern "C" Mission *Status_getMission();                                 // 0x72850
extern "C" void DialogueWindow_set(DialogueWindow *w, Mission *m, int a, int b); // 0x75550
extern "C" void MGame_pauseSounds(MGame *self);                          // 0x7822c
extern "C" void PlayerEgo_setTurretMode(PlayerEgo *p, int v);            // 0x72b08
extern "C" void LevelScript_resetCamera(int script, int arg);            // 0x7246c
extern "C" void PlayerEgo_setFreeLookMode(PlayerEgo *p, int v);          // 0x76b94
extern "C" void TFC_enableFirstPersonCam(TargetFollowCamera *c, int v);  // 0x76ba0
extern "C" void PlayerEgo_hideShipForFirstPersonCameraView(PlayerEgo *p, int v); // 0x76bac
extern "C" void PlayerEgo_explode(PlayerEgo *p);                         // 0x786ac
extern "C" int PlayerEgo_explosionEnded(PlayerEgo *p);                   // 0x786b8
extern "C" int Mission_isCampaignMission(Mission *m);                    // 0x7372c
extern "C" int Status_getCurrentCampaignMission();                       // 0x71770
extern "C" int Objective_isSurvivalObjective(Objective *o);              // 0x786a0
extern "C" int RecordHandler_recordStoreRead(RecordHandler *rh, int id); // 0x7483c
extern "C" void FModSound_play(int snd, int a, int b, float c);          // 0x71548

__attribute__((visibility("hidden"))) extern int **g_goWormText;   // @0x190572
__attribute__((visibility("hidden"))) extern int **g_goDeathText;  // @0x19069c
__attribute__((visibility("hidden"))) extern DialogueWindow **g_goDlgA; // @0x1907c0 (used for getText store)
__attribute__((visibility("hidden"))) extern int **g_goStatusA;    // @0x1906de
__attribute__((visibility("hidden"))) extern int **g_goStatusB;    // @0x1906ea
__attribute__((visibility("hidden"))) extern int **g_goStatusC;    // @0x190764
__attribute__((visibility("hidden"))) extern int **g_goStatusD;    // @0x190770
__attribute__((visibility("hidden"))) extern int **g_goRecHandler; // @0x19077c
__attribute__((visibility("hidden"))) extern int **g_goRecId;      // @0x19077a
__attribute__((visibility("hidden"))) extern int **g_goSnd;        // @0x190746

// Helper to (re)bind a DialogueWindow to the current level (the duplicated block).
static void bindDialogueLevel(MGame *self) {
    if (F<int>(self, 0x8c) == 0) {
        DialogueWindow *w = (DialogueWindow *)operator_new(0x74);
        DialogueWindow_ctor(w);
        Level *lvl = F<Level *>(self, 0x78);
        F<DialogueWindow *>(self, 0x8c) = w;
        if (lvl != 0) DialogueWindow_setLevel(w, lvl);
    } else if (DialogueWindow_hasLevel(F<DialogueWindow *>(self, 0x8c)) == 0) {
        Level *lvl = F<Level *>(self, 0x78);
        if (lvl != 0) DialogueWindow_setLevel(F<DialogueWindow *>(self, 0x8c), lvl);
    }
    DialogueWindow_set(F<DialogueWindow *>(self, 0x8c), Status_getMission(), 2, -1);
}

// MGame::gameOverCheck(): handle player death/explosion and end-of-level dialogue.
extern "C" void MGame_gameOverCheck(MGame *self) {
    if (PlayerEgo_getHitpoints(F<PlayerEgo *>(self, 0x58)) <= 0) {
        if (PlayerEgo_tryToStartEmergencySystem(F<PlayerEgo *>(self, 0x58)) != 0)
            return;
        if (PlayerEgo_isInWormhole(F<PlayerEgo *>(self, 0x58)) == 0) {
            PlayerEgo_setTurretMode(F<PlayerEgo *>(self, 0x58), 0);
            LevelScript_resetCamera(F<int>(self, 0x7c), F<int>(self, 0x78));
            PlayerEgo_setFreeLookMode(F<PlayerEgo *>(self, 0x58), 0);
            TFC_enableFirstPersonCam(F<TargetFollowCamera *>(self, 0xf4), 0);
            PlayerEgo_hideShipForFirstPersonCameraView(F<PlayerEgo *>(self, 0x58), 0);
            F<uint8_t>(self, 0x111) = 1;
            PlayerEgo_explode(F<PlayerEgo *>(self, 0x58));
            if (PlayerEgo_explosionEnded(F<PlayerEgo *>(self, 0x58)) != 0) {
                F<uint8_t>(self, 0x60) = 1;
                void *t = GameText_getText(**g_goDeathText);
                String_assign((char *)self + 0x64, t);
            }
        } else {
            F<uint8_t>(self, 0x60) = 1;
            void *t = GameText_getText(**g_goWormText);
            String_assign((char *)self + 0x64, t);
            F<uint8_t>(self, 0x111) = 1;
        }

        if (F<uint8_t>(self, 0x60) != 0) {
            if (Status_getMission() != 0 &&
                Mission_isCampaignMission(Status_getMission()) != 0) {
                int cm = Status_getCurrentCampaignMission();
                int *sa = *g_goStatusA;
                int v;
                if (cm == *sa) {
                    v = **g_goStatusB + 1;
                } else {
                    **g_goStatusC = 0;
                    v = 1;
                }
                *sa = cm;
                **g_goStatusD = v;
            }
            int rec = RecordHandler_recordStoreRead((RecordHandler *)**g_goRecHandler,
                                                    **g_goRecId);
            F<int>(self, 0x1e8) = rec;
        }
    }

    if (Level_checkGameOver(F<int>(self, 0x78), *(int *)((char *)F<int>(self, 0x78) + 8)) != 0) {
        bindDialogueLevel(self);
        F<uint8_t>(self, 0x5e) = 1;
        MGame_pauseSounds(self);
        F<uint8_t>(self, 0x5d) = 1;
    }

    // Timed end-of-mission check (0x48/0x4c is a 64-bit counter vs 0x1389).
    unsigned lo = F<unsigned>(self, 0x48);
    int hi = F<int>(self, 0x4c);
    bool ready = !(hi < (int)(lo < 0x1389));
    if (ready) {
        F<unsigned>(self, 0x48) = 0;
        F<int>(self, 0x4c) = 0;
        int *sc = F<int *>(self, 0x7c);
        if (sc[0] >= 1) {
            // 64-bit compare against script counters [2]/[3].
            bool done = !((long long)(unsigned)sc[0] - ((long long)sc[3] << 32 | (unsigned)sc[2]) < 0);
            bool survival = false;
            if (!done) {
                int cm = Status_getCurrentCampaignMission();
                if (cm != 0x2a) {
                    Objective *obj = *(Objective **)((char *)F<int>(self, 0x78) + 0x28);
                    survival = (obj == 0) || (Objective_isSurvivalObjective(obj) != 0);
                }
                if (!survival) {
                    bindDialogueLevel(self);
                    F<uint16_t>(self, 0x5d) = 0x101;
                    MGame_pauseSounds(self);
                }
            }
        }
    }

    if (F<uint8_t>(self, 0x60) != 0) {
        F<unsigned>(self, 0x48) = 0;
        F<int>(self, 0x4c) = 0;
        F<int>(self, 0x50) = 0;
        FModSound_play(**g_goSnd, 0x25, 0, 0.0f);
    }
}
