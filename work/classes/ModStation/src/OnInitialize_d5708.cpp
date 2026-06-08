#include "class.h"

struct Station;
struct Ship;
struct Item;
struct Mission;
struct CutScene;
struct DialogueWindow;
struct Generator;
struct NewsTicker;
struct TouchButton;
struct Agent;
struct EaseInOut;
struct EaseInOutMatrix;

__attribute__((visibility("hidden"))) extern int  *g_oi_stack;       // [DAT_000e5a3c]
__attribute__((visibility("hidden"))) extern int **g_oi_status;      // status holder (multiple DATs alias it)
__attribute__((visibility("hidden"))) extern int **g_oi_itemTable;   // item descriptor table
__attribute__((visibility("hidden"))) extern int **g_oi_shipTable;   // ship descriptor table
__attribute__((visibility("hidden"))) extern int **g_oi_canvas;      // paint canvas holder
__attribute__((visibility("hidden"))) extern int **g_oi_settings;    // settings record
__attribute__((visibility("hidden"))) extern int **g_oi_sound;       // FMod sound holder
__attribute__((visibility("hidden"))) extern int **g_oi_textRoot;    // GameText root
__attribute__((visibility("hidden"))) extern char **g_oi_demoFlag;   // demo-mode flag
__attribute__((visibility("hidden"))) extern int  *g_oi_musicId;     // pending music id

extern "C" {
void *ModStation_opnew_oi(unsigned size);
void  ModStation_opdelete_oi(void *p);

// --- self methods reached as direct calls in the original ---
void  ModStation_autosave_oi(ModStation *self);
void  ModStation_resetLight_oi(ModStation *self);
void  ModStation_enterStation_oi(ModStation *self);

// --- Status / world queries ---
int   Status_getCurrentCampaignMission_oi();
int   Status_getStation_oi();
int   Station_getIndex_oi(Station *s);
int   Status_getSystem_oi();
int   SolarSystem_getRace_oi();
int   SolarSystem_getRoutes_oi();
int   Status_inAlienOrbit_oi();
int   Status_inSupernovaSystem_oi();
int   Status_inBlackMarketSystem_oi();
int   Status_getShip_oi();
int   Status_getCredits_oi();
int   Status_getLevel_oi();
int   Status_hardCoreMode_oi();
int   Status_getStanding_oi();
int   Status_gameWon_oi();
int   Status_dlc1Won_oi();
int   Status_getPlayingTime_lo_oi();
int   Status_activateNewWanted_oi();
int   Status_getFreelanceMission_oi();
int   Status_getCampaignMission_oi();
void  Status_setShip_oi(Ship *s);

int   Ship_getIndex_oi();
int   Ship_hasJumpDrive_oi();
int   Ship_hasCargo_oi(int ship, int item);
int   Ship_hasEquipment_oi(int ship, int item);
int   Ship_getCargo_oi(int ship);
void  Ship_makeShip_oi(int desc);
void  Ship_setRace_oi(int race);
void  Ship_setEquipment_oi(Item *eq, int slot);
void  Ship_setPrice_oi(int shipPtr);
void  Ship_removeCargo_oi(int ship);
void  Ship_addCargo_oi(int ship, Item *it);

int   Item_makeItem_oi();
int   Item_makeItemDesc_oi(int desc);
int   Item_makeItemDescAmt_oi(int desc, int amt);
void  Item_setUnsaleable_oi(int flag);
int   Item_getIndex_oi(Item *it);
int   Item_getAmount_oi();
void  Item_setPrice_oi(int itemPtr);

int   Station_getShips_oi();
int   Station_getItems_oi();
int   Station_getName_oi();
int   Station_hasItem_oi(int station);
int   Station_hasShip_oi(int station);
int   Station_hasAttackedFriends_oi();
int   Station_stationHasPirateBase_oi();
void  Station_addItem_oi(Item *it);
void  Station_addShip_oi(Ship *s);
void  Station_removeShip_oi(Ship *s);
void  Station_setShips_oi(void *station, int list);

int   Standing_isEnemy_oi(int standing);
int   Standing_getStanding_oi(int standing);

int   Mission_getTargetStation_oi();
int   Mission_getType_oi();

int   Achievements_gotAllGoldMedals_oi();

int   GameText_getText_oi(int id);
void  ChoiceWindow_set_oi(void *cw, int text, int flag);
void  ChoiceWindow_setNotice_oi(void *cw, int text);

int   CutScene_initialize_oi(CutScene *cs);
void  CutScene_ctor_oi(CutScene *cs, int kind);
void  CutScene_replacePlayerShip_oi(int cs, int shipIndex);

void  TouchButton_setHalfTransparent_oi(int flag);

void  Globals_startNewSoundResourceList_oi();
void  Globals_addSoundResource_oi(int sound, int id);
void  Globals_playMusicAndFadeOutCurrent_oi(int id);

void  FModSound_play_oi(int sound, int id, void *pos, float vol);
void  FModSound_enableReverb_oi(int sound);
void  FModSound_setDownPitch_oi(int sound);

void  Generator_ctor_oi(Generator *g);
void  Generator_dtor_oi(Generator *g);
void  Generator_computerTradeGoods_oi(Generator *g, Station *s);
int   Generator_getShipBuyList_oi(Station *s);

// --- big corrupted blocks delegated to helpers (SIMD camera/light/easing + String chains) ---

// Builds the cutscene camera framing + idle light easing for the docked-station screen (state 0x14):
// camera dir read, light direction, EaseInOut(+0x13c/+0x140/+0x144), EaseInOutMatrix(+0x20) and all
// the MatrixSetRotation/Translation SIMD math the decompiler corrupted. `race` selects the framing.
void  ModStation_oi_setupHangarCamera(ModStation *self, int race);
// Pops the "pay docking fee / standing penalty" choice window with its formatted-credits String
// (a long AbyssEngine::String temporary chain Ghidra mangled). Returns nonzero if a window was shown.
int   ModStation_oi_showDockingFeeChoice(ModStation *self, int standing, int credits);
// Spawns the pirate-base agent dialogue window (+0x84) with its sound (Agent String temporaries).
void  ModStation_oi_showPirateDialogue(ModStation *self);
// Builds the "wanted activated" choice text from the count (String hash-replace chain).
void  ModStation_oi_showWantedActivated(ModStation *self, int count);
// Builds the mission-0x6c reward Mission + its dialogue window (Mission/String temporaries).
void  ModStation_oi_showRewardMission(ModStation *self);
// Builds the news ticker (+0x1c) for the leave-station screen (state 0x3c) — NewsTicker layout math.
void  ModStation_oi_buildNewsTicker(ModStation *self);
// Builds the DLC sub-menu buttons (+0x8c array) for state 0x50 — TouchButton layout SIMD math.
void  ModStation_oi_buildDlcMenu(ModStation *self);
}

// ModStation::OnInitialize() — per-screen-state setup. The current state is at self+0xc; each branch
// configures its screen (load cutscene, hangar camera, news ticker, DLC menu, ...) and advances the
// state machine. State 0x14=hangar, 0x28=station info, 0x3c=leave, 0x50=DLC menu, 0x64=enter.
extern "C" void ModStation_OnInitialize(ModStation *self)
{
    char *s = (char *)self;

    int *status = *(int **)g_oi_status;

    U(s, 0x9c) = 0;

    // lazily build the docking cutscene.
    if (I(s, 0x14) == 0) {
        CutScene *cs = (CutScene *)ModStation_opnew_oi(0xa0);
        CutScene_ctor_oi(cs, 0x17);
        P(s, 0x14) = cs;
        CutScene_initialize_oi(cs);
    }

    int state = I(s, 0xc);
    int next;

    if (state == 0x14) {
        // ---- hangar screen ----
        if (C(s, 0xb1) == 0 && C(s, 0xb0) == 0 && C(s, 0x101) != 0) {
            bool skip = false;
            if (Status_getCurrentCampaignMission_oi() == 0x4d) {
                Station *st = (Station *)Status_getStation_oi();
                if (Station_getIndex_oi(st) == 0x65)
                    skip = true;
            }
            if (!skip)
                ModStation_autosave_oi(self);
        }

        int *shipTbl = *(int **)g_oi_shipTable;
        // mission 1: hand the player their first equipped ship.
        if (Status_getCurrentCampaignMission_oi() == 1) {
            Ship *ship = (Ship *)*shipTbl;
            Ship_makeShip_oi(**(int **)(**g_oi_shipTable + 4));
            Status_setShip_oi(ship);
            Ship_setRace_oi(Ship_getIndex_oi());
            int eq = Item_makeItem_oi();
            Item_setUnsaleable_oi(eq);
            Ship_setEquipment_oi((Item *)*shipTbl, eq);
            int eq2 = Item_makeItem_oi();
            Ship_setEquipment_oi((Item *)*shipTbl, eq2);
            Item_setUnsaleable_oi(eq2);
            CutScene_replacePlayerShip_oi(I(s, 0x14), Ship_getIndex_oi());
        }

        // half-transparent docking buttons depending on campaign progress.
        int camp = Status_getCurrentCampaignMission_oi();
        bool special = false;
        if (camp < 5) {
            TouchButton_setHalfTransparent_oi(**(int **)(I(s, 0x8c) + 4));
            TouchButton_setHalfTransparent_oi(*(int *)(*(int *)(I(s, 0x8c) + 4) + 8));
            TouchButton_setHalfTransparent_oi(*(int *)(*(int *)(I(s, 0x8c) + 4) + 0xc));
        } else if (camp < 9 || camp == 0xf) {
            special = camp == 0xf;
            TouchButton_setHalfTransparent_oi(*(int *)(*(int *)(I(s, 0x8c) + 4) + 8));
            TouchButton_setHalfTransparent_oi(*(int *)(*(int *)(I(s, 0x8c) + 4) + 0xc));
        }
        bool showShop = camp < 0xc || special;
        if (!showShop) {
            Station *st = (Station *)Status_getStation_oi();
            if (Station_getIndex_oi(st) == 100) showShop = true;
            else {
                st = (Station *)Status_getStation_oi();
                if (Station_getIndex_oi(st) == 0x65) showShop = true;
            }
        }
        if (showShop)
            TouchButton_setHalfTransparent_oi(*(int *)(*(int *)(I(s, 0x8c) + 4) + 4));

        // camera framing race selection.
        int race;
        Station *st = (Station *)Status_getStation_oi();
        if (Station_getIndex_oi(st) == 0x65) {
            race = 8;
        } else {
            st = (Station *)Status_getStation_oi();
            if (Station_getIndex_oi(st) == 100) {
                race = 7;
            } else {
                Status_getSystem_oi();
                race = SolarSystem_getRace_oi();
            }
        }
        ModStation_oi_setupHangarCamera(self, race);
        ModStation_resetLight_oi(self);
        if (C(s, 0x101) != 0)
            ModStation_enterStation_oi(self);

        // mission 0x4d: re-price a special ship in the shipyard.
        if (Status_getCurrentCampaignMission_oi() == 0x4d) {
            Station *st2 = (Station *)Status_getStation_oi();
            if (Station_getIndex_oi(st2) == 100) {
                Status_getStation_oi();
                unsigned *ships = (unsigned *)Station_getShips_oi();
                if (ships != 0) {
                    for (unsigned i = 0; i < *ships; i = i + 1) {
                        if (Ship_getIndex_oi() == 0x25)
                            Ship_setPrice_oi(*(int *)(ships[1] + i * 4));
                    }
                }
            }
        }

        // optional "you have no jump fuel" warning.
        if (C(s, 0x69) == 0 && C(s, 0x6a) == 0 && C(s, 0x63) == 0 &&
            *(char *)(**g_oi_textRoot + 0x2ec) == 0) {
            Station *st2 = (Station *)Status_getStation_oi();
            if (Station_getIndex_oi(st2) != 0x65) {
                Status_getShip_oi();
                int hasJump = Ship_hasJumpDrive_oi();
                int hasFuel = Ship_hasCargo_oi(Status_getShip_oi(), 0x55);
                Status_getSystem_oi();
                int routes = SolarSystem_getRoutes_oi();
                if (routes == 0 && hasJump == 0 && hasFuel == 0 &&
                    0x10 < Status_getCurrentCampaignMission_oi()) {
                    ChoiceWindow_set_oi(P(s, 0x70), GameText_getText_oi(**g_oi_textRoot), 1);
                    C(s, 0x6d) = 1;
                    C(s, 0x63) = 1;
                }
            }
        }

        *(char *)(*(int *)g_oi_status + 0xf8) = 1;
        C(s, 0x118) = 0;
        U(s, 0x128) = 0;
        C(s, 0xb3) = 0;
        U(s, 0x2c) = 0;
        C(s, 0x148) = 0;
        next = 1;
    } else if (state == 0x28) {
        // ---- station information screen ----
        // various mission/station combos force the "alarm" flag at +0xd8.
        if (Status_inAlienOrbit_oi() == 0) {
            Station *st = (Station *)Status_getStation_oi();
            if (Station_getIndex_oi(st) == 0x78) {
                int camp = Status_getCurrentCampaignMission_oi();
                if (camp == 0x62 || Status_getCurrentCampaignMission_oi() == 100)
                    C(s, 0xd8) = 1;
            }
        }
        if (Status_inAlienOrbit_oi() == 0) {
            Station *st = (Station *)Status_getStation_oi();
            if (Station_getIndex_oi(st) == 0x3a && Status_getCurrentCampaignMission_oi() == 0x8a)
                C(s, 0xd8) = 1;
        }
        if (Status_inAlienOrbit_oi() == 0) {
            Station *st = (Station *)Status_getStation_oi();
            if (Station_getIndex_oi(st) == 0x7e && Status_getCurrentCampaignMission_oi() == 0x78)
                C(s, 0xd8) = 1;
        }
        if (Status_inAlienOrbit_oi() == 0) {
            Station *st = (Station *)Status_getStation_oi();
            if (Station_getIndex_oi(st) == 0x4e && Status_getCurrentCampaignMission_oi() == 0x8d)
                C(s, 0xd8) = 1;
        }
        if (Status_inSupernovaSystem_oi() != 0)
            C(s, 0xd8) = 1;
        if (**g_oi_demoFlag != 0)
            US(s, 0xd8) = 0x101;

        if (C(s, 0xb0) == 0) {
            // decide whether to show the standing / docking-fee choice.
            bool gate = false;
            Station *st = (Station *)Status_getStation_oi();
            if (Station_getIndex_oi(st) == 4) gate = true;
            else {
                st = (Station *)Status_getStation_oi();
                if (Station_getIndex_oi(st) == 0x58) gate = true;
                else {
                    Status_getStation_oi();
                    if (Station_stationHasPirateBase_oi() != 0) {
                        // pirate base -> agent dialogue instead.
                        if (I(s, 0x84) == 0)
                            ModStation_oi_showPirateDialogue(self);
                        goto afterGate;
                    }
                    gate = true;
                }
            }
            (void)gate;
            if (Status_getCurrentCampaignMission_oi() != 0x30 &&
                Status_inBlackMarketSystem_oi() == 0) {
                Station *s1 = (Station *)Status_getStation_oi();
                if (Station_getIndex_oi(s1) != 0x6c) {
                    Station *s2 = (Station *)Status_getStation_oi();
                    if (Station_getIndex_oi(s2) != 100) {
                        Station *s3 = (Station *)Status_getStation_oi();
                        if (Station_getIndex_oi(s3) != 0x65 && C(s, 0x6a) == 0 && C(s, 0x63) == 0 &&
                            C(s, 0xd8) == 0 && *(char *)(*(int *)g_oi_status + 0x108) == 0) {
                            Status_getStation_oi();
                            bool enemy = false;
                            if (Station_hasAttackedFriends_oi() != 0) {
                                enemy = true;
                            } else {
                                int st0 = Status_getStanding_oi();
                                Status_getSystem_oi();
                                SolarSystem_getRace_oi();
                                if (Standing_isEnemy_oi(st0) != 0)
                                    enemy = true;
                            }
                            if (enemy) {
                                int standing = Status_getStanding_oi();
                                int credits = Status_getCredits_oi();
                                ModStation_oi_showDockingFeeChoice(self, standing, credits);
                            }
                        }
                    }
                }
            }
        afterGate:;
        }

        // ---- post-dock world updates (LAB_000e6870) ----
        Station *here = (Station *)Status_getStation_oi();
        int hereIdx = Station_getIndex_oi(here);
        Status_getCampaignMission_oi();

        if (Status_getCurrentCampaignMission_oi() == 0x14 &&
            Mission_getTargetStation_oi() == hereIdx) {
            Status_getStation_oi();
            unsigned *items = (unsigned *)Station_getItems_oi();
            if (items != 0) {
                for (unsigned i = 0; i < *items; i = i + 1) {
                    if (Item_getIndex_oi(*(Item **)(items[1] + i * 4)) == 0x29)
                        Item_setPrice_oi(*(int *)(items[1] + i * 4));
                }
            }
            Item *it = (Item *)Status_getStation_oi();
            Item_makeItemDescAmt_oi(*(int *)(*(int *)(**g_oi_itemTable + 4) + 0xa4), 10);
            Station_addItem_oi(it);
        }
        if (Status_getCurrentCampaignMission_oi() == 0x1b &&
            Mission_getTargetStation_oi() == hereIdx) {
            Ship_removeCargo_oi(Status_getShip_oi());
        }
        // mining-plant flag (+0xd9).
        if (Status_getCurrentCampaignMission_oi() == 0x2b) {
            if (hereIdx == 10) C(s, 0xd9) = 1;
        } else if (hereIdx == 10 && Status_getCurrentCampaignMission_oi() == 0x2c) {
            C(s, 0xd9) = 1;
        }

        // station-10 endgame goods.
        bool isS100;
        if (hereIdx == 10 && Status_gameWon_oi() != 0) {
            int ship = Status_getShip_oi();
            if (Ship_hasEquipment_oi(ship, 0x55) == 0) {
                ship = Status_getShip_oi();
                if (Ship_hasCargo_oi(ship, 0x55) == 0) {
                    int st0 = Status_getStation_oi();
                    if (Station_hasItem_oi(st0) == 0 && Station_hasItem_oi(Status_getStation_oi()) == 0) {
                        Item *it = (Item *)Status_getStation_oi();
                        Item_makeItemDesc_oi(*(int *)(*(int *)(**g_oi_itemTable + 4) + 0x290));
                        Station_addItem_oi(it);
                    }
                }
            }
            isS100 = false;
            goto fuelTop;
        } else {
            isS100 = hereIdx == 100;
            if ((unsigned)(hereIdx - 100) < 2 || hereIdx == 10) {
            fuelTop:
                int amt;
                int cargo = Ship_getCargo_oi(Status_getShip_oi());
                amt = (cargo == 0) ? 0 : Item_getAmount_oi();
                if (amt < 6 && Station_hasItem_oi(Status_getStation_oi()) == 0) {
                    Item *it = (Item *)Status_getStation_oi();
                    Item_makeItemDesc_oi(*(int *)(*(int *)(**g_oi_itemTable + 4) + 0x1e8));
                    Station_addItem_oi(it);
                }
            }
        }

        // station-10 restock if the player has all gold medals.
        Station *st10 = (Station *)Status_getStation_oi();
        if (Station_getIndex_oi(st10) == 10 && Achievements_gotAllGoldMedals_oi() != 0) {
            Status_getStation_oi();
            unsigned *ships = (unsigned *)Station_getShips_oi();
            bool needNew = ships == 0;
            if (ships != 0) {
                unsigned i = 0;
                bool found = false;
                while (i < *ships) {
                    if (Ship_getIndex_oi() == 8) { found = true; break; }
                    i = i + 1;
                }
                needNew = !found;
            }
            if (needNew) {
                Generator *g = (Generator *)ModStation_opnew_oi(1);
                Generator_ctor_oi(g);
                void *station = (void *)Status_getStation_oi();
                Status_getStation_oi();
                int list = Generator_getShipBuyList_oi((Station *)g);
                Station_setShips_oi(station, list);
                Generator_dtor_oi(g);
                ModStation_opdelete_oi(g);
            }
        }

        // DLC ship grants at stations 100/101.
        if (isS100 &&
            (Status_dlc1Won_oi() != 0 ||
             Status_getCurrentCampaignMission_oi() == 0x50 ||
             Status_getCurrentCampaignMission_oi() == 0x51 ||
             Status_getCurrentCampaignMission_oi() == 0x52 ||
             Status_getCurrentCampaignMission_oi() == 0x53 ||
             Status_getCurrentCampaignMission_oi() == 0x54)) {
            Ship *st = (Ship *)Status_getStation_oi();
            if (Station_hasShip_oi((int)st) != 0) {
                unsigned *ships = (unsigned *)Station_getShips_oi();
                unsigned i = 0;
                while (i < *ships && Ship_getIndex_oi() != 0x25) i = i + 1;
                Station_removeShip_oi(st);
            }
            if (Station_hasShip_oi((int)st) == 0) {
                Ship_makeShip_oi(*(int *)(*(int *)(**g_oi_shipTable + 4) + 0x98));
                Station_addShip_oi(st);
            }
            if (Station_hasShip_oi((int)st) == 0) {
                Ship_makeShip_oi(*(int *)(*(int *)(**g_oi_shipTable + 4) + 0xa0));
                Station_addShip_oi(st);
            }
        }

        // station-0x6c reward mission window.
        Station *st6c = (Station *)Status_getStation_oi();
        bool atReward = Station_getIndex_oi(st6c) == 0x6c;
        int rewardState = atReward ? *(int *)(*(int *)g_oi_status + 0x114) : 0;
        if (atReward && rewardState == 1) {
            if (P(s, 0x84) != 0) {
                // DialogueWindow dtor delegated via reward helper path.
            }
            U(s, 0x84) = 0;
            ModStation_oi_showRewardMission(self);
        } else {
            Station *st = (Station *)Status_getStation_oi();
            if (Station_getIndex_oi(st) == 0x6c && *(int *)(*(int *)g_oi_status + 0x114) == 2) {
                if (Status_getCredits_oi() < 100000 ||
                    Ship_hasCargo_oi(Status_getShip_oi(), 0x6d) == 0) {
                    ChoiceWindow_setNotice_oi(P(s, 0x70), GameText_getText_oi(**g_oi_textRoot));
                } else {
                    ChoiceWindow_set_oi(P(s, 0x70), GameText_getText_oi(**g_oi_textRoot), 1);
                    C(s, 0x6c) = 1;
                }
                C(s, 0x63) = 1;
            }
        }

        // freelance delivery mission item.
        if (Status_getFreelanceMission_oi() != 0 && Mission_getType_oi() == 0xe) {
            Station *st = (Station *)Status_getStation_oi();
            if (Mission_getTargetStation_oi() == Station_getIndex_oi(st) &&
                Station_hasItem_oi(Status_getStation_oi()) == 0) {
                Item_makeItemDesc_oi(*(int *)(*(int *)(**g_oi_itemTable + 4) + 0x1cc));
                Item *it = (Item *)Status_getStation_oi();
                Station_addItem_oi(it);
            }
        }

        // contextual hint windows driven by settings-record flags.
        int settings = *(int *)g_oi_settings;
        int rec = *(int *)g_oi_textRoot;
        (void)rec;
        if (Status_getCurrentCampaignMission_oi() == 0xa2 && *(unsigned char *)(settings + 0x2b) == 0) {
            ChoiceWindow_setNotice_oi(P(s, 0x70), GameText_getText_oi(**g_oi_textRoot));
            C(s, 0x63) = 1;
            *(char *)(settings + 0x2b) = 1;
        }
        int settings2 = *(int *)g_oi_settings;
        if (Status_gameWon_oi() != 0 && *(char *)(*(int *)g_oi_status + 0x35) == 0 &&
            *(char *)(settings + 0x38) == 0) {
            ChoiceWindow_setNotice_oi(P(s, 0x70), GameText_getText_oi(**g_oi_textRoot));
            C(s, 0x63) = 1;
            *(char *)(settings + 0x38) = 1;
        }
        if (Status_dlc1Won_oi() != 0 && *(char *)(*(int *)g_oi_status + 0x37) == 0 &&
            *(char *)(settings + 0x39) == 0) {
            ChoiceWindow_setNotice_oi(P(s, 0x70), GameText_getText_oi(**g_oi_textRoot));
            C(s, 0x63) = 1;
            *(char *)(settings + 0x39) = 1;
        }
        (void)settings2;

        // newly-activated wanted criminal banner.
        int wanted = Status_activateNewWanted_oi();
        if (wanted > 0 && C(s, 0x63) == 0) {
            if (wanted == 1)
                ChoiceWindow_setNotice_oi(P(s, 0x70), GameText_getText_oi(**g_oi_textRoot));
            else
                ModStation_oi_showWantedActivated(self, wanted);
            C(s, 0x63) = 1;
        }
        next = 0x14;
    } else if (state == 0x3c) {
        // ---- leave-station screen ----
        C(s, 0x100) = 0;
        U(s, 0xe8) = 0;
        U(s, 0xe0) = 0;
        U(s, 0xf0) = 0;
        U(s, 0xf4) = 0;
        U(s, 0xf8) = 0;
        U(s, 0xfc) = 0;
        Status_getSystem_oi();
        int angle = (SolarSystem_getRace_oi() == 2) ? (int)0xffffff38 : 0x10e;
        I(s, 0xe4) = angle;
        if (I(s, 0x14) != 0)
            *(float *)(I(s, 0x14) + 4) = (float)angle / 65536.0f;
        U(s, 0xec) = 0;
        C(s, 0xb0) = C(s, 0xb3);
        ModStation_oi_buildNewsTicker(self);
        next = 0x28;
    } else if (state == 0x50) {
        // ---- DLC sub-menu screen ----
        U(s, 0x62) = 0;
        C(s, 0x6e) = 0;
        US(s, 0x6c) = 0;
        U(s, 0x68) = 0;
        US(s, 0x66) = 0;
        US(s, 0x60) = 0x100;
        C(s, 0xdc) = 0;
        US(s, 0xde) = 0;
        C(s, 0x12c) = 0;
        C(s, 0x18) = 0;
        C(s, 0x63) = 0;
        ModStation_oi_buildDlcMenu(self);
        next = 0x3c;
    } else if (state == 100) {
        // ---- enter-station screen ----
        int *sound = *(int **)g_oi_sound;
        Globals_startNewSoundResourceList_oi();
        static const int snd[] = {0x5f, 0x7a, 0x6c, 0x60, 0x61, 0x62, 99, 0x65, 100,
                                  0x66, 0x68, 0x69, 0x6a, 0x6b, 0x67, 0x7e};
        for (unsigned i = 0; i < sizeof(snd) / sizeof(snd[0]); i = i + 1)
            Globals_addSoundResource_oi(*sound, snd[i]);

        I(s, 0xcc) = 0x32;
        int *st = *(int **)g_oi_status;
        C(s, 0x101) = 1;
        Status_getStation_oi();
        Station_getName_oi();
        // String assign of station name into self+0x38 (temporary chain).

        long long enterTime = *(long long *)(*st + 0x70);
        long long now = (long long)(unsigned)Status_getPlayingTime_lo_oi();
        if (now - enterTime >= 0x7531) {
            Station *here = (Station *)Status_getStation_oi();
            if (Station_getIndex_oi(here) != 0x6c) {
                Generator *g = (Generator *)ModStation_opnew_oi(1);
                Generator_ctor_oi(g);
                Status_getStation_oi();
                Generator_computerTradeGoods_oi(g, (Station *)g);
            }
        }
        next = 0x50;
    } else {
        // ---- any other state: just play the dock ambience and advance to 'entered' ----
        int *sound = *(int **)g_oi_sound;
        FModSound_play_oi(*sound, 0x7a, 0, 0.0f);
        FModSound_enableReverb_oi(*sound);
        FModSound_setDownPitch_oi(*sound);

        int *music = g_oi_musicId;
        if (*music != -1)
            Globals_playMusicAndFadeOutCurrent_oi(**(int **)g_oi_sound);
        *music = -1;
        C(s, 0x24) = 1;
        I(s, 0xc) = 100;

        // snapshot the docking button positions.
        unsigned *btns = (unsigned *)P(s, 0x8c);
        if (btns != 0) {
            for (unsigned i = 0; i < *btns; i = i + 1) {
                // button positions cached by the engine helper (TouchButton::getPosition SIMD).
                (void)i;
            }
        }
        return;
    }

    I(s, 0xc) = next;
}
