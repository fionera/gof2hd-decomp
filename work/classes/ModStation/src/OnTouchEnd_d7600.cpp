#include "class.h"

struct Station;
struct Ship;
struct Item;
struct Mission;
struct Agent;
struct DialogueWindow;
struct HangarWindow;
struct MenuTouchWindow;
struct Radio;
struct RadioMessage;
struct ScrollTouchBox;
struct RecordHandler;
struct Layout;

__attribute__((visibility("hidden"))) extern int  *g_ote_stack;     // [DAT_000e7908]
__attribute__((visibility("hidden"))) extern int **g_ote_helpLayout;// [DAT_000e790c] help-window layout
__attribute__((visibility("hidden"))) extern int **g_ote_status;    // Status holder
__attribute__((visibility("hidden"))) extern int **g_ote_achievements;
__attribute__((visibility("hidden"))) extern int **g_ote_module;    // next application module
__attribute__((visibility("hidden"))) extern int **g_ote_canvas;    // paint canvas
__attribute__((visibility("hidden"))) extern int **g_ote_sound;     // FMod sound
__attribute__((visibility("hidden"))) extern int **g_ote_textRoot;  // GameText root
__attribute__((visibility("hidden"))) extern int **g_ote_galaxy;    // Galaxy holder
__attribute__((visibility("hidden"))) extern int **g_ote_itemTable; // item descriptor table
__attribute__((visibility("hidden"))) extern int **g_ote_shipTable; // ship descriptor table

extern "C" {
void *ModStation_opnew_ote(unsigned size);
void  ModStation_opdelete_ote(void *p);

// --- self methods reached directly ---
void  ModStation_autosave_ote(ModStation *self);
void  ModStation_enterStation_ote(ModStation *self);
void  ModStation_resetLight_ote(ModStation *self);
void  ModStation_resetIdleCamForHangar_ote(ModStation *self);
void  ModStation_checkHints_ote(ModStation *self);

// --- engine / world ---
void  ApplicationManager_SetCurrentApplicationModule_ote(int module);
int   ApplicationManager_GetApplicationData_ote();

int   Status_getStation_ote();
int   Station_getIndex_ote(Station *s);
int   Station_stationHasPirateBase_ote();
int   Station_hasShip_ote(int station);
int   Station_getAgents_ote(Station *s);
void  Station_addShip_ote(Ship *s);
void  Station_departStation_ote(Station *s);
void  Station_setAttackedFriends_ote(Station *s, int flag);
void  Station_setItems_ote(Station *s, void *arr, int flag);

int   Status_getShip_ote();
int   Status_getCredits_ote();
void  Status_changeCredits_ote(int status);
int   Status_getCurrentCampaignMission_ote();
void  Status_nextCampaignMission_ote(int status);
void  Status_departStation_ote(Station *s);
void  Status_setStation_ote(Station *s);
void  Status_setMission_ote(int status);
void  Status_removeMission_ote(int status);
void  Status_incMissionCount_ote(int status);
void  Status_setPassengers_ote(int status, int n);
void  Status_checkForLevelUp_ote();
void  Status_activateNewWanted_ote();

int   Ship_getCargo_ote();
int   Ship_getIndex_ote();
void  Ship_getRace_ote(Ship *s);
void  Ship_removeCargo_ote(int ship, int item);
void  Ship_removeCargo1_ote(Item *it);
void  Ship_addCargo_ote(Item *it);
void  Ship_makeShip_ote(int desc);

int   Item_isUnsaleable_ote(Item *it);
int   Item_getIndex_ote(Item *it);
int   Item_getAmount_ote();
void  Item_makeItem_ote(int desc);

int   Mission_getType_ote();
int   Mission_isCampaignMission_ote(Mission *m);
int   Mission_getProductionGoodIndex_ote(Mission *m);
int   Mission_getProductionGoodAmount_ote();
int   Mission_getReward_ote();
int   Mission_getBonus_ote();

int   Agent_getOffer_ote(Agent *a);
int   Agent_getSellItemIndex_ote(Agent *a);
void  Agent_setEvent_ote(Agent *a, int e);
void  Agent_setOfferAccepted_ote(Agent *a, int flag);

void  Achievements_resetNewMedals_ote(void *ach);

int   GameText_getText_ote(int id);
void  Galaxy_getStation_ote(int index);

void  Layout_OnTouchEnd_ote(Layout *l, int p1, int p2);
int   Layout_OnTouchEndR_ote(Layout *l, int p1, int p2);
int   Layout_helpPressed_ote(Layout *l);
void  Layout_initHelpWindow_ote(int l, int textStr);
void  Layout_showMissionRewardMessage_ote(int l, int flag);

int   DialogueWindow_OnTouchEnd_ote(int dw, int p1);
void  DialogueWindow_dtor_ote(DialogueWindow *dw);
int   ChoiceWindow_OnTouchEnd_ote(int cw, int p1);
void  ChoiceWindow_set_ote(int cw, int textStr, int flag);
void  ChoiceWindow_setNotice_ote(int cw, int textStr);
int   HangarWindow_OnTouchEnd_ote(HangarWindow *w, int p1, int p2);
void  HangarWindow_ctor_ote(HangarWindow *w);
void  HangarWindow_dtor_ote(HangarWindow *w);
void  HangarWindow_initialize_ote();
void  HangarWindow_showCreditsBuyWindow_ote(HangarWindow *w);
void  HangarWindow_setSellMode_ote(int w);
int   StarMap_OnTouchEnd_ote(int sm, int p1);
int   SpaceLounge_OnTouchEnd_ote(int l, int p1, int p2);
int   SpaceLounge_hangarNeedsUpdate_ote();
void  SpaceLounge_refresh_ote();
int   StatusWindow_OnTouchEnd_ote(int w, int p1, int p2);
int   MissionsWindow_OnTouchEnd_ote(int w, int p1);
int   MenuTouchWindow_OnTouchEnd_ote(MenuTouchWindow *w, int p1, int p2, void *p3);
void  MenuTouchWindow_ctor_ote(MenuTouchWindow *w, int kind);
int   NewsTicker_OnTouchEnd_ote(int nt, int p1);
int   TouchButton_OnTouchEnd_ote(int btn, int p1);

void  FModSound_play_ote(int sound, int id, void *p, float vol);
void  FModSound_stop_ote(int sound);
void  FModSound_setParamValue_ote(int sound, int a, int b, float v);

void  RecordHandler_saveOptions_ote(RecordHandler *rh);
void  CutScene_replacePlayerShip_ote(int cs, int shipIndex);
void  CutScene_checkForTurret_ote(int cs);

// --- corrupted/SIMD blocks delegated to helpers ---
// Snapshots the cached docking-button positions into the parallel x/y arrays (TouchButton::getPosition
// SIMD reads the decompiler corrupted). Called after a sub-window opens/closes.
void  ModStation_ote_cacheButtonPositions(ModStation *self);
// Pops the "pay docking fee" choice window with its formatted-credits hash-replace String chain.
void  ModStation_ote_showDockingFeeChoice(ModStation *self, int credits);
// Pops the mission-0x6c reward choice window (String hash-replace chain).
void  ModStation_ote_showRewardChoice(ModStation *self, int credits);
// Builds the supernova radio-cutscene Radio + its four RadioMessages + the scroll box (SIMD layout).
void  ModStation_ote_startRadioCutscene(ModStation *self);
// Recomputes the idle-camera target after a news-ticker tap (VectorSignedToFloat wobble math).
void  ModStation_ote_kickIdleCamera(ModStation *self);
// The store-launch tail thunk (the original tail-calls a loaded code pointer to switch modules with
// the given argument). Returns nothing; control returns to the caller afterwards.
void  ModStation_ote_launchModule(int module, int arg);

// --- internal sub-handlers (split out of the original monolithic function) ---
void  handleChoiceDecline(ModStation *self, int param_1, int param_2);
void  handleChoiceDeclineTail(ModStation *self);
void  handleMainButtons(ModStation *self, int param_1, int param_2);
void  handleMissionComplete(ModStation *self);
void  handleCampaignTransition(ModStation *self, int cm);
void  finishMissionReward(ModStation *self);
}

// the help-window layout pointer (DAT_000e790c equivalent), used by the reward message call.
static int *help_layout()
{
    return *(int **)g_ote_helpLayout;
}

// ModStation::OnTouchEnd(int x, int y, void *touch) — routes a touch release to the active station
// sub-window or the main button strip; the dialogue-accept path runs the full mission-completion and
// campaign-advance handler.
extern "C" void ModStation_OnTouchEnd(ModStation *self, int param_1, int param_2, void *param_3)
{
    char *s = (char *)self;
    int *status = *(int **)g_ote_status;

    // only react to the touch we captured; clear the captured-touch slot.
    if (P(s, 0x128) != param_3)
        return;
    int starMapOpen = C(s, 0x60);
    C(s, 0x118) = 0;
    P(s, 0x128) = 0;
    if (starMapOpen != 0)
        return;

    // help window takes priority.
    int *help = *(int **)g_ote_helpLayout;
    if (*(char *)*help != 0) {
        if (Layout_OnTouchEndR_ote((Layout *)*help, param_1, param_2) != 0)
            *(char *)*help = 0;
        return;
    }

    // ---- dialogue window open ----
    if (C(s, 0x69) != 0) {
        if (DialogueWindow_OnTouchEnd_ote(I(s, 0x84), param_1) != 0) {
            if (I(s, 0x54) != 0) {                  // this[1].field_C: an active mission
                handleMissionComplete(self);
                return;
            }
            // plain dialogue dismissal.
            DialogueWindow *dw = (DialogueWindow *)P(s, 0x84);
            C(s, 0x69) = 0;
            if (dw != 0) {
                DialogueWindow_dtor_ote(dw);
                ModStation_opdelete_ote(dw);
            }
            char justEntered = C(s, 0x71);          // this[1].field_70+1
            I(s, 0x84) = 0;
            if (justEntered == 0) {
                Station *st = (Station *)Status_getStation_ote();
                if (Station_getIndex_ote(st) != 4) {
                    st = (Station *)Status_getStation_ote();
                    if (Station_getIndex_ote(st) != 0x58) {
                        Status_getStation_ote();
                        if (Station_stationHasPirateBase_ote() != 0) {
                            C(s, 0x4e) = 0;          // this[1].field_20+2
                            Station *home = (Station *)**(int **)g_ote_status;
                            Status_getStation_ote();
                            Status_departStation_ote(home);
                            Achievements_resetNewMedals_ote((void *)**(int **)g_ote_achievements);
                            int mod = (Status_getCurrentCampaignMission_ote() == 0x10) ? -1 : 1;
                            **(int **)g_ote_module = mod;
                            ApplicationManager_SetCurrentApplicationModule_ote(**(int **)g_ote_module);
                            C(s, 0x24) = 0;
                        }
                    }
                }
            }
            return;
        }
        if (C(s, 0x65) != 0)
            return;
    }

    // ---- choice window (jump confirm / docking fee / reward) ----
    if (C(s, 0x6f) != 0) {
        int r = ChoiceWindow_OnTouchEnd_ote(I(s, 0x70), param_1);
        if (r == 1) {
            char departed = C(s, 0x6b);             // this->field_68+3
            C(s, 0x4e) = 0;
            if (departed == 0) {
                C(s, 0x6f) = 0;
                C(s, 0x98) = 0;                     // this[1].field_4C
            } else {
                C(s, 0x4e) = 0;
                Station *home = (Station *)**(int **)g_ote_status;
                Status_getStation_ote();
                Status_departStation_ote(home);
                Achievements_resetNewMedals_ote((void *)**(int **)g_ote_achievements);
                **(int **)g_ote_module = 1;
                ApplicationManager_SetCurrentApplicationModule_ote(**(int **)g_ote_module);
                C(s, 0x24) = 0;
            }
        } else if (r == 0) {
            handleChoiceDecline(self, param_1, param_2);
        }
        return;
    }

    // ---- main station screen: dispatch to the open sub-window, else the button strip ----
    if (C(s, 0x68) != 0) {                          // StatusWindow
        if (StatusWindow_OnTouchEnd_ote(I(s, 0x7c), param_1, param_2) != 0) {
            C(s, 0x68) = 0;
            ModStation_resetLight_ote(self);
        }
        return;
    }
    if (C(s, 0x67) != 0) {                          // StarMap
        if (StarMap_OnTouchEnd_ote(I(s, 0x10), param_1) != 0) {
            C(s, 0x67) = 0;
            ModStation_resetLight_ote(self);
        }
        return;
    }
    if (C(s, 0x66) != 0) {                          // HangarWindow
        if (HangarWindow_OnTouchEnd_ote((HangarWindow *)P(s, 0x78), param_1, param_2) != 0) {
            int *st = *(int **)g_ote_status;
            // tally any tradeable cargo sold this visit.
            Status_getShip_ote();
            unsigned *cargo = (unsigned *)Ship_getCargo_ote();
            int sold = 0;
            if (cargo != 0) {
                for (unsigned i = 0; i < *cargo; i = i + 1) {
                    int idx = Item_getIndex_ote(*(Item **)(cargo[1] + i * 4));
                    if (0x83 < idx && Item_getIndex_ote(*(Item **)(cargo[1] + i * 4)) < 0x9a)
                        sold += Item_getAmount_ote();
                }
            }
            if (*(int *)(s + 0x50 + 0x40) < sold)   // this[1].field_40
                *(int *)(*st + 0xa8) += sold - *(int *)(s + 0x50 + 0x40);
            HangarWindow_setSellMode_ote(I(s, 0x78));
            ModStation_resetIdleCamForHangar_ote(self);
            char *hw = *(char **)(s + 0x78);
            C(s, 0x66) = 0;
            if (*hw != 0) {
                *hw = 0;
                int cs = I(s, 0x14);
                Status_getShip_ote();
                int shipIndex = Ship_getIndex_ote();
                Ship *sh = (Ship *)Status_getShip_ote();
                Ship_getRace_ote(sh);
                CutScene_replacePlayerShip_ote(cs, shipIndex);
            }
            if (I(s, 0x14) != 0)
                CutScene_checkForTurret_ote(I(s, 0x14));
            int snd = **(int **)g_ote_sound;
            FModSound_stop_ote(snd);
            FModSound_play_ote(snd, 0x7a, 0, 0.0f);
            FModSound_setParamValue_ote(snd, 0, snd, 0.0f);
            ModStation_ote_cacheButtonPositions(self);
        }
        return;
    }
    if (C(s, 0x64) != 0) {                          // SpaceLounge
        if (SpaceLounge_OnTouchEnd_ote(I(s, 0x74), param_1, param_2) != 0) {
            C(s, 0x65) = 0;
            ModStation_resetIdleCamForHangar_ote(self);
            ModStation_resetLight_ote(self);
            int snd = **(int **)g_ote_sound;
            FModSound_setParamValue_ote(snd, 0, snd, 0.0f);
            FModSound_stop_ote(snd);
            FModSound_play_ote(snd, 0x7a, 0, 0.0f);
            CutScene_checkForTurret_ote(I(s, 0x14));
            if (I(s, 0x74) != 0 && SpaceLounge_hangarNeedsUpdate_ote() != 0) {
                if (P(s, 0x78) != 0) {
                    HangarWindow_dtor_ote((HangarWindow *)P(s, 0x78));
                    ModStation_opdelete_ote(P(s, 0x78));
                }
                I(s, 0x78) = 0;
            }
        }
        return;
    }
    if (C(s, 0x65) != 0) {                          // MissionsWindow
        if (MissionsWindow_OnTouchEnd_ote(I(s, 0x80), param_1) != 0) {
            C(s, 0x64) = 0;
            int snd = **(int **)g_ote_sound;
            FModSound_setParamValue_ote(snd, 0, snd, 0.0f);
        }
        return;
    }
    if (C(s, 0x6e) != 0) {                          // MenuTouchWindow (DLC)
        if (MenuTouchWindow_OnTouchEnd_ote((MenuTouchWindow *)P(s, 0x50),
                                           param_1, param_2, param_3) != 0) {
            C(s, 0x6e) = 0;
            ModStation_ote_cacheButtonPositions(self);
        }
        return;
    }
    if (C(s, 0x6d) != 0) {                          // m_nStarMapWindowOpen+1: button strip active
        handleMainButtons(self, param_1, param_2);
    }
}

// --- the decline branch of the jump/depart choice window ---
extern "C" void handleChoiceDecline(ModStation *self, int param_1, int param_2)
{
    char *s = (char *)self;
    int *status = *(int **)g_ote_status;
    (void)param_1; (void)param_2;

    if (C(s, 0x4e) != 0) {                          // a departure was pending
        C(s, 0x4e) = 0;
        int camp = Status_getCurrentCampaignMission_ote();
        if (camp == 0x18) {
            Station *st = (Station *)Status_getStation_ote();
            if (Station_getIndex_ote(st) == 10) {
                unsigned i = 0;
                for (;;) {
                    Station *st1 = (Station *)Status_getStation_ote();
                    unsigned *ag = (unsigned *)Station_getAgents_ote(st1);
                    if (*ag <= i) break;
                    Station *st2 = (Station *)Status_getStation_ote();
                    int agents = Station_getAgents_ote(st2);
                    Agent *a = *(Agent **)(*(int *)(agents + 4) + i * 4);
                    if (Agent_getOffer_ote(a) == 2 && Agent_getSellItemIndex_ote(a) == 0x44) {
                        Agent_setEvent_ote(a, 1);
                        Agent_setOfferAccepted_ote(a, 1);
                    }
                    i = i + 1;
                }
            }
        }
        Station *home = (Station *)*status;
        if (Status_getCurrentCampaignMission_ote() == 0x30) {
            Galaxy_getStation_ote(**(int **)g_ote_galaxy);
            Status_departStation_ote(home);
            **(char **)g_ote_galaxy = 1;
        } else {
            Status_getStation_ote();
            Status_departStation_ote(home);
        }
        *(int *)(*status + 0x5c) = -1;
        *(int *)(*status + 0x60) = -1;
        *(int *)(*status + 0x64) = -1;
        *(int *)(*status + 0x68) = -1;
        Achievements_resetNewMedals_ote((void *)**(int **)g_ote_achievements);
        **(int **)g_ote_module = 1;
        ApplicationManager_SetCurrentApplicationModule_ote(**(int **)g_ote_module);
        C(s, 0x24) = 0;
        return;
    }

    if (C(s, 0x6b) != 0) {                          // standing-penalty branch
        int credits = Status_getCredits_ote();
        if (*(int *)(s + 0x50 + 0x44) <= credits) {  // this[1].field_44 == price
            Status_changeCredits_ote(*status);
            C(s, 0x6b) = 0;
            C(s, 0x94) = 1;                          // this[1].field_48
            Station *st = (Station *)Status_getStation_ote();
            Station_setAttackedFriends_ote(st, 0);
            C(s, 0x71) = 1;                          // this[1].field_70+1
            ModStation_enterStation_ote(self);
            ModStation_autosave_ote(self);
            handleChoiceDeclineTail(self);
            return;
        }
        ModStation_ote_showDockingFeeChoice(self, credits);
        C(s, 0x6b) = 1;
        C(s, 0x4e) = 1;
        C(s, 0x6f) = 1;
        return;
    }

    handleChoiceDeclineTail(self);
}

// the post-standing-penalty continuation (mission-0x6c reward / level-up cleanup).
extern "C" void handleChoiceDeclineTail(ModStation *self)
{
    char *s = (char *)self;
    int *status = *(int **)g_ote_status;

    if (C(s, 0x98) != 0) {                          // this[1].field_4C: pending app-data flag
        C(s, 0x6f) = 0;
        C(s, 0x98) = 0;
        int appData = ApplicationManager_GetApplicationData_ote();
        *(char *)(appData + 0xd) = 1;
        return;
    }
    if (C(s, 0x6c) == 0) {                          // this->field_6C
        if (C(s, 0x6d) != 0) {                      // this->field_6C+1: reward-mission state
            int credits = Status_getCredits_ote();
            if (credits < 25000) {
                ModStation_ote_showRewardChoice(self, credits);
                C(s, 0x6d) = 0;
                return;
            }
            Status_changeCredits_ote(*status);
            C(s, 0x6f) = 0;
            C(s, 0x6d) = 0;
            Station *home = (Station *)*status;
            Galaxy_getStation_ote(**(int **)g_ote_galaxy);
            Status_setStation_ote(home);
            **(int **)g_ote_module = 0;
            ApplicationManager_SetCurrentApplicationModule_ote(**(int **)g_ote_module);
        }
        if (C(s, 0x18) != 0 && C(s, 0x66) != 0) {
            C(s, 0x18) = 0;
            C(s, 0x66) = 0;
        }
        C(s, 0x6f) = 0;
    } else {
        Status_changeCredits_ote(*status);
        Ship_removeCargo_ote(Status_getShip_ote(), 0x6d);
        *(int *)(*status + 0x114) = 3;
        RecordHandler_saveOptions_ote((RecordHandler *)**(int **)g_ote_status);
        ChoiceWindow_setNotice_ote(I(s, 0x70), GameText_getText_ote(**g_ote_textRoot));
        Station *st = (Station *)Status_getStation_ote();
        Station_setItems_ote(st, 0, 0);
        Station_setItems_ote(*(Station **)(*status + 0x14c), 0, 0);
        C(s, 0x6c) = 0;
    }
}

// --- main button strip dispatch (launch / buy ship / DLC menu / news ticker) ---
extern "C" void handleMainButtons(ModStation *self, int param_1, int param_2)
{
    char *s = (char *)self;
    int *help = *(int **)g_ote_helpLayout;

    // launch button.
    if (TouchButton_OnTouchEnd_ote(*(int *)(s + 0x50 + 0), param_1) != 0)
        return;

    // "buy credits" button -> hangar credits window.
    if (TouchButton_OnTouchEnd_ote(*(int *)(s + 0x50 + 4), param_1) != 0) {
        RecordHandler *rh = (RecordHandler *)**(int **)g_ote_status;
        *(char *)(*(int *)g_ote_status + 0x4e) = 1;
        RecordHandler_saveOptions_ote(rh);
        if (I(s, 0x78) == 0) {
            HangarWindow *hw = (HangarWindow *)ModStation_opnew_ote(0x134);
            HangarWindow_ctor_ote(hw);
            I(s, 0x78) = (int)hw;
        }
        HangarWindow_initialize_ote();
        C(s, 0x18) = 1;
        C(s, 0x66) = 1;
        HangarWindow_showCreditsBuyWindow_ote((HangarWindow *)P(s, 0x78));
    }

    // the five DLC / station-menu buttons.
    I(s, 0x44) = -1;
    for (unsigned i = 0; i < 5; i = i + 1) {
        if (TouchButton_OnTouchEnd_ote(*(int *)(*(int *)(s + 0x8c) + i * 4), param_1) != 0) {
            I(s, 0x44) = (int)i;
            // dispatches via the main button's vtable thunk in the original.
            ModStation_ote_launchModule(I(s, 0), 0x10000);
            return;
        }
    }

    // the layout (system info box) hot area.
    if (Layout_OnTouchEndR_ote((Layout *)*help, param_1, param_2) != 0) {
        if (I(s, 0x50) == 0) {
            MenuTouchWindow *w = (MenuTouchWindow *)ModStation_opnew_ote(0x240);
            MenuTouchWindow_ctor_ote(w, 2);
            I(s, 0x50) = (int)w;
        }
        Status_checkForLevelUp_ote();
        C(s, 0x6e) = 1;
        ModStation_ote_cacheButtonPositions(self);
        return;
    }

    if (Layout_helpPressed_ote((Layout *)*help) != 0)
        Layout_initHelpWindow_ote(*help, GameText_getText_ote(**g_ote_textRoot));

    if (NewsTicker_OnTouchEnd_ote(I(s, 0x1c), param_1) == 0)
        ModStation_ote_kickIdleCamera(self);
}

// --- the full mission-completion + campaign-advance handler (reached on dialogue accept) ---
extern "C" void handleMissionComplete(ModStation *self)
{
    char *s = (char *)self;
    int *status = *(int **)g_ote_status;

    Mission *mission = (Mission *)I(s, 0x54);

    // mission-type specific cargo cleanup before scoring.
    if (mission != 0) {
        int type = Mission_getType_ote();
        int campaign = Mission_isCampaignMission_ote(mission);
        if (type == 8) {
            if (campaign == 0) {
                int ship = Status_getShip_ote();
                int good = Mission_getProductionGoodIndex_ote(mission);
                Mission_getProductionGoodAmount_ote();
                Ship_removeCargo_ote(ship, good);
                if (I(s, 0x78) != 0)
                    HangarWindow_initialize_ote();
            }
        } else if (campaign == 0 && Mission_getType_ote() == 0xb) {
            Status_setPassengers_ote(*status, 0);
            if (Mission_isCampaignMission_ote(mission) == 0)
                *(int *)(*status + 0xb8) += Mission_getProductionGoodAmount_ote();
            Status_getShip_ote();
            unsigned *cargo = (unsigned *)Ship_getCargo_ote();
            if (cargo != 0) {
                for (unsigned i = 0; i < *cargo; i = i + 1) {
                    if (Item_isUnsaleable_ote(*(Item **)(cargo[1] + i * 4)) != 0 &&
                        (Item_getIndex_ote(*(Item **)(cargo[1] + i * 4)) == 0x74 ||
                         Item_getIndex_ote(*(Item **)(cargo[1] + i * 4)) == 0x75)) {
                        Ship_removeCargo1_ote((Item *)Status_getShip_ote());
                        if (I(s, 0x78) != 0)
                            HangarWindow_initialize_ote();
                        break;
                    }
                }
            }
        } else if (Mission_getType_ote() != 3 && Mission_getType_ote() != 5 &&
                   Mission_getType_ote() != 0xb) {
            if (Mission_getType_ote() == 0) {
                Status_getShip_ote();
                unsigned *cargo = (unsigned *)Ship_getCargo_ote();
                if (cargo != 0) {
                    for (unsigned i = 0; i < *cargo; i = i + 1) {
                        if (Item_isUnsaleable_ote(*(Item **)(cargo[1] + i * 4)) != 0 &&
                            (Item_getIndex_ote(*(Item **)(cargo[1] + i * 4)) == 0x74 ||
                             Item_getIndex_ote(*(Item **)(cargo[1] + i * 4)) == 0x75)) {
                            Ship_removeCargo1_ote((Item *)Status_getShip_ote());
                            if (I(s, 0x78) != 0)
                                HangarWindow_initialize_ote();
                            break;
                        }
                    }
                }
                *(int *)(*status + 0x9c) += Mission_getProductionGoodAmount_ote();
            } else if (Mission_getType_ote() == 0xe) {
                Status_getShip_ote();
                unsigned *cargo = (unsigned *)Ship_getCargo_ote();
                if (cargo != 0) {
                    for (unsigned i = 0; i < *cargo; i = i + 1) {
                        if (Item_getIndex_ote(*(Item **)(cargo[1] + i * 4)) == 0x73) {
                            Ship_removeCargo1_ote((Item *)Status_getShip_ote());
                            if (I(s, 0x78) != 0)
                                HangarWindow_initialize_ote();
                            break;
                        }
                    }
                }
            }
        }
    }

    // --- scoring / campaign advance (LAB_000e7962) ---
    if (Mission_isCampaignMission_ote(mission) == 0) {
        Status_incMissionCount_ote(*status);
        int reward = Mission_getReward_ote();
        int bonus = Mission_getBonus_ote();
        Layout_showMissionRewardMessage_ote(*help_layout(), (char)(bonus + reward));
        finishMissionReward(self);
        return;
    }

    int camp = Status_getCurrentCampaignMission_ote();
    if (camp == 0x2b) {
        // mining-plant unlock -> supernova radio cutscene.
        Status_removeMission_ote(*status);
        Status_setMission_ote(*status);
        C(s, 0x69) = 0;
        int snd = **(int **)g_ote_sound;
        FModSound_stop_ote(snd);
        FModSound_play_ote(snd, 0x90, 0, 0.0f);
        ModStation_ote_startRadioCutscene(self);
        return;
    }

    if (Status_getCurrentCampaignMission_ote() != 0x94)
        Status_nextCampaignMission_ote(*status);

    int cm = Status_getCurrentCampaignMission_ote();
    unsigned d = (unsigned)(cm - 0x2c);
    bool handled = false;
    if (d < 0xf) {
        if ((1 << (d & 0xff) & 0x5830) != 0) {
            int cs = I(s, 0x14);
            Status_getShip_ote();
            int shipIndex = Ship_getIndex_ote();
            Ship *sh = (Ship *)Status_getShip_ote();
            Ship_getRace_ote(sh);
            CutScene_replacePlayerShip_ote(cs, shipIndex);
            finishMissionReward(self);
            return;
        }
        if (d == 1) {
            Status_removeMission_ote(*status);
            I(s, 0x54) = 0;
            Status_setMission_ote(*status);
            I(s, 0x44) = 1;
            C(s, 0x69) = 0;
            ModStation_ote_launchModule(I(s, 0), 0x10000);
            return;
        }
        if (d != 0)
            handled = false;     // fall through to the 0x4b.. handling
        else
            handled = true;      // d == 0 (mission 0x2c): handled by the 0x9 reset path below
    }

    if (!handled) {
        if ((unsigned)(cm - 0x4b) > 8 || (1 << ((cm - 0x4b) & 0xff) & 0x103) == 0) {
            handleCampaignTransition(self, cm);
            return;
        }
    }

    // missions 0x2c / 9 / 0x4b / 0x4c / 0x53: launch the storyline cutscene module.
    Status_removeMission_ote(*status);
    I(s, 0x54) = 0;
    Status_setMission_ote(*status);
    C(s, 0x69) = 0;
    *(int *)(*status + 0 /* cutscene slot */) = 0;
    ModStation_ote_launchModule(**(int **)g_ote_module, 5);
}

// per-mission campaign station/ship side effects, then either restock-and-reward or depart.
extern "C" void handleCampaignTransition(ModStation *self, int cm)
{
    char *s = (char *)self;
    int *status = *(int **)g_ote_status;

    if (cm == 0x12) {
        Status_removeMission_ote(*status);
        C(s, 0x69) = 0;
        I(s, 0x54) = 0;
        return;
    }

    Station *here = (Station *)Status_getStation_ote();
    int hereIdx = Station_getIndex_ote(here);

    if (cm == 0x4d && hereIdx == 100) {
        Ship *sh = (Ship *)Status_getStation_ote();
        Ship_makeShip_ote(*(int *)(*(int *)(**(int **)g_ote_shipTable + 4) + 0x94));
        Station_addShip_ote(sh);
        finishMissionReward(self);
        return;
    }
    if (cm == 0x4e) {
        *(int *)(*status + 0x5c) = -1;
        *(int *)(*status + 0x60) = -1;
        *(int *)(*status + 0x64) = -1;
        *(int *)(*status + 0x68) = -1;
        Achievements_resetNewMedals_ote((void *)**(int **)g_ote_achievements);
        Station *home = (Station *)*status;
        Status_getStation_ote();
        Status_departStation_ote(home);
        **(int **)g_ote_module = 1;
        ApplicationManager_SetCurrentApplicationModule_ote(**(int **)g_ote_module);
        C(s, 0x24) = 0;
        finishMissionReward(self);
        return;
    }
    Station *st = (Station *)Status_getStation_ote();
    if (cm == 0x54 && Station_getIndex_ote(st) == 100) {
        Ship *sh = (Ship *)Status_getStation_ote();
        if (Station_hasShip_ote((int)sh) == 0) {
            Ship_makeShip_ote(*(int *)(*(int *)(**(int **)g_ote_shipTable + 4) + 0x98));
            Station_addShip_ote(sh);
        }
        if (Station_hasShip_ote((int)sh) == 0) {
            Ship_makeShip_ote(*(int *)(*(int *)(**(int **)g_ote_shipTable + 4) + 0xa0));
            Station_addShip_ote(sh);
        }
        Item_makeItem_ote(*(int *)(*(int *)(**(int **)g_ote_itemTable + 4) + 0x224));
        Ship_addCargo_ote((Item *)Status_getShip_ote());
        finishMissionReward(self);
        return;
    }

    // the remaining campaign ids each set a module flag and depart.
    if (cm == 0xa0 || cm == 0x90 || cm == 99 || cm == 0x77 || cm == 0x85 || cm == 0x6d ||
        cm == 0x59) {
        Station *home = (Station *)*status;
        **(char **)g_ote_module = 1;
        Galaxy_getStation_ote(**(int **)g_ote_galaxy);
        Status_departStation_ote(home);
        ApplicationManager_SetCurrentApplicationModule_ote(**(int **)g_ote_module);
        C(s, 0x24) = 0;
        return;
    }
    if (cm == 0x68) {
        if (P(s, 0x78) != 0) {
            HangarWindow_dtor_ote((HangarWindow *)P(s, 0x78));
            ModStation_opdelete_ote(P(s, 0x78));
        }
        C(s, 0x66) = 0;
        I(s, 0x78) = 0;
        finishMissionReward(self);
        return;
    }
    if (cm == 0x80) {
        Status_activateNewWanted_ote();
        if (*(char *)(*(int *)g_ote_status + 0x2a) == 0)
            C(s, 0x6e) = 1;
        finishMissionReward(self);
        return;
    }

    finishMissionReward(self);
}

// credits the reward, removes the mission and refreshes the lounge.
extern "C" void finishMissionReward(ModStation *self)
{
    char *s = (char *)self;
    int *status = *(int **)g_ote_status;

    Mission_getReward_ote();
    int bonus = Mission_getBonus_ote();
    if ((unsigned)bonus >= 0)
        Status_getCurrentCampaignMission_ote();
    Status_changeCredits_ote(*status);
    Status_removeMission_ote(*status);
    I(s, 0x54) = 0;
    if (I(s, 0x74) != 0)
        SpaceLounge_refresh_ote();
    C(s, 0x69) = 0;
    ModStation_checkHints_ote(self);
}
