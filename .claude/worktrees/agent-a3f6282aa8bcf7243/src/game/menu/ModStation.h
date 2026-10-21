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
    // ----------------------------------------------------------------------
    // Everything at offset >= 0xa4 lives in a SPARSE region that the Ghidra
    // ModStation struct (which spans only 0x00-0x8f) never modelled; the
    // decompiler reaches it as this[1].field_NN / this[2].field_NN, i.e.
    // *(T*)((char*)this + 0xNN). Modelling that region as named members with
    // guessed String padding kept shifting later fields to the wrong byte
    // offsets, so those fields are NOT declared here. Each access lives in the
    // .cpp as a binary-exact positional RAWREAD: *(T*)((char*)this + 0xNN).
    // (The full object is larger than this declared size; it is only ever
    //  allocated/owned through ::operator new(sizeof from the binary), and the
    //  trailing sparse bytes are reached positionally, never via this type's
    //  size, so leaving them out of the layout is safe for this TU.)
    // ----------------------------------------------------------------------

    ModStation();
    virtual ~ModStation();

    void OnInitialize();
    // Drives the station main-menu cursor and screen selection. The engine
    // dispatches key events through the two-arg variant (the first argument is
    // the ignored dispatch slot, the second the key code); the single-arg form
    // is inlined into it.
    void OnKeyPress(long long unused, long long key);
    // The station screen ignores key-release and reports the loading screen as
    // already shown (no-op handlers in the original).
    long long OnKeyRelease(long long unused, long long key);
    int ShowLoadingScreen();
    void OnRelease();
    void OnRender2D();
    void OnRender3D();
    void OnResume();
    void OnSuspend();
    void OnTouchBegin(int x, int y, void *touch);
    void OnTouchEnd(int x, int y, void *touch);
    void OnTouchMove(int x, int y, void *touch);
    // Two-arg engine-dispatch stubs (the base touch interface invokes these
    // without a touch handle); they are no-ops in the original binary.
    int OnTouchBegin(int x, int y);
    int OnTouchEnd(int x, int y);
    int OnTouchMove(int x, int y);
    void OnUpdate();
    void addAchievement(int medalId, int kind);
    void autosave();
    void checkHints();
    void checkMedals();
    void checkPendingProducts();
    // Re-enters the docked station: hands the current station back to Status,
    // marks it visited, applies any earned medals, and caches the player's ship
    // / weapon / shield indices into the save record.
    void enterStation();
    void leaveStation();
    // Restores the hangar's lighting rig to the home-system's race profile.
    void resetLight();
    void resetIdleCamForHangar();
    void setGameLoaded();
    void showCBSMessage();
    void showDlcMenu();
    void showMapWindow();
};

#endif
