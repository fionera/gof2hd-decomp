#ifndef GOF2_SPACELOUNGE_H
#define GOF2_SPACELOUNGE_H
#include "gof2/common.h"
// struct derived from offset-access field map (named typed members in offset order)
struct Agent;
struct StarMap;
struct ChoiceWindow;
struct ListItemWindow;
struct CutScene;
struct ScrollTouchWindow;
struct TouchButton;
struct ImagePart;
namespace AbyssEngine { class EaseInOutMatrix; class EaseInOut; }

// Non-template float accessor overload. common.h provides a template `F<T>` that
// requires an explicit type argument; the recovered SpaceLounge code calls `F(self, off)`
// bare and expects a float& (the historic per-class helper returned float&). A non-template
// overload is preferred over the template for these calls.
static inline float &F(void *p, int off) { return *(float *)((char *)p + off); }

class SpaceLounge {
public:
    // ---- named layout (recovered from offset-access map; `// +0xNN` are the original
    // 32-bit byte offsets, kept as documentation. Member sizes are natural 64-bit.) ----
    void              *_vtable;                  // +0x0  vtable pointer
    StarMap           *starMap;                  // +0x4  active star-map sub-screen
    ChoiceWindow      *choiceWindow;             // +0x8  owned yes/no chat dialog
    ListItemWindow    *listWindow;               // +0xc  owned ship/list sub-window
    int                mode;                      // +0x14 lounge interaction mode (0..3)
    int                field_0x18;                // +0x18 (init-only flag)
    uint8_t            chatActive;                // +0x19 chat session active
    uint8_t            field_0x1a;                // +0x1a (draw-time choice flag)
    uint8_t            choiceVisible;             // +0x1b choice window taking input
    uint8_t            listVisible;               // +0x1c list window taking input
    int                selectedAgent;             // +0x20 index of selected agent
    Array<Agent *>    *agents;                    // +0x24 Station::agents (cross-object)
    Array<String *>   *agentTexts;                // +0x28 per-agent text array
    int                field_0x2c;                // +0x2c (chat scroll/offset)
    int                field_0x30;                // +0x30
    uint8_t            mapVisible;                // +0x34 star-map mode active
    uint8_t            hangarUpdate;              // +0x35 hangar needs refresh flag
    uint8_t            field_0x36;                // +0x36
    Array<Array<ImagePart *> *> *silhouetteGrid;  // +0x38 per-system agent silhouette grid
    Array<ImagePart *> *agentImageParts;          // +0x3c selected-agent silhouette parts
    Array<Vector *>   *agentRects;                // +0x40 per-agent screen hit-rect pairs
    CutScene          *cutScene;                  // +0x44 owned lounge backdrop cutscene
    AbyssEngine::EaseInOutMatrix *cameraEase;     // +0x48 owned camera-pan ease
    Vector             silhouettePos;             // +0x4c (0x4c/0x50/0x54) projected pos
    char              *agentVisited;              // +0x58 per-agent visited byte buffer (0x15)
    Array<TouchButton *> *buttons;                // +0x5c owned offer buttons
    ScrollTouchWindow *scrollWindow;              // +0x60 owned offer-text scroll window
    int                field_0x64;                // +0x64
    int                field_0x68;                // +0x68 button layout count
    int                panelWidth;                // +0x6c agent panel width
    int                panelX;                    // +0x70 agent panel x
    int                panelY;                    // +0x74 agent panel y
    int                buttonPanelY;              // +0x78 button panel y
    int                buttonY1;                  // +0x7c button row y (single)
    int                buttonY0;                  // +0x80 button row y (base)
    int                buttonX;                   // +0x84 button x
    int                hoverAgent;                // +0x88 hovered agent index
    int                field_0x8c;                // +0x8c
    int                field_0x90;                // +0x90
    int                field_0x94;                // +0x94
    int                field_0x98;                // +0x98
    int                field_0x9c;                // +0x9c
    int                field_0xa0;                // +0xa0
    String             title;                     // +0xa4 header title String
    uint8_t            initialized;               // +0xb0 ctor/init completed
    uint8_t            touchDown;                 // +0xb2 touch currently down
    int                touchX;                    // +0xb4 last touch x
    int                touchY;                    // +0xb8 last touch y
    uint8_t            field_0xbc;                // +0xbc (camera-ease in-progress)
    uint8_t            introDone;                 // +0xbd intro/camera intro finished
    AbyssEngine::EaseInOut *headEase;             // +0xc0 owned head-bob ease
    uint8_t            field_0xc4;                // +0xc4 (head-bob direction)
    Matrix             baseMatrix;                // +0xc8 base camera matrix (60B in binary)
    float              field_0x104;               // +0x104 wave-time accumulator
    int                field_0x108;               // +0x108 head-bob rotation step count

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
};
#endif
