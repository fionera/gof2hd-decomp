#include "class.h"

extern "C" {
extern void *__stack_chk_guard;
__attribute__((noreturn)) void __stack_chk_fail(int diff) noexcept;

void *operator_new(uint32_t size);
void  operator_delete(void *p) noexcept;

void  ArrayTB_ctor(void *self);
void  ArrayTB_setLength(int n, void *self);
void  ArrayImg_releaseClasses(void *self);
void *ArrayImg_dtor(void *self);

void  TouchButton_ctor(void *self, void *text, int kind, int x, int y, char a, char b);
void  TouchButton_ctorTab(void *self, void *text, int kind, int x, int y, char flags);
int   TouchButton_getWidth(void *self);
void  TouchButton_setAlwaysPressed(void *self, bool pressed);
void  TouchButton_setTextColor(void *self, int color);
void *TouchButton_dtor(void *self);

void  ScrollTouchWindow_ctor(void *self, int x, int y, int w, int h, bool b);
void *ScrollTouchWindow_dtor(void *self);
void  ScrollTouchWindow_setText(void *self, void *a, void *b);

void *ChoiceWindow_ctor(void *self);
void *ChoiceWindow_dtor(void *self);

void *WantedWindow_ctor(void *self);
void  WantedWindow_init(void *self);

void  Layout_setWindowDimensions(void *layout, int x, int y, int w, int h);
int   Layout_getHelpButtonOffset();
void  Layout_formatCredits(void *out, int credits);

void *GameText_getText(int id);
void  String_fromC(void *s, const char *text, bool copy);
void  String_fromText(void *s, void *text, bool copy);
void  String_fromInt(void *s, int value);
void  String_dtor(void *s);
void  String_assign(void *dst, void *src);

int   Status_gameWon();
int   Status_inAlienOrbit();
int   Status_wantedBoardAccessible();
int   Status_getCurrentCampaignMission();
void *Status_getCampaignMission();
void *Status_getFreelanceMission();
void  Status_replaceHash(void *out, void *key, void *a, void *b, void *c);

int   Mission_getType(void *mission);
int   Mission_isEmpty(void *mission);
int   Mission_getProductionGoodAmount(void *mission);
int   Mission_getStatusValue(void *mission);
void  Mission_getTargetStationName(void *mission, void *out);
void *Mission_getAgent(void *mission);
int   Mission_getReward(void *mission);
int   Mission_getBonus(void *mission);

void  Globals_getAgentMissionText(void *out, void *agent);

int   Achievements_gotAllGoldMedals();

void *Status_getShip();
int   Ship_getIndex(void *ship);

void *Agent_getImageParts(void *agent);
void *ImageFactory_loadChar(void *factory, void *parts);

int   ApplicationManager_GetCurrentApplicationModule(void *appMgr);

extern char *g_mwi_flagA;        // *(DAT_15fd80): layout-state flag A
extern char *g_mwi_flagB;        // *(DAT_15fd88): layout-state flag B
extern char *g_mwi_flagC;        // *(DAT_15fd98): layout-state flag C
extern int  *g_mwi_screenW;      // *(DAT_15fd9c): screen width
extern int  *g_mwi_screenH;      // *(DAT_15fda0 base): screen height
extern void *g_mwi_layout;       // *(DAT_15fda8): Layout singleton
extern int  *g_mwi_titleTable;   // *(DAT_15fda4): GameText id base
extern void *g_mwi_status;       // *(DAT_15fdac / others): Status singleton
extern void *g_mwi_campaign;     // *(DAT_15fdb0): campaign-state record
extern void *g_mwi_imageFactory; // *(DAT_160404): image factory
extern void *g_mwi_appMgr;       // *(DAT_160408): application manager
extern int   g_mwi_actionColor;  // DAT_1603f0: action-button text colour
}

// MissionsWindow::init() -- rebuild the whole missions screen layout & sub-windows.
extern "C" int MissionsWindow_init(void *self)
{
    char *s = (char *)self;
    void *volatile cookie = __stack_chk_guard;

    void *layout = *(void **)g_mwi_layout;
    int titleId = *g_mwi_titleTable;

    // --- window geometry (orientation / state dependent) ---
    if (*g_mwi_flagA == 0) {
        i32(self, 0x30) = 0;
        i32(self, 0x34) = 0;
        i32(self, 0x38) = *g_mwi_screenW;
        i32(self, 0x3c) = *g_mwi_screenH;
    } else {
        int w, h;
        if (*g_mwi_flagB == 0) {
            h = 0x514;
            if (*g_mwi_flagC == 0) h = 0x28a;
            w = (*g_mwi_flagC == 0) ? 500 : 1000;
        } else {
            h = 0x392;
            w = 1000;
        }
        i32(self, 0x38) = h;
        i32(self, 0x3c) = w;
        i32(self, 0x30) = (*g_mwi_screenW >> 1) - (h >> 1);
        i32(self, 0x34) = (*g_mwi_screenH >> 1) - (w >> 1);
    }

    // --- two tab buttons ---
    void *tabs = operator_new(0xc);
    ArrayTB_ctor(tabs);
    pp(self, 0x14) = tabs;
    ArrayTB_setLength(2, tabs);

    void *b0 = operator_new(200);
    void *t0 = GameText_getText(titleId);
    int helpOff = Layout_getHelpButtonOffset();
    TouchButton_ctorTab(b0, t0, 3, (i32(self, 0x38) + i32(self, 0x30)) - helpOff, i32(self, 0x34), 0x12);
    *(void **)(*(int *)(pp(self, 0x14)) + 4 + 4) = b0;

    void *b1 = operator_new(200);
    void *t1 = GameText_getText(titleId);
    int helpOff2 = Layout_getHelpButtonOffset();
    int w1 = TouchButton_getWidth(b1);
    TouchButton_ctorTab(b1, t1, 3,
                        (((i32(self, 0x38) + i32(self, 0x30)) - helpOff2) - w1) +
                            *(int *)((char *)layout + 0x38), i32(self, 0x34), 0x12);
    *(void **)(*(int *)(pp(self, 0x14)) + 4) = b1;
    TouchButton_setAlwaysPressed(*(void **)(*(int *)(pp(self, 0x14)) + 4), true);

    Layout_setWindowDimensions(layout, i32(self, 0x30), i32(self, 0x34), i32(self, 0x38), i32(self, 0x3c));

    // --- tear down any previous sub-objects ---
    if (pp(self, 0x18) != 0) {
        ArrayImg_releaseClasses(pp(self, 0x18));
        if (pp(self, 0x18) != 0) operator_delete(ArrayImg_dtor(pp(self, 0x18)));
    }
    pp(self, 0x18) = 0;
    if (pp(self, 0x0) != 0) operator_delete(ScrollTouchWindow_dtor(pp(self, 0x0)));
    pp(self, 0x0) = 0;
    if (pp(self, 0x4) != 0) operator_delete(ScrollTouchWindow_dtor(pp(self, 0x4)));
    pp(self, 0x4) = 0;
    if (pp(self, 0xc) != 0) operator_delete(ChoiceWindow_dtor(pp(self, 0xc)));
    pp(self, 0xc) = 0;
    if (pp(self, 0x24) != 0) operator_delete(TouchButton_dtor(pp(self, 0x24)));
    pp(self, 0x24) = 0;
    if (pp(self, 0x28) != 0) operator_delete(TouchButton_dtor(pp(self, 0x28)));
    pp(self, 0x28) = 0;
    if (pp(self, 0x2c) != 0) operator_delete(TouchButton_dtor(pp(self, 0x2c)));
    pp(self, 0x8) = 0;
    pp(self, 0x2c) = 0;
    i32(self, 0x20) = 0;

    // --- left (campaign) scroll window ---
    int topY = *(int *)((char *)layout + 0xc) + i32(self, 0x34) + *(int *)((char *)layout + 0x20) +
               *(int *)((char *)layout + 0x5c) + *(int *)((char *)layout + 0x2c);
    int reserve = (Status_gameWon() == 0) ? *(int *)((char *)layout + 0x30) : 0;
    void *sw0 = operator_new(0x24);
    ScrollTouchWindow_ctor(sw0,
        *(int *)((char *)layout + 0x28) + i32(self, 0x30), topY,
        (i32(self, 0x38) >> 1) - (*(int *)((char *)layout + 0x2c) + *(int *)((char *)layout + 0x28)),
        (((((i32(self, 0x34) - topY) + i32(self, 0x3c)) - *(int *)((char *)layout + 0x10)) -
          *(int *)((char *)layout + 0x24)) - reserve) + *(int *)((char *)layout + 0x2c) * -2, false);
    pp(self, 0x0) = sw0;

    // --- campaign-mission summary text ---
    bool campShow = (Status_gameWon() == 0) ||
                    (*(char *)(*(char **)g_mwi_campaign + 0x37) != 0 ||
                     *(char *)(*(char **)g_mwi_campaign + 0x35) != 0);
    if (campShow) {
        char text[0xc];
        String_fromC(text, "", false);
        if (Status_getCurrentCampaignMission() < 0xa4) {
            void *t = GameText_getText(titleId);
            String_assign(text, t);
        }
        int type = Mission_getType(Status_getCampaignMission());
        bool production = (type == 0xa7) || (Mission_getType(Status_getCampaignMission()) == 0xae);
        void *key = *(void **)g_mwi_status;
        char hdr[0xc], val[0xc], suffix[0xc], merged[0xc];
        if (production) {
            String_fromText(hdr, text, false);
            int need = Mission_getProductionGoodAmount(Status_getCampaignMission());
            int have = Mission_getStatusValue(Status_getCampaignMission());
            String_fromInt(val, need - have);
            String_fromC(suffix, "", false);
            Status_replaceHash(merged, key, hdr, val, suffix);
        } else {
            String_fromText(hdr, text, false);
            Mission_getTargetStationName(Status_getCampaignMission(), val);
            String_fromC(suffix, "", false);
            Status_replaceHash(merged, key, hdr, val, suffix);
        }
        String_assign(text, merged);
        String_dtor(merged); String_dtor(suffix); String_dtor(val); String_dtor(hdr);

        char a[0xc], b[0xc];
        String_fromC(a, "", false);
        String_fromText(b, text, false);
        ScrollTouchWindow_setText(pp(self, 0x0), a, b);
        String_dtor(b); String_dtor(a);
        String_dtor(text);
    } else {
        bool useGold = Achievements_gotAllGoldMedals() != 0 && Ship_getIndex(Status_getShip()) != 8;
        char a[0xc], b[0xc];
        String_fromC(a, "", false);
        void *t = GameText_getText(titleId);
        String_fromText(b, t, false);
        ScrollTouchWindow_setText(pp(self, 0x0), a, b);
        String_dtor(b); String_dtor(a);
        (void)useGold;
    }

    // --- right (freelance) scroll window ---
    int fmEmpty = Mission_isEmpty(Status_getFreelanceMission());
    void *sw1 = operator_new(0x24);
    int half = i32(self, 0x38) >> 1;
    int pad = *(int *)((char *)layout + 0x2c);
    int rx = i32(self, 0x30) + half + pad;
    if (fmEmpty == 0) {
        int ry = *(int *)((char *)layout + 0x2d8) + topY + pad;
        ScrollTouchWindow_ctor(sw1, rx, ry, (half - pad) - *(int *)((char *)layout + 0x28),
            (((((i32(self, 0x34) - ry) + i32(self, 0x3c)) - *(int *)((char *)layout + 0x10)) -
              *(int *)((char *)layout + 0x24)) - *(int *)((char *)layout + 0x4c)) -
                *(int *)((char *)layout + 0x30), false);
        pp(self, 0x4) = sw1;

        char text[0xc], reward[0xc], suffix[0xc], merged[0xc];
        Globals_getAgentMissionText(text, Mission_getAgent(Status_getFreelanceMission()));
        void *key = *(void **)g_mwi_status;
        char body[0xc];
        String_fromText(body, text, false);
        int rew = Mission_getReward(Status_getFreelanceMission());
        int bonus = Mission_getBonus(Status_getFreelanceMission());
        Layout_formatCredits(reward, rew + bonus);
        String_fromC(suffix, "", false);
        Status_replaceHash(merged, key, body, reward, suffix);
        String_assign(text, merged);
        String_dtor(merged); String_dtor(suffix); String_dtor(reward); String_dtor(body);

        char a[0xc], b[0xc];
        String_fromC(a, "", false);
        String_fromText(b, text, false);
        ScrollTouchWindow_setText(pp(self, 0x4), a, b);
        String_dtor(b); String_dtor(a);

        void *parts = Agent_getImageParts(Mission_getAgent(Status_getFreelanceMission()));
        pp(self, 0x18) = ImageFactory_loadChar(*(void **)g_mwi_imageFactory, parts);
        String_dtor(text);
    } else {
        ScrollTouchWindow_ctor(sw1, rx, topY, (half - pad) - *(int *)((char *)layout + 0x28),
            ((i32(self, 0x3c) + (i32(self, 0x34) - (topY + pad * 2))) -
             *(int *)((char *)layout + 0x10)) - *(int *)((char *)layout + 0x24), false);
        pp(self, 0x4) = sw1;
        char a[0xc], b[0xc];
        String_fromC(a, "", false);
        void *t = GameText_getText(titleId);
        String_fromText(b, t, false);
        ScrollTouchWindow_setText(pp(self, 0x4), a, b);
        String_dtor(b); String_dtor(a);
    }

    // --- action buttons (Accept / Reject / Auto-pilot) ---
    if (Status_inAlienOrbit() == 0) {
        int btnY = ((i32(self, 0x38) >> 1) >> 1) - *(int *)((char *)layout + 0x28);
        if (Status_gameWon() == 0) {
            void *bAccept = operator_new(200);
            void *t = GameText_getText(titleId);
            TouchButton_ctor(bAccept, t, 0, *(int *)((char *)layout + 0x28) + i32(self, 0x30),
                             (((i32(self, 0x34) + i32(self, 0x3c)) - *(int *)((char *)layout + 0x10)) -
                              *(int *)((char *)layout + 0x24)) - *(int *)((char *)layout + 0x2c),
                             '!', 4);
            pp(self, 0x24) = bAccept;
        }
        if (Mission_isEmpty(Status_getFreelanceMission()) == 0) {
            void *bReject = operator_new(200);
            void *t = GameText_getText(titleId);
            TouchButton_ctor(bReject, t, 0,
                             i32(self, 0x30) + (i32(self, 0x38) >> 1) + *(int *)((char *)layout + 0x2c),
                             (((i32(self, 0x34) - *(int *)((char *)layout + 0x2c)) + i32(self, 0x3c)) -
                              *(int *)((char *)layout + 0x10)) - *(int *)((char *)layout + 0x24),
                             '!', 4);
            pp(self, 0x28) = bReject;

            if (ApplicationManager_GetCurrentApplicationModule(*(void **)g_mwi_appMgr) == 5) {
                void *bMap = operator_new(200);
                void *t2 = GameText_getText(titleId);
                TouchButton_ctor(bMap, t2, 0,
                                 i32(self, 0x30) + btnY + (i32(self, 0x38) >> 1) +
                                     *(int *)((char *)layout + 0x2c) * 2,
                                 (((i32(self, 0x34) - *(int *)((char *)layout + 0x2c)) + i32(self, 0x3c)) -
                                  *(int *)((char *)layout + 0x10)) - *(int *)((char *)layout + 0x24),
                                 '!', 4);
                pp(self, 0x2c) = bMap;
                TouchButton_setTextColor(bMap, g_mwi_actionColor);
            }
            void *cw = operator_new(0x5c);
            ChoiceWindow_ctor(cw);
            pp(self, 0xc) = cw;
        }
    }

    i32(self, 0x40) = 0;
    u8(self, 0x23) = 0;

    if (Status_wantedBoardAccessible() != 0) {
        if (pp(self, 0x10) == 0) {
            void *ww = operator_new(0xb4);
            WantedWindow_ctor(ww);
            pp(self, 0x10) = ww;
        } else {
            WantedWindow_init(pp(self, 0x10));
        }
    }

    uint32_t guardDelta = (uint32_t)__stack_chk_guard - (uint32_t)cookie;
    if (guardDelta != 0)
        __stack_chk_fail((int)guardDelta);
    return 0;
}
