#include "game/ui/StatusWindow.h"
#include "game/ship/Ship.h"
#include "game/mission/BluePrint.h"
#include "engine/render/PaintCanvas.h"
#include "game/mission/Status.h"
#include "game/mission/Achievements.h"
#include "engine/core/GameText.h"
#include "engine/render/ImageFactory.h"
#include "engine/render/ImagePart.h"
#include "game/ui/Layout.h"
#include "game/core/Globals.h"
#include "game/world/Standing.h"
#include "game/core/String.h"
#include "game/ui/TouchButton.h"


StatusWindow::~StatusWindow() {
    if (this->tabButtons != 0) {
        ArrayReleaseClasses(*this->tabButtons);
        delete this->tabButtons;
    }
    this->tabButtons = 0;

    if (this->medalButtons != 0) {
        ArrayReleaseClasses(*this->medalButtons);
        delete this->medalButtons;
    }
    this->medalButtons = 0;

    if (this->imageParts != 0) {
        ArrayReleaseClasses(*this->imageParts);
        delete this->imageParts;
    }
    this->imageParts = 0;

    if (this->detailLines != 0) {
        ArrayReleaseClasses(*this->detailLines);
        delete this->detailLines;
    }
    this->detailLines = 0;
}

bool StatusWindow::OnTouchMove(int x, int y) {
    if ((Globals::layout->field_0xc_leftMargin < y && y < Globals::h - Globals::layout->field_0x10_rightMargin) ||
        Globals::mouse_wheel != 0) {
        int delta = y - this->lastTouchY;
        this->scrollVelocity = delta;
        this->scrollDamping = 1.0f;
        this->scrollOffset += delta;
        this->lastTouchY = y;
        if (this->selectedMedal >= 0) {
            int dragDist = this->touchStartY - y;
            if (dragDist < 0) dragDist = -dragDist;
            if (dragDist > 3) {
                (*this->medalButtons)[this->selectedMedal]->setAlwaysPressed(false);
                this->selectedMedal = -1;
            }
        }
    }
    Globals::layout->OnTouchMove(x, y);
    if (Globals::iPad == 0) {
        for (unsigned i = 0; i < this->tabButtons->size(); ++i)
            (*this->tabButtons)[i]->OnTouchMove(x, y);
    }
    if (this->activeTab == 1) {
        int *medals = Globals::achievements->getMedals();
        for (int i = 0; i < this->medalCount; ++i) {
            if (medals[i] != 0 || Globals::achievements->isEliteMedal(i) != 0)
                (*this->medalButtons)[i]->OnTouchMove(x, y);
        }
    }
    return false;
}

float StatusWindow::getRelativeScrollStartPos() {
    int range = this->scrollOffset;
    if (range > 0) return 0.0f;
    return -(float) range / (float) this->contentHeight;
}

void Status_replaceHash(void *out, void *key, void *a, void *b); // lint: void_ptr (external symbol; param types are mangling-load-bearing)

// lint: void_ptr (external symbol; param types are mangling-load-bearing)


bool StatusWindow::OnTouchEnd(int x, int y) {
    int velocity = this->scrollVelocity;
    int offset = this->scrollOffset + velocity;
    this->scrollDamping = 0.9f;
    this->isDragging = 0;
    this->scrollOffset = offset;
    this->scrollTarget = offset;
    this->scrollVelocityF = (velocity < 0 ? -velocity : velocity) > 3 ? (float) velocity : 0.0f;
    if (Globals::layout->OnTouchEnd(x, y))
        return true;

    if (!Globals::iPad) {
        for (unsigned int i = 0; i < this->tabButtons->size(); i++) {
            if ((*this->tabButtons)[i]->OnTouchEnd(x, y)) {
                this->activeTab = i;
                this->contentHeight = this->tabHeights[i];
                this->scrollOffset = 0;
            }
        }
    }
    if (this->activeTab == 1) {
        for (int i = 0; i < this->medalCount; i++) {
            if ((*this->medalButtons)[i]->OnTouchEnd(x, y)) {
                int *medals = Globals::achievements->getMedals();
                bool elite = Globals::achievements->isEliteMedal(i);
                if (elite || medals[i] != 0) {
                    if (this->selectedMedal >= 0)
                        (*this->medalButtons)[this->selectedMedal]->setAlwaysPressed(false);
                    this->selectedMedal = i;
                    if (this->detailLines != 0) {
                        ArrayReleaseClasses(*this->detailLines);
                        delete this->detailLines;
                    }
                    this->detailLines = 0;
                    this->detailLines = new Array<String *>();
                    int stage = elite ? 1 : medals[this->selectedMedal];
                    String text = Globals::status->replaceHash(
                        String(*Globals::gameText->getText(this->selectedMedal + 1552), false),
                        String(String(Globals::achievements->getValue(this->selectedMedal, stage)), false));
                    text += this->getMedalHintText(this->selectedMedal);
                    Globals::globals->getLineArray((unsigned int) (uintptr_t) Globals::font, text,
                                                   this->boxWidth - Globals::layout->field_0x4c * 2,
                                                   this->detailLines);
                    (*this->medalButtons)[i]->setAlwaysPressed(true);
                }
                break;
            }
        }
    }
    if (Globals::layout->helpPressed()) {
        if (this->activeTab == 0)
            Globals::layout->initHelpWindow(String(*Globals::gameText->getText(640), false));
        else if (this->activeTab == 1)
            Globals::layout->initHelpWindow(String(*Globals::gameText->getText(647), false));
    }
    return false;
}

bool StatusWindow::OnTouchBegin(int x, int y) {
    this->lastTouchY = y;
    this->touchStartY = y;
    this->scrollVelocity = 0;
    this->isDragging = 1;
    Globals::layout->OnTouchBegin(x, y);
    if (Globals::iPad == 0) {
        for (unsigned i = 0; i < this->tabButtons->size(); ++i)
            (*this->tabButtons)[i]->OnTouchBegin(x, y);
    }
    if (this->activeTab == 1) {
        int *medals = Globals::achievements->getMedals();
        for (int i = 0; i < this->medalCount; ++i) {
            if (medals[i] != 0 || Globals::achievements->isEliteMedal(i) != 0)
                (*this->medalButtons)[i]->OnTouchBegin(x, y);
        }
    }
    return false;
}

float StatusWindow::getRelativeScrollHeight() {
    if (this->viewportHeight > this->contentHeight)
        return 0.0f;
    if (this->scrollOffset > 0)
        return (float) (this->viewportHeight - this->scrollOffset) / (float) this->contentHeight;
    if (this->scrollOffset < this->viewportHeight - this->contentHeight)
        return (float) (this->scrollOffset + this->contentHeight) / (float) this->contentHeight;
    return (float) this->viewportHeight / (float) this->contentHeight;
}

void StatusWindow::update(int frameTime) {
    (void) frameTime;

    if (this->isDragging == 0) {
        float v = this->scrollDamping * this->scrollVelocityF;
        this->scrollVelocityF = v;

        float mag = v > 0.0f ? v : -v;
        if (mag > 1.0f) {
            this->scrollOffset = (int) (v + (float) this->scrollOffset);
        }
    }

    int off = this->scrollOffset;
    if (off > 0) {
        this->scrollDamping = 1.0f;
        this->scrollVelocityF = (float) (-off) * 0.5f;
    }

    int bottom = this->viewportHeight - this->contentHeight;
    if (bottom < 0) {
        if (off < bottom) {
            this->scrollDamping = 1.0f;
            this->scrollVelocityF = (float) (bottom - off) * 0.5f;
        }
    } else {
        this->scrollVelocityF = 0.0f;
        this->scrollOffset = 0;
    }

    for (unsigned int idx = 0; idx < this->tabButtons->size(); idx++) {
        (*this->tabButtons)[idx]->setAlwaysPressed(idx == this->activeTab);
    }
}


String StatusWindow::getMedalHintText(int medalIndex) {
    int stage = Globals::achievements->getMedals()[medalIndex];
    String out;
    if (stage == 0)
        return out;
    // Silver medals list what is still missing for gold.
    if (medalIndex == 2 && stage == 2) {
        out += String("\n\n") + *Globals::gameText->getText(276);
        for (unsigned int i = 0; i < Globals::status->field_94->size(); i++) {
            if (!(*Globals::status->field_94)[i])
                out += String("\n- ") + *Globals::gameText->getText(1428 + i);
        }
    } else if (medalIndex == 3 && stage == 2) {
        out += String("\n\n") + *Globals::gameText->getText(276);
        for (unsigned int i = 0; i < Globals::status->field_98->size(); i++) {
            if (!(*Globals::status->field_98)[i])
                out += String("\n- ") + *Globals::gameText->getText(1439 + i);
        }
    } else if (medalIndex == 9 && stage == 2) {
        out += String("\n\n") + *Globals::gameText->getText(276);
        for (unsigned int i = 0; i < Globals::status->field_ac->size(); i++) {
            if (!(*Globals::status->field_ac)[i])
                out += String("\n- ") + *Globals::gameText->getText(1406 + i);
        }
    } else if (medalIndex == 13 && stage == 2) {
        out += String("\n\n") + *Globals::gameText->getText(276);
        for (unsigned int i = 0; i < 13; i++) {
            if (!(*Globals::status->bluePrints)[i]->locked)
                out += String("\n- ") + *Globals::gameText->getText((*Globals::status->bluePrints)[i]->getIndex() + 1274);
        }
    } else if (medalIndex == 14 && stage == 2) {
        out += String("\n\n") + *Globals::gameText->getText(276);
        for (unsigned int i = 0; i < 13; i++) {
            if ((*Globals::status->bluePrints)[i]->productionCount == 0)
                out += String("\n- ") + *Globals::gameText->getText((*Globals::status->bluePrints)[i]->getIndex() + 1274);
        }
    }
    return out;
}



// lint: void_ptr (external symbol; param types are mangling-load-bearing)

static int g_sw_charDef = 0;

void StatusWindow::reInit() {
    this->imageParts = Globals::imageFactory->loadChar(&g_sw_charDef);

    bool enemy0 = ((Standing *) (intptr_t) Globals::status->getStanding())->isEnemy(0);
    bool enemy1 = ((Standing *) (intptr_t) Globals::status->getStanding())->isEnemy(1);
    bool enemy2 = ((Standing *) (intptr_t) Globals::status->getStanding())->isEnemy(2);
    bool enemy3 = ((Standing *) (intptr_t) Globals::status->getStanding())->isEnemy(3);

    Globals::Canvas->Image2DCreate(enemy0 ? 0x495 : enemy1 ? 0x494 : 0x493, this->rankImage0);
    Globals::Canvas->Image2DCreate(enemy1 ? 0x497 : enemy0 ? 0x496 : 0x492, this->rankImage1);
    Globals::Canvas->Image2DCreate(enemy2 ? 0x499 : enemy3 ? 0x498 : 0x490, this->rankImage2);
    Globals::Canvas->Image2DCreate(enemy3 ? 0x49b : enemy2 ? 0x49a : 0x491, this->rankImage3);

    this->charImageWidth = Globals::Canvas->GetImage2DWidth(this->rankImage0);
    this->charImageHeight = Globals::Canvas->GetImage2DHeight(this->rankImage0);
}

void StatusWindow::draw() {
    Globals::Canvas->SetColor(0xff);
    Globals::Canvas->FillRectangle(0, 0, Globals::w, Globals::h);
    Globals::Canvas->SetColor(0xffffffff);
    Globals::layout->drawBG();

    float scrollStart = this->getRelativeScrollStartPos();
    float viewH = (float) this->viewportHeight;
    float scrollHeight = this->getRelativeScrollHeight();
    int barH = (int) (scrollHeight * viewH);
    int barY = (int) (scrollStart * viewH);
    if (barH > 0 || barY >= 1)
        Globals::layout->drawScrollBar(Globals::w - Globals::layout->field_0x48 - Globals::layout->buttonInsetX,
                                       Globals::layout->field_0x20_top + Globals::layout->field_0xc_leftMargin,
                                       this->viewportHeight, barY, barH);

    int top = Globals::layout->field_0x20_top + Globals::layout->field_0xc_leftMargin;
    int width;
    if (Globals::iPad) {
        width = Globals::w >> 1;
    } else {
        top += this->scrollOffset;
        width = Globals::w;
    }
    if (barH >= 1)
        width = width - Globals::layout->field_0x48 - Globals::layout->field_0x2c_rowHeight;
    this->boxWidth = width - Globals::layout->buttonInsetX * 2;

    String value;
    String separator = GameText::getLanguage() == 9 ? String("") : String(":");

    if (this->activeTab == 0 || Globals::iPad) {
        // Pilot box.
        Globals::layout->drawBox(0, Globals::layout->buttonInsetX, top, this->boxWidth, Globals::layout->field_0x1c,
                                 String(*Globals::gameText->getText(1597), false));
        top = Globals::layout->field_0x1c + top + Globals::layout->field_0x2c_rowHeight;
        Globals::layout->drawBox(5, Globals::layout->buttonInsetX, top,
                                 (this->boxWidth >> 1) - Globals::layout->field_0x2c_rowHeight,
                                 Globals::layout->field_0x2d8, String(""));
        Globals::imageFactory->drawChar(this->imageParts, Globals::layout->field_0x4c + Globals::layout->buttonInsetX,
                                        top, false);
        value = Globals::layout->formatCredits(Globals::status->getCredits());
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, value,
                                    (this->boxWidth >> 1) - Globals::layout->field_0x2c_rowHeight -
                                        Globals::layout->field_0x4c -
                                        Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font, value),
                                    Globals::layout->field_0x2c_rowHeight + top, false);
        value = *Globals::gameText->getText(321) + String(" ") + Globals::status->getLevel();
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, value,
                                    (this->boxWidth >> 1) - Globals::layout->field_0x2c_rowHeight -
                                        Globals::layout->field_0x4c -
                                        Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font, value),
                                    Globals::layout->field_0x2c_rowHeight + top + Globals::layout->field_0x4, false);
        Globals::globals->longToTimeStringNoSeconds(Globals::status->getPlayingTime(), value);
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, value,
                                    (this->boxWidth >> 1) - Globals::layout->field_0x2c_rowHeight -
                                        Globals::layout->field_0x4c -
                                        Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font, value),
                                    Globals::layout->field_0x2c_rowHeight + top + Globals::layout->field_0x4 * 2, false);

        // Ship box.
        Globals::layout->drawBox(5, (this->boxWidth >> 1) + Globals::layout->buttonInsetX +
                                        Globals::layout->field_0x2c_rowHeight,
                                 top, (this->boxWidth >> 1) - Globals::layout->field_0x2c_rowHeight,
                                 Globals::layout->field_0x2d8, String(""));
        Globals::imageFactory->drawShip(Globals::status->getShip()->getIndex(),
                                        Globals::layout->buttonInsetX + (this->boxWidth >> 1) +
                                            Globals::layout->field_0x2c_rowHeight * 2,
                                        top);
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font,
                                    *Globals::gameText->getText(Globals::status->getShip()->getIndex() + 913),
                                    Globals::layout->buttonInsetX + (this->boxWidth >> 1) +
                                        Globals::layout->field_0x2c_rowHeight * 3 + Globals::layout->field_0x2cc,
                                    Globals::layout->field_0x2c_rowHeight + top, false);
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font,
                                    *Globals::gameText->getText(569) + separator,
                                    Globals::layout->buttonInsetX + (this->boxWidth >> 1) +
                                        Globals::layout->field_0x2c_rowHeight * 3 + Globals::layout->field_0x2cc,
                                    Globals::layout->field_0x4 + Globals::layout->field_0x2c_rowHeight + top, false);
        String decimals = String((int) ((Globals::status->getShip()->getFirePower() -
                                         (float) (int) Globals::status->getShip()->getFirePower()) *
                                        100.0f))
                              .SubString(0, 2);
        value = (int) Globals::status->getShip()->getFirePower() + String(".") + decimals;
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, value,
                                    this->boxWidth + Globals::layout->buttonInsetX - Globals::layout->field_0x4c -
                                        Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font, value),
                                    Globals::layout->field_0x2c_rowHeight + top + Globals::layout->field_0x4, false);
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font,
                                    *Globals::gameText->getText(570) + separator,
                                    Globals::layout->buttonInsetX + (this->boxWidth >> 1) +
                                        Globals::layout->field_0x2c_rowHeight * 3 + Globals::layout->field_0x2cc,
                                    Globals::layout->field_0x2c_rowHeight + top + Globals::layout->field_0x4 * 2,
                                    false);
        Globals::Canvas->DrawString(
            (unsigned int) (uintptr_t) Globals::font, String(Globals::status->getShip()->getCombinedHP()),
            this->boxWidth + Globals::layout->buttonInsetX - Globals::layout->field_0x4c -
                Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font,
                                              String(Globals::status->getShip()->getCombinedHP())),
            Globals::layout->field_0x2c_rowHeight + top + Globals::layout->field_0x4 * 2, false);

        // Rank / standing box.
        top = Globals::layout->field_0x2d8 + top + Globals::layout->field_0x2c_rowHeight;
        Globals::layout->drawBox(0, Globals::layout->buttonInsetX, top, this->boxWidth, Globals::layout->field_0x1c,
                                 String(*Globals::gameText->getText(576), false));
        top = Globals::layout->field_0x1c + top + Globals::layout->field_0x2c_rowHeight;
        Globals::layout->drawBox(5, Globals::layout->buttonInsetX, top,
                                 (this->boxWidth >> 1) - Globals::layout->field_0x2c_rowHeight,
                                 this->charImageHeight + Globals::layout->field_0x2c_rowHeight * 2 +
                                     Globals::layout->field_0x4,
                                 String(""));
        Globals::layout->drawBox(5, (this->boxWidth >> 1) + Globals::layout->buttonInsetX +
                                        Globals::layout->field_0x2c_rowHeight,
                                 top, (this->boxWidth >> 1) - Globals::layout->field_0x2c_rowHeight,
                                 this->charImageHeight + Globals::layout->field_0x2c_rowHeight * 2 +
                                     Globals::layout->field_0x4,
                                 String(""));
        int rankY = Globals::layout->field_0x2c_rowHeight + top;
        Globals::Canvas->DrawImage2D(this->rankImage0, Globals::layout->buttonInsetX + Globals::layout->field_0x4c, rankY);
        Globals::Canvas->DrawImage2D(this->rankImage1,
                                     Globals::layout->buttonInsetX + (this->boxWidth >> 1) -
                                         Globals::layout->field_0x2c_rowHeight - Globals::layout->field_0x4c,
                                     rankY, 0x11, 0x12);
        Globals::Canvas->DrawImage2D(this->rankImage2,
                                     Globals::layout->buttonInsetX + (this->boxWidth >> 1) +
                                         Globals::layout->field_0x2c_rowHeight + Globals::layout->field_0x4c,
                                     rankY);
        Globals::Canvas->DrawImage2D(this->rankImage3,
                                     this->boxWidth + Globals::layout->buttonInsetX - Globals::layout->field_0x4c, rankY,
                                     0x11, 0x12);
        int emblemX = Globals::layout->buttonInsetX + (this->boxWidth >> 2);
        int textY = Globals::layout->field_0x2c_rowHeight + (this->charImageHeight + rankY);
        int emblemY = textY - (this->charImageHeight >> 1) - (this->standingBarHeight >> 1);
        Globals::Canvas->DrawImage2D(this->standingEmblemImage, emblemX, emblemY, 0x11, 0x14);
        float rate = ((Standing *) (intptr_t) Globals::status->getStanding())->getStandingRate(0);
        Globals::Canvas->DrawRegion2D(this->standingBarImage, 0, 0, (int) -(rate * (float) this->standingBarWidth),
                                      this->standingBarHeight, 0.0f, 0, 0, emblemX, emblemY);
        Globals::Canvas->DrawImage2D(this->standingFrameImage,
                                     (int) ((float) emblemX + -rate * (float) this->standingBarWidth),
                                     textY - (this->charImageHeight >> 1), 0x11, 0x44);
        emblemX = this->boxWidth - ((this->boxWidth - Globals::layout->field_0x2c_rowHeight * 2) >> 2) +
                  Globals::layout->field_0x2c_rowHeight * 2;
        Globals::Canvas->DrawImage2D(this->standingEmblemImage, emblemX, emblemY, 0x11, 0x14);
        rate = ((Standing *) (intptr_t) Globals::status->getStanding())->getStandingRate(1);
        Globals::Canvas->DrawRegion2D(this->standingBarImage, 0, 0, (int) -(rate * (float) this->standingBarWidth),
                                      this->standingBarHeight, 0.0f, 0, 0, emblemX, emblemY);
        Globals::Canvas->DrawImage2D(this->standingFrameImage,
                                     (int) ((float) emblemX + -rate * (float) this->standingBarWidth),
                                     textY - (this->charImageHeight >> 1), 0x11, 0x44);
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, *Globals::gameText->getText(406),
                                    Globals::layout->field_0x4c + Globals::layout->buttonInsetX, textY, false);
        Globals::Canvas->DrawString(
            (unsigned int) (uintptr_t) Globals::font, *Globals::gameText->getText(407),
            Globals::layout->buttonInsetX + (this->boxWidth >> 1) - Globals::layout->field_0x2c_rowHeight -
                Globals::layout->field_0x4c -
                Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font, *Globals::gameText->getText(407)),
            textY, false);
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, *Globals::gameText->getText(408),
                                    Globals::layout->buttonInsetX + (this->boxWidth >> 1) +
                                        Globals::layout->field_0x2c_rowHeight + Globals::layout->field_0x4c,
                                    textY, false);
        Globals::Canvas->DrawString(
            (unsigned int) (uintptr_t) Globals::font, *Globals::gameText->getText(409),
            this->boxWidth + Globals::layout->buttonInsetX - Globals::layout->field_0x4c -
                Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font, *Globals::gameText->getText(409)),
            textY, false);

        // Statistics box.
        top = textY + Globals::layout->field_0x4 + Globals::layout->field_0x2c_rowHeight;
        Globals::layout->drawBox(0, Globals::layout->buttonInsetX, top, this->boxWidth, Globals::layout->field_0x1c,
                                 String(*Globals::gameText->getText(577), false));
        top = Globals::layout->field_0x1c + top + Globals::layout->field_0x2c_rowHeight;
        Globals::layout->drawBox(5, Globals::layout->buttonInsetX, top,
                                 (this->boxWidth >> 1) - Globals::layout->field_0x2c_rowHeight,
                                 Globals::layout->field_0x2c_rowHeight + Globals::layout->field_0x4 * 6, String(""));
        Globals::layout->drawBox(5, (this->boxWidth >> 1) + Globals::layout->buttonInsetX +
                                        Globals::layout->field_0x2c_rowHeight,
                                 top, (this->boxWidth >> 1) - Globals::layout->field_0x2c_rowHeight,
                                 Globals::layout->field_0x2c_rowHeight + Globals::layout->field_0x4 * 6, String(""));
        int rowY = Globals::layout->field_0x2c_rowHeight + top;
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, *Globals::gameText->getText(568) + separator,
                                    Globals::layout->field_0x4c + Globals::layout->buttonInsetX, rowY, false);
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, String(Globals::status->getMissionCount()),
                                    (this->boxWidth >> 1) - Globals::layout->field_0x2c_rowHeight -
                                        Globals::layout->field_0x4c -
                                        Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font, String(Globals::status->getMissionCount())),
                                    rowY, false);
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, *Globals::gameText->getText(176) + separator,
                                    Globals::layout->field_0x4c + Globals::layout->buttonInsetX, rowY + Globals::layout->field_0x4, false);
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, String(Globals::status->getKills()),
                                    (this->boxWidth >> 1) - Globals::layout->field_0x2c_rowHeight -
                                        Globals::layout->field_0x4c -
                                        Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font, String(Globals::status->getKills())),
                                    rowY + Globals::layout->field_0x4, false);
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, *Globals::gameText->getText(552) + separator,
                                    Globals::layout->field_0x4c + Globals::layout->buttonInsetX, rowY + Globals::layout->field_0x4 * 2, false);
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, String(Globals::status->field_d8),
                                    (this->boxWidth >> 1) - Globals::layout->field_0x2c_rowHeight -
                                        Globals::layout->field_0x4c -
                                        Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font, String(Globals::status->field_d8)),
                                    rowY + Globals::layout->field_0x4 * 2, false);
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, *Globals::gameText->getText(559) + separator,
                                    Globals::layout->field_0x4c + Globals::layout->buttonInsetX, rowY + Globals::layout->field_0x4 * 3, false);
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, String(Globals::status->getCapturedCrates()),
                                    (this->boxWidth >> 1) - Globals::layout->field_0x2c_rowHeight -
                                        Globals::layout->field_0x4c -
                                        Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font, String(Globals::status->getCapturedCrates())),
                                    rowY + Globals::layout->field_0x4 * 3, false);
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, *Globals::gameText->getText(557) + separator,
                                    Globals::layout->field_0x4c + Globals::layout->buttonInsetX, rowY + Globals::layout->field_0x4 * 4, false);
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, String(Globals::status->getStationsVisited()),
                                    (this->boxWidth >> 1) - Globals::layout->field_0x2c_rowHeight -
                                        Globals::layout->field_0x4c -
                                        Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font, String(Globals::status->getStationsVisited())),
                                    rowY + Globals::layout->field_0x4 * 4, false);
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, *Globals::gameText->getText(3235) + separator,
                                    Globals::layout->field_0x4c + Globals::layout->buttonInsetX, rowY + Globals::layout->field_0x4 * 5, false);
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, String(Globals::status->field_118),
                                    (this->boxWidth >> 1) - Globals::layout->field_0x2c_rowHeight -
                                        Globals::layout->field_0x4c -
                                        Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font, String(Globals::status->field_118)),
                                    rowY + Globals::layout->field_0x4 * 5, false);
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, *Globals::gameText->getText(564) + separator,
                                    Globals::layout->buttonInsetX + (this->boxWidth >> 1) +
                                        Globals::layout->field_0x2c_rowHeight + Globals::layout->field_0x4c,
                                    rowY, false);
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, String(Globals::status->getJumpgateUsed()),
                                    this->boxWidth + Globals::layout->buttonInsetX - Globals::layout->field_0x4c -
                                        Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font, String(Globals::status->getJumpgateUsed())),
                                    rowY, false);
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, *Globals::gameText->getText(558) + separator,
                                    Globals::layout->buttonInsetX + (this->boxWidth >> 1) +
                                        Globals::layout->field_0x2c_rowHeight + Globals::layout->field_0x4c,
                                    rowY + Globals::layout->field_0x4, false);
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, String(Globals::status->getGoodsProduced()),
                                    this->boxWidth + Globals::layout->buttonInsetX - Globals::layout->field_0x4c -
                                        Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font, String(Globals::status->getGoodsProduced())),
                                    rowY + Globals::layout->field_0x4, false);
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, *Globals::gameText->getText(560) + separator,
                                    Globals::layout->buttonInsetX + (this->boxWidth >> 1) +
                                        Globals::layout->field_0x2c_rowHeight + Globals::layout->field_0x4c,
                                    rowY + Globals::layout->field_0x4 * 2, false);
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, String(Globals::status->field_a0),
                                    this->boxWidth + Globals::layout->buttonInsetX - Globals::layout->field_0x4c -
                                        Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font, String(Globals::status->field_a0)),
                                    rowY + Globals::layout->field_0x4 * 2, false);
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, *Globals::gameText->getText(561) + separator,
                                    Globals::layout->buttonInsetX + (this->boxWidth >> 1) +
                                        Globals::layout->field_0x2c_rowHeight + Globals::layout->field_0x4c,
                                    rowY + Globals::layout->field_0x4 * 3, false);
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, String(Globals::status->field_a4),
                                    this->boxWidth + Globals::layout->buttonInsetX - Globals::layout->field_0x4c -
                                        Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font, String(Globals::status->field_a4)),
                                    rowY + Globals::layout->field_0x4 * 3, false);
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, *Globals::gameText->getText(567) + separator,
                                    Globals::layout->buttonInsetX + (this->boxWidth >> 1) +
                                        Globals::layout->field_0x2c_rowHeight + Globals::layout->field_0x4c,
                                    rowY + Globals::layout->field_0x4 * 4, false);
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, String(Globals::status->field_d4),
                                    this->boxWidth + Globals::layout->buttonInsetX - Globals::layout->field_0x4c -
                                        Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font, String(Globals::status->field_d4)),
                                    rowY + Globals::layout->field_0x4 * 4, false);
    }

    if (this->activeTab == 1 || Globals::iPad) {
        // Medal grid.
        int columnW = this->boxWidth / 3;
        int gridX;
        if (Globals::iPad) {
            gridX = this->boxWidth + (columnW >> 1) + Globals::layout->buttonInsetX;
            top = Globals::layout->field_0xc_leftMargin + Globals::layout->field_0x20_top + this->scrollOffset;
        } else {
            gridX = Globals::layout->buttonInsetX + (columnW >> 1);
        }
        int gridY = Globals::layout->field_0xc_leftMargin + (this->medalRowHeight >> 1) +
                    Globals::layout->field_0x2c_rowHeight;
        if (Globals::iPad) {
            Globals::layout->drawBox(0, this->boxWidth + Globals::layout->buttonInsetX * 2, top,
                                     Globals::layout->buttonInsetX + this->boxWidth, Globals::layout->field_0x1c,
                                     String(*Globals::gameText->getText(168), false));
            gridY = Globals::layout->field_0x1c + gridY + Globals::layout->field_0x2c_rowHeight;
        }
        for (unsigned int i = 0; (int) i < this->medalCount; i++) {
            int column = i % 3;
            int row = i / 3;
            int y = row * this->medalRowHeight + gridY + this->scrollOffset;
            (*this->medalButtons)[i]->setPosition(column * columnW + gridX, y);
            if (y >= 0 && y <= Globals::h)
                (*this->medalButtons)[i]->draw();
        }
        if (this->selectedMedal >= 0) {
            Globals::Canvas->SetColor(0xffffffff);
            Globals::layout->drawBox(2, Globals::layout->buttonInsetX,
                                     Globals::h - Globals::layout->field_0x10_rightMargin - Globals::layout->field_0x24 -
                                         (Globals::layout->field_0x4 * this->detailLines->size() +
                                         Globals::layout->field_0x4c * 2),
                                     this->boxWidth,
                                     Globals::layout->field_0x4c * 2 + Globals::layout->field_0x4 * this->detailLines->size(),
                                     String(""));
            Globals::layout->drawBox(5, Globals::layout->buttonInsetX,
                                     Globals::h - Globals::layout->field_0x10_rightMargin - Globals::layout->field_0x24 -
                                         (Globals::layout->field_0x4 * this->detailLines->size() +
                                         Globals::layout->field_0x4c * 2),
                                     this->boxWidth,
                                     Globals::layout->field_0x4c * 2 + Globals::layout->field_0x4 * this->detailLines->size(),
                                     String(""));
            Globals::globals->drawLines((unsigned int) (uintptr_t) Globals::font, this->detailLines,
                                        Globals::layout->field_0x4c + Globals::layout->buttonInsetX,
                                        Globals::h - Globals::layout->field_0x10_rightMargin - Globals::layout->field_0x24 -
                                            Globals::layout->field_0x2c_rowHeight -
                                            Globals::layout->field_0x4 * this->detailLines->size(),
                                        false);
        }
    }

    Globals::layout->drawHeader(String(*Globals::gameText->getText(169), false));
    Globals::layout->drawFooter();
    if (!Globals::iPad) {
        for (unsigned int i = 0; i < this->tabButtons->size(); i++)
            (*this->tabButtons)[i]->draw();
    }
}


StatusWindow::StatusWindow() {
    this->tabButtons = new Array<TouchButton *>();
    ArraySetLength(2, *this->tabButtons);
    (*this->tabButtons)[1] = new TouchButton(*Globals::gameText->getText(0xa8), 3,
                                             Globals::w - Globals::layout->getHelpButtonOffset(), 0, 0x12);
    (*this->tabButtons)[0] = new TouchButton(*Globals::gameText->getText(0x241), 3,
                                             Globals::w - Globals::layout->getHelpButtonOffset() -
                                             (*this->tabButtons)[1]->getWidth() + Globals::layout->field_0x38,
                                             0, 0x12);
    this->activeTab = Globals::iPad;
    (*this->tabButtons)[this->activeTab]->setAlwaysPressed(true);
    this->medalRowHeight = Globals::layout->field_0x84;
    this->detailLines = 0;
    this->selectedMedal = -1;

    this->medalButtons = new Array<TouchButton *>();
    this->medalCount = 45;
    ArraySetLength(45, *this->medalButtons);
    int *medals = Globals::achievements->getMedals();
    for (int i = 0; i < this->medalCount; i++)
        (*this->medalButtons)[i] = new TouchButton(i, medals[i], *Globals::gameText->getText(0x5e3 + i), 0, 0, 0x44);

    this->reInit();
    Globals::Canvas->Image2DCreate(0x48e, this->standingEmblemImage);
    Globals::Canvas->Image2DCreate(0x48f, this->standingBarImage);
    Globals::Canvas->Image2DCreate(0x48d, this->standingFrameImage);
    this->standingBarWidth = Globals::Canvas->GetImage2DWidth(this->standingEmblemImage) / 2;
    this->standingBarHeight = Globals::Canvas->GetImage2DHeight(this->standingEmblemImage);
    this->scrollOffset = 0;
    this->lastTouchY = 0;
    this->scrollTarget = 0;
    this->scrollVelocity = 0;
    this->scrollDamping = 0.0f;
    this->scrollVelocityF = 0.0f;
    this->touchStartY = 0;
    this->isDragging = 0;

    this->tabHeights = new int[3];
    Layout *layout = Globals::layout;
    this->tabHeights[0] = layout->field_0x1c * 3 + layout->field_0x2d8 + layout->field_0x2c_rowHeight * 2 +
                          this->charImageHeight + layout->field_0x4 +
                          (layout->field_0x4 + layout->field_0x2c_rowHeight) * 6;
    if (Globals::iPad != 0)
        this->tabHeights[1] = layout->field_0x1c + layout->field_0x2c_rowHeight +
                              this->medalCount / 3 * this->medalRowHeight + 10;
    else
        this->tabHeights[1] = this->medalCount / 3 * this->medalRowHeight + 10;
    this->contentHeight = this->tabHeights[this->activeTab];
    this->viewportHeight = Globals::h - layout->field_0x10_rightMargin - layout->field_0xc_leftMargin -
                           layout->field_0x20_top - layout->field_0x24;
}
