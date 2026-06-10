#ifndef GOF2_SPACELOUNGE_H
#define GOF2_SPACELOUNGE_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
struct SpaceLounge;
struct Agent;

// Non-template float accessor overload. common.h provides a template `F<T>` that
// requires an explicit type argument; the recovered SpaceLounge code calls `F(self, off)`
// bare and expects a float& (the historic per-class helper returned float&). A non-template
// overload is preferred over the template for these calls.
static inline float &F(void *p, int off) { return *(float *)((char *)p + off); }

// B/I/P offset-cast helpers are provided by gof2/TouchButton.h (included by the .cpp).

struct SpaceLounge { void* _opaque;
    // ---- methods (converted from free functions) ----
    void OnRender3D();
    void OnRenderBG();
    int OnTouchBegin(int x, int y);
    void OnTouchEnd(int x, int y);
    int OnTouchMove(int x, int y);
    void draw();
    void draw3DShip();
    void drawLounge();
    unsigned char hangarNeedsUpdate();
    int init();
    unsigned char introFinished();
    bool listMode();
    unsigned char mapMode();
    void onKeyPress(int key);
    void setHangarUpdate(bool value);
    void startChat();
    void update(int dt);
    void updateScreenPositions();
};  // no offset accesses observed
#endif
