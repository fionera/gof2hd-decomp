#include "gof2/MissionsWindow.h"
#include "gof2/PaintCanvasClass.h"
#include "gof2/ChoiceWindow.h"
#include "gof2/Item.h"
#include "gof2/ScrollTouchWindow.h"
#include "gof2/StarMap.h"
#include "gof2/Status.h"
#include "gof2/ApplicationManager.h"
#include "gof2/GameText.h"
#include "gof2/ImageFactory.h"
#include "gof2/Layout.h"
#include "gof2/String.h"
#include "gof2/Achievements.h"

// NOTE on includes: Agent.h, Mission.h, TouchButton.h and WantedWindow.h cannot be
// included here. Agent.h/Mission.h/TouchButton.h each define an unguarded helper
// `struct RetStr`, so pulling in more than one collides ("redefinition of RetStr"),
// and WantedWindow.h is malformed (method declarations leak out of a helper function
// body, colliding with TouchButton's OnTouch* and redefining Blk16). MissionsWindow
// only needs a handful of methods from each of these types, so minimal local
// declarations are provided below. The full definitions live in their own headers;
// this translation unit only needs the receiver layout and call signatures to compile.
struct Agent {
    int *getImageParts();
    void  getName();           // returns RetStr in full header; result discarded here
    void  getStationName();    // returns RetStr in full header; result discarded here
    struct Mission *getMission();
    bool  isGenericAgent();
    void  setOfferAccepted(bool v);
};
struct Mission {
    void getTargetStationName();   // returns RetStr in full header; result discarded here
    bool isEmpty();
};
struct TouchButton {
    void         dtor();
    int          getWidth();
    void         setAlwaysPressed(bool value);
    void         setTextColor(int color);
    void         draw();
    unsigned int OnTouchEnd(int py);
};
struct WantedWindow {
    int  init();
    void OnTouchEnd(int y);
};

// GameText singleton: getText(int key) is a member of the GameText table object. The
// decompiler folded the table pointer and the key into a single (GameText*)key cast and
// dropped the explicit key argument. The active GameText table is reachable through these
// hidden PC-relative globals (one per recovered function), used here only as the receiver.
__attribute__((visibility("hidden"))) extern void *g_mw_gameText;
// Achievements singleton holder (PC-relative DAT, same global address as gG_achievements /
// g_ch_ach). The decompiler folded the receiver into a 0-arg call; recovered as
// (*(Achievements **)g_mw_ach)->gotAllGoldMedals().
__attribute__((visibility("hidden"))) extern void *g_mw_ach;




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


// MissionsWindow::OnTouchBegin(int, int)
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


void  ArrayTB_ctor(void *self);
void  ArrayTB_setLength(int n, void *self);
void  ArrayImg_releaseClasses(void *self);
void *ArrayImg_dtor(void *self);

void  TouchButton_ctor(void *self, void *text, int kind, int x, int y, char a, char b);
void  TouchButton_ctorTab(void *self, void *text, int kind, int x, int y, char flags);

void  ScrollTouchWindow_ctor(void *self, int x, int y, int w, int h, bool b);
void *ScrollTouchWindow_dtor(void *self);

void *ChoiceWindow_ctor(void *self);
void *ChoiceWindow_dtor(void *self);

void *WantedWindow_ctor(void *self);

void  Layout_formatCredits(void *out, int credits);

void  String_fromC(void *s, const char *text, bool copy);
void  String_fromText(void *s, void *text, bool copy);
void  String_fromInt(void *s, int value);

int   Status_wantedBoardAccessible();
void  Status_replaceHash(void *out, void *key, void *a, void *b, void *c);

int   Mission_getType(void *mission);
int   Mission_getProductionGoodAmount(void *mission);
int   Mission_getStatusValue(void *mission);
void *Mission_getAgent(void *mission);
int   Mission_getReward(void *mission);
int   Mission_getBonus(void *mission);

void  Globals_getAgentMissionText(void *out, void *agent);


int   Ship_getIndex(void *ship);


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
    void *tabs = ::operator new(0xc);
    ArrayTB_ctor(tabs);
    pp(self, 0x14) = tabs;
    ArrayTB_setLength(2, tabs);

    void *b0 = ::operator new(200);
    void *t0 = ((GameText *)g_mw_gameText)->getText(titleId);
    int helpOff = ((Layout *)layout)->getHelpButtonOffset();
    TouchButton_ctorTab(b0, t0, 3, (i32(self, 0x38) + i32(self, 0x30)) - helpOff, i32(self, 0x34), 0x12);
    *(void **)(*(int *)(pp(self, 0x14)) + 4 + 4) = b0;

    void *b1 = ::operator new(200);
    void *t1 = ((GameText *)g_mw_gameText)->getText(titleId);
    int helpOff2 = ((Layout *)layout)->getHelpButtonOffset();
    int w1 = ((TouchButton *)(b1))->getWidth();
    TouchButton_ctorTab(b1, t1, 3,
                        (((i32(self, 0x38) + i32(self, 0x30)) - helpOff2) - w1) +
                            i32(layout, 0x38), i32(self, 0x34), 0x12);
    *(void **)(*(int *)(pp(self, 0x14)) + 4) = b1;
    ((TouchButton *)(*(void **)(*(int *)(pp(self, 0x14)) + 4)))->setAlwaysPressed(true);

    ((Layout *)(layout))->setWindowDimensions(i32(self, 0x30), i32(self, 0x34), i32(self, 0x38), i32(self, 0x3c));

    // --- tear down any previous sub-objects ---
    if (pp(self, 0x18) != 0) {
        ArrayImg_releaseClasses(pp(self, 0x18));
        if (pp(self, 0x18) != 0) ::operator delete(ArrayImg_dtor(pp(self, 0x18)));
    }
    pp(self, 0x18) = 0;
    if (pp(self, 0x0) != 0) ::operator delete(ScrollTouchWindow_dtor(pp(self, 0x0)));
    pp(self, 0x0) = 0;
    if (pp(self, 0x4) != 0) ::operator delete(ScrollTouchWindow_dtor(pp(self, 0x4)));
    pp(self, 0x4) = 0;
    if (pp(self, 0xc) != 0) ::operator delete(ChoiceWindow_dtor(pp(self, 0xc)));
    pp(self, 0xc) = 0;
    if (pp(self, 0x24) != 0) { void *b = pp(self, 0x24); ((TouchButton *)b)->dtor(); ::operator delete(b); }
    pp(self, 0x24) = 0;
    if (pp(self, 0x28) != 0) { void *b = pp(self, 0x28); ((TouchButton *)b)->dtor(); ::operator delete(b); }
    pp(self, 0x28) = 0;
    if (pp(self, 0x2c) != 0) { void *b = pp(self, 0x2c); ((TouchButton *)b)->dtor(); ::operator delete(b); }
    pp(self, 0x8) = 0;
    pp(self, 0x2c) = 0;
    i32(self, 0x20) = 0;

    // --- left (campaign) scroll window ---
    int topY = i32(layout, 0xc) + i32(self, 0x34) + i32(layout, 0x20) +
               i32(layout, 0x5c) + i32(layout, 0x2c);
    int reserve = (((Status *)(*(void **)g_mwi_status))->gameWon() == 0) ? i32(layout, 0x30) : 0;
    void *sw0 = ::operator new(0x24);
    ScrollTouchWindow_ctor(sw0,
        i32(layout, 0x28) + i32(self, 0x30), topY,
        (i32(self, 0x38) >> 1) - (i32(layout, 0x2c) + i32(layout, 0x28)),
        (((((i32(self, 0x34) - topY) + i32(self, 0x3c)) - i32(layout, 0x10)) -
          i32(layout, 0x24)) - reserve) + i32(layout, 0x2c) * -2, false);
    pp(self, 0x0) = sw0;

    // --- campaign-mission summary text ---
    bool campShow = (((Status *)(*(void **)g_mwi_status))->gameWon() == 0) ||
                    (*(char *)(*(char **)g_mwi_campaign + 0x37) != 0 ||
                     *(char *)(*(char **)g_mwi_campaign + 0x35) != 0);
    if (campShow) {
        char text[0xc];
        String_fromC(text, "", false);
        if (((Status *)(*(void **)g_mwi_status))->getCurrentCampaignMission() < 0xa4) {
            void *t = ((GameText *)g_mw_gameText)->getText(titleId);
            ((String *)(text))->assign((String *)t);
        }
        int type = Mission_getType((void *)(intptr_t)((Status *)(*(void **)g_mwi_status))->getCampaignMission());
        bool production = (type == 0xa7) || (Mission_getType((void *)(intptr_t)((Status *)(*(void **)g_mwi_status))->getCampaignMission()) == 0xae);
        void *key = *(void **)g_mwi_status;
        char hdr[0xc], val[0xc], suffix[0xc], merged[0xc];
        if (production) {
            String_fromText(hdr, text, false);
            int need = Mission_getProductionGoodAmount((void *)(intptr_t)((Status *)(*(void **)g_mwi_status))->getCampaignMission());
            int have = Mission_getStatusValue((void *)(intptr_t)((Status *)(*(void **)g_mwi_status))->getCampaignMission());
            String_fromInt(val, need - have);
            String_fromC(suffix, "", false);
            Status_replaceHash(merged, key, hdr, val, suffix);
        } else {
            String_fromText(hdr, text, false);
            ((Mission *)((void *)(intptr_t)((Status *)(*(void **)g_mwi_status))->getCampaignMission()))->getTargetStationName();
            String_fromC(suffix, "", false);
            Status_replaceHash(merged, key, hdr, val, suffix);
        }
        ((String *)(text))->assign((String *)merged);
        ((String *)(merged))->dtor(); ((String *)(suffix))->dtor(); ((String *)(val))->dtor(); ((String *)(hdr))->dtor();

        char a[0xc], b[0xc];
        String_fromC(a, "", false);
        String_fromText(b, text, false);
        ((ScrollTouchWindow *)(pp(self, 0x0)))->setText(*(String *)a, *(String *)b);
        ((String *)(b))->dtor(); ((String *)(a))->dtor();
        ((String *)(text))->dtor();
    } else {
        bool useGold = ((Achievements *)(*(void **)g_mw_ach))->gotAllGoldMedals() != 0 && Ship_getIndex(((Status *)(*(void **)g_mwi_status))->getShip()) != 8;
        char a[0xc], b[0xc];
        String_fromC(a, "", false);
        void *t = ((GameText *)g_mw_gameText)->getText(titleId);
        String_fromText(b, t, false);
        ((ScrollTouchWindow *)(pp(self, 0x0)))->setText(*(String *)a, *(String *)b);
        ((String *)(b))->dtor(); ((String *)(a))->dtor();
        (void)useGold;
    }

    // --- right (freelance) scroll window ---
    int fmEmpty = ((Mission *)(((Status *)(*(void **)g_mwi_status))->getFreelanceMission()))->isEmpty();
    void *sw1 = ::operator new(0x24);
    int half = i32(self, 0x38) >> 1;
    int pad = i32(layout, 0x2c);
    int rx = i32(self, 0x30) + half + pad;
    if (fmEmpty == 0) {
        int ry = i32(layout, 0x2d8) + topY + pad;
        ScrollTouchWindow_ctor(sw1, rx, ry, (half - pad) - i32(layout, 0x28),
            (((((i32(self, 0x34) - ry) + i32(self, 0x3c)) - i32(layout, 0x10)) -
              i32(layout, 0x24)) - i32(layout, 0x4c)) -
                i32(layout, 0x30), false);
        pp(self, 0x4) = sw1;

        char text[0xc], reward[0xc], suffix[0xc], merged[0xc];
        Globals_getAgentMissionText(text, Mission_getAgent(((Status *)(*(void **)g_mwi_status))->getFreelanceMission()));
        void *key = *(void **)g_mwi_status;
        char body[0xc];
        String_fromText(body, text, false);
        int rew = Mission_getReward(((Status *)(*(void **)g_mwi_status))->getFreelanceMission());
        int bonus = Mission_getBonus(((Status *)(*(void **)g_mwi_status))->getFreelanceMission());
        Layout_formatCredits(reward, rew + bonus);
        String_fromC(suffix, "", false);
        Status_replaceHash(merged, key, body, reward, suffix);
        ((String *)(text))->assign((String *)merged);
        ((String *)(merged))->dtor(); ((String *)(suffix))->dtor(); ((String *)(reward))->dtor(); ((String *)(body))->dtor();

        char a[0xc], b[0xc];
        String_fromC(a, "", false);
        String_fromText(b, text, false);
        ((ScrollTouchWindow *)(pp(self, 0x4)))->setText(*(String *)a, *(String *)b);
        ((String *)(b))->dtor(); ((String *)(a))->dtor();

        void *parts = ((Agent *)(Mission_getAgent(((Status *)(*(void **)g_mwi_status))->getFreelanceMission())))->getImageParts();
        pp(self, 0x18) = ((ImageFactory *)(*(void **)g_mwi_imageFactory))->loadChar((int *)parts);
        ((String *)(text))->dtor();
    } else {
        ScrollTouchWindow_ctor(sw1, rx, topY, (half - pad) - i32(layout, 0x28),
            ((i32(self, 0x3c) + (i32(self, 0x34) - (topY + pad * 2))) -
             i32(layout, 0x10)) - i32(layout, 0x24), false);
        pp(self, 0x4) = sw1;
        char a[0xc], b[0xc];
        String_fromC(a, "", false);
        void *t = ((GameText *)g_mw_gameText)->getText(titleId);
        String_fromText(b, t, false);
        ((ScrollTouchWindow *)(pp(self, 0x4)))->setText(*(String *)a, *(String *)b);
        ((String *)(b))->dtor(); ((String *)(a))->dtor();
    }

    // --- action buttons (Accept / Reject / Auto-pilot) ---
    if (((Status *)(*(void **)g_mwi_status))->inAlienOrbit() == 0) {
        int btnY = ((i32(self, 0x38) >> 1) >> 1) - i32(layout, 0x28);
        if (((Status *)(*(void **)g_mwi_status))->gameWon() == 0) {
            void *bAccept = ::operator new(200);
            void *t = ((GameText *)g_mw_gameText)->getText(titleId);
            TouchButton_ctor(bAccept, t, 0, i32(layout, 0x28) + i32(self, 0x30),
                             (((i32(self, 0x34) + i32(self, 0x3c)) - i32(layout, 0x10)) -
                              i32(layout, 0x24)) - i32(layout, 0x2c),
                             '!', 4);
            pp(self, 0x24) = bAccept;
        }
        if (((Mission *)(((Status *)(*(void **)g_mwi_status))->getFreelanceMission()))->isEmpty() == 0) {
            void *bReject = ::operator new(200);
            void *t = ((GameText *)g_mw_gameText)->getText(titleId);
            TouchButton_ctor(bReject, t, 0,
                             i32(self, 0x30) + (i32(self, 0x38) >> 1) + i32(layout, 0x2c),
                             (((i32(self, 0x34) - i32(layout, 0x2c)) + i32(self, 0x3c)) -
                              i32(layout, 0x10)) - i32(layout, 0x24),
                             '!', 4);
            pp(self, 0x28) = bReject;

            if (ApplicationManager_GetCurrentApplicationModule(*(void **)g_mwi_appMgr) == 5) {
                void *bMap = ::operator new(200);
                void *t2 = ((GameText *)g_mw_gameText)->getText(titleId);
                TouchButton_ctor(bMap, t2, 0,
                                 i32(self, 0x30) + btnY + (i32(self, 0x38) >> 1) +
                                     i32(layout, 0x2c) * 2,
                                 (((i32(self, 0x34) - i32(layout, 0x2c)) + i32(self, 0x3c)) -
                                  i32(layout, 0x10)) - i32(layout, 0x24),
                                 '!', 4);
                pp(self, 0x2c) = bMap;
                ((TouchButton *)(bMap))->setTextColor(g_mwi_actionColor);
            }
            void *cw = ::operator new(0x5c);
            ChoiceWindow_ctor(cw);
            pp(self, 0xc) = cw;
        }
    }

    i32(self, 0x40) = 0;
    u8(self, 0x23) = 0;

    if (Status_wantedBoardAccessible() != 0) {
        if (pp(self, 0x10) == 0) {
            void *ww = ::operator new(0xb4);
            WantedWindow_ctor(ww);
            pp(self, 0x10) = ww;
        } else {
            ((WantedWindow *)(pp(self, 0x10)))->init();
        }
    }

    
    return 0;
}

// ---- draw_1504dc.cpp ----
extern "C" {

// Two mode-specific draw tail calls (mode==1 / flag@0x22).
void MissionsWindow_drawWanted(void *self);   // DAT_1ac504 thunk
void MissionsWindow_drawStarMap(void *self);  // DAT_1ac274 thunk

void  Layout_drawHeader(void *layout, void *title);

void  String_fromC(void *s, const char *text, bool copy);
void  String_fromText(void *s, void *text, bool copy);

int   Status_wantedBoardAccessible();
// Draw uses the Status singleton via free-function helpers.

void *Mission_getAgent(void *mission);
int   Mission_getType(void *mission);



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

    if (i32(self, 0x40) == 1) { MissionsWindow_drawWanted(self);  return; }
    if (u8(self, 0x22) != 0)  { MissionsWindow_drawStarMap(self); return; }

    void *canvas = *(void **)g_mwd_canvas;
    void *layout = *(void **)g_mwd_layout;
    void *color = *(void **)g_mwd_color;
    void *font = *(void **)g_mwd_font;
    int titleId = *g_mwd_textId;

    ((PaintCanvas*)canvas)->SetColor((unsigned int)(uintptr_t)color);

    char header[0xc];
    void *ht = ((GameText *)g_mw_gameText)->getText(titleId);
    String_fromText(header, ht, false);
    Layout_drawHeader(layout, header);
    ((String *)(header))->dtor();

    if (Status_wantedBoardAccessible() != 0) {
        void **tabs = (void **)pp(self, 0x14);
        for (unsigned int i = 0; i < *(unsigned int *)tabs; i++)
            ((TouchButton *)(((void **)tabs[1])[i]))->draw();
    }

    int ox = i32(self, 0x30), oy = i32(self, 0x34);
    int ow = i32(self, 0x38), oh = i32(self, 0x3c);

    // Campaign-mission title box + body box.
    {
        char box[0xc];
        void *t = ((GameText *)g_mw_gameText)->getText(titleId);
        String_fromText(box, t, false);
        int c = i32(layout, 0xc), p20 = i32(layout, 0x20);
        int p28 = i32(layout, 0x28), p2c = i32(layout, 0x2c);
        ((Layout *)(layout))->drawBox(1, p28 + ox, oy + c + p20, (ow >> 1) - (p2c + p28), i32(layout, 0x5c), box, (unsigned)(uintptr_t)canvas);
        ((String *)(box))->dtor();
    }
    {
        char box[0xc];
        String_fromC(box, "", false);
        int c = i32(layout, 0xc), p10 = i32(layout, 0x10);
        int p20 = i32(layout, 0x20), p24 = i32(layout, 0x24);
        int p28 = i32(layout, 0x28), p2c = i32(layout, 0x2c);
        int p5c = i32(layout, 0x5c);
        ((Layout *)(layout))->drawBox(5, p28 + ox, oy + c + p20 + p5c + p2c, (ow >> 1) - (p2c + p28), ((oh - (p20 + c + p5c + p2c * 2)) - p10) - p24, box, (unsigned)(uintptr_t)canvas);
        ((String *)(box))->dtor();
    }

    ((ScrollTouchWindow *)(pp(self, 0x0)))->draw();
    if (pp(self, 0x24) != 0) ((TouchButton *)(pp(self, 0x24)))->draw();

    // Freelance-mission title + body box (right column).
    {
        char box[0xc];
        void *t = ((GameText *)g_mw_gameText)->getText(titleId);
        String_fromText(box, t, false);
        int c = i32(layout, 0xc), p20 = i32(layout, 0x20);
        int p28 = i32(layout, 0x28), p2c = i32(layout, 0x2c);
        ((Layout *)(layout))->drawBox(1, ox + (ow >> 1) + p2c, oy + c + p20, ((ow >> 1) - p2c) - p28, i32(layout, 0x5c), box, (unsigned)(uintptr_t)canvas);
        ((String *)(box))->dtor();
    }
    {
        char box[0xc];
        String_fromC(box, "", false);
        int c = i32(layout, 0xc), p10 = i32(layout, 0x10);
        int p20 = i32(layout, 0x20), p24 = i32(layout, 0x24);
        int p28 = i32(layout, 0x28), p2c = i32(layout, 0x2c);
        int p5c = i32(layout, 0x5c);
        ((Layout *)(layout))->drawBox(5, ox + (ow >> 1) + p2c, oy + p2c + c + p20 + p5c, ((ow >> 1) - p2c) - p28, ((oh - (c + p2c * 2 + p20 + p5c)) - p10) - p24, box, (unsigned)(uintptr_t)canvas);
        ((String *)(box))->dtor();
    }

    // Active freelance mission details.
    void *fm = ((Status *)(*(void **)g_mwi_status))->getFreelanceMission();
    if (fm != 0 && ((Mission *)(fm))->isEmpty() == 0 && pp(self, 0x18) != 0) {
        ((ImageFactory *)(*(void **)g_mwd_imageFactory))->drawChar((Arr *)pp(self, 0x18), ox + (ow >> 1) + i32(layout, 0x2c), i32(layout, 0x2c) + oy + i32(layout, 0xc) +
                                  i32(layout, 0x20) + i32(layout, 0x5c), false);

        int detailX = ox + (ow >> 1) + i32(layout, 0x2d4) +
                      i32(layout, 0x2c) * 2;
        int detailY = oy + i32(layout, 0xc) + i32(layout, 0x20) +
                      i32(layout, 0x2c) + i32(layout, 0x5c);

        char nameStr[0xc];
        ((Agent *)(Mission_getAgent(fm)))->getName();
        ((PaintCanvas*)canvas)->DrawString((unsigned int)(uintptr_t)font, (void*)nameStr, detailX, detailY, false);
        ((String *)(nameStr))->dtor();

        char stationStr[0xc];
        ((Agent *)(Mission_getAgent(fm)))->getStationName();
        ((PaintCanvas*)canvas)->DrawString((unsigned int)(uintptr_t)font, (void*)stationStr, detailX, detailY, false);
        ((String *)(stationStr))->dtor();

        void *typeTxt = ((GameText *)g_mw_gameText)->getText(Mission_getType(((Agent *)(Mission_getAgent(fm)))->getMission()) + 0x162);
        ((PaintCanvas*)canvas)->DrawString((unsigned int)(uintptr_t)font, (void*)typeTxt, detailX, detailY, false);
    }

    ((ScrollTouchWindow *)(pp(self, 0x4)))->draw();
    if (pp(self, 0x2c) != 0) ((TouchButton *)(pp(self, 0x2c)))->draw();
    if (pp(self, 0x28) != 0) ((TouchButton *)(pp(self, 0x28)))->draw();

    ((Layout *)(layout))->drawFooter();
    if (u8(self, 0x21) != 0 || u8(self, 0x20) != 0)
        ((ChoiceWindow *)(pp(self, 0xc)))->draw();

    
}

// ---- MissionsWindow_14fa10.cpp ----
extern "C" int   _mw_GetTextHeight(void *canvas);   // 0x6fe20
extern "C" void *_mw_init(void *self);              // 0x753d0

// Two PC-relative singletons; each holds a pointer (read via one extra deref).
// The second is loaded and dereferenced but its value is unused in the target,
// so we read it through a volatile lvalue to keep the load from being elided.
__attribute__((visibility("hidden"))) extern void **g_mw_a;
__attribute__((visibility("hidden"))) extern void **g_mw_b;


// MissionsWindow::MissionsWindow()
MissionsWindow::MissionsWindow()
{
    void *canvas = *g_mw_a;
    void *dead = *(void *volatile *)*g_mw_b;
    i32(this, 0x24) = 0;
    i32(this, 0x28) = 0;
    i32(this, 0x2c) = 0;
    Blk16 z = {0, 0, 0, 0};
    this->field_0xc = z;
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
void MissionsWindow::render3D()
{
    if (i32(this, 0x40) == 1)
        _mw_WantedWindow_render3D(pp(this, 0x10));
    if (u8(this, 0x22))
        return _mw_render3D_tail(pp(this, 0x8));
}

// ---- OnTouchEnd_150d74.cpp ----
extern "C" {


void StarMap_ctor(void *map, bool a, void *mission, bool b, int c);
// StarMap::OnTouchEnd returns void in the (offset-derived) header, but the original
// returns a hit flag here; declare the real ABI signature so the test compiles.
int  StarMap_OnTouchEnd(StarMap *map, int x, int y);


int  Status_wantedBoardAccessible();

void *Ship_getCargo(void *ship);
void  Ship_removeCargo(void *ship, void *item);


int  Mission_getType(void *mission);
void *Mission_getAgent(void *mission);


void String_fromText(void *s, void *text, bool copy);



int MissionsWindow_init(void *self);

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

    // Wanted-board sub-window active.
    if (i32(self, 0x40) == 1) {
        ((WantedWindow *)(pp(self, 0x10)))->OnTouchEnd(z);
        if (*(int *)pp(self, 0x10) == 0) {
            i32(self, 0x40) = 0;
            *(int *)pp(self, 0x10) = 1;
        }
        goto done;
    }

    if (u8(self, 0x20) != 0) {
        // Confirmation dialog open.
        int r = ((ChoiceWindow *)(pp(self, 0xc)))->OnTouchEnd(y, z);
        if (r == 1) {
            u8(self, 0x20) = 0;
            goto done;
        }
        if (r == 0) {
            // Confirmed: clear out the freelance-mission cargo/passengers and re-init.
            void *fsrc = *(void **)g_mwt_freelanceSrc;
            int type = Mission_getType(((Status *)(*(void **)g_mwt_freelanceSrc))->getFreelanceMission());
            bool clearCargo = (type == 0);
            if (!clearCargo && Mission_getType(((Status *)(*(void **)g_mwt_freelanceSrc))->getFreelanceMission()) == 3) clearCargo = true;
            if (!clearCargo && Mission_getType(((Status *)(*(void **)g_mwt_freelanceSrc))->getFreelanceMission()) == 5) clearCargo = true;
            if (clearCargo) {
                void *cargo = Ship_getCargo(((Status *)(*(void **)g_mwt_freelanceSrc))->getShip());
                if (cargo != 0) {
                    unsigned int *c = (unsigned int *)cargo;
                    for (unsigned int i = 0; i < *c; i++) {
                        void *item = *(void **)(c[1] + i * 4);
                        if (((Item *)(item))->isUnsaleable() != 0 && ((Item *)(item))->getIndex() == 0x74) {
                            Ship_removeCargo(((Status *)(*(void **)g_mwt_freelanceSrc))->getShip(), item);
                            u8(self, 0x23) = 1;
                            break;
                        }
                    }
                }
            } else if (Mission_getType(((Status *)(*(void **)g_mwt_freelanceSrc))->getFreelanceMission()) == 0xb) {
                ((Status *)(fsrc))->setPassengers(0);
            }

            void *agent = Mission_getAgent(((Status *)(*(void **)g_mwt_freelanceSrc))->getFreelanceMission());
            if (((Agent *)(agent))->isGenericAgent() == 0)
                ((Agent *)(Mission_getAgent(((Status *)(*(void **)g_mwt_freelanceSrc))->getFreelanceMission())))->setOfferAccepted(false);
            // NOTE: decompiler dropped the Mission* argument; reset path clears the slot.
            ((Status *)(fsrc))->setFreelanceMission((Mission *)0);

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
                if (((TouchButton *)(((void **)tabs[1])[i]))->OnTouchEnd(z) != 0)
                    u32(self, 0x40) = i;
            }
        }
        ((ScrollTouchWindow *)(pp(self, 0x0)))->OnTouchEnd(y, z);
        ((ScrollTouchWindow *)(pp(self, 0x4)))->OnTouchEnd(y, z);

        if (pp(self, 0x24) != 0 && ((TouchButton *)(pp(self, 0x24)))->OnTouchEnd(z) != 0) {
            // "Show on map" for the campaign mission.
            void *appMgr = *(void **)g_mwt_appMgr;
            void *mod = ((ApplicationManager *)(appMgr))->GetApplicationModule(5);
            void *map = *(void **)((char *)mod + 0x10);
            pp(self, 0x8) = map;
            if (map == 0) {
                void *m = ::operator new(0x1e8);
                StarMap_ctor(m, true, (void *)(intptr_t)((Status *)(*(void **)g_mwt_freelanceSrc))->getCampaignMission(), false, -1);
                void *mod2 = ((ApplicationManager *)(appMgr))->GetApplicationModule(5);
                *(void **)((char *)mod2 + 0x10) = m;
                void *mod3 = ((ApplicationManager *)(appMgr))->GetApplicationModule(5);
                pp(self, 0x8) = *(void **)((char *)mod3 + 0x10);
            } else {
                ((StarMap *)(map))->init(true, (Mission *)(void *)(intptr_t)((Status *)(*(void **)g_mwt_freelanceSrc))->getCampaignMission(), false, -1);
            }
            u8(self, 0x22) = 1;
            ((Layout *)(*(void **)g_mwt_resetLayout))->resetWindowDimensions();
        } else {
            if (pp(self, 0x2c) != 0 && ((TouchButton *)(pp(self, 0x2c)))->OnTouchEnd(z) != 0) {
                void *cw = pp(self, 0xc);
                void *t = ((GameText *)g_mw_gameText)->getText(0x1a2);
                ((ChoiceWindow *)(cw))->set(*(String *)t, true);
                u8(self, 0x20) = 1;
            }
            if (pp(self, 0x28) != 0 && ((TouchButton *)(pp(self, 0x28)))->OnTouchEnd(z) != 0) {
                // "Show on map" for the freelance mission.
                void *appMgr = *(void **)g_mwt_appMgr;
                void *mod = ((ApplicationManager *)(appMgr))->GetApplicationModule(5);
                void *map = *(void **)((char *)mod + 0x10);
                pp(self, 0x8) = map;
                if (map == 0) {
                    void *m = ::operator new(0x1e8);
                    StarMap_ctor(m, true, ((Status *)(*(void **)g_mwt_freelanceSrc))->getFreelanceMission(), false, -1);
                    void *mod2 = ((ApplicationManager *)(appMgr))->GetApplicationModule(5);
                    *(void **)((char *)mod2 + 0x10) = m;
                    pp(self, 0x8) = m;
                } else {
                    ((StarMap *)(map))->init(true, ((Status *)(*(void **)g_mwt_freelanceSrc))->getFreelanceMission(), false, -1);
                }
                u8(self, 0x22) = 1;
                ((Layout *)(*(void **)g_mwt_resetLayout))->resetWindowDimensions();
                goto done;
            }
            void *layout = *(void **)g_mwt_layout;
            if (((Layout *)(layout))->OnTouchEnd(z, 0) != 0) {
                ((Layout *)(layout))->resetWindowDimensions();
                goto done;
            }
            if (((Layout *)(layout))->helpPressed() != 0) {
                char title[0xc];
                void *t = ((GameText *)g_mw_gameText)->getText(0x27b);
                String_fromText(title, t, false);
                ((Layout *)(layout))->initHelpWindow(title);
                ((String *)(title))->dtor();
            }
        }
    } else {
        // StarMap overlay is showing.
        if (StarMap_OnTouchEnd((StarMap *)(pp(self, 0x8)), y, z) != 0) {
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
    
}

// ---- update_1509b0.cpp ----
extern "C" {

// Two mode-specific tail calls reached via GOT thunks.
void MissionsWindow_acceptAction(void *self);   // DAT_1ac514 thunk (mode == 1)
void MissionsWindow_cancelAction(void *self);   // DAT_1ac284 thunk (flag @0x22)



int   Mission_getType(void *mission);
int   Mission_getProductionGoodAmount(void *mission);
int   Mission_getStatusValue(void *mission);

void  String_fromC(void *s, const char *text, bool copy);
void  String_fromText(void *s, void *text, bool copy);
void  String_fromInt(void *s, int value);


void  Status_replaceHash(void *out, void *key, void *a, void *b, void *c);


extern void *g_mw_status;        // *(DAT_160bbc): Status singleton
extern void *g_mw_campaign;      // *(DAT_160bc0): campaign-state record
extern int  *g_mw_textBase;      // *(DAT_160bc8): GameText id base for mission titles
extern int  *g_mw_titleTable;    // *(DAT_160bcc): per-mission title id table
extern void *g_mw_hashSource;    // *(DAT_160bbc): replaceHash key source (== status)
}

// MissionsWindow::update(int dt)
extern "C" void MissionsWindow_update(void *self, int dt)
{

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

    ((ScrollTouchWindow *)(pp(self, 0x0)))->update(dt);
    ((ScrollTouchWindow *)(pp(self, 0x4)))->update(dt);

    void *status = *(void **)g_mw_status;
    int type = Mission_getType((void *)(intptr_t)((Status *)(*(void **)g_mw_status))->getCampaignMission());
    bool relevant = (type == 0xa7);
    if (!relevant) {
        if (Mission_getType((void *)(intptr_t)((Status *)(*(void **)g_mw_status))->getCampaignMission()) == 0xae)
            relevant = true;
    }

    if (relevant) {
        void *camp = *(void **)g_mw_campaign;
        bool show = (((Status *)(*(void **)g_mw_status))->gameWon() == 0) ||
                    (*(char *)((char *)camp + 0x37) != 0 || *(char *)((char *)camp + 0x35) != 0);
        if (show) {
            char text[0xc];
            String_fromC(text, "", false);
            if (((Status *)(*(void **)g_mw_status))->getCurrentCampaignMission() < 0xa4) {
                int base = *g_mw_textBase;
                void *titleTxt = ((GameText *)g_mw_gameText)->getText(g_mw_titleTable[((Status *)(*(void **)g_mw_status))->getCurrentCampaignMission()]);
                (void)base;
                ((String *)(text))->assign((String *)titleTxt);

                void *key = *(void **)g_mw_hashSource;
                char hdr[0xc], amount[0xc], suffix[0xc], merged[0xc];
                String_fromText(hdr, text, false);
                int need = Mission_getProductionGoodAmount((void *)(intptr_t)((Status *)(*(void **)g_mw_status))->getCampaignMission());
                int have = Mission_getStatusValue((void *)(intptr_t)((Status *)(*(void **)g_mw_status))->getCampaignMission());
                String_fromInt(amount, need - have);
                String_fromC(suffix, "", false);
                Status_replaceHash(merged, key, hdr, amount, suffix);
                ((String *)(text))->assign((String *)merged);
                ((String *)(merged))->dtor(); ((String *)(suffix))->dtor(); ((String *)(amount))->dtor(); ((String *)(hdr))->dtor();

                void *win = pp(self, 0x0);
                char a[0xc], b[0xc];
                String_fromC(a, "", false);
                String_fromText(b, text, false);
                ((ScrollTouchWindow *)(win))->setText(*(String *)a, *(String *)b);
                ((String *)(b))->dtor();
                ((String *)(a))->dtor();
            }
            ((String *)(text))->dtor();
        }
    }

    // Highlight the selected tab.
    void **tabs = (void **)pp(self, 0x14);
    for (unsigned int i = 0; i < *(unsigned int *)tabs; i++)
        ((TouchButton *)(((void **)tabs[1])[i]))->setAlwaysPressed(i == u32(self, 0x40));

    
}
