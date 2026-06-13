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

class SpaceLounge {
public: void* _opaque;
    // ---- methods (converted from free functions) ----
    ~SpaceLounge();
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
    void refresh();
    void setHangarUpdate(bool value);
    void startChat();
    void update(int dt);
    void updateScreenPositions();

    // ---- recovered tail-call fragments --------------------------------------
    // The decompiler split OnRender3D/OnRenderBG/draw3DShip/draw/update at their
    // mode-dependent tail calls into separate fragments. Each forwards the work to
    // the sibling sub-screen (StarMap / CutScene / ListItemWindow / Layout) that the
    // lounge is currently showing. Receivers are passed in by the call site (or, for
    // draw_cutscene_tail, read from the active draw-layout slot).
    void OnRender3D_map_tail(void *map);            // StarMap::render()
    void OnRender3D_cutscene_tail(void *cutscene);  // CutScene::render3D()
    void OnRenderBG_tail(void *cutscene);           // CutScene::renderBG()
    void draw3DShip_tail(void *ship);               // ListItemWindow::render()
    void draw_map_tail(void *map);                  // StarMap::draw()
    void draw_cutscene_tail();                      // Layout::drawFooter()
    void update_map_tail(void *map, int dt);        // StarMap::update(dt)
    void update_ship_tail(void *list, int dt);      // ListItemWindow::update(dt)

    // Out-of-line destructor body the deleting-dtor thunk reached (ModStation frees
    // the lounge through it). Runs ~SpaceLounge() and returns the storage.
    void *dtor();
};  // no offset accesses observed
#endif
