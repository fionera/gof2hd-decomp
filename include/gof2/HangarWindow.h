#ifndef GOF2_HANGARWINDOW_H
#define GOF2_HANGARWINDOW_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 - HangarWindow class. Android libgof2hdaa.so, armv7 Thumb.
// Top-level class (no AbyssEngine namespace). Field offsets recovered per-method from the
// target disassembly; we do NOT model a full layout - access fields via byte-offset casts.


// Opaque referenced types.
struct HangarWindow;
struct HangarList;
struct ListItem;
struct Item;
struct BluePrint;
struct Station;



// AbyssEngine::AEMath::Vector - 3 floats. (Vector is already provided by common.h.)

// 16-byte block used for NEON vld1/vst1.32 field copies in the constructor.
#ifndef GOF2_BLK16_DEFINED
#define GOF2_BLK16_DEFINED
struct Blk16 { int a, b, c, d; };
#endif

// String12 is provided by gof2/common.h.

// Typed byte-offset accessors for opaque (void*) referenced objects whose full
// layout is not modeled in this translation unit. Return references so call sites
// can read and assign. (Same pattern as RocketGun.h / Radio.h.)

// Layout - HUD layout/skin object. Defined here (rather than including gof2/Layout.h,
// which redefines placement-new and clashes with libc++'s <new>) so the window code can
// read its geometry fields through a real named struct. Mirrors gof2/Layout.h's layout.
struct Layout {
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
    int field_0x114;                    // +0x114
    int field_0x264;                    // +0x264
    int field_0x2cc;                    // +0x2cc

    // ---- methods used by HangarWindow (signatures match the call sites in
    // HangarWindow.cpp; gof2/Layout.h is intentionally NOT included here because it
    // redefines this struct and pulls in conflicting RetStr/B/I/P helpers). ----
    void drawBG();
    void drawBox(int style, int x, int y, int h, int w, void *text);
    void drawFooter();
    void drawScrollBar(int x, int y, int trackH, int pos, int range);
    int OnTouchBegin(int coord);
    int OnTouchEnd(int touch, int coord);
    int OnTouchMove(int coord);
    unsigned char helpPressed();
    void initHelpWindow(void *text);
    void resetWindowDimensions();
};

// Free-function geometry helpers from the Layout class (no `this`); declared here
// so call sites resolve without including gof2/Layout.h.
extern "C" int Layout_getHelpButtonOffset();
extern "C" int Layout_getFooterTransitionWidth();

// TouchButton - opaque; only ever reached via ((TouchButton*)ptr)->method(...).
// Declared locally (gof2/TouchButton.h is NOT included: it redefines the RetStr/B/I/P
// helpers that Station.h/RecordHandler.h also provide, and several of its method
// signatures disagree with the call sites here). Signatures mirror the call sites.
struct TouchButton {
    int OnTouchBegin(int coord);
    int OnTouchEnd(int coord);
    int OnTouchMove(int coord);
    void draw();
    int getHeight();
    int getWidth();
    uint8_t isTouched();
    uint8_t isVisible();
    void replaceTextKeepSize(void *text);
    void resetTouch();
    void setAlwaysPressed(bool value);
    void setPosition(int x, int y, int flags);
    void setPosition2(int x, int y);
    void setSplitText(void *value);
    void setText(void *text);
    void setVisible(bool value);
};

// GameText - opaque text table; only ever reached via getText(). The decompiler
// dropped the string-key argument at every call site here, so the local declaration
// takes none (gof2/GameText.h's getText takes an int key). Returns void* (a String*).
struct GameText {
    void *getText();
};

// Field accessors via byte offset.

class HangarWindow {
public:
    uint8_t field_0x0;                  // +0x0
    void* tabButtons;                    // +0x4
    int lastDelta;                      // +0x8
    uint8_t active;                  // +0xc
    void* itemList;                   // +0x10
    void* hangarList;                   // +0x14
    void* listItemWindow;                   // +0x18
    void* choiceWindow;                   // +0x1c
    void* dialog;                   // +0x20
    void* buttons;                   // +0x24
    void* pendingMountItem;                   // +0x28
    void* pendingDemountItem;                   // +0x2c
    void* tabIcons;                   // +0x30
    uint8_t dialogActive;                 // +0x3c
    int field_0x40;                     // +0x40
    int field_0x44;                     // +0x44
    int field_0x48;                     // +0x48
    int contentWidth;                     // +0x4c
    int contentHeight;                     // +0x50
    void* columnWidths;                   // +0x54
    int viewMode;                     // +0x58
    void* selectedItem;                   // +0x68
    int holdTime;                     // +0x6c
    int repeatTimer;                     // +0x70
    int field_0x78;                     // +0x78
    unsigned int field_0x7c;            // +0x7c
    void* bluePrint;                   // +0x80
    void* bluePrintItem;                   // +0x84
    uint8_t buyMode;                 // +0x88
    uint8_t specialMode;                 // +0x89
    int savedStationAmount;                     // +0x8c
    uint16_t shipSwapPending;                // +0x90
    uint8_t field_0x91;                 // +0x91
    uint8_t field_0x92;                 // +0x92
    uint8_t sellShipPending;                 // +0x93
    int bluePrintBuyCount;                     // +0x94
    int savedCredits;                     // +0x98
    int savedLoad;                     // +0x9c
    int savedAmount;                     // +0xa0
    int savedBlueprintAmount;                     // +0xa4
    int currentLoad;                     // +0xa8
    uint8_t bluePrintPurchasePending;                 // +0xac
    uint8_t autoEquipped;                 // +0xad
    uint8_t buyCreditsActive;                 // +0xae
    uint8_t notEnoughCredits;                 // +0xaf
    uint8_t freeCreditsActive;                 // +0xb0
    uint8_t autoCompletePending;                 // +0xb1
    int scrollOffset;                     // +0xb4
    int lastTouchY;                     // +0xb8
    int scrollOffsetBackup;                     // +0xbc
    int scrollDelta;                     // +0xc0
    int field_0xc1;                     // +0xc1
    float damping;                   // +0xc4
    int field_0xc5;                     // +0xc5
    float velocity;                   // +0xc8
    int field_0xc9;                     // +0xc9
    int touchStartY;                     // +0xcc
    int field_0xcd;                     // +0xcd
    uint8_t dragging;                 // +0xd0
    uint8_t suppressTouchEnd;                 // +0xd1
    uint8_t field_0xd2;                 // +0xd2
    int currentContentHeight;                     // +0xd4
    int visibleHeight;                     // +0xd8
    int progressBarWidth;                     // +0xdc
    int progressBarHeight;                     // +0xe0
    int savedScrollOffset;                     // +0xe4
    int field_0xf0;                     // +0xf0
    int hintOffsetX;                     // +0xf4
    uint8_t autoEquipPending;                 // +0xf8
    unsigned int autoEquipIndex;            // +0xfc
    Blk16 field_0x100;                  // +0x100
    int rowSpacing;                    // +0x10c
    int buttonHeight;                    // +0x110
    int field_0x114;                    // +0x114
    int iconOffsetY;                    // +0x118
    uint8_t replaceEquipPending;                // +0x11c
    uint8_t upgradeMode;                // +0x11d
    uint8_t localBluePrint;                // +0x11e
    uint8_t listModeFlag;                // +0x11f
    int buttonWidth;                    // +0x120
    int gridButtonHeight;                    // +0x124
    int gridSpacingX;                    // +0x128
    int gridSpacingY;                    // +0x12c
    uint8_t field_0x130;                // +0x130

    // ---- methods (converted from free functions) ----
    void OnTouchBegin(int touch, int coord);
    void OnTouchEnd(int touch, int coord);
    unsigned int OnTouchMove(int touch, int coord);
    void autoEquipSecondaryWeapons(int row);
    void ctor();
    bool currentItemIsHighlighted();
    void demountItem(void *item, int slot);
    int getCurrentTab();
    float getRelativeScrollHeight();
    float getRelativeScrollStartPos();
    void hideMessage();
    int highlightItem(void *item);
    void initialize();
    bool isInSpecialMode();
    bool listMode();
    void mountItem(void *item);
    bool readyToClose();
    void refreshCargoAvailabilityForBlueprints();
    void refreshCurrentContentHeight();
    void render();
    void render3D();
    void selectItem(void *item);
    void setSellMode();
    void showCreditsBuyWindow();
    void showFreeCreditsWindow();
    void transaction(bool buy);
    void update(int delta);
};
#endif
