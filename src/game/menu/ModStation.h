#ifndef GOF2_MODSTATION_H
#define GOF2_MODSTATION_H
#include "common.h"

// Galaxy on Fire 2 — ModStation: the docked-station module (hangar, shipyard, missions,
// star map, status and DLC sub-screens reachable from a docked station).

// Cross types referenced by pointer only.
class StarMap;
class NewsTicker;
class DialogueWindow;
namespace AbyssEngine { class EaseInOutMatrix; }

class ModStation {
public:
    int              vtable;            // vtable pointer (set to vtable base + 8)
    unsigned         fadeColor;         // 2D fade/clear colour
    int*             field_0x08;
    int              state;             // screen-state machine value (init 100)
    int              field_0x10;
    StarMap*         starMap;           // star-map sub-screen
    char             field_0x18;        // low-byte flag
    NewsTicker*      newsTicker;        // station news ticker
    AbyssEngine::EaseInOutMatrix* cameraTween;  // hangar idle-camera tween
    char             field_0x24;        // station-active flag
    int              dt;                // last-frame elapsed time (ms)
    int              field_0x2c;
    long long        accumTime;         // accumulated docked time
    String           stationName;       // current station name
    int              selectedButton;    // highlighted main-button index
    int*             buttonState;       // highlighted-button state array
    int              field_0x4c;
    void*            dlcMenu;           // DLC MenuTouchWindow
    int              field_0x54;        // reused: cached Mission* / Radio* / credits-button
    void*            radioMessages;     // RadioMessage Array
    int              field_0x5c;
    int              m_nStarMapWindowOpen; // star-map / cutscene window flags (bytes 0..3)
    int              field_0x64;        // sub-window open flags (bytes 0..3)
    int              field_0x68;        // sub-window open flags (bytes 0..3)
    int*             field_0x6c;        // sub-window open flags (bytes 0..3)
    void*            choiceWindow;      // station choice/notice window
    void*            spaceLounge;       // space-lounge sub-screen
    void*            hangarWindow;      // hangar sub-screen
    void*            statusWindow;      // status sub-screen
    DialogueWindow*  m_pDialogueWindow; // mission/dialogue window
    int              field_0x84;        // DialogueWindow (variant slot)
    void*            medalChoiceWindow; // medal choice window
    int*             buttonRow;         // TouchButton array (main button strip)
    void*            buttonLaunch;      // launch TouchButton
    void*            buttonCredits;     // credits TouchButton
    void*            scrollBox;         // radio-cutscene scroll box
    int              field_0x9c;
    void*            hangarShipGeom;    // hangar-ship geometry / ticker drag left-bound X
    char             field_0xb0;        // flags (game-loaded etc.)
    char             field_0xb1;
    char             field_0xb3;
    char             field_0xc1;        // CBS-message flag
    int*             medalArray;        // Array<int*> of (medalId, value) pairs
    int              medalIndex;        // current medal being shown
    int              medalCount;        // number of pending medals
    char             field_0xd8;        // station "alarm" flag
    char             field_0xd9;        // mining-plant flag
    char             field_0xdb;        // wingman-event-consumed flag
    char             field_0xdc;
    int              dragOriginX;       // ticker drag origin X
    int              dragAccumX;        // ticker drag accumulated X
    int              dragVelX;          // ticker drag velocity X
    int              dragScaleX;        // ticker drag scale (1.0f)
    int              dragStartX;        // ticker drag start X
    char             field_0x100;       // ticker drag-active flag
    void*            scrollBox2;        // radio-cutscene scroll box (build path)
    int              scrollOffset;      // radio reveal scroll offset / touch X
    int              scrollTarget;      // radio reveal scroll target / touch Y
    char             field_0x118;       // hangar light intensity (low byte also a flag)
    int              field_0x11c;       // hangar light target
    void*            capturedTouch;     // active touch pointer
    char             field_0x12c;
    int              restCamX;          // idle-camera rest X
    int              restCamY;          // idle-camera rest Y
    int              restCamZ;          // idle-camera rest Z
    void*            easeCamX;          // idle-camera EaseInOut (X)
    void*            easeCamY;          // idle-camera EaseInOut (Y)
    void*            easeCamZ;          // idle-camera EaseInOut (Z)
    char             field_0x148;
    void*            easeCamScalarX;    // scalar EaseInOut (idle cam X)
    void*            easeCamScalarY;    // scalar EaseInOut (idle cam Y)
    void*            easeCamScalarZ;    // scalar EaseInOut (idle cam Z)

    ModStation();
    ~ModStation();

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
