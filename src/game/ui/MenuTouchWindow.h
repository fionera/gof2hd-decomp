#ifndef GOF2_MENUTOUCHWINDOW_H
#define GOF2_MENUTOUCHWINDOW_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "game/ui/TouchButton.h"
#include "fieldaccess.h"
#include "aetypes.h"

// Opaque application / module-transition holder. The first slot (+0x0) is a
// transition callback the menu invokes to switch the active app module. The
// holder is reached as *(void**)gXxxTransition and its first 4 bytes are read
// as a function pointer (ARM32). Two call shapes share offset 0:
//   void(*)(void*, int)        - startValkyrie / startSupernova  (mode arg)
//   void(*)(void*, int, int)   - startGOF2                       (two args)
struct ModuleTransitionThunk {
    union {
        void (*transitionFn)(void *app, int mode);      // +0x0 (2-arg shape)
        void (*transitionFn3)(void *app, int a, int b); // +0x0 (3-arg shape)
        void *field_0x0;                                // +0x0 raw alias
    };
};

// Opaque refresh-thunk holder. The first slot (+0x0) is a no-arg callback the
// menu invokes after (re)loading the preview records to refresh the visible
// list. Reached as *(void**)gPrevRefreshThunk and its first 4 bytes are read as
// a function pointer (ARM32): loadPreviewRecords does
//   (*(code *)(thunkSlot))();   // void(*)()
struct RefreshThunk {
    union {
        void (*refreshFn)();  // +0x0 no-arg refresh callback
        void *field_0x0;      // +0x0 raw alias
    };
};

class MenuTouchWindow {
public:
    uint8_t cinematicSteerActive; // steer engaged (set when Layout consumes touch in state 0xd)
    uint8_t pendingActivate; // +0x1 set when Layout consumes touch in state 0xd, cleared at OnTouchEnd start
    uint8_t pad_0x2[2];
    Array<void *> *buttons; // Array<TouchButton*>* top-level menu buttons
    void *cinematicTouchIdA; // cinematic touch id A (r3 handle)
    void *cinematicTouchIdB; // cinematic touch id B (r3 handle)
    uint8_t pad_0x10[4];
    void *cinematicBtnA; // TouchButton* (cinematic A)
    void *cinematicBtnB; // TouchButton* (cinematic B)
    uint8_t pad_0x1c[0x74];
    int cinematicAnchorA; // cinematic touch x anchor A
    int cinematicAnchorB; // cinematic touch x anchor B
    unsigned short cinematicTouchState; // +0x98 cinematic touch state bits
    uint8_t pad_0x9a[2];
    Array<void *> *previewStrings0; // Array<String*>* (load/save text rows)
    Array<void *> *previewStrings1; // Array<String*>*
    uint8_t pad_0xa4[8];
    Array<void *> *optionsButtons; // Array<TouchButton*>* (state 3)
    Array<void *> *buttonsB0; // Array<TouchButton*>* (state 0xe)
    Array<void *> *buttonsB4; // Array<TouchButton*>* (state 0xc)
    Array<void *> *buttonsB8; // Array<TouchButton*>* (state 0x11)
    void *previewRecords; // Array<GameRecord*>* (save slot previews)
    Array<void *> *scrollEntries; // Array<TouchButton*>* (state 4/0xf entries)
    void *okButton; // TouchButton* (load/save OK / row button)
    void *backButton; // TouchButton* (load/save back)
    void *optBtnCC; // TouchButton* (options widget)
    void *optBtnD0; // TouchButton*
    void *optBtnD4; // TouchButton*
    void *optBtnD8; // TouchButton*
    void *optBtnDC; // TouchButton*
    uint8_t pad_0xe0[4];
    void *scrollUpButton; // TouchButton* (state 3 list scroll-up)
    void *scrollExtraButton; // TouchButton* (optional extra button)
    void *sliders; // Array<TouchSlider*>* (options sliders)
    void *scrollWindowA; // ScrollTouchWindow* (state 4)
    void *scrollWindowB; // ScrollTouchWindow* (state 0xf/0x10)
    Array<void *> *scrollSlots; // Array<TouchButton*>* (state 0xf slots)
    void *missionsWindow; // MissionsWindow* (state 9)
    Array<void *> *recordRows; // Array<Array<String*>*>* save-slot text rows
    void *choiceWindow; // ChoiceWindow* (modal dialog)
    uint8_t upButtonPressed; // up-arrow pressed
    uint8_t downButtonPressed; // down-arrow pressed
    uint8_t pad_0x10a[0x12];
    uint32_t scrollbarImageId; // image id for scroll strip / thumb
    int field_0x120; // init -1 (selection/highlight cache)
    uint8_t pad_0x124[0x10];
    void *heapBufA; // heap byte-buffer (operator delete[])
    void *heapBufB; // heap byte-buffer (operator delete[])
    uint8_t pad_0x13c[0x18];
    int listEntryWidth; // state 3 hit-test width
    int listEntryHeight; // state 3 hit-test height
    uint8_t pad_0x15c[0xc];
    int backgroundEnabled; // menuType / draw-bg flag (ctor arg)
    int menuState; // current menu state
    uint8_t messageShowing; // a ChoiceWindow dialog is up
    uint8_t pad_0x171[2];
    uint8_t saveDialogShowing; // +0x173 saved-game dialog flag
    uint8_t returnToMenuSubFlag; // +0x174 return-to-menu sub-flag
    uint8_t pad_0x175[2];
    uint8_t quitConfirmShowing; // quit-confirmation dialog
    uint8_t returnToMenuShowing; // return-to-menu confirmation
    uint8_t leaderboardDialogShowing; // leaderboard report dialog
    uint8_t dlcMessageShowing; // DLC/purchase dialog (busy)
    uint8_t pad_0x17b;
    uint8_t dlcResultDialogShowing; // DLC purchase-result dialog
    uint8_t dlcErrorDialogShowing; // +0x17d DLC error dialog
    uint8_t loadFailedDialogShowing; // +0x17e load-failed dialog
    uint8_t genericConfirmA; // +0x17f generic confirm A
    uint8_t supernovaMessageShowing; // supernova nag dialog
    uint8_t supernovaPurchaseDialogShowing; // +0x181 supernova-purchase dialog
    uint8_t pad_0x182[2];
    int screenshotState; // -1 while making screenshot
    uint8_t pad_0x188;
    uint8_t storeInitDialogShowing; // +0x189 store-init dialog flag
    uint8_t pad_0x18a[2];
    int selectedRow; // currently selected list row
    uint8_t purchaseRestorePending; // +0x190 purchase-restore pending
    uint8_t genericConfirmB; // +0x191 generic confirm B
    uint8_t pad_0x192[2];
    int scrollOffset; // scroll Y offset (also read as float)
    int listX; // list left coordinate
    int listTopY; // list top Y
    int listBottomY; // list bottom Y span
    uint32_t fadeValue; // saved fade value (-> status +0x2c)
    int buttonWidth; // default button width
    int buttonYBias; // y bias
    int buttonRowGap; // button row gap
    int listRowGap; // list row gap
    int metricA; // layout metric
    int metricB; // layout metric
    int metricC; // layout metric
    uint8_t field_0x1c4; // init-only flag
    uint8_t pad_0x1c5[0x14];
    uint8_t scrollbarHit; // +0x1d9 scrollbar-hit flag (state 0xf)
    uint8_t pad_0x1da;
    uint8_t listStateSuppress; // +0x1db list-state suppress flag
    uint8_t pad_0x1dc[4];
    int field_0x1e0; // init-only
    int contentHeightCache; // image height cache (state 0xf)
    uint8_t pad_0x1e8[0x24];
    int dragLastX; // last drag x (window-relative)
    uint8_t pad_0x210[4];
    int dragVelocity; // drag delta accumulator
    uint32_t inertiaDecay; // inertia decay factor (float bits)
    float inertiaVel; // inertia velocity
    int dragStartX; // drag start x
    uint8_t dragging; // active drag flag
    uint8_t pad_0x225[3];
    int contentHeight; // scroll content height
    int pageHeight; // scroll page height
    int field_0x230; // init-only
    int field_0x234; // init-only
    uint8_t cutsceneMode; // cutscene-mode flag

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

    int OnTouchEnd(int y, int x, void *touchId);

    void createRecordButtons(bool inSaveMode);

    void startValkyrie();

    int OnTouchBegin(int y, int x, void *touchId);

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

    int OnTouchMove(int y, int x, void *touchId);

    void setSkipButtonVisible(bool visible);

    void drawLoadSaveMenu(bool param1);

    void startSupernova();

    void startGOF2();
};
#endif
