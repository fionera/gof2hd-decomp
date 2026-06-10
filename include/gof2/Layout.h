#ifndef GOF2_LAYOUT_H
#define GOF2_LAYOUT_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 — Layout class (Android libgof2hdaa.so, armv7 Thumb).
// Top-level class (no AbyssEngine:: namespace; only some field/arg types live in
// AbyssEngine). Field offsets recovered from per-method target disassembly. We do
// NOT model a full layout — methods access fields via byte-offset casts from `this`.
//
// Known field offsets (partial; large object ~0x414 bytes):
//   0x3b0 int   drawColor          0x3bc int  helpButtonId/width arg
//   0x3c0 uint8 helpPressed flag   0x3c4 ptr  help window/button
//   0x400 uint8 fading flag        0x401 uint8 fadeOut flag
//   0x404 uint  fadeColor          0x408 int  fadeProgress   0x40c int fadeDuration
//   0x410 uint8 fillScreen flag


// Layout owns three TouchButton* (0x3b4/0x3b8/0x3bc) and a ChoiceWindow* (0x3c4),
// each deleted in the destructor. Declared so it demangles to Layout::~Layout();
// all other methods are authored as extern "C" wrappers.


// AbyssEngine::String — 12-byte object passed by value as a trivially-copied
// aggregate; heap members modeled opaquely.


namespace AbyssEngine {
// AbyssEngine::String — 12-byte object. Modeled with the ctors/dtor we need so the
// compiler emits the engine's String(const String&, bool) copy-ctor and ~String()
// at the right points; bodies live in the engine (linked elsewhere).

String operator+(const String &left, const String &right);
} // namespace AbyssEngine
using AbyssEngine::String;
using AbyssEngine::String12;

// placement new is provided by <new> (pulled in via common.h's standard headers).

// Stack-protector guard (the engine resolves this to a PC-relative game global).
extern "C" void *__stack_chk_guard;
extern "C" __attribute__((noreturn)) void __stack_chk_fail(...);



// Field accessor via byte offset.

struct Layout {
    ~Layout();
    uint8_t field_0x0;                  // +0x0
    int field_0x4;                      // +0x4
    int field_0x8;                      // +0x8
    int field_0xc;                      // +0xc
    int field_0x10;                     // +0x10
    int field_0x14;                     // +0x14
    int field_0x18;                     // +0x18
    int field_0x1c;                     // +0x1c
    int field_0x20;                     // +0x20
    int field_0x24;                     // +0x24
    int field_0x28;                     // +0x28
    int field_0x2c;                     // +0x2c
    int field_0x30;                     // +0x30
    int field_0x34;                     // +0x34
    int field_0x38;                     // +0x38
    int field_0x3c;                     // +0x3c
    int field_0x40;                     // +0x40
    int field_0x44;                     // +0x44
    int field_0x48;                     // +0x48
    int field_0x4c;                     // +0x4c
    int field_0x50;                     // +0x50
    int field_0x54;                     // +0x54
    int field_0x58;                     // +0x58
    int field_0x5c;                     // +0x5c
    int field_0x60;                     // +0x60
    int field_0x64;                     // +0x64
    int field_0x70;                     // +0x70
    int field_0x84;                     // +0x84
    int field_0x98;                     // +0x98
    int field_0x9c;                     // +0x9c
    int field_0xa0;                     // +0xa0
    int field_0xa4;                     // +0xa4
    int field_0xa8;                     // +0xa8
    int field_0xac;                     // +0xac
    int field_0xcc;                     // +0xcc
    int field_0x114;                    // +0x114
    int field_0x1f0;                    // +0x1f0
    int field_0x1f4;                    // +0x1f4
    int field_0x1f8;                    // +0x1f8
    int field_0x200;                    // +0x200
    int field_0x238;                    // +0x238
    int field_0x264;                    // +0x264
    int field_0x278;                    // +0x278
    int field_0x27c;                    // +0x27c
    int field_0x280;                    // +0x280
    uint8_t field_0x284;                // +0x284
    uint8_t field_0x285;                // +0x285
    uint8_t field_0x286;                // +0x286
    int field_0x288;                    // +0x288
    int field_0x28c;                    // +0x28c
    int field_0x290;                    // +0x290
    int field_0x2bc;                    // +0x2bc
    int field_0x2cc;                    // +0x2cc
    int field_0x2d4;                    // +0x2d4
    int field_0x2d8;                    // +0x2d8
    int field_0x2dc;                    // +0x2dc
    int field_0x2e0;                    // +0x2e0
    int field_0x2e4;                    // +0x2e4
    int field_0x2e8;                    // +0x2e8
    uint8_t field_0x2ec;                // +0x2ec
    uint8_t field_0x2ed;                // +0x2ed
    int field_0x2f0;                    // +0x2f0
    int field_0x2f4;                    // +0x2f4
    int field_0x2f8;                    // +0x2f8
    int field_0x2fc;                    // +0x2fc
    int field_0x300;                    // +0x300
    int field_0x304;                    // +0x304
    int field_0x308;                    // +0x308
    int field_0x30c;                    // +0x30c
    int field_0x310;                    // +0x310
    int field_0x314;                    // +0x314
    int field_0x318;                    // +0x318
    int field_0x31c;                    // +0x31c
    int field_0x320;                    // +0x320
    unsigned field_0x324;               // +0x324
    int field_0x328;                    // +0x328
    int field_0x32c;                    // +0x32c
    int field_0x330;                    // +0x330
    unsigned field_0x334;               // +0x334
    unsigned field_0x338;               // +0x338
    int field_0x33c;                    // +0x33c
    unsigned field_0x344;               // +0x344
    unsigned field_0x348;               // +0x348
    unsigned field_0x34c;               // +0x34c
    unsigned field_0x350;               // +0x350
    unsigned field_0x354;               // +0x354
    unsigned field_0x358;               // +0x358
    unsigned field_0x35c;               // +0x35c
    unsigned field_0x360;               // +0x360
    unsigned field_0x364;               // +0x364
    unsigned field_0x368;               // +0x368
    unsigned field_0x36c;               // +0x36c
    unsigned field_0x370;               // +0x370
    int field_0x374;                    // +0x374
    unsigned field_0x378;               // +0x378
    unsigned field_0x380;               // +0x380
    int field_0x384;                    // +0x384
    unsigned field_0x388;               // +0x388
    unsigned field_0x38c;               // +0x38c
    unsigned field_0x390;               // +0x390
    int field_0x394;                    // +0x394
    int field_0x398;                    // +0x398
    unsigned field_0x39c;               // +0x39c
    int field_0x3a0;                    // +0x3a0
    int field_0x3a4;                    // +0x3a4
    int field_0x3a8;                    // +0x3a8
    int field_0x3ac;                    // +0x3ac
    unsigned field_0x3b0;               // +0x3b0
    void* field_0x3b4;                  // +0x3b4
    void* field_0x3b8;                  // +0x3b8
    void* field_0x3bc;                  // +0x3bc
    uint8_t field_0x3c0;                // +0x3c0
    void* field_0x3c4;                  // +0x3c4
    int* field_0x3c8;                   // +0x3c8 (tip line-array; deref'd for line count)
    uint8_t field_0x3cc;                // +0x3cc
    int field_0x3d0;                    // +0x3d0
    int field_0x3d4;                    // +0x3d4
    int field_0x3d8;                    // +0x3d8
    uint8_t field_0x3dc;                // +0x3dc
    int field_0x3e0;                    // +0x3e0
    int field_0x3e4;                    // +0x3e4
    int field_0x3e8;                    // +0x3e8
    int field_0x3ec;                    // +0x3ec
    int field_0x3f0;                    // +0x3f0
    int field_0x3f4;                    // +0x3f4
    int field_0x3f8;                    // +0x3f8
    int field_0x3fc;                    // +0x3fc
    uint8_t field_0x400;                // +0x400
    uint8_t field_0x401;                // +0x401
    uint32_t field_0x404;               // +0x404
    int field_0x408;                    // +0x408
    int field_0x409;                    // +0x409
    int field_0x40c;                    // +0x40c
    int field_0x40d;                    // +0x40d
    uint8_t field_0x410;                // +0x410

    // ---- methods (converted from free functions) ----
    int OnTouchBegin(int x, int y);
    int OnTouchEnd(int x, int y);
    int OnTouchMove(int x, int y);
    void ctor();
    void drawBG();
    void drawBGBorder(unsigned corner, unsigned edge, int x, int y, int w, int h, int inset, int pad);
    void drawBGPattern(unsigned img, int x, int y, int w, int h);
    void drawBox(int style, int x, int y, int w, int h, void *text, unsigned flags);
    void drawBox6(int p2, int p3, int p4, int p5, int p6, const void *str);
    void drawEmptyFooter(int showBack);
    uint8_t drawFade();
    void drawFooter();
    void drawFooterImpl(int stationMode, int showBack);
    void drawFooterNoBackButton();
    void drawFooterStation();
    void drawHeader0();
    void drawHeader1(const void *param);
    void drawHeader7(void *title, int transition);
    void drawHelpWindow();
    int drawMask4(int p1, int p2, int p3, int p4);
    void drawMissionRewardMessage(int transition);
    void drawScrollBar(int x, int y, int trackH, int pos, int range);
    void drawTip();
    void drawWindow1(const void *param);
    void drawWindow2(const void *param, int flag);
    void drawWindow5(const void *param, int p3, int p4, int p5, int p6);
    void drawWindow7(void *title, int x, int y, int w, int h, int drawBG);
    void enableFillScreen(bool v);
    int getFooterTransitionWidth();
    int getHelpButtonOffset();
    float getPulseValue(float speed);
    uint8_t helpPressed();
    void initHelpWindow(void *text);
    void initTip();
    uint8_t isFading();
    void reload();
    void resetWindowDimensions();
    void setDrawColor(int color);
    void setWindowDimensions(int p1, int p2, int p3, int p4);
    void showMissionRewardMessage(int show, bool flag);
    void startFade(uint8_t fadeOut, int color, int duration);
    void update(int dt);
};
#endif
