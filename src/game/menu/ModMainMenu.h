#ifndef GOF2_MODMAINMENU_H
#define GOF2_MODMAINMENU_H
#include "engine/core/Array.h"
#include "../../engine/core/AEString.h"
#include "engine/core/ApplicationManager.h"
#include "engine/core/IApplicationModule.h"
class CutScene;
class MenuTouchWindow;

class ModMainMenu : public IApplicationModule {
public:
    int state;
    uint8_t initialized;
    int frameTime;
    MenuTouchWindow *touchWindow;
    CutScene *cutScene;
    int logoImage;
    int fadeTimer;
    uint8_t logoActive;
    uint8_t hasSavedGame;

    ModMainMenu();

    virtual ~ModMainMenu();

    virtual int OnInitialize();

    virtual void OnRelease();

    virtual void OnKeyPress(long long key, long long mod) {
        (void) key;
        (void) mod;
    }

    virtual void OnKeyRelease(long long key, long long mod) {
        (void) key;
        (void) mod;
    }

    virtual void OnTouchBegin(int x, int y) {
        (void) x;
        (void) y;
    }

    virtual void OnTouchMove(int x, int y) {
        (void) x;
        (void) y;
    }

    virtual void OnTouchEnd(int x, int y) {
        (void) x;
        (void) y;
    }

    virtual void OnTouchBegin(int x, int y, void *touch); // lint: void_ptr (exported virtual signature)

    virtual void OnTouchMove(int x, int y, void *touch); // lint: void_ptr (exported virtual signature)

    virtual void OnTouchEnd(int x, int y, void *touch); // lint: void_ptr (exported virtual signature)

    virtual void OnUpdate();

    virtual void OnRender3D();

    virtual void OnRender2D();

    virtual void OnSuspend();

    virtual void OnResume();

    virtual int ShowLoadingScreen() {
        return 1;
    }
};
#endif
