#include "game/ui/MissionsWindow.h"
#include "game/ship/Ship.h"
#include "game/ship/Agent.h"
#include "game/core/PaintCanvasClass.h"
#include "game/ui/ChoiceWindow.h"
#include "game/ui/ScrollTouchWindow.h"
#include "game/ui/TouchButton.h"
#include "game/ui/WantedWindow.h"
#include "game/ui/Layout.h"
#include "game/mission/Item.h"
#include "game/mission/Mission.h"
#include "game/mission/Status.h"
#include "game/mission/Achievements.h"
#include "game/core/Globals.h"
#include "game/world/StarMap.h"
#include "game/core/String.h"
#include "engine/core/ApplicationManager.h"
#include "engine/core/GameText.h"
#include "engine/render/ImageFactory.h"
#include "engine/render/ImagePart.h"

using AbyssEngine::String;

// Engine singletons reached through PC-relative globals. The decompiler folded the
// active-table pointer and the lookup key into a single receiver cast and could not
// recover a typed owner, so they are referenced here only through these handles.
extern GameText *g_mw_gameText;    // active GameText table
extern PaintCanvas* gCanvas;       // canonical render canvas singleton (binary .bss 0x2281b8)

extern Layout **g_mw_m_layout;     // Layout singleton (OnTouchMove)
extern Layout **g_mw_b_layout;     // Layout singleton (OnTouchBegin)

extern Layout **g_mwi_layout;      // Layout singleton (init)
extern int  *g_mwi_titleTable;     // GameText id base (init)
extern char *g_mwi_flagA;          // layout-state flag A
extern char *g_mwi_flagB;          // layout-state flag B
extern char *g_mwi_flagC;          // layout-state flag C
extern int  *g_mwi_screenW;        // screen width
extern int  *g_mwi_screenH;        // screen height
extern void *g_mwi_campaign;       // campaign-state record (untyped)
extern ImageFactory **g_mwi_imageFactory; // image factory
extern int   g_mwi_actionColor;    // action-button text colour

extern Layout **g_mwd_layout;      // Layout singleton (draw)
extern int  *g_mwd_textId;         // GameText id base (draw)
extern void *g_mwd_color;          // box colour
extern ImageFactory **g_mwd_imageFactory; // image factory (draw)
extern void *g_mwd_font;           // default font

extern char *g_mwt_flagA;          // layout-state flag A (touch)
extern char *g_mwt_flagB;          // layout-state flag B (touch)
extern char *g_mwt_flagC;          // layout-state flag C (touch)
extern int  *g_mwt_screenW;        // screen width (touch)
extern int  *g_mwt_screenH;        // screen height (touch)
extern Layout **g_mwt_layout;      // Layout singleton (touch)
extern Layout **g_mwt_resetLayout; // layout for resetWindowDimensions

extern void *g_mw_campaign;        // campaign-state record (update)
extern int  *g_mw_textBase;        // GameText id base (update)
extern int  *g_mw_titleTable;      // per-mission title id table (update)

// Engine String-plumbing helpers the decompiler folded into free calls over raw
// String storage; they wrap the corresponding Status/Layout/Globals members.
extern "C" void Status_replaceHash(void *out, void *key, void *a, void *b, void *c);

extern "C" int  ApplicationManager_GetCurrentApplicationModule(void *appMgr);
extern "C" int  _mw_GetTextHeight(void *canvas);

// The two campaign/freelance tab buttons are built through this dedicated engine
// ctor variant (kind/flags-style "tab" button); it has no clean public overload, so
// it stays a typed constructor helper over freshly allocated TouchButton storage.
extern "C" void TouchButton_ctorTab(void *self, void *text, int kind, int x, int y, char flags);

// StarMap::OnTouchEnd is typed void in the header but the shipped binary returns a
// "hit" flag here; this thin wrapper exposes the real return so the dispatch reads it.
extern "C" int StarMap_OnTouchEnd(StarMap *map, int x, int y);

int MissionsWindow::OnTouchMove(int p1, int p2)
{
    if (this->m_mode == 1)
        return this->m_pWantedWindow->OnTouchMove(p1, p2);
    if (this->m_choiceActive) {
        this->m_pChoiceWindow->OnTouchMove(p1, p2);
    } else if (this->m_starMapActive) {
        this->m_pStarMap->OnTouchMove(p1, p2);
    } else {
        if (gStatus->wantedBoardAccessible()) {
            Array<TouchButton *> *arr = this->m_pTabButtons;
            for (unsigned i = 0; i < arr->size(); i++)
                (*arr)[i]->OnTouchMove(p1, p2);
        }
        (*g_mw_m_layout)->OnTouchMove(p1, p2);
        this->m_pCampaignWindow->OnTouchMove(p1, p2);
        this->m_pFreelanceWindow->OnTouchMove(p1, p2);
        if (this->m_pAcceptButton) this->m_pAcceptButton->OnTouchMove(p1, p2);
        if (this->m_pMapButton)    this->m_pMapButton->OnTouchMove(p1, p2);
        if (this->m_pRejectButton) this->m_pRejectButton->OnTouchMove(p1, p2);
    }
    return 0;
}

int MissionsWindow::OnTouchBegin(int p1, int p2)
{
    if (this->m_mode == 1)
        return this->m_pWantedWindow->OnTouchBegin(p1, p2);
    if (this->m_choiceActive) {
        this->m_pChoiceWindow->OnTouchBegin(p1, p2);
    } else if (this->m_starMapActive) {
        this->m_pStarMap->OnTouchBegin(p1, p2);
    } else {
        if (gStatus->wantedBoardAccessible()) {
            Array<TouchButton *> *arr = this->m_pTabButtons;
            for (unsigned i = 0; i < arr->size(); i++)
                (*arr)[i]->OnTouchBegin(p1, p2);
        }
        (*g_mw_b_layout)->OnTouchBegin(p1, p2);
        this->m_pCampaignWindow->OnTouchBegin(p1, p2);
        this->m_pFreelanceWindow->OnTouchBegin(p1, p2);
        if (this->m_pAcceptButton) this->m_pAcceptButton->OnTouchBegin(p1, p2);
        if (this->m_pMapButton)    this->m_pMapButton->OnTouchBegin(p1, p2);
        if (this->m_pRejectButton) this->m_pRejectButton->OnTouchBegin(p1, p2);
    }
    return 0;
}

void MissionsWindow::setHangarUpdate(bool v)
{
    this->m_hangarNeedsUpdate = v;
}

uint8_t MissionsWindow::hangarNeedsUpdate()
{
    return this->m_hangarNeedsUpdate;
}

MissionsWindow::MissionsWindow()
{
    PaintCanvas *canvas = gCanvas;
    this->m_pAcceptButton = nullptr;
    this->m_pRejectButton = nullptr;
    this->m_pMapButton = nullptr;
    this->m_pCampaignWindow = nullptr;
    this->m_pFreelanceWindow = nullptr;
    this->m_pStarMap = nullptr;
    this->m_pChoiceWindow = nullptr;
    this->m_pWantedWindow = nullptr;
    this->m_pTabButtons = nullptr;
    this->m_pAgentImageParts = nullptr;
    int h = _mw_GetTextHeight(canvas);
    this->m_textHalfHeight = h / 2 - 1;
    this->init();
}

MissionsWindow::~MissionsWindow()
{
    if (this->m_pAgentImageParts) {
        for (ImagePart *e : *this->m_pAgentImageParts) delete e;
        this->m_pAgentImageParts->clear();
        delete this->m_pAgentImageParts;
    }
    this->m_pAgentImageParts = nullptr;
    if (this->m_pTabButtons) {
        for (TouchButton *e : *this->m_pTabButtons) delete e;
        this->m_pTabButtons->clear();
        delete this->m_pTabButtons;
    }
    this->m_pTabButtons = nullptr;
    delete this->m_pCampaignWindow;  this->m_pCampaignWindow = nullptr;
    delete this->m_pFreelanceWindow; this->m_pFreelanceWindow = nullptr;
    delete this->m_pChoiceWindow;    this->m_pChoiceWindow = nullptr;
    delete this->m_pAcceptButton;    this->m_pAcceptButton = nullptr;
    delete this->m_pRejectButton;    this->m_pRejectButton = nullptr;
    delete this->m_pMapButton;       this->m_pMapButton = nullptr;
    delete this->m_pWantedWindow;    this->m_pWantedWindow = nullptr;
}

int MissionsWindow::init()
{
    Layout *L = *g_mwi_layout;
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

    TouchButton *tab1 = (TouchButton *)::operator new(sizeof(TouchButton));
    String *t0 = g_mw_gameText->getText(titleId);
    int helpOff = L->getHelpButtonOffset();
    TouchButton_ctorTab(tab1, t0, 3, (this->m_width + this->m_x) - helpOff, this->m_y, 0x12);
    (*tabs)[1] = tab1;

    TouchButton *tab0 = (TouchButton *)::operator new(sizeof(TouchButton));
    String *t1 = g_mw_gameText->getText(titleId);
    int helpOff2 = L->getHelpButtonOffset();
    int w1 = tab0->getWidth();
    TouchButton_ctorTab(tab0, t1, 3,
                        (((this->m_width + this->m_x) - helpOff2) - w1) + L->field_0x38,
                        this->m_y, 0x12);
    (*tabs)[0] = tab0;
    tab0->setAlwaysPressed(true);

    L->setWindowDimensions(this->m_x, this->m_y, this->m_width, this->m_height);

    // --- tear down any previous sub-objects ---
    if (this->m_pAgentImageParts) {
        for (ImagePart *e : *this->m_pAgentImageParts) delete e;
        this->m_pAgentImageParts->clear();
        delete this->m_pAgentImageParts;
    }
    this->m_pAgentImageParts = nullptr;
    delete this->m_pCampaignWindow;  this->m_pCampaignWindow = nullptr;
    delete this->m_pFreelanceWindow; this->m_pFreelanceWindow = nullptr;
    delete this->m_pChoiceWindow;    this->m_pChoiceWindow = nullptr;
    delete this->m_pAcceptButton;    this->m_pAcceptButton = nullptr;
    delete this->m_pRejectButton;    this->m_pRejectButton = nullptr;
    delete this->m_pMapButton;
    this->m_pStarMap = nullptr;
    this->m_pMapButton = nullptr;
    this->m_choiceActive = 0;

    // --- left (campaign) scroll window ---
    int topY = L->field_0xc + this->m_y + L->field_0x20 + L->field_0x5c + L->field_0x2c;
    int reserve = (gStatus->gameWon() == 0) ? L->field_0x30 : 0;
    this->m_pCampaignWindow = new ScrollTouchWindow(
        L->buttonInsetX + this->m_x, topY,
        (this->m_width >> 1) - (L->field_0x2c + L->buttonInsetX),
        (((((this->m_y - topY) + this->m_height) - L->field_0x10) - L->field_0x24) - reserve)
            + L->field_0x2c * -2, false);

    // --- campaign-mission summary text ---
    bool campShow = (gStatus->gameWon() == 0) ||
                    (*(char *)(*(char **)g_mwi_campaign + 0x37) != 0 ||
                     *(char *)(*(char **)g_mwi_campaign + 0x35) != 0);
    if (campShow) {
        String text("", false);
        if (gStatus->getCurrentCampaignMission() < 0xa4) {
            String *t = g_mw_gameText->getText(titleId);
            text = *t;
        }
        void *key = gStatus;
        Mission *cm = (Mission *)((void *)(intptr_t)gStatus->getCampaignMission());
        int type = cm->getType();
        bool production = (type == 0xa7) || (cm->getType() == 0xae);
        String suffix("", false);
        String merged;
        if (production) {
            String hdr(text);
            int need = cm->getProductionGoodAmount();
            int have = cm->getStatusValue();
            String val; val.ctor_int(need - have);
            Status_replaceHash(&merged, key, &hdr, &val, &suffix);
        } else {
            String hdr(text);
            cm->getTargetStationName();
            String val;
            Status_replaceHash(&merged, key, &hdr, &val, &suffix);
        }
        text = merged;

        String a("", false);
        String b(text);
        this->m_pCampaignWindow->setText(a, b);
    } else {
        bool useGold = gAchievements->gotAllGoldMedals() != 0 &&
                       ((Ship *)(gStatus->getShip()))->getIndex() != 8;
        String a("", false);
        String *t = g_mw_gameText->getText(titleId);
        String b(*t);
        this->m_pCampaignWindow->setText(a, b);
        (void)useGold;
    }

    // --- right (freelance) scroll window ---
    int fmEmpty = gStatus->getFreelanceMission()->isEmpty();
    int half = this->m_width >> 1;
    int pad = L->field_0x2c;
    int rx = this->m_x + half + pad;
    if (fmEmpty == 0) {
        int ry = L->field_0x2d8 + topY + pad;
        this->m_pFreelanceWindow = new ScrollTouchWindow(
            rx, ry, (half - pad) - L->buttonInsetX,
            (((((this->m_y - ry) + this->m_height) - L->field_0x10) - L->field_0x24) - L->field_0x4c)
                - L->field_0x30, false);

        Mission *fm = gStatus->getFreelanceMission();
        String text = gGlobals->getAgentMissionText(fm->getAgent());
        void *key = gStatus;
        String body(text);
        int rew = fm->getReward();
        int bonus = fm->getBonus();
        String reward = Layout::formatCredits(rew + bonus);
        String suffix("", false);
        String merged;
        Status_replaceHash(&merged, key, &body, &reward, &suffix);
        text = merged;

        String a("", false);
        String b(text);
        this->m_pFreelanceWindow->setText(a, b);

        int *parts = ((Agent *)(fm->getAgent()))->getImageParts();
        this->m_pAgentImageParts = (*g_mwi_imageFactory)->loadChar(parts);
    } else {
        this->m_pFreelanceWindow = new ScrollTouchWindow(
            rx, topY, (half - pad) - L->buttonInsetX,
            ((this->m_height + (this->m_y - (topY + pad * 2))) - L->field_0x10) - L->field_0x24, false);
        String a("", false);
        String *t = g_mw_gameText->getText(titleId);
        String b(*t);
        this->m_pFreelanceWindow->setText(a, b);
    }

    // --- action buttons (Accept / Reject / Show-on-map) ---
    if (gStatus->inAlienOrbit() == 0) {
        int btnY = ((this->m_width >> 1) >> 1) - L->buttonInsetX;
        if (gStatus->gameWon() == 0) {
            String *t = g_mw_gameText->getText(titleId);
            this->m_pAcceptButton = new TouchButton(
                t, 0, L->buttonInsetX + this->m_x,
                (((this->m_y + this->m_height) - L->field_0x10) - L->field_0x24) - L->field_0x2c,
                btnY, '!', 4);
        }
        if (gStatus->getFreelanceMission()->isEmpty() == 0) {
            String *t = g_mw_gameText->getText(titleId);
            this->m_pRejectButton = new TouchButton(
                t, 0, this->m_x + (this->m_width >> 1) + L->field_0x2c,
                (((this->m_y - L->field_0x2c) + this->m_height) - L->field_0x10) - L->field_0x24,
                btnY, '!', 4);

            if (ApplicationManager_GetCurrentApplicationModule(gAppManager) == 5) {
                String *t2 = g_mw_gameText->getText(titleId);
                this->m_pMapButton = new TouchButton(
                    t2, 0, this->m_x + btnY + (this->m_width >> 1) + L->field_0x2c * 2,
                    (((this->m_y - L->field_0x2c) + this->m_height) - L->field_0x10) - L->field_0x24,
                    btnY, '!', 4);
                this->m_pMapButton->setTextColor(g_mwi_actionColor);
            }
            this->m_pChoiceWindow = new ChoiceWindow();
        }
    }

    this->m_mode = 0;
    this->m_hangarNeedsUpdate = 0;

    if (gStatus->wantedBoardAccessible() != 0) {
        if (this->m_pWantedWindow == nullptr)
            this->m_pWantedWindow = new WantedWindow();
        else
            this->m_pWantedWindow->init();
    }

    return 0;
}

void MissionsWindow::draw()
{
    if (this->m_mode == 1)            { this->drawWanted();  return; }
    if (this->m_starMapActive != 0)   { this->drawStarMap(); return; }

    PaintCanvas *canvas = gCanvas;
    Layout *L = *g_mwd_layout;
    void *color = *(void **)g_mwd_color;
    void *font = *(void **)g_mwd_font;
    int titleId = *g_mwd_textId;

    ((PaintCanvas *)canvas)->SetColor((unsigned int)(uintptr_t)color);

    {
        String *ht = g_mw_gameText->getText(titleId);
        String header(*ht);
        L->drawHeader(header);
    }

    if (gStatus->wantedBoardAccessible() != 0) {
        Array<TouchButton *> *tabs = this->m_pTabButtons;
        for (unsigned int i = 0; i < tabs->size(); i++)
            (*tabs)[i]->draw();
    }

    int ox = this->m_x, oy = this->m_y;
    int ow = this->m_width, oh = this->m_height;

    // Campaign-mission title box + body box.
    {
        String *t = g_mw_gameText->getText(titleId);
        String box(*t);
        int c = L->field_0xc, p20 = L->field_0x20;
        int p28 = L->buttonInsetX, p2c = L->field_0x2c;
        L->drawBox(1, p28 + ox, oy + c + p20, (ow >> 1) - (p2c + p28), L->field_0x5c, box, (unsigned)(uintptr_t)canvas);
    }
    {
        String box("", false);
        int c = L->field_0xc, p10 = L->field_0x10;
        int p20 = L->field_0x20, p24 = L->field_0x24;
        int p28 = L->buttonInsetX, p2c = L->field_0x2c;
        int p5c = L->field_0x5c;
        L->drawBox(5, p28 + ox, oy + c + p20 + p5c + p2c, (ow >> 1) - (p2c + p28),
                   ((oh - (p20 + c + p5c + p2c * 2)) - p10) - p24, box, (unsigned)(uintptr_t)canvas);
    }

    this->m_pCampaignWindow->draw();
    if (this->m_pAcceptButton) this->m_pAcceptButton->draw();

    // Freelance-mission title + body box (right column).
    {
        String *t = g_mw_gameText->getText(titleId);
        String box(*t);
        int c = L->field_0xc, p20 = L->field_0x20;
        int p28 = L->buttonInsetX, p2c = L->field_0x2c;
        L->drawBox(1, ox + (ow >> 1) + p2c, oy + c + p20, ((ow >> 1) - p2c) - p28, L->field_0x5c, box, (unsigned)(uintptr_t)canvas);
    }
    {
        String box("", false);
        int c = L->field_0xc, p10 = L->field_0x10;
        int p20 = L->field_0x20, p24 = L->field_0x24;
        int p28 = L->buttonInsetX, p2c = L->field_0x2c;
        int p5c = L->field_0x5c;
        L->drawBox(5, ox + (ow >> 1) + p2c, oy + p2c + c + p20 + p5c, ((ow >> 1) - p2c) - p28,
                   ((oh - (c + p2c * 2 + p20 + p5c)) - p10) - p24, box, (unsigned)(uintptr_t)canvas);
    }

    // Active freelance mission details.
    Mission *fm = gStatus->getFreelanceMission();
    if (fm != nullptr && fm->isEmpty() == 0 && this->m_pAgentImageParts != nullptr) {
        (*g_mwd_imageFactory)->drawChar(
            this->m_pAgentImageParts, ox + (ow >> 1) + L->field_0x2c,
            L->field_0x2c + oy + L->field_0xc + L->field_0x20 + L->field_0x5c, false);

        int detailX = ox + (ow >> 1) + L->field_0x2d4 + L->field_0x2c * 2;
        int detailY = oy + L->field_0xc + L->field_0x20 + L->field_0x2c + L->field_0x5c;

        String name = ((Agent *)(fm->getAgent()))->getName();
        ((PaintCanvas *)canvas)->DrawString((unsigned int)(uintptr_t)font, name, detailX, detailY, false);

        String station = ((Agent *)(fm->getAgent()))->getStationName();
        ((PaintCanvas *)canvas)->DrawString((unsigned int)(uintptr_t)font, station, detailX, detailY, false);

        String *typeTxt = g_mw_gameText->getText(
            ((Agent *)(fm->getAgent()))->getMission()->getType() + 0x162);
        ((PaintCanvas *)canvas)->DrawString((unsigned int)(uintptr_t)font, *typeTxt, detailX, detailY, false);
    }

    this->m_pFreelanceWindow->draw();
    if (this->m_pMapButton)    this->m_pMapButton->draw();
    if (this->m_pRejectButton) this->m_pRejectButton->draw();

    L->drawFooter();
    if (this->m_field_0x21 != 0 || this->m_choiceActive != 0)
        this->m_pChoiceWindow->draw();
}

void MissionsWindow::render3D()
{
    if (this->m_mode == 1)
        this->m_pWantedWindow->render3D();
    if (this->m_starMapActive)
        this->m_pStarMap->render();
}

void MissionsWindow::OnTouchEnd(int y, int z)
{
    // Wanted-board sub-window active.
    if (this->m_mode == 1) {
        this->m_pWantedWindow->OnTouchEnd(y, z);
        if (*(int *)this->m_pWantedWindow == 0) {
            this->m_mode = 0;
            *(int *)this->m_pWantedWindow = 1;
        }
        return;
    }

    if (this->m_choiceActive != 0) {
        // Confirmation dialog open.
        int r = this->m_pChoiceWindow->OnTouchEnd(y, z);
        if (r == 1) {
            this->m_choiceActive = 0;
            return;
        }
        if (r == 0) {
            // Confirmed: clear out the freelance-mission cargo/passengers and re-init.
            Status *st = gStatus;
            Mission *fm = st->getFreelanceMission();
            int type = fm->getType();
            bool clearCargo = (type == 0) || (fm->getType() == 3) || (fm->getType() == 5);
            if (clearCargo) {
                Array<Item *> *cargo = ((Ship *)(st->getShip()))->getCargo();
                if (cargo != nullptr) {
                    for (unsigned int i = 0; i < cargo->size(); i++) {
                        Item *item = (*cargo)[i];
                        if (item->isUnsaleable() != 0 && item->getIndex() == 0x74) {
                            ((Ship *)(st->getShip()))->removeCargo(item);
                            this->m_hangarNeedsUpdate = 1;
                            break;
                        }
                    }
                }
            } else if (fm->getType() == 0xb) {
                st->setPassengers(0);
            }

            Agent *agent = fm->getAgent();
            if (agent->isGenericAgent() == 0)
                fm->getAgent()->setOfferAccepted(false);
            st->setFreelanceMission(nullptr);

            unsigned char savedFlag = this->m_hangarNeedsUpdate;
            this->init();
            this->m_hangarNeedsUpdate = savedFlag;
            return;
        }
        // r != 0 and != 1: fall through to the normal touch handling.
    }

    // Normal (no dialog) path, unless the star-map overlay is showing.
    if (this->m_starMapActive == 0) {
        if (gStatus->wantedBoardAccessible() != 0) {
            Array<TouchButton *> *tabs = this->m_pTabButtons;
            for (unsigned int i = 0; i < tabs->size(); i++) {
                if ((*tabs)[i]->OnTouchEnd(y, z) != 0)
                    this->m_mode = (int)i;
            }
        }
        this->m_pCampaignWindow->OnTouchEnd(y, z);
        this->m_pFreelanceWindow->OnTouchEnd(y, z);

        if (this->m_pAcceptButton && this->m_pAcceptButton->OnTouchEnd(y, z) != 0) {
            // "Show on map" for the campaign mission.
            ApplicationManager *appMgr = gAppManager;
            void *mod = appMgr->GetApplicationModule(5);
            void *map = *(void **)((char *)mod + 0x10);
            this->m_pStarMap = (StarMap *)map;
            if (map == nullptr) {
                StarMap *m = new StarMap(true,
                    (Mission *)(void *)(intptr_t)gStatus->getCampaignMission(),
                    false, -1);
                void *mod2 = appMgr->GetApplicationModule(5);
                *(void **)((char *)mod2 + 0x10) = m;
                void *mod3 = appMgr->GetApplicationModule(5);
                this->m_pStarMap = (StarMap *)*(void **)((char *)mod3 + 0x10);
            } else {
                this->m_pStarMap->init(true,
                    (Mission *)(void *)(intptr_t)gStatus->getCampaignMission(),
                    false, -1);
            }
            this->m_starMapActive = 1;
            (*g_mwt_resetLayout)->resetWindowDimensions();
        } else {
            if (this->m_pMapButton && this->m_pMapButton->OnTouchEnd(y, z) != 0) {
                String *t = g_mw_gameText->getText(0x1a2);
                this->m_pChoiceWindow->set(*t, true);
                this->m_choiceActive = 1;
            }
            if (this->m_pRejectButton && this->m_pRejectButton->OnTouchEnd(y, z) != 0) {
                // "Show on map" for the freelance mission.
                ApplicationManager *appMgr = gAppManager;
                void *mod = appMgr->GetApplicationModule(5);
                void *map = *(void **)((char *)mod + 0x10);
                this->m_pStarMap = (StarMap *)map;
                if (map == nullptr) {
                    StarMap *m = new StarMap(true,
                        gStatus->getFreelanceMission(), false, -1);
                    void *mod2 = appMgr->GetApplicationModule(5);
                    *(void **)((char *)mod2 + 0x10) = m;
                    this->m_pStarMap = m;
                } else {
                    this->m_pStarMap->init(true,
                        gStatus->getFreelanceMission(), false, -1);
                }
                this->m_starMapActive = 1;
                (*g_mwt_resetLayout)->resetWindowDimensions();
                return;
            }
            Layout *layout = *g_mwt_layout;
            if (layout->OnTouchEnd(z, 0) != 0) {
                layout->resetWindowDimensions();
                return;
            }
            if (layout->helpPressed() != 0) {
                String *t = g_mw_gameText->getText(0x27b);
                String title(*t);
                layout->initHelpWindow(title);
            }
        }
    } else {
        // Star-map overlay is showing.
        if (StarMap_OnTouchEnd(this->m_pStarMap, y, z) != 0) {
            int wantW, wantH, posX;
            if (*g_mwt_flagA == 0) {
                this->m_x = 0;
                this->m_y = 0;
                wantW = 0;
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
                this->m_x = (*g_mwt_screenW >> 1) - posX;
                this->m_y = (*g_mwt_screenH >> 1) - (wantW >> 1);
            }
            this->m_width = wantH;
            this->m_height = wantW;
            this->m_starMapActive = 0;
        }
    }
}

void MissionsWindow::update(int dt)
{
    // Mode 1: advance the wanted board and bail out.
    if (this->m_mode == 1) {
        this->acceptAction();
        return;
    }
    // Star-map overlay active: advance it and bail out.
    if (this->m_starMapActive != 0) {
        this->cancelAction();
        return;
    }

    this->m_pCampaignWindow->update(dt);
    this->m_pFreelanceWindow->update(dt);

    Mission *cm = (Mission *)((void *)(intptr_t)gStatus->getCampaignMission());
    int type = cm->getType();
    bool relevant = (type == 0xa7) || (cm->getType() == 0xae);

    if (relevant) {
        void *camp = *(void **)g_mw_campaign;
        bool show = (gStatus->gameWon() == 0) ||
                    (*(char *)((char *)camp + 0x37) != 0 || *(char *)((char *)camp + 0x35) != 0);
        if (show) {
            String text("", false);
            if (gStatus->getCurrentCampaignMission() < 0xa4) {
                String *titleTxt = g_mw_gameText->getText(
                    g_mw_titleTable[gStatus->getCurrentCampaignMission()]);
                text = *titleTxt;

                void *key = gStatus;
                String hdr(text);
                int need = cm->getProductionGoodAmount();
                int have = cm->getStatusValue();
                String amount; amount.ctor_int(need - have);
                String suffix("", false);
                String merged;
                Status_replaceHash(&merged, key, &hdr, &amount, &suffix);
                text = merged;

                String a("", false);
                String b(text);
                this->m_pCampaignWindow->setText(a, b);
            }
        }
    }

    // Highlight the selected tab.
    Array<TouchButton *> *tabs = this->m_pTabButtons;
    for (unsigned int i = 0; i < tabs->size(); i++)
        (*tabs)[i]->setAlwaysPressed((int)i == this->m_mode);
}

// ---- mode-specific sub-window dispatchers ----
// draw()/update() short-circuit into one of these whenever an overlay is active.

void MissionsWindow::drawWanted()
{
    this->m_pWantedWindow->draw();
}

void MissionsWindow::drawStarMap()
{
    this->m_pStarMap->draw();
}

void MissionsWindow::acceptAction()
{
    this->m_pWantedWindow->update(0);
}

void MissionsWindow::cancelAction()
{
    this->m_pStarMap->update(0);
}
