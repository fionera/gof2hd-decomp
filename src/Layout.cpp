#include "gof2/Layout.h"
#include "gof2/ChoiceWindow.h"
#include "gof2/FModSound.h"
#include "gof2/Status.h"
#include "gof2/GameText.h"
#include "gof2/String.h"
#include "gof2/TouchButton.h"


extern "C" float __aeabi_l2f(long long);
extern "C" void *ChoiceWindow_dtor(void *p);
extern "C" void operator_delete_li(void *p);
extern "C" void *operator_new(unsigned sz);
extern "C" void operator_delete(void *p);
extern "C" void Layout_initConstBlock(Layout *self, int hd, int wide, int scale, int res);
extern "C" void Layout_setBtnRect(void *btn, int x, int y, int anchor);
extern "C" void Layout_loadImage(unsigned canvas, int id, void *field);
extern "C" void Layout_headerAnim(void *btn);

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
struct PaintCanvas;
struct TouchButton;
extern "C" {
int  PaintCanvas_GetImage2DWidth(...);
int  PaintCanvas_GetImage2DHeight(...);
unsigned PaintCanvas_GetColor(...);
void PaintCanvas_SetColor(...);
void PaintCanvas_restoreColor(...);
void PaintCanvas_FillRectangle(...);
void PaintCanvas_DrawRectangle(...);
void PaintCanvas_DrawImage2D3(...);
void PaintCanvas_DrawImage2D5(...);
void PaintCanvas_DrawImage2D9(...);
void PaintCanvas_DrawString(...);
void PaintCanvas_DrawRegion2D(...);
int  PaintCanvas_GetTextWidth(...);
int  PaintCanvas_GetTextHeight(...);
void PaintCanvas_Image2DCreate(...);
void PaintCanvas_GetLineArray(...);

void    TouchButton_footerAnim(...);
void    TouchButton_getPosition(...);
void    TouchButton_ctorStr(...);
void    TouchButton_ctorImg(...);
void    TouchButton_ctorImg2(...);

void *ChoiceWindow_ctor(...);


int  Ship_getCurrentLoad(...);
int  Ship_getMaxLoad(...);

void String_cstr_ctor(...);
void String_copy_ctor(...);
void String_concat(...);
void String_from_uint(...);
void String_default_ctor(...);
void String_subString(...);
void String_copy_global(...);
int  AbyssString_Compare(...);
unsigned short GameText_getLanguage(...);

void Globals_drawLines(...);
int  AERandom_nextInt(...);
}

// ---- isFading_d514c.cpp ----
// ldrb.w r0,[r0,#0x400]; bx lr
uint8_t Layout::isFading() {
    Layout *self = this;
    return self->fading;
}

// ---- getHelpButtonOffset_d4bd0.cpp ----
__attribute__((visibility("hidden"))) extern int **gTouchButtonHolder;  // ldr [0xe4bec]

// Layout::getHelpButtonOffset() -> TouchButton::getWidth(self->f3bc) - (**holder)[0x38/4]
int Layout::getHelpButtonOffset() {
    Layout *self = this;
    int w = ((TouchButton *)(self->helpButton))->getWidth();
    return w - (*gTouchButtonHolder)[0x38 / 4];
}

// ---- update_d5152.cpp ----

// Layout::update(int dt)
void Layout::update(int dt) {
    Layout *self = this;
    void *cw = self->choiceWindow;
    if (cw)
        ((ChoiceWindow *)(cw))->update(dt);
    if (self->rewardMessageActive != 0) {
        int t = self->rewardMessageTimer + dt;
        self->rewardMessageTimer = t;
        if (t >= 6999 + 1)        // 0x1b58 = 7000; cmp r0,#7000; mov.ge
            self->rewardMessageActive = 0;
    }
    if (self->fading != 0) {
        int t = self->fadeProgress + dt;
        int dur = self->fadeDuration;
        self->fadeProgress = t;
        if (t > dur) {
            self->fading = 0;
            self->fadeProgress = 0;
        }
    }
}

// ---- getFooterTransitionWidth_d3c08.cpp ----
// PC-relative hidden global: pointer to the PaintCanvas singleton holder.
__attribute__((visibility("hidden"))) extern void **gPaintCanvasHolder; // ldr [0xe3c38]

// Layout::getFooterTransitionWidth()
int Layout::getFooterTransitionWidth() {
    Layout *self = this;
    void **holder = gPaintCanvasHolder;
    int w1 = PaintCanvas_GetImage2DWidth(*holder, self->footerImageRight);
    int w2 = PaintCanvas_GetImage2DWidth(*holder, self->footerImageLeft);
    return w2 + w1;
}

// ---- OnTouchBegin_d51a8.cpp ----
extern "C" int Layout_dispatchTouchBegin(void *btn, int x, int y);     // tail 0x1ac0c8

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
    return Layout_dispatchTouchBegin(btn, x, y);
}

// ---- getPulseValue_d4bf0.cpp ----
// Status singleton (PC-rel double-indirect). getPlayingTime returns a 64-bit value
// (r0/r1), converted to float via __aeabi_l2f, scaled by the arg, fed to Sinf.
struct Status;
__attribute__((visibility("hidden"))) extern Status **gStatus;  // ldr [0xe4c5c]
float Sinf(float);   // AbyssEngine::AEMath::Sinf -> 0x6f1a8

// Layout::getPulseValue(float speed). The holder (gStatus's value) is cached; the
// actual Status* (*holder) is reloaded for each getPlayingTime call.
float Layout::getPulseValue(float speed) {
    Layout *self = this;
    Status **holder = gStatus;
    float a = Sinf(__aeabi_l2f(((Status *)(*holder))->getPlayingTime()) * speed);
    float b = Sinf(__aeabi_l2f(((Status *)(*holder))->getPlayingTime()) * speed);
    return a > 0.0f ? b : -b;
}

// ---- setDrawColor_d3138.cpp ----
// str.w r1,[r0,#0x3b0]; bx lr
void Layout::setDrawColor(int color) {
    Layout *self = this;
    self->drawColor = color;
}

// ---- drawMask_d3140.cpp ----
__attribute__((visibility("hidden"))) extern int *gW;  // ldr [0xe3160]
__attribute__((visibility("hidden"))) extern int *gH;  // ldr [0xe3164]
extern "C" void Layout_drawMaskImpl(void *pc, int a, int b, int w, int h);  // 0x74ddc

// Layout::drawMask() -> drawMask(*gH, 0, 0, *gW, *gH)
void Layout_drawMask0() {
    void *pc = (void *)*gH;
    int w = *gW;
    Layout_drawMaskImpl(pc, 0, 0, w, *gH);
}

// ---- drawBG_d3544.cpp ----
__attribute__((visibility("hidden"))) extern int *gA;  // ldr [0xe356c]
__attribute__((visibility("hidden"))) extern int *gB;  // ldr [0xe3570]
// 0x74e00

// Layout::drawBG() -> drawBGPattern(this, this->0x324, 0, 0, *gA, *gB)
void Layout::drawBG() {
    Layout *self = this;
    int p4 = *gA;
    int p5 = *gB;
    ((Layout *)(self))->drawBGPattern(self->bgPatternImage, 0, 0, p4, p5);
}

// ---- enableFillScreen_d5032.cpp ----
// strb.w r1,[r0,#0x410]; bx lr
void Layout::enableFillScreen(bool v) {
    Layout *self = this;
    self->fillScreen = v;
}

// ---- _Layout_d2bc8.cpp ----
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

// ---- helpPressed_d4bc8.cpp ----
// ldrb.w r0,[r0,#0x3c0]; bx lr
uint8_t Layout::helpPressed() {
    Layout *self = this;
    return self->helpPressedFlag;
}

// ---- drawFooterNoBackButton_d4090.cpp ----
// Layout::drawFooterNoBackButton() -> drawFooter(this, 0, 0)
void Layout::drawFooterNoBackButton() {
    Layout *self = this;
    return ((Layout *)(self))->drawFooterImpl(0, 0);
}

// ---- drawFooter_d3fb4.cpp ----
// -> 0x1ac098

// Layout::drawFooter() -> drawFooter(this, 0, 1)
void Layout::drawFooter() {
    Layout *self = this;
    return ((Layout *)(self))->drawFooterImpl(0, 1);
}

// ---- OnTouchMove_d5206.cpp ----
extern "C" int Layout_dispatchTouchMove(void *btn, int x, int y);      // tail 0x1ac0d8

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
    return Layout_dispatchTouchMove(btn, x, y);
}

// ---- drawBGBorder_d393c.cpp ----
// 9-arg impl. Forwarded: (p5 as self, p2, p3, p4, p5, p6, instack, 0, 0).
extern "C" void Layout_drawBGBorderImpl(int self, unsigned p2, int p3, int p4,
                                        int p5, int p6, int s8, int z1, int z2); // 0x74e30

// Layout::drawBGBorder(uint p1, uint p2, int p3, int p4, int p5, int p6) + stack arg s8.
// This 7-arg forwarding wrapper (engine 0x74e30) is distinct from the 6-arg
// Layout::drawBGBorder method (0x74e78) used by drawBox; renamed to avoid the
// extern "C" name clash.
void Layout_drawBGBorder7(unsigned p1, unsigned p2, int p3, int p4,
                                     int p5, int p6, int s8) {
    Layout_drawBGBorderImpl(p5, p2, p3, p4, p5, p6, s8, 0, 0);
}

// ---- OnTouchEnd_d5264.cpp ----
extern "C" int Layout_dispatchTouchEnd(void *btn, int x, int y);       // tail 0x1ac0e8

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
    return Layout_dispatchTouchEnd(btn, x, y);
}

// ---- startFade_d5014.cpp ----
// Layout::startFade(bool, int, int)
void Layout::startFade(uint8_t fadeOut, int color, int duration) {
    Layout *self = this;
    self->fadeOut = fadeOut;
    self->fading = 1;
    self->fadeColor = color & 0xffffff00u;
    self->fadeProgress = 0;
    self->fadeDuration = duration;
}

// ---- initTip_d495c.cpp ----
struct PaintCanvas;

extern "C" void ArrayReleaseClasses_StringPtr(void *arr);    // 0x6facc
extern "C" void *ArrayStringPtr_dtor(void *arr);             // 0x6f64c
extern "C" void ArrayStringPtr_ctor(void *arr);             // 0x6f628

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
    Layout *self = this;
    void **tipArr = (void **)((char *)self + 0x3c8);
    if (*tipArr != 0) {
        ArrayReleaseClasses_StringPtr(*tipArr);
        if (*tipArr != 0)
            operator_delete(ArrayStringPtr_dtor(*tipArr));
        *tipArr = 0;
    }
    void *arr = operator_new(0xc);
    ArrayStringPtr_ctor(arr);

    unsigned color = *g_tipColor;
    int textId = **g_tipTextId;
    PaintCanvas *canvas = **g_tipCanvas;
    AERandom_nextInt(**g_tipRandN);
    void *str = gGameText->obj->getText(textId);

    *tipArr = arr;
    int *m = *g_tipMetric;
    int width = m[0x78 / 4] + m[0x4c / 4] * -2;
    PaintCanvas_GetLineArray(canvas, color, str, width, *tipArr);
}

// ---- drawEmptyFooter_d3fbc.cpp ----
struct PaintCanvas;
struct TouchButton;

extern "C" void PaintCanvas_DrawImage2D(PaintCanvas *pc, unsigned img, int x,
                                        int y, int anchor);                 // 0x71d70
// 0x74e00
// Footer "scroll into place" tail-called helper at 0x1ac0a8.

// Hidden globals from drawEmptyFooter disasm.
__attribute__((visibility("hidden"))) extern unsigned *g_efColor;   // @0xe3fd2 ([0]=color)
__attribute__((visibility("hidden"))) extern int *g_efScreenH;      // @0xe3ff8 ([0]=screen height)
__attribute__((visibility("hidden"))) extern int *g_efScreenW;      // @0xe4016 ([0]=screen width)
__attribute__((visibility("hidden"))) extern int **g_efMetric;      // @0xe401c ([0][0x10]=footer height)

// Layout::drawEmptyFooter(bool showBack)
void Layout::drawEmptyFooter(int showBack) {
    Layout *self = this;
    unsigned color = *g_efColor;
    PaintCanvas_SetColor(color);
    int w = PaintCanvas_GetImage2DWidth(color);
    int screenH = *g_efScreenH;
    PaintCanvas_DrawImage2D((PaintCanvas *)color, self->footerImageLeft, 0, screenH, 0x11);

    int screenW = *g_efScreenW;
    int footerH = (*g_efMetric)[0x10 / 4];
    ((Layout *)(self))->drawBGPattern(self->footerPatternImage, w, screenH - footerH, screenW - w * 2, footerH);
    PaintCanvas_DrawImage2D((PaintCanvas *)color, self->footerImageLeft,
                            screenW - w, screenH - (*g_efMetric)[0x10 / 4], 0x01);
    if (showBack == 0) return;
    ((TouchButton *)(self->backButton))->setVisible(1);
    unsigned char sp[8] __attribute__((aligned(4)));
    TouchButton_footerAnim(self->backButton, 1, footerH, sp);
}

// ---- Layout_d1028.cpp ----
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
// that bulk constant fill is delegated to Layout_initConstBlock (its data lives
// in the engine .rodata).  The readable tail field writes (0x1f0..0x320 and
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

void Layout::ctor() {
    Layout *self = this;
    int hd    = (*g_cfgHd != 0);
    int wide  = (*g_cfgWide != 0);
    int scale = (*g_cfgScale != 0);
    int res   = *g_resByte;

    // Header / geometry constants (NEON block-copied in the target).
    Layout_initConstBlock(self, hd, wide, scale, res);

    // ---- Readable tail (lines 1480..1740 of the decompile) ----
    if (!hd) {
        // The non-HD branch fills the 0x204..0x274 doubleword group from
        // constant vectors and seeds a few scalars; modeled as the engine
        // const-block plus the explicit scalar writes below.
        self->field_0x238 = 0x46;
        self->field_0x27c = 0x1c;
        self->field_0x280 = 0;
        self->field_0x3e0 = 1;
        self->field_0x3e4 = 0;
        self->field_0x3e8 = 0x8c;     // 0x3e8
        self->field_0x3f4 = 0x4d;
        self->field_0x3f8 = 0x84;
    }

    // 0x3ec / 0x3f0 reward-box dimensions.
    if (hd) {
        self->field_0x3ec = (res == 0) ? 0xc4 : 0x188;
        self->field_0x3f0 = (res == 0) ? 0x32 : 100;
    } else {
        self->field_0x3ec = 0x160;
        self->field_0x3f0 = 0x4b;
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
    self->field_0x2bc = v2bc;
    self->field_0x3fc = v3fc_;
    self->field_0x200 = v200;
    self->field_0x1f0 = v1f0;
    self->field_0x1f4 = v1f4;
    self->field_0x1f8 = v1f8;
    self->field_0x284 = 1;            // bVar65^1 collapses to a config bit
    self->field_0x285 = 1;
    self->field_0x286 = 1;
    self->field_0x288 = v288;
    self->field_0x28c = v28c;
    self->field_0x290 = wide ? (res == 0 ? 3 : 7) : 4;

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
        self->field_0x2f4 = cw2f4;
        self->field_0x2f8 = 0;    // engine const (DAT)
        self->field_0x2fc = (*g_cfgWide != 0) ? 0x48 : 0x24;
        self->field_0x300 = (*g_cfgWide != 0) ? 400 : 200;
        self->field_0x304 = (*g_cfgWide != 0) ? 700 : 0x15e;
        cwTitle = (*g_cfgWide != 0) ? 500 : 0xfa;
    } else {
        self->field_0x2f4 = 0x70;
        self->field_0x2f8 = 0;
        self->field_0x2fc = 0x32;
        self->field_0x300 = 0x119;
        self->field_0x304 = 0x1ec;
        cwTitle = 0x15f;
    }
    self->field_0x308 = cwTitle;

    // Choice-button offsets group (0x30c..0x318).
    int b318;
    if (!hd) {
        if (*g_cfgC == 0) {
            if (wide) {
                self->field_0x30c = 7;
                self->field_0x310 = (res == 0) ? 0xfffffffb : 0xfffffff7;
                self->field_0x314 = (res == 0) ? 0xfffffffb : 0xfffffff8;
            } else {
                self->field_0x30c = 4;
                self->field_0x310 = 0xfffffff8;
                self->field_0x314 = 0xfffffff8;
            }
            b318 = 0;
        } else {
            self->field_0x30c = 3;
            self->field_0x310 = 0xfffffff4;
            self->field_0x314 = 0xfffffffb;
            b318 = 4;
        }
    } else {
        self->field_0x30c = 6;
        self->field_0x310 = 0xffffffe8;
        self->field_0x314 = 0xfffffff6;
        b318 = 8;
    }
    self->field_0x318 = b318;

    // Help-window position (0x31c / 800).
    if (!hd) {
        if (wide) {
            self->field_0x31c = (*g_cfgD != 0) ? 0x76 : 0xec;
            self->field_0x320 = 0;    // engine float const (DAT_000e2bb8..)
        } else {
            self->field_0x31c = 0x1d8;
            self->field_0x320 = 0x120;
        }
    } else {
        self->field_0x31c = 0x14b;
        self->field_0x320 = 0xca;
    }
}

// ---- drawMask_d3168.cpp ----
__attribute__((visibility("hidden"))) extern void **gPC;          // ldr [0xe31ac]
extern "C" int Layout_drawMaskTail(void *pc, int color, int p4, void *st); // tail 0x1ac088

// Layout::drawMask(int p1, int p2, int p3, int p4) — singleton reloaded each call.
int Layout::drawMask4(int p1, int p2, int p3, int p4) {
    Layout *self = this;
    int saved = PaintCanvas_GetColor(*gPC);
    PaintCanvas_SetColor(*gPC, 0x80);
    PaintCanvas_FillRectangle(*gPC, p1, p2, p3);
    return Layout_drawMaskTail(*gPC, saved, p4, (void *)0);
}

// ---- formatCredits_d34d8.cpp ----
void Layout_formatNumber(void *out, int n);   // 0x74df4 (formatNumber)

// Layout::formatCredits(int) -> out = formatNumber(n) + "$"
void Layout_formatCredits(void *out, int n) {
    unsigned char num[sizeof(String12)] __attribute__((aligned(4)));
    unsigned char suffix[sizeof(String12)] __attribute__((aligned(4)));
    Layout_formatNumber(num, n);
    String_cstr_ctor(suffix, "$", false);
    String_concat(out, num, suffix);
    ((String *)(suffix))->dtor();
    ((String *)(num))->dtor();
}

// ---- drawBGPattern_d3574.cpp ----
struct PaintCanvas;

extern "C" int __aeabi_idiv(int a, int b);                                 // 0x7198c

// Hidden global from drawBGPattern disasm.
__attribute__((visibility("hidden"))) extern PaintCanvas **g_bgCanvas;  // @0xe358a

// Layout::drawBGPattern(uint img, int x, int y, int w, int h): tile `img` to fill the area.
void Layout::drawBGPattern(unsigned img, int x, int y, int w, int h) {
    Layout *self = this;
    PaintCanvas *pc = *g_bgCanvas;
    PaintCanvas_SetColor(self->drawColor);
    int iw = PaintCanvas_GetImage2DWidth(img);
    int cols = __aeabi_idiv(w, iw);
    int fullW = cols * iw;
    int ih = PaintCanvas_GetImage2DHeight(img);
    int rows = __aeabi_idiv(h, ih);
    int fullH = rows * ih;

    for (int r = 0; r < rows; r++) {
        int py = y + r * ih;
        int px = x;
        for (int c = 0; c < cols; c++) {
            PaintCanvas_DrawImage2D3(pc, img, px, py);
            px += iw;
        }
    }

    int remH = h - fullH;
    int remW = w - fullW;
    if (remW > 0) {
        int py = y + fullH;
        for (int r = 0; r < rows; r++) {
            PaintCanvas_DrawRegion2D(pc, img, 0, 0, remW, ih, 0, 0, 0, 0, x + fullW, py);
            py += ih;
        }
    }
    if (remH > 0) {
        int px = x + fullW;
        (void)px;
        int px2 = x;
        for (int c = 0; c < cols; c++) {
            PaintCanvas_DrawRegion2D(pc, img, 0, 0, iw, remH, 0, 0, 0, 0, px2, y + fullH);
            px2 += iw;
        }
    }
    if (remW > 0 || remH > 0) {
        PaintCanvas_DrawRegion2D(pc, img, 0, 0, remW, remH, 0, 0, 0, 0,
                                 x + fullW, y + fullH);
    }
}

// ---- formatNumber_d32b4.cpp ----
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

    unsigned char digits[sizeof(String12)] __attribute__((aligned(4)));  // aSStack_34
    String_from_uint(digits, mag);
    int len = *(int *)((char *)digits + 8);

    String_cstr_ctor(out, g_fnEmpty, false);

    unsigned char sep[sizeof(String12)] __attribute__((aligned(4)));      // aSStack_40
    unsigned short lang = GameText_getLanguage();
    if (lang < 0xc && ((1u << (lang & 0xff)) & 0xc01u) != 0)
        String_cstr_ctor(sep, g_fnSepA, false);
    else
        String_cstr_ctor(sep, g_fnSepB, false);

    if (len < 4) {
        ((String *)(out))->assign((String *)digits);
    } else {
        int i = len;
        while (i > 2) {
            i -= 3;
            unsigned char grp[sizeof(String12)] __attribute__((aligned(4)));  // aSStack_64
            String_subString(grp, digits, i, len);

            unsigned char prefix[sizeof(String12)] __attribute__((aligned(4))); // aSStack_70
            if (*(unsigned *)((char *)out + 8) < 2)
                String_default_ctor(prefix);
            else
                String_copy_ctor(prefix, sep, false);

            unsigned char t1[sizeof(String12)] __attribute__((aligned(4)));  // aSStack_58
            String_concat(t1, prefix, grp);
            unsigned char t2[sizeof(String12)] __attribute__((aligned(4)));  // aSStack_4c
            String_concat(t2, t1, out);
            ((String *)(out))->assign((String *)t2);

            ((String *)(t2))->dtor();
            ((String *)(t1))->dtor();
            ((String *)(prefix))->dtor();
            ((String *)(grp))->dtor();
        }
        unsigned char head[sizeof(String12)] __attribute__((aligned(4)));   // aSStack_4c
        String_subString(head, digits, 0, i);
        if (*(int *)((char *)head + 8) != 0) {
            unsigned char j1[sizeof(String12)] __attribute__((aligned(4))); // aSStack_64
            String_concat(j1, head, sep);
            unsigned char j2[sizeof(String12)] __attribute__((aligned(4))); // aSStack_58
            String_concat(j2, j1, out);
            ((String *)(out))->assign((String *)j2);
            ((String *)(j2))->dtor();
            ((String *)(j1))->dtor();
        }
        ((String *)(head))->dtor();
    }

    if ((unsigned)value > 0x7fffffff) {
        unsigned char ov[sizeof(String12)] __attribute__((aligned(4)));     // aSStack_58
        String_cstr_ctor(ov, g_fnOverflow, false);
        unsigned char r[sizeof(String12)] __attribute__((aligned(4)));      // aSStack_4c
        String_concat(r, ov, out);
        ((String *)(out))->assign((String *)r);
        ((String *)(r))->dtor();
        ((String *)(ov))->dtor();
    }

    ((String *)(sep))->dtor();
    ((String *)(digits))->dtor();

    
}

// ---- drawWindow_d36f4.cpp ----
extern "C" void Layout_drawWindowImpl5(Layout *self, void *str, int a, int b,
                                       int p4, int p5, int flag);  // 0x74e0c
__attribute__((visibility("hidden"))) extern int *gW1;  // ldr [0xe3774]
__attribute__((visibility("hidden"))) extern int *gW2;  // ldr [0xe3778]
__attribute__((visibility("hidden"))) extern int *gW3;  // ldr [0xe377c]

// Layout::drawWindow(String, bool flag)
//   -> drawWindow(this, copy, 0, 0, *gW2, *gW3 - (*gW1)[2], flag)
void Layout::drawWindow2(const void *param, int flag) {
    Layout *self = this;
    unsigned char tmp[sizeof(String12)] __attribute__((aligned(4)));
    String_copy_ctor(tmp, param, false);
    int p4 = *gW2;
    int p5 = *gW3 - ((int *)gW1)[2];
    Layout_drawWindowImpl5(self, tmp, 0, 0, p4, p5, flag);
    ((String *)(tmp))->dtor();
}

// ---- setWindowDimensions_d4c60.cpp ----
// Function pointer (TouchButton::setPosition) loaded from a global, called via blx.
typedef void (*SetPosFn)(void *btn, int x, int y, int mode);
__attribute__((visibility("hidden"))) extern SetPosFn gSetPos;   // ldr [0xe4d30]
struct TB; struct TBHolder { TB *o; };
__attribute__((visibility("hidden"))) extern int **gTB;          // ldr [0xe4d34]
__attribute__((visibility("hidden"))) extern int *gPosX;         // ldr [0xe4d38]
__attribute__((visibility("hidden"))) extern int *gPosY;         // ldr [0xe4d3c]

// Layout::setWindowDimensions(int p1, int p2, int p3, int p4)
void Layout::setWindowDimensions(int p1, int p2, int p3, int p4) {
    Layout *self = this;
    SetPosFn setPos = gSetPos;
    self->windowX = p1;
    self->windowY = p2;
    self->windowWidth = p3;
    self->windowHeight = p4;
    setPos(self->helpButton, p3 + p1, p2, 0x12);
    int *tb = *gTB;
    setPos(self->backButton, tb[0x28 / 4] + self->windowX,
           (self->windowY + self->windowHeight) - self->field_0x3fc, 0x21);
    setPos(self->secondaryButton, tb[0x28 / 4] + self->windowX,
           (self->windowY + self->windowHeight) - self->field_0x3fc, 0x21);
}

// ---- showMissionRewardMessage_d4d40.cpp ----
__attribute__((visibility("hidden"))) extern int **gFmod;  // ldr [0xe4d70]
// FModSound::play(int sound, Vector* a, Vector* b, float v) — blx 0x71548.
struct Vec3 { float x, y, z; };

// Layout::showMissionRewardMessage(int show, bool flag)
void Layout::showMissionRewardMessage(int show, bool flag) {
    Layout *self = this;
    if (show == 0)
        return;
    self->rewardMessageFlag = flag;
    self->rewardMessageActive = 1;
    int *g = *gFmod;
    self->rewardMessageTimer = 0;
    self->rewardCredits = show;
    ((FModSound *)(*g))->play(0x24, 0, 0, 0.0f);
}

// ---- drawWindow_d38d4.cpp ----
extern "C" void Layout_drawWindowImpl5(Layout *self, void *str, int p3, int p4,
                                       int p5, int p6, int flag);  // 0x74e0c

// Layout::drawWindow(String, int p3, int p4, int p5, int p6)
//   -> drawWindow(this, copy, p3, p4, p5, p6, 1)
void Layout::drawWindow5(const void *param, int p3, int p4, int p5, int p6) {
    Layout *self = this;
    unsigned char tmp[sizeof(String12)] __attribute__((aligned(4)));
    String_copy_ctor(tmp, param, false);
    Layout_drawWindowImpl5(self, tmp, p3, p4, p5, p6, 1);
    ((String *)(tmp))->dtor();
}

// ---- drawBGBorder_d395c.cpp ----
struct PaintCanvas;

extern "C" int __aeabi_idiv(int a, int b);                                 // 0x7198c

// Hidden globals from drawBGBorder disasm.
__attribute__((visibility("hidden"))) extern PaintCanvas **g_bbCanvas;  // @0xe3972
__attribute__((visibility("hidden"))) extern int g_bbFlipTR;            // @0xe3bf4 ([0])
__attribute__((visibility("hidden"))) extern int g_bbFlipL;             // @0xe3bf8 ([0])
__attribute__((visibility("hidden"))) extern int g_bbFlipR;             // @0xe3bfc ([0])

// Layout::drawBGBorder(uint corner, uint edge, int x, int y, int w, int h, int inset, int pad)
void Layout::drawBGBorder(unsigned corner, unsigned edge, int x, int y, int w, int h, int inset, int pad) {
    Layout *self = this;
    PaintCanvas *pc = *g_bbCanvas;
    int cw = PaintCanvas_GetImage2DWidth(corner);
    int ch = PaintCanvas_GetImage2DHeight(corner);
    int ew = PaintCanvas_GetImage2DWidth(edge);
    int eh = PaintCanvas_GetImage2DHeight(edge);

    // Four corners (top-left at base; others mirrored via the 5-arg variant).
    PaintCanvas_DrawImage2D3(pc, corner, inset + x, inset + y);
    int rightX = ((w + x) - cw) - inset;
    PaintCanvas_DrawImage2D5(pc, corner, rightX, inset + y, 1);
    int bottomY = ((y + h) - ch) - inset;
    PaintCanvas_DrawImage2D5(pc, corner, inset + x, bottomY, 2);
    PaintCanvas_DrawImage2D5(pc, corner, rightX, bottomY, 3);

    // Top/bottom edges (horizontal tiling of `edge`).
    int spanW = w + cw * -2 + inset * -2;
    int colsH = __aeabi_idiv(spanW, ew);
    int baseX = pad + x;
    int tileX = cw + pad + x;
    int topRowY = pad + y;
    int botRowY = (h - inset) + pad + y;
    for (int i = 0; i < colsH; i++) {
        PaintCanvas_DrawImage2D3(pc, edge, tileX, topRowY);
        PaintCanvas_DrawImage2D5(pc, edge, tileX, botRowY, 2);
        tileX += ew;
    }
    int remW = spanW - colsH * ew;
    if (remW > 0) {
        PaintCanvas_DrawRegion2D(pc, edge, 0, 0, remW, eh, 0, 0, 0, 0,
                                 colsH * ew + baseX + cw, topRowY);
        PaintCanvas_DrawRegion2D(pc, edge, 0, 0, remW, eh, 0, g_bbFlipTR, 0, 0,
                                 spanW + baseX + cw, botRowY);
    }

    // Left/right edges (vertical tiling of `edge`).
    int spanH = h + ch * -2 + inset * -2;
    int rowsV = __aeabi_idiv(spanH, ew);
    int leftX = pad + x + inset;
    int rightEdgeX = w + x + pad;
    for (int i = 0; i < rowsV; i++) {
        PaintCanvas_DrawRegion2D(pc, edge, 0, 0, ew, eh, 0, g_bbFlipL, 0, 0, leftX, 0);
        PaintCanvas_DrawRegion2D(pc, edge, 0, 0, ew, eh, 0, g_bbFlipR, 0, 0, rightEdgeX, 0);
    }
    int remH = spanH - rowsV * ew;
    if (remH > 0) {
        PaintCanvas_DrawRegion2D(pc, edge, 0, 0, remH, eh, 0, g_bbFlipL, 0, 0, leftX, 0);
        PaintCanvas_DrawRegion2D(pc, edge, 0, 0, remH, eh, 0, g_bbFlipR, 0, 0,
                                 rightEdgeX + inset * -2, 0);
    }
}

// ---- drawScrollBar_d4258.cpp ----
struct PaintCanvas;

// 0x74e00

// Hidden globals from drawScrollBar disasm.
__attribute__((visibility("hidden"))) extern PaintCanvas **g_sbCanvas;  // @0xe426e
__attribute__((visibility("hidden"))) extern unsigned g_sbColor0;       // @0xe4370 ([0])
__attribute__((visibility("hidden"))) extern unsigned g_sbColor1;       // @0xe4374 ([0])
__attribute__((visibility("hidden"))) extern int **g_sbMetric;          // @0xe42a2 ([0][0x48])

// Layout::drawScrollBar(int x, int y, int trackH, int pos, int range)
void Layout::drawScrollBar(int x, int y, int trackH, int pos, int range) {
    Layout *self = this;
    PaintCanvas *pc = *g_sbCanvas;
    int iw = PaintCanvas_GetImage2DWidth(self->scrollBarImage);
    int ih = PaintCanvas_GetImage2DHeight(self->scrollBarImage);

    PaintCanvas_SetColor(*(unsigned *)&g_sbColor0);
    PaintCanvas_SetColor(*(unsigned *)&g_sbColor1);

    int inset = self->field_0x3e0;
    PaintCanvas_DrawRectangle(pc, x, inset + y, (*g_sbMetric)[0x48 / 4], trackH - inset * 2);
    PaintCanvas_SetColor(self->drawColor);

    int thumb = range - 1;
    if (thumb <= ih * 2) thumb = ih * 2;
    int off = pos + 1;
    if (trackH <= thumb + off)
        off = (trackH - thumb) - inset;

    int handle = self->field_0x3e4;
    thumb = thumb + handle * -4;
    off = off + handle * 2;
    if (ih * 2 < thumb) {
        ((Layout *)(self))->drawBGPattern(self->field_0x378, x + 1 + handle, ih + y + off, iw, thumb + ih * -2);
        handle = self->field_0x3e4;
    } else {
        int lim = trackH + ih * -2;
        if (lim <= off) off = lim;
    }

    PaintCanvas_DrawImage2D3(pc, self->scrollBarImage, handle + x + 1);
    PaintCanvas_DrawImage2D5(pc, self->scrollBarImage,
                             self->field_0x3e4 + x + 1, (thumb - ih) + y + off, 0x02);
}

// ---- drawFade_d5038.cpp ----
struct PaintCanvas;


// Hidden globals from drawFade disasm.
__attribute__((visibility("hidden"))) extern PaintCanvas **g_dfCanvasA;  // @0xe5052
__attribute__((visibility("hidden"))) extern int **g_dfDimA;             // @0xe50c8 ([0][0])
__attribute__((visibility("hidden"))) extern PaintCanvas **g_dfCanvasB;  // @0xe50ee
__attribute__((visibility("hidden"))) extern int **g_dfDimB;             // @0xe5108 ([0][0])

// Layout::drawFade(): overlay the fade-in/out rectangles, returning the active flag.
uint8_t Layout::drawFade() {
    Layout *self = this;
    if (self->fading != 0) {
        PaintCanvas *pc = *g_dfCanvasA;
        unsigned saved = PaintCanvas_GetColor(pc);

        // progress = duration / endTime  (fields 0x408 / 0x40c, signed->float)
        float t = (float)self->fadeProgress / (float)self->fadeDuration;
        if (self->fadeOut != 0)
            t = 1.0f - t;
        if (t > 1.0f) t = 1.0f;

        unsigned color = self->fadeColor;
        if (t > 0.0f)
            color += (int)(t * 255.0f);
        PaintCanvas_SetColor(color);
        PaintCanvas_FillRectangle(*g_dfCanvasA, 0, 0, **g_dfDimA);
        PaintCanvas_SetColor(saved);
    }
    if (self->fillScreen != 0) {
        PaintCanvas *pc = *g_dfCanvasB;
        unsigned saved = PaintCanvas_GetColor(pc);
        PaintCanvas_SetColor(0xff);
        PaintCanvas_FillRectangle(*g_dfCanvasB, 0, 0, **g_dfDimB);
        PaintCanvas_SetColor(saved);
    }
    return self->fading;
}

// ---- drawBox_d4384.cpp ----
struct PaintCanvas;

// 0x74e00
extern "C" void Layout_drawBGBorder8(Layout *self, unsigned a, unsigned b, int x,
                                     int y, int w, int h, int p8, int p9);  // 0x74e30
extern "C" void Layout_drawBGBorder6(Layout *self, unsigned a, int b, int x,
                                     int y, int w);                         // 0x74e78
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
    Layout *self = this;
    PaintCanvas *pc = *g_dbCanvas;
    unsigned saved = PaintCanvas_GetColor(pc);
    PaintCanvas_SetColor(self->drawColor);

    switch (style) {
    case 0: {
        int iw = PaintCanvas_GetImage2DWidth(self->field_0x348);
        PaintCanvas_DrawImage2D3(pc, self->field_0x348, x, y);
        ((Layout *)(self))->drawBGPattern(self->field_0x34c, iw + x, y, w + iw * -2, h);
        PaintCanvas_DrawImage2D5(pc, self->field_0x348, (w + x) - iw, y, 0x01);
        if (*(int *)((char *)text + 8) == 0) break;
        int *mt = *g_dbMetric0;
        int tx = mt[0x44 / 4];
        if ((flags & 2) == 0) {
            if ((int)(flags << 0x1d) < 0) {
                int tw = PaintCanvas_GetTextWidth(*(unsigned *)g_dbCanvas, *g_dbFont0c);
                tx = w / 2 - tw / 2;
            }
        } else {
            int tw = PaintCanvas_GetTextWidth(*(unsigned *)g_dbCanvas, *g_dbFont0r);
            tx = (w - tx) - tw;
        }
        int ty = (y + (mt[0x1c / 4] >> 1) + 1) - self->field_0x3ac;
        PaintCanvas_DrawString(*(unsigned *)g_dbCanvas, *g_dbFont0, text, tx + x, ty);
        break;
    }
    case 1: {
        int iw = PaintCanvas_GetImage2DWidth(self->field_0x350);
        PaintCanvas_DrawImage2D3(pc, self->field_0x350, x, y);
        ((Layout *)(self))->drawBGPattern(self->field_0x354, iw + x, y, w + iw * -2, h);
        PaintCanvas_DrawImage2D5(pc, self->field_0x350, (w + x) - iw, y, 0x01);
        if (*(int *)((char *)text + 8) == 0) break;
        int *mt = *g_dbMetric1;
        int tx = mt[0x44 / 4];
        if ((flags & 2) == 0) {
            if ((int)(flags << 0x1d) < 0) {
                int tw = PaintCanvas_GetTextWidth(*(unsigned *)g_dbCanvas, *g_dbFont1c);
                tx = w / 2 - tw / 2;
            }
        } else {
            int tw = PaintCanvas_GetTextWidth(*(unsigned *)g_dbCanvas, *g_dbFont1r);
            tx = (w - tx) - tw;
        }
        int ty = (y + (mt[0x5c / 4] >> 1) + 1) - self->field_0x3ac;
        PaintCanvas_DrawString(*(unsigned *)g_dbCanvas, *g_dbFont1, text, tx + x, ty);
        break;
    }
    case 2:
        ((Layout *)(self))->drawBGPattern(self->bgPatternImage, x, y, w, h);
        break;
    case 3: {
        int iw = PaintCanvas_GetImage2DWidth(self->field_0x358);
        PaintCanvas_DrawImage2D3(pc, self->field_0x358, x, y);
        ((Layout *)(self))->drawBGPattern(self->field_0x35c, iw + x, y, w + iw * -2, h);
        PaintCanvas_DrawImage2D5(pc, self->field_0x358, (w + x) - iw, y, 0x01);
        break;
    }
    case 4: {
        int iw = PaintCanvas_GetImage2DWidth(self->field_0x36c);
        PaintCanvas_DrawImage2D3(pc, self->field_0x36c, x, y);
        ((Layout *)(self))->drawBGPattern(self->field_0x370, iw + x, y, w + iw * -2, h);
        PaintCanvas_DrawImage2D5(pc, self->field_0x36c, (w + x) - iw, y, 0x01);
        break;
    }
    case 5:
        Layout_drawBGBorder6(self, self->field_0x380, self->field_0x384, x, y, w);
        break;
    case 6: {
        int iw = PaintCanvas_GetImage2DWidth(self->field_0x388);
        PaintCanvas_DrawImage2D3(pc, self->field_0x388, x, y);
        ((Layout *)(self))->drawBGPattern(self->field_0x38c, iw + x, y, w + iw * -2, h);
        PaintCanvas_DrawImage2D5(pc, self->field_0x388, (w + x) - iw, y, 0x01);
        if (*(int *)((char *)text + 8) == 0) break;
        int *mt = *g_dbMetric6;
        int tx = mt[0x44 / 4];
        if ((flags & 2) == 0) {
            if ((int)(flags << 0x1d) < 0) {
                int tw = PaintCanvas_GetTextWidth(*(unsigned *)g_dbCanvas, *g_dbFont6c);
                tx = w / 2 - tw / 2;
            }
        } else {
            int tw = PaintCanvas_GetTextWidth(*(unsigned *)g_dbCanvas, *g_dbFont6r);
            tx = (w - tx) - tw;
        }
        int ty = (y + (h >> 1) + 1) - self->field_0x3ac;
        PaintCanvas_DrawString(*(unsigned *)g_dbCanvas, *g_dbFont6, text, tx + x, ty);
        break;
    }
    case 7: {
        int *mt = *g_dbMetric7;
        int hdr = mt[8 / 4];
        ((Layout *)(self))->drawBGPattern(self->bgPatternImage, x, hdr + y, w, h - hdr);
        int ih = PaintCanvas_GetImage2DHeight(self->field_0x394);
        Layout_drawBGBorder8(self, self->field_0x390, self->field_0x394,
                             x, hdr + y, w, h - hdr, -ih, -ih);
        if (*(int *)((char *)text + 8) == 0) break;
        PaintCanvas_SetColor(0xffffffff);
        PaintCanvas_DrawImage2D3(pc, self->field_0x32c, x, y);
        int ty = (y + (mt[8 / 4] / 2) + 1) - self->field_0x3ac;
        PaintCanvas_DrawString(*(unsigned *)g_dbCanvas, *g_dbFont7, text,
                               mt[0x28 / 4] + x, ty);
        break;
    }
    case 8:
        Layout_drawBGBorder6(self, self->field_0x39c, self->field_0x3a0, x, y, w);
        break;
    case 9: {
        int iw = PaintCanvas_GetImage2DWidth(self->field_0x360);
        PaintCanvas_DrawImage2D3(pc, self->field_0x360, x, y);
        ((Layout *)(self))->drawBGPattern(self->field_0x364, iw + x, y, w + iw * -2, h);
        PaintCanvas_DrawImage2D5(pc, self->field_0x360, (w + x) - iw, y, 0x01);
        break;
    }
    case 10: {
        int iw = PaintCanvas_GetImage2DWidth(self->field_0x368);
        PaintCanvas_DrawImage2D3(pc, self->field_0x368, x, y);
        ((Layout *)(self))->drawBGPattern(self->field_0x370, iw + x, y, w + iw * -2, h);
        PaintCanvas_DrawImage2D5(pc, self->field_0x368, (w + x) - iw, y, 0x01);
        break;
    }
    default:
        break;
    }

    PaintCanvas_restoreColor(*(unsigned *)g_dbCanvas, saved);
}

// ---- drawWindow_d3780.cpp ----
struct PaintCanvas;

// 0x74e00
// Color-restore tail helper @0x1ac088.

// Hidden globals from drawWindow disasm.
__attribute__((visibility("hidden"))) extern PaintCanvas **g_dwCanvas;  // @0xe3796
__attribute__((visibility("hidden"))) extern int **g_dwBorderTop;       // @0xe37b0 ([0][8])
__attribute__((visibility("hidden"))) extern int **g_dwMetric;          // @0xe37d8 ([0][8],[0][0x28])
__attribute__((visibility("hidden"))) extern const char g_dwCmpLit[];   // @0xe381a
__attribute__((visibility("hidden"))) extern void ***g_dwFont;          // @0xe382c

// Layout::drawWindow(String, int x, int y, int w, int h, bool drawBG)
void Layout::drawWindow7(void *title, int x, int y, int w, int h, int drawBG) {
    Layout *self = this;
    PaintCanvas *pc = *g_dwCanvas;
    unsigned saved = PaintCanvas_GetColor(pc);
    if (drawBG != 0) {
        int top = (*g_dwBorderTop)[8 / 4];
        ((Layout *)(self))->drawBGPattern(self->bgPatternImage, x, top + y, w, h - top);
    }
    PaintCanvas_SetColor(*(unsigned *)g_dwCanvas);  // *puVar3 (the color int slot)
    int *m = *g_dwMetric;
    int top = m[8 / 4];
    PaintCanvas_DrawRectangle(*g_dwCanvas, x, top + y, w, h - top);
    PaintCanvas_SetColor(self->drawColor);
    PaintCanvas_DrawImage2D3(*g_dwCanvas, self->field_0x32c, x);
    if (*(int *)((char *)title + 8) != 0 &&
        AbyssString_Compare(title, g_dwCmpLit) == 0) {
        int *mm = *g_dwMetric;
        int half = mm[8 / 4];
        half += half >> 31;
        int ty = (y + (half >> 1) + 1) - self->field_0x3ac;
        PaintCanvas_DrawString(*g_dwCanvas, **g_dwFont, title, mm[0x28 / 4] + x, ty);
    }
    PaintCanvas_restoreColor(*g_dwCanvas, saved);
}

// ---- drawBox_d48f0.cpp ----
extern "C" void Layout_drawBoxImpl(Layout *self, int p2, int p3, int p4,
                                   int p5, int p6, void *str, int flag);  // 0x74e84

// Layout::drawBox(int, int, int, int, int, String) -> drawBox(..., copy, 1)
void Layout::drawBox6(int p2, int p3, int p4, int p5, int p6, const void *str) {
    Layout *self = this;
    unsigned char tmp[sizeof(String12)] __attribute__((aligned(4)));
    String_copy_ctor(tmp, str, false);
    Layout_drawBoxImpl(self, p2, p3, p4, p5, p6, tmp, 1);
    ((String *)(tmp))->dtor();
}

// ---- drawTip_d4a14.cpp ----
struct PaintCanvas;

extern "C" void Layout_drawBoxStr(PaintCanvas *pc, int p2, int x, int y,
                               int w, int h, void *str);                   // 0x7462c

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
    Layout *self = this;
    int *guard = g_dtGuard;
    int g0 = *guard;
    if (self->tipLines != 0) {
        PaintCanvas_SetColor(*g_dtColor);
        int *mA = *g_dtMetricA;
        int dimW = *g_dtDimW;
        int dimH = *g_dtDimH;
        int boxW = mA[0x78 / 4];

        unsigned char box[sizeof(String12)] __attribute__((aligned(4)));
        String_cstr_ctor(box, g_dtBoxLit, false);
        Layout_drawBoxStr((PaintCanvas *)mA, 5,
                       (dimH >> 1) - (boxW >> 1), (dimW >> 1) + 0xd, boxW, 100, box);
        ((String *)(box))->dtor();

        PaintCanvas_DrawImage2D5((PaintCanvas *)*g_dtColor, self->tipBoxImage,
                                 dimH >> 1, (dimW >> 1) + 0x3f, 0x11);

        int lineCount = *self->tipLines;
        int y = (dimW >> 1) + 0x3f - ((lineCount * mA[4 / 4]) >> 1);
        Globals_drawLines(*g_dtLinesB, *(void **)g_dtLinesA, self->tipLines,
                          dimH >> 1, y);
    }
    
}

// ---- drawHeader_d4200.cpp ----
extern "C" void Layout_drawHeaderImpl(Layout *self, void *str, int flag); // 0x74e6c

// Layout::drawHeader(String) -> drawHeader(this, copy, 1)
void Layout::drawHeader1(const void *param) {
    Layout *self = this;
    unsigned char tmp[sizeof(String12)] __attribute__((aligned(4)));
    String_copy_ctor(tmp, param, false);
    Layout_drawHeaderImpl(self, tmp, 1);
    ((String *)(tmp))->dtor();
}

// ---- drawHelpWindow_d4bc0.cpp ----
extern "C" void Layout_drawHelpWindowImpl(void *p);  // -> 0x1ac0b8

// Layout::drawHelpWindow() -> ext(*(this+0x3c4))
void Layout::drawHelpWindow() {
    Layout *self = this;
    return Layout_drawHelpWindowImpl(self->choiceWindow);
}

// ---- initHelpWindow_d4b40.cpp ----
extern "C" void *operator_new_li(unsigned sz);                  // 0x6eb24
__attribute__((visibility("hidden"))) extern int *gFmodHelp;        // ldr [0xe4bb8] (distinct global from showMissionRewardMessage's gFmod)
// gGameText holder declared at top of file.

// Layout::initHelpWindow(String text)
void Layout::initHelpWindow(void *text) {
    Layout *self = this;
    if (self->choiceWindow == 0) {
        void *cw = operator_new_li(0x5c);
        ChoiceWindow_ctor(cw);
        self->choiceWindow = cw;
    }
    ((FModSound *)(*gFmodHelp))->play(0x7e, 0, 0, 0);
    void *cw = self->choiceWindow;
    void *title = ((GameText *)(gGameText->obj))->getText(0x187);
    ((ChoiceWindow *)(cw))->set(*(String *)title, *(String *)text);
    self->choiceWindowOpen = 1;
    self->helpPressedFlag = 0;
}

// ---- resetWindowDimensions_d3048.cpp ----
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
    Layout *self = this;
    int *guard = g_rwGuard;
    int g0 = *guard;
    int h = **g_rwH;
    int w = **g_rwW;

    self->windowX = 0;
    self->windowY = 0;
    self->windowWidth = w;
    self->windowHeight = h;

    Layout_setBtnRect(self->helpButton, w, 0, 0x12);

    int *m = *g_rwMetric;
    int inset = m[0x28 / 4];
    Layout_setBtnRect(self->backButton,
                      inset + self->windowX,
                      (self->windowY + self->windowHeight) - self->field_0x3fc,
                      0x21);
    Layout_setBtnRect(self->secondaryButton,
                      inset + self->windowX,
                      (self->windowY + self->windowHeight) - self->field_0x3fc,
                      0x21);

    if (self->backButton != 0) {
        float pos[2] __attribute__((aligned(4)));
        TouchButton_getPosition((TouchButton *)pos);
        *g_rwOutX = (int)pos[0];
        TouchButton_getPosition((TouchButton *)pos);
        *g_rwOutY = (int)pos[1];
    }

    
}

// ---- tagString_d31b0.cpp ----

// Hidden globals from tagString disasm.
__attribute__((visibility("hidden"))) extern const void *g_tagBaseString;  // @0xe329c
__attribute__((visibility("hidden"))) extern const char g_tagLit0[];       // @0xe31d2
__attribute__((visibility("hidden"))) extern const char g_tagLit1[];       // @0xe31ec
__attribute__((visibility("hidden"))) extern const char g_tagLit2[];       // @0xe320c (return prefix, ctor'd but unused in concat below)

// Layout::tagString(String): build "<lit0><base><lit1>" + param into the r0 return slot.
// out  = param_1 (the String return value, arrives in r0).
// in   = the incoming String (r2 -> aSStack_28 source).
void Layout_tagString(void *out, const void *in) {
    unsigned char s_lit0[sizeof(String12)] __attribute__((aligned(4)));   // aSStack_4c
    unsigned char s_base[sizeof(String12)] __attribute__((aligned(4)));   // aSStack_58
    unsigned char s_ab[sizeof(String12)]   __attribute__((aligned(4)));   // aSStack_40
    unsigned char s_lit1[sizeof(String12)] __attribute__((aligned(4)));   // aSStack_64
    unsigned char s_abc[sizeof(String12)]  __attribute__((aligned(4)));   // aSStack_34
    unsigned char s_full[sizeof(String12)] __attribute__((aligned(4)));   // aSStack_28
    unsigned char s_lit2[sizeof(String12)] __attribute__((aligned(4)));   // aSStack_70

    String_cstr_ctor(s_lit0, g_tagLit0, false);
    String_copy_global(s_base, g_tagBaseString);
    String_concat(s_ab, s_lit0, s_base);
    String_cstr_ctor(s_lit1, g_tagLit1, false);
    String_concat(s_abc, s_ab, s_lit1);
    String_concat(s_full, s_abc, (void *)in);
    String_cstr_ctor(s_lit2, g_tagLit2, false);
    String_concat(out, s_full, (void *)s_lit2);

    ((String *)(s_lit2))->dtor();
    ((String *)(s_full))->dtor();
    ((String *)(s_abc))->dtor();
    ((String *)(s_lit1))->dtor();
    ((String *)(s_ab))->dtor();
    ((String *)(s_base))->dtor();
    ((String *)(s_lit0))->dtor();
}

// ---- reload_d2c20.cpp ----
struct PaintCanvas;
struct TouchButton;

extern "C" void __aeabi_memset4(void *dst, int n, int v);                // 0x74d94
// 0xd3048
// Image2DCreate-into-field loader (fn ptr @0xe2c64): (canvas, imgId, &field).

__attribute__((visibility("hidden"))) extern int *g_rlGuard;    // @0xe2c38 (stack guard [0])
__attribute__((visibility("hidden"))) extern unsigned *g_rlCanvas; // @0xe2c62 (*puVar8)
__attribute__((visibility("hidden"))) extern char *g_rlHdFlag;  // @0xe2da8 (HD variant)
__attribute__((visibility("hidden"))) extern int *g_rlBackText; // @0xe2ec0 (*piVar10)
__attribute__((visibility("hidden"))) extern int *g_rlScreenH;  // @0xe2ed6 (*piVar9)
__attribute__((visibility("hidden"))) extern int *g_rlMenuY;    // @0xe2f8a

// Layout::reload(): (re)load all layout images/strings and rebuild the buttons.
void Layout::reload() {
    Layout *self = this;
    int *guard = g_rlGuard;
    int g0 = *guard;

    self->bgPatternImage = -1;
    self->field_0x328 = -1;
    self->field_0x32c = -1;
    self->field_0x3a8 = -1;
    __aeabi_memset4((char *)self + 0x334, 0x70, 0xff);

    unsigned canvas = *g_rlCanvas;
    Layout_loadImage(canvas, 0x503, (char *)self + 0x398);
    Layout_loadImage(canvas, 0x47e, (char *)self + 0x324);
    Layout_loadImage(canvas, 0x4ff, (char *)self + 0x328);
    Layout_loadImage(canvas, 0x500, (char *)self + 0x330);
    Layout_loadImage(canvas, 0x474, (char *)self + 0x32c);
    Layout_loadImage(canvas, 0x502, (char *)self + 0x334);
    Layout_loadImage(canvas, 0x506, (char *)self + 0x340);
    Layout_loadImage(canvas, 0x501, (char *)self + 0x338);
    Layout_loadImage(canvas, 0x507, (char *)self + 0x344);
    Layout_loadImage(canvas, 0x4fe, (char *)self + 0x33c);
    Layout_loadImage(canvas, 0x482, (char *)self + 0x348);
    Layout_loadImage(canvas, 0x481, (char *)self + 0x34c);
    Layout_loadImage(canvas, 0x486, (char *)self + 0x378);
    Layout_loadImage(canvas, 0x487, (char *)self + 0x374);
    Layout_loadImage(canvas, 0x48b, (char *)self + 0x37c);
    Layout_loadImage(canvas, 0x52d, (char *)self + 0x3a4);

    if (*g_rlHdFlag == 0) {
        Layout_loadImage(canvas, 0x480, (char *)self + 0x350);
        Layout_loadImage(canvas, 0x47f, (char *)self + 0x354);
        Layout_loadImage(canvas, 0x479, (char *)self + 0x358);
        Layout_loadImage(canvas, 0x478, (char *)self + 0x35c);
        Layout_loadImage(canvas, 0x489, (char *)self + 0x36c);
        Layout_loadImage(*g_rlCanvas, 0x488, (char *)self + 0x370);
    } else {
        Layout_loadImage(canvas, 0x6bb, (char *)self + 0x350);
        Layout_loadImage(canvas, 0x6ba, (char *)self + 0x354);
        Layout_loadImage(canvas, 0x6b9, (char *)self + 0x358);
        Layout_loadImage(canvas, 0x6b8, (char *)self + 0x35c);
        Layout_loadImage(canvas, 0x6b7, (char *)self + 0x36c);
        Layout_loadImage(*g_rlCanvas, 0x6bc, (char *)self + 0x370);
    }

    Layout_loadImage(canvas, 0x530, (char *)self + 0x360);
    Layout_loadImage(canvas, 0x531, (char *)self + 0x364);
    Layout_loadImage(canvas, 0x52f, (char *)self + 0x368);
    Layout_loadImage(canvas, 0x47c, (char *)self + 900);
    Layout_loadImage(canvas, 0x47d, (char *)self + 0x380);
    Layout_loadImage(canvas, 0x47b, (char *)self + 0x388);
    Layout_loadImage(canvas, 0x47a, (char *)self + 0x38c);
    Layout_loadImage(canvas, 0x484, (char *)self + 0x390);
    Layout_loadImage(canvas, 0x483, (char *)self + 0x394);
    Layout_loadImage(canvas, 0x50c, (char *)self + 0x3a0);
    Layout_loadImage(canvas, 0x50d, (char *)self + 0x39c);

    // Back button (string-labelled).
    TouchButton *bBack = (TouchButton *)operator_new(200);
    void *txt = gGameText->obj->getText(*g_rlBackText);
    int sh = *g_rlScreenH;
    TouchButton_ctorStr(bBack, txt, 2, self->field_0x28, sh - 3, '!');
    self->backButton = bBack;
    self->backButtonWidth = ((TouchButton *)(bBack))->getWidth();

    // Secondary button (image if available, else string fallback).
    unsigned img535 = 0xffffffff;
    PaintCanvas_Image2DCreate((PaintCanvas *)*g_rlCanvas, 0x535, &img535);
    TouchButton *b2 = (TouchButton *)operator_new(200);
    if (img535 == 0xffffffff) {
        void *t = gGameText->obj->getText(*g_rlBackText);
        TouchButton_ctorStr(b2, t, 2, self->field_0x28,
                            *g_rlScreenH - self->field_0x3fc, '!');
    } else {
        TouchButton_ctorImg(b2, img535, 2, self->field_0x28,
                            *g_rlScreenH - self->field_0x3fc, '!');
    }
    self->secondaryButton = b2;

    // Help button (image).
    unsigned img471 = 0xffffffff;
    PaintCanvas_Image2DCreate((PaintCanvas *)*g_rlCanvas, 0x471, &img471);
    TouchButton *bHelp = (TouchButton *)operator_new(200);
    TouchButton_ctorImg2(bHelp, img471, 1, *g_rlMenuY, 0, self->field_0x3c, 0x12, 0x04);
    self->helpButton = bHelp;

    int th = PaintCanvas_GetTextHeight(*g_rlCanvas);
    self->choiceWindowOpen = 0;
    self->choiceWindow = 0;
    self->tipLines = 0;
    self->field_0x3ac = th / 2 - 1;
    self->drawColor = -1;
    ((Layout *)(self))->resetWindowDimensions();
    self->rewardMessageTimer = 0;
    self->helpButtonEnabled = 0;
    self->field_0x3d8 = 0;
    self->fading = 0;
    self->field_0x3dc = 0;
    self->fadeColor = 0;
    self->fadeProgress = 0;
    self->rewardMessageActive = 0;
    self->field_0x40d = 0;
    self->field_0x409 = 0;

    
}

// ---- drawFooter_d3c3c.cpp ----
struct PaintCanvas;
struct TouchButton;
struct Status;
struct Ship;

// 0x74e00
void Layout_formatCredits(void *out, int n);                    // 0x74e54

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
    Layout *self = this;
    int *guard = g_dfGuard;
    int g0 = *guard;
    PaintCanvas *pc = *g_dfCanvas;
    PaintCanvas_SetColor(self->drawColor);
    int wRight = PaintCanvas_GetImage2DWidth(self->footerImageRight);
    int wLeft = PaintCanvas_GetImage2DWidth(self->footerImageLeft);

    PaintCanvas_DrawImage2D5(pc, self->footerImageLeft, self->windowX,
                             self->windowY + self->windowHeight, 0x11);
    int *m = *g_dfMetric;
    int footerH = m[0x10 / 4];
    PaintCanvas_DrawImage2D3(pc, self->footerImageRight, self->windowX + wLeft,
                             (self->windowY + self->windowHeight) - footerH);
    int both = wLeft + wRight;
    ((Layout *)(self))->drawBGPattern(self->field_0x338, both + self->windowX, (self->windowY + self->windowHeight) - footerH, self->windowWidth + both * -2, footerH);
    PaintCanvas_DrawImage2D5(pc, self->footerImageRight,
                             (self->windowX - both) + self->windowWidth,
                             (self->windowHeight + self->windowY) - footerH, 0x01);
    PaintCanvas_DrawImage2D5(pc, self->footerImageLeft,
                             (self->windowX - wRight) + self->windowWidth,
                             (self->windowHeight + self->windowY) - footerH, 0x01);

    int backVis = (!stationMode) && showBack;
    ((TouchButton *)(self->backButton))->setVisible(backVis);
    if (!backVis) {
        ((TouchButton *)(self->secondaryButton))->draw();
    } else if (showBack) {
        ((TouchButton *)(self->backButton))->draw();
    }

    // Cargo load text, in warning color if over capacity.
    ((Status *)(*gStatus))->getShip();
    int load = Ship_getCurrentLoad(((Status *)(*gStatus))->getShip());
    ((Status *)(*gStatus))->getShip();
    int maxLoad = Ship_getMaxLoad(((Status *)(*gStatus))->getShip());
    if (maxLoad < load)
        PaintCanvas_SetColor(*(unsigned *)&g_dfWarnColor);

    ((Status *)(*gStatus))->getShip();
    int cur = Ship_getCurrentLoad(((Status *)(*gStatus))->getShip());
    unsigned char sLoad[sizeof(String12)] __attribute__((aligned(4)));   // aSStack_58
    String_from_uint(sLoad, cur);
    unsigned char sSep[sizeof(String12)] __attribute__((aligned(4)));    // aSStack_64
    String_cstr_ctor(sSep, g_dfSep, false);
    unsigned char s1[sizeof(String12)] __attribute__((aligned(4)));      // aSStack_4c
    String_concat(s1, sLoad, sSep);

    ((Status *)(*gStatus))->getShip();
    int mx = Ship_getMaxLoad(((Status *)(*gStatus))->getShip());
    unsigned char sMax[sizeof(String12)] __attribute__((aligned(4)));    // aSStack_70
    String_from_uint(sMax, mx);
    unsigned char s2[sizeof(String12)] __attribute__((aligned(4)));      // aSStack_40
    String_concat(s2, s1, sMax);
    unsigned char sTail[sizeof(String12)] __attribute__((aligned(4)));   // aSStack_7c
    String_cstr_ctor(sTail, g_dfTail, false);
    unsigned char loadStr[sizeof(String12)] __attribute__((aligned(4))); // aSStack_34
    String_concat(loadStr, s2, sTail);

    ((String *)(sTail))->dtor();
    ((String *)(s2))->dtor();
    ((String *)(sMax))->dtor();
    ((String *)(s1))->dtor();
    ((String *)(sSep))->dtor();
    ((String *)(sLoad))->dtor();

    {
        int x = self->windowX;
        int w = self->windowWidth;
        void *font = *g_dfFont;
        unsigned cv = *(unsigned *)g_dfCanvas;
        int tw = PaintCanvas_GetTextWidth(cv, font);
        PaintCanvas_DrawString(cv, font, loadStr, (x + w / 2) - tw / 2,
                               (self->windowHeight + self->windowY) - self->field_0x14);
    }
    PaintCanvas_SetColor(self->drawColor);

    int credits = ((Status *)(*gStatus))->getCredits();
    unsigned char credStr[sizeof(String12)] __attribute__((aligned(4)));  // aSStack_40
    Layout_formatCredits(credStr, credits);

    {
        int x = self->windowX;
        int w = self->windowWidth;
        void *font = *g_dfFont;
        unsigned cv = *(unsigned *)g_dfCanvas;
        if (stationMode) {
            int rightInset = m[0x74 / 4];
            int tw = PaintCanvas_GetTextWidth(cv, font);
            PaintCanvas_DrawString(cv, font, credStr, ((w + x) - rightInset) - tw / 2,
                                   (self->windowHeight + self->windowY) - self->field_0x14);
        } else {
            int tw = PaintCanvas_GetTextWidth(cv, font);
            PaintCanvas_DrawString(cv, font, credStr, (w + x - 10) - tw,
                                   (self->windowHeight + self->windowY) - self->field_0x14);
        }
    }

    ((String *)(credStr))->dtor();
    ((String *)(loadStr))->dtor();
    
}

// ---- drawHeader_d40a0.cpp ----
extern "C" void Layout_drawHeaderImpl(Layout *self, void *str, int flag); // 0x74e6c

// Layout::drawHeader() -> drawHeader(this, String(""), 0)
void Layout::drawHeader0() {
    Layout *self = this;
    unsigned char tmp[sizeof(String12)] __attribute__((aligned(4)));
    String_cstr_ctor(tmp, "", false);
    Layout_drawHeaderImpl(self, tmp, 0);
    ((String *)(tmp))->dtor();
}

// ---- drawFooterStation_d4098.cpp ----
// Layout::drawFooterStation() -> drawFooter(this, 1, 0)  (this passed implicitly in r0)
void Layout::drawFooterStation() {
    Layout *self = this;
    return ((Layout *)(self))->drawFooterImpl(1, 0);
}

// ---- drawHeader_d4100.cpp ----
struct PaintCanvas;

// 0x74e00
// Header transition tail helper @0x1ac0a8.

// Hidden globals from drawHeader disasm.
__attribute__((visibility("hidden"))) extern unsigned *g_dhColor;   // @0xe4118 ([0]=image/color)
__attribute__((visibility("hidden"))) extern int **g_dhMetric;      // @0xe41aa ([0][0x28],[0][0x44])
__attribute__((visibility("hidden"))) extern void ***g_dhFont;      // @0xe41ac

// Layout::drawHeader(String title, bool transition)
void Layout::drawHeader7(void *title, int transition) {
    Layout *self = this;
    unsigned img = *g_dhColor;
    PaintCanvas_SetColor(self->drawColor);
    int iw = PaintCanvas_GetImage2DWidth(img);
    int ih = PaintCanvas_GetImage2DHeight(img);
    PaintCanvas_DrawImage2D3((PaintCanvas *)img, self->field_0x330,
                             self->windowX, self->windowY);
    ((Layout *)(self))->drawBGPattern(self->field_0x328, self->windowX + iw, self->windowY, self->windowWidth + iw * -2, ih);
    PaintCanvas_DrawImage2D9((PaintCanvas *)img, self->field_0x330,
                             self->windowWidth + self->windowX,
                             self->windowY, iw, ih, 0x11, 0x12, 0x01);
    if (*(int *)((char *)title + 8) != 0) {
        PaintCanvas_DrawImage2D3((PaintCanvas *)img, self->field_0x32c,
                                 self->windowX, self->windowY);
        int *m = *g_dhMetric;
        PaintCanvas_DrawString((PaintCanvas *)img, **g_dhFont, title,
                               m[0x28 / 4] + m[0x44 / 4] + self->windowX,
                               self->field_0x18 + self->windowY);
    }
    self->helpButtonEnabled = (uint8_t)transition;
    if (transition != 0 && self->choiceWindowOpen == 0) {
        Layout_headerAnim(self->helpButton);
        return;
    }
}

// ---- drawWindow_d387c.cpp ----
extern "C" void Layout_drawWindowImpl(Layout *self, void *str, int flag); // 0x74e24

// Layout::drawWindow(String) -> drawWindow(this, copy, 0). The on-stack String
// temp makes the compiler emit the -fstack-protector canary automatically.
void Layout::drawWindow1(const void *param) {
    Layout *self = this;
    unsigned char tmp[sizeof(String12)] __attribute__((aligned(4)));
    String_copy_ctor(tmp, param, false);
    Layout_drawWindowImpl(self, tmp, 0);
    ((String *)(tmp))->dtor();
}

// ---- drawMissionRewardMessage_d4d74.cpp ----
struct PaintCanvas;

extern "C" void Layout_drawBoxStr(PaintCanvas *pc, int p2, int x, int y,
                               int w, int h, void *str);                   // 0x7462c
void Layout_formatCredits(void *out, int n);                    // 0x74e54

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
    Layout *self = this;
    int *guard = g_mrGuard;
    int g0 = *guard;
    if (self->rewardMessageActive != 0) {
        PaintCanvas *pc = *g_mrCanvas;
        unsigned saved = PaintCanvas_GetColor(pc);
        unsigned origColor = self->drawColor;
        PaintCanvas_SetColor(0xffffffff);

        // Pulse alpha based on field 0x3d0 (animation timer).
        int t = self->rewardMessageTimer;
        float a;
        if (t < 2000) {
            a = (float)t / 6000.0f;
        } else if (t > 5000) {
            a = (float)(7000 - t) / 6000.0f;
        } else {
            a = 1.0f;
        }
        unsigned col = (unsigned)((int)(a * 256.0f)) - 0x100;
        PaintCanvas_SetColor(col);

        unsigned newColor = PaintCanvas_GetColor(pc);
        int boxW = self->field_0x3e8;
        int boxH = self->field_0x3ec;
        int boxX = self->field_0x3f0;
        int boxY = self->field_0x3f4;
        self->drawColor = newColor;

        if (transition != 0) {
            int sw = *g_mrDimA;
            unsigned char s0[sizeof(String12)] __attribute__((aligned(4)));
            String_cstr_ctor(s0, g_mrLit0, false);
            Layout_drawBoxStr(pc, 2, (sw >> 1) - (boxH >> 1), boxX, boxH, boxW, s0);
            ((String *)(s0))->dtor();

            sw = *g_mrDimA;
            unsigned char s1[sizeof(String12)] __attribute__((aligned(4)));
            String_cstr_ctor(s1, g_mrLit1, false);
            Layout_drawBoxStr(pc, 8, (sw >> 1) - (boxH >> 1), boxX, boxH, boxW, s1);
            ((String *)(s1))->dtor();
        }

        int sh = *g_mrDimB;
        PaintCanvas_DrawImage2D5(*g_mrCanvas, self->field_0x3a4, sh >> 1,
                                 (char)boxX, 0x11);

        void *txt = gGameText->obj->getText(*g_mrTextId);
        unsigned char line[sizeof(String12)] __attribute__((aligned(4)));
        String_copy_ctor(line, txt, false);

        sh = *g_mrDimB;
        void *font = *g_mrFont;
        unsigned cv = *(unsigned *)g_mrCanvas;
        int tw = PaintCanvas_GetTextWidth(cv, font);
        PaintCanvas_DrawString(cv, font, line, (sh >> 1) - (tw >> 1), boxX + boxY);

        unsigned char suffix[sizeof(String12)] __attribute__((aligned(4)));
        String_cstr_ctor(suffix, g_mrLit2, false);
        unsigned char credits[sizeof(String12)] __attribute__((aligned(4)));
        Layout_formatCredits(credits, self->rewardCredits);
        unsigned char joined[sizeof(String12)] __attribute__((aligned(4)));
        String_concat(joined, suffix, credits);
        ((String *)(line))->assign((String *)joined);
        ((String *)(joined))->dtor();
        ((String *)(credits))->dtor();
        ((String *)(suffix))->dtor();

        sh = *g_mrDimB;
        cv = *(unsigned *)g_mrCanvas;
        tw = PaintCanvas_GetTextWidth(cv, font);
        PaintCanvas_DrawString(cv, font, line, (sh >> 1) - (tw >> 1),
                               self->field_0x3f8 + boxX);
        PaintCanvas_SetColor(saved);
        self->drawColor = origColor;
        ((String *)(line))->dtor();
    }
    
}
