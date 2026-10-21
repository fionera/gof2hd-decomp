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

int StatusWindow::OnTouchMove(int x, int y) {
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
    return 0;
}

float StatusWindow::getRelativeScrollStartPos() {
    int range = this->scrollOffset;
    if (range > 0) return 0.0f;
    return -(float) range / (float) this->contentHeight;
}

void Status_replaceHash(void *out, void *key, void *a, void *b); // lint: void_ptr (external symbol; param types are mangling-load-bearing)

// lint: void_ptr (external symbol; param types are mangling-load-bearing)


void StatusWindow::OnTouchEnd(int x, int y) {
    int vy = this->scrollVelocity;
    int newOff = this->scrollOffset + vy;
    float vf = (float) vy;
    int absvy = vy < 0 ? -vy : vy;
    Layout *layout = Globals::layout;

    this->scrollDamping = 0.9f;
    this->isDragging = 0;
    this->scrollOffset = newOff;
    this->scrollTarget = newOff;
    this->scrollVelocityF = (absvy > 3) ? vf : 0.0f;

    if (layout->OnTouchEnd(x, y) != 0)
        return;

    if (Globals::iPad == 0) {
        for (unsigned int i = 0; i < this->tabButtons->size(); i++) {
            if ((*this->tabButtons)[i]->OnTouchEnd(x, y) != 0) {
                this->activeTab = i;
                this->contentHeight = this->tabHeights[i];
                this->scrollOffset = 0;
            }
        }
    }

    if (this->activeTab == 1) {
        for (int i = 0; i < this->medalCount; i++) {
            if ((*this->medalButtons)[i]->OnTouchEnd(x, y) != 0) {
                Achievements *ach = Globals::achievements;
                int *medals = ach->getMedals();
                int elite = ach->isEliteMedal(i);
                if (elite != 0 || medals[i] != 0) {
                    if (this->selectedMedal >= 0) {
                        (*this->medalButtons)[this->selectedMedal]->setAlwaysPressed(false);
                    }
                    this->selectedMedal = i;

                    if (this->detailLines != 0) {
                        ArrayReleaseClasses(*this->detailLines);
                        delete this->detailLines;
                    }
                    this->detailLines = new Array<String *>();

                    int count = (elite == 0) ? medals[this->selectedMedal] : 1;

                    String hdr, valStr, valTmp, full;
                    Status *key = Globals::status;
                    String *t = (Globals::gameText)->getText(this->selectedMedal + 0x610);
                    hdr.Set((t)->data);
                    String *val = (String *) (intptr_t) ach->getValue(this->selectedMedal, count);
                    valStr.Set(val->data);
                    valTmp.Set((valStr).data);
                    Status_replaceHash(&full, key, &hdr, &valTmp);

                    String hint = this->getMedalHintText(this->selectedMedal);
                    full += hint;

                    (Globals::globals)->getLineArray(
                        (unsigned int) (uintptr_t) Globals::font,
                        full, this->boxWidth - layout->field_0x4c * 2, this->detailLines);
                    (*this->medalButtons)[i]->setAlwaysPressed(true);
                }
                break;
            }
        }
    }

    if (layout->helpPressed() != 0) {
        if (this->activeTab == 1) {
            String title;
            String *t = (Globals::gameText)->getText(0x287);
            title.Set((t)->data);
            layout->initHelpWindow(title);
        } else if (this->activeTab == 0) {
            String title;
            String *t = (Globals::gameText)->getText(0x280);
            title.Set((t)->data);
            layout->initHelpWindow(title);
        }
    }
}

int StatusWindow::OnTouchBegin(int x, int y) {
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
    return 0;
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
    int *medals = Globals::achievements->getMedals();
    int state = medals[medalIndex];

    String out;
    {
        if (out.data) delete[] out.data;
        out.data = nullptr;
        out.length = 0;
    }

    if (state != 0) {
        String tmpA;
        String tmpB;

        if (medalIndex == 2 && state == 2) {
            tmpA.ctor_char("\n", false);
            String *hdr = (Globals::gameText)->getText(0x114);
            tmpB = tmpA + *hdr;
            out += tmpB;

            Status *base = Globals::status;
            Array<bool> *list = base->field_94;
            for (unsigned int i = 0; i < list->size(); i++) {
                if ((*list)[i] == 0) {
                    tmpA.ctor_char("\n", false);
                    String *t = (Globals::gameText)->getText(0x594 + (int) i);
                    tmpB = tmpA + *t;
                    out += tmpB;
                }
            }
        } else if (medalIndex == 3 && state == 2) {
            tmpA.ctor_char("\n", false);
            String *hdr = (Globals::gameText)->getText(0x114);
            tmpB = tmpA + *hdr;
            out += tmpB;

            Status *base = Globals::status;
            Array<bool> *list = base->field_98;
            for (unsigned int i = 0; i < list->size(); i++) {
                if ((*list)[i] == 0) {
                    tmpA.ctor_char("\n", false);
                    String *t = (Globals::gameText)->getText(0x59f + (int) i);
                    tmpB = tmpA + *t;
                    out += tmpB;
                }
            }
        } else if (medalIndex == 9 && state == 2) {
            tmpA.ctor_char("\n", false);
            String *hdr = (Globals::gameText)->getText(0x114);
            tmpB = tmpA + *hdr;
            out += tmpB;

            Status *base = Globals::status;
            Array<bool> *list = base->field_ac;
            for (unsigned int i = 0; i < list->size(); i++) {
                if ((*list)[i] == 0) {
                    tmpA.ctor_char("\n", false);
                    String *t = (Globals::gameText)->getText(0x57e + (int) i);
                    tmpB = tmpA + *t;
                    out += tmpB;
                }
            }
        } else if (medalIndex == 0xd && state == 2) {
            tmpA.ctor_char("\n", false);
            String *hdr = (Globals::gameText)->getText(0x114);
            tmpB = tmpA + *hdr;
            out += tmpB;

            Status *root = Globals::status;
            for (unsigned int i = 0; i < 0xd; i++) {
                BluePrint *bp = (*root->bluePrints)[i];
                if (bp->locked == 0) {
                    tmpA.ctor_char("\n", false);
                    int idx = bp->getIndex();
                    String *t = (Globals::gameText)->getText(idx + 0x4fa);
                    tmpB = tmpA + *t;
                    out += tmpB;
                }
            }
        } else if (medalIndex == 0xe && state == 2) {
            tmpA.ctor_char("\n", false);
            String *hdr = (Globals::gameText)->getText(0x114);
            tmpB = tmpA + *hdr;
            out += tmpB;

            Status *root = Globals::status;
            for (unsigned int i = 0; i < 0xd; i++) {
                BluePrint *bp = (*root->bluePrints)[i];
                if (bp->productionCount == 0) {
                    tmpA.ctor_char("\n", false);
                    int idx = bp->getIndex();
                    String *t = (Globals::gameText)->getText(idx + 0x4fa);
                    tmpB = tmpA + *t;
                    out += tmpB;
                }
            }
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

static int *g_swd_textId = nullptr;

void StatusWindow::draw() {
    PaintCanvas *canvas = Globals::Canvas;
    Layout *layout = Globals::layout;
    unsigned int font = (unsigned int) (uintptr_t) Globals::font;
    char *land = (char *) &Globals::iPad;
    int screenW = Globals::w;
    int screenH = Globals::h;

    canvas->SetColor(0xffu);
    canvas->FillRectangle(0, 0, screenW, screenH);
    canvas->SetColor(0xffffffffu);
    layout->drawBG();

    float relStart = this->getRelativeScrollStartPos();
    int contentH = this->viewportHeight;
    float ch = (float) contentH;
    float relH = this->getRelativeScrollHeight();
    int barH = (int) (relH * ch);
    if (barH > 0 || (int) (relStart * ch) > 0) {
        layout->drawScrollBar((screenW - layout->field_0x48) - layout->buttonInsetX,
                              layout->field_0x20_top + layout->field_0xc_leftMargin, contentH, (int) (relStart * ch), barH);
    }

    int top = layout->field_0x20_top + layout->field_0xc_leftMargin;
    int colW;
    if (*land == 0) {
        colW = screenW;
        top += this->scrollOffset;
    } else {
        colW = screenW >> 1;
    }
    if (barH > 0)
        colW = (colW - layout->field_0x48) - layout->field_0x2c_rowHeight;
    this->boxWidth = colW + layout->buttonInsetX * -2;

    String creditStr;
    {
        if (creditStr.data) delete[] creditStr.data;
        creditStr.data = nullptr;
        creditStr.length = 0;
    }
    String sep;
    if (GameText::getLanguage() == 9)
        sep.ctor_char("\xa1", false);
    else
        sep.ctor_char(":", false);

    int tab = this->activeTab;
    char drewStats = 0;

    if (tab == 0 || *land != 0) {
        int boxW = this->boxWidth;
        int x0 = layout->buttonInsetX;
        int pad = layout->field_0x2c_rowHeight;
        String lbl;

        String *t = (Globals::gameText)->getText(*g_swd_textId);
        lbl.Set((t)->data);
        layout->drawBox(0, x0, top, boxW, layout->field_0x1c, lbl, 0);

        int y = layout->field_0x1c + top + pad;

        lbl.ctor_char("", false);
        layout->drawBox(5, x0, y, (boxW >> 1) - pad, layout->field_0x2d8, lbl, 0);
        (Globals::imageFactory)->drawChar(this->imageParts, layout->field_0x4c + x0, y, false);
        String credTmp = Globals::layout->formatCredits(Globals::status->getCredits());
        creditStr = credTmp;
        int tw = canvas->GetTextWidth((unsigned) (uintptr_t) font, creditStr);
        canvas->DrawString((unsigned) (uintptr_t) font, creditStr, (((boxW >> 1) - pad) - x0) - tw, y, false);

        String lvlPrefix, lvlText, lvlFull;
        String *lt = (Globals::gameText)->getText(*g_swd_textId);
        lvlPrefix.ctor_char(" ", false);
        lvlText = *lt;
        int lvl = Globals::status->getLevel();
        lvlFull = lvlText;
        lvlFull += lvl;
        creditStr = lvlFull;
        tw = canvas->GetTextWidth((unsigned) (uintptr_t) font, creditStr);
        canvas->DrawString((unsigned) (uintptr_t) font, creditStr, (((boxW >> 1) - pad) - x0) - tw, y, false);

        String timeStr;
        (Globals::globals)->longToTimeStringNoSeconds(Globals::status->getPlayingTime(), timeStr);
        tw = canvas->GetTextWidth((unsigned) (uintptr_t) font, creditStr);
        canvas->DrawString((unsigned) (uintptr_t) font, creditStr, (((boxW >> 1) - pad) - x0) - tw, y, false);

        lbl.ctor_char("", false);
        layout->drawBox(5, (boxW >> 1) + x0 + pad, y, (boxW >> 1) - pad, layout->field_0x2d8, lbl, 0);
        (Globals::imageFactory)->drawShip(Globals::status->getShip()->getIndex(), x0 + (boxW >> 1) + pad * 2, y);
        String *shipNameTxt = (Globals::gameText)->getText(Globals::status->getShip()->getIndex());
        canvas->DrawString((unsigned) (uintptr_t) font, *shipNameTxt,
                           x0 + (boxW >> 1) + pad * 3 + layout->field_0x2cc, y, false);

        String frac, fracStr, pct;
        int firePow = Globals::status->getShip()->getFirePower();
        float firePowF = *(float *) &firePow;
        frac.Set((long long) (int) ((firePowF - (float) (int) firePowF) * 100.0f));
        fracStr = frac.SubString(0, 2);
        int fp2 = Globals::status->getShip()->getFirePower();
        pct.ctor_char("%", false);
        String fpFull = (fp2 + pct) + fracStr;
        creditStr = fpFull;
        tw = canvas->GetTextWidth((unsigned) (uintptr_t) font, creditStr);
        canvas->DrawString((unsigned) (uintptr_t) font, creditStr, ((y + x0) - pad) - tw, y, false);

        String hpStr;
        hpStr.Set((long long) (Globals::status->getShip())->getCombinedHP());
        tw = canvas->GetTextWidth((unsigned) (uintptr_t) font, hpStr);
        canvas->DrawString((unsigned) (uintptr_t) font, hpStr, ((y + x0) - pad) - tw, y, false);

        int standingX0 = x0 + (boxW >> 2);
        Standing *standing = (Standing *) (intptr_t) Globals::status->getStanding();
        float rate = standing->getStandingRate(0);
        canvas->DrawImage2D(this->standingEmblemImage, standingX0, y, (unsigned char) '\x11');
        canvas->DrawRegion2D(this->standingBarImage, this->standingBarWidth, 0,
                             (int) -(rate * (float) this->standingBarWidth), this->standingBarHeight,
                             -(rate * (float) this->standingBarWidth), 0, 0, 0, standingX0);
        canvas->DrawImage2D(this->standingFrameImage,
                            (int) ((float) standingX0 - rate * (float) this->standingBarWidth),
                            y, (unsigned char) '\x11');

        int standingX1 = (boxW - ((boxW - pad * 2) >> 2)) + pad * 2;
        Standing *standing2 = (Standing *) (intptr_t) Globals::status->getStanding();
        float rate2 = standing2->getStandingRate(1);
        canvas->DrawImage2D(this->standingEmblemImage, standingX1, y, (unsigned char) '\x11');
        canvas->DrawRegion2D(this->standingBarImage, this->standingBarWidth, 0,
                             (int) -(rate2 * (float) this->standingBarWidth), this->standingBarHeight,
                             -(rate2 * (float) this->standingBarWidth), 0, 0, 0, standingX1);
        canvas->DrawImage2D(this->standingFrameImage,
                            (int) ((float) standingX1 - rate2 * (float) this->standingBarWidth),
                            y, (unsigned char) '\x11');

        Status *st = Globals::status;
        String rowStr;
        int rowX = layout->field_0x4c + x0;
        for (unsigned r = 0; r < 6; r++) {
            int rowVal;
            switch (r) {
                case 0: rowVal = st->getMissionCount();
                    break;
                case 1: rowVal = st->getKills();
                    break;
                case 2: rowVal = st->getCapturedCrates();
                    break;
                case 3: rowVal = st->getStationsVisited();
                    break;
                case 4: rowVal = st->getJumpgateUsed();
                    break;
                default: rowVal = st->getGoodsProduced();
                    break;
            }
            String *labelTxt = (Globals::gameText)->getText(*g_swd_textId);
            canvas->DrawString((unsigned) (uintptr_t) font, *labelTxt, rowX, y, false);
            rowStr.Set((long long) (rowVal));
            tw = canvas->GetTextWidth((unsigned) (uintptr_t) font, rowStr);
            canvas->DrawString((unsigned) (uintptr_t) font, rowStr, ((y + x0) - pad) - tw, y, false);
        }

        drewStats = *land;
        tab = this->activeTab;
    }

    if (drewStats != 0 || tab == 1) {
        int boxW = this->boxWidth;
        int third = boxW / 3;
        int x0 = layout->buttonInsetX;
        int rowH = this->medalRowHeight;
        int gridX0 = drewStats ? (boxW + (third >> 1) + x0) : (x0 + (third >> 1));
        int gridY0 = layout->field_0xc_leftMargin + (rowH >> 1) + layout->field_0x2c_rowHeight;

        if (drewStats != 0) {
            String hdr;
            String *t = (Globals::gameText)->getText(*g_swd_textId);
            hdr.Set((t)->data);
            layout->drawBox(0, boxW + x0 * 2, top, x0 + boxW, layout->field_0x1c, hdr, 0);
            gridY0 += layout->field_0x1c + layout->field_0x2c_rowHeight;
        }

        for (int i = 0; i < this->medalCount; i++) {
            int col = (int) ((unsigned) i / 3u);
            int by = col * rowH + gridY0 + this->scrollOffset;
            (*this->medalButtons)[i]->setPosition((i - col * 3) * third + gridX0, by, 0x44);
            if (by >= 0 && by <= screenH)
                (*this->medalButtons)[i]->draw();
        }

        if (this->selectedMedal >= 0) {
            canvas->SetColor(0xffffffffu);
            int lines = (int) this->detailLines->size();
            int lineH = layout->field_0x4;
            String lbl;
            lbl.ctor_char("", false);
            layout->drawBox(2, layout->buttonInsetX, (((screenH - layout->field_0x10_rightMargin) -
                                                       layout->field_0x24) - lineH * lines) +
                                                     layout->field_0x4c * -2, this->boxWidth,
                            layout->field_0x4c * 2 + lineH * lines, lbl, 0);

            lbl.ctor_char("", false);
            layout->drawBox(5, layout->buttonInsetX, (((screenH - layout->field_0x10_rightMargin) -
                                                       layout->field_0x24) - lineH * lines) +
                                                     layout->field_0x4c * -2, this->boxWidth,
                            layout->field_0x4c * 2 + lineH * lines, lbl, 0);

            static_cast<Globals *>(Globals::globals)->drawLines((unsigned) (uintptr_t) font, this->detailLines,
                                                              layout->field_0x4c + layout->buttonInsetX,
                                                              (char) screenH);
        }
    }

    String header;
    String *ht = (Globals::gameText)->getText(*g_swd_textId);
    header.Set((ht)->data);
    layout->drawHeader(header);
    layout->drawFooter();

    if (*land == 0) {
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
