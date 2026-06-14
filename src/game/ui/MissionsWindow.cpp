#include "gof2/game/ui/MissionsWindow.h"
#include "gof2/game/ship/Ship.h"
#include "gof2/game/core/PaintCanvasClass.h"
#include "gof2/game/ui/ChoiceWindow.h"
#include "gof2/game/mission/Item.h"
#include "gof2/game/ui/ScrollTouchWindow.h"
#include "gof2/game/world/StarMap.h"
#include "gof2/game/mission/Status.h"
#include "gof2/engine/core/ApplicationManager.h"
#include "gof2/engine/core/GameText.h"
#include "gof2/engine/render/ImageFactory.h"
#include "gof2/game/ui/Layout.h"
#include "gof2/game/core/String.h"
#include "gof2/game/mission/Achievements.h"

// String is now centralized (gof2/common.h), so the full type headers below coexist
// without the historical "redefinition of String" collision.
#include "gof2/game/ui/TouchButton.h"
#include "gof2/game/mission/Mission.h"
#include "gof2/game/ship/Agent.h"
#include "gof2/game/ui/WantedWindow.h"

// GameText singleton: getText(int key) is a member of the GameText table object. The
// decompiler folded the table pointer and the key into a single (GameText*)key cast and
// dropped the explicit key argument. The active GameText table is reachable through these
// hidden PC-relative globals (one per recovered function), used here only as the receiver.
__attribute__((visibility("hidden"))) extern void *g_mw_gameText;
// Achievements singleton holder (PC-relative DAT, same global address as gG_achievements /
// g_ch_ach). The decompiler folded the receiver into a 0-arg call; recovered as
// (*(Achievements **)g_mw_ach)->gotAllGoldMedals().
__attribute__((visibility("hidden"))) extern void *g_mw_ach;

extern "C" int  _mw_m_tail(void *obj, int p1, int p2);          // 0x1ac528 tail
extern "C" void _mw_m_Choice(void *w, int p1, int p2);
extern "C" void _mw_m_StarMap(void *w, int p1, int p2);
extern "C" int  _mw_m_wantedBoardAccessible(void *st);
extern "C" void _mw_m_Button(void *btn, int p1, int p2);
extern "C" void _mw_m_Layout(void *l, int p1, int p2);
extern "C" void _mw_m_STW(void *w, int p1, int p2);

__attribute__((visibility("hidden"))) extern void **g_mw_m_status;
__attribute__((visibility("hidden"))) extern void **g_mw_m_layout;

// MissionsWindow::OnTouchMove(int, int)
int MissionsWindow::OnTouchMove(int p1, int p2)
{
    if (this->m_mode == 1)
        return _mw_m_tail(this->m_pWantedWindow, p1, p2);
    if (this->m_choiceActive) {
        _mw_m_Choice(this->m_pChoiceWindow, p1, p2);
    } else if (this->m_starMapActive) {
        _mw_m_StarMap(this->m_pStarMap, p1, p2);
    } else {
        if (_mw_m_wantedBoardAccessible(*g_mw_m_status)) {
            Array<TouchButton *> *arr = this->m_pTabButtons;
            for (unsigned i = 0; i < arr->size(); i++)
                _mw_m_Button((*arr)[i], p1, p2);
        }
        _mw_m_Layout(*g_mw_m_layout, p1, p2);
        _mw_m_STW(this->m_pCampaignWindow, p1, p2);
        _mw_m_STW(this->m_pFreelanceWindow, p1, p2);
        if (this->m_pAcceptButton) _mw_m_Button(this->m_pAcceptButton, p1, p2);
        if (this->m_pMapButton) _mw_m_Button(this->m_pMapButton, p1, p2);
        if (this->m_pRejectButton) _mw_m_Button(this->m_pRejectButton, p1, p2);
    }
    return 0;
}

// MissionsWindow::setHangarUpdate(bool) -- store the flag byte at +0x23.
void MissionsWindow::setHangarUpdate(bool v)
{
    this->m_hangarNeedsUpdate = v;
}

extern "C" void *_mw_STW_dtor(void *w);
extern "C" void *_mw_ChoiceWindow_dtor(void *w);
extern "C" void *_mw_TouchButton_dtor(void *b);
extern "C" void *_mw_WantedWindow_dtor(void *w);

// MissionsWindow::~MissionsWindow()
MissionsWindow::~MissionsWindow()
{
    if (this->m_pAgentImageParts) {
        for (ImagePart *e : *this->m_pAgentImageParts) delete e;
        this->m_pAgentImageParts->clear();
        delete this->m_pAgentImageParts;
    }
    this->m_pAgentImageParts = 0;
    if (this->m_pTabButtons) {
        for (TouchButton *e : *this->m_pTabButtons) delete e;
        this->m_pTabButtons->clear();
        delete this->m_pTabButtons;
    }
    this->m_pTabButtons = 0;
    if (this->m_pCampaignWindow) operator delete(_mw_STW_dtor(this->m_pCampaignWindow));
    this->m_pCampaignWindow = 0;
    if (this->m_pFreelanceWindow) operator delete(_mw_STW_dtor(this->m_pFreelanceWindow));
    this->m_pFreelanceWindow = 0;
    if (this->m_pChoiceWindow) operator delete(_mw_ChoiceWindow_dtor(this->m_pChoiceWindow));
    this->m_pChoiceWindow = 0;
    if (this->m_pAcceptButton) operator delete(_mw_TouchButton_dtor(this->m_pAcceptButton));
    this->m_pAcceptButton = 0;
    if (this->m_pRejectButton) operator delete(_mw_TouchButton_dtor(this->m_pRejectButton));
    this->m_pRejectButton = 0;
    if (this->m_pMapButton) operator delete(_mw_TouchButton_dtor(this->m_pMapButton));
    this->m_pMapButton = 0;
    if (this->m_pWantedWindow) operator delete(_mw_WantedWindow_dtor(this->m_pWantedWindow));
    this->m_pWantedWindow = 0;
}

// MissionsWindow::hangarNeedsUpdate() -- raw uint8 getter at +0x23.
uint8_t MissionsWindow::hangarNeedsUpdate()
{
    return this->m_hangarNeedsUpdate;
}

extern "C" int  _mw_b_tail(void *obj, int p1, int p2);          // 0x1ac518 tail
extern "C" void _mw_b_Choice(void *w, int p1, int p2);
extern "C" void _mw_b_StarMap(void *w, int p1, int p2);
extern "C" int  _mw_b_wantedBoardAccessible(void *st);
extern "C" void _mw_b_Button(void *btn, int p1, int p2);
extern "C" void _mw_b_Layout(void *l, int p1, int p2);
extern "C" void _mw_b_STW(void *w, int p1, int p2);

__attribute__((visibility("hidden"))) extern void **g_mw_b_status;
__attribute__((visibility("hidden"))) extern void **g_mw_b_layout;

// MissionsWindow::OnTouchBegin(int, int)
int MissionsWindow::OnTouchBegin(int p1, int p2)
{
    if (this->m_mode == 1)
        return _mw_b_tail(this->m_pWantedWindow, p1, p2);
    if (this->m_choiceActive) {
        _mw_b_Choice(this->m_pChoiceWindow, p1, p2);
    } else if (this->m_starMapActive) {
        _mw_b_StarMap(this->m_pStarMap, p1, p2);
    } else {
        if (_mw_b_wantedBoardAccessible(*g_mw_b_status)) {
            Array<TouchButton *> *arr = this->m_pTabButtons;
            for (unsigned i = 0; i < arr->size(); i++)
                _mw_b_Button((*arr)[i], p1, p2);
        }
        _mw_b_Layout(*g_mw_b_layout, p1, p2);
        _mw_b_STW(this->m_pCampaignWindow, p1, p2);
        _mw_b_STW(this->m_pFreelanceWindow, p1, p2);
        if (this->m_pAcceptButton) _mw_b_Button(this->m_pAcceptButton, p1, p2);
        if (this->m_pMapButton) _mw_b_Button(this->m_pMapButton, p1, p2);
        if (this->m_pRejectButton) _mw_b_Button(this->m_pRejectButton, p1, p2);
    }
    return 0;
}

extern "C" {

void  TouchButton_ctorTab(void *self, void *text, int kind, int x, int y, char flags);

void  ScrollTouchWindow_ctor(void *self, int x, int y, int w, int h, bool b);
void *ScrollTouchWindow_dtor(void *self);

void *ChoiceWindow_ctor(void *self);
void *ChoiceWindow_dtor(void *self);

void *WantedWindow_ctor(void *self);

void  Layout_formatCredits(void *out, int credits);

void  Status_replaceHash(void *out, void *key, void *a, void *b, void *c);

void  Globals_getAgentMissionText(void *out, void *agent);

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
int MissionsWindow::init() {
    void *layout = *(void **)g_mwi_layout;
    int titleId = *g_mwi_titleTable;

    // --- window geometry (orientation / state dependent) ---
    if (*g_mwi_flagA == 0) {
        this->m_x = 0;
        this->m_y = 0;
        this->m_width = *g_mwi_screenW;
        this->m_height = *g_mwi_screenH;
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
        this->m_width = h;
        this->m_height = w;
        this->m_x = (*g_mwi_screenW >> 1) - (h >> 1);
        this->m_y = (*g_mwi_screenH >> 1) - (w >> 1);
    }

    // --- two tab buttons ---
    Array<TouchButton *> *tabs = new Array<TouchButton *>();
    this->m_pTabButtons = tabs;
    tabs->resize(2);

    void *b0 = ::operator new(200);
    void *t0 = ((GameText *)g_mw_gameText)->getText(titleId);
    int helpOff = ((Layout *)layout)->getHelpButtonOffset();
    TouchButton_ctorTab(b0, t0, 3, (this->m_width + this->m_x) - helpOff, this->m_y, 0x12);
    (*tabs)[1] = (TouchButton *)b0;

    void *b1 = ::operator new(200);
    void *t1 = ((GameText *)g_mw_gameText)->getText(titleId);
    int helpOff2 = ((Layout *)layout)->getHelpButtonOffset();
    int w1 = ((TouchButton *)(b1))->getWidth();
    TouchButton_ctorTab(b1, t1, 3,
                        (((this->m_width + this->m_x) - helpOff2) - w1) +
                            ((Layout *)layout)->field_0x38, this->m_y, 0x12);
    (*tabs)[0] = (TouchButton *)b1;
    (*tabs)[0]->setAlwaysPressed(true);

    ((Layout *)(layout))->setWindowDimensions(this->m_x, this->m_y, this->m_width, this->m_height);

    // --- tear down any previous sub-objects ---
    if (this->m_pAgentImageParts != 0) {
        for (ImagePart *e : *this->m_pAgentImageParts) delete e;
        this->m_pAgentImageParts->clear();
        delete this->m_pAgentImageParts;
    }
    this->m_pAgentImageParts = 0;
    if (this->m_pCampaignWindow != 0) ::operator delete(ScrollTouchWindow_dtor(this->m_pCampaignWindow));
    this->m_pCampaignWindow = 0;
    if (this->m_pFreelanceWindow != 0) ::operator delete(ScrollTouchWindow_dtor(this->m_pFreelanceWindow));
    this->m_pFreelanceWindow = 0;
    if (this->m_pChoiceWindow != 0) ::operator delete(ChoiceWindow_dtor(this->m_pChoiceWindow));
    this->m_pChoiceWindow = 0;
    if (this->m_pAcceptButton != 0) { void *b = this->m_pAcceptButton; ((TouchButton *)b)->~TouchButton(); ::operator delete(b); }
    this->m_pAcceptButton = 0;
    if (this->m_pRejectButton != 0) { void *b = this->m_pRejectButton; ((TouchButton *)b)->~TouchButton(); ::operator delete(b); }
    this->m_pRejectButton = 0;
    if (this->m_pMapButton != 0) { void *b = this->m_pMapButton; ((TouchButton *)b)->~TouchButton(); ::operator delete(b); }
    this->m_pStarMap = 0;
    this->m_pMapButton = 0;
    this->m_choiceActive = 0;

    // --- left (campaign) scroll window ---
    int topY = ((Layout *)layout)->field_0xc + this->m_y + ((Layout *)layout)->field_0x20 +
               ((Layout *)layout)->field_0x5c + ((Layout *)layout)->field_0x2c;
    int reserve = (((Status *)(*(void **)g_mwi_status))->gameWon() == 0) ? ((Layout *)layout)->field_0x30 : 0;
    void *sw0 = ::operator new(0x24);
    ScrollTouchWindow_ctor(sw0,
        ((Layout *)layout)->buttonInsetX + this->m_x, topY,
        (this->m_width >> 1) - (((Layout *)layout)->field_0x2c + ((Layout *)layout)->buttonInsetX),
        (((((this->m_y - topY) + this->m_height) - ((Layout *)layout)->field_0x10) -
          ((Layout *)layout)->field_0x24) - reserve) + ((Layout *)layout)->field_0x2c * -2, false);
    this->m_pCampaignWindow = (ScrollTouchWindow *)sw0;

    // --- campaign-mission summary text ---
    // RAWREAD: g_mwi_campaign+0x37, +0x35 (campaign-state record is an untyped void* global; no modeled class)
    bool campShow = (((Status *)(*(void **)g_mwi_status))->gameWon() == 0) ||
                    (*(char *)(*(char **)g_mwi_campaign + 0x37) != 0 ||
                     *(char *)(*(char **)g_mwi_campaign + 0x35) != 0);
    if (campShow) {
        char text[0xc];
        ((String *)(text))->ctor_char("", false);
        if (((Status *)(*(void **)g_mwi_status))->getCurrentCampaignMission() < 0xa4) {
            void *t = ((GameText *)g_mw_gameText)->getText(titleId);
            ((String *)(text))->assign((String *)t);
        }
        int type = ((Mission *)((void *)(intptr_t)((Status *)(*(void **)g_mwi_status))->getCampaignMission()))->getType();
        bool production = (type == 0xa7) || (((Mission *)((void *)(intptr_t)((Status *)(*(void **)g_mwi_status))->getCampaignMission()))->getType() == 0xae);
        void *key = *(void **)g_mwi_status;
        char hdr[0xc], val[0xc], suffix[0xc], merged[0xc];
        if (production) {
            ((String *)(hdr))->ctor_copy((String *)(text), false);
            int need = ((Mission *)((void *)(intptr_t)((Status *)(*(void **)g_mwi_status))->getCampaignMission()))->getProductionGoodAmount();
            int have = ((Mission *)((void *)(intptr_t)((Status *)(*(void **)g_mwi_status))->getCampaignMission()))->getStatusValue();
            ((String *)(val))->ctor_int(need - have);
            ((String *)(suffix))->ctor_char("", false);
            Status_replaceHash(merged, key, hdr, val, suffix);
        } else {
            ((String *)(hdr))->ctor_copy((String *)(text), false);
            ((Mission *)((void *)(intptr_t)((Status *)(*(void **)g_mwi_status))->getCampaignMission()))->getTargetStationName();
            ((String *)(suffix))->ctor_char("", false);
            Status_replaceHash(merged, key, hdr, val, suffix);
        }
        ((String *)(text))->assign((String *)merged);
        ((String *)(merged))->dtor(); ((String *)(suffix))->dtor(); ((String *)(val))->dtor(); ((String *)(hdr))->dtor();

        char a[0xc], b[0xc];
        ((String *)(a))->ctor_char("", false);
        ((String *)(b))->ctor_copy((String *)(text), false);
        this->m_pCampaignWindow->setText(*(String *)a, *(String *)b);
        ((String *)(b))->dtor(); ((String *)(a))->dtor();
        ((String *)(text))->dtor();
    } else {
        bool useGold = ((Achievements *)(*(void **)g_mw_ach))->gotAllGoldMedals() != 0 && ((Ship *)(((Status *)(*(void **)g_mwi_status))->getShip()))->getIndex() != 8;
        char a[0xc], b[0xc];
        ((String *)(a))->ctor_char("", false);
        void *t = ((GameText *)g_mw_gameText)->getText(titleId);
        ((String *)(b))->ctor_copy((String *)(t), false);
        this->m_pCampaignWindow->setText(*(String *)a, *(String *)b);
        ((String *)(b))->dtor(); ((String *)(a))->dtor();
        (void)useGold;
    }

    // --- right (freelance) scroll window ---
    int fmEmpty = ((Mission *)(((Status *)(*(void **)g_mwi_status))->getFreelanceMission()))->isEmpty();
    void *sw1 = ::operator new(0x24);
    int half = this->m_width >> 1;
    int pad = ((Layout *)layout)->field_0x2c;
    int rx = this->m_x + half + pad;
    if (fmEmpty == 0) {
        int ry = ((Layout *)layout)->field_0x2d8 + topY + pad;
        ScrollTouchWindow_ctor(sw1, rx, ry, (half - pad) - ((Layout *)layout)->buttonInsetX,
            (((((this->m_y - ry) + this->m_height) - ((Layout *)layout)->field_0x10) -
              ((Layout *)layout)->field_0x24) - ((Layout *)layout)->field_0x4c) -
                ((Layout *)layout)->field_0x30, false);
        this->m_pFreelanceWindow = (ScrollTouchWindow *)sw1;

        char text[0xc], reward[0xc], suffix[0xc], merged[0xc];
        Globals_getAgentMissionText(text, ((Mission *)(((Status *)(*(void **)g_mwi_status))->getFreelanceMission()))->getAgent());
        void *key = *(void **)g_mwi_status;
        char body[0xc];
        ((String *)(body))->ctor_copy((String *)(text), false);
        int rew = ((Mission *)(((Status *)(*(void **)g_mwi_status))->getFreelanceMission()))->getReward();
        int bonus = ((Mission *)(((Status *)(*(void **)g_mwi_status))->getFreelanceMission()))->getBonus();
        Layout_formatCredits(reward, rew + bonus);
        ((String *)(suffix))->ctor_char("", false);
        Status_replaceHash(merged, key, body, reward, suffix);
        ((String *)(text))->assign((String *)merged);
        ((String *)(merged))->dtor(); ((String *)(suffix))->dtor(); ((String *)(reward))->dtor(); ((String *)(body))->dtor();

        char a[0xc], b[0xc];
        ((String *)(a))->ctor_char("", false);
        ((String *)(b))->ctor_copy((String *)(text), false);
        this->m_pFreelanceWindow->setText(*(String *)a, *(String *)b);
        ((String *)(b))->dtor(); ((String *)(a))->dtor();

        void *parts = ((Agent *)(((Mission *)(((Status *)(*(void **)g_mwi_status))->getFreelanceMission()))->getAgent()))->getImageParts();
        this->m_pAgentImageParts = ((ImageFactory *)(*(void **)g_mwi_imageFactory))->loadChar((int *)parts);
        ((String *)(text))->dtor();
    } else {
        ScrollTouchWindow_ctor(sw1, rx, topY, (half - pad) - ((Layout *)layout)->buttonInsetX,
            ((this->m_height + (this->m_y - (topY + pad * 2))) -
             ((Layout *)layout)->field_0x10) - ((Layout *)layout)->field_0x24, false);
        this->m_pFreelanceWindow = (ScrollTouchWindow *)sw1;
        char a[0xc], b[0xc];
        ((String *)(a))->ctor_char("", false);
        void *t = ((GameText *)g_mw_gameText)->getText(titleId);
        ((String *)(b))->ctor_copy((String *)(t), false);
        this->m_pFreelanceWindow->setText(*(String *)a, *(String *)b);
        ((String *)(b))->dtor(); ((String *)(a))->dtor();
    }

    // --- action buttons (Accept / Reject / Auto-pilot) ---
    if (((Status *)(*(void **)g_mwi_status))->inAlienOrbit() == 0) {
        int btnY = ((this->m_width >> 1) >> 1) - ((Layout *)layout)->buttonInsetX;
        if (((Status *)(*(void **)g_mwi_status))->gameWon() == 0) {
            void *bAccept = ::operator new(200);
            void *t = ((GameText *)g_mw_gameText)->getText(titleId);
            new ((bAccept)) TouchButton((String *)t, 0, ((Layout *)layout)->buttonInsetX + this->m_x, (((this->m_y + this->m_height) - ((Layout *)layout)->field_0x10) -
                              ((Layout *)layout)->field_0x24) - ((Layout *)layout)->field_0x2c, btnY, '!', 4);  // width=btnY recovered via Ghidra
            this->m_pAcceptButton = (TouchButton *)bAccept;
        }
        if (((Mission *)(((Status *)(*(void **)g_mwi_status))->getFreelanceMission()))->isEmpty() == 0) {
            void *bReject = ::operator new(200);
            void *t = ((GameText *)g_mw_gameText)->getText(titleId);
            new ((bReject)) TouchButton((String *)t, 0, this->m_x + (this->m_width >> 1) + ((Layout *)layout)->field_0x2c, (((this->m_y - ((Layout *)layout)->field_0x2c) + this->m_height) -
                              ((Layout *)layout)->field_0x10) - ((Layout *)layout)->field_0x24, btnY, '!', 4);  // width=btnY recovered via Ghidra
            this->m_pRejectButton = (TouchButton *)bReject;

            if (ApplicationManager_GetCurrentApplicationModule(*(void **)g_mwi_appMgr) == 5) {
                void *bMap = ::operator new(200);
                void *t2 = ((GameText *)g_mw_gameText)->getText(titleId);
                new ((bMap)) TouchButton((String *)t2, 0, this->m_x + btnY + (this->m_width >> 1) +
                                     ((Layout *)layout)->field_0x2c * 2, (((this->m_y - ((Layout *)layout)->field_0x2c) + this->m_height) -
                                  ((Layout *)layout)->field_0x10) - ((Layout *)layout)->field_0x24, btnY, '!', 4);  // width=btnY recovered via Ghidra
                this->m_pMapButton = (TouchButton *)bMap;
                ((TouchButton *)(bMap))->setTextColor(g_mwi_actionColor);
            }
            void *cw = ::operator new(0x5c);
            ChoiceWindow_ctor(cw);
            this->m_pChoiceWindow = (ChoiceWindow *)cw;
        }
    }

    this->m_mode = 0;
    this->m_hangarNeedsUpdate = 0;

    if (((Status *)(*(void **)g_mwi_status))->wantedBoardAccessible() != 0) {
        if (this->m_pWantedWindow == 0) {
            void *ww = ::operator new(0xb4);
            WantedWindow_ctor(ww);
            this->m_pWantedWindow = (WantedWindow *)ww;
        } else {
            this->m_pWantedWindow->init();
        }
    }

    
    return 0;
}

extern "C" {

// Two mode-specific draw tail calls (mode==1 / flag@0x22).
void MissionsWindow_drawWanted(void *self);   // DAT_1ac504 thunk

void  Layout_drawHeader(void *layout, void *title);

// Draw uses the Status singleton via free-function helpers.

extern void *g_mwd_canvas;    // *(DAT_16059c): paint canvas
extern void *g_mwd_layout;    // *(DAT_1605a0): Layout singleton
extern int  *g_mwd_textId;    // *(DAT_1605a4): GameText id base
extern void *g_mwd_color;     // *(DAT_1605a8): box colour
extern void *g_mwd_imageFactory; // *(DAT_1609a0): image factory
extern void *g_mwd_font;      // *(DAT_1609a4): default font
}

// MissionsWindow::draw()
void MissionsWindow::draw() {
    if (this->m_mode == 1) { MissionsWindow_drawWanted(this);  return; }
    if (this->m_starMapActive != 0)  { ((MissionsWindow *)(this))->drawStarMap(); return; }

    void *canvas = *(void **)g_mwd_canvas;
    void *layout = *(void **)g_mwd_layout;
    void *color = *(void **)g_mwd_color;
    void *font = *(void **)g_mwd_font;
    int titleId = *g_mwd_textId;

    ((PaintCanvas*)canvas)->SetColor((unsigned int)(uintptr_t)color);

    char header[0xc];
    void *ht = ((GameText *)g_mw_gameText)->getText(titleId);
    ((String *)(header))->ctor_copy((String *)(ht), false);
    Layout_drawHeader(layout, header);
    ((String *)(header))->dtor();

    if (((Status *)(*(void **)g_mwi_status))->wantedBoardAccessible() != 0) {
        Array<TouchButton *> *tabs = this->m_pTabButtons;
        for (unsigned int i = 0; i < tabs->size(); i++)
            (*tabs)[i]->draw();
    }

    int ox = this->m_x, oy = this->m_y;
    int ow = this->m_width, oh = this->m_height;

    // Campaign-mission title box + body box.
    {
        char box[0xc];
        void *t = ((GameText *)g_mw_gameText)->getText(titleId);
        ((String *)(box))->ctor_copy((String *)(t), false);
        int c = ((Layout *)layout)->field_0xc, p20 = ((Layout *)layout)->field_0x20;
        int p28 = ((Layout *)layout)->buttonInsetX, p2c = ((Layout *)layout)->field_0x2c;
        ((Layout *)(layout))->drawBox(1, p28 + ox, oy + c + p20, (ow >> 1) - (p2c + p28), ((Layout *)layout)->field_0x5c, box, (unsigned)(uintptr_t)canvas);
        ((String *)(box))->dtor();
    }
    {
        char box[0xc];
        ((String *)(box))->ctor_char("", false);
        int c = ((Layout *)layout)->field_0xc, p10 = ((Layout *)layout)->field_0x10;
        int p20 = ((Layout *)layout)->field_0x20, p24 = ((Layout *)layout)->field_0x24;
        int p28 = ((Layout *)layout)->buttonInsetX, p2c = ((Layout *)layout)->field_0x2c;
        int p5c = ((Layout *)layout)->field_0x5c;
        ((Layout *)(layout))->drawBox(5, p28 + ox, oy + c + p20 + p5c + p2c, (ow >> 1) - (p2c + p28), ((oh - (p20 + c + p5c + p2c * 2)) - p10) - p24, box, (unsigned)(uintptr_t)canvas);
        ((String *)(box))->dtor();
    }

    this->m_pCampaignWindow->draw();
    if (this->m_pAcceptButton != 0) this->m_pAcceptButton->draw();

    // Freelance-mission title + body box (right column).
    {
        char box[0xc];
        void *t = ((GameText *)g_mw_gameText)->getText(titleId);
        ((String *)(box))->ctor_copy((String *)(t), false);
        int c = ((Layout *)layout)->field_0xc, p20 = ((Layout *)layout)->field_0x20;
        int p28 = ((Layout *)layout)->buttonInsetX, p2c = ((Layout *)layout)->field_0x2c;
        ((Layout *)(layout))->drawBox(1, ox + (ow >> 1) + p2c, oy + c + p20, ((ow >> 1) - p2c) - p28, ((Layout *)layout)->field_0x5c, box, (unsigned)(uintptr_t)canvas);
        ((String *)(box))->dtor();
    }
    {
        char box[0xc];
        ((String *)(box))->ctor_char("", false);
        int c = ((Layout *)layout)->field_0xc, p10 = ((Layout *)layout)->field_0x10;
        int p20 = ((Layout *)layout)->field_0x20, p24 = ((Layout *)layout)->field_0x24;
        int p28 = ((Layout *)layout)->buttonInsetX, p2c = ((Layout *)layout)->field_0x2c;
        int p5c = ((Layout *)layout)->field_0x5c;
        ((Layout *)(layout))->drawBox(5, ox + (ow >> 1) + p2c, oy + p2c + c + p20 + p5c, ((ow >> 1) - p2c) - p28, ((oh - (c + p2c * 2 + p20 + p5c)) - p10) - p24, box, (unsigned)(uintptr_t)canvas);
        ((String *)(box))->dtor();
    }

    // Active freelance mission details.
    void *fm = ((Status *)(*(void **)g_mwi_status))->getFreelanceMission();
    if (fm != 0 && ((Mission *)(fm))->isEmpty() == 0 && this->m_pAgentImageParts != 0) {
        ((ImageFactory *)(*(void **)g_mwd_imageFactory))->drawChar((Array<ImagePart *> *)this->m_pAgentImageParts, ox + (ow >> 1) + ((Layout *)layout)->field_0x2c, ((Layout *)layout)->field_0x2c + oy + ((Layout *)layout)->field_0xc +
                                  ((Layout *)layout)->field_0x20 + ((Layout *)layout)->field_0x5c, false);

        int detailX = ox + (ow >> 1) + ((Layout *)layout)->field_0x2d4 +
                      ((Layout *)layout)->field_0x2c * 2;
        int detailY = oy + ((Layout *)layout)->field_0xc + ((Layout *)layout)->field_0x20 +
                      ((Layout *)layout)->field_0x2c + ((Layout *)layout)->field_0x5c;

        char nameStr[0xc];
        ((Agent *)(((Mission *)(fm))->getAgent()))->getName();
        ((PaintCanvas*)canvas)->DrawString((unsigned int)(uintptr_t)font, (void*)nameStr, detailX, detailY, false);
        ((String *)(nameStr))->dtor();

        char stationStr[0xc];
        ((Agent *)(((Mission *)(fm))->getAgent()))->getStationName();
        ((PaintCanvas*)canvas)->DrawString((unsigned int)(uintptr_t)font, (void*)stationStr, detailX, detailY, false);
        ((String *)(stationStr))->dtor();

        void *typeTxt = ((GameText *)g_mw_gameText)->getText(((Mission *)(((Agent *)(((Mission *)(fm))->getAgent()))->getMission()))->getType() + 0x162);
        ((PaintCanvas*)canvas)->DrawString((unsigned int)(uintptr_t)font, (void*)typeTxt, detailX, detailY, false);
    }

    this->m_pFreelanceWindow->draw();
    if (this->m_pMapButton != 0) this->m_pMapButton->draw();
    if (this->m_pRejectButton != 0) this->m_pRejectButton->draw();

    ((Layout *)(layout))->drawFooter();
    if (this->m_field_0x21 != 0 || this->m_choiceActive != 0)
        this->m_pChoiceWindow->draw();

    
}

extern "C" int   _mw_GetTextHeight(void *canvas);
extern "C" void *_mw_init(void *self);

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
    this->m_pAcceptButton = 0;
    this->m_pRejectButton = 0;
    this->m_pMapButton = 0;
    // Bulk-clear of the sub-object pointer block (+0x0..+0x18) in the original ctor.
    this->m_pCampaignWindow = 0;
    this->m_pFreelanceWindow = 0;
    this->m_pStarMap = 0;
    this->m_pChoiceWindow = 0;
    this->m_pWantedWindow = 0;
    this->m_pTabButtons = 0;
    this->m_pAgentImageParts = 0;
    (void)dead;
    int h = _mw_GetTextHeight(canvas);
    this->m_textHalfHeight = h / 2 - 1;
    _mw_init(this);
}

extern "C" void _mw_WantedWindow_render3D(void *ww);
extern "C" void _mw_render3D_tail(void *obj);          // 0x1ac258 tail

// MissionsWindow::render3D()
void MissionsWindow::render3D()
{
    if (this->m_mode == 1)
        _mw_WantedWindow_render3D(this->m_pWantedWindow);
    if (this->m_starMapActive)
        return _mw_render3D_tail(this->m_pStarMap);
}

extern "C" {

void StarMap_ctor(void *map, bool a, void *mission, bool b, int c);
// StarMap::OnTouchEnd returns void in the (offset-derived) header, but the original
// returns a hit flag here; declare the real ABI signature so the test compiles.
int  StarMap_OnTouchEnd(StarMap *map, int x, int y);

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
    MissionsWindow *win = (MissionsWindow *)self;

    // Wanted-board sub-window active.
    if (win->m_mode == 1) {
        win->m_pWantedWindow->OnTouchEnd(y, z);
        if (*(int *)win->m_pWantedWindow == 0) {
            win->m_mode = 0;
            *(int *)win->m_pWantedWindow = 1;
        }
        goto done;
    }

    if (win->m_choiceActive != 0) {
        // Confirmation dialog open.
        int r = win->m_pChoiceWindow->OnTouchEnd(y, z);
        if (r == 1) {
            win->m_choiceActive = 0;
            goto done;
        }
        if (r == 0) {
            // Confirmed: clear out the freelance-mission cargo/passengers and re-init.
            void *fsrc = *(void **)g_mwt_freelanceSrc;
            int type = ((Mission *)(((Status *)(*(void **)g_mwt_freelanceSrc))->getFreelanceMission()))->getType();
            bool clearCargo = (type == 0);
            if (!clearCargo && ((Mission *)(((Status *)(*(void **)g_mwt_freelanceSrc))->getFreelanceMission()))->getType() == 3) clearCargo = true;
            if (!clearCargo && ((Mission *)(((Status *)(*(void **)g_mwt_freelanceSrc))->getFreelanceMission()))->getType() == 5) clearCargo = true;
            if (clearCargo) {
                void *cargo = ((Ship *)(((Status *)(*(void **)g_mwt_freelanceSrc))->getShip()))->getCargo();
                if (cargo != 0) {
                    unsigned int *c = (unsigned int *)cargo;
                    for (unsigned int i = 0; i < *c; i++) {
                        void *item = *(void **)(c[1] + i * 4);
                        if (((Item *)(item))->isUnsaleable() != 0 && ((Item *)(item))->getIndex() == 0x74) {
                            ((Ship *)(((Status *)(*(void **)g_mwt_freelanceSrc))->getShip()))->removeCargo((Item *)item);
                            win->m_hangarNeedsUpdate = 1;
                            break;
                        }
                    }
                }
            } else if (((Mission *)(((Status *)(*(void **)g_mwt_freelanceSrc))->getFreelanceMission()))->getType() == 0xb) {
                ((Status *)(fsrc))->setPassengers(0);
            }

            void *agent = ((Mission *)(((Status *)(*(void **)g_mwt_freelanceSrc))->getFreelanceMission()))->getAgent();
            if (((Agent *)(agent))->isGenericAgent() == 0)
                ((Agent *)(((Mission *)(((Status *)(*(void **)g_mwt_freelanceSrc))->getFreelanceMission()))->getAgent()))->setOfferAccepted(false);
            // NOTE: decompiler dropped the Mission* argument; reset path clears the slot.
            ((Status *)(fsrc))->setFreelanceMission((Mission *)0);

            unsigned char savedFlag = win->m_hangarNeedsUpdate;
            ((MissionsWindow *)(self))->init();
            win->m_hangarNeedsUpdate = savedFlag;
            goto done;
        }
        // r != 0 and != 1: fall through to the normal touch handling.
    }

    // Normal (no dialog) path, unless StarMap is showing.
    if (win->m_starMapActive == 0) {
        if (((Status *)(*(void **)g_mwi_status))->wantedBoardAccessible() != 0) {
            Array<TouchButton *> *tabs = win->m_pTabButtons;
            for (unsigned int i = 0; i < tabs->size(); i++) {
                if ((*tabs)[i]->OnTouchEnd(y, z) != 0)
                    win->m_mode = (int)i;
            }
        }
        win->m_pCampaignWindow->OnTouchEnd(y, z);
        win->m_pFreelanceWindow->OnTouchEnd(y, z);

        if (win->m_pAcceptButton != 0 && win->m_pAcceptButton->OnTouchEnd(y, z) != 0) {
            // "Show on map" for the campaign mission.
            void *appMgr = *(void **)g_mwt_appMgr;
            void *mod = ((ApplicationManager *)(appMgr))->GetApplicationModule(5);
            // RAWREAD: mod+0x10 (GetApplicationModule returns void*; StarMap-module subtype's cached StarMap* slot is unmodeled)
            void *map = *(void **)((char *)mod + 0x10);
            win->m_pStarMap = (StarMap *)map;
            if (map == 0) {
                void *m = ::operator new(0x1e8);
                StarMap_ctor(m, true, (void *)(intptr_t)((Status *)(*(void **)g_mwt_freelanceSrc))->getCampaignMission(), false, -1);
                void *mod2 = ((ApplicationManager *)(appMgr))->GetApplicationModule(5);
                // RAWREAD: mod2+0x10 (unmodeled StarMap-module cache slot)
                *(void **)((char *)mod2 + 0x10) = m;
                void *mod3 = ((ApplicationManager *)(appMgr))->GetApplicationModule(5);
                // RAWREAD: mod3+0x10 (unmodeled StarMap-module cache slot)
                win->m_pStarMap = (StarMap *)*(void **)((char *)mod3 + 0x10);
            } else {
                ((StarMap *)(map))->init(true, (Mission *)(void *)(intptr_t)((Status *)(*(void **)g_mwt_freelanceSrc))->getCampaignMission(), false, -1);
            }
            win->m_starMapActive = 1;
            ((Layout *)(*(void **)g_mwt_resetLayout))->resetWindowDimensions();
        } else {
            if (win->m_pMapButton != 0 && win->m_pMapButton->OnTouchEnd(y, z) != 0) {
                void *cw = win->m_pChoiceWindow;
                void *t = ((GameText *)g_mw_gameText)->getText(0x1a2);
                ((ChoiceWindow *)(cw))->set(*(String *)t, true);
                win->m_choiceActive = 1;
            }
            if (win->m_pRejectButton != 0 && win->m_pRejectButton->OnTouchEnd(y, z) != 0) {
                // "Show on map" for the freelance mission.
                void *appMgr = *(void **)g_mwt_appMgr;
                void *mod = ((ApplicationManager *)(appMgr))->GetApplicationModule(5);
                // RAWREAD: mod+0x10 (GetApplicationModule returns void*; StarMap-module subtype's cached StarMap* slot is unmodeled)
                void *map = *(void **)((char *)mod + 0x10);
                win->m_pStarMap = (StarMap *)map;
                if (map == 0) {
                    void *m = ::operator new(0x1e8);
                    StarMap_ctor(m, true, ((Status *)(*(void **)g_mwt_freelanceSrc))->getFreelanceMission(), false, -1);
                    void *mod2 = ((ApplicationManager *)(appMgr))->GetApplicationModule(5);
                    // RAWREAD: mod2+0x10 (unmodeled StarMap-module cache slot)
                    *(void **)((char *)mod2 + 0x10) = m;
                    win->m_pStarMap = (StarMap *)m;
                } else {
                    ((StarMap *)(map))->init(true, ((Status *)(*(void **)g_mwt_freelanceSrc))->getFreelanceMission(), false, -1);
                }
                win->m_starMapActive = 1;
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
                ((String *)(title))->ctor_copy((String *)(t), false);
                ((Layout *)(layout))->initHelpWindow(title);
                ((String *)(title))->dtor();
            }
        }
    } else {
        // StarMap overlay is showing.
        if (StarMap_OnTouchEnd(win->m_pStarMap, y, z) != 0) {
            int wantW, wantH, posX;
            if (*g_mwt_flagA == 0) {
                win->m_x = 0;
                win->m_y = 0;
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
                win->m_x = (*g_mwt_screenW >> 1) - posX;
                win->m_y = (*g_mwt_screenH >> 1) - (wantW >> 1);
            }
            win->m_width = wantH;
            win->m_height = wantW;
            win->m_starMapActive = 0;
        }
    }

done:
    
}

extern "C" {

void  Status_replaceHash(void *out, void *key, void *a, void *b, void *c);

extern void *g_mw_status;        // *(DAT_160bbc): Status singleton
extern void *g_mw_campaign;      // *(DAT_160bc0): campaign-state record
extern int  *g_mw_textBase;      // *(DAT_160bc8): GameText id base for mission titles
extern int  *g_mw_titleTable;    // *(DAT_160bcc): per-mission title id table
extern void *g_mw_hashSource;    // *(DAT_160bbc): replaceHash key source (== status)
}

// MissionsWindow::update(int dt)
void MissionsWindow::update(int dt) {

    // Mode 1: confirm/accept and bail out.
    if (this->m_mode == 1) {
        ((MissionsWindow *)(this))->acceptAction();
        return;
    }
    // Cancel flag set: cancel and bail out.
    if (this->m_starMapActive != 0) {
        ((MissionsWindow *)(this))->cancelAction();
        return;
    }

    this->m_pCampaignWindow->update(dt);
    this->m_pFreelanceWindow->update(dt);

    void *status = *(void **)g_mw_status;
    int type = ((Mission *)((void *)(intptr_t)((Status *)(*(void **)g_mw_status))->getCampaignMission()))->getType();
    bool relevant = (type == 0xa7);
    if (!relevant) {
        if (((Mission *)((void *)(intptr_t)((Status *)(*(void **)g_mw_status))->getCampaignMission()))->getType() == 0xae)
            relevant = true;
    }

    if (relevant) {
        void *camp = *(void **)g_mw_campaign;
        // RAWREAD: camp+0x37, +0x35 (campaign-state record is an untyped void* global; no modeled class)
        bool show = (((Status *)(*(void **)g_mw_status))->gameWon() == 0) ||
                    (*(char *)((char *)camp + 0x37) != 0 || *(char *)((char *)camp + 0x35) != 0);
        if (show) {
            char text[0xc];
            ((String *)(text))->ctor_char("", false);
            if (((Status *)(*(void **)g_mw_status))->getCurrentCampaignMission() < 0xa4) {
                int base = *g_mw_textBase;
                void *titleTxt = ((GameText *)g_mw_gameText)->getText(g_mw_titleTable[((Status *)(*(void **)g_mw_status))->getCurrentCampaignMission()]);
                (void)base;
                ((String *)(text))->assign((String *)titleTxt);

                void *key = *(void **)g_mw_hashSource;
                char hdr[0xc], amount[0xc], suffix[0xc], merged[0xc];
                ((String *)(hdr))->ctor_copy((String *)(text), false);
                int need = ((Mission *)((void *)(intptr_t)((Status *)(*(void **)g_mw_status))->getCampaignMission()))->getProductionGoodAmount();
                int have = ((Mission *)((void *)(intptr_t)((Status *)(*(void **)g_mw_status))->getCampaignMission()))->getStatusValue();
                ((String *)(amount))->ctor_int(need - have);
                ((String *)(suffix))->ctor_char("", false);
                Status_replaceHash(merged, key, hdr, amount, suffix);
                ((String *)(text))->assign((String *)merged);
                ((String *)(merged))->dtor(); ((String *)(suffix))->dtor(); ((String *)(amount))->dtor(); ((String *)(hdr))->dtor();

                ScrollTouchWindow *campWin = this->m_pCampaignWindow;
                char a[0xc], b[0xc];
                ((String *)(a))->ctor_char("", false);
                ((String *)(b))->ctor_copy((String *)(text), false);
                campWin->setText(*(String *)a, *(String *)b);
                ((String *)(b))->dtor();
                ((String *)(a))->dtor();
            }
            ((String *)(text))->dtor();
        }
    }

    // Highlight the selected tab.
    Array<TouchButton *> *tabs = this->m_pTabButtons;
    for (unsigned int i = 0; i < tabs->size(); i++)
        (*tabs)[i]->setAlwaysPressed((int)i == this->m_mode);

}

// ---- mode-specific sub-window dispatchers ----
// draw() and update() short-circuit into one of these whenever an overlay is
// active.  Each is a thin forward to the matching sub-window member -- the
// original target was a PLT/tail thunk through the GOT, exactly as render3D()
// forwards into WantedWindow::render3D.  The wanted board lives at +0x10 and is
// active while m_mode (+0x40) == 1; the star-map overlay lives at +0x8 and is
// active while the star-map flag (+0x22) is set.

// MissionsWindow::drawWanted() -- paint the wanted-board sub-window.
void MissionsWindow::drawWanted()
{
    this->m_pWantedWindow->draw();
}

// MissionsWindow::drawStarMap() -- paint the star-map overlay.
void MissionsWindow::drawStarMap()
{
    this->m_pStarMap->draw();
}

// MissionsWindow::acceptAction() -- advance the wanted-board sub-window one frame.
void MissionsWindow::acceptAction()
{
    this->m_pWantedWindow->update(0);
}

// MissionsWindow::cancelAction() -- advance the star-map overlay one frame.
void MissionsWindow::cancelAction()
{
    this->m_pStarMap->update(0);
}

// ---- public member faces over the recovered window logic ----
// These thin members expose the already-recovered window behaviour (rebuild
// layout, paint, dispatch a touch-release, advance one frame) through the real
// MissionsWindow:: interface used by the rest of the engine.

// MissionsWindow::OnTouchEnd(int, int) -- route a touch-release to the active sub-view.
void MissionsWindow::OnTouchEnd(int y, int z)
{
    MissionsWindow_OnTouchEnd(this, y, z);
}

// MissionsWindow_dtor -- C-ABI complete-object destructor wrapper: run the real
// destructor and hand the storage back to the caller for release.
extern "C" void *MissionsWindow_dtor(void *p)
{
    if (p) ((MissionsWindow *)p)->~MissionsWindow();
    return p;
}
