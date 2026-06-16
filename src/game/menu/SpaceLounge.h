#ifndef GOF2_SPACELOUNGE_H
#define GOF2_SPACELOUNGE_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"

class Agent;
class StarMap;
class ChoiceWindow;
class ListItemWindow;
class CutScene;
class ScrollTouchWindow;
class TouchButton;
class ImagePart;
namespace AbyssEngine { class EaseInOutMatrix; class EaseInOut; }

// The lounge sub-screen of a station: shows the agents present, their portraits and
// silhouettes, drives the chat/offer dialog flow and (optionally) hands control off to
// the star map or the ship list window.
class SpaceLounge {
public:
    // (+0x0 is the compiler-managed C++ vptr; this screen is polymorphic)
    StarMap                     *starMap;          // active star-map sub-screen
    ChoiceWindow                *choiceWindow;     // owned yes/no chat dialog
    ListItemWindow              *listWindow;       // owned ship/list sub-window
    int                          mode;             // lounge interaction mode (0..3)
    int                          field_0x18;        // init/redraw guard, only ever reset to 0
    uint8_t                      chatActive;       // chat session active
    uint8_t                      popupActive;      // +0x1a confirm-popup taking input
    uint8_t                      choiceVisible;    // choice window taking input
    uint8_t                      listVisible;      // list window taking input
    int                          selectedAgent;    // index of selected agent
    Array<Agent *>              *agents;           // Station::agents (cross-object)
    Array<String *>             *agentTexts;       // per-agent text array
    int                          chatScroll;        // +0x2c offer-text scroll offset
    int                          chatAnswer;        // +0x30 selected chat answer
    uint8_t                      mapVisible;        // star-map mode active
    uint8_t                      hangarUpdate;      // hangar needs refresh flag
    uint8_t                      singleOffer;       // +0x36 show only the primary offer button
    Array<Array<ImagePart *> *> *silhouetteGrid;    // per-system agent silhouette grid
    Array<ImagePart *>          *agentImageParts;   // selected-agent silhouette parts
    Array<Vector *>             *agentRects;        // per-agent screen hit-rect pairs
    CutScene                    *cutScene;          // owned lounge backdrop cutscene
    AbyssEngine::EaseInOutMatrix *cameraEase;       // owned camera-pan ease
    Vector                       silhouettePos;     // projected silhouette position
    char                        *agentVisited;      // per-agent visited byte buffer
    Array<TouchButton *>        *buttons;           // owned offer buttons
    ScrollTouchWindow           *scrollWindow;      // owned offer-text scroll window
    int                          buttonsHeight;     // +0x64 total stacked button height
    int                          visibleButtonCount; // +0x68 number of offer buttons shown
    int                          panelWidth;        // agent panel width
    int                          panelX;            // agent panel x
    int                          panelY;            // agent panel y
    int                          buttonPanelY;      // button panel y
    int                          buttonY1;          // button row y (single)
    int                          buttonY0;          // button row y (base)
    int                          buttonX;           // button x
    int                          hoverAgent;        // hovered agent index
    int                          field_0x8c;
    int                          field_0x90;
    int                          field_0x94;
    int                          field_0x98;
    int                          field_0x9c;
    int                          field_0xa0;
    String                       title;             // header title String
    uint8_t                      initialized;       // ctor/init completed
    uint8_t                      touchDown;         // touch currently down
    int                          touchX;            // last touch x
    int                          touchY;            // last touch y
    uint8_t                      cameraAnimating;   // +0xbc camera ease still running
    uint8_t                      introDone;         // intro/camera intro finished
    AbyssEngine::EaseInOut      *headEase;          // owned head-bob ease
    uint8_t                      headBobReverse;    // +0xc4 head-bob easing toward lower bound
    Matrix                       baseMatrix;        // base camera matrix
    float                        headBobPhase;      // +0x104 wave-time accumulator
    int                          headBobSteps;      // +0x108 head-bob rotation step count

    SpaceLounge();
    virtual ~SpaceLounge();

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

    // Mode-dependent tail handlers: forward rendering/updating to whichever sub-screen
    // (StarMap / CutScene / ListItemWindow / Layout) is live for the current mode.
    void OnRender3D_map_tail(void *map);
    void OnRender3D_cutscene_tail(void *cutscene);
    void OnRenderBG_tail(void *cutscene);
    void draw3DShip_tail(void *ship);
    void draw_map_tail(void *map);
    void draw_cutscene_tail();
    void update_map_tail(void *map, int dt);
    void update_ship_tail(void *list, int dt);

    // Deleting destructor body reached through the deleting-dtor thunk: runs the
    // destructor and returns the storage so the caller can free it.
    void *dtor();
};
#endif
