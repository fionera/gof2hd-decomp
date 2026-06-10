#ifndef GOF2_MENUTOUCHWINDOW_H
#define GOF2_MENUTOUCHWINDOW_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 -- MenuTouchWindow (Android libgof2hdaa.so, armv7 Thumb).
// Qualified target name is top-level: "MenuTouchWindow::..." (class not in a namespace;
// only its argument types are, e.g. AbyssEngine::...).
// Field offsets recovered per-method from the target disassembly; accessed via byte-offset casts.


void *operator new(__SIZE_TYPE__ size);
void operator delete(void *ptr) noexcept;

// Field accessors --------------------------------------------------------------

struct MenuTouchWindow {
    unsigned* field_0x4;                // +0x4
    unsigned short field_0x98;          // +0x98
    unsigned short field_0x108;         // +0x108
    unsigned short field_0x17a;         // +0x17a

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
