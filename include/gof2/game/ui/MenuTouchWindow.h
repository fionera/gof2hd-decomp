#ifndef GOF2_MENUTOUCHWINDOW_H
#define GOF2_MENUTOUCHWINDOW_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 -- MenuTouchWindow (Android libgof2hdaa.so, armv7 Thumb).
// Qualified target name is top-level: "MenuTouchWindow::..." (class not in a namespace;
// only its argument types are, e.g. AbyssEngine::...).
// Field offsets recovered per-method from the target disassembly; previously accessed via
// byte-offset casts, now modelled as named typed members in offset order (natural 64-bit
// layout matching the 32-bit target, // +0xNN comments). MenuTouchWindow shares the
// ScrollTouchWindow base header region; the few base-region fields it touches in the
// cinematic state (case 0xb) are named here in offset order -- ScrollTouchWindow.h is
// NOT edited. Offsets whose meaning is not recoverable keep deterministic field_0xNN names.

void *operator new(__SIZE_TYPE__ size);
void operator delete(void *ptr) noexcept;

// Field accessors --------------------------------------------------------------

class MenuTouchWindow {
public:
    uint8_t        cinematicSteerActive;     // +0x000 steer engaged (set when Layout consumes touch in state 0xd)
    uint8_t        field_0x1;                // +0x001 flag cleared at OnTouchEnd start
    uint8_t        pad_0x2[2];               // +0x002
    Array<void *> *buttons;                  // +0x004 Array<TouchButton*>* top-level menu buttons
    int            cinematicTouchIdA;        // +0x008 base-region: cinematic touch id A
    int            cinematicTouchIdB;        // +0x00c base-region: cinematic touch id B
    uint8_t        pad_0x10[4];              // +0x010
    void          *cinematicBtnA;            // +0x014 base-region: TouchButton* (cinematic A)
    void          *cinematicBtnB;            // +0x018 base-region: TouchButton* (cinematic B)
    uint8_t        pad_0x1c[0x74];           // +0x01c region not accessed by name
    int            cinematicAnchorA;         // +0x090 cinematic touch x anchor A
    int            cinematicAnchorB;         // +0x094 cinematic touch x anchor B
    unsigned short field_0x98;               // +0x098 cinematic touch state bits
    uint8_t        pad_0x9a[2];              // +0x09a
    Array<void *> *previewStrings0;          // +0x09c Array<String*>* (load/save text rows)
    Array<void *> *previewStrings1;          // +0x0a0 Array<String*>*
    uint8_t        pad_0xa4[8];              // +0x0a4
    Array<void *> *optionsButtons;           // +0x0ac Array<TouchButton*>* (state 3)
    Array<void *> *buttonsB0;                // +0x0b0 Array<TouchButton*>* (state 0xe)
    Array<void *> *buttonsB4;                // +0x0b4 Array<TouchButton*>* (state 0xc)
    Array<void *> *buttonsB8;                // +0x0b8 Array<TouchButton*>* (state 0x11)
    void          *previewRecords;           // +0x0bc Array<GameRecord*>* (save slot previews)
    Array<void *> *scrollEntries;            // +0x0c0 Array<TouchButton*>* (state 4/0xf entries)
    void          *okButton;                 // +0x0c4 TouchButton* (load/save OK / row button)
    void          *backButton;               // +0x0c8 TouchButton* (load/save back)
    void          *optBtnCC;                 // +0x0cc TouchButton* (options widget)
    void          *optBtnD0;                 // +0x0d0 TouchButton*
    void          *optBtnD4;                 // +0x0d4 TouchButton*
    void          *optBtnD8;                 // +0x0d8 TouchButton*
    void          *optBtnDC;                 // +0x0dc TouchButton*
    uint8_t        pad_0xe0[4];              // +0x0e0
    void          *scrollUpButton;           // +0x0e4 TouchButton* (state 3 list scroll-up)
    void          *scrollExtraButton;        // +0x0e8 TouchButton* (optional extra button)
    void          *sliders;                  // +0x0ec Array<TouchSlider*>* (options sliders)
    void          *scrollWindowA;            // +0x0f0 ScrollTouchWindow* (state 4)
    void          *scrollWindowB;            // +0x0f4 ScrollTouchWindow* (state 0xf/0x10)
    Array<void *> *scrollSlots;              // +0x0f8 Array<TouchButton*>* (state 0xf slots)
    void          *missionsWindow;           // +0x0fc MissionsWindow* (state 9)
    Array<void *> *recordRows;               // +0x100 Array<Array<String*>*>* save-slot text rows
    void          *choiceWindow;             // +0x104 ChoiceWindow* (modal dialog)
    uint8_t        upButtonPressed;          // +0x108 up-arrow pressed
    uint8_t        downButtonPressed;        // +0x109 down-arrow pressed
    uint8_t        pad_0x10a[0x12];          // +0x10a
    uint32_t       scrollbarImageId;         // +0x11c image id for scroll strip / thumb
    int            field_0x120;              // +0x120 init -1 (selection/highlight cache)
    uint8_t        pad_0x124[0x10];          // +0x124
    void          *heapBufA;                 // +0x134 heap byte-buffer (operator delete[])
    void          *heapBufB;                 // +0x138 heap byte-buffer (operator delete[])
    uint8_t        pad_0x13c[0x18];          // +0x13c
    int            listEntryWidth;           // +0x154 state 3 hit-test width
    int            listEntryHeight;          // +0x158 state 3 hit-test height
    uint8_t        pad_0x15c[0xc];           // +0x15c
    int            backgroundEnabled;        // +0x168 menuType / draw-bg flag (ctor arg)
    int            menuState;                // +0x16c current menu state
    uint8_t        messageShowing;           // +0x170 a ChoiceWindow dialog is up
    uint8_t        pad_0x171[2];             // +0x171
    uint8_t        field_0x173;              // +0x173 saved-game dialog flag
    uint8_t        field_0x174;              // +0x174 return-to-menu sub-flag
    uint8_t        pad_0x175[2];             // +0x175
    uint8_t        quitConfirmShowing;       // +0x177 quit-confirmation dialog
    uint8_t        returnToMenuShowing;      // +0x178 return-to-menu confirmation
    uint8_t        leaderboardDialogShowing; // +0x179 leaderboard report dialog
    uint8_t        dlcMessageShowing;        // +0x17a DLC/purchase dialog (busy)
    uint8_t        pad_0x17b;                // +0x17b
    uint8_t        dlcResultDialogShowing;   // +0x17c DLC purchase-result dialog
    uint8_t        field_0x17d;              // +0x17d DLC error dialog
    uint8_t        field_0x17e;              // +0x17e load-failed dialog
    uint8_t        field_0x17f;              // +0x17f generic confirm A
    uint8_t        supernovaMessageShowing;  // +0x180 supernova nag dialog
    uint8_t        field_0x181;              // +0x181 supernova-purchase dialog
    uint8_t        pad_0x182[2];             // +0x182
    int            screenshotState;          // +0x184 -1 while making screenshot
    uint8_t        pad_0x188;                // +0x188
    uint8_t        field_0x189;              // +0x189 store-init dialog flag
    uint8_t        pad_0x18a[2];             // +0x18a
    int            selectedRow;              // +0x18c currently selected list row
    uint8_t        field_0x190;              // +0x190 purchase-restore pending
    uint8_t        field_0x191;              // +0x191 generic confirm B
    uint8_t        pad_0x192[2];             // +0x192
    int            scrollOffset;             // +0x194 scroll Y offset (also read as float)
    int            listX;                    // +0x198 list left coordinate
    int            listTopY;                 // +0x19c list top Y
    int            listBottomY;              // +0x1a0 list bottom Y span
    uint32_t       fadeValue;                // +0x1a4 saved fade value (-> status +0x2c)
    int            buttonWidth;              // +0x1a8 default button width (layout +0x294)
    int            buttonYBias;              // +0x1ac y bias (layout +0x298)
    int            buttonRowGap;             // +0x1b0 button row gap (layout +0x29c)
    int            listRowGap;               // +0x1b4 list row gap (layout +0x2a0)
    int            metricA;                  // +0x1b8 layout metric (+0x2a4)
    int            metricB;                  // +0x1bc layout metric (+0x2a8)
    int            metricC;                  // +0x1c0 layout metric (+0x2ac)
    uint8_t        field_0x1c4;              // +0x1c4 init-only flag
    uint8_t        pad_0x1c5[0x14];          // +0x1c5
    uint8_t        field_0x1d9;              // +0x1d9 scrollbar-hit flag (state 0xf)
    uint8_t        pad_0x1da;                // +0x1da
    uint8_t        field_0x1db;              // +0x1db list-state suppress flag
    uint8_t        pad_0x1dc[4];             // +0x1dc
    int            field_0x1e0;              // +0x1e0 init-only
    int            contentHeightCache;       // +0x1e4 image height cache (state 0xf)
    uint8_t        pad_0x1e8[0x24];          // +0x1e8 (covers Vector4 blob zeroed from +0x215)
    int            dragLastX;                // +0x20c last drag x (window-relative)
    uint8_t        pad_0x210[4];             // +0x210
    int            dragVelocity;             // +0x214 drag delta accumulator
    uint32_t       inertiaDecay;             // +0x218 inertia decay factor (float bits)
    float          inertiaVel;               // +0x21c inertia velocity
    int            dragStartX;               // +0x220 drag start x
    uint8_t        dragging;                 // +0x224 active drag flag
    uint8_t        pad_0x225[3];             // +0x225
    int            contentHeight;            // +0x228 scroll content height
    int            pageHeight;               // +0x22c scroll page height
    int            field_0x230;              // +0x230 init-only
    int            field_0x234;              // +0x234 init-only
    uint8_t        cutsceneMode;             // +0x238 cutscene-mode flag

    // Methods (declarations consolidated from per-method merge artifacts).
    void showSupernovaMessage();
    bool isInMissionScreen();
    uint8_t isShowingMessage();
    bool isMakingScreenshot();
    void hideMessage();
    void render3D();
    bool inCinematicMode();
    float getRelativeScrollStartPos();
    int OnTouchEnd(int y, int x);
    MenuTouchWindow *dtor();
    void createRecordButtons(bool inSaveMode);
    void startValkyrie();
    int OnTouchBegin(int y, int x, int touchId);
    int loadGame(int slot);
    void addButton(int id, void *label, int row, void *arr, int yOff);
    void setCutsceneMode(bool mode);
    void loadPreviewRecords();
    void saveGame(int slot);
    void update(int dt);
    void startSupernovaChallenge();
    void callDlcMenu();
    void draw();
    float getRelativeScrollHeight();
    int OnTouchMove(int y, int x);
    void ctor(int menuType);
    void setSkipButtonVisible(bool visible);
    void drawLoadSaveMenu(bool param1);
    void startSupernova();
    void startGOF2();
};
#endif
