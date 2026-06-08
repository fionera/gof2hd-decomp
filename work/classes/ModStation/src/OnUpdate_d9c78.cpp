#include "class.h"

struct Station;
struct Mission;
struct DialogueWindow;
struct CutScene;
struct SpaceLounge;

__attribute__((visibility("hidden"))) extern int  *g_ou_stack;     // [DAT_000e9d98]
__attribute__((visibility("hidden"))) extern int **g_ou_sound;     // FMod sound holder
__attribute__((visibility("hidden"))) extern int **g_ou_layout;    // Layout holder
__attribute__((visibility("hidden"))) extern int **g_ou_status;    // Status holder
__attribute__((visibility("hidden"))) extern int **g_ou_achievements;
__attribute__((visibility("hidden"))) extern char **g_ou_spaceLoungeFlag; // [DAT_000e9db0]
__attribute__((visibility("hidden"))) extern int **g_ou_appData;   // ApplicationData holder
__attribute__((visibility("hidden"))) extern int **g_ou_textRoot;  // GameText root
__attribute__((visibility("hidden"))) extern int **g_ou_module;    // next app module

extern "C" {
void *ModStation_opnew_ou(unsigned size);
void  ModStation_opdelete_ou(void *p);

// --- self methods reached as direct calls ---
void  ModStation_autosave_ou(ModStation *self);
void  ModStation_checkPendingProducts_ou(ModStation *self);
void  ModStation_checkMedals_ou(ModStation *self);
void  ModStation_checkHints_ou(ModStation *self);

// --- engine / world ---
int   ApplicationManager_GetElapsedTimeMillis_ou();
int   ApplicationManager_GetApplicationData_ou();
void  ApplicationManager_SetCurrentApplicationModule_ou(int module);

void  FModSound_updateAll_ou(int sound);
void  FModSound_play_ou(int sound, int id, void *p, float vol);
void  FModSound_stop_ou(int sound);
void  FModSound_setParamValue_ou(int sound, int a, int b, float v);

void  Layout_update_ou(int layout);
void  Layout_formatCredits_ou(int outStr);

void  Status_incPlayingTime_ou(long long delta);
int   Status_getCredits_ou();
void  Status_changeCredits_ou(int status);
int   Status_getCurrentCampaignMission_ou();
void  Status_nextCampaignMission_ou(int status);
void  Status_setCurrentCampaignMission_ou(int status);
int   Status_getStation_ou();
int   Station_getIndex_ou(Station *s);
int   Status_getSystem_ou();
int   SolarSystem_getIndex_ou();
int   Status_getCampaignMission_ou();
int   Status_missionCompleted_ou(int status, int a, long long b);
int   Status_missionFailed_ou(int status, int a, int b);
void  Status_removeMission_ou(int status);

int   Mission_getType_ou();
int   Mission_getStatusValue_ou();
void  Mission_setStatusValue_ou(int v);
void  Mission_ctor_ou(Mission *m, int a, int b, int c);
void  Mission_setCampaignMission_ou(int flag);

void  Achievements_updateCredits_ou(void *ach, int credits);
int   Globals_getInAppPurchaseArrayIndex_ou(int globals, void *arr);

void  TouchButton_setText_ou(int btn);

void  ChoiceWindow_set_ou(int cw, int text, int flag);
void  ChoiceWindow_setNotice_ou(int cw, int text);
void  ChoiceWindow_update_ou(int cw);
int   GameText_getText_ou(int id);

void  NewsTicker_update_ou(int nt);
void  CutScene_process_ou(int cs);
void  CutScene_update_ou(int cs);
void  CutScene_ctor_ou(CutScene *cs, int kind);
int   CutScene_initialize_ou(CutScene *cs);

void  DialogueWindow_ctor_msg_ou(DialogueWindow *dw, int titleStr, int bodyStr, int *param);
void  DialogueWindow_ctor_mission_ou(DialogueWindow *dw, int mission, int level, int kind);
void  DialogueWindow_ctor_ou(DialogueWindow *dw);
void  DialogueWindow_setMission_ou(int dw, int mission, int flag);
void  DialogueWindow_update_ou(int dw);

void  StarMap_update_ou(int sm, int dt);
void  MissionsWindow_update_ou(int w);
void  HangarWindow_update_ou(int w);
void  HangarWindow_hideMessage_ou();
void  StatusWindow_update_ou(int w);
void  MenuTouchWindow_update_ou(void *w, int dt);
void  MenuTouchWindow_dtor_ou(void *w);

int   SpaceLounge_introFinished_ou();
void  SpaceLounge_ctor_ou(SpaceLounge *l);
void  SpaceLounge_init_ou(int l);
void  SpaceLounge_update_ou(int l);
void  SpaceLounge_setHangarUpdate_ou(int l);

int   Radio_lastMessageShown_ou();

// --- corrupted SIMD blocks delegated to helpers ---
// Runs the three idle-camera "wobble" easings (pitch/yaw/roll) at +0x130..+0x12e and writes the
// resulting camera matrix; the original is dense MatrixSetTranslation/EaseInOut SIMD math.
void  ModStation_ou_updateIdleCamera(ModStation *self, int elapsed);
// Advances the radio-message reveal cutscene scroll box (ScrollTouchBox SIMD positioning).
void  ModStation_ou_updateRadioReveal(ModStation *self, int elapsed);
// Plays out the per-frame idle-hangar ship geometry animation (translate/rotate/moveForward SIMD).
void  ModStation_ou_animateHangarShip(ModStation *self);
// Smooths the hangar light intensity toward its target (FloatVectorMin / clamp SIMD math).
void  ModStation_ou_updateHangarLight(ModStation *self, int elapsed);
}

// ModStation::OnUpdate() — the per-frame station tick: accumulates time, updates audio/layout, the
// active sub-window, the docking-cutscene + idle camera, then drives mission completion/failure
// dialogues and the campaign-progress hooks.
extern "C" void ModStation_OnUpdate(ModStation *self)
{
    char *s = (char *)self;

    int *status = *(int **)g_ou_status;
    int *sound  = *(int **)g_ou_sound;

    // clamp elapsed time to [0, 0x96] ms.
    int elapsed;
    int e = ApplicationManager_GetElapsedTimeMillis_ou();
    if (e < 0x97 && ApplicationManager_GetElapsedTimeMillis_ou() < 0) {
        elapsed = 0;
    } else {
        if (ApplicationManager_GetElapsedTimeMillis_ou() < 0x97)
            elapsed = ApplicationManager_GetElapsedTimeMillis_ou();
        else
            elapsed = 0x96;
    }

    I(s, 0x28) = elapsed;                       // field_28 = dt
    *(long long *)(s + 0x30) += (long long)elapsed; // field_30 = accumulated time (64-bit)

    FModSound_updateAll_ou(*sound);
    int *layout = *(int **)g_ou_layout;
    Layout_update_ou(*layout);
    if (C(s, 0x32) == 0)
        Status_incPlayingTime_ou((long long)(unsigned)**(int **)g_ou_status);

    // refresh the credits button text.
    int creditsBtn = *(int *)(s + 0x50 + 0x4); // this[1].field_4
    Status_getCredits_ou();
    Layout_formatCredits_ou((int)(s + 0)); // formatted into a String temporary
    TouchButton_setText_ou(creditsBtn);

    Achievements_updateCredits_ou((void *)**(int **)g_ou_achievements, Status_getCredits_ou());

    // entering the space lounge intro (triggered by the flag at g_ou_spaceLoungeFlag).
    if (C(s, 0x24) != 0) {
        char *flag = *g_ou_spaceLoungeFlag;
        if (*flag != 0) {
            C(s, 0x94) = 1;          // this[1].field_48 low byte
            *flag = 0;
            if (I(s, 0x74) == 0) {
                SpaceLounge *sl = (SpaceLounge *)ModStation_opnew_ou(0x10c);
                SpaceLounge_ctor_ou(sl);
                I(s, 0x74) = (int)sl;
            } else {
                SpaceLounge_init_ou(I(s, 0x74));
            }
            FModSound_setParamValue_ou(*sound, 0, *sound, 0.0f);
            FModSound_stop_ou(*sound);
            FModSound_play_ou(*sound, 0x6c, 0, 0.0f);
            C(s, 0x65) = 1;          // this->field_64+1
            goto epilogue;
        }
    }

    // in-app-purchase result handling.
    {
        int appData = ApplicationManager_GetApplicationData_ou();
        if (*(char *)(appData + 0x42) != 0) {
            ChoiceWindow_setNotice_ou(I(s, 0x70), GameText_getText_ou(**g_ou_textRoot));
            C(s, 0x6f) = 1;          // m_nStarMapWindowOpen+3
            *(char *)(appData + 0x42) = 0;
        }
        if (*(char *)(appData + 0x41) != 0 && *(int **)(appData + 0x58) != 0 &&
            **(int **)(appData + 0x58) != 0 &&
            (unsigned)(*(int *)(appData + 0x48) - 0x32) < 5) {
            int idx = Globals_getInAppPurchaseArrayIndex_ou(0, *(void **)(appData + 0x48));
            if (-1 < *(int *)(*(int *)(*(int *)(appData + 0x60) + 4) + idx * 4)) {
                Status_changeCredits_ou(*status);
                ModStation_autosave_ou(self);
                ChoiceWindow_setNotice_ou(I(s, 0x70), GameText_getText_ou(**g_ou_textRoot));
                C(s, 0x6f) = 1;
                *(char *)(appData + 0x41) = 0;
                if (I(s, 0x78) != 0)
                    HangarWindow_hideMessage_ou();
                if (I(s, 0x50) != 0) {
                    MenuTouchWindow_dtor_ou(P(s, 0x50));
                    ModStation_opdelete_ou(P(s, 0x50));
                }
                I(s, 0x50) = 0;
            }
        }
    }

    // idle camera + ticker + cutscene while no sub-window is up.
    if (C(s, 0x64) == 0 && C(s, 0x65) == 0 && C(s, 0x66) == 0 && C(s, 0x67) == 0 &&
        C(s, 0x68) == 0 && C(s, 0x60) == 0) {
        if (I(s, 0x1c) != 0)
            NewsTicker_update_ou(I(s, 0x1c));
        if (I(s, 0x14) != 0)
            CutScene_process_ou(I(s, 0x14));
        ModStation_ou_updateIdleCamera(self, elapsed);
    }

    // only proceed past the warm-up once enough time has elapsed.
    if (*(long long *)(s + 0x30) < (long long)(*(int *)(s + 0x30) < 1000 ? 1 : 0))
        goto epilogue;

    if (C(s, 0x60) != 0) {
        // docking cutscene playback + campaign transition at its end.
        int t = *(int *)(s + 0x50 + 0x24); // this[1].field_24
        if (t < 1 + 0 && 0 < I(s, 0x28) + t) {
            if (I(s, 0x14) != 0) {
                // tear down the old cutscene.
                ModStation_opdelete_ou(P(s, 0x14));
            }
            I(s, 0x14) = 0;
            CutScene *cs = (CutScene *)ModStation_opnew_ou(0xa0);
            CutScene_ctor_ou(cs, 2);
            I(s, 0x14) = (int)cs;
            CutScene_initialize_ou(cs);
            *(int *)(I(s, 0x14) + 9 * 4) = 0;
            t = *(int *)(s + 0x50 + 0x24);
            C(s, 0x60) = 1;
        }
        *(int *)(s + 0x50 + 0x24) = t + I(s, 0x28);
        CutScene_update_ou(I(s, 0x14));

        if (Radio_lastMessageShown_ou() != 0)
            ModStation_ou_updateRadioReveal(self, elapsed);

        if (*(int *)(s + 0x50 + 0x24) >= 0 /* DAT_000ea664 */ ) {
            C(s, 0x24) = 0;
            Status_nextCampaignMission_ou(*status);
            int nextModule = **(int **)g_ou_appData;
            ApplicationManager_SetCurrentApplicationModule_ou(nextModule);
            C(s, 0x60) = 0;
            goto epilogue;
        }
    } else if (C(s, 0x68) != 0) {           // DialogueWindow open variant 1
        DialogueWindow_update_ou(I(s, 0x84));
    } else if (C(s, 0x69) != 0) {           // StarMap
        StarMap_update_ou(I(s, 0x10), I(s, 0x28));
    } else if (C(s, 0x67) != 0) {           // HangarWindow
        HangarWindow_update_ou(I(s, 0x78));
    } else if (C(s, 0x66) != 0) {           // MissionsWindow
        MissionsWindow_update_ou(I(s, 0x80));
    } else if (C(s, 0x64) != 0) {           // SpaceLounge
        SpaceLounge_update_ou(I(s, 0x74));
    } else if (C(s, 0x65) != 0) {           // StatusWindow
        StatusWindow_update_ou(I(s, 0x7c));
    }

    if (C(s, 0x6f) != 0)
        ChoiceWindow_update_ou(I(s, 0x70));
    if (C(s, 0x6e) != 0)
        MenuTouchWindow_update_ou(P(s, 0x50), I(s, 0x28));

    // mission completion / failure + campaign dialogue flow (runs once per docked tick).
    if (C(s, 0x6f) == 0 && C(s, 0x69) == 0 && C(s, 0x6c) == 0) {
        if (C(s, 0x96) == 0) {              // this[1].field_48+2
            ModStation_checkPendingProducts_ou(self);
            C(s, 0x96) = 1;
        }
        if (C(s, 0x95) == 0) {              // this[1].field_48+1
            ModStation_checkMedals_ou(self);
            C(s, 0x95) = 1;
        }
        // "remember to save" hint.
        if (C(s, 0x6a) == 0 && C(s, 0x6e) != 0) {
            int rec = *(int *)g_ou_status;
            if (*(char *)(rec + 0x2a) == 0) {
                ChoiceWindow_setNotice_ou(I(s, 0x70), GameText_getText_ou(**g_ou_textRoot));
                C(s, 0x6f) = 1;
                *(char *)(rec + 0x2a) = 1;
                C(s, 0x6e) = 0;
            }
        }

        bool special = false;
        int intro = (C(s, 0x65) != 0) ? SpaceLounge_introFinished_ou() : 0;
        int completed = Status_missionCompleted_ou(*status, 1, (long long)intro);
        Mission *m = (Mission *)completed;
        int camp = Status_getCurrentCampaignMission_ou();
        if (m == 0 && camp == 0x74) {
            if (C(s, 0x65) != 0 && SpaceLounge_introFinished_ou() != 0) {
                Status_getSystem_ou();
                if (SolarSystem_getIndex_ou() == 0x12) {
                    Status_getCampaignMission_ou();
                    int mask = Mission_getStatusValue_ou();
                    Station *st = (Station *)Status_getStation_ou();
                    int sidx = Station_getIndex_ou(st);
                    special = true;
                    if ((1 << ((sidx - 0x5a) & 0xff) & mask) == 0) {
                        int v = Status_getCampaignMission_ou();
                        Status_getCampaignMission_ou();
                        Mission_getStatusValue_ou();
                        Status_getStation_ou();
                        Mission_setStatusValue_ou(v);
                    }
                }
            }
        }

        // mission 0x94..0x97 supernova-system special dialogue vs. the generic path.
        int campNow = Status_getCurrentCampaignMission_ou();
        bool generic = campNow < 0x94 || Status_getCurrentCampaignMission_ou() > 0x97;
        if (!generic) {
            Station *st = (Station *)Status_getStation_ou();
            int sidx = Station_getIndex_ou(st);
            int bit = (sidx == 0x42) ? 2 : (sidx == 0x37) ? 1 : (sidx == 9) ? 4 : 0;
            bool introOk = C(s, 0x65) != 0 && SpaceLounge_introFinished_ou() != 0;
            if (m == 0) {
                if (introOk && 0x93 < Status_getCurrentCampaignMission_ou() &&
                    Status_getCurrentCampaignMission_ou() < 0x97 &&
                    (sidx == 9 || sidx == 0x37 || sidx == 0x42)) {
                    Status_getCampaignMission_ou();
                    if ((Mission_getStatusValue_ou() & bit) == 0) {
                        int v = Status_getCampaignMission_ou();
                        Status_getCampaignMission_ou();
                        Mission_getStatusValue_ou();
                        Mission_setStatusValue_ou(v);
                        Status_getStation_ou();
                        DialogueWindow *dw = (DialogueWindow *)ModStation_opnew_ou(0x74);
                        DialogueWindow_ctor_ou(dw);
                        I(s, 0x84) = (int)dw;
                        Mission *nm = (Mission *)ModStation_opnew_ou(0x78);
                        Mission_ctor_ou(nm, 0xa0, 0, -1);
                        Mission_setCampaignMission_ou((int)nm);
                        DialogueWindow_setMission_ou(I(s, 0x84), (int)nm, 1);
                        C(s, 0x69) = 1;
                        goto afterDialogue;
                    }
                }
                generic = true;
            } else if (introOk && 0x93 < Status_getCurrentCampaignMission_ou() &&
                       sidx == 0x60 && Status_getCurrentCampaignMission_ou() <= 0x96) {
                Status_setCurrentCampaignMission_ou(*status);
                DialogueWindow *dw = (DialogueWindow *)ModStation_opnew_ou(0x74);
                DialogueWindow_ctor_ou(dw);
                I(s, 0x54) = (int)m;       // this[1].field_C cached mission
                I(s, 0x84) = (int)dw;
                DialogueWindow_setMission_ou(I(s, 0x84), (int)m, 1);
                C(s, 0x69) = 1;
                goto afterDialogue;
            } else {
                generic = true;
            }
        }
        if (generic) {
            if (special) {
                // storyline arrival dialogue (mission 0x74 hub).
                Station *st = (Station *)Status_getStation_ou();
                int sidx = Station_getIndex_ou(st);
                int slot = (sidx == 0x5e) ? 3 : (sidx - 0x5a);
                FModSound_play_ou(*sound, slot + 0x619, 0, 0.0f);
                int kind = (slot == 1) ? 0x39 : 0x3a;
                if (slot == 0 || slot == 2) kind = 0;
                DialogueWindow *dw = (DialogueWindow *)ModStation_opnew_ou(0x74);
                DialogueWindow_ctor_msg_ou(dw, GameText_getText_ou(**g_ou_textRoot),
                                           GameText_getText_ou(**g_ou_textRoot),
                                           (int *)(long)kind);
                C(s, 0x69) = 1;
                I(s, 0x84) = (int)dw;
            } else if (m == 0) {
                Mission *fm = (Mission *)Status_missionFailed_ou(*status, 1, 0);
                if (fm != 0) {
                    DialogueWindow *dw = (DialogueWindow *)ModStation_opnew_ou(0x74);
                    DialogueWindow_ctor_mission_ou(dw, (int)fm, 0, 2);
                    C(s, 0x69) = 1;
                    I(s, 0x84) = (int)dw;
                    Status_removeMission_ou(*status);
                    if (Mission_getType_ou() == 0xd) {
                        *(short *)(*status + 0xf0) = 0;
                        ModStation_autosave_ou(self);
                    }
                }
            } else {
                I(s, 0x54) = (int)m;
                DialogueWindow *dw = (DialogueWindow *)ModStation_opnew_ou(0x74);
                DialogueWindow_ctor_mission_ou(dw, (int)m, 0, 1);
                C(s, 0x69) = 1;
                I(s, 0x84) = (int)dw;
                if (Mission_getType_ou() == 0xd) {
                    *(short *)(*status + 0xf0) = 0;
                    ModStation_autosave_ou(self);
                }
                if (I(s, 0x74) != 0)
                    SpaceLounge_setHangarUpdate_ou(I(s, 0x74));
            }
        }
    afterDialogue:
        ModStation_checkHints_ou(self);
    }

    // idle hangar light + ship animation.
    if (C(s, 0x50 + 0x70) == 0)            // this[1].field_70 low byte
        ModStation_ou_updateHangarLight(self, elapsed);
    if (I(s, 0x14) != 0 && C(s, 0x60) == 0)
        ModStation_ou_animateHangarShip(self);

epilogue:;
}
