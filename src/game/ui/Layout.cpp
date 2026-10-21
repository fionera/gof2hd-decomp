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

void TouchButton_footerAnim(...);

void Globals_drawLines(...);

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

int Layout::OnTouchMove(int x, int y) {
    if (this->helpButtonEnabled != 0)
        this->helpButton->OnTouchMove(x, y);
    if (this->choiceWindow != nullptr && this->choiceWindowOpen != 0) {
        this->choiceWindow->OnTouchMove(x, y);
        return 0;
    }
    if (this->backButton->isVisible() != 0)
        return static_cast<int>(this->backButton->OnTouchMove(x, y));
    return static_cast<int>(this->secondaryButton->OnTouchMove(x, y));
}

int Layout::OnTouchEnd(int x, int y) {
    if (this->choiceWindowOpen == 0 && this->helpButtonEnabled != 0)
        this->helpPressedFlag = this->helpButton->OnTouchEnd(x, y);
    if (this->choiceWindow != nullptr && this->choiceWindowOpen != 0)
        return this->choiceWindow->OnTouchEnd(x, y) == 0;
    if (this->backButton->isVisible() != 0)
        return static_cast<int>(this->backButton->OnTouchEnd(x, y));
    return static_cast<int>(this->secondaryButton->OnTouchEnd(x, y));
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



static char *g_cfgScale = nullptr;




static char *g_cfgC = nullptr;

static char *g_cfgD = nullptr;

Layout::Layout() {
    int hd = (Globals::iPadHD != 0);
    int wide = (Globals::n9 != 0);
    int scale = (*g_cfgScale != 0);
    int res = Globals::retinaDisplay;

    memset(&this->field_0x4, 0, 0x2d8 - 0x04);

    if (!hd) {
        this->field_0x238_blk16.a = 0x46;
        this->field_0x27c = 0x1c;
        this->field_0x280 = 0;
        this->scrollBarInset = 1;
        this->scrollBarHandle = 0;
        this->rewardBoxWidth = 0x8c;
        this->rewardBoxY = 0x4d;
        this->rewardBoxY2 = 0x84;
    }

    if (hd) {
        this->rewardBoxHeight = (res == 0) ? 0xc4 : 0x188;
        this->rewardBoxX = (res == 0) ? 0x32 : 100;
    } else {
        this->rewardBoxHeight = 0x160;
        this->rewardBoxX = 0x4b;
    }

    int v288, v28c, v3fc_, v200, v1f0, v1f4, v1f8, v2bc;
    if (!hd) {
        int wb = (res != 0) ? 4 : 2;
        if (Globals::iPadLarge == 0) wb = 2;
        int below = (res == 0) ? 0xfffffff3 : 0xffffffe6;
        if (!wide) below = 0xffffffe6;
        int o1f0 = (res == 0) ? 0x13 : 0x26;
        if (!wide) o1f0 = 0x22;
        int o3fc = (res == 0) ? 3 : 6;
        if (!wide) o3fc = 5;
        int o1f8 = (Globals::iPadLarge == 0) ? 2 : 4;
        if (res == 0) o1f8 = 2;
        v1f4 = (Globals::iPadLarge == 0) ? (res ? 0x20 : 0x10) : wb;
        v2bc = wide ? 2 : 3;
        v200 = below;
        v1f0 = o1f0;
        v3fc_ = o3fc;
        v1f8 = o1f8;
        v288 = wide ? 0 : 0x14;
        v28c = wide ? 0 : 0xffffffed;
    } else {
        v200 = 0xffffffee;
        v1f4 = 0x16;
        v1f0 = 0x1a;
        v3fc_ = 4;
        v1f8 = 2;
        v2bc = 2;
        v288 = 0x14;
        v28c = 0xffffffed;
    }
    this->field_0x2bc = v2bc;
    this->footerButtonOffset = v3fc_;
    this->field_0x200 = v200;
    this->field_0x1f0 = v1f0;
    this->field_0x1f4 = v1f4;
    this->field_0x1f8 = v1f8;
    this->field_0x284_sliderSlot5Enabled = 1;
    this->field_0x285 = 1;
    this->field_0x286 = 1;
    this->field_0x288 = v288;
    this->field_0x28c = v28c;
    this->field_0x290 = wide ? (res == 0 ? 3 : 7) : 4;

    int cwTitle;
    if (!hd) {
        int cw2f4;
        if (Globals::n9 != 0) {
            cw2f4 = 0xa0;
        } else if (Globals::iPad != 0) {
            cw2f4 = 0x50;
        } else {
            cw2f4 = wide ? 0x3c : 0x2d;
            if (res == 0) cw2f4 = 0x1e;
        }
        this->field_0x2f4 = cw2f4;
        this->field_0x2f8 = 0;
        this->field_0x2fc = (Globals::n9 != 0) ? 0x48 : 0x24;
        this->field_0x300 = (Globals::n9 != 0) ? 400 : 200;
        this->field_0x304 = (Globals::n9 != 0) ? 700 : 0x15e;
        cwTitle = (Globals::n9 != 0) ? 500 : 0xfa;
    } else {
        this->field_0x2f4 = 0x70;
        this->field_0x2f8 = 0;
        this->field_0x2fc = 0x32;
        this->field_0x300 = 0x119;
        this->field_0x304 = 0x1ec;
        cwTitle = 0x15f;
    }
    this->choiceWindowTitleHeight = cwTitle;

    int b318;
    if (!hd) {
        if (*g_cfgC == 0) {
            if (wide) {
                this->field_0x30c = 7;
                this->field_0x310 = (res == 0) ? 0xfffffffb : 0xfffffff7;
                this->field_0x314 = (res == 0) ? 0xfffffffb : 0xfffffff8;
            } else {
                this->field_0x30c = 4;
                this->field_0x310 = 0xfffffff8;
                this->field_0x314 = 0xfffffff8;
            }
            b318 = 0;
        } else {
            this->field_0x30c = 3;
            this->field_0x310 = 0xfffffff4;
            this->field_0x314 = 0xfffffffb;
            b318 = 4;
        }
    } else {
        this->field_0x30c = 6;
        this->field_0x310 = 0xffffffe8;
        this->field_0x314 = 0xfffffff6;
        b318 = 8;
    }
    this->field_0x318 = b318;

    if (!hd) {
        if (wide) {
            this->helpWindowX = (*g_cfgD != 0) ? 0x76 : 0xec;
            this->helpWindowY = 0;
        } else {
            this->helpWindowX = 0x1d8;
            this->helpWindowY = 0x120;
        }
    } else {
        this->helpWindowX = 0x14b;
        this->helpWindowY = 0xca;
    }
}



void Layout::drawBGPattern(unsigned img, int x, int y, int w, int h) {
    Globals::Canvas->SetColor(this->drawColor);
    int iw = Globals::Canvas->GetImage2DWidth(img);
    int cols = w / iw;
    int fullW = cols * iw;
    int ih = Globals::Canvas->GetImage2DHeight(img);
    int rows = h / ih;
    int fullH = rows * ih;

    for (int r = 0; r < rows; r++) {
        int px = x;
        for (int c = 0; c < cols; c++) {
            Globals::Canvas->DrawImage2D(img, px, r * ih + y);
            px += iw;
        }
    }

    int remH = h - fullH;
    int remW = w - fullW;
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

static const char g_fnEmpty[] = "";

static const char g_fnSepA[] = "";

static const char g_fnSepB[] = "";

static const char g_fnOverflow[] = "";

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
    int edgeX = cornerW + pad + x;
    for (int i = 0; i < cols; i++) {
        Globals::Canvas->DrawImage2D(edge, edgeX, topY);
        Globals::Canvas->DrawImage2D(edge, edgeX, bottomEdgeY, 2);
        edgeX += edgeW;
    }
    int remW = spanW - cols * edgeW;
    if (remW > 0) {
        Globals::Canvas->DrawRegion2D(edge, 0, 0, remW, edgeH, 0.0f, 0, 0, cols * edgeW + (pad + x + cornerW), topY);
        Globals::Canvas->DrawRegion2D(edge, 0, 0, remW, edgeH, 3.14159274f, 0, 0, spanW + (pad + x + cornerW),
                                      y + h - pad);
    }

    // Left and right edges (the edge image rotated by -90/+90 degrees).
    int spanH = h - cornerH * 2 - inset * 2;
    int rows = spanH / edgeW;
    int rightEdgeX = w + x + pad;
    int edgeTop = pad + y + cornerH;
    int edgeY = cornerH + pad + y;
    for (int i = 0; i < rows; i++) {
        Globals::Canvas->DrawRegion2D(edge, 0, 0, edgeW, edgeH, -1.57079637f, 0, 0, pad + x, edgeW + edgeY);
        Globals::Canvas->DrawRegion2D(edge, 0, 0, edgeW, edgeH, 1.57079637f, 0, 0, rightEdgeX, edgeY);
        edgeY += rows * edgeW;
    }
    int remH = spanH - rows * edgeW;
    if (remH > 0) {
        Globals::Canvas->DrawRegion2D(edge, 0, 0, remH, edgeH, -1.57079637f, 0, 0, pad + x, spanH + edgeTop);
        Globals::Canvas->DrawRegion2D(edge, 0, 0, remH, edgeH, 1.57079637f, 0, 0, rightEdgeX - inset * 2,
                                      edgeTop + rows * edgeW);
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
    PaintCanvas *pc = Globals::Canvas;
    unsigned saved = pc->GetColor();
    Globals::Canvas->SetColor(this->drawColor);

    switch (style) {
        case 0: {
            int iw = Globals::Canvas->GetImage2DWidth(this->field_0x348);
            pc->DrawImage2D(this->field_0x348, x, y);
            this->drawBGPattern(this->field_0x34c, iw + x, y, w + iw * -2, h);
            pc->DrawImage2D(this->field_0x348, (w + x) - iw, y, (unsigned char) (0x01));
            if (text.size() == 0) break;
            int *mt = (int *) Globals::layout;
            int tx = mt[0x44 / 4];
            if ((flags & 2) == 0) {
                if ((int) (flags << 0x1d) < 0) {
                    int tw = pc->GetTextWidth((unsigned) (unsigned long) ((unsigned int) (unsigned long) Globals::font), text);
                    tx = w / 2 - tw / 2;
                }
            } else {
                int tw = pc->GetTextWidth((unsigned) (unsigned long) ((unsigned int) (unsigned long) Globals::font), text);
                tx = (w - tx) - tw;
            }
            int ty = (y + (mt[0x1c / 4] >> 1) + 1) - this->textBaselineAdjust;
            pc->DrawString((unsigned) (unsigned long) ((unsigned int) (unsigned long) Globals::font), text, tx + x, ty, false);
            break;
        }
        case 1: {
            int iw = Globals::Canvas->GetImage2DWidth(this->field_0x350);
            pc->DrawImage2D(this->field_0x350, x, y);
            this->drawBGPattern(this->field_0x354, iw + x, y, w + iw * -2, h);
            pc->DrawImage2D(this->field_0x350, (w + x) - iw, y, (unsigned char) (0x01));
            if (text.size() == 0) break;
            int *mt = (int *) Globals::layout;
            int tx = mt[0x44 / 4];
            if ((flags & 2) == 0) {
                if ((int) (flags << 0x1d) < 0) {
                    int tw = pc->GetTextWidth((unsigned) (unsigned long) ((unsigned int) (unsigned long) Globals::font), text);
                    tx = w / 2 - tw / 2;
                }
            } else {
                int tw = pc->GetTextWidth((unsigned) (unsigned long) ((unsigned int) (unsigned long) Globals::font), text);
                tx = (w - tx) - tw;
            }
            int ty = (y + (mt[0x5c / 4] >> 1) + 1) - this->textBaselineAdjust;
            pc->DrawString((unsigned) (unsigned long) ((unsigned int) (unsigned long) Globals::font), text, tx + x, ty, false);
            break;
        }
        case 2:
            this->drawBGPattern(this->bgPatternImage, x, y, w, h);
            break;
        case 3: {
            int iw = Globals::Canvas->GetImage2DWidth(this->field_0x358);
            pc->DrawImage2D(this->field_0x358, x, y);
            this->drawBGPattern(this->field_0x35c, iw + x, y, w + iw * -2, h);
            pc->DrawImage2D(this->field_0x358, (w + x) - iw, y, (unsigned char) (0x01));
            break;
        }
        case 4: {
            int iw = Globals::Canvas->GetImage2DWidth(this->field_0x36c);
            pc->DrawImage2D(this->field_0x36c, x, y);
            this->drawBGPattern(this->field_0x370, iw + x, y, w + iw * -2, h);
            pc->DrawImage2D(this->field_0x36c, (w + x) - iw, y, (unsigned char) (0x01));
            break;
        }
        case 5: {
            int ch = Globals::Canvas->GetImage2DHeight(this->field_0x380);
            this->drawBGBorder(this->field_0x380, this->field_0x384, x, y, w, ch, 0, 0);
            break;
        }
        case 6: {
            int iw = Globals::Canvas->GetImage2DWidth(this->field_0x388);
            pc->DrawImage2D(this->field_0x388, x, y);
            this->drawBGPattern(this->field_0x38c, iw + x, y, w + iw * -2, h);
            pc->DrawImage2D(this->field_0x388, (w + x) - iw, y, (unsigned char) (0x01));
            if (text.size() == 0) break;
            int *mt = (int *) Globals::layout;
            int tx = mt[0x44 / 4];
            if ((flags & 2) == 0) {
                if ((int) (flags << 0x1d) < 0) {
                    int tw = pc->GetTextWidth((unsigned) (unsigned long) ((unsigned int) (unsigned long) Globals::font), text);
                    tx = w / 2 - tw / 2;
                }
            } else {
                int tw = pc->GetTextWidth((unsigned) (unsigned long) ((unsigned int) (unsigned long) Globals::font), text);
                tx = (w - tx) - tw;
            }
            int ty = (y + (h >> 1) + 1) - this->textBaselineAdjust;
            pc->DrawString((unsigned) (unsigned long) ((unsigned int) (unsigned long) Globals::font), text, tx + x, ty, false);
            break;
        }
        case 7: {
            int *mt = (int *) Globals::layout;
            int hdr = mt[8 / 4];
            this->drawBGPattern(this->bgPatternImage, x, hdr + y, w, h - hdr);
            int ih = Globals::Canvas->GetImage2DHeight(this->field_0x394);
            this->drawBGBorder(this->field_0x390, this->field_0x394, x, hdr + y, w, h - hdr, -ih, -ih);
            if (text.size() == 0) break;
            Globals::Canvas->SetColor(0xffffffff);
            pc->DrawImage2D(this->headerIconImage, x, y);
            int ty = (y + (mt[8 / 4] / 2) + 1) - this->textBaselineAdjust;
            pc->DrawString((unsigned) (unsigned long) ((unsigned int) (unsigned long) Globals::font), text,
                           mt[0x28 / 4] + x, ty, false);
            break;
        }
        case 8: {
            int ch = Globals::Canvas->GetImage2DHeight(this->field_0x39c);
            this->drawBGBorder(this->field_0x39c, this->field_0x3a0, x, y, w, ch, 0, 0);
            break;
        }
        case 9: {
            int iw = Globals::Canvas->GetImage2DWidth(this->field_0x360);
            pc->DrawImage2D(this->field_0x360, x, y);
            this->drawBGPattern(this->field_0x364, iw + x, y, w + iw * -2, h);
            pc->DrawImage2D(this->field_0x360, (w + x) - iw, y, (unsigned char) (0x01));
            break;
        }
        case 10: {
            int iw = Globals::Canvas->GetImage2DWidth(this->field_0x368);
            pc->DrawImage2D(this->field_0x368, x, y);
            this->drawBGPattern(this->field_0x370, iw + x, y, w + iw * -2, h);
            pc->DrawImage2D(this->field_0x368, (w + x) - iw, y, (unsigned char) (0x01));
            break;
        }
        default:
            break;
    }

    pc->SetColor(saved);
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
    this->field_0x3a8 = -1;

    memset(&this->footerImageLeft, 0xff, 0x70);

    PaintCanvas *canvas = Globals::Canvas;
    canvas->Image2DCreate(static_cast<unsigned short>(0x503), reinterpret_cast<unsigned int &>(this->tipBoxImage));
    canvas->Image2DCreate(static_cast<unsigned short>(0x47e), reinterpret_cast<unsigned int &>(this->bgPatternImage));
    canvas->Image2DCreate(static_cast<unsigned short>(0x4ff),
                          reinterpret_cast<unsigned int &>(this->headerPatternImage));
    canvas->Image2DCreate(static_cast<unsigned short>(0x500), reinterpret_cast<unsigned int &>(this->headerCapImage));
    canvas->Image2DCreate(static_cast<unsigned short>(0x474), reinterpret_cast<unsigned int &>(this->headerIconImage));
    canvas->Image2DCreate(static_cast<unsigned short>(0x502), reinterpret_cast<unsigned int &>(this->footerImageLeft));
    canvas->Image2DCreate(static_cast<unsigned short>(0x506), reinterpret_cast<unsigned int &>(this->field_0x340));
    canvas->Image2DCreate(static_cast<unsigned short>(0x501), reinterpret_cast<unsigned int &>(this->footerFillImage));
    canvas->Image2DCreate(static_cast<unsigned short>(0x507),
                          reinterpret_cast<unsigned int &>(this->footerPatternImage));
    canvas->Image2DCreate(static_cast<unsigned short>(0x4fe), reinterpret_cast<unsigned int &>(this->footerImageRight));
    canvas->Image2DCreate(static_cast<unsigned short>(0x482), reinterpret_cast<unsigned int &>(this->field_0x348));
    canvas->Image2DCreate(static_cast<unsigned short>(0x481), reinterpret_cast<unsigned int &>(this->field_0x34c));
    canvas->Image2DCreate(static_cast<unsigned short>(0x486),
                          reinterpret_cast<unsigned int &>(this->scrollBarFillImage));
    canvas->Image2DCreate(static_cast<unsigned short>(0x487), reinterpret_cast<unsigned int &>(this->scrollBarImage));
    canvas->Image2DCreate(static_cast<unsigned short>(0x48b), reinterpret_cast<unsigned int &>(this->field_0x37c));
    canvas->Image2DCreate(static_cast<unsigned short>(0x52d), reinterpret_cast<unsigned int &>(this->rewardIconImage));

    if (Globals::iPad == 0) {
        canvas->Image2DCreate(static_cast<unsigned short>(0x480), reinterpret_cast<unsigned int &>(this->field_0x350));
        canvas->Image2DCreate(static_cast<unsigned short>(0x47f), reinterpret_cast<unsigned int &>(this->field_0x354));
        canvas->Image2DCreate(static_cast<unsigned short>(0x479), reinterpret_cast<unsigned int &>(this->field_0x358));
        canvas->Image2DCreate(static_cast<unsigned short>(0x478), reinterpret_cast<unsigned int &>(this->field_0x35c));
        canvas->Image2DCreate(static_cast<unsigned short>(0x489), reinterpret_cast<unsigned int &>(this->field_0x36c));
        (Globals::Canvas)->Image2DCreate(static_cast<unsigned short>(0x488),
                                     reinterpret_cast<unsigned int &>(this->field_0x370));
    } else {
        canvas->Image2DCreate(static_cast<unsigned short>(0x6bb), reinterpret_cast<unsigned int &>(this->field_0x350));
        canvas->Image2DCreate(static_cast<unsigned short>(0x6ba), reinterpret_cast<unsigned int &>(this->field_0x354));
        canvas->Image2DCreate(static_cast<unsigned short>(0x6b9), reinterpret_cast<unsigned int &>(this->field_0x358));
        canvas->Image2DCreate(static_cast<unsigned short>(0x6b8), reinterpret_cast<unsigned int &>(this->field_0x35c));
        canvas->Image2DCreate(static_cast<unsigned short>(0x6b7), reinterpret_cast<unsigned int &>(this->field_0x36c));
        (Globals::Canvas)->Image2DCreate(static_cast<unsigned short>(0x6bc),
                                     reinterpret_cast<unsigned int &>(this->field_0x370));
    }

    canvas->Image2DCreate(static_cast<unsigned short>(0x530), reinterpret_cast<unsigned int &>(this->field_0x360));
    canvas->Image2DCreate(static_cast<unsigned short>(0x531), reinterpret_cast<unsigned int &>(this->field_0x364));
    canvas->Image2DCreate(static_cast<unsigned short>(0x52f), reinterpret_cast<unsigned int &>(this->field_0x368));
    canvas->Image2DCreate(static_cast<unsigned short>(0x47c), reinterpret_cast<unsigned int &>(this->field_0x384));
    canvas->Image2DCreate(static_cast<unsigned short>(0x47d), reinterpret_cast<unsigned int &>(this->field_0x380));
    canvas->Image2DCreate(static_cast<unsigned short>(0x47b), reinterpret_cast<unsigned int &>(this->field_0x388));
    canvas->Image2DCreate(static_cast<unsigned short>(0x47a), reinterpret_cast<unsigned int &>(this->field_0x38c));
    canvas->Image2DCreate(static_cast<unsigned short>(0x484), reinterpret_cast<unsigned int &>(this->field_0x390));
    canvas->Image2DCreate(static_cast<unsigned short>(0x483), reinterpret_cast<unsigned int &>(this->field_0x394));
    canvas->Image2DCreate(static_cast<unsigned short>(0x50c), reinterpret_cast<unsigned int &>(this->field_0x3a0));
    canvas->Image2DCreate(static_cast<unsigned short>(0x50d), reinterpret_cast<unsigned int &>(this->field_0x39c));

    TouchButton *bBack = new TouchButton(
        *Globals::gameText->getText(0xaa), 2, this->buttonInsetX, Globals::h - 3, 0x21);
    this->backButton = bBack;
    this->backButtonWidth = bBack->getWidth();

    unsigned img535 = 0xffffffff;
    (Globals::Canvas)->Image2DCreate(0x535, img535);
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
    (Globals::Canvas)->Image2DCreate(0x471, img471);
    TouchButton *bHelp =
        new TouchButton(img471, 1, Globals::w, 0, this->field_0x3c, 0x12, 0x04);
    this->helpButton = bHelp;

    int th = (Globals::Canvas)->GetTextHeight(
        static_cast<unsigned int>(reinterpret_cast<unsigned long>(Globals::font)));
    this->choiceWindowOpen = 0;
    this->choiceWindow = nullptr;
    this->tipLines = nullptr;
    this->textBaselineAdjust = th / 2 - 1;
    this->drawColor = -1;
    this->resetWindowDimensions();
    this->rewardMessageTimer = 0;
    this->helpButtonEnabled = 0;
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



static const char g_mrLit0[] = "";

static const char g_mrLit1[] = "";


static int *g_mrTextId = nullptr;


static const char g_mrLit2[] = "";

void Layout::drawMissionRewardMessage(bool transition) {
    if (this->rewardMessageActive != 0) {
        PaintCanvas *pc = Globals::Canvas;
        unsigned saved = pc->GetColor();
        unsigned origColor = this->drawColor;
        Globals::Canvas->SetColor(0xffffffff);

        int t = this->rewardMessageTimer;
        float a;
        if (t < 2000) {
            a = (float) t / 6000.0f;
        } else if (t > 5000) {
            a = (float) (7000 - t) / 6000.0f;
        } else {
            a = 1.0f;
        }
        unsigned col = (unsigned) ((int) (a * 256.0f)) - 0x100;
        Globals::Canvas->SetColor(col);

        unsigned newColor = pc->GetColor();
        int boxW = this->rewardBoxWidth;
        int boxH = this->rewardBoxHeight;
        int boxX = this->rewardBoxX;
        int boxY = this->rewardBoxY;
        this->drawColor = newColor;

        if (transition != 0) {
            int sw = Globals::w;
            String s0(g_mrLit0);
            this->drawBox(2, (sw >> 1) - (boxH >> 1), boxX, boxH, boxW, s0, 0u);

            sw = Globals::w;
            String s1(g_mrLit1);
            this->drawBox(8, (sw >> 1) - (boxH >> 1), boxX, boxH, boxW, s1, 0u);
        }

        int sh = Globals::w;
        pc->DrawImage2D(this->rewardIconImage, sh >> 1, (char) boxX, (unsigned char) (0x11));

        String *txt = Globals::gameText->getText(*g_mrTextId);
        String line(*txt);

        sh = Globals::w;
        unsigned font = (unsigned int) (unsigned long) Globals::font;
        int tw = pc->GetTextWidth((unsigned) (unsigned long) (font), line);
        pc->DrawString((unsigned) (unsigned long) (font), line, (sh >> 1) - (tw >> 1), boxX + boxY, false);

        String suffix(g_mrLit2);
        String credits = this->formatCredits(this->rewardCredits);
        line = suffix + credits;

        sh = Globals::w;
        tw = pc->GetTextWidth((unsigned) (unsigned long) (font), line);
        pc->DrawString((unsigned) (unsigned long) (font), line, (sh >> 1) - (tw >> 1),
                       this->rewardBoxY2 + boxX, false);
        Globals::Canvas->SetColor(saved);
        this->drawColor = origColor;
    }
}

void Layout::drawWindow(String title, int x, int y, int w, int h) {
    this->drawWindow(String(title, false), x, y, w, h, true);
}

void Layout::drawMask() {
    this->drawMask(0, 0, Globals::w, Globals::h);
}

void Layout::drawBGBorder(unsigned corner, unsigned edge, int x, int y, int w, int h) {
    this->drawBGBorder(corner, edge, x, y, w, h, 0, 0);
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
