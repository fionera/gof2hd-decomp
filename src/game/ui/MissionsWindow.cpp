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






// Objective text id of each campaign mission (original .rodata @ 0x20285c).
static const int CAMPAIGN_GOAL_TEXT[164] = {
    749, 749, 749, 749, 749, 749, 749, 749, 749, 749, 652, 653, 654, 655, 656, 749,
    657, 749, 658, 749, 659, 659, 749, 660, 661, 749, 749, 749, 662, 749, 749, 749,
    663, 664, 665, 666, 667, 749, 668, 749, 669, 749, 749, 749, 670, 670, 671, 749,
    672, 672, 672, 672, 672, 672, 672, 673, 674, 749, 675, 676, 749, 677, 678, 679,
    680, 749, 681, 682, 683, 684, 684, 685, 686, 687, 688, 749, 749, 689, 690, 690,
    691, 691, 749, 749, 692, 692, 693, 694, 694, 694, 694, 695, 696, 697, 698, 698,
    699, 700, 701, 701, 702, 702, 703, 704, 705, 706, 707, 708, 709, 710, 710, 711,
    712, 713, 714, 715, 716, 717, 718, 718, 719, 719, 720, 721, 721, 722, 722, 723,
    724, 724, 725, 726, 726, 726, 727, 728, 729, 730, 731, 732, 733, 734, 735, 736,
    736, 737, 738, 739, 740, 740, 740, 740, 741, 742, 743, 743, 744, 745, 746, 747,
    748, 748, 748, 0,
};






bool MissionsWindow::OnTouchMove(int p1, int p2) {
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
    return false;
}

bool MissionsWindow::OnTouchBegin(int p1, int p2) {
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
    return false;
}

void MissionsWindow::setHangarUpdate(bool v) {
    this->m_hangarNeedsUpdate = v;
}

uint8_t MissionsWindow::hangarNeedsUpdate() {
    return this->m_hangarNeedsUpdate;
}

MissionsWindow::MissionsWindow() {
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
    this->m_textHalfHeight = Globals::Canvas->GetTextHeight((unsigned int) (uintptr_t) Globals::font) / 2 - 1;
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
    if (!Globals::iPad) {
        this->m_x = 0;
        this->m_y = 0;
        this->m_width = Globals::w;
        this->m_height = Globals::h;
    } else {
        int width;
        float height;
        if (Globals::iPadHD) {
            height = 703.125f;
            width = 914;
        } else {
            height = !Globals::iPadLarge ? 500.0f : 1000.0f;
            width = !Globals::iPadLarge ? 650 : 1300;
        }
        this->m_width = width;
        this->m_height = (int) height;
        this->m_x = (Globals::w >> 1) - ((unsigned int) width >> 1);
        this->m_y = (Globals::h >> 1) - ((int) height >> 1);
    }

    this->m_pTabButtons = new Array<TouchButton *>();
    ArraySetLength(2, *this->m_pTabButtons);
    (*this->m_pTabButtons)[1] = new TouchButton(*Globals::gameText->getText(3219), 3,
                                                this->m_x + this->m_width - Globals::layout->getHelpButtonOffset(),
                                                this->m_y, 18);
    (*this->m_pTabButtons)[0] = new TouchButton(*Globals::gameText->getText(129), 3,
                                                this->m_x + this->m_width - Globals::layout->getHelpButtonOffset() -
                                                (*this->m_pTabButtons)[1]->getWidth() + Globals::layout->field_0x38,
                                                this->m_y, 18);
    (*this->m_pTabButtons)[0]->setAlwaysPressed(true);
    Globals::layout->setWindowDimensions(this->m_x, this->m_y, this->m_width, this->m_height);

    if (this->m_pAgentImageParts != nullptr) {
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
    this->m_pMapButton = nullptr;
    int inset = Globals::layout->buttonInsetX;
    this->m_pStarMap = nullptr;
    this->m_choiceActive = 0;
    this->m_field_0x21 = 0;
    this->m_starMapActive = 0;
    this->m_hangarNeedsUpdate = 0;

    // Campaign mission on the left half.
    int top = Globals::layout->field_0xc_leftMargin + this->m_y + Globals::layout->field_0x20_top +
              Globals::layout->field_0x5c + Globals::layout->field_0x2c_rowHeight;
    this->m_pCampaignWindow = new ScrollTouchWindow(
            Globals::layout->buttonInsetX + this->m_x, top,
            (this->m_width >> 1) - (Globals::layout->field_0x2c_rowHeight + Globals::layout->buttonInsetX),
            this->m_y - top + this->m_height - Globals::layout->field_0x10_rightMargin - Globals::layout->field_0x24 -
            (Globals::status->gameWon() ? 0 : Globals::layout->field_0x30_rowHeight) -
            Globals::layout->field_0x2c_rowHeight * 2,
            false);
    CampaignMissionFlags *flags = (CampaignMissionFlags *) Globals::options;
    if (!Globals::status->gameWon() || flags->campaignVisibleFlagA || flags->campaignVisibleFlagB) {
        String text("", false);
        if (Globals::status->getCurrentCampaignMission() < 164)
            text = *Globals::gameText->getText(CAMPAIGN_GOAL_TEXT[Globals::status->getCurrentCampaignMission()]);
        if (((Mission *) (intptr_t) Globals::status->getCampaignMission())->getType() == 167 ||
            ((Mission *) (intptr_t) Globals::status->getCampaignMission())->getType() == 174) {
            text = Globals::status->replaceHash(
                    text,
                    String(((Mission *) (intptr_t) Globals::status->getCampaignMission())->getProductionGoodAmount() -
                           ((Mission *) (intptr_t) Globals::status->getCampaignMission())->getStatusValue()),
                    String("#Q", false));
        } else {
            text = Globals::status->replaceHash(
                    text, ((Mission *) (intptr_t) Globals::status->getCampaignMission())->getTargetStationName(),
                    String("#", false));
        }
        this->m_pCampaignWindow->setText("", text);
    } else if (Globals::achievements->gotAllGoldMedals() && Globals::status->getShip()->getIndex() != 8) {
        this->m_pCampaignWindow->setText("", *Globals::gameText->getText(650));
    } else {
        this->m_pCampaignWindow->setText("", *Globals::gameText->getText(670));
    }

    // Freelance mission on the right half.
    int noFreelance = Globals::status->getFreelanceMission()->isEmpty();
    if (!noFreelance) {
        this->m_pFreelanceWindow = new ScrollTouchWindow(
                this->m_x + (this->m_width >> 1) + Globals::layout->field_0x2c_rowHeight,
                Globals::layout->field_0x2d8 + (top + Globals::layout->field_0x2c_rowHeight),
                (this->m_width >> 1) - Globals::layout->field_0x2c_rowHeight - Globals::layout->buttonInsetX,
                this->m_y - (Globals::layout->field_0x2d8 + (top + Globals::layout->field_0x2c_rowHeight)) +
                this->m_height - Globals::layout->field_0x10_rightMargin - Globals::layout->field_0x24 -
                Globals::layout->field_0x4c - Globals::layout->field_0x30_rowHeight,
                false);
        String text = Globals::globals->getAgentMissionText(Globals::status->getFreelanceMission()->getAgent());
        text = Globals::status->replaceHash(
                text,
                Globals::layout->formatCredits(Globals::status->getFreelanceMission()->getReward() +
                                               Globals::status->getFreelanceMission()->getBonus()),
                String("#C", false));
        this->m_pFreelanceWindow->setText("", text);
        this->m_pAgentImageParts =
                Globals::imageFactory->loadChar(Globals::status->getFreelanceMission()->getAgent()->getImageParts());
    } else {
        this->m_pFreelanceWindow = new ScrollTouchWindow(
                this->m_x + (this->m_width >> 1) + Globals::layout->field_0x2c_rowHeight, top,
                (this->m_width >> 1) - Globals::layout->field_0x2c_rowHeight - Globals::layout->buttonInsetX,
                this->m_height + (this->m_y - (top + Globals::layout->field_0x2c_rowHeight * 2)) -
                Globals::layout->field_0x10_rightMargin - Globals::layout->field_0x24,
                false);
        this->m_pFreelanceWindow->setText("", *Globals::gameText->getText(174));
    }

    int buttonWidth = (this->m_width >> 1) / 2 - inset;
    if (!Globals::status->inAlienOrbit()) {
        if (!Globals::status->gameWon())
            this->m_pAcceptButton = new TouchButton(
                    *Globals::gameText->getText(424), 0, Globals::layout->buttonInsetX + this->m_x,
                    this->m_y + this->m_height - Globals::layout->field_0x10_rightMargin - Globals::layout->field_0x24 -
                    Globals::layout->field_0x2c_rowHeight,
                    buttonWidth, 33, 4);
        if (!Globals::status->getFreelanceMission()->isEmpty()) {
            this->m_pRejectButton = new TouchButton(
                    *Globals::gameText->getText(424), 0,
                    this->m_x + (this->m_width >> 1) + Globals::layout->field_0x2c_rowHeight,
                    this->m_y - Globals::layout->field_0x2c_rowHeight + this->m_height -
                    Globals::layout->field_0x10_rightMargin - Globals::layout->field_0x24,
                    buttonWidth, 33, 4);
            if (Globals::appManager->GetCurrentApplicationModule() == 5) {
                this->m_pMapButton = new TouchButton(
                        *Globals::gameText->getText(423), 0,
                        this->m_x + buttonWidth + (this->m_width >> 1) + Globals::layout->field_0x2c_rowHeight * 2,
                        this->m_y - Globals::layout->field_0x2c_rowHeight + this->m_height -
                        Globals::layout->field_0x10_rightMargin - Globals::layout->field_0x24,
                        buttonWidth, 33, 4);
                this->m_pMapButton->setTextColor(0xff2a00ff);
            }
            this->m_pChoiceWindow = new ChoiceWindow();
        }
    }

    this->m_mode = 0;
    this->m_hangarNeedsUpdate = 0;
    if (Globals::status->wantedBoardAccessible()) {
        if (this->m_pWantedWindow != nullptr)
            this->m_pWantedWindow->init();
        else
            this->m_pWantedWindow = new WantedWindow();
    }
    return 0;
}

void MissionsWindow::draw() {
    if (this->m_mode == 1) {
        this->m_pWantedWindow->draw();
        return;
    }
    if (this->m_starMapActive) {
        this->m_pStarMap->draw();
        return;
    }

    Globals::Canvas->SetColor(0xffffffffu);
    Globals::layout->drawHeader(*Globals::gameText->getText(129));
    if (Globals::status->wantedBoardAccessible()) {
        for (unsigned int i = 0; i < this->m_pTabButtons->size(); i++)
            (*this->m_pTabButtons)[i]->draw();
    }

    // Campaign mission on the left half.
    Globals::layout->drawBox(1, this->m_x + Globals::layout->buttonInsetX,
                             this->m_y + Globals::layout->field_0xc_leftMargin + Globals::layout->field_0x20_top,
                             (this->m_width >> 1) - (Globals::layout->field_0x2c_rowHeight +
                                                     Globals::layout->buttonInsetX),
                             Globals::layout->field_0x5c, *Globals::gameText->getText(555));
    Globals::layout->drawBox(5, this->m_x + Globals::layout->buttonInsetX,
                             this->m_y + Globals::layout->field_0xc_leftMargin + Globals::layout->field_0x20_top +
                             Globals::layout->field_0x5c + Globals::layout->field_0x2c_rowHeight,
                             (this->m_width >> 1) - (Globals::layout->field_0x2c_rowHeight +
                                                     Globals::layout->buttonInsetX),
                             this->m_height - Globals::layout->field_0x20_top - Globals::layout->field_0xc_leftMargin -
                             Globals::layout->field_0x5c - 2 * Globals::layout->field_0x2c_rowHeight -
                             Globals::layout->field_0x10_rightMargin - Globals::layout->field_0x24,
                             String("", false));
    this->m_pCampaignWindow->draw();
    if (this->m_pAcceptButton != nullptr)
        this->m_pAcceptButton->draw();

    // Freelance mission on the right half.
    Globals::layout->drawBox(1, this->m_x + (this->m_width >> 1) + Globals::layout->field_0x2c_rowHeight,
                             this->m_y + Globals::layout->field_0xc_leftMargin + Globals::layout->field_0x20_top,
                             (this->m_width >> 1) - Globals::layout->field_0x2c_rowHeight -
                             Globals::layout->buttonInsetX,
                             Globals::layout->field_0x5c, *Globals::gameText->getText(556));
    Globals::layout->drawBox(5, this->m_x + (this->m_width >> 1) + Globals::layout->field_0x2c_rowHeight,
                             this->m_y + Globals::layout->field_0x2c_rowHeight + Globals::layout->field_0xc_leftMargin +
                             Globals::layout->field_0x20_top + Globals::layout->field_0x5c,
                             (this->m_width >> 1) - Globals::layout->field_0x2c_rowHeight -
                             Globals::layout->buttonInsetX,
                             this->m_height - Globals::layout->field_0xc_leftMargin -
                             Globals::layout->field_0x2c_rowHeight * 2 - Globals::layout->field_0x20_top -
                             Globals::layout->field_0x5c -
                             Globals::layout->field_0x10_rightMargin - Globals::layout->field_0x24,
                             String("", false));
    Mission *freelance = Globals::status->getFreelanceMission();
    if (freelance != nullptr && !freelance->isEmpty() && this->m_pAgentImageParts != nullptr) {
        Globals::imageFactory->drawChar(this->m_pAgentImageParts,
                                        this->m_x + (this->m_width >> 1) + Globals::layout->field_0x2c_rowHeight,
                                        Globals::layout->field_0x2c_rowHeight + this->m_y +
                                        Globals::layout->field_0xc_leftMargin + Globals::layout->field_0x20_top +
                                        Globals::layout->field_0x5c,
                                        false);
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, freelance->getAgent()->getName(),
                                    this->m_x + (this->m_width >> 1) + Globals::layout->field_0x2d4 +
                                    Globals::layout->field_0x2c_rowHeight * 2,
                                    this->m_y + Globals::layout->field_0x2c_rowHeight +
                                    Globals::layout->field_0xc_leftMargin + Globals::layout->field_0x20_top +
                                    Globals::layout->field_0x5c,
                                    false);
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, freelance->getAgent()->getStationName(),
                                    this->m_x + (this->m_width >> 1) + Globals::layout->field_0x2d4 +
                                    Globals::layout->field_0x2c_rowHeight * 2,
                                    this->m_y + Globals::layout->field_0x2c_rowHeight +
                                    Globals::layout->field_0xc_leftMargin + Globals::layout->field_0x20_top +
                                    Globals::layout->field_0x5c + Globals::layout->field_0x4,
                                    false);
        Globals::Canvas->DrawString(
                (unsigned int) (uintptr_t) Globals::font,
                *Globals::gameText->getText(freelance->getAgent()->getMission()->getType() + 354),
                this->m_x + (this->m_width >> 1) + Globals::layout->field_0x2d4 + Globals::layout->field_0x2c_rowHeight * 2,
                this->m_y + Globals::layout->field_0x2c_rowHeight + Globals::layout->field_0xc_leftMargin +
                Globals::layout->field_0x20_top + Globals::layout->field_0x5c + Globals::layout->field_0x4 * 2,
                false);
    }
    this->m_pFreelanceWindow->draw();
    if (this->m_pMapButton != nullptr)
        this->m_pMapButton->draw();
    if (this->m_pRejectButton != nullptr)
        this->m_pRejectButton->draw();
    Globals::layout->drawFooter();
    if (this->m_field_0x21 || this->m_choiceActive)
        this->m_pChoiceWindow->draw();
}

void MissionsWindow::render3D() {
    if (this->m_mode == 1)
        this->m_pWantedWindow->render3D();
    if (this->m_starMapActive)
        this->m_pStarMap->render();
}

bool MissionsWindow::OnTouchEnd(int x, int y) {
    if (this->m_mode == 1) {
        int result = this->m_pWantedWindow->OnTouchEnd(x, y);
        // The wanted board's first button leads back to the mission list.
        if (this->m_pWantedWindow->lastButtonHit == 0) {
            this->m_mode = 0;
            this->m_pWantedWindow->lastButtonHit = 1;
        }
        return result;
    }

    if (this->m_choiceActive) {
        int choice = this->m_pChoiceWindow->OnTouchEnd(x, y);
        if (choice == 0) {
            // Abort the freelance mission: take back mission cargo and passengers.
            if (Globals::status->getFreelanceMission()->getType() == 0 ||
                Globals::status->getFreelanceMission()->getType() == 3 ||
                Globals::status->getFreelanceMission()->getType() == 5) {
                Array<Item *> *cargo = Globals::status->getShip()->getCargo();
                if (cargo != nullptr) {
                    for (unsigned int i = 0; i < cargo->size(); i++) {
                        if ((*cargo)[i]->isUnsaleable() && (*cargo)[i]->getIndex() == 116) {
                            Globals::status->getShip()->removeCargo((*cargo)[i]);
                            this->m_hangarNeedsUpdate = 1;
                            break;
                        }
                    }
                }
            } else if (Globals::status->getFreelanceMission()->getType() == 11) {
                Globals::status->setPassengers(0);
            }
            if (!Globals::status->getFreelanceMission()->getAgent()->isGenericAgent())
                Globals::status->getFreelanceMission()->getAgent()->setOfferAccepted(false);
            Globals::status->setFreelanceMission(Mission::empty);
            uint8_t hangarNeedsUpdate = this->m_hangarNeedsUpdate;
            this->init();
            this->m_hangarNeedsUpdate = hangarNeedsUpdate;
            return false;
        }
        if (choice == 1) {
            this->m_choiceActive = 0;
            return false;
        }
    }

    if (this->m_starMapActive) {
        if (this->m_pStarMap->OnTouchEnd(x, y)) {
            int width;
            int halfWidth;
            int height;
            if (Globals::iPad) {
                if (Globals::iPadHD) {
                    height = 703;
                    halfWidth = 457;
                    width = 914;
                } else {
                    height = 1000;
                    width = 1300;
                    if (!Globals::iPadLarge) {
                        height = 500;
                        width = 650;
                    }
                    halfWidth = (unsigned int) width >> 1;
                }
                this->m_width = width;
                this->m_height = height;
                this->m_x = (Globals::w >> 1) - halfWidth;
                this->m_y = (Globals::h >> 1) - ((unsigned int) height >> 1);
            } else {
                width = Globals::w;
                height = Globals::h;
                this->m_x = 0;
                this->m_y = 0;
                this->m_width = width;
                this->m_height = height;
            }
            this->m_starMapActive = 0;
        }
        return false;
    }

    if (Globals::status->wantedBoardAccessible()) {
        for (unsigned int i = 0; i < this->m_pTabButtons->size(); i++) {
            if ((*this->m_pTabButtons)[i]->OnTouchEnd(x, y))
                this->m_mode = i;
        }
    }
    this->m_pCampaignWindow->OnTouchEnd(x, y);
    this->m_pFreelanceWindow->OnTouchEnd(x, y);

    if (this->m_pAcceptButton != nullptr && this->m_pAcceptButton->OnTouchEnd(x, y)) {
        // Show the campaign mission on the star map.
        this->m_pStarMap = ((ModStation *) Globals::appManager->GetApplicationModule(5))->starMap;
        if (this->m_pStarMap != nullptr) {
            this->m_pStarMap->init(true, (Mission *) (intptr_t) Globals::status->getCampaignMission(), false, -1);
        } else {
            ((ModStation *) Globals::appManager->GetApplicationModule(5))->starMap =
                    new StarMap(true, (Mission *) (intptr_t) Globals::status->getCampaignMission(), false, -1);
            this->m_pStarMap = ((ModStation *) Globals::appManager->GetApplicationModule(5))->starMap;
        }
        this->m_starMapActive = 1;
        Globals::layout->resetWindowDimensions();
        return false;
    }
    if (this->m_pMapButton != nullptr && this->m_pMapButton->OnTouchEnd(x, y)) {
        this->m_pChoiceWindow->set(*Globals::gameText->getText(418), true);
        this->m_choiceActive = 1;
    }
    if (this->m_pRejectButton != nullptr && this->m_pRejectButton->OnTouchEnd(x, y)) {
        // Show the freelance mission on the star map.
        this->m_pStarMap = ((ModStation *) Globals::appManager->GetApplicationModule(5))->starMap;
        if (this->m_pStarMap != nullptr) {
            this->m_pStarMap->init(true, Globals::status->getFreelanceMission(), false, -1);
        } else {
            ((ModStation *) Globals::appManager->GetApplicationModule(5))->starMap =
                    new StarMap(true, Globals::status->getFreelanceMission(), false, -1);
            this->m_pStarMap = ((ModStation *) Globals::appManager->GetApplicationModule(5))->starMap;
        }
        this->m_starMapActive = 1;
        Globals::layout->resetWindowDimensions();
        return false;
    }

    if (Globals::layout->OnTouchEnd(x, y)) {
        Globals::layout->resetWindowDimensions();
        return true;
    }
    bool (Layout::*helpPressedFn)() = reinterpret_cast<bool (Layout::*)()>(&Layout::helpPressed);
    if ((Globals::layout->*helpPressedFn)())
        Globals::layout->initHelpWindow(String(*Globals::gameText->getText(635), false));
    return false;
}

void MissionsWindow::update(int dt) {
    if (this->m_mode == 1) {
        this->m_pWantedWindow->update(dt);
        return;
    }
    if (this->m_starMapActive) {
        this->m_pStarMap->update(dt);
        return;
    }

    this->m_pCampaignWindow->update(dt);
    this->m_pFreelanceWindow->update(dt);

    // Production missions show the remaining amount of goods in the campaign text.
    CampaignMissionFlags *flags = (CampaignMissionFlags *) Globals::options;
    if ((((Mission *) (intptr_t) Globals::status->getCampaignMission())->getType() == 167 ||
         ((Mission *) (intptr_t) Globals::status->getCampaignMission())->getType() == 174) &&
        (!Globals::status->gameWon() || flags->campaignVisibleFlagA || flags->campaignVisibleFlagB)) {
        String text("");
        if (Globals::status->getCurrentCampaignMission() < 164) {
            text = *Globals::gameText->getText(CAMPAIGN_GOAL_TEXT[Globals::status->getCurrentCampaignMission()]);
            text = Globals::status->replaceHash(
                    text,
                    String(((Mission *) (intptr_t) Globals::status->getCampaignMission())->getProductionGoodAmount() -
                           ((Mission *) (intptr_t) Globals::status->getCampaignMission())->getStatusValue()),
                    String("#Q"));
            this->m_pCampaignWindow->setText(String(""), text);
        }
    }

    for (unsigned int i = 0; i < this->m_pTabButtons->size(); i++)
        (*this->m_pTabButtons)[i]->setAlwaysPressed((int) i == this->m_mode);
}
