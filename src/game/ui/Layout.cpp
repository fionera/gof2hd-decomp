#include "game/ui/Layout.h"
#include "game/ship/Ship.h"
#include "engine/core/AERandom.h"
#include "game/ui/ChoiceWindow.h"
#include "engine/audio/FModSound.h"
#include "game/mission/Status.h"
#include "engine/core/GameText.h"
#include "game/core/String.h"
#include "game/ui/TouchButton.h"
#include "engine/render/PaintCanvas.h"
#include "platform/libc.h"

// Global GameText singleton holder; receiver for getText(key).

// Cross-class engine helpers whose call sites pass mismatched argument lists;
// declared once with an unspecified-argument signature so every site type-checks
// while staying link-compatible with the real engine entry points.
extern "C" {
void    TouchButton_footerAnim(...);
void    TouchButton_getPosition(...);
void    TouchButton_ctorStr(...);
void    TouchButton_ctorImg(...);
void    TouchButton_ctorImg2(...);
unsigned short GameText_getLanguage(...);
void    Globals_drawLines(...);
}

uint8_t Layout::isFading() {
    return this->fading;
}

__attribute__((visibility("hidden"))) extern int **gTouchButtonHolder;

int Layout::getHelpButtonOffset() {
    int w = this->helpButton->getWidth();
    return w - (*gTouchButtonHolder)[0x38 / 4];
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

__attribute__((visibility("hidden"))) extern PaintCanvas **gPaintCanvasHolder;

int Layout::getFooterTransitionWidth() {
    PaintCanvas **holder = gPaintCanvasHolder;
    int w1 = (*holder)->GetImage2DWidth(this->footerImageRight);
    int w2 = (*holder)->GetImage2DWidth(this->footerImageLeft);
    return w2 + w1;
}

int Layout::OnTouchBegin(int x, int y) {
    if (this->helpButtonEnabled != 0)
        this->helpButton->OnTouchBegin(x, y);
    if (this->choiceWindow != nullptr && this->choiceWindowOpen != 0) {
        this->choiceWindow->OnTouchBegin(x, y);
        return 0;
    }
    TouchButton *btn = this->backButton->isVisible() == 0
                           ? this->secondaryButton
                           : this->backButton;
    return this->dispatchTouchBegin(btn, x, y);
}

float Sinf(float);   // AbyssEngine::AEMath::Sinf

// Pulse value driven by elapsed playing time; sign of the result follows the
// first sample.
float Layout::getPulseValue(float speed) {
    Status *status = gStatus;
    float a = Sinf((float)status->getPlayingTime() * speed);
    float b = Sinf((float)status->getPlayingTime() * speed);
    return a > 0.0f ? b : -b;
}

void Layout::setDrawColor(int color) {
    this->drawColor = color;
}

__attribute__((visibility("hidden"))) extern int *gW;
__attribute__((visibility("hidden"))) extern int *gH;

__attribute__((visibility("hidden"))) extern int *gA;
__attribute__((visibility("hidden"))) extern int *gB;
void Layout::drawBG() {
    int p4 = *gA;
    int p5 = *gB;
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

void Layout::drawFooterNoBackButton() {
    this->drawFooterImpl(0, 0);
}

void Layout::drawFooter() {
    this->drawFooterImpl(0, 1);
}

int Layout::OnTouchMove(int x, int y) {
    if (this->helpButtonEnabled != 0)
        this->helpButton->OnTouchMove(x, y);
    if (this->choiceWindow != nullptr && this->choiceWindowOpen != 0) {
        this->choiceWindow->OnTouchMove(x, y);
        return 0;
    }
    TouchButton *btn = this->backButton->isVisible() == 0
                           ? this->secondaryButton
                           : this->backButton;
    return this->dispatchTouchMove(btn, x, y);
}

int Layout::OnTouchEnd(int x, int y) {
    if (this->choiceWindowOpen == 0 && this->helpButtonEnabled != 0)
        this->helpPressedFlag = this->helpButton->OnTouchEnd(x, y);
    if (this->choiceWindow != nullptr && this->choiceWindowOpen != 0)
        return this->choiceWindow->OnTouchEnd(x, y) == 0;
    TouchButton *btn = this->backButton->isVisible() == 0
                           ? this->secondaryButton
                           : this->backButton;
    return this->dispatchTouchEnd(btn, x, y);
}

void Layout::startFade(bool fadeOut, int color, int duration) {
    this->fadeOut = fadeOut;
    this->fading = 1;
    this->fadeColor = color & 0xffffff00u;
    this->fadeProgress = 0;
    this->fadeDuration = duration;
}

// Hidden globals from initTip:
//   g_tipColor  : color value
//   g_tipTextId : text-table id
//   g_tipCanvas : canvas
//   g_tipMetric : object whose [0x78]/[0x4c] give the wrap width
__attribute__((visibility("hidden"))) extern unsigned *g_tipColor;
__attribute__((visibility("hidden"))) extern int **g_tipTextId;
__attribute__((visibility("hidden"))) extern PaintCanvas ***g_tipCanvas;
__attribute__((visibility("hidden"))) extern int **g_tipMetric;

// Rebuild the random "tip" string line array.
void Layout::initTip() {
    if (this->tipLines != nullptr) {
        for (String *line : *this->tipLines) delete line;
        this->tipLines->clear();
        delete this->tipLines;
        this->tipLines = nullptr;
    }
    Array<String*> *arr = new Array<String*>();

    unsigned color = *g_tipColor;
    int textId = **g_tipTextId;
    PaintCanvas *canvas = **g_tipCanvas;
    gRandom->nextInt();
    String *str = gGameText->getText(textId);

    this->tipLines = arr;
    int *m = *g_tipMetric;
    int width = m[0x78 / 4] + m[0x4c / 4] * -2;
    canvas->GetLineArray(color, str, width, (char *)this->tipLines);
}

// Hidden globals from drawEmptyFooter.
__attribute__((visibility("hidden"))) extern unsigned *g_efColor;   // [0]=color
__attribute__((visibility("hidden"))) extern int *g_efScreenH;      // [0]=screen height
__attribute__((visibility("hidden"))) extern int *g_efScreenW;      // [0]=screen width
__attribute__((visibility("hidden"))) extern int **g_efMetric;      // [0][0x10]=footer height

void Layout::drawEmptyFooter(bool showBack) {
    PaintCanvas *pc = gCanvas;
    unsigned color = *g_efColor;
    pc->SetColor(color);
    int w = pc->GetImage2DWidth(color);
    int screenH = *g_efScreenH;
    pc->DrawImage2D(this->footerImageLeft, 0, screenH, (unsigned char)(0x11));

    int screenW = *g_efScreenW;
    int footerH = (*g_efMetric)[0x10 / 4];
    this->drawBGPattern(this->footerPatternImage, w, screenH - footerH, screenW - w * 2, footerH);
    pc->DrawImage2D(this->footerImageLeft,
                    screenW - w, screenH - (*g_efMetric)[0x10 / 4], (unsigned char)(0x01));
    if (showBack == 0) return;
    this->backButton->setVisible(1);
    unsigned char sp[8] __attribute__((aligned(4)));
    TouchButton_footerAnim(this->backButton, 1, footerH, sp);
}

// Config-driven field initialiser. Layout geometry is chosen from a handful of
// display-config flags read through global bytes:
//   hd    : high-detail master flag (compact fixed-constant path when set)
//   wide  : aspect/scale flag
//   scale : secondary scale flag
//   res   : resolution selector (0 => half-size variants)
// The header geometry block (0x04..0x2d8) is bulk-seeded by initConstBlock();
// the readable scalar tail writes below reproduce the rest.

// Config-flag source globals (each points to a single byte).
__attribute__((visibility("hidden"))) extern char *g_cfgHd;
__attribute__((visibility("hidden"))) extern char *g_cfgWide;
__attribute__((visibility("hidden"))) extern char *g_cfgScale;
__attribute__((visibility("hidden"))) extern unsigned char *g_resByte;
__attribute__((visibility("hidden"))) extern char *g_cfgA;
__attribute__((visibility("hidden"))) extern char *g_cfgB;
__attribute__((visibility("hidden"))) extern char *g_cfgC;
__attribute__((visibility("hidden"))) extern char *g_cfgD;

Layout::Layout() {
    int hd    = (*g_cfgHd != 0);
    int wide  = (*g_cfgWide != 0);
    int scale = (*g_cfgScale != 0);
    int res   = *g_resByte;

    // Header / geometry constants.
    this->initConstBlock(hd, wide, scale, res);

    if (!hd) {
        this->field_0x238 = 0x46;
        this->field_0x27c = 0x1c;
        this->field_0x280 = 0;
        this->scrollBarInset = 1;
        this->scrollBarHandle = 0;
        this->rewardBoxWidth = 0x8c;
        this->rewardBoxY = 0x4d;
        this->rewardBoxY2 = 0x84;
    }

    // Reward-box dimensions.
    if (hd) {
        this->rewardBoxHeight = (res == 0) ? 0xc4 : 0x188;
        this->rewardBoxX = (res == 0) ? 0x32 : 100;
    } else {
        this->rewardBoxHeight = 0x160;
        this->rewardBoxX = 0x4b;
    }

    // Footer/back-button geometry group.
    int v288, v28c, v3fc_, v200, v1f0, v1f4, v1f8, v2bc;
    if (!hd) {
        int wb = (res != 0) ? 4 : 2;
        if (*g_cfgA == 0) wb = 2;
        int below = (res == 0) ? 0xfffffff3 : 0xffffffe6;
        if (!wide) below = 0xffffffe6;
        int o1f0 = (res == 0) ? 0x13 : 0x26;
        if (!wide) o1f0 = 0x22;
        int o3fc = (res == 0) ? 3 : 6;
        if (!wide) o3fc = 5;
        int o1f8 = (*g_cfgA == 0) ? 2 : 4;
        if (res == 0) o1f8 = 2;
        v1f4 = (*g_cfgA == 0) ? (res ? 0x20 : 0x10) : wb;
        v2bc = wide ? 2 : 3;
        v200 = below; v1f0 = o1f0; v3fc_ = o3fc; v1f8 = o1f8;
        v288 = wide ? 0 : 0x14;
        v28c = wide ? 0 : 0xffffffed;
    } else {
        v200 = 0xffffffee; v1f4 = 0x16; v1f0 = 0x1a; v3fc_ = 4; v1f8 = 2;
        v2bc = 2; v288 = 0x14; v28c = 0xffffffed;
    }
    this->field_0x2bc = v2bc;
    this->footerButtonOffset = v3fc_;
    this->field_0x200 = v200;
    this->field_0x1f0 = v1f0;
    this->field_0x1f4 = v1f4;
    this->field_0x1f8 = v1f8;
    this->field_0x284 = 1;
    this->field_0x285 = 1;
    this->field_0x286 = 1;
    this->field_0x288 = v288;
    this->field_0x28c = v28c;
    this->field_0x290 = wide ? (res == 0 ? 3 : 7) : 4;

    // Choice-window geometry group.
    int cwTitle;
    if (!hd) {
        int cw2f4;
        if (*g_cfgWide != 0) {
            cw2f4 = 0xa0;            // wide
        } else if (*g_cfgB != 0) {
            cw2f4 = 0x50;
        } else {
            cw2f4 = wide ? 0x3c : 0x2d;
            if (res == 0) cw2f4 = 0x1e;
        }
        this->field_0x2f4 = cw2f4;
        this->field_0x2f8 = 0;
        this->field_0x2fc = (*g_cfgWide != 0) ? 0x48 : 0x24;
        this->field_0x300 = (*g_cfgWide != 0) ? 400 : 200;
        this->field_0x304 = (*g_cfgWide != 0) ? 700 : 0x15e;
        cwTitle = (*g_cfgWide != 0) ? 500 : 0xfa;
    } else {
        this->field_0x2f4 = 0x70;
        this->field_0x2f8 = 0;
        this->field_0x2fc = 0x32;
        this->field_0x300 = 0x119;
        this->field_0x304 = 0x1ec;
        cwTitle = 0x15f;
    }
    this->choiceWindowTitleHeight = cwTitle;

    // Choice-button offsets group.
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

    // Help-window position.
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

__attribute__((visibility("hidden"))) extern PaintCanvas **gPC;

int Layout::drawMask4(int p1, int p2, int p3, int p4) {
    PaintCanvas *pc = *gPC;
    int saved = pc->GetColor();
    pc->SetColor(0x80);
    pc->FillRectangle(p1, p2, p3, 0);
    (void)p4;
    this->drawMaskTail(pc, saved);
    return 0;
}

void Layout_formatNumber(String *out, int n);

// Format `n` as a grouped credit amount with a trailing "$".
void Layout_formatCredits(String *out, int n) {
    String num;
    Layout_formatNumber(&num, n);
    String suffix("$");
    *out = num + suffix;
}

// Hidden global from drawBGPattern.
__attribute__((visibility("hidden"))) extern PaintCanvas **g_bgCanvas;

// Tile `img` across the (x, y, w, h) area.
void Layout::drawBGPattern(unsigned img, int x, int y, int w, int h) {
    PaintCanvas *pc = *g_bgCanvas;
    gCanvas->SetColor(this->drawColor);
    int iw = gCanvas->GetImage2DWidth(img);
    int cols = w / iw;
    int fullW = cols * iw;
    int ih = gCanvas->GetImage2DHeight(img);
    int rows = h / ih;
    int fullH = rows * ih;

    for (int r = 0; r < rows; r++) {
        int py = y + r * ih;
        int px = x;
        for (int c = 0; c < cols; c++) {
            pc->DrawImage2D(img, px, py);
            px += iw;
        }
    }

    int remH = h - fullH;
    int remW = w - fullW;
    if (remW > 0) {
        int py = y + fullH;
        for (int r = 0; r < rows; r++) {
            pc->DrawRegion2D(img, 0, 0, remW, ih, 0, 0, 0, x + fullW, py);
            py += ih;
        }
    }
    if (remH > 0) {
        int px2 = x;
        for (int c = 0; c < cols; c++) {
            pc->DrawRegion2D(img, 0, 0, iw, remH, 0, 0, 0, px2, y + fullH);
            px2 += iw;
        }
    }
    if (remW > 0 || remH > 0) {
        pc->DrawRegion2D(img, 0, 0, remW, remH, 0, 0, 0, x + fullW, y + fullH);
    }
}

// Hidden globals from formatNumber.
__attribute__((visibility("hidden"))) extern const char g_fnEmpty[];
__attribute__((visibility("hidden"))) extern const char g_fnSepA[];    // langs 0/10/11
__attribute__((visibility("hidden"))) extern const char g_fnSepB[];    // other langs
__attribute__((visibility("hidden"))) extern const char g_fnOverflow[];

// Build a grouped (thousands-separated) decimal string into `out`.
void Layout_formatNumber(String *out, int value) {
    unsigned mag = (value < 0) ? (unsigned)(-value) : (unsigned)value;

    String digits;
    digits.ctor_int((int)mag);
    int len = (int)digits.size();

    *out = String(g_fnEmpty);

    String sep;
    unsigned short lang = GameText_getLanguage();
    if (lang < 0xc && ((1u << (lang & 0xff)) & 0xc01u) != 0)
        sep = String(g_fnSepA);
    else
        sep = String(g_fnSepB);

    if (len < 4) {
        out->assign(&digits);
    } else {
        int i = len;
        while (i > 2) {
            i -= 3;
            String grp;
            grp.SubString(&digits, i, len);
            String prefix = (out->size() < 2) ? String() : sep;
            *out = prefix + grp + *out;
        }
        String head;
        head.SubString(&digits, 0, i);
        if (head.size() != 0)
            *out = head + sep + *out;
    }

    if ((unsigned)value > 0x7fffffff)
        *out = String(g_fnOverflow) + *out;
}

__attribute__((visibility("hidden"))) extern int *gW1;
__attribute__((visibility("hidden"))) extern int *gW2;
__attribute__((visibility("hidden"))) extern int *gW3;

// Centre a window over the full content area and forward to the core renderer.
void Layout::drawWindow2(const String *param, int flag) {
    String tmp(*param);
    int p4 = *gW2;
    int p5 = *gW3 - ((int *)gW1)[2];
    this->drawWindowImpl5(&tmp, 0, 0, p4, p5, flag);
}

// TouchButton::setPosition resolved through a global function pointer.
typedef void (*SetPosFn)(void *btn, int x, int y, int mode);
__attribute__((visibility("hidden"))) extern SetPosFn gSetPos;
__attribute__((visibility("hidden"))) extern int **gTB;

void Layout::setWindowDimensions(int p1, int p2, int p3, int p4) {
    SetPosFn setPos = gSetPos;
    this->windowX = p1;
    this->windowY = p2;
    this->windowWidth = p3;
    this->windowHeight = p4;
    setPos(this->helpButton, p3 + p1, p2, 0x12);
    int *tb = *gTB;
    setPos(this->backButton, tb[0x28 / 4] + this->windowX,
           (this->windowY + this->windowHeight) - this->footerButtonOffset, 0x21);
    setPos(this->secondaryButton, tb[0x28 / 4] + this->windowX,
           (this->windowY + this->windowHeight) - this->footerButtonOffset, 0x21);
}

__attribute__((visibility("hidden"))) extern FModSound **gFmod;

void Layout::showMissionRewardMessage(int show, bool flag) {
    if (show == 0)
        return;
    this->rewardMessageFlag = flag;
    this->rewardMessageActive = 1;
    FModSound *sound = *gFmod;
    this->rewardMessageTimer = 0;
    this->rewardCredits = show;
    sound->play(0x24, 0, 0, 0.0f);
}

void Layout::drawWindow5(const String *param, int p3, int p4, int p5, int p6) {
    String tmp(*param);
    this->drawWindowImpl5(&tmp, p3, p4, p5, p6, 1);
}

// Hidden globals from drawBGBorder.
__attribute__((visibility("hidden"))) extern PaintCanvas **g_bbCanvas;
__attribute__((visibility("hidden"))) extern int g_bbFlipTR;
__attribute__((visibility("hidden"))) extern int g_bbFlipL;
__attribute__((visibility("hidden"))) extern int g_bbFlipR;

// Draw a tiled rounded-rect border (corner images + edge tiling).
void Layout::drawBGBorder(unsigned corner, unsigned edge, int x, int y, int w, int h, int inset, int pad) {
    PaintCanvas *pc = *g_bbCanvas;
    int cw = gCanvas->GetImage2DWidth(corner);
    int ch = gCanvas->GetImage2DHeight(corner);
    int ew = gCanvas->GetImage2DWidth(edge);
    int eh = gCanvas->GetImage2DHeight(edge);

    // Four corners (top-left at base; others mirrored).
    pc->DrawImage2D(corner, inset + x, inset + y);
    int rightX = ((w + x) - cw) - inset;
    pc->DrawImage2D(corner, rightX, inset + y, (unsigned char)(1));
    int bottomY = ((y + h) - ch) - inset;
    pc->DrawImage2D(corner, inset + x, bottomY, (unsigned char)(2));
    pc->DrawImage2D(corner, rightX, bottomY, (unsigned char)(3));

    // Top/bottom edges (horizontal tiling of `edge`).
    int spanW = w + cw * -2 + inset * -2;
    int colsH = spanW / ew;
    int baseX = pad + x;
    int tileX = cw + pad + x;
    int topRowY = pad + y;
    int botRowY = (h - inset) + pad + y;
    for (int i = 0; i < colsH; i++) {
        pc->DrawImage2D(edge, tileX, topRowY);
        pc->DrawImage2D(edge, tileX, botRowY, (unsigned char)(2));
        tileX += ew;
    }
    int remW = spanW - colsH * ew;
    if (remW > 0) {
        pc->DrawRegion2D(edge, 0, 0, remW, eh, 0, 0, 0,
                         colsH * ew + baseX + cw, topRowY);
        pc->DrawRegion2D(edge, 0, 0, remW, eh, 0, g_bbFlipTR, 0,
                         spanW + baseX + cw, botRowY);
    }

    // Left/right edges (vertical tiling of `edge`).
    int spanH = h + ch * -2 + inset * -2;
    int rowsV = spanH / ew;
    int leftX = pad + x + inset;
    int rightEdgeX = w + x + pad;
    for (int i = 0; i < rowsV; i++) {
        pc->DrawRegion2D(edge, 0, 0, ew, eh, 0, g_bbFlipL, 0, leftX, 0);
        pc->DrawRegion2D(edge, 0, 0, ew, eh, 0, g_bbFlipR, 0, rightEdgeX, 0);
    }
    int remH = spanH - rowsV * ew;
    if (remH > 0) {
        pc->DrawRegion2D(edge, 0, 0, remH, eh, 0, g_bbFlipL, 0, leftX, 0);
        pc->DrawRegion2D(edge, 0, 0, remH, eh, 0, g_bbFlipR, 0,
                         rightEdgeX + inset * -2, 0);
    }
}

// Hidden globals from drawScrollBar.
__attribute__((visibility("hidden"))) extern PaintCanvas **g_sbCanvas;
__attribute__((visibility("hidden"))) extern unsigned g_sbColor0;
__attribute__((visibility("hidden"))) extern unsigned g_sbColor1;
__attribute__((visibility("hidden"))) extern int **g_sbMetric;          // [0][0x48]

void Layout::drawScrollBar(int x, int y, int trackH, int pos, int range) {
    PaintCanvas *pc = *g_sbCanvas;
    int iw = gCanvas->GetImage2DWidth(this->scrollBarImage);
    int ih = gCanvas->GetImage2DHeight(this->scrollBarImage);

    gCanvas->SetColor(g_sbColor0);
    gCanvas->SetColor(g_sbColor1);

    int inset = this->scrollBarInset;
    pc->DrawRectangle(x, inset + y, (*g_sbMetric)[0x48 / 4], trackH - inset * 2);
    gCanvas->SetColor(this->drawColor);

    int thumb = range - 1;
    if (thumb <= ih * 2) thumb = ih * 2;
    int off = pos + 1;
    if (trackH <= thumb + off)
        off = (trackH - thumb) - inset;

    int handle = this->scrollBarHandle;
    thumb = thumb + handle * -4;
    off = off + handle * 2;
    if (ih * 2 < thumb) {
        this->drawBGPattern(this->scrollBarFillImage, x + 1 + handle, ih + y + off, iw, thumb + ih * -2);
        handle = this->scrollBarHandle;
    } else {
        int lim = trackH + ih * -2;
        if (lim <= off) off = lim;
    }

    pc->DrawImage2D(this->scrollBarImage, handle + x + 1, 0);
    pc->DrawImage2D(this->scrollBarImage,
                    this->scrollBarHandle + x + 1, (thumb - ih) + y + off, (unsigned char)(0x02));
}

// Hidden globals from drawFade.
__attribute__((visibility("hidden"))) extern PaintCanvas **g_dfCanvasA;
__attribute__((visibility("hidden"))) extern int **g_dfDimA;             // [0][0]
__attribute__((visibility("hidden"))) extern PaintCanvas **g_dfCanvasB;
__attribute__((visibility("hidden"))) extern int **g_dfDimB;             // [0][0]

// Overlay the fade-in/out rectangles, returning the active flag.
uint8_t Layout::drawFade() {
    if (this->fading != 0) {
        PaintCanvas *pc = *g_dfCanvasA;
        unsigned saved = pc->GetColor();

        float t = (float)this->fadeProgress / (float)this->fadeDuration;
        if (this->fadeOut != 0)
            t = 1.0f - t;
        if (t > 1.0f) t = 1.0f;

        unsigned color = this->fadeColor;
        if (t > 0.0f)
            color += (int)(t * 255.0f);
        gCanvas->SetColor(color);
        (*g_dfCanvasA)->FillRectangle(0, 0, **g_dfDimA, 0);
        gCanvas->SetColor(saved);
    }
    if (this->fillScreen != 0) {
        PaintCanvas *pc = *g_dfCanvasB;
        unsigned saved = pc->GetColor();
        gCanvas->SetColor(0xff);
        (*g_dfCanvasB)->FillRectangle(0, 0, **g_dfDimB, 0);
        gCanvas->SetColor(saved);
    }
    return this->fading;
}

// Hidden globals from drawBox.
__attribute__((visibility("hidden"))) extern PaintCanvas **g_dbCanvas;
__attribute__((visibility("hidden"))) extern int **g_dbMetric0;         // case0 [0][0x44],[0][0x1c]
__attribute__((visibility("hidden"))) extern void **g_dbFont0c;
__attribute__((visibility("hidden"))) extern void **g_dbFont0r;
__attribute__((visibility("hidden"))) extern void **g_dbFont0;
__attribute__((visibility("hidden"))) extern int **g_dbMetric1;         // case1 [0][0x44],[0][0x5c]
__attribute__((visibility("hidden"))) extern void **g_dbFont1c;
__attribute__((visibility("hidden"))) extern void **g_dbFont1r;
__attribute__((visibility("hidden"))) extern void **g_dbFont1;
__attribute__((visibility("hidden"))) extern int **g_dbMetric6;         // case6 [0][0x44]
__attribute__((visibility("hidden"))) extern void **g_dbFont6c;
__attribute__((visibility("hidden"))) extern void **g_dbFont6r;
__attribute__((visibility("hidden"))) extern void **g_dbFont6;
__attribute__((visibility("hidden"))) extern int **g_dbMetric7;         // case7 [0][8],[0][0x28]
__attribute__((visibility("hidden"))) extern void **g_dbFont7;

// Draw one of the styled box backgrounds (0..10) with optional centred label text.
void Layout::drawBox(int style, int x, int y, int w, int h, String text, unsigned char flags) {
    PaintCanvas *pc = *g_dbCanvas;
    unsigned saved = pc->GetColor();
    gCanvas->SetColor(this->drawColor);

    switch (style) {
    case 0: {
        int iw = gCanvas->GetImage2DWidth(this->field_0x348);
        pc->DrawImage2D(this->field_0x348, x, y);
        this->drawBGPattern(this->field_0x34c, iw + x, y, w + iw * -2, h);
        pc->DrawImage2D(this->field_0x348, (w + x) - iw, y, (unsigned char)(0x01));
        if (text.size() == 0) break;
        int *mt = *g_dbMetric0;
        int tx = mt[0x44 / 4];
        if ((flags & 2) == 0) {
            if ((int)(flags << 0x1d) < 0) {
                int tw = pc->GetTextWidth((unsigned)(unsigned long)(*g_dbFont0c), (void *)0);
                tx = w / 2 - tw / 2;
            }
        } else {
            int tw = pc->GetTextWidth((unsigned)(unsigned long)(*g_dbFont0r), (void *)0);
            tx = (w - tx) - tw;
        }
        int ty = (y + (mt[0x1c / 4] >> 1) + 1) - this->textBaselineAdjust;
        pc->DrawString((unsigned)(unsigned long)(*g_dbFont0), &text, tx + x, ty, false);
        break;
    }
    case 1: {
        int iw = gCanvas->GetImage2DWidth(this->field_0x350);
        pc->DrawImage2D(this->field_0x350, x, y);
        this->drawBGPattern(this->field_0x354, iw + x, y, w + iw * -2, h);
        pc->DrawImage2D(this->field_0x350, (w + x) - iw, y, (unsigned char)(0x01));
        if (text.size() == 0) break;
        int *mt = *g_dbMetric1;
        int tx = mt[0x44 / 4];
        if ((flags & 2) == 0) {
            if ((int)(flags << 0x1d) < 0) {
                int tw = pc->GetTextWidth((unsigned)(unsigned long)(*g_dbFont1c), (void *)0);
                tx = w / 2 - tw / 2;
            }
        } else {
            int tw = pc->GetTextWidth((unsigned)(unsigned long)(*g_dbFont1r), (void *)0);
            tx = (w - tx) - tw;
        }
        int ty = (y + (mt[0x5c / 4] >> 1) + 1) - this->textBaselineAdjust;
        pc->DrawString((unsigned)(unsigned long)(*g_dbFont1), &text, tx + x, ty, false);
        break;
    }
    case 2:
        this->drawBGPattern(this->bgPatternImage, x, y, w, h);
        break;
    case 3: {
        int iw = gCanvas->GetImage2DWidth(this->field_0x358);
        pc->DrawImage2D(this->field_0x358, x, y);
        this->drawBGPattern(this->field_0x35c, iw + x, y, w + iw * -2, h);
        pc->DrawImage2D(this->field_0x358, (w + x) - iw, y, (unsigned char)(0x01));
        break;
    }
    case 4: {
        int iw = gCanvas->GetImage2DWidth(this->field_0x36c);
        pc->DrawImage2D(this->field_0x36c, x, y);
        this->drawBGPattern(this->field_0x370, iw + x, y, w + iw * -2, h);
        pc->DrawImage2D(this->field_0x36c, (w + x) - iw, y, (unsigned char)(0x01));
        break;
    }
    case 5:
        this->drawBGBorder6(this->field_0x380, this->field_0x384, x, y, w);
        break;
    case 6: {
        int iw = gCanvas->GetImage2DWidth(this->field_0x388);
        pc->DrawImage2D(this->field_0x388, x, y);
        this->drawBGPattern(this->field_0x38c, iw + x, y, w + iw * -2, h);
        pc->DrawImage2D(this->field_0x388, (w + x) - iw, y, (unsigned char)(0x01));
        if (text.size() == 0) break;
        int *mt = *g_dbMetric6;
        int tx = mt[0x44 / 4];
        if ((flags & 2) == 0) {
            if ((int)(flags << 0x1d) < 0) {
                int tw = pc->GetTextWidth((unsigned)(unsigned long)(*g_dbFont6c), (void *)0);
                tx = w / 2 - tw / 2;
            }
        } else {
            int tw = pc->GetTextWidth((unsigned)(unsigned long)(*g_dbFont6r), (void *)0);
            tx = (w - tx) - tw;
        }
        int ty = (y + (h >> 1) + 1) - this->textBaselineAdjust;
        pc->DrawString((unsigned)(unsigned long)(*g_dbFont6), &text, tx + x, ty, false);
        break;
    }
    case 7: {
        int *mt = *g_dbMetric7;
        int hdr = mt[8 / 4];
        this->drawBGPattern(this->bgPatternImage, x, hdr + y, w, h - hdr);
        int ih = gCanvas->GetImage2DHeight(this->field_0x394);
        this->drawBGBorder8(this->field_0x390, this->field_0x394, x, hdr + y, w, h - hdr, -ih, -ih);
        if (text.size() == 0) break;
        gCanvas->SetColor(0xffffffff);
        pc->DrawImage2D(this->headerIconImage, x, y);
        int ty = (y + (mt[8 / 4] / 2) + 1) - this->textBaselineAdjust;
        pc->DrawString((unsigned)(unsigned long)(*g_dbFont7), &text,
                       mt[0x28 / 4] + x, ty, false);
        break;
    }
    case 8:
        this->drawBGBorder6(this->field_0x39c, this->field_0x3a0, x, y, w);
        break;
    case 9: {
        int iw = gCanvas->GetImage2DWidth(this->field_0x360);
        pc->DrawImage2D(this->field_0x360, x, y);
        this->drawBGPattern(this->field_0x364, iw + x, y, w + iw * -2, h);
        pc->DrawImage2D(this->field_0x360, (w + x) - iw, y, (unsigned char)(0x01));
        break;
    }
    case 10: {
        int iw = gCanvas->GetImage2DWidth(this->field_0x368);
        pc->DrawImage2D(this->field_0x368, x, y);
        this->drawBGPattern(this->field_0x370, iw + x, y, w + iw * -2, h);
        pc->DrawImage2D(this->field_0x368, (w + x) - iw, y, (unsigned char)(0x01));
        break;
    }
    default:
        break;
    }

    pc->SetColor(saved);
}

// Hidden globals from drawWindow.
__attribute__((visibility("hidden"))) extern PaintCanvas **g_dwCanvas;
__attribute__((visibility("hidden"))) extern int **g_dwBorderTop;       // [0][8]
__attribute__((visibility("hidden"))) extern int **g_dwMetric;          // [0][8],[0][0x28]
__attribute__((visibility("hidden"))) extern const char g_dwCmpLit[];
__attribute__((visibility("hidden"))) extern void ***g_dwFont;

// Render the window frame (background fill, header bar, optional title text).
void Layout::drawWindow7(String *title, int x, int y, int w, int h, int drawBG) {
    PaintCanvas *pc = *g_dwCanvas;
    unsigned saved = pc->GetColor();
    if (drawBG != 0) {
        int top = (*g_dwBorderTop)[8 / 4];
        this->drawBGPattern(this->bgPatternImage, x, top + y, w, h - top);
    }
    gCanvas->SetColor(*(unsigned *)g_dwCanvas);  // canvas slot doubles as the colour int
    int *m = *g_dwMetric;
    int top = m[8 / 4];
    pc->DrawRectangle(x, top + y, w, h - top);
    gCanvas->SetColor(this->drawColor);
    pc->DrawImage2D(this->headerIconImage, x, 0);
    if (title->size() != 0 && title->Compare_char(g_dwCmpLit) == 0) {
        int *mm = *g_dwMetric;
        int half = mm[8 / 4];
        half += half >> 31;
        int ty = (y + (half >> 1) + 1) - this->textBaselineAdjust;
        pc->DrawString((unsigned)(unsigned long)(**g_dwFont), title, mm[0x28 / 4] + x, ty, false);
    }
    pc->SetColor(saved);
}

void Layout::drawBox6(int p2, int p3, int p4, int p5, int p6, const String *str) {
    String tmp(*str);
    this->drawBoxImpl(p2, p3, p4, p5, p6, &tmp, 1);
}

// Hidden globals from drawTip.
__attribute__((visibility("hidden"))) extern unsigned *g_dtColor;// [0]=color
__attribute__((visibility("hidden"))) extern int **g_dtMetricA;  // [0][0x78],[0][4]
__attribute__((visibility("hidden"))) extern int *g_dtDimW;      // [0]=width
__attribute__((visibility("hidden"))) extern int *g_dtDimH;      // [0]=height
__attribute__((visibility("hidden"))) extern const char g_dtBoxLit[];
__attribute__((visibility("hidden"))) extern void **g_dtLinesA;  // [0]
__attribute__((visibility("hidden"))) extern unsigned *g_dtLinesB;// [0]

void Layout::drawTip() {
    if (this->tipLines != nullptr) {
        gCanvas->SetColor(*g_dtColor);
        int *mA = *g_dtMetricA;
        int dimW = *g_dtDimW;
        int dimH = *g_dtDimH;
        int boxW = mA[0x78 / 4];

        String box(g_dtBoxLit);
        this->drawBoxStr(5, (dimH >> 1) - (boxW >> 1), (dimW >> 1) + 0xd, boxW, 100, &box);

        gCanvas->DrawImage2D(this->tipBoxImage,
                                 dimH >> 1, (dimW >> 1) + 0x3f, (unsigned char)(0x11));

        int lineCount = (int)this->tipLines->size();
        int y = (dimW >> 1) + 0x3f - ((lineCount * mA[4 / 4]) >> 1);
        Globals_drawLines(*g_dtLinesB, *(void **)g_dtLinesA, this->tipLines,
                          dimH >> 1, y);
    }
}

void Layout::drawHeader1(const String *param) {
    String tmp(*param);
    this->drawHeaderImpl(&tmp, 1);
}

void Layout::drawHelpWindow() {
    this->drawHelpWindowImpl();
}

__attribute__((visibility("hidden"))) extern FModSound **gFmodHelp;  // distinct global from showMissionRewardMessage's gFmod

void Layout::initHelpWindow(String text) {
    if (this->choiceWindow == nullptr)
        this->choiceWindow = new ChoiceWindow();
    (*gFmodHelp)->play(0x7e, 0, 0, 0);
    String *title = gGameText->getText(0x187);
    this->choiceWindow->set(*title, text);
    this->choiceWindowOpen = 1;
    this->helpPressedFlag = 0;
}

// Hidden globals from resetWindowDimensions.
__attribute__((visibility("hidden"))) extern int **g_rwW;        // ** -> width
__attribute__((visibility("hidden"))) extern int **g_rwH;        // ** -> height
__attribute__((visibility("hidden"))) extern int **g_rwMetric;   // [0][0x28] = inset
__attribute__((visibility("hidden"))) extern int *g_rwOutX;      // button x out
__attribute__((visibility("hidden"))) extern int *g_rwOutY;      // button y out

void Layout::resetWindowDimensions() {
    int h = **g_rwH;
    int w = **g_rwW;

    this->windowX = 0;
    this->windowY = 0;
    this->windowWidth = w;
    this->windowHeight = h;

    this->setBtnRect(this->helpButton, w, 0, 0x12);

    int *m = *g_rwMetric;
    int inset = m[0x28 / 4];
    this->setBtnRect(this->backButton, inset + this->windowX, (this->windowY + this->windowHeight) - this->footerButtonOffset, 0x21);
    this->setBtnRect(this->secondaryButton, inset + this->windowX, (this->windowY + this->windowHeight) - this->footerButtonOffset, 0x21);

    if (this->backButton != nullptr) {
        float pos[2] __attribute__((aligned(4)));
        TouchButton_getPosition((TouchButton *)pos);
        *g_rwOutX = (int)pos[0];
        TouchButton_getPosition((TouchButton *)pos);
        *g_rwOutY = (int)pos[1];
    }
}

// Hidden globals from tagString.
__attribute__((visibility("hidden"))) extern const String *g_tagBaseString;
__attribute__((visibility("hidden"))) extern const char g_tagLit0[];
__attribute__((visibility("hidden"))) extern const char g_tagLit1[];
__attribute__((visibility("hidden"))) extern const char g_tagLit2[];

// Build "<lit0><base><lit1>" + in + <lit2> into `out`.
void Layout_tagString(String *out, const String *in) {
    *out = String(g_tagLit0) + *g_tagBaseString + String(g_tagLit1) + *in + String(g_tagLit2);
}

__attribute__((visibility("hidden"))) extern PaintCanvas **g_rlCanvas;
__attribute__((visibility("hidden"))) extern char *g_rlHdFlag;  // HD variant
__attribute__((visibility("hidden"))) extern int *g_rlBackText;
__attribute__((visibility("hidden"))) extern int *g_rlScreenH;
__attribute__((visibility("hidden"))) extern int *g_rlMenuY;

// (re)load all layout images/strings and rebuild the buttons.
void Layout::reload() {
    this->bgPatternImage = -1;
    this->headerPatternImage = -1;
    this->headerIconImage = -1;
    this->field_0x3a8 = -1;
    // Bulk -1 fill of the footer image-handle block (members 0x334..0x3a4);
    // addressed by byte offset because the run spans documentation-only fields.
    memset((char *)this + 0x334, 0xff, 0x70);

    PaintCanvas *canvas = *g_rlCanvas;
    this->loadImage(canvas, 0x503, &this->tipBoxImage);
    this->loadImage(canvas, 0x47e, &this->bgPatternImage);
    this->loadImage(canvas, 0x4ff, &this->headerPatternImage);
    this->loadImage(canvas, 0x500, &this->headerCapImage);
    this->loadImage(canvas, 0x474, &this->headerIconImage);
    this->loadImage(canvas, 0x502, &this->footerImageLeft);
    this->loadImage(canvas, 0x506, &this->field_0x340);
    this->loadImage(canvas, 0x501, &this->field_0x338);
    this->loadImage(canvas, 0x507, &this->footerPatternImage);
    this->loadImage(canvas, 0x4fe, &this->footerImageRight);
    this->loadImage(canvas, 0x482, &this->field_0x348);
    this->loadImage(canvas, 0x481, &this->field_0x34c);
    this->loadImage(canvas, 0x486, &this->scrollBarFillImage);
    this->loadImage(canvas, 0x487, &this->scrollBarImage);
    this->loadImage(canvas, 0x48b, &this->field_0x37c);
    this->loadImage(canvas, 0x52d, &this->field_0x3a4);

    if (*g_rlHdFlag == 0) {
        this->loadImage(canvas, 0x480, &this->field_0x350);
        this->loadImage(canvas, 0x47f, &this->field_0x354);
        this->loadImage(canvas, 0x479, &this->field_0x358);
        this->loadImage(canvas, 0x478, &this->field_0x35c);
        this->loadImage(canvas, 0x489, &this->field_0x36c);
        this->loadImage(*g_rlCanvas, 0x488, &this->field_0x370);
    } else {
        this->loadImage(canvas, 0x6bb, &this->field_0x350);
        this->loadImage(canvas, 0x6ba, &this->field_0x354);
        this->loadImage(canvas, 0x6b9, &this->field_0x358);
        this->loadImage(canvas, 0x6b8, &this->field_0x35c);
        this->loadImage(canvas, 0x6b7, &this->field_0x36c);
        this->loadImage(*g_rlCanvas, 0x6bc, &this->field_0x370);
    }

    this->loadImage(canvas, 0x530, &this->field_0x360);
    this->loadImage(canvas, 0x531, &this->field_0x364);
    this->loadImage(canvas, 0x52f, &this->field_0x368);
    this->loadImage(canvas, 0x47c, &this->field_0x384);
    this->loadImage(canvas, 0x47d, &this->field_0x380);
    this->loadImage(canvas, 0x47b, &this->field_0x388);
    this->loadImage(canvas, 0x47a, &this->field_0x38c);
    this->loadImage(canvas, 0x484, &this->field_0x390);
    this->loadImage(canvas, 0x483, &this->field_0x394);
    this->loadImage(canvas, 0x50c, &this->field_0x3a0);
    this->loadImage(canvas, 0x50d, &this->field_0x39c);

    // Back button (string-labelled). Constructed via the engine ctor ABI entry
    // (its argument list does not map to a single public TouchButton ctor).
    TouchButton *bBack = (TouchButton *)::operator new(sizeof(TouchButton));
    String *txt = gGameText->getText(*g_rlBackText);
    int sh = *g_rlScreenH;
    TouchButton_ctorStr(bBack, txt, 2, this->buttonInsetX, sh - 3, '!');
    this->backButton = bBack;
    this->backButtonWidth = bBack->getWidth();

    // Secondary button (image if available, else string fallback).
    unsigned img535 = 0xffffffff;
    (*g_rlCanvas)->Image2DCreate(0x535, &img535);
    TouchButton *b2 = (TouchButton *)::operator new(sizeof(TouchButton));
    if (img535 == 0xffffffff) {
        String *t = gGameText->getText(*g_rlBackText);
        TouchButton_ctorStr(b2, t, 2, this->buttonInsetX,
                            *g_rlScreenH - this->footerButtonOffset, '!');
    } else {
        TouchButton_ctorImg(b2, img535, 2, this->buttonInsetX,
                            *g_rlScreenH - this->footerButtonOffset, '!');
    }
    this->secondaryButton = b2;

    // Help button (image).
    unsigned img471 = 0xffffffff;
    (*g_rlCanvas)->Image2DCreate(0x471, &img471);
    TouchButton *bHelp = (TouchButton *)::operator new(sizeof(TouchButton));
    TouchButton_ctorImg2(bHelp, img471, 1, *g_rlMenuY, 0, this->field_0x3c, 0x12, 0x04);
    this->helpButton = bHelp;

    int th = (*g_rlCanvas)->GetTextHeight(0);
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
    this->field_0x3dc = 0;
    this->fadeColor = 0;
    this->fadeProgress = 0;
    this->rewardMessageActive = 0;
    this->field_0x40d = 0;
    this->field_0x409 = 0;
}

void Layout_formatCredits(String *out, int n);

// Hidden globals from drawFooter.
__attribute__((visibility("hidden"))) extern PaintCanvas **g_dfCanvas;
__attribute__((visibility("hidden"))) extern int **g_dfMetric;     // [0][0x10],[0][0x74]
__attribute__((visibility("hidden"))) extern int g_dfWarnColor;
__attribute__((visibility("hidden"))) extern const char g_dfSep[];
__attribute__((visibility("hidden"))) extern const char g_dfTail[];
__attribute__((visibility("hidden"))) extern void **g_dfFont;

// Render the footer bar: end caps, pattern fill, the back/secondary button, the
// cargo-load text (warning-coloured when over capacity) and the credit total.
void Layout::drawFooterImpl(int stationMode, int showBack) {
    PaintCanvas *pc = *g_dfCanvas;
    gCanvas->SetColor(this->drawColor);
    int wRight = gCanvas->GetImage2DWidth(this->footerImageRight);
    int wLeft = gCanvas->GetImage2DWidth(this->footerImageLeft);

    pc->DrawImage2D(this->footerImageLeft, this->windowX,
                    this->windowY + this->windowHeight, (unsigned char)(0x11));
    int *m = *g_dfMetric;
    int footerH = m[0x10 / 4];
    pc->DrawImage2D(this->footerImageRight, this->windowX + wLeft,
                    (this->windowY + this->windowHeight) - footerH);
    int both = wLeft + wRight;
    this->drawBGPattern(this->field_0x338, both + this->windowX, (this->windowY + this->windowHeight) - footerH, this->windowWidth + both * -2, footerH);
    pc->DrawImage2D(this->footerImageRight,
                    (this->windowX - both) + this->windowWidth,
                    (this->windowHeight + this->windowY) - footerH, (unsigned char)(0x01));
    pc->DrawImage2D(this->footerImageLeft,
                    (this->windowX - wRight) + this->windowWidth,
                    (this->windowHeight + this->windowY) - footerH, (unsigned char)(0x01));

    int backVis = (!stationMode) && showBack;
    this->backButton->setVisible(backVis);
    if (!backVis)
        this->secondaryButton->draw();
    else if (showBack)
        this->backButton->draw();

    // Cargo load text, in warning colour if over capacity.
    Status *status = gStatus;
    int load = status->getShip()->getCurrentLoad();
    int maxLoad = status->getShip()->getMaxLoad();
    if (maxLoad < load)
        gCanvas->SetColor(*(unsigned *)&g_dfWarnColor);

    String sLoad;
    sLoad.ctor_int(status->getShip()->getCurrentLoad());
    String sMax;
    sMax.ctor_int(status->getShip()->getMaxLoad());
    String loadStr = sLoad + String(g_dfSep) + sMax + String(g_dfTail);

    {
        int x = this->windowX;
        int w = this->windowWidth;
        void *font = *g_dfFont;
        int tw = pc->GetTextWidth((unsigned)(unsigned long)(font), (void *)0);
        pc->DrawString((unsigned)(unsigned long)(font), &loadStr, (x + w / 2) - tw / 2,
                       (this->windowHeight + this->windowY) - this->footerTextInset, false);
    }
    gCanvas->SetColor(this->drawColor);

    String credStr;
    Layout_formatCredits(&credStr, status->getCredits());

    {
        int x = this->windowX;
        int w = this->windowWidth;
        void *font = *g_dfFont;
        if (stationMode) {
            int rightInset = m[0x74 / 4];
            int tw = pc->GetTextWidth((unsigned)(unsigned long)(font), (void *)0);
            pc->DrawString((unsigned)(unsigned long)(font), &credStr, ((w + x) - rightInset) - tw / 2,
                           (this->windowHeight + this->windowY) - this->footerTextInset, false);
        } else {
            int tw = pc->GetTextWidth((unsigned)(unsigned long)(font), (void *)0);
            pc->DrawString((unsigned)(unsigned long)(font), &credStr, (w + x - 10) - tw,
                           (this->windowHeight + this->windowY) - this->footerTextInset, false);
        }
    }
}

void Layout::drawHeader0() {
    String tmp("");
    this->drawHeaderImpl(&tmp, 0);
}

void Layout::drawFooterStation() {
    this->drawFooterImpl(1, 0);
}

// Hidden globals from drawHeader.
__attribute__((visibility("hidden"))) extern unsigned *g_dhColor;   // [0]=image/color
__attribute__((visibility("hidden"))) extern int **g_dhMetric;      // [0][0x28],[0][0x44]
__attribute__((visibility("hidden"))) extern void ***g_dhFont;

// Render the header bar (end caps, pattern fill, title text and help-button
// slide-in animation).
void Layout::drawHeader7(String *title, int transition) {
    PaintCanvas *pc = gCanvas;
    unsigned img = *g_dhColor;
    pc->SetColor(this->drawColor);
    int iw = pc->GetImage2DWidth(img);
    int ih = pc->GetImage2DHeight(img);
    pc->DrawImage2D(this->headerCapImage, this->windowX, this->windowY);
    this->drawBGPattern(this->headerPatternImage, this->windowX + iw, this->windowY, this->windowWidth + iw * -2, ih);
    pc->DrawImage2D(this->headerCapImage,
                    this->windowWidth + this->windowX,
                    this->windowY, iw, ih, (unsigned char)(0x11), (unsigned char)(0x12), (unsigned char)(0x01));
    if (title->size() != 0) {
        pc->DrawImage2D(this->headerIconImage, this->windowX, this->windowY);
        int *m = *g_dhMetric;
        pc->DrawString((unsigned)(unsigned long)(**g_dhFont), title,
                       m[0x28 / 4] + m[0x44 / 4] + this->windowX,
                       this->headerTitleY + this->windowY, false);
    }
    this->helpButtonEnabled = (uint8_t)transition;
    if (transition != 0 && this->choiceWindowOpen == 0)
        this->headerAnim(this->helpButton);
}

void Layout::drawWindow1(const String *param) {
    String tmp(*param);
    this->drawWindowImpl(&tmp, 0);
}

void Layout_formatCredits(String *out, int n);

// Hidden globals from drawMissionRewardMessage.
__attribute__((visibility("hidden"))) extern PaintCanvas **g_mrCanvas;
__attribute__((visibility("hidden"))) extern int *g_mrDimA;        // [0]=screen W
__attribute__((visibility("hidden"))) extern const char g_mrLit0[];
__attribute__((visibility("hidden"))) extern const char g_mrLit1[];
__attribute__((visibility("hidden"))) extern int *g_mrDimB;        // [0]=screen H
__attribute__((visibility("hidden"))) extern int *g_mrTextId;      // [0]=text id
__attribute__((visibility("hidden"))) extern void **g_mrFont;      // [0]=font
__attribute__((visibility("hidden"))) extern const char g_mrLit2[];

void Layout::drawMissionRewardMessage(bool transition) {
    if (this->rewardMessageActive != 0) {
        PaintCanvas *pc = *g_mrCanvas;
        unsigned saved = pc->GetColor();
        unsigned origColor = this->drawColor;
        gCanvas->SetColor(0xffffffff);

        // Pulse alpha based on the animation timer.
        int t = this->rewardMessageTimer;
        float a;
        if (t < 2000) {
            a = (float)t / 6000.0f;
        } else if (t > 5000) {
            a = (float)(7000 - t) / 6000.0f;
        } else {
            a = 1.0f;
        }
        unsigned col = (unsigned)((int)(a * 256.0f)) - 0x100;
        gCanvas->SetColor(col);

        unsigned newColor = pc->GetColor();
        int boxW = this->rewardBoxWidth;
        int boxH = this->rewardBoxHeight;
        int boxX = this->rewardBoxX;
        int boxY = this->rewardBoxY;
        this->drawColor = newColor;

        if (transition != 0) {
            int sw = *g_mrDimA;
            String s0(g_mrLit0);
            this->drawBoxStr(2, (sw >> 1) - (boxH >> 1), boxX, boxH, boxW, &s0);

            sw = *g_mrDimA;
            String s1(g_mrLit1);
            this->drawBoxStr(8, (sw >> 1) - (boxH >> 1), boxX, boxH, boxW, &s1);
        }

        int sh = *g_mrDimB;
        pc->DrawImage2D(this->field_0x3a4, sh >> 1, (char)boxX, (unsigned char)(0x11));

        String *txt = gGameText->getText(*g_mrTextId);
        String line(*txt);

        sh = *g_mrDimB;
        void *font = *g_mrFont;
        int tw = pc->GetTextWidth((unsigned)(unsigned long)(font), (void *)0);
        pc->DrawString((unsigned)(unsigned long)(font), &line, (sh >> 1) - (tw >> 1), boxX + boxY, false);

        String suffix(g_mrLit2);
        String credits;
        Layout_formatCredits(&credits, this->rewardCredits);
        line = suffix + credits;

        sh = *g_mrDimB;
        tw = pc->GetTextWidth((unsigned)(unsigned long)(font), (void *)0);
        pc->DrawString((unsigned)(unsigned long)(font), &line, (sh >> 1) - (tw >> 1),
                       this->rewardBoxY2 + boxX, false);
        gCanvas->SetColor(saved);
        this->drawColor = origColor;
    }
}

// ---- Internal helper methods --------------------------------------------
// The small dispatch / forwarder / init helpers that the public drawing API
// delegates to.

// Centre a window using the cached screen metrics, then forward to the renderer.
void Layout::drawWindowImpl(const String *str, int flag) {
    this->drawWindow2(str, flag);
}

// The core window renderer reached from the 2-arg / 5-arg wrappers.
void Layout::drawWindowImpl5(String *str, int x, int y, int w, int h, int flag) {
    this->drawWindow7(str, x, y, w, h, flag);
}

// The header-bar renderer core.
void Layout::drawHeaderImpl(String *title, int transition) {
    this->drawHeader7(title, transition);
}

// Owning-String variant of drawBox.
void Layout::drawBoxImpl(int style, int x, int y, int w, int h, String *text, int flags) {
    this->drawBox(style, x, y, w, h, *text, flags);
}

// drawBox over a borrowed String (no copy); flags default to 0.
void Layout::drawBoxStr(int style, int x, int y, int w, int h, String *text) {
    this->drawBox(style, x, y, w, h, *text, 0u);
}

// 8-arg drawBox variant: `color` overrides the draw colour for the box, `z` is
// the box height.
void Layout::drawBox8(int kind, int x, int y, int w, int color, String *text, int z) {
    unsigned saved = this->drawColor;
    this->drawColor = (unsigned)color;
    this->drawBox(kind, x, y, w, z, *text, 0u);
    this->drawColor = saved;
}

// Dim the whole screen behind a modal with a translucent rectangle in the mask
// colour, restoring the previous colour.
void Layout::drawMaskImpl(int x, int y, int w, int h) {
    PaintCanvas *pc = gCanvas;
    unsigned saved = pc->GetColor();
    pc->SetColor(this->drawColor);
    pc->FillRectangle(x, y, w, h);
    pc->SetColor(saved);
}

// Restore the canvas colour saved before a draw.
void Layout::drawMaskTail(PaintCanvas *pc, int savedColor) {
    pc->SetColor((unsigned)savedColor);
}

// Paint the active help/choice window.
void Layout::drawHelpWindowImpl() {
    if (this->choiceWindow != nullptr)
        this->choiceWindow->draw();
}

// drawBGBorder rounded-rect overloads onto the full 8-parameter renderer. The
// 6-arg form reuses the corner image's height as inset/pad.
void Layout::drawBGBorder6(unsigned corner, unsigned edge, int x, int y, int w) {
    int ch = gCanvas->GetImage2DHeight(corner);
    this->drawBGBorder(corner, edge, x, y, w, ch, 0, 0);
}

void Layout::drawBGBorder8(unsigned corner, unsigned edge, int x, int y, int w, int h, int inset, int pad) {
    this->drawBGBorder(corner, edge, x, y, w, h, inset, pad);
}

// ---- Canonical-named public draw entries --------------------------------

// Take an owning copy of the title and render the full window frame (with
// background) via the core renderer.
void Layout::drawWindow(String title, int x, int y, int w, int h) {
    String tmp(title);
    this->drawWindow7(&tmp, x, y, w, h, 1);
}

// Dim the entire screen behind a modal using the cached screen-size globals.
void Layout::drawMask() {
    int w = *gW;
    int h = *gH;
    this->drawMaskImpl(0, 0, w, h);
}

// Render a tiled rounded-rect border via the full 8-parameter renderer.
void Layout::drawBGBorderImpl(unsigned corner, unsigned edge, int x, int y, int w, int h, int inset, int pad) {
    this->drawBGBorder(corner, edge, x, y, w, h, inset, pad);
}

// Position a child TouchButton (back/secondary/help) at (x, y) with an anchor.
void Layout::setBtnRect(TouchButton *btn, int x, int y, int anchor) {
    btn->setPosition(x, y, (unsigned char)anchor);
}

// Create an Image2D resource into a layout field slot.
void Layout::loadImage(PaintCanvas *canvas, int imageId, void *field) {
    canvas->Image2DCreate((unsigned short)imageId, (unsigned int *)field);
}

// Slide the help button into the header when the header animates in.
void Layout::headerAnim(TouchButton *btn) {
    unsigned char sp[8] __attribute__((aligned(4)));
    TouchButton_footerAnim(btn, 0, 0, sp);
}

// Zero-seed the header geometry block (members 0x04..0x2d8) before the ctor's
// explicit scalar writes; addressed by byte offset because the run spans
// documentation-only fields.
void Layout::initConstBlock(int hd, int wide, int scale, int res) {
    (void)hd; (void)wide; (void)scale; (void)res;
    memset((char *)this + 0x04, 0, 0x2d8 - 0x04);
}

// Touch dispatch tails: forward the event to the resolved child TouchButton.
int Layout::dispatchTouchBegin(TouchButton *btn, int x, int y) {
    return btn->OnTouchBegin(x, y);
}

int Layout::dispatchTouchMove(TouchButton *btn, int x, int y) {
    return (int)btn->OnTouchMove(x, y);
}

int Layout::dispatchTouchEnd(TouchButton *btn, int x, int y) {
    return (int)btn->OnTouchEnd(x, y);
}

// Engine-named aliases routed to the canonical handlers.
int Layout::touch_end(int x, int y) {
    return this->OnTouchEnd(x, y);
}

void Layout::drawHeader_call(String *title) {
    this->drawHeader7(title, 0);
}
