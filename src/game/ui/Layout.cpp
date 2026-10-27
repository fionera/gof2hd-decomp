#include "game/ui/Layout.h"
#include "game/ship/Ship.h"
#include "engine/core/AERandom.h"
#include "game/ui/ChoiceWindow.h"
#include "engine/audio/FModSound.h"
#include "game/mission/Status.h"
#include "game/core/Globals.h"
#include "engine/core/GameText.h"
#include "game/core/String.h"
#include "game/ui/TouchButton.h"
#include "engine/render/PaintCanvas.h"



uint8_t Layout::isFading() {
    return this->fading;
}


int Layout::getHelpButtonOffset() {
    int w = this->helpButton->getWidth();
    return w - ((int *) Globals::layout)[0x38 / 4];
}

void Layout::update(int dt) {
    if (this->choiceWindow)
        this->choiceWindow->update(dt);
    if (this->rewardMessageActive != 0) {
        int t = this->rewardMessageTimer + dt;
        this->rewardMessageTimer = t;
        if (t >= 7000)
            this->rewardMessageActive = 0;
    }
    if (this->fading != 0) {
        int t = this->fadeProgress + dt;
        int dur = this->fadeDuration;
        this->fadeProgress = t;
        if (t > dur) {
            this->fading = 0;
            this->fadeProgress = 0;
        }
    }
}


int Layout::getFooterTransitionWidth() {
    PaintCanvas **holder = &Globals::Canvas;
    int w1 = (*holder)->GetImage2DWidth(this->footerImageRight);
    int w2 = (*holder)->GetImage2DWidth(this->footerImageLeft);
    return w2 + w1;
}

bool Layout::OnTouchBegin(int x, int y) {
    if (this->helpButtonEnabled != 0)
        this->helpButton->OnTouchBegin(x, y);
    if (this->choiceWindow != nullptr && this->choiceWindowOpen != 0) {
        this->choiceWindow->OnTouchBegin(x, y);
        return 0;
    }
    if (this->backButton->isVisible() != 0)
        return this->backButton->OnTouchBegin(x, y);
    return this->secondaryButton->OnTouchBegin(x, y);
}

float Layout::getPulseValue(float speed) {
    float a = AbyssEngine::AEMath::Sinf((float) Globals::status->getPlayingTime() * speed);
    float b = AbyssEngine::AEMath::Sinf((float) Globals::status->getPlayingTime() * speed);
    return a > 0.0f ? b : -b;
}

void Layout::setDrawColor(int color) {
    this->drawColor = color;
}





void Layout::drawBG() {
    int p4 = Globals::w;
    int p5 = Globals::h;
    this->drawBGPattern(this->bgPatternImage, 0, 0, p4, p5);
}

void Layout::enableFillScreen(bool v) {
    this->fillScreen = v;
}

Layout::~Layout() {
    delete this->backButton;
    this->backButton = nullptr;
    delete this->secondaryButton;
    this->secondaryButton = nullptr;
    delete this->helpButton;
    this->helpButton = nullptr;
    delete this->choiceWindow;
    this->choiceWindow = nullptr;
}

uint8_t Layout::helpPressed() {
    return this->helpPressedFlag;
}



static int g_dfWarnColor = (int) 0xFF2A00FF;

static const char g_dfSep[] = " / ";

static const char g_dfTail[] = "t";


void Layout::drawFooterNoBackButton() {
    this->drawFooter(false, false);
}

void Layout::drawFooter() {
    this->drawFooter(false, true);
}

bool Layout::OnTouchMove(int x, int y) {
    if (this->helpButtonEnabled != 0)
        this->helpButton->OnTouchMove(x, y);
    if (this->choiceWindow != nullptr && this->choiceWindowOpen != 0) {
        this->choiceWindow->OnTouchMove(x, y);
        return false;
    }
    if (this->backButton->isVisible() != 0)
        return this->backButton->OnTouchMove(x, y);
    return this->secondaryButton->OnTouchMove(x, y);
}

bool Layout::OnTouchEnd(int x, int y) {
    if (this->choiceWindowOpen == 0 && this->helpButtonEnabled != 0)
        this->helpPressedFlag = this->helpButton->OnTouchEnd(x, y);
    if (this->choiceWindow != nullptr && this->choiceWindowOpen != 0)
        return this->choiceWindow->OnTouchEnd(x, y) == 0;
    if (this->backButton->isVisible() != 0)
        return this->backButton->OnTouchEnd(x, y);
    return this->secondaryButton->OnTouchEnd(x, y);
}

void Layout::startFade(bool fadeOut, int color, int duration) {
    this->fading = 1;
    this->fadeOut = fadeOut;
    this->fadeColor = color & 0xffffff00u;
    this->fadeProgress = 0;
    this->fadeDuration = duration;
}





void Layout::initTip() {
    if (this->tipLines != nullptr) {
        ArrayReleaseClasses(*this->tipLines);
        delete this->tipLines;
        this->tipLines = nullptr;
    }
    this->tipLines = new Array<String *>();
    Globals::Canvas->GetLineArray((unsigned int) (uintptr_t) Globals::font,
                                  *Globals::gameText->getText(Globals::rnd->nextInt(13) + 340),
                                  Globals::layout->field_0x78 - 2 * Globals::layout->field_0x4c, this->tipLines);
}





void Layout::drawEmptyFooter(bool showBack) {
    Globals::Canvas->SetColor(this->drawColor);
    int capW = Globals::Canvas->GetImage2DWidth(this->field_0x340);
    Globals::Canvas->DrawImage2D(this->footerImageLeft, 0, Globals::h, 0x11, 0x21);
    this->drawBGPattern(this->footerPatternImage, capW, Globals::h - Globals::layout->field_0x10_rightMargin,
                        Globals::w - 2 * capW, Globals::layout->field_0x10_rightMargin);
    Globals::Canvas->DrawImage2D(this->footerImageLeft, Globals::w - capW,
                                 Globals::h - Globals::layout->field_0x10_rightMargin, 1);
    if (showBack) {
        this->backButton->setVisible(true);
        this->backButton->draw();
    }
}



Layout::Layout() {
    this->windowTopInset = Globals::iPadHD ? 30 * 1.40625f : (Globals::n9 ? 45 : (Globals::retinaDisplay ? 60 : 30));
    this->field_0x4 = Globals::iPadHD ? 15 * 1.40625f : (Globals::n9 ? 22 : (Globals::retinaDisplay ? 30 : 15));
    this->field_0xc_leftMargin = Globals::iPadHD ? 40 : (Globals::n9 ? 51 : (Globals::retinaDisplay ? 58 : 29));
    this->field_0x10_rightMargin = Globals::iPadHD ? 52 : (Globals::n9 ? 58 : (Globals::retinaDisplay ? 74 : 37));
    this->footerTextInset = Globals::iPadHD ? 22 * 1.40625f : (Globals::n9 ? 37 : (Globals::retinaDisplay ? 50 : 22));
    this->headerTitleY = Globals::iPadHD ? 12 : (Globals::n9 ? 11 : (Globals::retinaDisplay ? 14 : 9));
    this->field_0x1c = Globals::iPadHD ? 20 * 1.40625f : (Globals::n9 ? 30 : (Globals::retinaDisplay ? 40 : 20));
    this->field_0x24 = Globals::iPadHD ? 5 * 1.40625f : (Globals::n9 ? 7 : (Globals::retinaDisplay ? 10 : 5));
    this->field_0x20_top = Globals::iPadHD ? 5 * 1.40625f : (Globals::n9 ? 7 : (Globals::retinaDisplay ? 10 : 5));
    this->field_0x2c_rowHeight = Globals::iPadHD ? 4 : (Globals::n9 ? 6 : (Globals::retinaDisplay ? 6 : 3));
    this->field_0x30_rowHeight = Globals::iPadHD ? 30 * 1.40625f : (Globals::n9 ? 51 : (Globals::retinaDisplay ? 60 : 30));
    this->buttonInsetX = Globals::iPadHD ? 5 * 1.40625f : (Globals::n9 ? 9 : (Globals::retinaDisplay ? 10 : 5));
    this->field_0x38 = Globals::iPadHD ? 9 : (Globals::n9 ? 12 : (Globals::retinaDisplay ? 14 : 7));
    this->field_0x34 = Globals::iPadHD ? 8 : (Globals::n9 ? 4 : (Globals::retinaDisplay ? 12 : 6));
    this->field_0x3c = Globals::iPadHD ? 82 : (Globals::n9 ? 106 : (Globals::retinaDisplay ? 118 : 59));
    this->field_0x40 = Globals::iPadHD ? 140 : (Globals::n9 ? 180 : (Globals::retinaDisplay ? 200 : 100));
    this->field_0x44 = Globals::iPadHD ? 7 : (Globals::n9 ? 9 : (Globals::retinaDisplay ? 10 : 5));
    this->field_0x48 = Globals::iPadHD ? 15 : (Globals::n9 ? 16 : (Globals::retinaDisplay ? 22 : 11));
    this->field_0x4c = Globals::iPadHD ? 7 : (Globals::n9 ? 10 : (Globals::retinaDisplay ? 10 : 5));
    this->field_0x58 = Globals::iPadHD ? 243 * 1.40625f : (Globals::n9 ? 364 : (Globals::retinaDisplay ? 486 : 243));
    this->field_0x54 = Globals::iPadHD ? 347 * 1.40625f : (Globals::n9 ? 520 : (Globals::retinaDisplay ? 694 : 347));
    this->field_0x50 = Globals::iPadHD ? 89 * 1.40625f : (Globals::n9 ? 160 : (Globals::retinaDisplay ? 178 : 89));
    this->field_0x5c = Globals::iPadHD ? 45 * 1.40625f : (Globals::n9 ? 51 : (Globals::iPadLarge ? 90 : (Globals::retinaDisplay ? 60 : (Globals::iPad ? 45 : 30))));
    this->field_0x60 = Globals::iPadHD ? 64 * 1.40625f : (Globals::n9 ? 79 : (Globals::iPadLarge ? 128 : (Globals::retinaDisplay ? 88 : (Globals::iPad ? 64 : 44))));
    this->field_0x64 = Globals::iPadHD ? 80 * 1.40625f : (Globals::n9 ? 108 : (Globals::iPadLarge ? 160 : (Globals::retinaDisplay ? 120 : (Globals::iPad ? 80 : 60))));
    this->field_0x68 = Globals::iPadHD ? 403 * 1.40625f : (Globals::n9 ? 545 : (Globals::iPadLarge ? 806 : (Globals::retinaDisplay ? 606 : (Globals::iPad ? 403 : 303))));
    this->field_0x6c = Globals::iPadHD ? 136 * 1.40625f : (Globals::n9 ? 170 : (Globals::iPadLarge ? 272 : (Globals::retinaDisplay ? 262 : (Globals::iPad ? 136 : 131))));
    this->field_0x70_rowHeight = Globals::iPadHD ? 45 * 1.40625f : (Globals::n9 ? 51 : (Globals::iPadLarge ? 90 : (Globals::retinaDisplay ? 60 : (Globals::iPad ? 45 : 30))));
    this->field_0x2c0 = Globals::iPadHD ? -2 : (Globals::n9 ? -3 : (Globals::retinaDisplay ? -4 : -2));
    this->field_0x2c8 = Globals::iPadHD ? 45 * 1.40625f : (Globals::n9 ? 45 : (Globals::iPadLarge ? 90 : (Globals::retinaDisplay ? 60 : (Globals::iPad ? 45 : 30))));
    this->field_0x74 = Globals::iPadHD ? 115 * 1.40625f : (Globals::n9 ? 207 : (Globals::retinaDisplay ? 230 : 115));
    this->field_0x78 = Globals::iPadHD ? 263 * 1.40625f : (Globals::n9 ? 473 : (Globals::retinaDisplay ? 526 : 263));
    this->field_0x2d0 = Globals::iPadHD ? 45 * 1.40625f : (Globals::n9 ? 53 : (Globals::iPadLarge ? 90 : (Globals::retinaDisplay ? 60 : (Globals::iPad ? 45 : 30))));
    this->field_0x2c4 = Globals::iPadHD ? 126 : (Globals::n9 ? 108 : (Globals::iPadLarge ? 180 : (Globals::retinaDisplay ? 120 : (Globals::iPad ? 90 : 60))));
    this->field_0x2cc = Globals::iPadHD ? 90 * 1.40625f : (Globals::n9 ? 106 : (Globals::iPadLarge ? 180 : (Globals::retinaDisplay ? 120 : (Globals::iPad ? 90 : 60))));
    this->field_0x2d8 = Globals::iPadHD ? 141.0f : ((Globals::retinaDisplay || Globals::n9) ? (Globals::iPad ? 200 : 125) : (Globals::iPad ? 125 : 75));
    this->field_0x2d4 = Globals::iPadHD ? 80 * 1.40625f : ((Globals::retinaDisplay || Globals::n9) ? (Globals::iPad ? 160 : 100) : (Globals::iPad ? 100 : 60));
    this->field_0x7c = Globals::iPadHD ? 12 : (Globals::n9 ? 13 : (Globals::retinaDisplay ? 18 : 9));
    this->field_0x84 = Globals::iPadHD ? 80 * 1.40625f : (Globals::n9 ? 120 : (Globals::retinaDisplay ? 160 : 80));
    this->field_0x88 = Globals::iPadHD ? 42 : (Globals::n9 ? 45 : (Globals::retinaDisplay ? 60 : 30));
    this->field_0x80_touchMargin = Globals::iPadHD ? 14 : (Globals::n9 ? 15 : (Globals::retinaDisplay ? 20 : 10));
    this->field_0x8c = Globals::iPadHD ? 32.0f * 1.40625f : (Globals::n9 ? 57.6f : (Globals::retinaDisplay ? 64.0f : 32.0f));
    this->field_0x90 = Globals::iPadHD ? 0.46222222f : (Globals::retinaDisplay ? 0.325f : (Globals::iPad ? 0.325f : (Globals::n9 ? 0.325f : 0.65f)));
    this->field_0x94 = Globals::iPadHD ? 14 : (Globals::n9 ? 15 : (Globals::retinaDisplay ? 20 : 10));
    this->field_0x98 = Globals::iPadHD ? 420 * 1.40625f : (Globals::n9 ? 630 : (Globals::retinaDisplay ? 840 : 420));
    this->field_0x9c = Globals::iPadHD ? 70 : (Globals::n9 ? 75 : (Globals::retinaDisplay ? 100 : 50));
    this->field_0x294_buttonWidth = Globals::iPadHD ? 281 : (Globals::n9 ? 360 : (Globals::retinaDisplay ? 400 : 200));
    this->field_0xac = Globals::iPadHD ? 61 * 1.40625f : (Globals::iPadLarge ? 122 : (Globals::n9 ? 61 : (Globals::iPad ? 61 : (Globals::retinaDisplay ? 82 : 41))));
    this->field_0xa8 = Globals::iPadHD ? 61 * 1.40625f : (Globals::iPadLarge ? 122 : (Globals::n9 ? 61 : (Globals::iPad ? 61 : (Globals::retinaDisplay ? 82 : 41))));
    this->centerYBase = Globals::iPadHD ? 21 : (Globals::n9 ? 22 : (Globals::retinaDisplay ? 30 : 15));
    this->progressBarYBase = Globals::iPadHD ? 52 : (Globals::n9 ? 55 : (Globals::retinaDisplay ? 74 : 37));
    this->field_0xa4 = Globals::iPadHD ? 7 : (Globals::n9 ? 9 : (Globals::retinaDisplay ? 10 : 5));
    this->field_0xdc = Globals::iPadHD ? 10.0f * 1.40625f : (Globals::n9 ? 15.0f : (Globals::retinaDisplay ? 20.0f : 10.0f));
    this->driftSpeedDivisor = Globals::iPadHD ? 20.0f * 1.40625f : (Globals::n9 ? 12.0f : (Globals::retinaDisplay ? 10.0f : 20.0f));
    this->field_0xa0 = Globals::iPadHD ? 25 * 1.40625f : (Globals::n9 ? 45 : (Globals::retinaDisplay ? 50 : 25));
    this->oreLabelYOffset = Globals::iPadHD ? 2 * 1.40625f : (Globals::n9 ? 5 : (Globals::retinaDisplay ? 4 : 2));
    this->animSpeedScale = Globals::iPadHD ? 1.40625f : (Globals::n9 ? 1.5f : (Globals::retinaDisplay ? 2.0f : 1.0f));
    this->hudInset = Globals::iPadHD ? 7 : (Globals::n9 ? 0 : (Globals::retinaDisplay ? 10 : 5));
    this->field_0x298_buttonYBias = Globals::iPadHD ? 84 * 1.40625f : (Globals::n9 ? 146 : (Globals::retinaDisplay ? 168 : 84));
    this->oreTextYOffset = Globals::iPadHD ? 5.0f : (Globals::n9 ? 4 : (Globals::retinaDisplay ? 5 : 3));
    this->field_0xb8 = Globals::iPadHD ? 18 * 1.40625f : (Globals::n9 ? 25 : (Globals::retinaDisplay ? 36 : 18));
    this->field_0xb0 = Globals::iPadHD ? 3 * 1.40625f : (Globals::n9 ? 4 : (Globals::retinaDisplay ? 6 : 3));
    this->marqueeWidth = Globals::iPadHD ? 28 : (Globals::n9 ? 36 : (Globals::retinaDisplay ? 40 : 20));
    this->field_0x29c_buttonRowGap = Globals::iPadHD ? 12 * 1.40625f : (Globals::n9 ? 8 : (Globals::retinaDisplay ? 24 : 12));
    this->field_0xbc = Globals::iPadHD ? 11 : (Globals::n9 ? 12 : (Globals::retinaDisplay ? 16 : 8));
    this->ringRadiusScale = Globals::iPadHD ? 1.40625f : (Globals::n9 ? 1.5f : (Globals::retinaDisplay ? 2.0f : 1.0f));
    this->field_0xf8 = Globals::iPadHD ? 562 : (Globals::n9 ? 600 : (Globals::retinaDisplay ? 800 : 400));
    this->field_0xc8 = Globals::iPadHD ? 4 * 1.40625f : (Globals::n9 ? 10 : (Globals::retinaDisplay ? 10 : 4));
    this->field_0xb4 = Globals::iPadHD ? 20 : (Globals::n9 ? 22 : (Globals::retinaDisplay ? 29 : 15));
    this->ringNearMidThreshold = Globals::iPadHD ? 40 * 1.40625f : (Globals::n9 ? 60 : (Globals::retinaDisplay ? 80 : 40));
    this->field_0x108 = Globals::iPadHD ? -2 : (Globals::n9 ? -1 : (Globals::retinaDisplay ? -2 : 0));
    this->field_0xc4 = Globals::iPadHD ? 8 * 1.40625f : (Globals::n9 ? 11 : (Globals::retinaDisplay ? 14 : 8));
    this->ringMidFarThreshold = Globals::iPadHD ? 200 * 1.40625f : (Globals::n9 ? 300 : (Globals::retinaDisplay ? 400 : 200));
    this->field_0xc0 = Globals::iPadHD ? 9 : (Globals::n9 ? 11 : (Globals::retinaDisplay ? 13 : 6));
    this->field_0x2ac_metricC = Globals::iPadHD ? 147 : (Globals::n9 ? 189 : (Globals::retinaDisplay ? 210 : 105));
    this->field_0x2a0_listRowGap = Globals::iPadHD ? 12 * 1.40625f : (Globals::n9 ? 6 : (Globals::retinaDisplay ? 24 : 12));
    this->field_0xf0 = Globals::iPadHD ? 80 * 1.40625f : (Globals::n9 ? 120 : (Globals::retinaDisplay ? 160 : 80));
    this->field_0xcc = Globals::iPadHD ? 180 * 1.40625f : (Globals::n9 ? 270 : (Globals::retinaDisplay ? 360 : 180));
    this->field_0x2a8_metricB = Globals::iPadHD ? 140 : (Globals::n9 ? 180 : (Globals::retinaDisplay ? 200 : 100));
    this->field_0x2a4_metricA = Globals::iPadHD ? 110 * 1.40625f : (Globals::n9 ? 198 : (Globals::retinaDisplay ? 220 : 110));
    this->field_0x2b4 = Globals::iPadHD ? 35 : (Globals::n9 ? 37 : (Globals::retinaDisplay ? 50 : 25));
    this->field_0x2b0 = Globals::iPadHD ? 30 * 1.40625f : (Globals::n9 ? 54 : (Globals::retinaDisplay ? 60 : 30));
    this->field_0x114_strip5c = Globals::iPadHD ? 1 * 1.40625f : (Globals::n9 ? -1 : (Globals::retinaDisplay ? -1 : 1));
    this->field_0x10c_rowBaseY = Globals::iPadHD ? 1 * 1.40625f : (Globals::n9 ? 2 : (Globals::retinaDisplay ? 2 : 1));
    this->field_0x118 = Globals::iPadHD ? 9 : (Globals::n9 ? 13 : (Globals::retinaDisplay ? 14 : 7));
    this->field_0x2b8 = Globals::iPadHD ? 7 * 1.40625f : (Globals::n9 ? 14 : (Globals::retinaDisplay ? 16 : 7));
    this->field_0x12c = Globals::iPadHD ? 15 * 1.40625f : (Globals::n9 ? 22 : (Globals::retinaDisplay ? 30 : 15));
    this->field_0x11c = Globals::iPadHD ? 25 * 1.40625f : (Globals::n9 ? 37 : (Globals::retinaDisplay ? 50 : 25));
    this->field_0x124 = Globals::iPadHD ? 1 * 1.40625f : (Globals::n9 ? 2 : (Globals::retinaDisplay ? 2 : 1));
    this->field_0x1b0 = Globals::iPadHD ? 18 : (Globals::n9 ? 19 : (Globals::retinaDisplay ? 26 : 13));
    this->field_0x110_strip58 = Globals::iPadHD ? 2 * 1.40625f : (Globals::n9 ? 1 : (Globals::retinaDisplay ? 1 : 2));
    this->field_0x120 = Globals::iPadHD ? 21 : (Globals::n9 ? 22 : (Globals::retinaDisplay ? 28 : 15));
    this->field_0x134 = Globals::iPadHD ? 108 * 1.40625f : (Globals::n9 ? 120 : (Globals::iPadLarge ? 216 : (Globals::retinaDisplay ? 160 : (Globals::iPad ? 108 : 80))));
    this->field_0x138 = Globals::iPadHD ? 47 * 1.40625f : (Globals::n9 ? 52 : (Globals::iPadLarge ? 94 : (Globals::retinaDisplay ? 70 : (Globals::iPad ? 47 : 35))));
    this->field_0x128 = Globals::iPadHD ? 1 * 1.40625f : (Globals::n9 ? 2 : (Globals::retinaDisplay ? 2 : 1));
    this->field_0x130 = Globals::iPadHD ? 78 : (Globals::n9 ? 60 : (Globals::iPadLarge ? 112 : (Globals::retinaDisplay ? 80 : (Globals::iPad ? 56 : 40))));
    this->field_0x14c = Globals::iPadHD ? 189 : (Globals::n9 ? 243 : (Globals::retinaDisplay ? 270 : 135));
    this->field_0x140 = Globals::iPadHD ? 15 : (Globals::n9 ? 19 : (Globals::retinaDisplay ? 22 : 11));
    this->field_0x13c = Globals::iPadHD ? 23 : (Globals::n9 ? 25 : (Globals::retinaDisplay ? 34 : 17));
    this->field_0x144 = Globals::iPadHD ? 13 * 1.40625f : (Globals::n9 ? 19 : (Globals::retinaDisplay ? 26 : 13));
    this->field_0x148 = Globals::iPadHD ? 2 * 1.40625f : (Globals::n9 ? 19 : (Globals::retinaDisplay ? 4 : 2));
    this->field_0x150 = Globals::iPadHD ? 5 : (Globals::n9 ? 6 : (Globals::retinaDisplay ? 8 : 4));
    this->field_0x15c = Globals::iPadHD ? 147 * 1.40625f : (Globals::iPadLarge ? 294 : (Globals::iPad ? 147 : (Globals::n9 ? 136 : (Globals::retinaDisplay ? 158 : 79))));
    this->field_0x154 = Globals::iPadHD ? 50 * 1.40625f : (Globals::iPadLarge ? 100 : (Globals::iPad ? 50 : (Globals::n9 ? 14 : (Globals::retinaDisplay ? 30 : 15))));
    this->field_0x158 = Globals::iPadHD ? 70 : (Globals::iPadLarge ? 100 : (Globals::iPad ? 50 : (Globals::n9 ? 13 : (Globals::retinaDisplay ? 28 : 14))));
    this->field_0x174 = Globals::iPadHD ? 84.0f : (Globals::iPadLarge ? 84 : (Globals::iPad ? 42 : (Globals::n9 ? 45 : (Globals::retinaDisplay ? 60 : 30))));
    this->field_0x170 = Globals::iPadHD ? 174 : (Globals::iPadLarge ? 248 : (Globals::iPad ? 124 : (Globals::n9 ? 106 : (Globals::retinaDisplay ? 128 : 64))));
    this->field_0x178 = Globals::iPadHD ? 84 : (Globals::iPadLarge ? 120 : (Globals::iPad ? 60 : (Globals::n9 ? 27 : (Globals::retinaDisplay ? 44 : 22))));
    this->field_0x164 = Globals::iPadHD ? 0.0f : (Globals::iPadLarge ? 0 : (Globals::iPad ? 0 : (Globals::n9 ? 23 : (Globals::retinaDisplay ? 25 : 13))));
    this->field_0x168 = Globals::iPadHD ? 25 * 1.40625f : (Globals::n9 ? 37 : (Globals::retinaDisplay ? 50 : 25));
    this->field_0x160 = Globals::iPadHD ? 49 * 1.40625f : (Globals::iPadLarge ? 98 : (Globals::iPad ? 49 : (Globals::n9 ? 13 : (Globals::retinaDisplay ? 26 : 13))));
    this->field_0x16c = Globals::iPadHD ? 131 * 1.40625f : (Globals::iPadLarge ? 262 : (Globals::iPad ? 131 : (Globals::n9 ? 115 : (Globals::retinaDisplay ? 138 : 69))));
    this->field_0x17c = Globals::iPadHD ? 155 * 1.40625f : (Globals::iPadLarge ? 310 : (Globals::iPad ? 155 : (Globals::n9 ? 146 : (Globals::retinaDisplay ? 170 : 85))));
    this->field_0x188 = Globals::iPadHD ? 17 * 1.40625f : (Globals::n9 ? 30 : (Globals::retinaDisplay ? 34 : 17));
    this->field_0x180 = Globals::iPadHD ? 23 : (Globals::n9 ? 30 : (Globals::retinaDisplay ? 34 : 17));
    this->field_0x18c = Globals::iPadHD ? 244.0f : (Globals::iPadLarge ? 446 : (Globals::iPad ? 223 : (Globals::n9 ? 320 : (Globals::retinaDisplay ? 348 : 174))));
    this->field_0x190 = Globals::iPadHD ? -6 : (Globals::iPadLarge ? -8 : (Globals::n9 ? -4 : (Globals::iPad ? -4 : (Globals::retinaDisplay ? -2 : -1))));
    this->field_0x184 = Globals::iPadHD ? 163 : (Globals::iPadLarge ? 346 : (Globals::iPad ? 173 : (Globals::n9 ? 210 : (Globals::retinaDisplay ? 232 : 116))));
    this->field_0x19c = Globals::iPadHD ? 90 : (Globals::iPadLarge ? 128 : (Globals::iPad ? 64 : (Globals::n9 ? 54 : (Globals::retinaDisplay ? 90 : 45))));
    this->field_0x198 = Globals::iPadHD ? 12 * 1.40625f : (Globals::iPadLarge ? 24 : (Globals::iPad ? 12 : (Globals::n9 ? 9 : (Globals::retinaDisplay ? 12 : 6))));
    this->field_0x1a0 = Globals::iPadHD ? 18 : (Globals::iPadLarge ? 26 : (Globals::iPad ? 13 : (Globals::n9 ? 10 : (Globals::retinaDisplay ? 14 : 7))));
    this->field_0x1a4 = Globals::iPadHD ? 22 : (Globals::n9 ? 28 : (Globals::retinaDisplay ? 32 : 16));
    this->field_0x1ac = Globals::iPadHD ? 173 * 1.40625f : (Globals::iPadLarge ? 346 : (Globals::iPad ? 173 : (Globals::n9 ? 208 : (Globals::retinaDisplay ? 232 : 116))));
    this->field_0x1c4 = Globals::iPadHD ? 19 * 1.40625f : (Globals::n9 ? 34 : (Globals::retinaDisplay ? 38 : 19));
    this->field_0x194 = Globals::iPadHD ? 8 : (Globals::n9 ? 10 : (Globals::retinaDisplay ? 12 : 6));
    this->field_0x1c8 = Globals::iPadHD ? 29 * 1.40625f : (Globals::n9 ? 43 : (Globals::retinaDisplay ? 58 : 29));
    this->field_0x1a8 = Globals::iPadHD ? 16 * 1.40625f : (Globals::n9 ? 24 : (Globals::retinaDisplay ? 32 : 16));
    this->eventBannerScale = Globals::iPadHD ? 1.40625f : (Globals::n9 ? 2.0f : (Globals::retinaDisplay ? 2.0f : 1.0f));
    this->field_0x1d0 = Globals::iPadHD ? 5 * 1.40625f : (Globals::n9 ? 7 : (Globals::retinaDisplay ? 10 : 5));
    this->field_0x1c0 = Globals::iPadHD ? 66 * 1.40625f : (Globals::n9 ? 80 : (Globals::retinaDisplay ? 132 : 66));
    this->field_0x1d8 = Globals::iPadHD ? 4 * 1.40625f : (Globals::n9 ? 6 : (Globals::retinaDisplay ? 8 : 4));
    this->field_0x1b4 = Globals::iPadHD ? 42 : (Globals::n9 ? 45 : (Globals::retinaDisplay ? 60 : 30));
    this->field_0x1bc = Globals::iPadHD ? 1 : (Globals::n9 ? 2 : (Globals::retinaDisplay ? 2 : 1));
    this->field_0x1b8 = Globals::iPadHD ? 46 : (Globals::n9 ? 40 : (Globals::retinaDisplay ? 66 : 33));
    this->field_0x1cc = Globals::iPadHD ? 36 * 1.40625f : (Globals::n9 ? 54 : (Globals::retinaDisplay ? 72 : 36));
    this->field_0x1d4 = Globals::iPadHD ? 17.0f : (Globals::n9 ? 18 : (Globals::retinaDisplay ? 24 : 12));
    this->field_0x1dc = Globals::iPadHD ? 10 * 1.40625f : (Globals::n9 ? 14 : (Globals::retinaDisplay ? 20 : 10));
    this->field_0x1e8 = Globals::retinaDisplay ? 1 : (Globals::iPadHD ? 1 : (Globals::n9 ? 1 : 0));
    this->field_0x1e4 = Globals::iPadHD ? 5 : (Globals::n9 ? 6 : (Globals::retinaDisplay ? 8 : 4));
    this->quickMenuIconGap = Globals::iPadHD ? 1 * 1.40625f : (Globals::n9 ? 2 : (Globals::retinaDisplay ? 2 : 1));
    this->quickMenuHeaderLift = Globals::iPadHD ? 2 : (Globals::n9 ? 4 : (Globals::retinaDisplay ? 4 : 2));
    this->field_0x26c = Globals::iPadHD ? 66 * 1.40625f : (Globals::n9 ? 99 : (Globals::retinaDisplay ? 132 : 66));
    this->field_0x1ec = Globals::iPadHD ? 26 : (Globals::n9 ? 34 : (Globals::retinaDisplay ? 38 : 19));
    this->orbitInfoSecurityY = Globals::iPadHD ? 49 : (Globals::n9 ? 52 : (Globals::retinaDisplay ? 70 : 35));
    this->orbitInfoSystemY = Globals::iPadHD ? 20 * 1.40625f : (Globals::n9 ? 30 : (Globals::retinaDisplay ? 40 : 20));
    this->field_0x1fc = Globals::iPadHD ? 2 : (Globals::n9 ? 4 : (Globals::retinaDisplay ? 4 : 2));
    this->field_0x208 = Globals::iPadHD ? 4 * 1.40625f : (Globals::n9 ? 7 : (Globals::retinaDisplay ? 8 : 4));
    this->field_0x238_blk16.a = Globals::iPadHD ? 50 * 1.40625f : (Globals::n9 ? 90 : (Globals::retinaDisplay ? 100 : 50));
    this->field_0x20c = Globals::iPadHD ? 5 * 1.40625f : (Globals::n9 ? 7 : (Globals::retinaDisplay ? 10 : 5));
    this->field_0x250 = Globals::iPadHD ? 1 * 1.40625f : (Globals::n9 ? 2 : (Globals::retinaDisplay ? 2 : 1));
    this->field_0x24c = Globals::iPadHD ? -2 : (Globals::n9 ? -1 : (Globals::retinaDisplay ? -2 : 0));
    this->field_0x234 = Globals::iPadHD ? 1 * 1.40625f : (Globals::n9 ? 2 : (Globals::retinaDisplay ? 2 : 1));
    this->field_0x204 = Globals::iPadHD ? 9 * 1.40625f : (Globals::n9 ? 16 : (Globals::retinaDisplay ? 18 : 9));
    this->field_0x210 = Globals::iPadHD ? 5 : (Globals::n9 ? 6 : (Globals::retinaDisplay ? 8 : 4));
    this->field_0x238_blk16.d = Globals::iPadHD ? 3 * 1.40625f : (Globals::n9 ? 5 : (Globals::retinaDisplay ? 6 : 3));
    this->field_0x218 = Globals::iPadHD ? 5 * 1.40625f : (Globals::n9 ? 7 : (Globals::retinaDisplay ? 10 : 5));
    this->field_0x238_blk16.c = Globals::iPadHD ? 7 : (Globals::n9 ? 9 : (Globals::retinaDisplay ? 10 : 5));
    this->field_0x214 = Globals::iPadHD ? 1 : (Globals::n9 ? -2 : (Globals::retinaDisplay ? -2 : 1));
    this->orbitInfoTextX = Globals::iPadHD ? 8 : (Globals::n9 ? 10 : (Globals::retinaDisplay ? 12 : 6));
    this->field_0x268 = Globals::iPadHD ? 11 : (Globals::n9 ? 12 : (Globals::retinaDisplay ? 16 : 8));
    this->field_0x25c = Globals::iPadHD ? 16 * 1.40625f : (Globals::n9 ? 28 : (Globals::retinaDisplay ? 32 : 16));
    this->field_0x238_blk16.b = Globals::iPadHD ? 15 * 1.40625f : (Globals::n9 ? 27 : (Globals::retinaDisplay ? 30 : 15));
    this->field_0x264 = Globals::iPadHD ? 300 * 1.40625f : (Globals::n9 ? 450 : (Globals::retinaDisplay ? 600 : 300));
    this->orbitInfoStationY = Globals::iPadHD ? 5 * 1.40625f : (Globals::n9 ? 7 : (Globals::retinaDisplay ? 10 : 5));
    this->field_0x274 = Globals::iPadHD ? 97 * 1.40625f : (Globals::n9 ? 145 : (Globals::retinaDisplay ? 194 : 97));
    this->field_0x248 = Globals::iPadHD ? 170 * 1.40625f : (Globals::n9 ? 306 : (Globals::retinaDisplay ? 340 : 170));
    this->field_0x280 = Globals::iPadHD ? 0 : (Globals::n9 ? 4 : (Globals::retinaDisplay ? 5 : 0));
    this->field_0x278 = Globals::iPadHD ? 421 : (Globals::n9 ? 450 : (Globals::retinaDisplay ? 600 : 300));
    this->field_0x258 = Globals::iPadHD ? 2 : (Globals::n9 ? 4 : (Globals::retinaDisplay ? 4 : 2));
    this->field_0x254 = Globals::iPadHD ? 5 : (Globals::n9 ? 7 : (Globals::retinaDisplay ? 8 : 4));
    this->field_0x260 = Globals::iPadHD ? 1 * 1.40625f : (Globals::n9 ? 0 : (Globals::retinaDisplay ? 0 : 1));
    this->footerButtonOffset = Globals::iPadHD ? 4 : (Globals::n9 ? 5 : (Globals::retinaDisplay ? 6 : 3));
    this->scrollBarHandle = Globals::iPadHD ? 0.0f : (Globals::n9 ? 1 : Globals::retinaDisplay);
    this->rewardBoxY2 = Globals::iPadHD ? 132 : (Globals::n9 ? 111 : (Globals::iPadLarge ? (Globals::retinaDisplay ? 254 : 127) : (Globals::retinaDisplay ? (Globals::iPad ? 144 : 186) : (Globals::iPad ? 72 : 55))));
    this->field_0x270 = Globals::iPadHD ? 70 * 1.40625f : (Globals::n9 ? 105 : (Globals::retinaDisplay ? 140 : 70));
    this->rewardBoxWidth = Globals::iPadHD ? 100 * 1.40625f : (Globals::n9 ? 112 : (Globals::iPadLarge ? 200 : (Globals::iPad ? 100 : (Globals::retinaDisplay ? 150 : 75))));
    this->rewardBoxY = Globals::iPadHD ? 55 * 1.40625f : (Globals::n9 ? 57 : (Globals::iPadLarge ? 110 : (Globals::iPad ? 55 : (Globals::retinaDisplay ? 76 : 38))));
    this->rewardBoxX = Globals::n9 ? 75 : (Globals::retinaDisplay ? 100 : 50);
    this->rewardBoxHeight = Globals::n9 ? 352 : (Globals::retinaDisplay ? 392 : 196);
    this->field_0x1f4 = Globals::iPadHD ? 16 * 1.40625f : (Globals::retinaDisplay ? (Globals::iPad ? 32 : 4) : (Globals::iPad ? 16 : 2));
    this->field_0x200 = Globals::iPadHD ? -18 : (Globals::n9 ? -26 : (Globals::retinaDisplay ? -26 : -13));
    this->field_0x1f0 = Globals::iPadHD ? 19 * 1.40625f : (Globals::n9 ? 34 : (Globals::retinaDisplay ? 38 : 19));
    this->field_0x1f8 = Globals::iPadHD ? 2 : (Globals::retinaDisplay ? (Globals::iPad ? 4 : 2) : 2);
    this->scrollBarInset = Globals::iPadHD ? 1 * 1.40625f : (Globals::n9 ? 2 : (Globals::retinaDisplay ? 2 : 1));
    this->field_0x2bc = Globals::iPadHD ? 2 : (Globals::n9 ? 3 : 2);
    this->field_0x286 = Globals::n9 ? 0 : 1;
    this->field_0x288 = Globals::n9 ? 20 : 0;
    this->field_0x27c = Globals::iPadHD ? 20 * 1.40625f : (Globals::n9 ? 30 : (Globals::retinaDisplay ? 40 : 20));
    this->field_0x290 = Globals::n9 ? 4 : (Globals::retinaDisplay ? 7 : 3);
    this->field_0x28c = Globals::n9 ? -19 : 0;
    this->field_0x284_sliderSlot5Enabled = Globals::n9 ? 0 : 1;
    this->field_0x2f4 = Globals::iPadHD ? 112 : (Globals::iPadLarge ? 160 : (Globals::iPad ? 80 : (Globals::n9 ? 45 : (Globals::retinaDisplay ? 60 : 30))));
    this->field_0x2f8 = Globals::iPadHD ? 281.25f : (Globals::iPadLarge ? 400.0f : (Globals::iPad ? 200.0f : (Globals::n9 ? 150.0f : (Globals::retinaDisplay ? 200.0f : 100.0f))));
    this->field_0x2fc = Globals::iPadHD ? 50 : (Globals::iPadLarge ? 72 : 36);
    this->field_0x300 = Globals::iPadHD ? 281 : (Globals::iPadLarge ? 400 : 200);
    this->field_0x304 = Globals::iPadHD ? 350 * 1.40625f : (Globals::iPadLarge ? 700 : 350);
    this->choiceWindowTitleHeight = Globals::iPadHD ? 250 * 1.40625f : (Globals::iPadLarge ? 500 : 250);
    this->field_0x285 = 1;
    this->field_0x30c = Globals::iPadLarge ? 6 : (Globals::iPad ? 3 : (Globals::n9 ? 4 : (Globals::retinaDisplay ? 7 : 3)));
    this->field_0x310 = Globals::iPadLarge ? -24 : (Globals::iPad ? -12 : (Globals::n9 ? -8 : (Globals::retinaDisplay ? -9 : -5)));
    this->field_0x314 = Globals::iPadLarge ? -10 : (Globals::iPad ? -5 : (Globals::n9 ? -8 : (Globals::retinaDisplay ? -8 : -5)));
    this->field_0x318 = Globals::iPadLarge ? 8 : (Globals::iPad ? 4 : 0);
    this->helpWindowX = Globals::iPadHD ? 331 : (Globals::iPadLarge ? 472 : ((Globals::retinaDisplay || Globals::n9 || Globals::iPad) ? 236 : 118));
    this->helpWindowY = Globals::iPadHD ? 144 * 1.40625f : (Globals::iPadLarge ? 288 : (Globals::iPad ? 144 : (Globals::n9 ? 150 : (Globals::retinaDisplay ? 150 : 76))));
}



void Layout::drawBGPattern(unsigned img, int x, int y, int w, int h) {
    Globals::Canvas->SetColor(this->drawColor);
    int iw = Globals::Canvas->GetImage2DWidth(img);
    int cols = w / iw;
    int fullW = cols * iw;
    int remW = w - fullW;
    int ih = Globals::Canvas->GetImage2DHeight(img);
    int rows = h / ih;
    int fullH = rows * ih;
    int remH = h - fullH;

    for (int r = 0; r < rows; r++) {
        int px = x;
        for (int c = 0; c < cols; c++) {
            Globals::Canvas->DrawImage2D(img, px, r * ih + y);
            px += iw;
        }
    }

    if (remW > 0) {
        int py = y;
        for (int r = 0; r < rows; r++) {
            Globals::Canvas->DrawRegion2D(img, 0, 0, remW, ih, 0, 0, 0, x + fullW, py);
            py += ih;
        }
    }
    if (remH > 0) {
        int px = x;
        for (int c = 0; c < cols; c++) {
            Globals::Canvas->DrawRegion2D(img, 0, 0, iw, remH, 0, 0, 0, px, y + fullH);
            px += iw;
        }
    }
    if (remW > 0 || remH > 0)
        Globals::Canvas->DrawRegion2D(img, 0, 0, remW, remH, 0, 0, 0, x + fullW, y + fullH);
}





String Layout::formatNumber(int value) {
    String digits(value < 0 ? -value : value);
    int len = digits.length;
    String result("");
    unsigned short lang = GameText::getLanguage();
    String sep = (lang == 0 || lang == 10 || lang == 11) ? String(",") : String(".");
    if (len >= 4) {
        int i = len;
        while (i >= 3) {
            result = digits.SubString(i - 3, i) + (result.size() >= 2 ? String(sep) : String()) + result;
            i -= 3;
        }
        String head = digits.SubString(0, i);
        if (head.length != 0)
            result = head + sep + result;
    } else {
        result = digits;
    }
    if (value < 0)
        result = String("-") + result;
    return result;
}

String Layout::formatCredits(int n) {
    String num = this->formatNumber(n);
    String suffix("$");
    return num + suffix;
}







void Layout::setWindowDimensions(int x, int y, int w, int h) {
    this->windowX = x;
    this->windowY = y;
    this->windowWidth = w;
    this->windowHeight = h;
    this->helpButton->setPosition(w + x, y, 0x12);
    this->backButton->setPosition(Globals::layout->buttonInsetX + this->windowX,
                                  this->windowY + this->windowHeight - this->footerButtonOffset, 0x21);
    this->secondaryButton->setPosition(Globals::layout->buttonInsetX + this->windowX,
                                       this->windowY + this->windowHeight - this->footerButtonOffset, 0x21);
    if (this->backButton != nullptr) {
        Globals::other_buttons_x[0] = (int) this->backButton->getPosition().x;
        Globals::other_buttons_y[0] = (int) this->backButton->getPosition().y;
    }
}


void Layout::showMissionRewardMessage(int show, bool flag) {
    if (show == 0)
        return;
    this->rewardMessageActive = 1;
    this->rewardMessageFlag = flag;
    this->rewardMessageTimer = 0;
    this->rewardCredits = show;
    Globals::sound->play(0x24, 0, 0, 0.0f);
}


void Layout::drawBGBorder(unsigned corner, unsigned edge, int x, int y, int w, int h) {
    this->drawBGBorder(corner, edge, x, y, w, h, 0, 0);
}

void Layout::drawBGBorder(unsigned corner, unsigned edge, int x, int y, int w, int h, int inset, int pad) {
    int cornerW = Globals::Canvas->GetImage2DWidth(corner);
    int cornerH = Globals::Canvas->GetImage2DHeight(corner);
    int edgeW = Globals::Canvas->GetImage2DWidth(edge);
    int edgeH = Globals::Canvas->GetImage2DHeight(edge);

    // Corners (flip flags: 1 = mirror x, 2 = mirror y).
    Globals::Canvas->DrawImage2D(corner, inset + x, inset + y);
    int rightX = w + x - cornerW - inset;
    Globals::Canvas->DrawImage2D(corner, rightX, inset + y, 1);
    int bottomY = y + h - cornerH - inset;
    Globals::Canvas->DrawImage2D(corner, inset + x, bottomY, 2);
    Globals::Canvas->DrawImage2D(corner, rightX, bottomY, 3);

    // Top and bottom edges.
    int spanW = w - cornerW * 2 - inset * 2;
    int cols = spanW / edgeW;
    int topY = pad + y;
    int bottomEdgeY = h - inset + topY;
    int remW = spanW - cols * edgeW;
    for (int i = 0; i < cols; i++) {
        Globals::Canvas->DrawImage2D(edge, cornerW + pad + x + i * edgeW, topY);
        Globals::Canvas->DrawImage2D(edge, cornerW + pad + x + i * edgeW, bottomEdgeY, 2);
    }
    if (remW > 0) {
        Globals::Canvas->DrawRegion2D(edge, 0, 0, remW, edgeH, 0.0f, 0, 0, cols * edgeW + (pad + x + cornerW), topY);
        Globals::Canvas->DrawRegion2D(edge, 0, 0, remW, edgeH, 3.14159274f, 0, 0, spanW + (pad + x + cornerW),
                                      y + h - pad);
    }

    // Left and right edges (the edge image rotated by -90/+90 degrees).
    int spanH = h - cornerH * 2 - inset * 2;
    int rows = spanH / edgeW;
    int edgeTop = pad + y + cornerH;
    int remH = spanH - rows * edgeW;
    for (int i = 0; i < rows; i++) {
        // Note: the original steps by rows * edgeW per tile, not edgeW.
        Globals::Canvas->DrawRegion2D(edge, 0, 0, edgeW, edgeH, -1.57079637f, 0, 0, pad + x,
                                      edgeW + cornerH + pad + y + i * rows * edgeW);
        Globals::Canvas->DrawRegion2D(edge, 0, 0, edgeW, edgeH, 1.57079637f, 0, 0, w + x + pad,
                                      cornerH + pad + y + i * rows * edgeW);
    }
    if (remH > 0) {
        Globals::Canvas->DrawRegion2D(edge, 0, 0, remH, edgeH, -1.57079637f, 0, 0, pad + x, spanH + edgeTop);
        Globals::Canvas->DrawRegion2D(edge, 0, 0, remH, edgeH, 1.57079637f, 0, 0, w + x + pad - inset * 2,
                                      cornerH + pad + y + rows * edgeW);
    }
}





void Layout::drawScrollBar(int x, int y, int trackH, int pos, int range) {
    int iw = Globals::Canvas->GetImage2DWidth(this->scrollBarImage);
    int ih = Globals::Canvas->GetImage2DHeight(this->scrollBarImage);
    Globals::Canvas->SetColor(0x111417ff);
    Globals::Canvas->SetColor(0x193043ff);
    Globals::Canvas->DrawRectangle(x, this->scrollBarInset + y, Globals::layout->field_0x48,
                                   trackH - 2 * this->scrollBarInset);
    Globals::Canvas->SetColor(this->drawColor);

    int thumb = range - 1;
    if (thumb <= ih * 2) thumb = ih * 2;
    int off = pos + 1;
    if (thumb + off >= trackH)
        off = trackH - thumb - this->scrollBarInset;
    thumb -= 4 * this->scrollBarHandle;
    off += 2 * this->scrollBarHandle;
    if (thumb > ih * 2) {
        this->drawBGPattern(this->scrollBarFillImage, x + 1 + this->scrollBarHandle, ih + y + off, iw,
                            thumb - 2 * ih);
    } else if (off >= trackH - 2 * ih) {
        off = trackH - 2 * ih;
    }
    Globals::Canvas->DrawImage2D(this->scrollBarImage, this->scrollBarHandle + (x + 1), y + off);
    Globals::Canvas->DrawImage2D(this->scrollBarImage, this->scrollBarHandle + (x + 1), thumb - ih + (y + off), 2);
}


uint8_t Layout::drawFade() {
    if (this->fading != 0) {
        unsigned int saved = Globals::Canvas->GetColor();
        float t = (float) this->fadeProgress / (float) this->fadeDuration;
        float alpha = this->fadeOut != 0 ? t : 1.0f - t;
        alpha = alpha < 1.0f ? alpha : 1.0f;
        int a = (int) (alpha * 255.0f);
        Globals::Canvas->SetColor(this->fadeColor + (alpha > 0.0f ? a : 0));
        Globals::Canvas->FillRectangle(0, 0, Globals::w, Globals::h);
        Globals::Canvas->SetColor(saved);
    }
    if (this->fillScreen != 0) {
        unsigned int saved = Globals::Canvas->GetColor();
        Globals::Canvas->SetColor(0xff);
        Globals::Canvas->FillRectangle(0, 0, Globals::w, Globals::h);
        Globals::Canvas->SetColor(saved);
    }
    return this->fading;
}
















void Layout::drawBox(int style, int x, int y, int w, int h, String text, unsigned char flags) {
    unsigned int savedColor = Globals::Canvas->GetColor();
    Globals::Canvas->SetColor(this->drawColor);
    switch (style) {
    case 1: {
            int capW = Globals::Canvas->GetImage2DWidth(this->field_0x350);
            Globals::Canvas->DrawImage2D(this->field_0x350, x, y);
            this->drawBGPattern(this->field_0x354, capW + x, y, w - capW * 2, h);
            Globals::Canvas->DrawImage2D(this->field_0x350, w + x - capW, y, 1);
            if (text.size() == 0)
                break;
            int textX = Globals::layout->field_0x44;
            if (flags & 2)
                textX = w - textX - Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font, text);
            else if (flags & 4)
                textX = w / 2 - Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font, text) / 2;
            Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, text, textX + x, y + (Globals::layout->field_0x5c >> 1) + 1 - this->textBaselineAdjust,
                                        false);
            break;
    }
    case 0: {
            int capW = Globals::Canvas->GetImage2DWidth(this->field_0x348);
            Globals::Canvas->DrawImage2D(this->field_0x348, x, y);
            this->drawBGPattern(this->field_0x34c, capW + x, y, w - capW * 2, h);
            Globals::Canvas->DrawImage2D(this->field_0x348, w + x - capW, y, 1);
            if (text.size() == 0)
                break;
            int textX = Globals::layout->field_0x44;
            if (flags & 2)
                textX = w - textX - Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font, text);
            else if (flags & 4)
                textX = w / 2 - Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font, text) / 2;
            Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, text, textX + x, y + (Globals::layout->field_0x1c >> 1) + 1 - this->textBaselineAdjust,
                                        false);
            break;
    }
    case 2:
        this->drawBGPattern(this->bgPatternImage, x, y, w, h);
        break;
    case 3: {
            int capW = Globals::Canvas->GetImage2DWidth(this->field_0x358);
            Globals::Canvas->DrawImage2D(this->field_0x358, x, y);
            this->drawBGPattern(this->field_0x35c, capW + x, y, w - capW * 2, h);
            Globals::Canvas->DrawImage2D(this->field_0x358, w + x - capW, y, 1);
            break;
    }
    case 4: {
            int capW = Globals::Canvas->GetImage2DWidth(this->field_0x36c);
            Globals::Canvas->DrawImage2D(this->field_0x36c, x, y);
            this->drawBGPattern(this->field_0x370, capW + x, y, w - capW * 2, h);
            Globals::Canvas->DrawImage2D(this->field_0x36c, w + x - capW, y, 1);
            break;
    }
    case 5:
        this->drawBGBorder(this->field_0x380, this->field_0x384, x, y, w, h);
        break;
    case 6: {
            int capW = Globals::Canvas->GetImage2DWidth(this->field_0x388);
            Globals::Canvas->DrawImage2D(this->field_0x388, x, y);
            this->drawBGPattern(this->field_0x38c, capW + x, y, w - capW * 2, h);
            Globals::Canvas->DrawImage2D(this->field_0x388, w + x - capW, y, 1);
            if (text.size() == 0)
                break;
            int textX = Globals::layout->field_0x44;
            if (flags & 2)
                textX = w - textX - Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font, text);
            else if (flags & 4)
                textX = w / 2 - Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font, text) / 2;
            Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, text, textX + x, y + (h >> 1) + 1 - this->textBaselineAdjust,
                                        false);
            break;
    }
    case 7: {
        this->drawBGPattern(this->bgPatternImage, x, Globals::layout->windowTopInset + y, w,
                            h - Globals::layout->windowTopInset);
        int inset = -Globals::Canvas->GetImage2DHeight(this->field_0x394);
        this->drawBGBorder(this->field_0x390, this->field_0x394, x, Globals::layout->windowTopInset + y, w,
                           h - Globals::layout->windowTopInset, inset, inset);
        if (text.size() == 0)
            break;
        Globals::Canvas->SetColor(0xffffffff);
        Globals::Canvas->DrawImage2D(this->headerIconImage, x, y);
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, text, Globals::layout->buttonInsetX + x,
                                    y + Globals::layout->windowTopInset / 2 + 1 - this->textBaselineAdjust, false);
        break;
    }
    case 8:
        this->drawBGBorder(this->field_0x39c, this->field_0x3a0, x, y, w, h);
        break;
    case 9: {
            int capW = Globals::Canvas->GetImage2DWidth(this->field_0x360);
            Globals::Canvas->DrawImage2D(this->field_0x360, x, y);
            this->drawBGPattern(this->field_0x364, capW + x, y, w - capW * 2, h);
            Globals::Canvas->DrawImage2D(this->field_0x360, w + x - capW, y, 1);
            break;
    }
    case 10: {
            int capW = Globals::Canvas->GetImage2DWidth(this->field_0x368);
            Globals::Canvas->DrawImage2D(this->field_0x368, x, y);
            this->drawBGPattern(this->field_0x370, capW + x, y, w - capW * 2, h);
            Globals::Canvas->DrawImage2D(this->field_0x368, w + x - capW, y, 1);
            break;
    }
    }
    Globals::Canvas->SetColor(savedColor);
}






void Layout::drawWindow(String title, int x, int y, int w, int h, bool drawBG) {
    unsigned int saved = Globals::Canvas->GetColor();
    if (drawBG)
        this->drawBGPattern(this->bgPatternImage, x, Globals::layout->windowTopInset + y, w,
                            h - Globals::layout->windowTopInset);
    Globals::Canvas->SetColor(0x255d8dff);
    Globals::Canvas->DrawRectangle(x, Globals::layout->windowTopInset + y, w, h - Globals::layout->windowTopInset);
    Globals::Canvas->SetColor(this->drawColor);
    Globals::Canvas->DrawImage2D(this->headerIconImage, x, y);
    if (title.size() != 0 && title.Compare("") == 0) {
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, title, Globals::layout->buttonInsetX + x,
                                    y + Globals::layout->windowTopInset / 2 + 1 - this->textBaselineAdjust, false);
    }
    Globals::Canvas->SetColor(saved);
}








void Layout::drawTip() {
    if (this->tipLines != nullptr) {
        Globals::Canvas->SetColor(0xffffffff);
        Globals::layout->drawBox(5, (Globals::w >> 1) - (Globals::layout->field_0x78 >> 1), (Globals::h >> 1) + 13,
                                 Globals::layout->field_0x78, 100, String(""));
        Globals::Canvas->DrawImage2D(this->tipBoxImage, Globals::w >> 1, (Globals::h >> 1) + 63, 0x11, 0x44);
        Globals::globals->drawLines((unsigned int) (uintptr_t) Globals::font, this->tipLines, Globals::w >> 1,
                                    (Globals::h >> 1) + 63 - this->tipLines->size() * Globals::layout->field_0x4 / 2,
                                    true);
    }
}

void Layout::drawHeader(String title) {
    this->drawHeader(String(title, false), true);
}

void Layout::drawHelpWindow() {
    this->choiceWindow->draw();
}


void Layout::initHelpWindow(String text) {
    if (this->choiceWindow == nullptr)
        this->choiceWindow = new ChoiceWindow();
    (Globals::sound)->play(0x7e, 0, 0, 0);
    this->choiceWindow->set(*Globals::gameText->getText(0x187), text, false);
    this->helpPressedFlag = 0;
    this->choiceWindowOpen = 1;
}




void Layout::resetWindowDimensions() {
    this->windowX = 0;
    this->windowY = 0;
    this->windowWidth = Globals::w;
    this->windowHeight = Globals::h;
    this->helpButton->setPosition(Globals::w, 0, 0x12);
    this->backButton->setPosition(Globals::layout->buttonInsetX + this->windowX,
                                  this->windowY + this->windowHeight - this->footerButtonOffset, 0x21);
    this->secondaryButton->setPosition(Globals::layout->buttonInsetX + this->windowX,
                                       this->windowY + this->windowHeight - this->footerButtonOffset, 0x21);
    if (this->backButton != nullptr) {
        Globals::other_buttons_x[0] = (int) this->backButton->getPosition().x;
        Globals::other_buttons_y[0] = (int) this->backButton->getPosition().y;
    }
}

String Layout::tagString(String in) {
    return String("<c:") + String((int) 0xff8000ff) + String(">") + in + String("<c:>");
}



void Layout::reload() {
    this->bgPatternImage = -1;
    this->headerPatternImage = -1;
    this->headerIconImage = -1;
    this->footerImageLeft = -1;
    this->footerFillImage = -1;
    this->footerImageRight = -1;
    this->field_0x340 = -1;
    this->footerPatternImage = -1;
    this->field_0x348 = -1;
    this->field_0x34c = -1;
    this->scrollBarImage = -1;
    this->scrollBarFillImage = -1;
    this->field_0x37c = -1;
    this->tipBoxImage = -1;
    this->field_0x3a8 = -1;
    this->field_0x354 = -1;
    this->field_0x350 = -1;
    this->field_0x36c = -1;
    this->field_0x370 = -1;
    this->field_0x35c = -1;
    this->field_0x368 = -1;
    this->field_0x364 = -1;
    this->field_0x358 = -1;
    this->field_0x360 = -1;
    this->field_0x384 = -1;
    this->field_0x380 = -1;
    this->field_0x38c = -1;
    this->field_0x388 = -1;
    this->field_0x394 = -1;
    this->field_0x390 = -1;
    this->field_0x3a0 = -1;
    this->field_0x39c = -1;

    Globals::Canvas->Image2DCreate(static_cast<unsigned short>(0x503), reinterpret_cast<unsigned int &>(this->tipBoxImage));
    Globals::Canvas->Image2DCreate(static_cast<unsigned short>(0x47e), reinterpret_cast<unsigned int &>(this->bgPatternImage));
    Globals::Canvas->Image2DCreate(static_cast<unsigned short>(0x4ff),
                          reinterpret_cast<unsigned int &>(this->headerPatternImage));
    Globals::Canvas->Image2DCreate(static_cast<unsigned short>(0x500), reinterpret_cast<unsigned int &>(this->headerCapImage));
    Globals::Canvas->Image2DCreate(static_cast<unsigned short>(0x474), reinterpret_cast<unsigned int &>(this->headerIconImage));
    Globals::Canvas->Image2DCreate(static_cast<unsigned short>(0x502), reinterpret_cast<unsigned int &>(this->footerImageLeft));
    Globals::Canvas->Image2DCreate(static_cast<unsigned short>(0x506), reinterpret_cast<unsigned int &>(this->field_0x340));
    Globals::Canvas->Image2DCreate(static_cast<unsigned short>(0x501), reinterpret_cast<unsigned int &>(this->footerFillImage));
    Globals::Canvas->Image2DCreate(static_cast<unsigned short>(0x507),
                          reinterpret_cast<unsigned int &>(this->footerPatternImage));
    Globals::Canvas->Image2DCreate(static_cast<unsigned short>(0x4fe), reinterpret_cast<unsigned int &>(this->footerImageRight));
    Globals::Canvas->Image2DCreate(static_cast<unsigned short>(0x482), reinterpret_cast<unsigned int &>(this->field_0x348));
    Globals::Canvas->Image2DCreate(static_cast<unsigned short>(0x481), reinterpret_cast<unsigned int &>(this->field_0x34c));
    Globals::Canvas->Image2DCreate(static_cast<unsigned short>(0x486),
                          reinterpret_cast<unsigned int &>(this->scrollBarFillImage));
    Globals::Canvas->Image2DCreate(static_cast<unsigned short>(0x487), reinterpret_cast<unsigned int &>(this->scrollBarImage));
    Globals::Canvas->Image2DCreate(static_cast<unsigned short>(0x48b), reinterpret_cast<unsigned int &>(this->field_0x37c));
    Globals::Canvas->Image2DCreate(static_cast<unsigned short>(0x52d), reinterpret_cast<unsigned int &>(this->rewardIconImage));

    if (Globals::iPad != 0) {
        Globals::Canvas->Image2DCreate(static_cast<unsigned short>(0x6bb), reinterpret_cast<unsigned int &>(this->field_0x350));
        Globals::Canvas->Image2DCreate(static_cast<unsigned short>(0x6ba), reinterpret_cast<unsigned int &>(this->field_0x354));
        Globals::Canvas->Image2DCreate(static_cast<unsigned short>(0x6b9), reinterpret_cast<unsigned int &>(this->field_0x358));
        Globals::Canvas->Image2DCreate(static_cast<unsigned short>(0x6b8), reinterpret_cast<unsigned int &>(this->field_0x35c));
        Globals::Canvas->Image2DCreate(static_cast<unsigned short>(0x6b7), reinterpret_cast<unsigned int &>(this->field_0x36c));
        Globals::Canvas->Image2DCreate(static_cast<unsigned short>(0x6bc),
                                     reinterpret_cast<unsigned int &>(this->field_0x370));
    } else {
        Globals::Canvas->Image2DCreate(static_cast<unsigned short>(0x480), reinterpret_cast<unsigned int &>(this->field_0x350));
        Globals::Canvas->Image2DCreate(static_cast<unsigned short>(0x47f), reinterpret_cast<unsigned int &>(this->field_0x354));
        Globals::Canvas->Image2DCreate(static_cast<unsigned short>(0x479), reinterpret_cast<unsigned int &>(this->field_0x358));
        Globals::Canvas->Image2DCreate(static_cast<unsigned short>(0x478), reinterpret_cast<unsigned int &>(this->field_0x35c));
        Globals::Canvas->Image2DCreate(static_cast<unsigned short>(0x489), reinterpret_cast<unsigned int &>(this->field_0x36c));
        Globals::Canvas->Image2DCreate(static_cast<unsigned short>(0x488),
                                     reinterpret_cast<unsigned int &>(this->field_0x370));
    }

    Globals::Canvas->Image2DCreate(static_cast<unsigned short>(0x530), reinterpret_cast<unsigned int &>(this->field_0x360));
    Globals::Canvas->Image2DCreate(static_cast<unsigned short>(0x531), reinterpret_cast<unsigned int &>(this->field_0x364));
    Globals::Canvas->Image2DCreate(static_cast<unsigned short>(0x52f), reinterpret_cast<unsigned int &>(this->field_0x368));
    Globals::Canvas->Image2DCreate(static_cast<unsigned short>(0x47c), reinterpret_cast<unsigned int &>(this->field_0x384));
    Globals::Canvas->Image2DCreate(static_cast<unsigned short>(0x47d), reinterpret_cast<unsigned int &>(this->field_0x380));
    Globals::Canvas->Image2DCreate(static_cast<unsigned short>(0x47b), reinterpret_cast<unsigned int &>(this->field_0x388));
    Globals::Canvas->Image2DCreate(static_cast<unsigned short>(0x47a), reinterpret_cast<unsigned int &>(this->field_0x38c));
    Globals::Canvas->Image2DCreate(static_cast<unsigned short>(0x484), reinterpret_cast<unsigned int &>(this->field_0x390));
    Globals::Canvas->Image2DCreate(static_cast<unsigned short>(0x483), reinterpret_cast<unsigned int &>(this->field_0x394));
    Globals::Canvas->Image2DCreate(static_cast<unsigned short>(0x50c), reinterpret_cast<unsigned int &>(this->field_0x3a0));
    Globals::Canvas->Image2DCreate(static_cast<unsigned short>(0x50d), reinterpret_cast<unsigned int &>(this->field_0x39c));

    TouchButton *bBack = new TouchButton(
        *Globals::gameText->getText(0xaa), 2, this->buttonInsetX, Globals::h - 3, 0x21);
    this->backButton = bBack;
    this->backButtonWidth = bBack->getWidth();

    unsigned img535 = 0xffffffff;
    Globals::Canvas->Image2DCreate(0x535, img535);
    TouchButton *b2;
    if (img535 == 0xffffffff) {
        b2 = new TouchButton(*Globals::gameText->getText(0xab), 2, this->buttonInsetX,
                             Globals::h - this->footerButtonOffset, 0x21);
    } else {
        b2 = new TouchButton(img535, 2, this->buttonInsetX,
                             Globals::h - this->footerButtonOffset, 0x21);
    }
    this->secondaryButton = b2;

    unsigned img471 = 0xffffffff;
    Globals::Canvas->Image2DCreate(0x471, img471);
    TouchButton *bHelp =
        new TouchButton(img471, 1, Globals::w, 0, this->field_0x3c, 0x12, 0x04);
    this->helpButton = bHelp;

    int th = Globals::Canvas->GetTextHeight(
        static_cast<unsigned int>(reinterpret_cast<unsigned long>(Globals::font)));
    this->choiceWindowOpen = 0;
    this->choiceWindow = nullptr;
    this->tipLines = nullptr;
    this->textBaselineAdjust = th / 2 - 1;
    this->drawColor = -1;
    this->resetWindowDimensions();
    this->helpButtonEnabled = 0;
    this->rewardMessageTimer = 0;
    this->field_0x3d8 = 0;
    this->fading = 0;
    this->fadeOut = 0;
    this->field_0x3dc = 0;
    this->fadeColor = 0;
    this->fadeProgress = 0;
    this->rewardMessageActive = 0;
    this->rewardMessageFlag = 0;
    reinterpret_cast<int &>(reinterpret_cast<uint8_t *>(&this->fadeProgress)[5]) = 0;
    reinterpret_cast<int &>(reinterpret_cast<uint8_t *>(&this->fadeProgress)[1]) = 0;
}

void Layout::drawHeader() {
    this->drawHeader(String(""), false);
}

void Layout::drawFooterStation() {
    this->drawFooter(true, false);
}




void Layout::drawHeader(String title, bool transition) {
    Globals::Canvas->SetColor(this->drawColor);
    int capW = Globals::Canvas->GetImage2DWidth(this->headerCapImage);
    int capH = Globals::Canvas->GetImage2DHeight(this->headerCapImage);
    Globals::Canvas->DrawImage2D(this->headerCapImage, this->windowX, this->windowY);
    this->drawBGPattern(this->headerPatternImage, this->windowX + capW, this->windowY, this->windowWidth - 2 * capW,
                        capH);
    Globals::Canvas->DrawImage2D(this->headerCapImage, this->windowWidth + this->windowX, this->windowY, capW, capH,
                                 0x11, 0x12, 1);
    if (title.size() != 0) {
        Globals::Canvas->DrawImage2D(this->headerIconImage, this->windowX, this->windowY);
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, title,
                                    Globals::layout->field_0x44 + this->windowX + Globals::layout->buttonInsetX,
                                    this->headerTitleY + this->windowY, false);
    }
    this->helpButtonEnabled = transition;
    if (transition && this->choiceWindowOpen == 0)
        this->helpButton->draw();
}









void Layout::drawMissionRewardMessage(bool transition) {
    if (this->rewardMessageActive) {
        unsigned int savedColor = Globals::Canvas->GetColor();
        unsigned int savedDrawColor = this->drawColor;
        Globals::Canvas->SetColor(0xffffffff);
        float fade;
        if (this->rewardMessageTimer < 2000)
            fade = (float) this->rewardMessageTimer / 2000.0f;
        else if (this->rewardMessageTimer > 5000)
            fade = (float) (7000 - this->rewardMessageTimer) / 2000.0f;
        else
            fade = 1.0f;
        Globals::Canvas->SetColor((int) (fade * 255.0f) - 256);
        this->drawColor = Globals::Canvas->GetColor();
        int boxW = this->rewardBoxWidth;
        int boxH = this->rewardBoxHeight;
        int boxY = this->rewardBoxX;
        int textY = this->rewardBoxY;
        if (transition) {
            this->drawBox(2, (Globals::w >> 1) - (boxH >> 1), boxY, boxH, boxW, String(""));
            this->drawBox(8, (Globals::w >> 1) - (boxH >> 1), boxY, boxH, boxW, String(""));
        }
        Globals::Canvas->DrawImage2D(this->rewardIconImage, Globals::w >> 1, boxY, 0x11, 0x14);
        String text(*Globals::gameText->getText(this->rewardMessageFlag ? 3206 : 216), false);
        Globals::Canvas->DrawString(
            (unsigned int) (uintptr_t) Globals::font, text,
            (Globals::w >> 1) - (Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font, text) >> 1),
            boxY + textY, false);
        text = String("+ ") + this->formatCredits(this->rewardCredits);
        Globals::Canvas->DrawString(
            (unsigned int) (uintptr_t) Globals::font, text,
            (Globals::w >> 1) - (Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font, text) >> 1),
            this->rewardBoxY2 + boxY, false);
        Globals::Canvas->SetColor(savedColor);
        this->drawColor = savedDrawColor;
    }
}

void Layout::drawWindow(String title, int x, int y, int w, int h) {
    this->drawWindow(String(title, false), x, y, w, h, true);
}

void Layout::drawMask() {
    this->drawMask(0, 0, Globals::w, Globals::h);
}

void Layout::drawBox(int style, int x, int y, int w, int h, String text) {
    this->drawBox(style, x, y, w, h, text, 1);
}

void Layout::drawFooter(bool stationMode, bool showBack) {
    Globals::Canvas->SetColor(this->drawColor);
    int rightW = Globals::Canvas->GetImage2DWidth(this->footerImageRight);
    int leftW = Globals::Canvas->GetImage2DWidth(this->footerImageLeft);
    Globals::Canvas->DrawImage2D(this->footerImageLeft, this->windowX, this->windowY + this->windowHeight, 0x11, 0x21);
    Globals::Canvas->DrawImage2D(this->footerImageRight, this->windowX + leftW,
                                 this->windowY + this->windowHeight - Globals::layout->field_0x10_rightMargin);
    int edgeW = leftW + rightW;
    this->drawBGPattern(this->footerFillImage, this->windowX + leftW + rightW,
                        this->windowY + this->windowHeight - Globals::layout->field_0x10_rightMargin,
                        this->windowWidth - 2 * edgeW, Globals::layout->field_0x10_rightMargin);
    Globals::Canvas->DrawImage2D(this->footerImageRight, this->windowX - edgeW + this->windowWidth,
                                 this->windowY + this->windowHeight - Globals::layout->field_0x10_rightMargin, 1);
    Globals::Canvas->DrawImage2D(this->footerImageLeft, this->windowX - leftW + this->windowWidth,
                                 this->windowHeight + this->windowY - Globals::layout->field_0x10_rightMargin, 1);

    this->backButton->setVisible(!stationMode && showBack);
    if (stationMode)
        this->secondaryButton->draw();
    else if (showBack)
        this->backButton->draw();

    if (Globals::status->getShip()->getCurrentLoad() > Globals::status->getShip()->getMaxLoad())
        Globals::Canvas->SetColor(0xff2a00ff);
    String load = String(Globals::status->getShip()->getCurrentLoad()) + String(" / ") +
                  String(Globals::status->getShip()->getMaxLoad()) + String("t");
    Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, load,
                                this->windowX + this->windowWidth / 2 -
                                Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font, load) / 2,
                                this->windowY + this->windowHeight - this->footerTextInset, false);
    Globals::Canvas->SetColor(this->drawColor);

    String credits = this->formatCredits(Globals::status->getCredits());
    if (stationMode) {
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, credits,
                                    this->windowX + this->windowWidth - Globals::layout->field_0x74 -
                                    Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font, credits) / 2,
                                    this->windowY + this->windowHeight - this->footerTextInset, false);
    } else {
        Globals::Canvas->DrawString((unsigned int) (uintptr_t) Globals::font, credits,
                                    this->windowX + this->windowWidth - 10 -
                                    Globals::Canvas->GetTextWidth((unsigned int) (uintptr_t) Globals::font, credits),
                                    this->windowY + this->windowHeight - this->footerTextInset, false);
    }
}

void Layout::drawMask(int x, int y, int w, int h) {
    unsigned int saved = Globals::Canvas->GetColor();
    Globals::Canvas->SetColor(0x80);
    Globals::Canvas->FillRectangle(x, y, w, h);
    Globals::Canvas->SetColor(saved);
}

void Layout::drawWindow(String title) {
    this->drawWindow(title, false);
}

void Layout::drawWindow(String title, bool drawBG) {
    this->drawWindow(title, 0, 0, Globals::w, Globals::h - Globals::layout->windowTopInset, drawBG);
}
