#ifndef GOF2_HANGARWINDOW_H
#define GOF2_HANGARWINDOW_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "game/ui/ChoiceWindow.h"
#include "game/ui/ListItemWindow.h"

// Galaxy on Fire 2 - HangarWindow: the station shop / hangar screen. Owns the
// hangar item-list model, the tab/action TouchButton banks, the embedded item
// detail sub-window and the two ChoiceWindow dialogs.

class HangarList;
class ListItem;
class Item;
class BluePrint;
class Ship;

// 16-byte geometry block copied from the layout config with a single NEON store.
struct Blk16 { int a, b, c, d; };

// Layout, TouchButton and GameText keep deliberately minimal local declarations
// here. The full gof2/game/ui/Layout.h, gof2/game/ui/TouchButton.h and
// gof2/engine/core/GameText.h exist, but their cleaned method arities (two-argument
// touch handlers, keyed getText, the wider drawBox signature) do not match the
// argument lists the hangar screen calls them with - those call lists are a
// decompiler artifact of this translation unit. Until the call sites can be
// re-derived, these forwarders mirror exactly what HangarWindow.cpp invokes.
struct Layout {
    uint8_t field_0x0;
    int field_0x4;
    int field_0x8;
    int field_0xc;
    int field_0x10;
    int field_0x14;
    int field_0x18;
    int field_0x1c;
    int field_0x20;
    int field_0x24;
    int field_0x28;
    int field_0x2c;
    int field_0x30;
    int field_0x34;
    int field_0x38;
    int field_0x3c;
    int field_0x40;
    int field_0x44;
    int field_0x48;
    int field_0x4c;
    int field_0x50;
    int field_0x54;
    int field_0x58;
    int field_0x5c;
    int field_0x60;
    int field_0x64;
    int field_0x70;
    int field_0x84;
    int field_0x114;
    int field_0x264;
    int field_0x2cc;

    void drawBG();
    void drawBox(int style, int x, int y, int h, int w, void *text);
    void drawFooter();
    void drawScrollBar(int x, int y, int trackH, int pos, int range);
    int OnTouchBegin(int coord);
    int OnTouchEnd(int touch, int coord);
    int OnTouchMove(int coord);
    unsigned char helpPressed();
    static String formatCredits(int n);   // mirrors game/ui/Layout.h
    void initHelpWindow(void *text);
    void resetWindowDimensions();
};

// Geometry helpers exposed by Layout without a `this` parameter.
int Layout_getHelpButtonOffset();
int Layout_getFooterTransitionWidth();

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

// Opaque text catalogue. The decompiler dropped the string-key argument at every
// call site in this translation unit, so getText() is declared nullary here. It
// returns the resolved AbyssEngine::String entry.
struct GameText {
    AbyssEngine::String *getText();
};

class HangarWindow {
public:
    uint8_t field_0x0;
    Array<TouchButton*>* tabButtons;
    int lastDelta;
    uint8_t active;
    Array<Item*>* itemList;
    HangarList* hangarList;
    ListItemWindow* listItemWindow;
    ChoiceWindow* choiceWindow;
    ChoiceWindow* dialog;
    Array<TouchButton*>* buttons;
    Item* pendingMountItem;
    Item* pendingDemountItem;
    void* tabIcons;
    unsigned int scrollHintImageA;   // +0x34 (Image2D handle, id 0x233e)
    unsigned int scrollHintImageB;   // +0x38 (Image2D handle, id 0x233f)
    uint8_t dialogActive;
    int field_0x40;
    int field_0x44;
    int field_0x48;
    int contentWidth;
    int contentHeight;
    void* columnWidths;
    int viewMode;
    ListItem* selectedItem;
    int holdTime;
    int repeatTimer;
    unsigned int progressBarBorderImage; // +0x74 (Image2D handle, id 0x477)
    int progressBarBgImage;          // +0x78
    unsigned int progressBarFillImage; // +0x7c
    BluePrint* bluePrint;
    ListItem* bluePrintItem;
    uint8_t buyMode;
    uint8_t specialMode;
    int savedStationAmount;
    uint16_t shipSwapPending;
    uint8_t dlcMenuPending;          // +0x92 (Ghidra +0x92)
    uint8_t swapConfirmFlag;         // +0x93
    uint8_t sellShipPending;
    int bluePrintBuyCount;
    int savedCredits;
    int savedLoad;
    int savedAmount;
    int savedBlueprintAmount;
    int currentLoad;
    uint8_t bluePrintPurchasePending;
    uint8_t autoEquipped;
    uint8_t buyCreditsActive;
    uint8_t notEnoughCredits;
    uint8_t freeCreditsActive;
    uint8_t autoCompletePending;
    int scrollOffset;
    int lastTouchY;
    int scrollOffsetBackup;
    int scrollDelta;
    int field_0xc1;
    float damping;
    int field_0xc5;
    float velocity;
    int field_0xc9;
    int touchStartY;
    int field_0xcd;
    uint8_t dragging;
    uint8_t suppressTouchEnd;
    uint8_t sellConfirmPending;      // +0xd2
    int currentContentHeight;
    int visibleHeight;
    int progressBarWidth;
    int progressBarHeight;
    int savedScrollOffset;
    unsigned int blueprintIconImage; // +0xe8 (Image2D handle, id 0x52e)
    unsigned int pendingIconImage;   // +0xec (Image2D handle, id 0x544)
    int hintImage;                   // +0xf0
    int hintOffsetX;
    uint8_t autoEquipPending;
    unsigned int autoEquipIndex;
    Blk16 field_0x100;               // field_0x100.d (+0x10c) = per-row spacing
    int buttonHeight;
    int field_0x114;
    int iconOffsetY;
    uint8_t replaceEquipPending;
    uint8_t upgradeMode;
    uint8_t localBluePrint;
    uint8_t listModeFlag;
    int buttonWidth;
    int gridButtonHeight;
    int gridSpacingX;
    int gridSpacingY;
    uint8_t routeWarningPending;     // +0x130

    HangarWindow();
    ~HangarWindow();

    void OnTouchBegin(int touch, int coord);
    void OnTouchEnd(int touch, int coord);
    unsigned int OnTouchMove(int touch, int coord);
    void autoEquipSecondaryWeapons(int row);
    bool currentItemIsHighlighted();
    void demountItem(Item *item, int slot);
    int getCurrentTab();
    float getRelativeScrollHeight();
    float getRelativeScrollStartPos();
    void hideMessage();
    int highlightItem(ListItem *item);
    void initialize();
    bool isInSpecialMode();
    bool listMode();
    void mountItem(Item *item);
    bool readyToClose();
    void refreshCargoAvailabilityForBlueprints();
    void refreshCurrentContentHeight();
    void render();
    void render3D();
    void selectItem(ListItem *item);
    void setSellMode();
    void setSellMode(bool buy);
    void showCreditsBuyWindow();
    void showFreeCreditsWindow();
    void transaction(bool buy);
    void update(int delta);

    // Tail of OnTouchEnd(): the equipment swap-confirmation / buy-mode dialog flow.
    // Resolves the active ChoiceWindow's touch result and commits (or cancels) the
    // pending mount/demount.
    void buildMissionOffer(int touch, int coord);

    // The ship the hangar acts on: always the player's current ship held by the
    // global Status. Static because it depends on no per-window state.
    static Ship *statusShip();

    // render3D()'s tail-call thunk: in the embedded item-list view mode the 3D pass
    // is delegated to the item-list sub-window.
    static void render3D_thunk(ListItemWindow *listItemWindow);
};
#endif
