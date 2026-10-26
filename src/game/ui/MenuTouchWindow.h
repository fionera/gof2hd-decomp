#ifndef GOF2_MENUTOUCHWINDOW_H
#define GOF2_MENUTOUCHWINDOW_H
#include "engine/core/Array.h"
#include "../../engine/core/AEString.h"
#include "game/ui/TouchButton.h"
class TouchButton;
class TouchSlider;
class GameRecord;
class ChoiceWindow;
class MissionsWindow;
class ScrollTouchWindow;

class MenuTouchWindow {
public:
    uint8_t cinematicSteerActive;
    uint8_t pendingActivate;
    uint8_t pad_0x2[2];
    Array<TouchButton *> *buttons;
    void *cinematicTouchIdA; // lint: void_ptr (opaque touch handle; matches exported void* touchId)
    void *cinematicTouchIdB; // lint: void_ptr (opaque touch handle; matches exported void* touchId)
    ScrollTouchWindow *cinematicTextWindow;
    TouchButton *cinematicBtnA;
    TouchButton *cinematicBtnB;
    // Touch-control preview (steering pad / fire buttons), laid out by Globals::setCoordsSteer/Fire.
    uint32_t steerImageA;
    uint32_t steerImageB;
    uint32_t steerImageC;
    uint32_t steerImageD;
    uint8_t pad_0x2c[2];
    uint16_t steerCoord_0x2e;
    uint16_t steerCoord_0x30;
    uint16_t steerCoord_0x32;
    uint16_t steerCoord_0x34;
    uint8_t pad_0x36[6];
    uint16_t steerCoord_0x3c;
    uint16_t steerCoord_0x3e;
    uint16_t steerCoord_0x40;
    uint16_t steerCoord_0x42;
    uint8_t pad_0x44[4];
    uint16_t steerCoord_0x48;
    uint16_t steerCoord_0x4a;
    uint16_t fireCoord_0x4c;
    uint16_t fireCoord_0x4e;
    uint32_t fireImage_0x50;
    uint32_t fireImage_0x54;
    uint32_t fireImage_0x58;
    uint32_t fireImage_0x5c;
    uint16_t fireCoord_0x60;
    uint16_t fireCoord_0x62;
    uint16_t fireCoord_0x64;
    uint16_t fireCoord_0x66;
    uint16_t fireCoord_0x68;
    uint16_t fireCoord_0x6a;
    uint16_t fireCoord_0x6c;
    uint16_t fireCoord_0x6e;
    uint16_t fireCoord_0x70;
    uint16_t fireCoord_0x72;
    uint32_t fireSize_0x74;
    uint32_t fireImageA;
    uint32_t fireImageB;
    uint32_t fireImage_0x80;
    uint32_t fireImage_0x84;
    uint32_t fireImage_0x88;
    uint32_t fireImage_0x8c;
    int cinematicAnchorA;
    int cinematicAnchorB;
    unsigned short cinematicTouchState;
    uint8_t pad_0x9a[2];
    Array<AbyssEngine::String *> *previewStrings0;
    Array<AbyssEngine::String *> *previewStrings1;
    uint8_t pad_0xa4[8];
    Array<TouchButton *> *optionsButtons;
    Array<TouchButton *> *buttonsB0;
    Array<TouchButton *> *buttonsB4;
    Array<TouchButton *> *buttonsB8;
    Array<GameRecord *> *previewRecords;
    Array<TouchButton *> *scrollEntries;
    TouchButton *okButton;
    TouchButton *backButton;
    TouchButton *optBtnCC;
    TouchButton *optBtnD0;
    TouchButton *optBtnD4;
    TouchButton *optBtnD8;
    TouchButton *optBtnDC;
    uint8_t pad_0xe0[4];
    TouchButton *scrollUpButton;
    TouchButton *scrollExtraButton;
    Array<TouchSlider *> *sliders;
    ScrollTouchWindow *scrollWindowA;
    ScrollTouchWindow *scrollWindowB;
    Array<TouchButton *> *scrollSlots;
    MissionsWindow *missionsWindow;
    Array<Array<AbyssEngine::String *> *> *recordRows;
    ChoiceWindow *choiceWindow;
    uint8_t upButtonPressed;
    uint8_t downButtonPressed;
    uint8_t pad_0x10a[2];
    uint32_t optionsImage_0x10c;
    uint32_t optionsImage_0x110;
    uint32_t optionsImage_0x114;
    uint32_t optionsImage_0x118;
    uint32_t scrollbarImageId;
    int field_0x120;
    uint32_t image_0x124;
    uint32_t image_0x128;
    uint8_t pad_0x12c[8];
    uint32_t *heapBufA;
    uint32_t *heapBufB;
    uint32_t scrollImage_0x13c;
    uint32_t scrollImage_0x140;
    uint32_t scrollImage_0x144;
    uint32_t scrollImage_0x148;
    uint32_t borderImage_0x14c;
    int optionsWidth;
    int listEntryWidth;
    int listEntryHeight;
    int field_0x15c;
    int sliderSpacing;
    int sliderRowHeight;
    int backgroundEnabled;
    int menuState;
    uint8_t messageShowing;
    uint8_t field_0x171;
    uint8_t field_0x172;
    uint8_t saveDialogShowing;
    uint8_t returnToMenuSubFlag;
    uint8_t field_0x175;
    uint8_t field_0x176;
    uint8_t quitConfirmShowing;
    uint8_t returnToMenuShowing;
    uint8_t leaderboardDialogShowing;
    uint8_t dlcMessageShowing;
    uint8_t field_0x17b;
    uint8_t dlcResultDialogShowing;
    uint8_t dlcErrorDialogShowing;
    uint8_t loadFailedDialogShowing;
    uint8_t genericConfirmA;
    uint8_t supernovaMessageShowing;
    uint8_t supernovaPurchaseDialogShowing;
    uint8_t pad_0x182[2];
    int screenshotState;
    uint8_t field_0x188;
    uint8_t storeInitDialogShowing;
    uint8_t pad_0x18a[2];
    int selectedRow;
    uint8_t purchaseRestorePending;
    uint8_t genericConfirmB;
    uint8_t pad_0x192[2];
    int scrollOffset;
    int listX;
    int listTopY;
    int listBottomY;
    float fadeValue; // difficulty factor of a new game (0.5 easy, 1.5 hard)
    int buttonWidth;
    int buttonYBias;
    int buttonRowGap;
    int listRowGap;
    int metricA;
    int metricB;
    int metricC;
    uint8_t field_0x1c4;
    uint8_t pad_0x1c5[0x14];
    uint8_t scrollbarHit;
    uint8_t field_0x1da;
    uint8_t listStateSuppress;
    uint8_t field_0x1dc;
    uint8_t field_0x1dd;
    uint8_t field_0x1de;
    uint8_t pad_0x1df;
    int field_0x1e0;
    int contentHeightCache;
    int slotImageWidth;
    int slotX;
    int slotY;
    int slotBorderWidth;
    int slotMarkerWidth;
    int slotMarkerHeight;
    int slotListWidth;
    int slotTextWidth;
    int slotSpacing;
    int dragLastX;
    int field_0x210;
    int dragVelocity;
    float inertiaDecay;
    float inertiaVel;
    int dragStartX;
    uint8_t dragging;
    uint8_t pad_0x225[3];
    int contentHeight;
    int pageHeight;
    int field_0x230;
    int field_0x234;
    uint8_t cutsceneMode;
    int field_0x23c; // tail field: every original `new MenuTouchWindow` allocates 576 bytes

    MenuTouchWindow(int menuType);

    ~MenuTouchWindow();

    void showSupernovaMessage();

    bool isInMissionScreen();

    uint8_t isShowingMessage();

    bool isMakingScreenshot();

    void hideMessage();

    void render3D();

    bool inCinematicMode();

    float getRelativeScrollStartPos();

    int OnTouchEnd(int x, int y, void *touchId); // lint: void_ptr (exported method signature; void* is mangling-load-bearing)

    // lint: void_ptr (exported method signature; void* is mangling-load-bearing)

    void createRecordButtons(bool inSaveMode);

    void startValkyrie();

    bool OnTouchBegin(int x, int y, void *touchId); // lint: void_ptr (exported method signature; void* is mangling-load-bearing)

    // lint: void_ptr (exported method signature; void* is mangling-load-bearing)

    int loadGame(int slot);

    void addButton(int id, AbyssEngine::String label, int row, Array<TouchButton *> *arr, int yOff);

    void setCutsceneMode(bool mode);

    void loadPreviewRecords();

    void saveGame(int slot);

    void update(int dt);

    void startSupernovaChallenge();

    void callDlcMenu();

    void draw();

    float getRelativeScrollHeight();

    bool OnTouchMove(int x, int y, void *touchId); // lint: void_ptr (exported method signature; void* is mangling-load-bearing)

    // lint: void_ptr (exported method signature; void* is mangling-load-bearing)

    void setSkipButtonVisible(bool visible);

    void drawLoadSaveMenu(bool param1);

    void startSupernova();

    void startGOF2();
};
#endif
