#ifndef GOF2_MODMAINMENU_H
#define GOF2_MODMAINMENU_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"

// Galaxy on Fire 2 — ModMainMenu (main-menu / title screen module). Top-level class,
// Several engine objects are referenced by pointer only and kept opaque.
struct CutScene;
struct MenuTouchWindow;

class ModMainMenu {
public:
    int              paintCanvas;  // paint canvas handle
    void*            appManager;    // ApplicationManager*
    int              state;         // state-machine state (100 = idle)
    uint8_t          initialized;   // init-complete flag
    int              frameTime;     // clamped frame delta (ms)
    MenuTouchWindow* touchWindow;   // touch-driven menu window
    CutScene*        cutScene;      // rotating cut-scene backdrop
    int              logoImage;     // logo image handle
    int              fadeTimer;     // logo fade accumulator (ms)
    uint8_t          logoActive;    // logo / touch-lock active flag
    uint8_t          hasSavedGame;  // saved-game preview present

    ModMainMenu();
    ~ModMainMenu();

    void OnInitialize();
    void OnUpdate();
    void OnRelease();
    void OnResume();
    void OnSuspend();
    void OnRender2D();
    void OnRender3D();
    void OnTouchBegin(int x, int y, void* touch);
    void OnTouchMove(int x, int y, void* touch);
    void OnTouchEnd(int x, int y, void* touch);
};
#endif
