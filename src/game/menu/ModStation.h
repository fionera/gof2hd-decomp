#ifndef GOF2_MODSTATION_H
#define GOF2_MODSTATION_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"

// Galaxy on Fire 2 — ModStation: the docked-station module (hangar, shipyard, missions,
// star map, status and DLC sub-screens reachable from a docked station).

// Cross types referenced by pointer only.
class StarMap;
class CutScene;
class NewsTicker;
class DialogueWindow;
namespace AbyssEngine { class EaseInOutMatrix; }

class ModStation {
public:
    // +0x00 is the compiler-managed vptr (the original installed it manually as
    // "vtable base + 8"; ModStation is now a real polymorphic type so the vptr
    // is synthesised here instead of being a hand-planted member).
    unsigned         fadeColor;         // +0x04 2D fade/clear colour
    int*             field_0x08;        // +0x08 (unused in this TU)
    int              state;             // +0x0c screen-state machine value (init 100)
    StarMap*         starMap;           // +0x10 star-map sub-screen
    CutScene*        cutScene;          // +0x14 docking cutscene
    char             pendingHangarClose;// +0x18 deferred hangar-close flag
    NewsTicker*      newsTicker;        // +0x1c station news ticker
    AbyssEngine::EaseInOutMatrix* cameraTween;  // +0x20 hangar idle-camera tween
    char             stationActive;     // +0x24 station screen active flag
    int              dt;                // +0x28 last-frame elapsed time (ms)
    int              loadTick;          // +0x2c per-screen load tick
    long long        accumTime;         // +0x30 accumulated docked time
    String           stationName;       // +0x38 current station name
    int              selectedButton;    // +0x44 highlighted main-button index
    int*             buttonState;       // +0x48 highlighted-button state array
    int              departPending;     // +0x4c pending-departure flag (byte 2)
    void*            dlcMenu;           // +0x50 DLC MenuTouchWindow
    int              activeMission;     // +0x54 reused: cached Mission* / Radio* / credits-button
    void*            radioMessages;     // +0x58 RadioMessage Array
    int              field_0x5c;        // +0x5c (unused in this TU)
    int              m_nStarMapWindowOpen; // +0x60 star-map / cutscene window flags (bytes 0..3)
    int              subWindowFlags;    // +0x64 sub-window open flags (bytes 0..3)
    int              modalFlags;        // +0x68 modal-window open flags (bytes 0..3)
    int              screenFlags;       // +0x6c screen-state flags (bytes 0..3)
    void*            choiceWindow;      // +0x70 station choice/notice window
    void*            spaceLounge;       // +0x74 space-lounge sub-screen
    void*            hangarWindow;      // +0x78 hangar sub-screen
    void*            statusWindow;      // +0x7c status sub-screen
    DialogueWindow*  m_pDialogueWindow; // +0x80 mission/dialogue window
    DialogueWindow*  dialogueWindow;    // +0x84 mission/event dialogue window (variant slot)
    void*            medalChoiceWindow; // +0x88 medal choice window
    int*             buttonRow;         // +0x8c TouchButton array (main button strip)
    void*            buttonLaunch;      // +0x90 launch TouchButton
    void*            buttonCredits;     // +0x94 credits TouchButton
    void*            scrollBox;         // +0x98 radio-cutscene scroll box
    int              introTimer;        // +0x9c intro timer
    void*            hangarShipGeom;    // +0xa0 hangar-ship geometry / ticker drag left-bound X
    char             gameLoaded;        // +0xb0 game-loaded flag
    char             autoSaved;         // +0xb1 autosaved-this-visit flag
    char             reloadPending;     // +0xb3 reload-on-leave flag
    char             cbsMessageOpen;    // +0xc1 CBS-message flag
    int*             medalArray;        // +0xc4 Array<int*> of (medalId, value) pairs
    int              medalIndex;        // +0xc8 current medal being shown
    int              medalCount;        // +0xcc number of pending medals
    char             alarmActive;       // +0xd8 station "alarm" flag
    char             miningPlantFlag;   // +0xd9 mining-plant flag
    char             wingmanEventConsumed; // +0xdb wingman-event-consumed flag
    char             cbsHintShown;      // +0xdc CBS-hint-shown flag
    int              dragOriginX;       // +0xe0 ticker drag origin X
    int              dragAccumX;        // +0xe4 ticker drag accumulated X
    int              dragVelX;          // +0xe8 ticker drag velocity X
    int              dragScaleX;        // +0xec ticker drag scale (1.0f)
    int              dragStartX;        // +0xf0 ticker drag start X
    char             dragActive;        // +0x100 ticker drag-active flag
    void*            scrollBox2;        // +0x104 radio-cutscene scroll box (build path)
    int              scrollOffset;      // +0x110 radio reveal scroll offset / touch X
    int              scrollTarget;      // +0x114 radio reveal scroll target / touch Y
    char             hangarLightIntensity; // +0x118 hangar light intensity (low byte also a flag)
    int              hangarLightTarget; // +0x11c hangar light target
    void*            capturedTouch;     // +0x128 active touch pointer
    char             touchPadFlag;      // +0x12c touch-pad flag
    int              restCamX;          // +0x130 idle-camera rest X
    int              restCamY;          // +0x134 idle-camera rest Y
    int              restCamZ;          // +0x138 idle-camera rest Z
    void*            easeCamX;          // +0x13c idle-camera EaseInOut (X)
    void*            easeCamY;          // +0x140 idle-camera EaseInOut (Y)
    void*            easeCamZ;          // +0x144 idle-camera EaseInOut (Z)
    char             idleCamFlag;       // +0x148 idle-camera flag
    void*            easeCamScalarX;    // +0x14c scalar EaseInOut (idle cam X)
    void*            easeCamScalarY;    // +0x150 scalar EaseInOut (idle cam Y)
    void*            easeCamScalarZ;    // +0x154 scalar EaseInOut (idle cam Z)

    ModStation();
    virtual ~ModStation();

    // Slot at vtable byte-offset 0x10 (index 4): launches the selected station
    // sub-module. The five DLC/station-menu buttons and the campaign-mission
    // transition path dispatch through this virtual slot on `this`
    // (in the binary: `(*(this->vptr+0x10))(this, 0x10000, 0)`).
    virtual void launchModule(int arg);

    void OnInitialize();
    void OnKeyPress(long long key);
    void OnRelease();
    void OnRender2D();
    void OnRender3D();
    void OnSuspend();
    void OnTouchBegin(int x, int y, void *touch);
    void OnTouchEnd(int x, int y, void *touch);
    void OnTouchMove(int x, int y, void *touch);
    void OnUpdate();
    void addAchievement(int medalId, int kind);
    void autosave();
    void checkHints();
    void checkMedals();
    void checkPendingProducts();
    void leaveStation();
    void resetIdleCamForHangar();
    void setGameLoaded();
    void showCBSMessage();
    void showDlcMenu();
    void showMapWindow();
};

#endif
