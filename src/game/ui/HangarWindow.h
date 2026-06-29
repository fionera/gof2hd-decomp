#ifndef GOF2_HANGARWINDOW_H
#define GOF2_HANGARWINDOW_H
#include "engine/core/Array.h"
#include "../../engine/core/AEString.h"
#include "game/core/HangarList.h"
#include "game/mission/Item.h"

#include "game/ui/ChoiceWindow.h"
#include "game/ui/ListItemWindow.h"
#include "game/ui/Layout.h"

#include "game/ui/Blk16.h"
class BluePrint;
class ChoiceWindow;
class HangarList;
class Item;
class ListItem;
class ListItemWindow;
class Ship;
class TouchButton;

class HangarWindow {
public:
    uint8_t field_0x0;
    Array<TouchButton *> *tabButtons;
    int lastDelta;
    uint8_t active;
    Array<Item *> *itemList;
    HangarList *hangarList;
    ListItemWindow *listItemWindow;
    ChoiceWindow *choiceWindow;
    ChoiceWindow *dialog;
    Array<TouchButton *> *buttons;
    Item *pendingMountItem;
    Item *pendingDemountItem;
    unsigned int *tabIcons;
    unsigned int scrollHintImageA;
    unsigned int scrollHintImageB;
    uint8_t dialogActive;
    int field_0x40;
    int field_0x44;
    int field_0x48;
    int contentWidth;
    int contentHeight;
    int *columnWidths;
    int viewMode;
    uint8_t _pad_0x5c[12];   // 0x5c..0x68 (unmodeled members; restores selectedItem @0x68)
    ListItem *selectedItem;
    int holdTime;
    int repeatTimer;
    unsigned int progressBarBorderImage;
    int progressBarBgImage;
    unsigned int progressBarFillImage;
    BluePrint *bluePrint;
    ListItem *bluePrintItem;
    uint8_t buyMode;
    uint8_t specialMode;
    int savedStationAmount;
    uint16_t shipSwapPending;
    uint8_t dlcMenuPending;
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
    int scrollDelta;     // @0xc0 (init also writes &scrollDelta+1 unaligned -> ex field_0xc1)
    float damping;       // @0xc4 (ex field_0xc5: &damping+1 unaligned write)
    float velocity;      // @0xc8 (ex field_0xc9: &velocity+1 unaligned write)
    int touchStartY;     // @0xcc (ex field_0xcd: &touchStartY+1 unaligned write)
    uint8_t dragging;
    uint8_t suppressTouchEnd;
    uint8_t sellConfirmPending;
    int currentContentHeight;
    int visibleHeight;
    int progressBarWidth;
    int progressBarHeight;
    int savedScrollOffset;
    unsigned int blueprintIconImage;
    unsigned int pendingIconImage;
    int hintImage;
    int hintOffsetX;
    uint8_t autoEquipPending;
    unsigned int autoEquipIndex;
    Blk16 field_0x100;
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
    uint8_t routeWarningPending;

    HangarWindow();

    ~HangarWindow();

    void OnTouchBegin(int touch, int coord);

    void OnTouchEnd(int touch, int coord);

    unsigned int OnTouchMove(int touch, int coord);

    bool isInitialized();

    ListItem *getCurrentItem();

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

    void setSellMode(bool buy);

    void showCreditsBuyWindow();

    void showFreeCreditsWindow();

    void transaction(bool buy);

    void update(int delta);

    void buildMissionOffer(int touch, int coord);

    static Ship *statusShip();

    static void render3D_thunk(ListItemWindow *listItemWindow);

    static int lastTab;
};

#if __SIZEOF_POINTER__ == 4
// Former field_0xc1/c5/c9/cd were unaligned-write artifacts (init writes &scrollDelta+1 ...)
// over the real fields scrollDelta/damping/velocity/touchStartY; they are not separate members.
static_assert(__builtin_offsetof(HangarWindow, scrollDelta) == 0xc0, "HangarWindow::scrollDelta (ex field_0xc1-1)");
static_assert(__builtin_offsetof(HangarWindow, damping) == 0xc4, "HangarWindow::damping (ex field_0xc5-1)");
static_assert(__builtin_offsetof(HangarWindow, velocity) == 0xc8, "HangarWindow::velocity (ex field_0xc9-1)");
static_assert(__builtin_offsetof(HangarWindow, touchStartY) == 0xcc, "HangarWindow::touchStartY (ex field_0xcd-1)");
static_assert(__builtin_offsetof(HangarWindow, field_0x100) == 0x100, "HangarWindow::field_0x100");
static_assert(__builtin_offsetof(HangarWindow, buttonHeight) == 0x110, "HangarWindow::buttonHeight");
static_assert(__builtin_offsetof(HangarWindow, field_0x114) == 0x114, "HangarWindow::field_0x114");
static_assert(__builtin_offsetof(HangarWindow, iconOffsetY) == 0x118, "HangarWindow::iconOffsetY");
#endif
#endif
