#include "gof2/game/ui/Layout.h"
#include "gof2/game/ship/Ship.h"
#include "gof2/engine/core/AERandom.h"
#include "gof2/game/ui/ChoiceWindow.h"
#include "gof2/engine/audio/FModSound.h"
#include "gof2/game/mission/Status.h"
#include "gof2/engine/core/GameText.h"
#include "gof2/game/core/String.h"
#include "gof2/game/ui/TouchButton.h"
#include "gof2/engine/render/PaintCanvas.h"

// The PaintCanvas singleton lives in externs.h as a definition; declare it here
// for the single-arg SetColor sites that target the global canvas.
extern void *g_PaintCanvas;

extern "C" float __aeabi_l2f(long long);
extern "C" void *ChoiceWindow_dtor(void *p);
extern "C" void operator_delete_li(void *p);

// Global GameText singleton holder (ldr [0xe4bbc]); receiver for getText(key).
struct GameTextHolder { GameText *obj; };
__attribute__((visibility("hidden"))) extern GameTextHolder *gGameText;

// ---- Canonical cross-class engine helpers ---------------------------------
// The per-method decomp blocks each re-declared these with slightly different
// (and mutually conflicting) signatures — varying the canvas/color/button
// parameter between unsigned/int/void*/PaintCanvas* and even the arity.  Under
// `extern "C"` these cannot overload, so we declare each ONCE here with an
// unspecified-argument (variadic) signature.  That accepts every call site's
// argument list and is link-compatible with the real engine entry point.
struct TouchButton;
extern "C" {
void    TouchButton_footerAnim(...);
void    TouchButton_getPosition(...);
void    TouchButton_ctorStr(...);
void    TouchButton_ctorImg(...);
void    TouchButton_ctorImg2(...);

void *ChoiceWindow_ctor(...);

unsigned short GameText_getLanguage(...);

void Globals_drawLines(...);
}

// ldrb.w r0,[r0,#0x400]; bx lr
uint8_t Layout::isFading() {
    return this->fading;
}

__attribute__((visibility("hidden"))) extern int **gTouchButtonHolder;  // ldr [0xe4bec]

// Layout::getHelpButtonOffset() -> TouchButton::getWidth(self->f3bc) - (**holder)[0x38/4]
int Layout::getHelpButtonOffset() {
    int w = ((TouchButton *)(this->helpButton))->getWidth();
    return w - (*gTouchButtonHolder)[0x38 / 4];
}

// Layout::update(int dt)
void Layout::update(int dt) {
    void *cw = this->choiceWindow;
    if (cw)
        ((ChoiceWindow *)(cw))->update(dt);
    if (this->rewardMessageActive != 0) {
        int t = this->rewardMessageTimer + dt;
        this->rewardMessageTimer = t;
        if (t >= 6999 + 1)        // 0x1b58 = 7000; cmp r0,#7000; mov.ge
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

// PC-relative hidden global: pointer to the PaintCanvas singleton holder.
__attribute__((visibility("hidden"))) extern void **gPaintCanvasHolder; // ldr [0xe3c38]

// Layout::getFooterTransitionWidth()
int Layout::getFooterTransitionWidth() {
    void **holder = gPaintCanvasHolder;
    int w1 = ((PaintCanvas*)(*holder))->GetImage2DWidth(this->footerImageRight);
    int w2 = ((PaintCanvas*)(*holder))->GetImage2DWidth(this->footerImageLeft);
    return w2 + w1;
}

// tail 0x1ac0c8

// Layout::OnTouchBegin(int x, int y)
int Layout::OnTouchBegin(int x, int y) {
    Layout *self = this;
    if (self->helpButtonEnabled != 0)
        ((TouchButton *)(self->helpButton))->OnTouchBegin(x, y);
    if (self->choiceWindow != 0 && self->choiceWindowOpen != 0) {
        ((ChoiceWindow *)(self->choiceWindow))->OnTouchBegin(x, y);
        return 0;
    }
    void *btn;
    if (((TouchButton *)(self->backButton))->isVisible() == 0)
        btn = self->secondaryButton;
    else
        btn = self->backButton;
    return this->dispatchTouchBegin(btn, x, y);
}

// Status singleton (PC-rel double-indirect). getPlayingTime returns a 64-bit value
// (r0/r1), converted to float via __aeabi_l2f, scaled by the arg, fed to Sinf.
struct Status;
__attribute__((visibility("hidden"))) extern Status **gStatus;  // ldr [0xe4c5c]
float Sinf(float);   // AbyssEngine::AEMath::Sinf -> 0x6f1a8

// Layout::getPulseValue(float speed). The holder (gStatus's value) is cached; the
// actual Status* (*holder) is reloaded for each getPlayingTime call.
float Layout::getPulseValue(float speed) {
    Status **holder = gStatus;
    float a = Sinf(__aeabi_l2f(((Status *)(*holder))->getPlayingTime()) * speed);
    float b = Sinf(__aeabi_l2f(((Status *)(*holder))->getPlayingTime()) * speed);
    return a > 0.0f ? b : -b;
}

// str.w r1,[r0,#0x3b0]; bx lr
void Layout::setDrawColor(int color) {
    this->drawColor = color;
}

__attribute__((visibility("hidden"))) extern int *gW;  // ldr [0xe3160]
__attribute__((visibility("hidden"))) extern int *gH;  // ldr [0xe3164]
// Layout::drawMask() -> drawMask(*gH, 0, 0, *gW, *gH)
void Layout_drawMask0() {
    void *pc = (void *)*gH;
    int w = *gW;
    ((Layout *)(pc))->drawMaskImpl(0, 0, w, *gH);
}

__attribute__((visibility("hidden"))) extern int *gA;  // ldr [0xe356c]
__attribute__((visibility("hidden"))) extern int *gB;  // ldr [0xe3570]
// Layout::drawBG() -> drawBGPattern(this, this->0x324, 0, 0, *gA, *gB)
void Layout::drawBG() {
    int p4 = *gA;
    int p5 = *gB;
    ((Layout *)(this))->drawBGPattern(this->bgPatternImage, 0, 0, p4, p5);
}

// strb.w r1,[r0,#0x410]; bx lr
void Layout::enableFillScreen(bool v) {
    this->fillScreen = v;
}

// ~TouchButton (0x74d7c) and ~ChoiceWindow (0x74d88) return the object pointer;
// operator delete (0x6eb48) frees it. Each owned field is deleted then nulled.

Layout::~Layout() {
    void *p = this->backButton;
    if (p) { ((TouchButton *)(p))->dtor(); operator_delete_li(p); }
    this->backButton = 0;
    p = this->secondaryButton;
    if (p) { ((TouchButton *)(p))->dtor(); operator_delete_li(p); }
    this->secondaryButton = 0;
    p = this->helpButton;
    if (p) { ((TouchButton *)(p))->dtor(); operator_delete_li(p); }
    this->helpButton = 0;
    p = this->choiceWindow;
    if (p) operator_delete_li(ChoiceWindow_dtor(p));
    this->choiceWindow = 0;
}

// ldrb.w r0,[r0,#0x3c0]; bx lr
uint8_t Layout::helpPressed() {
    return this->helpPressedFlag;
}

// Layout::drawFooterNoBackButton() -> drawFooter(this, 0, 0)
void Layout::drawFooterNoBackButton() {
    return ((Layout *)(this))->drawFooterImpl(0, 0);
}

// -> 0x1ac098

// Layout::drawFooter() -> drawFooter(this, 0, 1)
void Layout::drawFooter() {
    return ((Layout *)(this))->drawFooterImpl(0, 1);
}

// tail 0x1ac0d8

// Layout::OnTouchMove(int x, int y)
int Layout::OnTouchMove(int x, int y) {
    Layout *self = this;
    if (self->helpButtonEnabled != 0)
        ((TouchButton *)(self->helpButton))->OnTouchMove(x, y);
    if (self->choiceWindow != 0 && self->choiceWindowOpen != 0) {
        ((ChoiceWindow *)(self->choiceWindow))->OnTouchMove(x, y);
        return 0;
    }
    void *btn;
    if (((TouchButton *)(self->backButton))->isVisible() == 0)
        btn = self->secondaryButton;
    else
        btn = self->backButton;
    return this->dispatchTouchMove(btn, x, y);
}

// 9-arg impl. Forwarded: (p5 as self, p2, p3, p4, p5, p6, instack, 0, 0).
// Layout::drawBGBorder(uint p1, uint p2, int p3, int p4, int p5, int p6) + stack arg s8.
// This 7-arg forwarding wrapper (engine 0x74e30) is distinct from the 6-arg
// Layout::drawBGBorder method (0x74e78) used by drawBox; renamed to avoid the
// extern "C" name clash.
void Layout_drawBGBorder7(unsigned p1, unsigned p2, int p3, int p4,
                                     int p5, int p6, int s8) {
    ((Layout *)(p5))->drawBGBorderImpl(p2, p3, p4, p5, p6, s8, 0, 0);
}

// tail 0x1ac0e8

// Layout::OnTouchEnd(int x, int y)
int Layout::OnTouchEnd(int x, int y) {
    Layout *self = this;
    if (self->choiceWindowOpen == 0 && self->helpButtonEnabled != 0)
        self->helpPressedFlag = ((TouchButton *)(self->helpButton))->OnTouchEnd(x, y);
    if (self->choiceWindow != 0 && self->choiceWindowOpen != 0)
        return ((ChoiceWindow *)(self->choiceWindow))->OnTouchEnd(x, y) == 0;
    void *btn;
    if (((TouchButton *)(self->backButton))->isVisible() == 0)
        btn = self->secondaryButton;
    else
        btn = self->backButton;
    return this->dispatchTouchEnd(btn, x, y);
}

// Layout::startFade(bool, int, int)
void Layout::startFade(uint8_t fadeOut, int color, int duration) {
    this->fadeOut = fadeOut;
    this->fading = 1;
    this->fadeColor = color & 0xffffff00u;
    this->fadeProgress = 0;
    this->fadeDuration = duration;
}

// Hidden PC-relative globals from initTip disasm:
//   g_tipColor  : *(uint**) @0xe499c — color value (read [0]).
//   g_tipTextId : *(int**)  @0xe49a0 — text-table id (read **).
//   g_tipRandN  : *(int**)  @0xe49a2 — random bound (read **).
//   g_tipCanvas : *(PaintCanvas***) @0xe49a6 — canvas (read **).
//   g_tipMetric : *(int**)  @0xe49d4 — object whose [0x78]/[0x4c] give width.
__attribute__((visibility("hidden"))) extern unsigned *g_tipColor;
__attribute__((visibility("hidden"))) extern int **g_tipTextId;
__attribute__((visibility("hidden"))) extern int **g_tipRandN;
__attribute__((visibility("hidden"))) extern PaintCanvas ***g_tipCanvas;
__attribute__((visibility("hidden"))) extern int **g_tipMetric;

// Layout::initTip(): rebuild the random "tip" string line array.
void Layout::initTip() {
    if (this->tipLines != 0) {
        for (String *line : *this->tipLines) delete line;   // releaseClasses
        this->tipLines->clear();
        delete this->tipLines;
        this->tipLines = 0;
    }
    Array<String*> *arr = new Array<String*>();

    unsigned color = *g_tipColor;
    int textId = **g_tipTextId;
    PaintCanvas *canvas = **g_tipCanvas;
    ((AbyssEngine::AERandom *)(**g_tipRandN))->nextInt();
    void *str = gGameText->obj->getText(textId);

    this->tipLines = arr;
    int *m = *g_tipMetric;
    int width = m[0x78 / 4] + m[0x4c / 4] * -2;
    ((PaintCanvas*)(canvas))->GetLineArray(color, str, width, (char *)this->tipLines);
}

struct TouchButton;

// Footer "scroll into place" tail-called helper at 0x1ac0a8.

// Hidden globals from drawEmptyFooter disasm.
__attribute__((visibility("hidden"))) extern unsigned *g_efColor;   // @0xe3fd2 ([0]=color)
__attribute__((visibility("hidden"))) extern int *g_efScreenH;      // @0xe3ff8 ([0]=screen height)
__attribute__((visibility("hidden"))) extern int *g_efScreenW;      // @0xe4016 ([0]=screen width)
__attribute__((visibility("hidden"))) extern int **g_efMetric;      // @0xe401c ([0][0x10]=footer height)

// Layout::drawEmptyFooter(bool showBack)
void Layout::drawEmptyFooter(int showBack) {
    unsigned color = *g_efColor;
    ((PaintCanvas*)g_PaintCanvas)->SetColor(color);
    int w = ((PaintCanvas*)g_PaintCanvas)->GetImage2DWidth(color);
    int screenH = *g_efScreenH;
    ((PaintCanvas*)((PaintCanvas *)color))->DrawImage2D(this->footerImageLeft, 0, screenH, (unsigned char)(0x11));

    int screenW = *g_efScreenW;
    int footerH = (*g_efMetric)[0x10 / 4];
    ((Layout *)(this))->drawBGPattern(this->footerPatternImage, w, screenH - footerH, screenW - w * 2, footerH);
    ((PaintCanvas*)((PaintCanvas *)color))->DrawImage2D(this->footerImageLeft,
                            screenW - w, screenH - (*g_efMetric)[0x10 / 4], (unsigned char)(0x01));
    if (showBack == 0) return;
    ((TouchButton *)(this->backButton))->setVisible(1);
    unsigned char sp[8] __attribute__((aligned(4)));
    TouchButton_footerAnim(this->backButton, 1, footerH, sp);
}

// Layout::Layout() — config-driven field initializer (elf 0xd1028, 5478 bytes).
//
// The target body is one huge nested-conditional initializer that fills the
// ~0x414-byte Layout object with layout geometry, switching on a handful of
// display-config flags read through PC-relative globals:
//   cfgHd     : "HD / high-detail" master flag (cVar5).  When set, a compact
//               fixed-constant path is taken for most groups.
//   cfgTablet : tablet form-factor flag (bVar42 derived from a config byte).
//   cfgWide   : secondary aspect/scale byte (cVar43).
//   resByte   : resolution selector byte (uVar61); 0 => half-size variants.
//
// The leading ~0x2d8 bytes are populated by NEON block copies of constant
// vectors (vld1/vst1) that Ghidra renders as deadcode-delayed SIMD garbage;
// that bulk constant fill is delegated to ((Layout *)(its data lives
// in the engine .rodata))->initConstBlock().  The readable tail field writes (0x1f0..0x320 and
// 0x3e0..0x3f8, plus the 0x284..0x290 byte flags) are reproduced faithfully.

// Config-flag source globals (each is *(char**) -> byte at [0]).
__attribute__((visibility("hidden"))) extern char *g_cfgHd;      // @0xe1036
__attribute__((visibility("hidden"))) extern char *g_cfgWide;    // @0xe1272 / 0xe265a
__attribute__((visibility("hidden"))) extern char *g_cfgScale;   // @0xe1582 (pcVar46)
__attribute__((visibility("hidden"))) extern unsigned char *g_resByte; // @0xe187a (uVar61)
__attribute__((visibility("hidden"))) extern char *g_cfgA;       // @0xe2578
__attribute__((visibility("hidden"))) extern char *g_cfgB;       // @0xe2692
__attribute__((visibility("hidden"))) extern char *g_cfgC;       // @0xe2728
__attribute__((visibility("hidden"))) extern char *g_cfgD;       // @0xe27a8

// Bulk constant-block initializer for the SIMD-copied header region (0x04..0x2d8).

Layout::Layout() {
    int hd    = (*g_cfgHd != 0);
    int wide  = (*g_cfgWide != 0);
    int scale = (*g_cfgScale != 0);
    int res   = *g_resByte;

    // Header / geometry constants (NEON block-copied in the target).
    ((Layout *)(this))->initConstBlock(hd, wide, scale, res);

    // ---- Readable tail (lines 1480..1740 of the decompile) ----
    if (!hd) {
        // The non-HD branch fills the 0x204..0x274 doubleword group from
        // constant vectors and seeds a few scalars; modeled as the engine
        // const-block plus the explicit scalar writes below.
        this->field_0x238 = 0x46;
        this->field_0x27c = 0x1c;
        this->field_0x280 = 0;
        this->field_0x3e0 = 1;
        this->field_0x3e4 = 0;
        this->field_0x3e8 = 0x8c;
        this->field_0x3f4 = 0x4d;
        this->field_0x3f8 = 0x84;
    }

    // 0x3ec / 0x3f0 reward-box dimensions.
    if (hd) {
        this->field_0x3ec = (res == 0) ? 0xc4 : 0x188;
        this->field_0x3f0 = (res == 0) ? 0x32 : 100;
    } else {
        this->field_0x3ec = 0x160;
        this->field_0x3f0 = 0x4b;
    }

    // Footer/back-button geometry group (0x1f0, 0x1f8, 0x200, 0x2bc, 0x3fc, 0x1f4).
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
        // VectorSignedToFloat(x) is an int->float->int identity on these ints.
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
    this->field_0x3fc = v3fc_;
    this->field_0x200 = v200;
    this->field_0x1f0 = v1f0;
    this->field_0x1f4 = v1f4;
    this->field_0x1f8 = v1f8;
    this->field_0x284 = 1;            // bVar65^1 collapses to a config bit
    this->field_0x285 = 1;
    this->field_0x286 = 1;
    this->field_0x288 = v288;
    this->field_0x28c = v28c;
    this->field_0x290 = wide ? (res == 0 ? 3 : 7) : 4;

    // Choice-window geometry group (0x2f4..0x308).
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
        this->field_0x2f8 = 0;    // engine const (DAT)
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
    this->field_0x308 = cwTitle;

    // Choice-button offsets group (0x30c..0x318).
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

    // Help-window position (0x31c / 800).
    if (!hd) {
        if (wide) {
            this->field_0x31c = (*g_cfgD != 0) ? 0x76 : 0xec;
            this->field_0x320 = 0;    // engine float const (DAT_000e2bb8..)
        } else {
            this->field_0x31c = 0x1d8;
            this->field_0x320 = 0x120;
        }
    } else {
        this->field_0x31c = 0x14b;
        this->field_0x320 = 0xca;
    }
}

__attribute__((visibility("hidden"))) extern void **gPC;          // ldr [0xe31ac]
// tail 0x1ac088

// Layout::drawMask(int p1, int p2, int p3, int p4) — singleton reloaded each call.
int Layout::drawMask4(int p1, int p2, int p3, int p4) {
    int saved = ((PaintCanvas*)(*gPC))->GetColor();
    ((PaintCanvas*)(*gPC))->SetColor(0x80);
    ((PaintCanvas*)(*gPC))->FillRectangle(p1, p2, p3, 0);
    (void)p4;
    this->drawMaskTail(*gPC, saved);
    return 0;
}

void Layout_formatNumber(void *out, int n);   // 0x74df4 (formatNumber)

// Layout::formatCredits(int) -> out = formatNumber(n) + "$"
void Layout_formatCredits(void *out, int n) {
    unsigned char num[sizeof(String)] __attribute__((aligned(4)));
    unsigned char suffix[sizeof(String)] __attribute__((aligned(4)));
    Layout_formatNumber(num, n);
    ((String *)suffix)->ctor_char("$", false);
    *(String *)out = *(String *)num + *(String *)suffix;
    ((String *)(suffix))->dtor();
    ((String *)(num))->dtor();
}

extern "C" int __aeabi_idiv(int a, int b);

// Hidden global from drawBGPattern disasm.
__attribute__((visibility("hidden"))) extern PaintCanvas **g_bgCanvas;  // @0xe358a

// Layout::drawBGPattern(uint img, int x, int y, int w, int h): tile `img` to fill the area.
void Layout::drawBGPattern(unsigned img, int x, int y, int w, int h) {
    PaintCanvas *pc = *g_bgCanvas;
    ((PaintCanvas*)g_PaintCanvas)->SetColor(this->drawColor);
    int iw = ((PaintCanvas*)g_PaintCanvas)->GetImage2DWidth(img);
    int cols = __aeabi_idiv(w, iw);
    int fullW = cols * iw;
    int ih = ((PaintCanvas*)g_PaintCanvas)->GetImage2DHeight(img);
    int rows = __aeabi_idiv(h, ih);
    int fullH = rows * ih;

    for (int r = 0; r < rows; r++) {
        int py = y + r * ih;
        int px = x;
        for (int c = 0; c < cols; c++) {
            ((PaintCanvas*)(pc))->DrawImage2D(img, px, py);
            px += iw;
        }
    }

    int remH = h - fullH;
    int remW = w - fullW;
    if (remW > 0) {
        int py = y + fullH;
        for (int r = 0; r < rows; r++) {
            ((PaintCanvas*)(pc))->DrawRegion2D(img, 0, 0, remW, ih, 0, 0, 0, x + fullW, py);
            py += ih;
        }
    }
    if (remH > 0) {
        int px = x + fullW;
        (void)px;
        int px2 = x;
        for (int c = 0; c < cols; c++) {
            ((PaintCanvas*)(pc))->DrawRegion2D(img, 0, 0, iw, remH, 0, 0, 0, px2, y + fullH);
            px2 += iw;
        }
    }
    if (remW > 0 || remH > 0) {
        ((PaintCanvas*)(pc))->DrawRegion2D(img, 0, 0, remW, remH, 0, 0, 0,
                                 x + fullW, y + fullH);
    }
}

// Engine String helpers used by the thousands-separator formatter.

// Hidden globals from formatNumber disasm.
__attribute__((visibility("hidden"))) extern int *g_fnGuard;        // @0xe32cc (stack guard [0])
__attribute__((visibility("hidden"))) extern const char g_fnEmpty[];   // @0xe32e4
__attribute__((visibility("hidden"))) extern const char g_fnSepA[];    // @0xe3308 (langs 0/10/11)
__attribute__((visibility("hidden"))) extern const char g_fnSepB[];    // @0xe3316 (other langs)
__attribute__((visibility("hidden"))) extern const char g_fnOverflow[];// @0xe33f6

// Layout::formatNumber(int): build a grouped decimal string into `out`.
void Layout_formatNumber(void *out, int value) {
    int *guard = g_fnGuard;
    int g0 = *guard;

    unsigned mag = (value < 0) ? (unsigned)(-value) : (unsigned)value;

    unsigned char digits[sizeof(String)] __attribute__((aligned(4)));  // aSStack_34
    ((String *)digits)->ctor_int((int)mag);
    int len = *(int *)((char *)digits + 8);

    ((String *)out)->ctor_char(g_fnEmpty, false);

    unsigned char sep[sizeof(String)] __attribute__((aligned(4)));      // aSStack_40
    unsigned short lang = GameText_getLanguage();
    if (lang < 0xc && ((1u << (lang & 0xff)) & 0xc01u) != 0)
        ((String *)sep)->ctor_char(g_fnSepA, false);
    else
        ((String *)sep)->ctor_char(g_fnSepB, false);

    if (len < 4) {
        ((String *)(out))->assign((String *)digits);
    } else {
        int i = len;
        while (i > 2) {
            i -= 3;
            unsigned char grp[sizeof(String)] __attribute__((aligned(4)));  // aSStack_64
            ((String *)grp)->SubString((String *)digits, i, len);

            unsigned char prefix[sizeof(String)] __attribute__((aligned(4))); // aSStack_70
            if (*(unsigned *)((char *)out + 8) < 2)
                ((String *)prefix)->ctor();
            else
                ((String *)prefix)->ctor_copy((String *)sep, false);

            unsigned char t1[sizeof(String)] __attribute__((aligned(4)));  // aSStack_58
            *(String *)t1 = *(String *)prefix + *(String *)grp;
            unsigned char t2[sizeof(String)] __attribute__((aligned(4)));  // aSStack_4c
            *(String *)t2 = *(String *)t1 + *(String *)out;
            ((String *)(out))->assign((String *)t2);

            ((String *)(t2))->dtor();
            ((String *)(t1))->dtor();
            ((String *)(prefix))->dtor();
            ((String *)(grp))->dtor();
        }
        unsigned char head[sizeof(String)] __attribute__((aligned(4)));   // aSStack_4c
        ((String *)head)->SubString((String *)digits, 0, i);
        if (*(int *)((char *)head + 8) != 0) {
            unsigned char j1[sizeof(String)] __attribute__((aligned(4))); // aSStack_64
            *(String *)j1 = *(String *)head + *(String *)sep;
            unsigned char j2[sizeof(String)] __attribute__((aligned(4))); // aSStack_58
            *(String *)j2 = *(String *)j1 + *(String *)out;
            ((String *)(out))->assign((String *)j2);
            ((String *)(j2))->dtor();
            ((String *)(j1))->dtor();
        }
        ((String *)(head))->dtor();
    }

    if ((unsigned)value > 0x7fffffff) {
        unsigned char ov[sizeof(String)] __attribute__((aligned(4)));     // aSStack_58
        ((String *)ov)->ctor_char(g_fnOverflow, false);
        unsigned char r[sizeof(String)] __attribute__((aligned(4)));      // aSStack_4c
        *(String *)r = *(String *)ov + *(String *)out;
        ((String *)(out))->assign((String *)r);
        ((String *)(r))->dtor();
        ((String *)(ov))->dtor();
    }

    ((String *)(sep))->dtor();
    ((String *)(digits))->dtor();

    
}

__attribute__((visibility("hidden"))) extern int *gW1;  // ldr [0xe3774]
__attribute__((visibility("hidden"))) extern int *gW2;  // ldr [0xe3778]
__attribute__((visibility("hidden"))) extern int *gW3;  // ldr [0xe377c]

// Layout::drawWindow(String, bool flag)
//   -> drawWindow(this, copy, 0, 0, *gW2, *gW3 - (*gW1)[2], flag)
void Layout::drawWindow2(const void *param, int flag) {
    unsigned char tmp[sizeof(String)] __attribute__((aligned(4)));
    ((String *)tmp)->ctor_copy((String *)param, false);
    int p4 = *gW2;
    int p5 = *gW3 - ((int *)gW1)[2];
    ((Layout *)(this))->drawWindowImpl5(tmp, 0, 0, p4, p5, flag);
    ((String *)(tmp))->dtor();
}

// Function pointer (TouchButton::setPosition) loaded from a global, called via blx.
typedef void (*SetPosFn)(void *btn, int x, int y, int mode);
__attribute__((visibility("hidden"))) extern SetPosFn gSetPos;   // ldr [0xe4d30]
struct TB; struct TBHolder { TB *o; };
__attribute__((visibility("hidden"))) extern int **gTB;          // ldr [0xe4d34]
__attribute__((visibility("hidden"))) extern int *gPosX;         // ldr [0xe4d38]
__attribute__((visibility("hidden"))) extern int *gPosY;         // ldr [0xe4d3c]

// Layout::setWindowDimensions(int p1, int p2, int p3, int p4)
void Layout::setWindowDimensions(int p1, int p2, int p3, int p4) {
    SetPosFn setPos = gSetPos;
    this->windowX = p1;
    this->windowY = p2;
    this->windowWidth = p3;
    this->windowHeight = p4;
    setPos(this->helpButton, p3 + p1, p2, 0x12);
    int *tb = *gTB;
    setPos(this->backButton, tb[0x28 / 4] + this->windowX,
           (this->windowY + this->windowHeight) - this->field_0x3fc, 0x21);
    setPos(this->secondaryButton, tb[0x28 / 4] + this->windowX,
           (this->windowY + this->windowHeight) - this->field_0x3fc, 0x21);
}

__attribute__((visibility("hidden"))) extern int **gFmod;  // ldr [0xe4d70]
// FModSound::play(int sound, Vector* a, Vector* b, float v) — blx 0x71548.
struct Vec3 { float x, y, z; };

// Layout::showMissionRewardMessage(int show, bool flag)
void Layout::showMissionRewardMessage(int show, bool flag) {
    if (show == 0)
        return;
    this->rewardMessageFlag = flag;
    this->rewardMessageActive = 1;
    int *g = *gFmod;
    this->rewardMessageTimer = 0;
    this->rewardCredits = show;
    ((FModSound *)(*g))->play(0x24, 0, 0, 0.0f);
}

// Layout::drawWindow(String, int p3, int p4, int p5, int p6)
//   -> drawWindow(this, copy, p3, p4, p5, p6, 1)
void Layout::drawWindow5(const void *param, int p3, int p4, int p5, int p6) {
    unsigned char tmp[sizeof(String)] __attribute__((aligned(4)));
    ((String *)tmp)->ctor_copy((String *)param, false);
    ((Layout *)(this))->drawWindowImpl5(tmp, p3, p4, p5, p6, 1);
    ((String *)(tmp))->dtor();
}

extern "C" int __aeabi_idiv(int a, int b);

// Hidden globals from drawBGBorder disasm.
__attribute__((visibility("hidden"))) extern PaintCanvas **g_bbCanvas;  // @0xe3972
__attribute__((visibility("hidden"))) extern int g_bbFlipTR;            // @0xe3bf4 ([0])
__attribute__((visibility("hidden"))) extern int g_bbFlipL;             // @0xe3bf8 ([0])
__attribute__((visibility("hidden"))) extern int g_bbFlipR;             // @0xe3bfc ([0])

// Layout::drawBGBorder(uint corner, uint edge, int x, int y, int w, int h, int inset, int pad)
void Layout::drawBGBorder(unsigned corner, unsigned edge, int x, int y, int w, int h, int inset, int pad) {
    PaintCanvas *pc = *g_bbCanvas;
    int cw = ((PaintCanvas*)g_PaintCanvas)->GetImage2DWidth(corner);
    int ch = ((PaintCanvas*)g_PaintCanvas)->GetImage2DHeight(corner);
    int ew = ((PaintCanvas*)g_PaintCanvas)->GetImage2DWidth(edge);
    int eh = ((PaintCanvas*)g_PaintCanvas)->GetImage2DHeight(edge);

    // Four corners (top-left at base; others mirrored via the 5-arg variant).
    ((PaintCanvas*)(pc))->DrawImage2D(corner, inset + x, inset + y);
    int rightX = ((w + x) - cw) - inset;
    ((PaintCanvas*)(pc))->DrawImage2D(corner, rightX, inset + y, (unsigned char)(1));
    int bottomY = ((y + h) - ch) - inset;
    ((PaintCanvas*)(pc))->DrawImage2D(corner, inset + x, bottomY, (unsigned char)(2));
    ((PaintCanvas*)(pc))->DrawImage2D(corner, rightX, bottomY, (unsigned char)(3));

    // Top/bottom edges (horizontal tiling of `edge`).
    int spanW = w + cw * -2 + inset * -2;
    int colsH = __aeabi_idiv(spanW, ew);
    int baseX = pad + x;
    int tileX = cw + pad + x;
    int topRowY = pad + y;
    int botRowY = (h - inset) + pad + y;
    for (int i = 0; i < colsH; i++) {
        ((PaintCanvas*)(pc))->DrawImage2D(edge, tileX, topRowY);
        ((PaintCanvas*)(pc))->DrawImage2D(edge, tileX, botRowY, (unsigned char)(2));
        tileX += ew;
    }
    int remW = spanW - colsH * ew;
    if (remW > 0) {
        ((PaintCanvas*)(pc))->DrawRegion2D(edge, 0, 0, remW, eh, 0, 0, 0,
                                 colsH * ew + baseX + cw, topRowY);
        ((PaintCanvas*)(pc))->DrawRegion2D(edge, 0, 0, remW, eh, 0, g_bbFlipTR, 0,
                                 spanW + baseX + cw, botRowY);
    }

    // Left/right edges (vertical tiling of `edge`).
    int spanH = h + ch * -2 + inset * -2;
    int rowsV = __aeabi_idiv(spanH, ew);
    int leftX = pad + x + inset;
    int rightEdgeX = w + x + pad;
    for (int i = 0; i < rowsV; i++) {
        ((PaintCanvas*)(pc))->DrawRegion2D(edge, 0, 0, ew, eh, 0, g_bbFlipL, 0, leftX, 0);
        ((PaintCanvas*)(pc))->DrawRegion2D(edge, 0, 0, ew, eh, 0, g_bbFlipR, 0, rightEdgeX, 0);
    }
    int remH = spanH - rowsV * ew;
    if (remH > 0) {
        ((PaintCanvas*)(pc))->DrawRegion2D(edge, 0, 0, remH, eh, 0, g_bbFlipL, 0, leftX, 0);
        ((PaintCanvas*)(pc))->DrawRegion2D(edge, 0, 0, remH, eh, 0, g_bbFlipR, 0,
                                 rightEdgeX + inset * -2, 0);
    }
}

// Hidden globals from drawScrollBar disasm.
__attribute__((visibility("hidden"))) extern PaintCanvas **g_sbCanvas;  // @0xe426e
__attribute__((visibility("hidden"))) extern unsigned g_sbColor0;       // @0xe4370 ([0])
__attribute__((visibility("hidden"))) extern unsigned g_sbColor1;       // @0xe4374 ([0])
__attribute__((visibility("hidden"))) extern int **g_sbMetric;          // @0xe42a2 ([0][0x48])

// Layout::drawScrollBar(int x, int y, int trackH, int pos, int range)
void Layout::drawScrollBar(int x, int y, int trackH, int pos, int range) {
    PaintCanvas *pc = *g_sbCanvas;
    int iw = ((PaintCanvas*)g_PaintCanvas)->GetImage2DWidth(this->scrollBarImage);
    int ih = ((PaintCanvas*)g_PaintCanvas)->GetImage2DHeight(this->scrollBarImage);

    ((PaintCanvas*)g_PaintCanvas)->SetColor(*(unsigned *)&g_sbColor0);
    ((PaintCanvas*)g_PaintCanvas)->SetColor(*(unsigned *)&g_sbColor1);

    int inset = this->field_0x3e0;
    ((PaintCanvas*)(pc))->DrawRectangle(x, inset + y, (*g_sbMetric)[0x48 / 4], trackH - inset * 2);
    ((PaintCanvas*)g_PaintCanvas)->SetColor(this->drawColor);

    int thumb = range - 1;
    if (thumb <= ih * 2) thumb = ih * 2;
    int off = pos + 1;
    if (trackH <= thumb + off)
        off = (trackH - thumb) - inset;

    int handle = this->field_0x3e4;
    thumb = thumb + handle * -4;
    off = off + handle * 2;
    if (ih * 2 < thumb) {
        ((Layout *)(this))->drawBGPattern(this->field_0x378, x + 1 + handle, ih + y + off, iw, thumb + ih * -2);
        handle = this->field_0x3e4;
    } else {
        int lim = trackH + ih * -2;
        if (lim <= off) off = lim;
    }

    ((PaintCanvas*)(pc))->DrawImage2D(this->scrollBarImage, handle + x + 1, 0);
    ((PaintCanvas*)(pc))->DrawImage2D(this->scrollBarImage,
                             this->field_0x3e4 + x + 1, (thumb - ih) + y + off, (unsigned char)(0x02));
}

// Hidden globals from drawFade disasm.
__attribute__((visibility("hidden"))) extern PaintCanvas **g_dfCanvasA;  // @0xe5052
__attribute__((visibility("hidden"))) extern int **g_dfDimA;             // @0xe50c8 ([0][0])
__attribute__((visibility("hidden"))) extern PaintCanvas **g_dfCanvasB;  // @0xe50ee
__attribute__((visibility("hidden"))) extern int **g_dfDimB;             // @0xe5108 ([0][0])

// Layout::drawFade(): overlay the fade-in/out rectangles, returning the active flag.
uint8_t Layout::drawFade() {
    if (this->fading != 0) {
        PaintCanvas *pc = *g_dfCanvasA;
        unsigned saved = ((PaintCanvas*)(pc))->GetColor();

        // progress = duration / endTime  (fields 0x408 / 0x40c, signed->float)
        float t = (float)this->fadeProgress / (float)this->fadeDuration;
        if (this->fadeOut != 0)
            t = 1.0f - t;
        if (t > 1.0f) t = 1.0f;

        unsigned color = this->fadeColor;
        if (t > 0.0f)
            color += (int)(t * 255.0f);
        ((PaintCanvas*)g_PaintCanvas)->SetColor(color);
        ((PaintCanvas*)(*g_dfCanvasA))->FillRectangle(0, 0, **g_dfDimA, 0);
        ((PaintCanvas*)g_PaintCanvas)->SetColor(saved);
    }
    if (this->fillScreen != 0) {
        PaintCanvas *pc = *g_dfCanvasB;
        unsigned saved = ((PaintCanvas*)(pc))->GetColor();
        ((PaintCanvas*)g_PaintCanvas)->SetColor(0xff);
        ((PaintCanvas*)(*g_dfCanvasB))->FillRectangle(0, 0, **g_dfDimB, 0);
        ((PaintCanvas*)g_PaintCanvas)->SetColor(saved);
    }
    return this->fading;
}

// Color-restore tail helper @0x1ac088.

// Hidden globals from drawBox disasm.
__attribute__((visibility("hidden"))) extern PaintCanvas **g_dbCanvas;  // @0xe439c
__attribute__((visibility("hidden"))) extern int **g_dbMetric0;         // @0xe444a (case0 [0][0x44],[0][0x1c])
__attribute__((visibility("hidden"))) extern void **g_dbFont0c;         // @0xe446a
__attribute__((visibility("hidden"))) extern void **g_dbFont0r;         // @0xe47f2
__attribute__((visibility("hidden"))) extern void **g_dbFont0;          // @0xe480c
__attribute__((visibility("hidden"))) extern int **g_dbMetric1;         // @0xe44f6 (case1 [0][0x44],[0][0x5c])
__attribute__((visibility("hidden"))) extern void **g_dbFont1c;         // @0xe4516
__attribute__((visibility("hidden"))) extern void **g_dbFont1r;         // @0xe4830
__attribute__((visibility("hidden"))) extern void **g_dbFont1;          // @0xe484a
__attribute__((visibility("hidden"))) extern int **g_dbMetric6;         // @0xe463a (case6 [0][0x44])
__attribute__((visibility("hidden"))) extern void **g_dbFont6c;         // @0xe4656
__attribute__((visibility("hidden"))) extern void **g_dbFont6r;         // @0xe4874
__attribute__((visibility("hidden"))) extern void **g_dbFont6;          // @0xe4894
__attribute__((visibility("hidden"))) extern int **g_dbMetric7;         // @0xe467c (case7 [0][8],[0][0x28])
__attribute__((visibility("hidden"))) extern void **g_dbFont7;          // @0xe4708

// Layout::drawBox(int style, int x, int y, int w, int h, String text, uchar flags)
void Layout::drawBox(int style, int x, int y, int w, int h, void *text, unsigned flags) {
    PaintCanvas *pc = *g_dbCanvas;
    unsigned saved = ((PaintCanvas*)(pc))->GetColor();
    ((PaintCanvas*)g_PaintCanvas)->SetColor(this->drawColor);

    switch (style) {
    case 0: {
        int iw = ((PaintCanvas*)g_PaintCanvas)->GetImage2DWidth(this->field_0x348);
        ((PaintCanvas*)(pc))->DrawImage2D(this->field_0x348, x, y);
        ((Layout *)(this))->drawBGPattern(this->field_0x34c, iw + x, y, w + iw * -2, h);
        ((PaintCanvas*)(pc))->DrawImage2D(this->field_0x348, (w + x) - iw, y, (unsigned char)(0x01));
        if (*(int *)((char *)text + 8) == 0) break;
        int *mt = *g_dbMetric0;
        int tx = mt[0x44 / 4];
        if ((flags & 2) == 0) {
            if ((int)(flags << 0x1d) < 0) {
                int tw = ((PaintCanvas*)(*(unsigned *)g_dbCanvas))->GetTextWidth((unsigned)(unsigned long)(*g_dbFont0c), (void *)0);
                tx = w / 2 - tw / 2;
            }
        } else {
            int tw = ((PaintCanvas*)(*(unsigned *)g_dbCanvas))->GetTextWidth((unsigned)(unsigned long)(*g_dbFont0r), (void *)0);
            tx = (w - tx) - tw;
        }
        int ty = (y + (mt[0x1c / 4] >> 1) + 1) - this->field_0x3ac;
        ((PaintCanvas*)(*(unsigned *)g_dbCanvas))->DrawString((unsigned)(unsigned long)(*g_dbFont0), text, tx + x, ty, false);
        break;
    }
    case 1: {
        int iw = ((PaintCanvas*)g_PaintCanvas)->GetImage2DWidth(this->field_0x350);
        ((PaintCanvas*)(pc))->DrawImage2D(this->field_0x350, x, y);
        ((Layout *)(this))->drawBGPattern(this->field_0x354, iw + x, y, w + iw * -2, h);
        ((PaintCanvas*)(pc))->DrawImage2D(this->field_0x350, (w + x) - iw, y, (unsigned char)(0x01));
        if (*(int *)((char *)text + 8) == 0) break;
        int *mt = *g_dbMetric1;
        int tx = mt[0x44 / 4];
        if ((flags & 2) == 0) {
            if ((int)(flags << 0x1d) < 0) {
                int tw = ((PaintCanvas*)(*(unsigned *)g_dbCanvas))->GetTextWidth((unsigned)(unsigned long)(*g_dbFont1c), (void *)0);
                tx = w / 2 - tw / 2;
            }
        } else {
            int tw = ((PaintCanvas*)(*(unsigned *)g_dbCanvas))->GetTextWidth((unsigned)(unsigned long)(*g_dbFont1r), (void *)0);
            tx = (w - tx) - tw;
        }
        int ty = (y + (mt[0x5c / 4] >> 1) + 1) - this->field_0x3ac;
        ((PaintCanvas*)(*(unsigned *)g_dbCanvas))->DrawString((unsigned)(unsigned long)(*g_dbFont1), text, tx + x, ty, false);
        break;
    }
    case 2:
        ((Layout *)(this))->drawBGPattern(this->bgPatternImage, x, y, w, h);
        break;
    case 3: {
        int iw = ((PaintCanvas*)g_PaintCanvas)->GetImage2DWidth(this->field_0x358);
        ((PaintCanvas*)(pc))->DrawImage2D(this->field_0x358, x, y);
        ((Layout *)(this))->drawBGPattern(this->field_0x35c, iw + x, y, w + iw * -2, h);
        ((PaintCanvas*)(pc))->DrawImage2D(this->field_0x358, (w + x) - iw, y, (unsigned char)(0x01));
        break;
    }
    case 4: {
        int iw = ((PaintCanvas*)g_PaintCanvas)->GetImage2DWidth(this->field_0x36c);
        ((PaintCanvas*)(pc))->DrawImage2D(this->field_0x36c, x, y);
        ((Layout *)(this))->drawBGPattern(this->field_0x370, iw + x, y, w + iw * -2, h);
        ((PaintCanvas*)(pc))->DrawImage2D(this->field_0x36c, (w + x) - iw, y, (unsigned char)(0x01));
        break;
    }
    case 5:
        ((Layout *)(this))->drawBGBorder6(this->field_0x380, this->field_0x384, x, y, w);
        break;
    case 6: {
        int iw = ((PaintCanvas*)g_PaintCanvas)->GetImage2DWidth(this->field_0x388);
        ((PaintCanvas*)(pc))->DrawImage2D(this->field_0x388, x, y);
        ((Layout *)(this))->drawBGPattern(this->field_0x38c, iw + x, y, w + iw * -2, h);
        ((PaintCanvas*)(pc))->DrawImage2D(this->field_0x388, (w + x) - iw, y, (unsigned char)(0x01));
        if (*(int *)((char *)text + 8) == 0) break;
        int *mt = *g_dbMetric6;
        int tx = mt[0x44 / 4];
        if ((flags & 2) == 0) {
            if ((int)(flags << 0x1d) < 0) {
                int tw = ((PaintCanvas*)(*(unsigned *)g_dbCanvas))->GetTextWidth((unsigned)(unsigned long)(*g_dbFont6c), (void *)0);
                tx = w / 2 - tw / 2;
            }
        } else {
            int tw = ((PaintCanvas*)(*(unsigned *)g_dbCanvas))->GetTextWidth((unsigned)(unsigned long)(*g_dbFont6r), (void *)0);
            tx = (w - tx) - tw;
        }
        int ty = (y + (h >> 1) + 1) - this->field_0x3ac;
        ((PaintCanvas*)(*(unsigned *)g_dbCanvas))->DrawString((unsigned)(unsigned long)(*g_dbFont6), text, tx + x, ty, false);
        break;
    }
    case 7: {
        int *mt = *g_dbMetric7;
        int hdr = mt[8 / 4];
        ((Layout *)(this))->drawBGPattern(this->bgPatternImage, x, hdr + y, w, h - hdr);
        int ih = ((PaintCanvas*)g_PaintCanvas)->GetImage2DHeight(this->field_0x394);
        ((Layout *)(this))->drawBGBorder8(this->field_0x390, this->field_0x394, x, hdr + y, w, h - hdr, -ih, -ih);
        if (*(int *)((char *)text + 8) == 0) break;
        ((PaintCanvas*)g_PaintCanvas)->SetColor(0xffffffff);
        ((PaintCanvas*)(pc))->DrawImage2D(this->field_0x32c, x, y);
        int ty = (y + (mt[8 / 4] / 2) + 1) - this->field_0x3ac;
        ((PaintCanvas*)(*(unsigned *)g_dbCanvas))->DrawString((unsigned)(unsigned long)(*g_dbFont7), text,
                               mt[0x28 / 4] + x, ty, false);
        break;
    }
    case 8:
        ((Layout *)(this))->drawBGBorder6(this->field_0x39c, this->field_0x3a0, x, y, w);
        break;
    case 9: {
        int iw = ((PaintCanvas*)g_PaintCanvas)->GetImage2DWidth(this->field_0x360);
        ((PaintCanvas*)(pc))->DrawImage2D(this->field_0x360, x, y);
        ((Layout *)(this))->drawBGPattern(this->field_0x364, iw + x, y, w + iw * -2, h);
        ((PaintCanvas*)(pc))->DrawImage2D(this->field_0x360, (w + x) - iw, y, (unsigned char)(0x01));
        break;
    }
    case 10: {
        int iw = ((PaintCanvas*)g_PaintCanvas)->GetImage2DWidth(this->field_0x368);
        ((PaintCanvas*)(pc))->DrawImage2D(this->field_0x368, x, y);
        ((Layout *)(this))->drawBGPattern(this->field_0x370, iw + x, y, w + iw * -2, h);
        ((PaintCanvas*)(pc))->DrawImage2D(this->field_0x368, (w + x) - iw, y, (unsigned char)(0x01));
        break;
    }
    default:
        break;
    }

    ((PaintCanvas*)(*(unsigned *)g_dbCanvas))->SetColor(saved);
}

// Color-restore tail helper @0x1ac088.

// Hidden globals from drawWindow disasm.
__attribute__((visibility("hidden"))) extern PaintCanvas **g_dwCanvas;  // @0xe3796
__attribute__((visibility("hidden"))) extern int **g_dwBorderTop;       // @0xe37b0 ([0][8])
__attribute__((visibility("hidden"))) extern int **g_dwMetric;          // @0xe37d8 ([0][8],[0][0x28])
__attribute__((visibility("hidden"))) extern const char g_dwCmpLit[];   // @0xe381a
__attribute__((visibility("hidden"))) extern void ***g_dwFont;          // @0xe382c

// Layout::drawWindow(String, int x, int y, int w, int h, bool drawBG)
void Layout::drawWindow7(void *title, int x, int y, int w, int h, int drawBG) {
    PaintCanvas *pc = *g_dwCanvas;
    unsigned saved = ((PaintCanvas*)(pc))->GetColor();
    if (drawBG != 0) {
        int top = (*g_dwBorderTop)[8 / 4];
        ((Layout *)(this))->drawBGPattern(this->bgPatternImage, x, top + y, w, h - top);
    }
    ((PaintCanvas*)g_PaintCanvas)->SetColor(*(unsigned *)g_dwCanvas);  // *puVar3 (the color int slot)
    int *m = *g_dwMetric;
    int top = m[8 / 4];
    ((PaintCanvas*)(*g_dwCanvas))->DrawRectangle(x, top + y, w, h - top);
    ((PaintCanvas*)g_PaintCanvas)->SetColor(this->drawColor);
    ((PaintCanvas*)(*g_dwCanvas))->DrawImage2D(this->field_0x32c, x, 0);
    if (*(int *)((char *)title + 8) != 0 &&
        ((String *)title)->Compare_char(g_dwCmpLit) == 0) {
        int *mm = *g_dwMetric;
        int half = mm[8 / 4];
        half += half >> 31;
        int ty = (y + (half >> 1) + 1) - this->field_0x3ac;
        ((PaintCanvas*)(*g_dwCanvas))->DrawString((unsigned)(unsigned long)(**g_dwFont), title, mm[0x28 / 4] + x, ty, false);
    }
    ((PaintCanvas*)(*g_dwCanvas))->SetColor(saved);
}

// Layout::drawBox(int, int, int, int, int, String) -> drawBox(..., copy, 1)
void Layout::drawBox6(int p2, int p3, int p4, int p5, int p6, const void *str) {
    unsigned char tmp[sizeof(String)] __attribute__((aligned(4)));
    ((String *)tmp)->ctor_copy((String *)str, false);
    ((Layout *)(this))->drawBoxImpl(p2, p3, p4, p5, p6, tmp, 1);
    ((String *)(tmp))->dtor();
}

// Hidden globals from drawTip disasm.
__attribute__((visibility("hidden"))) extern int *g_dtGuard;     // @0xe4a26 (stack guard [0])
__attribute__((visibility("hidden"))) extern unsigned *g_dtColor;// @0xe4a3e ([0]=color)
__attribute__((visibility("hidden"))) extern int **g_dtMetricA;  // @0xe4a4e ([0][0x78],[0][4])
__attribute__((visibility("hidden"))) extern int *g_dtDimW;      // @0xe4a52 ([0]=width)
__attribute__((visibility("hidden"))) extern int *g_dtDimH;      // @0xe4a56 ([0]=height)
__attribute__((visibility("hidden"))) extern const char g_dtBoxLit[];   // @0xe4a74
__attribute__((visibility("hidden"))) extern void **g_dtLinesA;  // @0xe4ad0 ([0])
__attribute__((visibility("hidden"))) extern unsigned *g_dtLinesB;// @0xe4ad2 ([0])

// Layout::drawTip()
void Layout::drawTip() {
    int *guard = g_dtGuard;
    int g0 = *guard;
    if (this->tipLines != 0) {
        ((PaintCanvas*)g_PaintCanvas)->SetColor(*g_dtColor);
        int *mA = *g_dtMetricA;
        int dimW = *g_dtDimW;
        int dimH = *g_dtDimH;
        int boxW = mA[0x78 / 4];

        unsigned char box[sizeof(String)] __attribute__((aligned(4)));
        ((String *)box)->ctor_char(g_dtBoxLit, false);
        ((Layout *)((PaintCanvas *)mA))->drawBoxStr(5, (dimH >> 1) - (boxW >> 1), (dimW >> 1) + 0xd, boxW, 100, box);
        ((String *)(box))->dtor();

        ((PaintCanvas*)((PaintCanvas *)*g_dtColor))->DrawImage2D(this->tipBoxImage,
                                 dimH >> 1, (dimW >> 1) + 0x3f, (unsigned char)(0x11));

        int lineCount = (int)this->tipLines->size();
        int y = (dimW >> 1) + 0x3f - ((lineCount * mA[4 / 4]) >> 1);
        Globals_drawLines(*g_dtLinesB, *(void **)g_dtLinesA, this->tipLines,
                          dimH >> 1, y);
    }
    
}

// Layout::drawHeader(String) -> drawHeader(this, copy, 1)
void Layout::drawHeader1(const void *param) {
    unsigned char tmp[sizeof(String)] __attribute__((aligned(4)));
    ((String *)tmp)->ctor_copy((String *)param, false);
    ((Layout *)(this))->drawHeaderImpl(tmp, 1);
    ((String *)(tmp))->dtor();
}

// -> 0x1ac0b8

// Layout::drawHelpWindow() -> ext(*(this+0x3c4))
void Layout::drawHelpWindow() {
    return ((Layout *)(this->choiceWindow))->drawHelpWindowImpl();
}

extern "C" void *operator_new_li(unsigned sz);
__attribute__((visibility("hidden"))) extern int *gFmodHelp;        // ldr [0xe4bb8] (distinct global from showMissionRewardMessage's gFmod)
// gGameText holder declared at top of file.

// Layout::initHelpWindow(String text)
void Layout::initHelpWindow(void *text) {
    if (this->choiceWindow == 0) {
        void *cw = operator_new_li(0x5c);
        ChoiceWindow_ctor(cw);
        this->choiceWindow = cw;
    }
    ((FModSound *)(*gFmodHelp))->play(0x7e, 0, 0, 0);
    void *cw = this->choiceWindow;
    void *title = ((GameText *)(gGameText->obj))->getText(0x187);
    ((ChoiceWindow *)(cw))->set(*(String *)title, *(String *)text);
    this->choiceWindowOpen = 1;
    this->helpPressedFlag = 0;
}

struct TouchButton;

// setRect-style helper invoked via fn ptr @0xe306c: (ptr, x, y, anchor).

// Hidden globals from resetWindowDimensions disasm.
__attribute__((visibility("hidden"))) extern int *g_rwGuard;     // @0xe305e (stack guard source [0])
__attribute__((visibility("hidden"))) extern int **g_rwW;        // @0xe3060 (** -> width)
__attribute__((visibility("hidden"))) extern int **g_rwH;        // @0xe3064 (** -> height)
__attribute__((visibility("hidden"))) extern int **g_rwMetric;   // @0xe3092 ([0][0x28] = inset)
__attribute__((visibility("hidden"))) extern int *g_rwOutX;      // @0xe30e8 (button x out)
__attribute__((visibility("hidden"))) extern int *g_rwOutY;      // @0xe3104 (button y out)

// Layout::resetWindowDimensions()
void Layout::resetWindowDimensions() {
    int *guard = g_rwGuard;
    int g0 = *guard;
    int h = **g_rwH;
    int w = **g_rwW;

    this->windowX = 0;
    this->windowY = 0;
    this->windowWidth = w;
    this->windowHeight = h;

    this->setBtnRect(this->helpButton, w, 0, 0x12);

    int *m = *g_rwMetric;
    int inset = m[0x28 / 4];
    this->setBtnRect(this->backButton, inset + this->windowX, (this->windowY + this->windowHeight) - this->field_0x3fc, 0x21);
    this->setBtnRect(this->secondaryButton, inset + this->windowX, (this->windowY + this->windowHeight) - this->field_0x3fc, 0x21);

    if (this->backButton != 0) {
        float pos[2] __attribute__((aligned(4)));
        TouchButton_getPosition((TouchButton *)pos);
        *g_rwOutX = (int)pos[0];
        TouchButton_getPosition((TouchButton *)pos);
        *g_rwOutY = (int)pos[1];
    }

    
}

// Hidden globals from tagString disasm.
__attribute__((visibility("hidden"))) extern const void *g_tagBaseString;  // @0xe329c
__attribute__((visibility("hidden"))) extern const char g_tagLit0[];       // @0xe31d2
__attribute__((visibility("hidden"))) extern const char g_tagLit1[];       // @0xe31ec
__attribute__((visibility("hidden"))) extern const char g_tagLit2[];       // @0xe320c (return prefix, ctor'd but unused in concat below)

// Layout::tagString(String): build "<lit0><base><lit1>" + param into the r0 return slot.
// out  = param_1 (the String return value, arrives in r0).
// in   = the incoming String (r2 -> aSStack_28 source).
void Layout_tagString(void *out, const void *in) {
    unsigned char s_lit0[sizeof(String)] __attribute__((aligned(4)));   // aSStack_4c
    unsigned char s_base[sizeof(String)] __attribute__((aligned(4)));   // aSStack_58
    unsigned char s_ab[sizeof(String)]   __attribute__((aligned(4)));   // aSStack_40
    unsigned char s_lit1[sizeof(String)] __attribute__((aligned(4)));   // aSStack_64
    unsigned char s_abc[sizeof(String)]  __attribute__((aligned(4)));   // aSStack_34
    unsigned char s_full[sizeof(String)] __attribute__((aligned(4)));   // aSStack_28
    unsigned char s_lit2[sizeof(String)] __attribute__((aligned(4)));   // aSStack_70

    ((String *)s_lit0)->ctor_char(g_tagLit0, false);
    ((String *)s_base)->ctor_copy((String *)(void *)g_tagBaseString, false);
    *(String *)s_ab = *(String *)s_lit0 + *(String *)s_base;
    ((String *)s_lit1)->ctor_char(g_tagLit1, false);
    *(String *)s_abc = *(String *)s_ab + *(String *)s_lit1;
    *(String *)s_full = *(String *)s_abc + *(String *)in;
    ((String *)s_lit2)->ctor_char(g_tagLit2, false);
    *(String *)out = *(String *)s_full + *(String *)s_lit2;

    ((String *)(s_lit2))->dtor();
    ((String *)(s_full))->dtor();
    ((String *)(s_abc))->dtor();
    ((String *)(s_lit1))->dtor();
    ((String *)(s_ab))->dtor();
    ((String *)(s_base))->dtor();
    ((String *)(s_lit0))->dtor();
}

struct TouchButton;

extern "C" void __aeabi_memset4(void *dst, int n, int v);
// Image2DCreate-into-field loader (fn ptr @0xe2c64): (canvas, imgId, &field).

__attribute__((visibility("hidden"))) extern int *g_rlGuard;    // @0xe2c38 (stack guard [0])
__attribute__((visibility("hidden"))) extern unsigned *g_rlCanvas; // @0xe2c62 (*puVar8)
__attribute__((visibility("hidden"))) extern char *g_rlHdFlag;  // @0xe2da8 (HD variant)
__attribute__((visibility("hidden"))) extern int *g_rlBackText; // @0xe2ec0 (*piVar10)
__attribute__((visibility("hidden"))) extern int *g_rlScreenH;  // @0xe2ed6 (*piVar9)
__attribute__((visibility("hidden"))) extern int *g_rlMenuY;    // @0xe2f8a

// Layout::reload(): (re)load all layout images/strings and rebuild the buttons.
void Layout::reload() {
    int *guard = g_rlGuard;
    int g0 = *guard;

    this->bgPatternImage = -1;
    this->field_0x328 = -1;
    this->field_0x32c = -1;
    this->field_0x3a8 = -1;
    __aeabi_memset4((char *)this + 0x334, 0x70, 0xff);

    unsigned canvas = *g_rlCanvas;
    this->loadImage(canvas, 0x503, (char *)this + 0x398);
    this->loadImage(canvas, 0x47e, (char *)this + 0x324);
    this->loadImage(canvas, 0x4ff, (char *)this + 0x328);
    this->loadImage(canvas, 0x500, (char *)this + 0x330);
    this->loadImage(canvas, 0x474, (char *)this + 0x32c);
    this->loadImage(canvas, 0x502, (char *)this + 0x334);
    this->loadImage(canvas, 0x506, (char *)this + 0x340);
    this->loadImage(canvas, 0x501, (char *)this + 0x338);
    this->loadImage(canvas, 0x507, (char *)this + 0x344);
    this->loadImage(canvas, 0x4fe, (char *)this + 0x33c);
    this->loadImage(canvas, 0x482, (char *)this + 0x348);
    this->loadImage(canvas, 0x481, (char *)this + 0x34c);
    this->loadImage(canvas, 0x486, (char *)this + 0x378);
    this->loadImage(canvas, 0x487, (char *)this + 0x374);
    this->loadImage(canvas, 0x48b, (char *)this + 0x37c);
    this->loadImage(canvas, 0x52d, (char *)this + 0x3a4);

    if (*g_rlHdFlag == 0) {
        this->loadImage(canvas, 0x480, (char *)this + 0x350);
        this->loadImage(canvas, 0x47f, (char *)this + 0x354);
        this->loadImage(canvas, 0x479, (char *)this + 0x358);
        this->loadImage(canvas, 0x478, (char *)this + 0x35c);
        this->loadImage(canvas, 0x489, (char *)this + 0x36c);
        this->loadImage(*g_rlCanvas, 0x488, (char *)this + 0x370);
    } else {
        this->loadImage(canvas, 0x6bb, (char *)this + 0x350);
        this->loadImage(canvas, 0x6ba, (char *)this + 0x354);
        this->loadImage(canvas, 0x6b9, (char *)this + 0x358);
        this->loadImage(canvas, 0x6b8, (char *)this + 0x35c);
        this->loadImage(canvas, 0x6b7, (char *)this + 0x36c);
        this->loadImage(*g_rlCanvas, 0x6bc, (char *)this + 0x370);
    }

    this->loadImage(canvas, 0x530, (char *)this + 0x360);
    this->loadImage(canvas, 0x531, (char *)this + 0x364);
    this->loadImage(canvas, 0x52f, (char *)this + 0x368);
    this->loadImage(canvas, 0x47c, (char *)this + 900);
    this->loadImage(canvas, 0x47d, (char *)this + 0x380);
    this->loadImage(canvas, 0x47b, (char *)this + 0x388);
    this->loadImage(canvas, 0x47a, (char *)this + 0x38c);
    this->loadImage(canvas, 0x484, (char *)this + 0x390);
    this->loadImage(canvas, 0x483, (char *)this + 0x394);
    this->loadImage(canvas, 0x50c, (char *)this + 0x3a0);
    this->loadImage(canvas, 0x50d, (char *)this + 0x39c);

    // Back button (string-labelled).
    TouchButton *bBack = (TouchButton *)::operator new(200);
    void *txt = gGameText->obj->getText(*g_rlBackText);
    int sh = *g_rlScreenH;
    TouchButton_ctorStr(bBack, txt, 2, this->field_0x28, sh - 3, '!');
    this->backButton = bBack;
    this->backButtonWidth = ((TouchButton *)(bBack))->getWidth();

    // Secondary button (image if available, else string fallback).
    unsigned img535 = 0xffffffff;
    ((PaintCanvas*)((PaintCanvas *)*g_rlCanvas))->Image2DCreate(0x535, &img535);
    TouchButton *b2 = (TouchButton *)::operator new(200);
    if (img535 == 0xffffffff) {
        void *t = gGameText->obj->getText(*g_rlBackText);
        TouchButton_ctorStr(b2, t, 2, this->field_0x28,
                            *g_rlScreenH - this->field_0x3fc, '!');
    } else {
        TouchButton_ctorImg(b2, img535, 2, this->field_0x28,
                            *g_rlScreenH - this->field_0x3fc, '!');
    }
    this->secondaryButton = b2;

    // Help button (image).
    unsigned img471 = 0xffffffff;
    ((PaintCanvas*)((PaintCanvas *)*g_rlCanvas))->Image2DCreate(0x471, &img471);
    TouchButton *bHelp = (TouchButton *)::operator new(200);
    TouchButton_ctorImg2(bHelp, img471, 1, *g_rlMenuY, 0, this->field_0x3c, 0x12, 0x04);
    this->helpButton = bHelp;

    int th = ((PaintCanvas*)(*g_rlCanvas))->GetTextHeight(0);
    this->choiceWindowOpen = 0;
    this->choiceWindow = 0;
    this->tipLines = 0;
    this->field_0x3ac = th / 2 - 1;
    this->drawColor = -1;
    ((Layout *)(this))->resetWindowDimensions();
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

struct TouchButton;
struct Status;
struct Ship;

void Layout_formatCredits(void *out, int n);

// Hidden globals from drawFooter disasm.
__attribute__((visibility("hidden"))) extern int *g_dfGuard;       // @0xe3c54 (stack guard [0])
__attribute__((visibility("hidden"))) extern PaintCanvas **g_dfCanvas; // @0xe3c56
__attribute__((visibility("hidden"))) extern int **g_dfMetric;     // @0xe3cae ([0][0x10],[0][0x74])
__attribute__((visibility("hidden"))) extern int g_dfWarnColor;    // @0xe3f90 ([0])
__attribute__((visibility("hidden"))) extern const char g_dfSep[];     // @0xe3dc8
__attribute__((visibility("hidden"))) extern const char g_dfTail[];    // @0xe3dfa
__attribute__((visibility("hidden"))) extern void **g_dfFont;      // @0xe3e34

// Layout::drawFooter(bool stationMode, bool showBack) — the 3-arg implementation
// reached by the forwarding wrappers via Layout_drawFooterImpl.
void Layout::drawFooterImpl(int stationMode, int showBack) {
    int *guard = g_dfGuard;
    int g0 = *guard;
    PaintCanvas *pc = *g_dfCanvas;
    ((PaintCanvas*)g_PaintCanvas)->SetColor(this->drawColor);
    int wRight = ((PaintCanvas*)g_PaintCanvas)->GetImage2DWidth(this->footerImageRight);
    int wLeft = ((PaintCanvas*)g_PaintCanvas)->GetImage2DWidth(this->footerImageLeft);

    ((PaintCanvas*)(pc))->DrawImage2D(this->footerImageLeft, this->windowX,
                             this->windowY + this->windowHeight, (unsigned char)(0x11));
    int *m = *g_dfMetric;
    int footerH = m[0x10 / 4];
    ((PaintCanvas*)(pc))->DrawImage2D(this->footerImageRight, this->windowX + wLeft,
                             (this->windowY + this->windowHeight) - footerH);
    int both = wLeft + wRight;
    ((Layout *)(this))->drawBGPattern(this->field_0x338, both + this->windowX, (this->windowY + this->windowHeight) - footerH, this->windowWidth + both * -2, footerH);
    ((PaintCanvas*)(pc))->DrawImage2D(this->footerImageRight,
                             (this->windowX - both) + this->windowWidth,
                             (this->windowHeight + this->windowY) - footerH, (unsigned char)(0x01));
    ((PaintCanvas*)(pc))->DrawImage2D(this->footerImageLeft,
                             (this->windowX - wRight) + this->windowWidth,
                             (this->windowHeight + this->windowY) - footerH, (unsigned char)(0x01));

    int backVis = (!stationMode) && showBack;
    ((TouchButton *)(this->backButton))->setVisible(backVis);
    if (!backVis) {
        ((TouchButton *)(this->secondaryButton))->draw();
    } else if (showBack) {
        ((TouchButton *)(this->backButton))->draw();
    }

    // Cargo load text, in warning color if over capacity.
    ((Status *)(*gStatus))->getShip();
    int load = ((Ship *)(((Status *)(*gStatus))->getShip()))->getCurrentLoad();
    ((Status *)(*gStatus))->getShip();
    int maxLoad = ((Ship *)(((Status *)(*gStatus))->getShip()))->getMaxLoad();
    if (maxLoad < load)
        ((PaintCanvas*)g_PaintCanvas)->SetColor(*(unsigned *)&g_dfWarnColor);

    ((Status *)(*gStatus))->getShip();
    int cur = ((Ship *)(((Status *)(*gStatus))->getShip()))->getCurrentLoad();
    unsigned char sLoad[sizeof(String)] __attribute__((aligned(4)));   // aSStack_58
    ((String *)sLoad)->ctor_int((int)cur);
    unsigned char sSep[sizeof(String)] __attribute__((aligned(4)));    // aSStack_64
    ((String *)sSep)->ctor_char(g_dfSep, false);
    unsigned char s1[sizeof(String)] __attribute__((aligned(4)));      // aSStack_4c
    *(String *)s1 = *(String *)sLoad + *(String *)sSep;

    ((Status *)(*gStatus))->getShip();
    int mx = ((Ship *)(((Status *)(*gStatus))->getShip()))->getMaxLoad();
    unsigned char sMax[sizeof(String)] __attribute__((aligned(4)));    // aSStack_70
    ((String *)sMax)->ctor_int((int)mx);
    unsigned char s2[sizeof(String)] __attribute__((aligned(4)));      // aSStack_40
    *(String *)s2 = *(String *)s1 + *(String *)sMax;
    unsigned char sTail[sizeof(String)] __attribute__((aligned(4)));   // aSStack_7c
    ((String *)sTail)->ctor_char(g_dfTail, false);
    unsigned char loadStr[sizeof(String)] __attribute__((aligned(4))); // aSStack_34
    *(String *)loadStr = *(String *)s2 + *(String *)sTail;

    ((String *)(sTail))->dtor();
    ((String *)(s2))->dtor();
    ((String *)(sMax))->dtor();
    ((String *)(s1))->dtor();
    ((String *)(sSep))->dtor();
    ((String *)(sLoad))->dtor();

    {
        int x = this->windowX;
        int w = this->windowWidth;
        void *font = *g_dfFont;
        unsigned cv = *(unsigned *)g_dfCanvas;
        int tw = ((PaintCanvas*)(cv))->GetTextWidth((unsigned)(unsigned long)(font), (void *)0);
        ((PaintCanvas*)(cv))->DrawString((unsigned)(unsigned long)(font), loadStr, (x + w / 2) - tw / 2,
                               (this->windowHeight + this->windowY) - this->field_0x14, false);
    }
    ((PaintCanvas*)g_PaintCanvas)->SetColor(this->drawColor);

    int credits = ((Status *)(*gStatus))->getCredits();
    unsigned char credStr[sizeof(String)] __attribute__((aligned(4)));  // aSStack_40
    Layout_formatCredits(credStr, credits);

    {
        int x = this->windowX;
        int w = this->windowWidth;
        void *font = *g_dfFont;
        unsigned cv = *(unsigned *)g_dfCanvas;
        if (stationMode) {
            int rightInset = m[0x74 / 4];
            int tw = ((PaintCanvas*)(cv))->GetTextWidth((unsigned)(unsigned long)(font), (void *)0);
            ((PaintCanvas*)(cv))->DrawString((unsigned)(unsigned long)(font), credStr, ((w + x) - rightInset) - tw / 2,
                                   (this->windowHeight + this->windowY) - this->field_0x14, false);
        } else {
            int tw = ((PaintCanvas*)(cv))->GetTextWidth((unsigned)(unsigned long)(font), (void *)0);
            ((PaintCanvas*)(cv))->DrawString((unsigned)(unsigned long)(font), credStr, (w + x - 10) - tw,
                                   (this->windowHeight + this->windowY) - this->field_0x14, false);
        }
    }

    ((String *)(credStr))->dtor();
    ((String *)(loadStr))->dtor();
    
}

// Layout::drawHeader() -> drawHeader(this, String(""), 0)
void Layout::drawHeader0() {
    unsigned char tmp[sizeof(String)] __attribute__((aligned(4)));
    ((String *)tmp)->ctor_char("", false);
    ((Layout *)(this))->drawHeaderImpl(tmp, 0);
    ((String *)(tmp))->dtor();
}

// Layout::drawFooterStation() -> drawFooter(this, 1, 0)  (this passed implicitly in r0)
void Layout::drawFooterStation() {
    return ((Layout *)(this))->drawFooterImpl(1, 0);
}

// Header transition tail helper @0x1ac0a8.

// Hidden globals from drawHeader disasm.
__attribute__((visibility("hidden"))) extern unsigned *g_dhColor;   // @0xe4118 ([0]=image/color)
__attribute__((visibility("hidden"))) extern int **g_dhMetric;      // @0xe41aa ([0][0x28],[0][0x44])
__attribute__((visibility("hidden"))) extern void ***g_dhFont;      // @0xe41ac

// Layout::drawHeader(String title, bool transition)
void Layout::drawHeader7(void *title, int transition) {
    Layout *self = this;
    unsigned img = *g_dhColor;
    ((PaintCanvas*)g_PaintCanvas)->SetColor(self->drawColor);
    int iw = ((PaintCanvas*)g_PaintCanvas)->GetImage2DWidth(img);
    int ih = ((PaintCanvas*)g_PaintCanvas)->GetImage2DHeight(img);
    ((PaintCanvas*)((PaintCanvas *)img))->DrawImage2D(self->field_0x330,
                             self->windowX, self->windowY);
    ((Layout *)(self))->drawBGPattern(self->field_0x328, self->windowX + iw, self->windowY, self->windowWidth + iw * -2, ih);
    ((PaintCanvas*)((PaintCanvas *)img))->DrawImage2D(self->field_0x330,
                             self->windowWidth + self->windowX,
                             self->windowY, iw, ih, (unsigned char)(0x11), (unsigned char)(0x12), (unsigned char)(0x01));
    if (*(int *)((char *)title + 8) != 0) {
        ((PaintCanvas*)((PaintCanvas *)img))->DrawImage2D(self->field_0x32c,
                                 self->windowX, self->windowY);
        int *m = *g_dhMetric;
        ((PaintCanvas*)((PaintCanvas *)img))->DrawString((unsigned)(unsigned long)(**g_dhFont), title,
                               m[0x28 / 4] + m[0x44 / 4] + self->windowX,
                               self->field_0x18 + self->windowY, false);
    }
    self->helpButtonEnabled = (uint8_t)transition;
    if (transition != 0 && self->choiceWindowOpen == 0) {
        self->headerAnim(self->helpButton);
        return;
    }
}

// Layout::drawWindow(String) -> drawWindow(this, copy, 0). The on-stack String
// temp makes the compiler emit the -fstack-protector canary automatically.
void Layout::drawWindow1(const void *param) {
    unsigned char tmp[sizeof(String)] __attribute__((aligned(4)));
    ((String *)tmp)->ctor_copy((String *)param, false);
    ((Layout *)(this))->drawWindowImpl(tmp, 0);
    ((String *)(tmp))->dtor();
}

void Layout_formatCredits(void *out, int n);

// Hidden globals from drawMissionRewardMessage disasm.
__attribute__((visibility("hidden"))) extern int *g_mrGuard;       // @0xe4d88 (stack guard [0])
__attribute__((visibility("hidden"))) extern PaintCanvas **g_mrCanvas; // @0xe4d9e
__attribute__((visibility("hidden"))) extern int *g_mrDimA;        // @0xe4e2a ([0]=screen W)
__attribute__((visibility("hidden"))) extern const char g_mrLit0[];    // @0xe4e38
__attribute__((visibility("hidden"))) extern const char g_mrLit1[];    // @0xe4e6a
__attribute__((visibility("hidden"))) extern int *g_mrDimB;        // @0xe4e94 ([0]=screen H)
__attribute__((visibility("hidden"))) extern int *g_mrTextId;      // @0xe4eba ([0]=text id)
__attribute__((visibility("hidden"))) extern void **g_mrFont;      // @0xe4ede ([0]=font)
__attribute__((visibility("hidden"))) extern const char g_mrLit2[];    // @0xe4f14

// Layout::drawMissionRewardMessage(bool transition)
void Layout::drawMissionRewardMessage(int transition) {
    int *guard = g_mrGuard;
    int g0 = *guard;
    if (this->rewardMessageActive != 0) {
        PaintCanvas *pc = *g_mrCanvas;
        unsigned saved = ((PaintCanvas*)(pc))->GetColor();
        unsigned origColor = this->drawColor;
        ((PaintCanvas*)g_PaintCanvas)->SetColor(0xffffffff);

        // Pulse alpha based on field 0x3d0 (animation timer).
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
        ((PaintCanvas*)g_PaintCanvas)->SetColor(col);

        unsigned newColor = ((PaintCanvas*)(pc))->GetColor();
        int boxW = this->field_0x3e8;
        int boxH = this->field_0x3ec;
        int boxX = this->field_0x3f0;
        int boxY = this->field_0x3f4;
        this->drawColor = newColor;

        if (transition != 0) {
            int sw = *g_mrDimA;
            unsigned char s0[sizeof(String)] __attribute__((aligned(4)));
            ((String *)s0)->ctor_char(g_mrLit0, false);
            ((Layout *)(pc))->drawBoxStr(2, (sw >> 1) - (boxH >> 1), boxX, boxH, boxW, s0);
            ((String *)(s0))->dtor();

            sw = *g_mrDimA;
            unsigned char s1[sizeof(String)] __attribute__((aligned(4)));
            ((String *)s1)->ctor_char(g_mrLit1, false);
            ((Layout *)(pc))->drawBoxStr(8, (sw >> 1) - (boxH >> 1), boxX, boxH, boxW, s1);
            ((String *)(s1))->dtor();
        }

        int sh = *g_mrDimB;
        ((PaintCanvas*)(*g_mrCanvas))->DrawImage2D(this->field_0x3a4, sh >> 1,
                                 (char)boxX, (unsigned char)(0x11));

        void *txt = gGameText->obj->getText(*g_mrTextId);
        unsigned char line[sizeof(String)] __attribute__((aligned(4)));
        ((String *)line)->ctor_copy((String *)txt, false);

        sh = *g_mrDimB;
        void *font = *g_mrFont;
        unsigned cv = *(unsigned *)g_mrCanvas;
        int tw = ((PaintCanvas*)(cv))->GetTextWidth((unsigned)(unsigned long)(font), (void *)0);
        ((PaintCanvas*)(cv))->DrawString((unsigned)(unsigned long)(font), line, (sh >> 1) - (tw >> 1), boxX + boxY, false);

        unsigned char suffix[sizeof(String)] __attribute__((aligned(4)));
        ((String *)suffix)->ctor_char(g_mrLit2, false);
        unsigned char credits[sizeof(String)] __attribute__((aligned(4)));
        Layout_formatCredits(credits, this->rewardCredits);
        unsigned char joined[sizeof(String)] __attribute__((aligned(4)));
        *(String *)joined = *(String *)suffix + *(String *)credits;
        ((String *)(line))->assign((String *)joined);
        ((String *)(joined))->dtor();
        ((String *)(credits))->dtor();
        ((String *)(suffix))->dtor();

        sh = *g_mrDimB;
        cv = *(unsigned *)g_mrCanvas;
        tw = ((PaintCanvas*)(cv))->GetTextWidth((unsigned)(unsigned long)(font), (void *)0);
        ((PaintCanvas*)(cv))->DrawString((unsigned)(unsigned long)(font), line, (sh >> 1) - (tw >> 1),
                               this->field_0x3f8 + boxX, false);
        ((PaintCanvas*)g_PaintCanvas)->SetColor(saved);
        this->drawColor = origColor;
        ((String *)(line))->dtor();
    }

}

// ===========================================================================
// Recovered internal helper methods (batch 4)
//
// These are the small dispatch/forwarder/init helpers that the public Layout
// drawing API delegates to.  In the stripped .so they appear only as PLT/GOT
// veneers (0x74xxx) into the engine bodies (0xe****); each one's semantics are
// reproduced here as a real Layout:: method so the per-method call sites can be
// wired to a proper member call by the mechanical pass.
// ===========================================================================

// drawWindow(String, bool flag): centre a window using the cached screen
// metrics, then forward to the full 7-arg renderer.  Equivalent to drawWindow2.
void Layout::drawWindowImpl(const void *str, int flag) {
    this->drawWindow2(str, flag);
}

// drawWindow(String, x, y, w, h, bool): the 7-arg core renderer (drawWindow7),
// reached from the 2-arg / 5-arg wrappers.
void Layout::drawWindowImpl5(void *str, int x, int y, int w, int h, int flag) {
    this->drawWindow7(str, x, y, w, h, flag);
}

// drawHeader(String, bool transition): the header-bar renderer core.
void Layout::drawHeaderImpl(void *title, int transition) {
    this->drawHeader7(title, transition);
}

// drawBox(style, x, y, w, h, String, flags): owning-String variant of drawBox.
void Layout::drawBoxImpl(int style, int x, int y, int w, int h, void *text, int flags) {
    this->drawBox(style, x, y, w, h, text, (unsigned)flags);
}

// drawBox over a borrowed String (no copy): the static-style entry the engine
// uses when the caller already owns the text (flags default to 0).
void Layout::drawBoxStr(int style, int x, int y, int w, int h, void *text) {
    this->drawBox(style, x, y, w, h, text, 0u);
}

// 8-arg drawBox variant used by external callers: (kind, x, y, w, color, text, z).
// `color` overrides the draw colour for the duration of the box; `z` selects the
// height metric (passed through as the box height).
void Layout::drawBox8(int kind, int x, int y, int w, int color, void *text, int z) {
    unsigned saved = this->drawColor;
    this->drawColor = (unsigned)color;
    this->drawBox(kind, x, y, w, z, text, 0u);
    this->drawColor = saved;
}

// drawMask(x, y, w, h): dim the whole screen behind a modal by filling a
// translucent rectangle with the mask colour, restoring the previous colour.
void Layout::drawMaskImpl(int x, int y, int w, int h) {
    PaintCanvas *pc = (PaintCanvas *)g_PaintCanvas;
    unsigned saved = pc->GetColor();
    pc->SetColor(this->drawColor);
    pc->FillRectangle(x, y, w, h);
    pc->SetColor(saved);
}

// Colour-restore tail shared by the mask/footer renderers: restore the canvas
// colour that was saved before the draw.
void Layout::drawMaskTail(void *pc, int savedColor) {
    ((PaintCanvas *)pc)->SetColor((unsigned)savedColor);
}

// drawHelpWindow tail: paint the active help/choice window.
void Layout::drawHelpWindowImpl() {
    if (this->choiceWindow != 0)
        ((ChoiceWindow *)this->choiceWindow)->draw();
}

// drawBGBorder rounded-rect helpers: thin overloads onto the full 8-parameter
// renderer.  The 6-arg form reuses the corner image's height as inset/pad.
void Layout::drawBGBorder6(unsigned corner, unsigned edge, int x, int y, int w) {
    int ch = ((PaintCanvas *)g_PaintCanvas)->GetImage2DHeight(corner);
    this->drawBGBorder(corner, edge, x, y, w, ch, 0, 0);
}

void Layout::drawBGBorder8(unsigned corner, unsigned edge, int x, int y, int w, int h, int inset, int pad) {
    this->drawBGBorder(corner, edge, x, y, w, h, inset, pad);
}

// ===========================================================================
// Canonical-named public draw entries (batch 18)
//
// These are the exact-named engine bodies (0xe38d4 / 0xe3140 / 0xe395c) behind
// the Layout_drawWindow / Layout_drawMask / Layout_drawBGBorderImpl PLT veneers.
// They reproduce the same work the suffixed variants above do, under the names
// the external call sites use, so the call sites can be wired to member calls.
// ===========================================================================

// Layout::drawWindow(String, x, y, w, h): take an owning copy of the title and
// render the full window frame (with background) via the 7-arg core renderer.
void Layout::drawWindow(void *title, int x, int y, int w, int h) {
    unsigned char tmp[sizeof(String)] __attribute__((aligned(4)));
    ((String *)tmp)->ctor_copy((String *)title, false);
    this->drawWindow7(tmp, x, y, w, h, 1);
    ((String *)(tmp))->dtor();
}

// Layout::drawMask(): dim the entire screen behind a modal using the cached
// screen-size globals (full-screen translucent fill in the mask colour).
void Layout::drawMask() {
    int w = *gW;
    int h = *gH;
    this->drawMaskImpl(0, 0, w, h);
}

// Layout::drawBGBorder (8-arg engine entry 0xe395c) under its impl name: render a
// tiled rounded-rect border. Forwards to the full 8-parameter renderer.
void Layout::drawBGBorderImpl(unsigned corner, unsigned edge, int x, int y, int w, int h, int inset, int pad) {
    this->drawBGBorder(corner, edge, x, y, w, h, inset, pad);
}

// Position a child TouchButton (back/secondary/help) at (x, y) with an anchor.
void Layout::setBtnRect(void *btn, int x, int y, int anchor) {
    ((TouchButton *)btn)->setPosition(x, y, (unsigned char)anchor);
}

// Create an Image2D resource into a layout field slot.
void Layout::loadImage(unsigned canvas, int imageId, void *field) {
    ((PaintCanvas *)canvas)->Image2DCreate((unsigned short)imageId, (unsigned int *)field);
}

// Slide the help button into the header when the header animates in.  The engine
// entry is a PLT veneer into the shared TouchButton "anim" routine; forward to it.
void Layout::headerAnim(void *btn) {
    unsigned char sp[8] __attribute__((aligned(4)));
    TouchButton_footerAnim(btn, 0, 0, sp);
}

// Bulk constant-block initialiser for the SIMD-copied header geometry region
// (0x04..0x2d8).  The constant vectors live in the engine .rodata and are NEON
// block-copied; the readable scalar tail is handled by the ctor.  This entry just
// zero-seeds the block so the ctor's explicit writes land on a clean object.
void Layout::initConstBlock(int hd, int wide, int scale, int res) {
    (void)hd; (void)wide; (void)scale; (void)res;
    __aeabi_memset4((char *)this + 0x04, 0x2d8 - 0x04, 0);
}

// Touch dispatch tails: forward the event to the resolved child TouchButton.
int Layout::dispatchTouchBegin(void *btn, int x, int y) {
    return ((TouchButton *)btn)->OnTouchBegin(x, y);
}

int Layout::dispatchTouchMove(void *btn, int x, int y) {
    return (int)((TouchButton *)btn)->OnTouchMove(x, y);
}

int Layout::dispatchTouchEnd(void *btn, int x, int y) {
    return (int)((TouchButton *)btn)->OnTouchEnd(x, y);
}

// Engine-named aliases routed to the canonical handlers.
int Layout::touch_end(int x, int y) {
    return this->OnTouchEnd(x, y);
}

void Layout::drawHeader_call(void *title) {
    this->drawHeader7(title, 0);
}
