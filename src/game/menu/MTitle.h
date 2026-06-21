#ifndef GOF2_MTITLE_H
#define GOF2_MTITLE_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "engine/core/IApplicationModule.h"

// MTitle -- the boot/title screen application module. Fades the publisher and
// game logos in and out, then hands control to the main-menu module.
class MTitle : public IApplicationModule {
public:
    int      renderPriority;   // render priority (= 100)
    uint32_t logoImage;        // first logo image2d handle
    uint32_t logoImage2;       // second logo image2d handle
    int      step;             // logo-sequence step (0,1 -> done at 2)
    int      timer;            // elapsed-time accumulator for the current step

    MTitle();
    ~MTitle();

    int       OnInitialize();
    void      OnRelease();
    long long OnKeyPress(long long key, long long mod);
    long long OnKeyRelease(long long key, long long mod);
    int       OnTouchBegin(int x, int y);
    int       OnTouchMove(int x, int y);
    void      OnUpdate();
    void      OnRender2D();
    void      OnRender3D();
    void      OnTouchEnd(int x, int y);
    int       ShowLoadingScreen();
};
#endif
