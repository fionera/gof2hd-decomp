#include "class.h"

extern "C" {
extern void *__stack_chk_guard;
__attribute__((noreturn)) void __stack_chk_fail(int diff) noexcept;

void *operator_new(uint32_t size);

int  WantedWindow_OnTouchEnd(void *win, int y);
int  ChoiceWindow_OnTouchEnd(void *win, int y);
void ChoiceWindow_set(void *win, void *text, bool b);
int  ScrollTouchWindow_OnTouchEnd(void *win, int y);
int  StarMap_OnTouchEnd(void *map, int y);
void StarMap_ctor(void *map, bool a, void *mission, bool b, int c);
void StarMap_init(void *map);

int  TouchButton_OnTouchEnd(void *btn, int y);

int  Status_wantedBoardAccessible();
void *Status_getCampaignMission();
void *Status_getFreelanceMission();
void  Status_setFreelanceMission(void *st);
void *Status_getShip();
void  Status_setPassengers(void *st, int n);

void *Ship_getCargo(void *ship);
void  Ship_removeCargo(void *ship, void *item);

int  Item_isUnsaleable(void *item);
int  Item_getIndex(void *item);

int  Mission_getType(void *mission);
void *Mission_getAgent(void *mission);

int  Agent_isGenericAgent(void *agent);
void Agent_setOfferAccepted(void *agent, bool accepted);

void *GameText_getText(int id);
void String_fromText(void *s, void *text, bool copy);
void String_dtor(void *s);

void *ApplicationManager_GetApplicationModule(void *appMgr);

int  Layout_OnTouchEnd(void *layout, int y, int z);
int  Layout_helpPressed(void *layout);
void Layout_initHelpWindow(void *layout, void *title);
void Layout_resetWindowDimensions(void *layout);

void MissionsWindow_init(void *self);

extern void *g_mwt_freelanceSrc;  // *(DAT_161174): freelance mission source
extern void *g_mwt_appMgr;        // *(DAT_16119c / 1611a8): application manager
extern void *g_mwt_campMod;       // *(DAT_161198): campaign module source
extern char *g_mwt_flagA;         // *(DAT_16117c): layout-state flag A
extern char *g_mwt_flagB;         // *(DAT_161180): layout-state flag B
extern char *g_mwt_flagC;         // *(DAT_161184): layout-state flag C
extern int  *g_mwt_screenW;       // *(DAT_161188): screen width
extern int  *g_mwt_screenH;       // *(DAT_16118c): screen height
extern void *g_mwt_layout;        // *(DAT_1611ac): Layout singleton
extern void *g_mwt_resetLayout;   // *(DAT_1611a0): layout for resetWindowDimensions
extern void *g_mwt_starmapMod;    // *(DAT_161178): starmap module store
}

// MissionsWindow::OnTouchEnd(int y, int z)
extern "C" void MissionsWindow_OnTouchEnd(void *self, int y, int z)
{
    char *s = (char *)self;
    void *volatile cookie = __stack_chk_guard;

    // Wanted-board sub-window active.
    if (i32(self, 0x40) == 1) {
        WantedWindow_OnTouchEnd(pp(self, 0x10), z);
        if (*(int *)pp(self, 0x10) == 0) {
            i32(self, 0x40) = 0;
            *(int *)pp(self, 0x10) = 1;
        }
        goto done;
    }

    if (u8(self, 0x20) != 0) {
        // Confirmation dialog open.
        int r = ChoiceWindow_OnTouchEnd(pp(self, 0xc), z);
        if (r == 1) {
            u8(self, 0x20) = 0;
            goto done;
        }
        if (r == 0) {
            // Confirmed: clear out the freelance-mission cargo/passengers and re-init.
            void *fsrc = *(void **)g_mwt_freelanceSrc;
            int type = Mission_getType(Status_getFreelanceMission());
            bool clearCargo = (type == 0);
            if (!clearCargo && Mission_getType(Status_getFreelanceMission()) == 3) clearCargo = true;
            if (!clearCargo && Mission_getType(Status_getFreelanceMission()) == 5) clearCargo = true;
            if (clearCargo) {
                void *cargo = Ship_getCargo(Status_getShip());
                if (cargo != 0) {
                    unsigned int *c = (unsigned int *)cargo;
                    for (unsigned int i = 0; i < *c; i++) {
                        void *item = *(void **)(c[1] + i * 4);
                        if (Item_isUnsaleable(item) != 0 && Item_getIndex(item) == 0x74) {
                            Ship_removeCargo(Status_getShip(), item);
                            u8(self, 0x23) = 1;
                            break;
                        }
                    }
                }
            } else if (Mission_getType(Status_getFreelanceMission()) == 0xb) {
                Status_setPassengers(fsrc, 0);
            }

            void *agent = Mission_getAgent(Status_getFreelanceMission());
            if (Agent_isGenericAgent(agent) == 0)
                Agent_setOfferAccepted(Mission_getAgent(Status_getFreelanceMission()), false);
            Status_setFreelanceMission(fsrc);

            unsigned char savedFlag = u8(self, 0x23);
            MissionsWindow_init(self);
            u8(self, 0x23) = savedFlag;
            goto done;
        }
        // r != 0 and != 1: fall through to the normal touch handling.
    }

    // Normal (no dialog) path, unless StarMap is showing.
    if (u8(self, 0x22) == 0) {
        if (Status_wantedBoardAccessible() != 0) {
            void **tabs = (void **)pp(self, 0x14);
            for (unsigned int i = 0; i < *(unsigned int *)tabs; i++) {
                if (TouchButton_OnTouchEnd(((void **)tabs[1])[i], z) != 0)
                    u32(self, 0x40) = i;
            }
        }
        ScrollTouchWindow_OnTouchEnd(pp(self, 0x0), z);
        ScrollTouchWindow_OnTouchEnd(pp(self, 0x4), z);

        if (pp(self, 0x24) != 0 && TouchButton_OnTouchEnd(pp(self, 0x24), z) != 0) {
            // "Show on map" for the campaign mission.
            void *appMgr = *(void **)g_mwt_appMgr;
            void *mod = ApplicationManager_GetApplicationModule(appMgr);
            void *map = *(void **)((char *)mod + 0x10);
            pp(self, 0x8) = map;
            if (map == 0) {
                void *m = operator_new(0x1e8);
                StarMap_ctor(m, true, Status_getCampaignMission(), false, -1);
                void *mod2 = ApplicationManager_GetApplicationModule(appMgr);
                *(void **)((char *)mod2 + 0x10) = m;
                void *mod3 = ApplicationManager_GetApplicationModule(appMgr);
                pp(self, 0x8) = *(void **)((char *)mod3 + 0x10);
            } else {
                Status_getCampaignMission();
                StarMap_init(map);
            }
            u8(self, 0x22) = 1;
            Layout_resetWindowDimensions(*(void **)g_mwt_resetLayout);
        } else {
            if (pp(self, 0x2c) != 0 && TouchButton_OnTouchEnd(pp(self, 0x2c), z) != 0) {
                void *cw = pp(self, 0xc);
                void *t = GameText_getText(0x1a2);
                ChoiceWindow_set(cw, t, true);
                u8(self, 0x20) = 1;
            }
            if (pp(self, 0x28) != 0 && TouchButton_OnTouchEnd(pp(self, 0x28), z) != 0) {
                // "Show on map" for the freelance mission.
                void *appMgr = *(void **)g_mwt_appMgr;
                void *mod = ApplicationManager_GetApplicationModule(appMgr);
                void *map = *(void **)((char *)mod + 0x10);
                pp(self, 0x8) = map;
                if (map == 0) {
                    void *m = operator_new(0x1e8);
                    StarMap_ctor(m, true, Status_getFreelanceMission(), false, -1);
                    void *mod2 = ApplicationManager_GetApplicationModule(appMgr);
                    *(void **)((char *)mod2 + 0x10) = m;
                    pp(self, 0x8) = m;
                } else {
                    Status_getFreelanceMission();
                    StarMap_init(map);
                }
                u8(self, 0x22) = 1;
                Layout_resetWindowDimensions(*(void **)g_mwt_resetLayout);
                goto done;
            }
            void *layout = *(void **)g_mwt_layout;
            if (Layout_OnTouchEnd(layout, z, 0) != 0) {
                Layout_resetWindowDimensions(layout);
                goto done;
            }
            if (Layout_helpPressed(layout) != 0) {
                char title[0xc];
                void *t = GameText_getText(0x27b);
                String_fromText(title, t, false);
                Layout_initHelpWindow(layout, title);
                String_dtor(title);
            }
        }
    } else {
        // StarMap overlay is showing.
        if (StarMap_OnTouchEnd(pp(self, 0x8), z) != 0) {
            int wantW, wantH, posX;
            if (*g_mwt_flagA == 0) {
                i32(self, 0x30) = 0;
                i32(self, 0x34) = 0;
                wantW = 0;   // restored from globals in original; layout-driven
                wantH = 0;
                posX = 0;
            } else {
                if (*g_mwt_flagB == 0) {
                    int w = 1000, h = 0x514;
                    if (*g_mwt_flagC == 0) { w = 500; h = 0x28a; }
                    posX = h >> 1;
                    wantW = w; wantH = h;
                } else {
                    wantW = 0x2bf; posX = 0x1c9; wantH = 0x392;
                }
                i32(self, 0x30) = (*g_mwt_screenW >> 1) - posX;
                i32(self, 0x34) = (*g_mwt_screenH >> 1) - (wantW >> 1);
            }
            i32(self, 0x38) = wantH;
            i32(self, 0x3c) = wantW;
            u8(self, 0x22) = 0;
        }
    }

done:
    uint32_t guardDelta = (uint32_t)__stack_chk_guard - (uint32_t)cookie;
    if (guardDelta != 0)
        __stack_chk_fail((int)guardDelta);
}
