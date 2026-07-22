#include "game/ui/MissionsWindow.h"
#include "game/ship/Ship.h"
#include "game/ship/Agent.h"
#include "engine/render/PaintCanvas.h"
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
#include "game/menu/ModStation.h"
#include <cstddef>

struct CampaignMissionFlags {
    char pad00[0x35];
    char campaignVisibleFlagA;
    char pad36;
    char campaignVisibleFlagB;
};
#if __SIZEOF_POINTER__ == 4
static_assert(offsetof(CampaignMissionFlags, campaignVisibleFlagA) == 0x35,
              "CampaignMissionFlags flagA offset");
static_assert(offsetof(CampaignMissionFlags, campaignVisibleFlagB) == 0x37,
              "CampaignMissionFlags flagB offset");
#endif



static int *g_mwi_titleTable = nullptr;
static int g_mwi_actionColor = 0;



static int *g_mw_titleTable = nullptr;

void Status_replaceHash(void *out, void *key, void *a, void *b, void *c); // lint: void_ptr (external symbol; param types are mangling-load-bearing)

// lint: void_ptr (external symbol; param types are mangling-load-bearing)

int ApplicationManager_GetCurrentApplicationModule(void *appMgr); // lint: void_ptr (external symbol; param types are mangling-load-bearing)

// lint: void_ptr (external symbol; param type is mangling-load-bearing)

int _mw_GetTextHeight(void *canvas); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

void TouchButton_ctorTab(void *self, void *text, int kind, int x, int y, char flags); // lint: void_ptr (external symbol; param type is mangling-load-bearing)

// lint: void_ptr (external symbol; param types are mangling-load-bearing)

int StarMap_OnTouchEnd(StarMap *map, int x, int y);

int MissionsWindow::OnTouchMove(int p1, int p2) {
    if (this->m_mode == 1)
        return this->m_pWantedWindow->OnTouchMove(p1, p2);
    if (this->m_choiceActive) {
        this->m_pChoiceWindow->OnTouchMove(p1, p2);
    } else if (this->m_starMapActive) {
        this->m_pStarMap->OnTouchMove(p1, p2);
    } else {
        if (Globals::status->wantedBoardAccessible()) {
            for (unsigned i = 0; i < this->m_pTabButtons->size(); i++)
                (*this->m_pTabButtons)[i]->OnTouchMove(p1, p2);
        }
        (Globals::layout)->OnTouchMove(p1, p2);
        this->m_pCampaignWindow->OnTouchMove(p1, p2);
        this->m_pFreelanceWindow->OnTouchMove(p1, p2);
        if (this->m_pAcceptButton) this->m_pAcceptButton->OnTouchMove(p1, p2);
        if (this->m_pMapButton) this->m_pMapButton->OnTouchMove(p1, p2);
        if (this->m_pRejectButton) this->m_pRejectButton->OnTouchMove(p1, p2);
    }
    return 0;
}

int MissionsWindow::OnTouchBegin(int p1, int p2) {
    if (this->m_mode == 1)
        return this->m_pWantedWindow->OnTouchBegin(p1, p2);
    if (this->m_choiceActive) {
        this->m_pChoiceWindow->OnTouchBegin(p1, p2);
    } else if (this->m_starMapActive) {
        this->m_pStarMap->OnTouchBegin(p1, p2);
    } else {
        if (Globals::status->wantedBoardAccessible()) {
            for (unsigned i = 0; i < this->m_pTabButtons->size(); i++)
                (*this->m_pTabButtons)[i]->OnTouchBegin(p1, p2);
        }
        (Globals::layout)->OnTouchBegin(p1, p2);
        this->m_pCampaignWindow->OnTouchBegin(p1, p2);
        this->m_pFreelanceWindow->OnTouchBegin(p1, p2);
        if (this->m_pAcceptButton) this->m_pAcceptButton->OnTouchBegin(p1, p2);
        if (this->m_pMapButton) this->m_pMapButton->OnTouchBegin(p1, p2);
        if (this->m_pRejectButton) this->m_pRejectButton->OnTouchBegin(p1, p2);
    }
    return 0;
}

void MissionsWindow::setHangarUpdate(bool v) {
    this->m_hangarNeedsUpdate = v;
}

uint8_t MissionsWindow::hangarNeedsUpdate() {
    return this->m_hangarNeedsUpdate;
}

MissionsWindow::MissionsWindow() {
    PaintCanvas *canvas = Globals::Canvas;
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

MissionsWindow::~MissionsWindow() {
    if (this->m_pAgentImageParts) {
        ArrayReleaseClasses(*this->m_pAgentImageParts);
        delete this->m_pAgentImageParts;
    }
    this->m_pAgentImageParts = nullptr;
    if (this->m_pTabButtons) {
        ArrayReleaseClasses(*this->m_pTabButtons);
        delete this->m_pTabButtons;
    }
    this->m_pTabButtons = nullptr;
    delete this->m_pCampaignWindow;
    this->m_pCampaignWindow = nullptr;
    delete this->m_pFreelanceWindow;
    this->m_pFreelanceWindow = nullptr;
    delete this->m_pChoiceWindow;
    this->m_pChoiceWindow = nullptr;
    delete this->m_pAcceptButton;
    this->m_pAcceptButton = nullptr;
    delete this->m_pRejectButton;
    this->m_pRejectButton = nullptr;
    delete this->m_pMapButton;
    this->m_pMapButton = nullptr;
    delete this->m_pWantedWindow;
    this->m_pWantedWindow = nullptr;
}

int MissionsWindow::init() {
    Layout *L = Globals::layout;
    int titleId = *g_mwi_titleTable;

    if (Globals::iPad == 0) {
        this->m_x = 0;
        this->m_y = 0;
        this->m_width = Globals::w;
        this->m_height = Globals::h;
    } else {
        int w, h;
        if (Globals::iPadHD == 0) {
            h = 0x514;
            if (Globals::iPadLarge == 0) h = 0x28a;
            w = (Globals::iPadLarge == 0) ? 500 : 1000;
        } else {
            h = 0x392;
            w = 1000;
        }
        this->m_width = h;
        this->m_height = w;
        this->m_x = (Globals::w >> 1) - (h >> 1);
        this->m_y = (Globals::h >> 1) - (w >> 1);
    }

    Array<TouchButton *> *tabs = new Array<TouchButton *>();
    this->m_pTabButtons = tabs;
    ArraySetLength(2, *tabs);

    TouchButton *tab1 = (TouchButton *) ::operator new(sizeof(TouchButton));
    String *t0 = Globals::gameText->getText(titleId);
    int helpOff = L->getHelpButtonOffset();
    TouchButton_ctorTab(tab1, t0, 3, (this->m_width + this->m_x) - helpOff, this->m_y, 0x12);
    (*tabs)[1] = tab1;

    TouchButton *tab0 = (TouchButton *) ::operator new(sizeof(TouchButton));
    String *t1 = Globals::gameText->getText(titleId);
    int helpOff2 = L->getHelpButtonOffset();
    int w1 = tab0->getWidth();
    TouchButton_ctorTab(tab0, t1, 3,
                        (((this->m_width + this->m_x) - helpOff2) - w1) + L->field_0x38,
                        this->m_y, 0x12);
    (*tabs)[0] = tab0;
    tab0->setAlwaysPressed(true);

    L->setWindowDimensions(this->m_x, this->m_y, this->m_width, this->m_height);

    if (this->m_pAgentImageParts) {
        ArrayReleaseClasses(*this->m_pAgentImageParts);
        delete this->m_pAgentImageParts;
    }
    this->m_pAgentImageParts = nullptr;
    delete this->m_pCampaignWindow;
    this->m_pCampaignWindow = nullptr;
    delete this->m_pFreelanceWindow;
    this->m_pFreelanceWindow = nullptr;
    delete this->m_pChoiceWindow;
    this->m_pChoiceWindow = nullptr;
    delete this->m_pAcceptButton;
    this->m_pAcceptButton = nullptr;
    delete this->m_pRejectButton;
    this->m_pRejectButton = nullptr;
    delete this->m_pMapButton;
    this->m_pStarMap = nullptr;
    this->m_pMapButton = nullptr;
    this->m_choiceActive = 0;

    int topY = L->field_0xc_leftMargin + this->m_y + L->field_0x20_top + L->field_0x5c + L->field_0x2c_rowHeight;
    int reserve = (Globals::status->gameWon() == 0) ? L->field_0x30_rowHeight : 0;
    this->m_pCampaignWindow = new ScrollTouchWindow(
        L->buttonInsetX + this->m_x, topY,
        (this->m_width >> 1) - (L->field_0x2c_rowHeight + L->buttonInsetX),
        (((((this->m_y - topY) + this->m_height) - L->field_0x10_rightMargin) - L->field_0x24) - reserve)
        + L->field_0x2c_rowHeight * -2, false);

    CampaignMissionFlags *campFlags = (CampaignMissionFlags *) Globals::options;
    bool campShow = (Globals::status->gameWon() == 0) ||
                    (campFlags->campaignVisibleFlagB != 0 ||
                     campFlags->campaignVisibleFlagA != 0);
    if (campShow) {
        String text("", false);
        if (Globals::status->getCurrentCampaignMission() < 0xa4) {
            String *t = Globals::gameText->getText(titleId);
            text = *t;
        }
        Status *key = Globals::status;
        Mission *cm = (Mission *) (intptr_t) Globals::status->getCampaignMission();
        int type = cm->getType();
        bool production = (type == 0xa7) || (cm->getType() == 0xae);
        String suffix("", false);
        String merged;
        if (production) {
            String hdr(text);
            int need = cm->getProductionGoodAmount();
            int have = cm->getStatusValue();
            String val;
            val.Set((long long) (need - have));
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
        bool useGold = Globals::achievements->gotAllGoldMedals() != 0 &&
                       ((Ship *) (Globals::status->getShip()))->getIndex() != 8;
        String a("", false);
        String *t = Globals::gameText->getText(titleId);
        String b(*t);
        this->m_pCampaignWindow->setText(a, b);
        (void) useGold;
    }

    int fmEmpty = Globals::status->getFreelanceMission()->isEmpty();
    int half = this->m_width >> 1;
    int pad = L->field_0x2c_rowHeight;
    int rx = this->m_x + half + pad;
    if (fmEmpty == 0) {
        int ry = L->field_0x2d8 + topY + pad;
        this->m_pFreelanceWindow = new ScrollTouchWindow(
            rx, ry, (half - pad) - L->buttonInsetX,
            (((((this->m_y - ry) + this->m_height) - L->field_0x10_rightMargin) - L->field_0x24) - L->field_0x4c)
            - L->field_0x30_rowHeight, false);

        Mission *fm = Globals::status->getFreelanceMission();
        String text = Globals::globals->getAgentMissionText(fm->getAgent());
        Status *key = Globals::status;
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

        int *parts = ((Agent *) (fm->getAgent()))->getImageParts();
        this->m_pAgentImageParts = (Globals::imageFactory)->loadChar(parts);
    } else {
        this->m_pFreelanceWindow = new ScrollTouchWindow(
            rx, topY, (half - pad) - L->buttonInsetX,
            ((this->m_height + (this->m_y - (topY + pad * 2))) - L->field_0x10_rightMargin) - L->field_0x24, false);
        String a("", false);
        String *t = Globals::gameText->getText(titleId);
        String b(*t);
        this->m_pFreelanceWindow->setText(a, b);
    }

    if (Globals::status->inAlienOrbit() == 0) {
        int btnY = ((this->m_width >> 1) >> 1) - L->buttonInsetX;
        if (Globals::status->gameWon() == 0) {
            String *t = Globals::gameText->getText(titleId);
            this->m_pAcceptButton = new TouchButton(
                *t, 0, L->buttonInsetX + this->m_x,
                (((this->m_y + this->m_height) - L->field_0x10_rightMargin) - L->field_0x24) - L->field_0x2c_rowHeight,
                btnY, '!', 4);
        }
        if (Globals::status->getFreelanceMission()->isEmpty() == 0) {
            String *t = Globals::gameText->getText(titleId);
            this->m_pRejectButton = new TouchButton(
                *t, 0, this->m_x + (this->m_width >> 1) + L->field_0x2c_rowHeight,
                (((this->m_y - L->field_0x2c_rowHeight) + this->m_height) - L->field_0x10_rightMargin) - L->field_0x24,
                btnY, '!', 4);

            if (ApplicationManager_GetCurrentApplicationModule(Globals::appManager) == 5) {
                String *t2 = Globals::gameText->getText(titleId);
                this->m_pMapButton = new TouchButton(
                    *t2, 0, this->m_x + btnY + (this->m_width >> 1) + L->field_0x2c_rowHeight * 2,
                    (((this->m_y - L->field_0x2c_rowHeight) + this->m_height) - L->field_0x10_rightMargin) - L->field_0x24,
                    btnY, '!', 4);
                this->m_pMapButton->setTextColor(g_mwi_actionColor);
            }
            this->m_pChoiceWindow = new ChoiceWindow();
        }
    }

    this->m_mode = 0;
    this->m_hangarNeedsUpdate = 0;

    if (Globals::status->wantedBoardAccessible() != 0) {
        if (this->m_pWantedWindow == nullptr)
            this->m_pWantedWindow = new WantedWindow();
        else
            this->m_pWantedWindow->init();
    }

    return 0;
}

void MissionsWindow::draw() {
    if (this->m_mode == 1) {
        this->m_pWantedWindow->draw();
        return;
    }
    if (this->m_starMapActive != 0) {
        this->m_pStarMap->draw();
        return;
    }

    PaintCanvas *canvas = Globals::Canvas;
    Layout *L = Globals::layout;
    unsigned int color = 0xFFFFFFFF;
    unsigned int font = (unsigned int) (uintptr_t) Globals::font;
    int titleId = 0x81;

    ((PaintCanvas *) canvas)->SetColor(color);

    {
        String *ht = Globals::gameText->getText(titleId);
        String header(*ht);
        L->drawHeader(header);
    }

    if (Globals::status->wantedBoardAccessible() != 0) {
        Array<TouchButton *> *tabs = this->m_pTabButtons;
        for (unsigned int i = 0; i < tabs->size(); i++)
            (*tabs)[i]->draw();
    }

    int ox = this->m_x, oy = this->m_y;
    int ow = this->m_width, oh = this->m_height;

    {
        String *t = Globals::gameText->getText(titleId);
        String box(*t);
        int c = L->field_0xc_leftMargin, p20 = L->field_0x20_top;
        int p28 = L->buttonInsetX, p2c = L->field_0x2c_rowHeight;
        L->drawBox(1, p28 + ox, oy + c + p20, (ow >> 1) - (p2c + p28), L->field_0x5c, box,
                   (unsigned) (uintptr_t) canvas);
    }
    {
        String box("", false);
        int c = L->field_0xc_leftMargin, p10 = L->field_0x10_rightMargin;
        int p20 = L->field_0x20_top, p24 = L->field_0x24;
        int p28 = L->buttonInsetX, p2c = L->field_0x2c_rowHeight;
        int p5c = L->field_0x5c;
        L->drawBox(5, p28 + ox, oy + c + p20 + p5c + p2c, (ow >> 1) - (p2c + p28),
                   ((oh - (p20 + c + p5c + p2c * 2)) - p10) - p24, box, (unsigned) (uintptr_t) canvas);
    }

    this->m_pCampaignWindow->draw();
    if (this->m_pAcceptButton) this->m_pAcceptButton->draw();

    {
        String *t = Globals::gameText->getText(titleId);
        String box(*t);
        int c = L->field_0xc_leftMargin, p20 = L->field_0x20_top;
        int p28 = L->buttonInsetX, p2c = L->field_0x2c_rowHeight;
        L->drawBox(1, ox + (ow >> 1) + p2c, oy + c + p20, ((ow >> 1) - p2c) - p28, L->field_0x5c, box,
                   (unsigned) (uintptr_t) canvas);
    }
    {
        String box("", false);
        int c = L->field_0xc_leftMargin, p10 = L->field_0x10_rightMargin;
        int p20 = L->field_0x20_top, p24 = L->field_0x24;
        int p28 = L->buttonInsetX, p2c = L->field_0x2c_rowHeight;
        int p5c = L->field_0x5c;
        L->drawBox(5, ox + (ow >> 1) + p2c, oy + p2c + c + p20 + p5c, ((ow >> 1) - p2c) - p28,
                   ((oh - (c + p2c * 2 + p20 + p5c)) - p10) - p24, box, (unsigned) (uintptr_t) canvas);
    }

    Mission *fm = Globals::status->getFreelanceMission();
    if (fm != nullptr && fm->isEmpty() == 0 && this->m_pAgentImageParts != nullptr) {
        (Globals::imageFactory)->drawChar(
            this->m_pAgentImageParts, ox + (ow >> 1) + L->field_0x2c_rowHeight,
            L->field_0x2c_rowHeight + oy + L->field_0xc_leftMargin + L->field_0x20_top + L->field_0x5c, false);

        int detailX = ox + (ow >> 1) + L->field_0x2d4 + L->field_0x2c_rowHeight * 2;
        int detailY = oy + L->field_0xc_leftMargin + L->field_0x20_top + L->field_0x2c_rowHeight + L->field_0x5c;

        String name = ((Agent *) (fm->getAgent()))->getName();
        ((PaintCanvas *) canvas)->DrawString(font, name, detailX, detailY, false);

        String station = ((Agent *) (fm->getAgent()))->getStationName();
        ((PaintCanvas *) canvas)->DrawString(font, station, detailX, detailY, false);

        String *typeTxt = Globals::gameText->getText(
            ((Agent *) (fm->getAgent()))->getMission()->getType() + 0x162);
        ((PaintCanvas *) canvas)->DrawString(font, *typeTxt, detailX, detailY, false);
    }

    this->m_pFreelanceWindow->draw();
    if (this->m_pMapButton) this->m_pMapButton->draw();
    if (this->m_pRejectButton) this->m_pRejectButton->draw();

    L->drawFooter();
    if (this->m_field_0x21 != 0 || this->m_choiceActive != 0)
        this->m_pChoiceWindow->draw();
}

void MissionsWindow::render3D() {
    if (this->m_mode == 1)
        this->m_pWantedWindow->render3D();
    if (this->m_starMapActive)
        this->m_pStarMap->render();
}

void MissionsWindow::OnTouchEnd(int y, int z) {
    if (this->m_mode == 1) {
        this->m_pWantedWindow->OnTouchEnd(y, z);
        if (this->m_pWantedWindow->lastButtonHit == 0) {
            this->m_mode = 0;
            this->m_pWantedWindow->lastButtonHit = 1;
        }
        return;
    }

    if (this->m_choiceActive != 0) {
        int r = this->m_pChoiceWindow->OnTouchEnd(y, z);
        if (r == 1) {
            this->m_choiceActive = 0;
            return;
        }
        if (r == 0) {
            Status *st = Globals::status;
            Mission *fm = st->getFreelanceMission();
            int type = fm->getType();
            bool clearCargo = (type == 0) || (fm->getType() == 3) || (fm->getType() == 5);
            if (clearCargo) {
                Array<Item *> *cargo = ((Ship *) (st->getShip()))->getCargo();
                if (cargo != nullptr) {
                    for (unsigned int i = 0; i < cargo->size(); i++) {
                        Item *item = (*cargo)[i];
                        if (item->isUnsaleable() != 0 && item->getIndex() == 0x74) {
                            ((Ship *) (st->getShip()))->removeCargo(item);
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
    }

    if (this->m_starMapActive == 0) {
        if (Globals::status->wantedBoardAccessible() != 0) {
            Array<TouchButton *> *tabs = this->m_pTabButtons;
            for (unsigned int i = 0; i < tabs->size(); i++) {
                if ((*tabs)[i]->OnTouchEnd(y, z) != 0)
                    this->m_mode = (int) i;
            }
        }
        this->m_pCampaignWindow->OnTouchEnd(y, z);
        this->m_pFreelanceWindow->OnTouchEnd(y, z);

        if (this->m_pAcceptButton && this->m_pAcceptButton->OnTouchEnd(y, z) != 0) {
            ApplicationManager *appMgr = Globals::appManager;
            ModStation *mod = (ModStation *) appMgr->GetApplicationModule(5);
            StarMap *map = mod->starMap;
            this->m_pStarMap = map;
            if (map == nullptr) {
                StarMap *m = new StarMap(true,
                                         (Mission *) (intptr_t) Globals::status->getCampaignMission(),
                                         false, -1);
                ModStation *mod2 = (ModStation *) appMgr->GetApplicationModule(5);
                mod2->starMap = m;
                ModStation *mod3 = (ModStation *) appMgr->GetApplicationModule(5);
                this->m_pStarMap = mod3->starMap;
            } else {
                this->m_pStarMap->init(true,
                                       (Mission *) (intptr_t) Globals::status->getCampaignMission(),
                                       false, -1);
            }
            this->m_starMapActive = 1;
            (Globals::layout)->resetWindowDimensions();
        } else {
            if (this->m_pMapButton && this->m_pMapButton->OnTouchEnd(y, z) != 0) {
                String *t = Globals::gameText->getText(0x1a2);
                this->m_pChoiceWindow->set(*t, true);
                this->m_choiceActive = 1;
            }
            if (this->m_pRejectButton && this->m_pRejectButton->OnTouchEnd(y, z) != 0) {
                ApplicationManager *appMgr = Globals::appManager;
                ModStation *mod = (ModStation *) appMgr->GetApplicationModule(5);
                StarMap *map = mod->starMap;
                this->m_pStarMap = map;
                if (map == nullptr) {
                    StarMap *m = new StarMap(true,
                                             Globals::status->getFreelanceMission(), false, -1);
                    ModStation *mod2 = (ModStation *) appMgr->GetApplicationModule(5);
                    mod2->starMap = m;
                    this->m_pStarMap = m;
                } else {
                    this->m_pStarMap->init(true,
                                           Globals::status->getFreelanceMission(), false, -1);
                }
                this->m_starMapActive = 1;
                (Globals::layout)->resetWindowDimensions();
                return;
            }
            Layout *layout = Globals::layout;
            if (layout->OnTouchEnd(z, 0) != 0) {
                layout->resetWindowDimensions();
                return;
            }
            if (layout->helpPressed() != 0) {
                String *t = Globals::gameText->getText(0x27b);
                String title(*t);
                layout->initHelpWindow(title);
            }
        }
    } else {
        if (StarMap_OnTouchEnd(this->m_pStarMap, y, z) != 0) {
            int wantW, wantH, posX;
            if (Globals::iPad == 0) {
                this->m_x = 0;
                this->m_y = 0;
                wantW = 0;
                wantH = 0;
                posX = 0;
            } else {
                if (Globals::iPadHD == 0) {
                    int w = 1000, h = 0x514;
                    if (Globals::iPadLarge == 0) {
                        w = 500;
                        h = 0x28a;
                    }
                    posX = h >> 1;
                    wantW = w;
                    wantH = h;
                } else {
                    wantW = 0x2bf;
                    posX = 0x1c9;
                    wantH = 0x392;
                }
                this->m_x = (Globals::w >> 1) - posX;
                this->m_y = (Globals::h >> 1) - (wantW >> 1);
            }
            this->m_width = wantH;
            this->m_height = wantW;
            this->m_starMapActive = 0;
        }
    }
}

void MissionsWindow::update(int dt) {
    if (this->m_mode == 1) {
        this->m_pWantedWindow->update(0);
        return;
    }

    if (this->m_starMapActive != 0) {
        this->m_pStarMap->update(0);
        return;
    }

    this->m_pCampaignWindow->update(dt);
    this->m_pFreelanceWindow->update(dt);

    Mission *cm = (Mission *) (intptr_t) Globals::status->getCampaignMission();
    int type = cm->getType();
    bool relevant = (type == 0xa7) || (cm->getType() == 0xae);

    if (relevant) {
        CampaignMissionFlags *camp = (CampaignMissionFlags *) Globals::options;
        bool show = (Globals::status->gameWon() == 0) ||
                    (camp->campaignVisibleFlagB != 0 || camp->campaignVisibleFlagA != 0);
        if (show) {
            String text("", false);
            if (Globals::status->getCurrentCampaignMission() < 0xa4) {
                String *titleTxt = Globals::gameText->getText(
                    g_mw_titleTable[Globals::status->getCurrentCampaignMission()]);
                text = *titleTxt;

                Status *key = Globals::status;
                String hdr(text);
                int need = cm->getProductionGoodAmount();
                int have = cm->getStatusValue();
                String amount;
                amount.Set((long long) (need - have));
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

    Array<TouchButton *> *tabs = this->m_pTabButtons;
    for (unsigned int i = 0; i < tabs->size(); i++)
        (*tabs)[i]->setAlwaysPressed((int) i == this->m_mode);
}
