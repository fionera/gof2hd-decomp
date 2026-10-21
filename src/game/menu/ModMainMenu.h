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

    ~ModMainMenu() override;

    int OnInitialize() override;

    void OnRelease() override;

    void OnKeyPress(long long key, long long mod) override;

    void OnKeyRelease(long long key, long long mod) override;

    void OnTouchBegin(int x, int y) override;

    void OnTouchMove(int x, int y) override;

    void OnTouchEnd(int x, int y) override;

    void OnTouchBegin(int x, int y, void *touch) override; // lint: void_ptr (exported virtual signature)

    void OnTouchMove(int x, int y, void *touch) override; // lint: void_ptr (exported virtual signature)

    void OnTouchEnd(int x, int y, void *touch) override; // lint: void_ptr (exported virtual signature)

    void OnUpdate() override;

    void OnRender3D() override;

    void OnRender2D() override;

    void OnSuspend() override;

    void OnResume() override;

    int ShowLoadingScreen() override;
};
#endif
