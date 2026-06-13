#ifndef GOF2_MODSTATION_H
#define GOF2_MODSTATION_H
#include "gof2/common.h"
// Galaxy on Fire 2 — ModStation (docked-station module). Top-level class, NO namespace.
// Field layout recovered from the constructor (Ghidra @0xe52d0) and per-method offset
// accesses. Trailing comments are the original 32-bit field offsets, kept for cross-
// reference; the live layout below is natural 64-bit. Fields whose semantics could not
// be recovered from the binary keep the deterministic field_0xNN name (still real,
// typed members). Several slots are reused for different objects over the screen's
// lifetime (union-like in the original); those keep a generic name.

// Other game types referenced by pointer only — keep opaque.
// (String comes from common.h via `using AbyssEngine::String;` — do not redeclare.)
struct ChoiceWindow;
struct RecordHandler;
struct StarMap;
struct CutScene;
struct NewsTicker;
struct DialogueWindow;
struct EaseInOutMatrix;
struct SpaceLounge;

// ---- tiny offset-cast helpers -------------------------------------------------
// (kept because sibling menu modules include this header for the B/I/P shims.)
#ifndef GOF2_BIP_HELPERS
#define GOF2_BIP_HELPERS
static inline char*           B (void* p, int off) { return (char*)p + off; }
static inline int&            I (void* p, int off) { return *(int*)((char*)p + off); }
static inline void*&          P (void* p, int off) { return *(void**)((char*)p + off); }
#endif

class ModStation {
public:
    // ---- recovered named layout (32-bit offsets in comments; live layout is 64-bit) ----
    int            vtable;            // +0x00  vtable pointer (set to vtable base + 8)
    unsigned       fadeColor;         // +0x04  2D fade/clear colour
    int*           field_0x08;        // +0x08
    int            state;             // +0x0c  screen-state machine value (init 100)
    int            field_0x10;        // +0x10
    StarMap*       starMap;           // +0x14  star-map sub-screen
    char           field_0x18;        // +0x18  (low byte flag)
    NewsTicker*    newsTicker;        // +0x1c  station news ticker
    EaseInOutMatrix* cameraTween;     // +0x20  hangar idle-camera tween; +0x20..0x23 also cleared bytes
    char           field_0x24;        // +0x24  station-active flag
    int            dt;                // +0x28  last-frame elapsed time (ms)
    int            field_0x2c;        // +0x2c
    long long      accumTime;         // +0x30  accumulated docked time (64-bit; +0x34 high word)
    String         stationName;       // +0x38  current station name (embedded String)
    int            selectedButton;    // +0x44  highlighted main-button index
    int*           buttonState;       // +0x48  highlighted-button state array
    int            field_0x4c;        // +0x4c
    void*          dlcMenu;           // +0x50  DLC MenuTouchWindow (slot reused: +0x54/0x90/0x94 sub-fields)
    int            field_0x54;        // +0x54  reused: cached Mission* / Radio* / credits-button
    void*          radioMessages;     // +0x58  RadioMessage Array
    int            field_0x5c;        // +0x5c
    int            m_nStarMapWindowOpen; // +0x60  star-map / cutscene window flags (bytes 0..3)
    int            field_0x64;        // +0x64  sub-window open flags (bytes 0..3)
    int            field_0x68;        // +0x68  sub-window open flags (bytes 0..3)
    int*           field_0x6c;        // +0x6c  sub-window open flags (bytes 0..3)
    void*          choiceWindow;      // +0x70  station choice/notice window
    void*          spaceLounge;       // +0x74  space-lounge sub-screen
    void*          hangarWindow;      // +0x78  hangar sub-screen
    void*          statusWindow;      // +0x7c  status sub-screen
    DialogueWindow* m_pDialogueWindow; // +0x80  mission/dialogue window
    int            field_0x84;        // +0x84  DialogueWindow (variant slot)
    void*          medalChoiceWindow; // +0x88  medal choice window
    int*           buttonRow;         // +0x8c  TouchButton array (main button strip)
    void*          buttonLaunch;      // +0x90  launch TouchButton
    void*          buttonCredits;     // +0x94  credits TouchButton; +0x94..0x96 also flag bytes
    void*          scrollBox;         // +0x98  radio-cutscene scroll box
    int            field_0x9c;        // +0x9c
    void*          hangarShipGeom;    // +0xa8  hangar-ship geometry / ticker drag left-bound X
    char           field_0xb0;        // +0xb0  flags (game-loaded etc.); +0xb0..0xb3 sibling bytes
    char           field_0xb1;        // +0xb1
    char           field_0xb3;        // +0xb3
    char           field_0xc1;        // +0xc1  CBS-message flag (within the 0xc0 word region)
    int*           medalArray;        // +0xbc  Array<int*> of (medalId, value) pairs
    int            medalIndex;        // +0xc0  current medal being shown
    int            medalCount;        // +0xc4  number of pending medals
    char           field_0xd8;        // +0xd8  station "alarm" flag (bytes 0xd8..0xdb)
    char           field_0xd9;        // +0xd9  mining-plant flag
    char           field_0xdb;        // +0xdb  wingman-event-consumed flag
    char           field_0xdc;        // +0xdc
    int            dragOriginX;       // +0xe4  ticker drag origin X
    int            dragAccumX;        // +0xe8  ticker drag accumulated X
    int            dragVelX;          // +0xf0  ticker drag velocity X
    int            dragScaleX;        // +0xf4  ticker drag scale (1.0f)
    int            dragStartX;        // +0xfc  ticker drag start X
    char           field_0x100;       // +0x100 ticker drag-active flag
    void*          scrollBox2;        // +0x108 radio-cutscene scroll box (build path)
    int            scrollOffset;      // +0x110 radio reveal scroll offset / touch X
    int            scrollTarget;      // +0x114 radio reveal scroll target / touch Y
    char           field_0x118;       // +0x118 hangar light intensity (low byte also a flag)
    int            field_0x11c;       // +0x11c hangar light target
    void*          capturedTouch;     // +0x128 active touch pointer
    char           field_0x12c;       // +0x12c
    int            restCamX;          // +0x130 idle-camera rest X
    int            restCamY;          // +0x134 idle-camera rest Y
    int            restCamZ;          // +0x138 idle-camera rest Z
    void*          easeCamX;          // +0x13c idle-camera EaseInOut (X)
    void*          easeCamY;          // +0x140 idle-camera EaseInOut (Y)
    void*          easeCamZ;          // +0x144 idle-camera EaseInOut (Z)
    char           field_0x148;       // +0x148
    void*          easeCamScalarX;    // +0x288 scalar EaseInOut (idle cam X)
    void*          easeCamScalarY;    // +0x28c scalar EaseInOut (idle cam Y)
    void*          easeCamScalarZ;    // +0x290 scalar EaseInOut (idle cam Z)

    // ---- methods (converted from free functions) ----
    ModStation();
    void OnInitialize();
    void OnKeyPress(long long key);
    void OnRelease();
    void OnRender2D();
    void OnRender3D();
    void OnSuspend();
    void OnTouchBegin(int x, int y, void *touch);
    void OnTouchEnd(int param_1, int param_2, void *param_3);
    void OnTouchMove(int x, int y, void *touch);
    void OnUpdate();
    void addAchievement(int param_1, int param_2);
    void autosave();
    void checkHints();
    void checkMedals();
    void checkPendingProducts();
    void dtor();
    ModStation * dtor_inner();
    void leaveStation();
    void resetIdleCamForHangar();
    void setGameLoaded();
    void showCBSMessage();
    void showDlcMenu();
    void showMapWindow();
};

#endif
