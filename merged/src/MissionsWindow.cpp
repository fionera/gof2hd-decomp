#include "MissionsWindow.h"




// ---- OnTouchMove_150ca8.cpp ----
extern "C" int  _mw_m_tail(void *obj, int p1, int p2);          // 0x1ac528 tail
extern "C" void _mw_m_Choice(void *w, int p1, int p2);          // 0x7474c
extern "C" void _mw_m_StarMap(void *w, int p1, int p2);         // 0x750e8
extern "C" int  _mw_m_wantedBoardAccessible(void *st);          // 0x77290
extern "C" void _mw_m_Button(void *btn, int p1, int p2);        // 0x74764
extern "C" void _mw_m_Layout(void *l, int p1, int p2);          // 0x74758
extern "C" void _mw_m_STW(void *w, int p1, int p2);             // 0x75bbc

__attribute__((visibility("hidden"))) extern void **g_mw_m_status;
__attribute__((visibility("hidden"))) extern void **g_mw_m_layout;

struct ArrHdr { unsigned len; void **data; };

// MissionsWindow::OnTouchMove(int, int)
struct MissionsWindow { int OnTouchMove(int, int); };
int MissionsWindow::OnTouchMove(int p1, int p2)
{
    if (i32(this, 0x40) == 1)
        return _mw_m_tail(pp(this, 0x10), p1, p2);
    if (u8(this, 0x20)) {
        _mw_m_Choice(pp(this, 0xc), p1, p2);
    } else if (u8(this, 0x22)) {
        _mw_m_StarMap(pp(this, 0x8), p1, p2);
    } else {
        if (_mw_m_wantedBoardAccessible(*g_mw_m_status)) {
            for (unsigned i = 0;; i++) {
                ArrHdr *arr = (ArrHdr *)pp(this, 0x14);
                if (i >= arr->len) break;
                _mw_m_Button(arr->data[i], p1, p2);
            }
        }
        _mw_m_Layout(*g_mw_m_layout, p1, p2);
        _mw_m_STW(pp(this, 0x0), p1, p2);
        _mw_m_STW(pp(this, 0x4), p1, p2);
        if (pp(this, 0x24)) _mw_m_Button(pp(this, 0x24), p1, p2);
        if (pp(this, 0x2c)) _mw_m_Button(pp(this, 0x2c), p1, p2);
        if (pp(this, 0x28)) _mw_m_Button(pp(this, 0x28), p1, p2);
    }
    return 0;
}

// ---- setHangarUpdate_1504b2.cpp ----
// MissionsWindow::setHangarUpdate(bool) -- store the flag byte at +0x23.
struct MissionsWindow { void setHangarUpdate(bool); };
void MissionsWindow::setHangarUpdate(bool v)
{
    u8(this, 0x23) = v;
}

// ---- _MissionsWindow_15040c.cpp ----
extern "C" void  _mw_ArrayReleaseImagePart(void *a);   // 0x75ae4
extern "C" void *_mw_ImagePartArray_dtor(void *a);     // 0x75af0
extern "C" void  _mw_ArrayReleaseTouchButton(void *a); // 0x75328
extern "C" void *_mw_TouchButtonArray_dtor(void *a);   // 0x75334
extern "C" void *_mw_STW_dtor(void *w);                // 0x75afc
extern "C" void *_mw_ChoiceWindow_dtor(void *w);       // 0x74d88
extern "C" void *_mw_TouchButton_dtor(void *b);        // 0x74d7c
extern "C" void *_mw_WantedWindow_dtor(void *w);       // 0x772a8

// MissionsWindow::~MissionsWindow()
struct MissionsWindow { ~MissionsWindow(); };
MissionsWindow::~MissionsWindow()
{
    if (pp(this, 0x18)) {
        _mw_ArrayReleaseImagePart(pp(this, 0x18));
        if (pp(this, 0x18)) operator delete(_mw_ImagePartArray_dtor(pp(this, 0x18)));
    }
    pp(this, 0x18) = 0;
    if (pp(this, 0x14)) {
        _mw_ArrayReleaseTouchButton(pp(this, 0x14));
        if (pp(this, 0x14)) operator delete(_mw_TouchButtonArray_dtor(pp(this, 0x14)));
    }
    pp(this, 0x14) = 0;
    if (pp(this, 0x0)) operator delete(_mw_STW_dtor(pp(this, 0x0)));
    pp(this, 0x0) = 0;
    if (pp(this, 0x4)) operator delete(_mw_STW_dtor(pp(this, 0x4)));
    pp(this, 0x4) = 0;
    if (pp(this, 0xc)) operator delete(_mw_ChoiceWindow_dtor(pp(this, 0xc)));
    pp(this, 0xc) = 0;
    if (pp(this, 0x24)) operator delete(_mw_TouchButton_dtor(pp(this, 0x24)));
    pp(this, 0x24) = 0;
    if (pp(this, 0x28)) operator delete(_mw_TouchButton_dtor(pp(this, 0x28)));
    pp(this, 0x28) = 0;
    if (pp(this, 0x2c)) operator delete(_mw_TouchButton_dtor(pp(this, 0x2c)));
    pp(this, 0x2c) = 0;
    if (pp(this, 0x10)) operator delete(_mw_WantedWindow_dtor(pp(this, 0x10)));
    pp(this, 0x10) = 0;
}

// ---- hangarNeedsUpdate_1509a8.cpp ----
// MissionsWindow::hangarNeedsUpdate() -- raw uint8 getter at +0x23.
struct MissionsWindow { uint8_t hangarNeedsUpdate(); };
uint8_t MissionsWindow::hangarNeedsUpdate()
{
    return u8(this, 0x23);
}

// ---- OnTouchBegin_150bdc.cpp ----
extern "C" int  _mw_b_tail(void *obj, int p1, int p2);          // 0x1ac518 tail
extern "C" void _mw_b_Choice(void *w, int p1, int p2);          // 0x7471c
extern "C" void _mw_b_StarMap(void *w, int p1, int p2);         // 0x74704
extern "C" int  _mw_b_wantedBoardAccessible(void *st);          // 0x77290
extern "C" void _mw_b_Button(void *btn, int p1, int p2);        // 0x74734
extern "C" void _mw_b_Layout(void *l, int p1, int p2);          // 0x74728
extern "C" void _mw_b_STW(void *w, int p1, int p2);             // 0x75ba4

__attribute__((visibility("hidden"))) extern void **g_mw_b_status;
__attribute__((visibility("hidden"))) extern void **g_mw_b_layout;

struct ArrHdr { unsigned len; void **data; };

// MissionsWindow::OnTouchBegin(int, int)
struct MissionsWindow { int OnTouchBegin(int, int); };
int MissionsWindow::OnTouchBegin(int p1, int p2)
{
    if (i32(this, 0x40) == 1)
        return _mw_b_tail(pp(this, 0x10), p1, p2);
    if (u8(this, 0x20)) {
        _mw_b_Choice(pp(this, 0xc), p1, p2);
    } else if (u8(this, 0x22)) {
        _mw_b_StarMap(pp(this, 0x8), p1, p2);
    } else {
        if (_mw_b_wantedBoardAccessible(*g_mw_b_status)) {
            for (unsigned i = 0;; i++) {
                ArrHdr *arr = (ArrHdr *)pp(this, 0x14);
                if (i >= arr->len) break;
                _mw_b_Button(arr->data[i], p1, p2);
            }
        }
        _mw_b_Layout(*g_mw_b_layout, p1, p2);
        _mw_b_STW(pp(this, 0x0), p1, p2);
        _mw_b_STW(pp(this, 0x4), p1, p2);
        if (pp(this, 0x24)) _mw_b_Button(pp(this, 0x24), p1, p2);
        if (pp(this, 0x2c)) _mw_b_Button(pp(this, 0x2c), p1, p2);
        if (pp(this, 0x28)) _mw_b_Button(pp(this, 0x28), p1, p2);
    }
    return 0;
}

// ---- init_14fa68.cpp ----
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

    uint32_t guardDelta = (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - (uint32_t)(__UINTPTR_TYPE__)cookie;
    if (guardDelta != 0)
        __stack_chk_fail((int)guardDelta);
    return 0;
}

// ---- draw_1504dc.cpp ----
extern "C" {
extern void *__stack_chk_guard;
__attribute__((noreturn)) void __stack_chk_fail(int diff) noexcept;

// Two mode-specific draw tail calls (mode==1 / flag@0x22).
void MissionsWindow_drawWanted(void *self);   // DAT_1ac504 thunk
void MissionsWindow_drawStarMap(void *self);  // DAT_1ac274 thunk

void  PaintCanvas_SetColor(void *canvas);
void  PaintCanvas_DrawString(void *canvas, void *font, void *text, int x, int y);

void  Layout_drawHeader(void *layout, void *title);
void  Layout_drawBox(void *layout, int style, int x, int y, int w, int h, void *label,
                     void *canvas, void *color);
void  Layout_drawFooter(void *layout);

void *GameText_getText(int id);
void  String_fromC(void *s, const char *text, bool copy);
void  String_fromText(void *s, void *text, bool copy);
void  String_dtor(void *s);

int   Status_wantedBoardAccessible();
void *Status_getFreelanceMission();

int   Mission_isEmpty(void *mission);
void *Mission_getAgent(void *mission);
int   Mission_getType(void *mission);

void  Agent_getName(void *agent, void *out);
void  Agent_getStationName(void *agent, void *out);
void *Agent_getMission(void *agent);

void  TouchButton_draw(void *btn);
void  ScrollTouchWindow_draw(void *win);
void  ChoiceWindow_draw(void *win);
void  ImageFactory_drawChar(void *factory, void *arr, int x, int y, bool b);

extern void *g_mwd_canvas;    // *(DAT_16059c): paint canvas
extern void *g_mwd_layout;    // *(DAT_1605a0): Layout singleton
extern int  *g_mwd_textId;    // *(DAT_1605a4): GameText id base
extern void *g_mwd_color;     // *(DAT_1605a8): box colour
extern void *g_mwd_imageFactory; // *(DAT_1609a0): image factory
extern void *g_mwd_font;      // *(DAT_1609a4): default font
}

// MissionsWindow::draw()
extern "C" void MissionsWindow_draw(void *self)
{
    char *s = (char *)self;
    void *volatile cookie = __stack_chk_guard;

    if (i32(self, 0x40) == 1) { MissionsWindow_drawWanted(self);  return; }
    if (u8(self, 0x22) != 0)  { MissionsWindow_drawStarMap(self); return; }

    void *canvas = *(void **)g_mwd_canvas;
    void *layout = *(void **)g_mwd_layout;
    void *color = *(void **)g_mwd_color;
    void *font = *(void **)g_mwd_font;
    int titleId = *g_mwd_textId;

    PaintCanvas_SetColor(canvas);

    char header[0xc];
    void *ht = GameText_getText(titleId);
    String_fromText(header, ht, false);
    Layout_drawHeader(layout, header);
    String_dtor(header);

    if (Status_wantedBoardAccessible() != 0) {
        void **tabs = (void **)pp(self, 0x14);
        for (unsigned int i = 0; i < *(unsigned int *)tabs; i++)
            TouchButton_draw(((void **)tabs[1])[i]);
    }

    int ox = i32(self, 0x30), oy = i32(self, 0x34);
    int ow = i32(self, 0x38), oh = i32(self, 0x3c);

    // Campaign-mission title box + body box.
    {
        char box[0xc];
        void *t = GameText_getText(titleId);
        String_fromText(box, t, false);
        int c = *(int *)((char *)layout + 0xc), p20 = *(int *)((char *)layout + 0x20);
        int p28 = *(int *)((char *)layout + 0x28), p2c = *(int *)((char *)layout + 0x2c);
        Layout_drawBox(layout, 1, p28 + ox, oy + c + p20, (ow >> 1) - (p2c + p28),
                       *(int *)((char *)layout + 0x5c), box, canvas, color);
        String_dtor(box);
    }
    {
        char box[0xc];
        String_fromC(box, "", false);
        int c = *(int *)((char *)layout + 0xc), p10 = *(int *)((char *)layout + 0x10);
        int p20 = *(int *)((char *)layout + 0x20), p24 = *(int *)((char *)layout + 0x24);
        int p28 = *(int *)((char *)layout + 0x28), p2c = *(int *)((char *)layout + 0x2c);
        int p5c = *(int *)((char *)layout + 0x5c);
        Layout_drawBox(layout, 5, p28 + ox, oy + c + p20 + p5c + p2c, (ow >> 1) - (p2c + p28),
                       ((oh - (p20 + c + p5c + p2c * 2)) - p10) - p24, box, canvas, color);
        String_dtor(box);
    }

    ScrollTouchWindow_draw(pp(self, 0x0));
    if (pp(self, 0x24) != 0) TouchButton_draw(pp(self, 0x24));

    // Freelance-mission title + body box (right column).
    {
        char box[0xc];
        void *t = GameText_getText(titleId);
        String_fromText(box, t, false);
        int c = *(int *)((char *)layout + 0xc), p20 = *(int *)((char *)layout + 0x20);
        int p28 = *(int *)((char *)layout + 0x28), p2c = *(int *)((char *)layout + 0x2c);
        Layout_drawBox(layout, 1, ox + (ow >> 1) + p2c, oy + c + p20,
                       ((ow >> 1) - p2c) - p28, *(int *)((char *)layout + 0x5c), box, canvas, color);
        String_dtor(box);
    }
    {
        char box[0xc];
        String_fromC(box, "", false);
        int c = *(int *)((char *)layout + 0xc), p10 = *(int *)((char *)layout + 0x10);
        int p20 = *(int *)((char *)layout + 0x20), p24 = *(int *)((char *)layout + 0x24);
        int p28 = *(int *)((char *)layout + 0x28), p2c = *(int *)((char *)layout + 0x2c);
        int p5c = *(int *)((char *)layout + 0x5c);
        Layout_drawBox(layout, 5, ox + (ow >> 1) + p2c, oy + p2c + c + p20 + p5c,
                       ((ow >> 1) - p2c) - p28, ((oh - (c + p2c * 2 + p20 + p5c)) - p10) - p24,
                       box, canvas, color);
        String_dtor(box);
    }

    // Active freelance mission details.
    void *fm = Status_getFreelanceMission();
    if (fm != 0 && Mission_isEmpty(fm) == 0 && pp(self, 0x18) != 0) {
        ImageFactory_drawChar(*(void **)g_mwd_imageFactory, pp(self, 0x18),
                              ox + (ow >> 1) + *(int *)((char *)layout + 0x2c),
                              *(int *)((char *)layout + 0x2c) + oy + *(int *)((char *)layout + 0xc) +
                                  *(int *)((char *)layout + 0x20) + *(int *)((char *)layout + 0x5c),
                              false);

        int detailX = ox + (ow >> 1) + *(int *)((char *)layout + 0x2d4) +
                      *(int *)((char *)layout + 0x2c) * 2;
        int detailY = oy + *(int *)((char *)layout + 0xc) + *(int *)((char *)layout + 0x20) +
                      *(int *)((char *)layout + 0x2c) + *(int *)((char *)layout + 0x5c);

        char nameStr[0xc];
        Agent_getName(Mission_getAgent(fm), nameStr);
        PaintCanvas_DrawString(canvas, font, nameStr, detailX, detailY);
        String_dtor(nameStr);

        char stationStr[0xc];
        Agent_getStationName(Mission_getAgent(fm), stationStr);
        PaintCanvas_DrawString(canvas, font, stationStr, detailX, detailY);
        String_dtor(stationStr);

        void *typeTxt = GameText_getText(Mission_getType(Agent_getMission(Mission_getAgent(fm))) + 0x162);
        PaintCanvas_DrawString(canvas, font, typeTxt, detailX, detailY);
    }

    ScrollTouchWindow_draw(pp(self, 0x4));
    if (pp(self, 0x2c) != 0) TouchButton_draw(pp(self, 0x2c));
    if (pp(self, 0x28) != 0) TouchButton_draw(pp(self, 0x28));

    Layout_drawFooter(layout);
    if (u8(self, 0x21) != 0 || u8(self, 0x20) != 0)
        ChoiceWindow_draw(pp(self, 0xc));

    uint32_t guardDelta = (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - (uint32_t)(__UINTPTR_TYPE__)cookie;
    if (guardDelta != 0)
        __stack_chk_fail((int)guardDelta);
}

// ---- MissionsWindow_14fa10.cpp ----
extern "C" int   _mw_GetTextHeight(void *canvas);   // 0x6fe20
extern "C" void *_mw_init(void *self);              // 0x753d0

// Two PC-relative singletons; each holds a pointer (read via one extra deref).
// The second is loaded and dereferenced but its value is unused in the target,
// so we read it through a volatile lvalue to keep the load from being elided.
__attribute__((visibility("hidden"))) extern void **g_mw_a;
__attribute__((visibility("hidden"))) extern void **g_mw_b;

struct __attribute__((aligned(4))) Blk16 { int a, b, c, d; };

// MissionsWindow::MissionsWindow()
struct MissionsWindow { MissionsWindow(); };
MissionsWindow::MissionsWindow()
{
    void *canvas = *g_mw_a;
    void *dead = *(void *volatile *)*g_mw_b;
    i32(this, 0x24) = 0;
    i32(this, 0x28) = 0;
    i32(this, 0x2c) = 0;
    Blk16 z = {0, 0, 0, 0};
    *(Blk16 *)((char *)this + 0xc) = z;
    *(Blk16 *)this = z;
    (void)dead;
    int h = _mw_GetTextHeight(canvas);
    i32(this, 0x1c) = h / 2 - 1;
    _mw_init(this);
}

// ---- render3D_1504b8.cpp ----
extern "C" void _mw_WantedWindow_render3D(void *ww);   // 0x772b4
extern "C" void _mw_render3D_tail(void *obj);          // 0x1ac258 tail

// MissionsWindow::render3D()
struct MissionsWindow { void render3D(); };
void MissionsWindow::render3D()
{
    if (i32(this, 0x40) == 1)
        _mw_WantedWindow_render3D(pp(this, 0x10));
    if (u8(this, 0x22))
        return _mw_render3D_tail(pp(this, 0x8));
}

// ---- OnTouchEnd_150d74.cpp ----
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
    uint32_t guardDelta = (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - (uint32_t)(__UINTPTR_TYPE__)cookie;
    if (guardDelta != 0)
        __stack_chk_fail((int)guardDelta);
}

// ---- update_1509b0.cpp ----
extern "C" {
extern void *__stack_chk_guard;
__attribute__((noreturn)) void __stack_chk_fail(int diff) noexcept;

// Two mode-specific tail calls reached via GOT thunks.
void MissionsWindow_acceptAction(void *self);   // DAT_1ac514 thunk (mode == 1)
void MissionsWindow_cancelAction(void *self);   // DAT_1ac284 thunk (flag @0x22)

void ScrollTouchWindow_update(void *win, int dt);
void ScrollTouchWindow_setText(void *win, void *a, void *b);

void *Status_getCampaignMission();
int   Status_gameWon();
int   Status_getCurrentCampaignMission();

int   Mission_getType(void *mission);
int   Mission_getProductionGoodAmount(void *mission);
int   Mission_getStatusValue(void *mission);

void  String_fromC(void *s, const char *text, bool copy);
void  String_fromText(void *s, void *text, bool copy);
void  String_fromInt(void *s, int value);
void  String_dtor(void *s);
void  String_assign(void *dst, void *src);

void *GameText_getText(int id);

void  Status_replaceHash(void *out, void *key, void *a, void *b, void *c);

void  TouchButton_setAlwaysPressed(void *btn, bool pressed);

extern void *g_mw_status;        // *(DAT_160bbc): Status singleton
extern void *g_mw_campaign;      // *(DAT_160bc0): campaign-state record
extern int  *g_mw_textBase;      // *(DAT_160bc8): GameText id base for mission titles
extern int  *g_mw_titleTable;    // *(DAT_160bcc): per-mission title id table
extern void *g_mw_hashSource;    // *(DAT_160bbc): replaceHash key source (== status)
}

// MissionsWindow::update(int dt)
extern "C" void MissionsWindow_update(void *self, int dt)
{
    void *volatile cookie = __stack_chk_guard;

    // Mode 1: confirm/accept and bail out.
    if (i32(self, 0x40) == 1) {
        MissionsWindow_acceptAction(self);
        return;
    }
    // Cancel flag set: cancel and bail out.
    if (u8(self, 0x22) != 0) {
        MissionsWindow_cancelAction(self);
        return;
    }

    ScrollTouchWindow_update(pp(self, 0x0), dt);
    ScrollTouchWindow_update(pp(self, 0x4), dt);

    void *status = *(void **)g_mw_status;
    int type = Mission_getType(Status_getCampaignMission());
    bool relevant = (type == 0xa7);
    if (!relevant) {
        if (Mission_getType(Status_getCampaignMission()) == 0xae)
            relevant = true;
    }

    if (relevant) {
        void *camp = *(void **)g_mw_campaign;
        bool show = (Status_gameWon() == 0) ||
                    (*(char *)((char *)camp + 0x37) != 0 || *(char *)((char *)camp + 0x35) != 0);
        if (show) {
            char text[0xc];
            String_fromC(text, "", false);
            if (Status_getCurrentCampaignMission() < 0xa4) {
                int base = *g_mw_textBase;
                void *titleTxt = GameText_getText(g_mw_titleTable[Status_getCurrentCampaignMission()]);
                (void)base;
                String_assign(text, titleTxt);

                void *key = *(void **)g_mw_hashSource;
                char hdr[0xc], amount[0xc], suffix[0xc], merged[0xc];
                String_fromText(hdr, text, false);
                int need = Mission_getProductionGoodAmount(Status_getCampaignMission());
                int have = Mission_getStatusValue(Status_getCampaignMission());
                String_fromInt(amount, need - have);
                String_fromC(suffix, "", false);
                Status_replaceHash(merged, key, hdr, amount, suffix);
                String_assign(text, merged);
                String_dtor(merged); String_dtor(suffix); String_dtor(amount); String_dtor(hdr);

                void *win = pp(self, 0x0);
                char a[0xc], b[0xc];
                String_fromC(a, "", false);
                String_fromText(b, text, false);
                ScrollTouchWindow_setText(win, a, b);
                String_dtor(b);
                String_dtor(a);
            }
            String_dtor(text);
        }
    }

    // Highlight the selected tab.
    void **tabs = (void **)pp(self, 0x14);
    for (unsigned int i = 0; i < *(unsigned int *)tabs; i++)
        TouchButton_setAlwaysPressed(((void **)tabs[1])[i], i == u32(self, 0x40));

    uint32_t guardDelta = (uint32_t)(__UINTPTR_TYPE__)__stack_chk_guard - (uint32_t)(__UINTPTR_TYPE__)cookie;
    if (guardDelta != 0)
        __stack_chk_fail((int)guardDelta);
}
