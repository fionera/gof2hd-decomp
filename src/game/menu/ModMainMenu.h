#ifndef GOF2_MODMAINMENU_H
#define GOF2_MODMAINMENU_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"

struct CutScene;
struct MenuTouchWindow;

class ModMainMenu {
public:
    int paintCanvas; // paint canvas handle
    void *appManager; // ApplicationManager*
    int state; // state-machine state (100 = idle)
    uint8_t initialized; // init-complete flag
    int frameTime; // clamped frame delta (ms)
    MenuTouchWindow *touchWindow; // touch-driven menu window
    CutScene *cutScene; // rotating cut-scene backdrop
    int logoImage; // logo image handle
    int fadeTimer; // logo fade accumulator (ms)
    uint8_t logoActive; // logo / touch-lock active flag
    uint8_t hasSavedGame; // saved-game preview present

    ModMainMenu();

    virtual ~ModMainMenu();

    // Virtual methods in vtable order (application-module interface).
    virtual void OnInitialize();

    virtual void OnRelease();

    virtual long long OnKeyPress(long long key, long long mod);

    virtual long long OnKeyRelease(long long key, long long mod);

    virtual void OnTouchBegin(int x, int y);

    virtual void OnTouchMove(int x, int y);

    virtual void OnTouchEnd(int x, int y);

    virtual void OnTouchBegin(int x, int y, void *touch);

    virtual void OnTouchMove(int x, int y, void *touch);

    virtual void OnTouchEnd(int x, int y, void *touch);

    virtual void OnUpdate();

    virtual void OnRender3D();

    virtual void OnRender2D();

    virtual void OnSuspend();

    virtual void OnResume();

    virtual int ShowLoadingScreen();
};
#endif
