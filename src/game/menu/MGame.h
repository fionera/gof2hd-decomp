#ifndef GOF2_MGAME_H
#define GOF2_MGAME_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"

// MGame — the in-flight game module: drives a single flight session (level, player,
// HUD, cameras, dialogue and the hyperspace-jump cinematic).

// Pointer-only collaborators (declared, not defined, here; full headers are pulled in
// by MGame.cpp where the members are actually dereferenced).
class PlayerEgo;
class Hud;
class Level;
class LevelScript;
class Radar;
class Radio;
class MenuTouchWindow;
class DialogueWindow;
class StarMap;
class ChoiceWindow;
class TargetFollowCamera;
class AEGeometry;
namespace AbyssEngine { class ApplicationManager; }
using ::AbyssEngine::ApplicationManager;

// Small opaque singletons referenced only as pointers from MGame.cpp.
struct Music;
struct Cfg;

class MGame {
public:
    unsigned skyboxTexture;             // +0x04  cube-texture handle
    ApplicationManager* appManager;     // +0x08  owning application module
    int loadProgress;                   // +0x0c
    int loadingImage;                   // +0x10  fade/splash image handle
    int cameraMode;                     // +0x14
    uint8_t field_0x18;                 // +0x18
    int field_0x1c;                     // +0x1c
    unsigned startTime;                 // +0x20
    unsigned startTimeHigh;             // +0x24
    unsigned lastTime;                  // +0x28
    unsigned lastTimeHigh;              // +0x2c
    int frameTime;                      // +0x30
    int frameTimeHigh;                  // +0x34
    int deltaTime;                      // +0x40
    int field_0x44;                     // +0x44
    unsigned elapsedTime;               // +0x48
    int elapsedTimeHigh;                // +0x4c
    int loadingTime;                    // +0x50
    uint8_t active;                     // +0x54  session live / drawing enabled
    PlayerEgo* player;                  // +0x58
    int field_0x5c;                     // +0x5c
    uint8_t pauseOpen;                  // +0x5d  in-flight menu/dialogue paused
    uint8_t cutsceneActive;             // +0x5e  dialogue/cutscene running (0x5e high byte)
    uint8_t jumpActive;                 // +0x5f  jump cinematic active
    uint8_t gameOverActive;             // +0x60  game-over splash up
    uint8_t campaignMission;            // +0x61
    AbyssEngine::String gameOverTitle;  // +0x64  game-over message title
    int field_0x70;                     // +0x70
    Hud* hud;                           // +0x74
    Level* level;                       // +0x78
    LevelScript* levelScript;           // +0x7c
    Radar* radar;                       // +0x80
    Radio* radio;                       // +0x84
    MenuTouchWindow* menuWindow;        // +0x88
    DialogueWindow* dialogueWindow;     // +0x8c
    StarMap* starMap;                   // +0x90
    ChoiceWindow* choiceWindow;         // +0x94
    int touch0Id;                       // +0x98  active free-cam touch id (finger 0)
    int touch1Id;                       // +0x9c  active free-cam touch id (finger 1)
    int menuTime;                       // +0xa0
    int freeCamFinger1X;                // +0xa4  free-cam finger-1 pos: Vector{0xa4,0xa8,0xac}
    int freeCamFinger1Y;                // +0xa8
    int freeCamFinger1Z;                // +0xac
    int freeCamFinger0X;                // +0xb0  free-cam finger-0 pos: Vector{0xb0,0xb4,0xb8}
    int freeCamFinger0Y;                // +0xb4
    int freeCamFinger0Z;                // +0xb8
    float flCameraRoll;                 // +0xbc  free-cam zoom distance
    void* activeTouchId;                // +0xc0
    uint8_t field_0xc1;                 // +0xc1
    uint8_t field_0xc2;                 // +0xc2
    uint8_t field_0xc3;                 // +0xc3
    uint8_t dockChoiceOpen;             // +0xc4
    uint8_t autopilotMenuOpen;          // +0xc5  dock-at-station choice up
    uint8_t field_0xc6;                 // +0xc6
    uint8_t starMapOpen;                // +0xc7
    uint16_t field_0xc8;                // +0xc8
    uint8_t menuTouchOpen;              // +0xc9  pause/menu-touch window visible
    int field_0xca;                     // +0xca
    uint8_t touchesStream;              // +0xcb
    uint8_t touchesStation;             // +0xcc
    uint8_t jumpGateSoundStarted;       // +0xcd
    uint8_t choiceWindowOpen;           // +0xce
    uint8_t field_0xcf;                 // +0xcf
    int choiceItemCount;                // +0xd0
    uint8_t field_0xd4;                 // +0xd4
    uint8_t turretMode;                 // +0xd5
    uint8_t hudMenuOpen;                // +0xd6
    int field_0xd8;                     // +0xd8
    uint8_t jumpDriveActive;            // +0xdc  hyperspace cinematic in progress
    uint8_t usingJumpDrive;             // +0xdd  charged (Khador) jump variant
    uint8_t field_0xe0;                 // +0xe0
    int egoJumpPosX;                    // +0xe4  jump target position Vector{0xe4,0xe8,0xec}
    int egoJumpPosY;                    // +0xe8
    int egoJumpPosZ;                    // +0xec
    unsigned cameraId;                  // +0xf0  PaintCanvas camera handle
    TargetFollowCamera* camera;         // +0xf4
    int hudTouchFlags;                  // +0xf8  last Hud touch-result bitmask
    int lastTapTime;                    // +0x100
    int lastTapTimeHigh;                // +0x104
    int lastAlignTime;                  // +0x108
    int lastAlignTimeHigh;              // +0x10c
    uint8_t field_0x110;                // +0x110
    uint8_t needsRedraw;                // +0x111  redraw / fullscreen-window flag
    AEGeometry* jumpFlash;              // +0x114  warp-tunnel geometry
    float flShakeX;                     // +0x118  accumulated free-cam rotation X
    float flShakeY;                     // +0x11c  accumulated free-cam rotation Y
    int field_0x120;                    // +0x120
    int dragLastX;                      // +0x124
    int dragLastY;                      // +0x128
    int dragRotIntX;                    // +0x12c
    int dragRotIntY;                    // +0x130
    int dragDeltaX;                     // +0x134
    int dragDeltaY;                     // +0x138
    float flShakeAmpX;                  // +0x13c
    float flShakeAmpY;                  // +0x140
    float flShakePhaseX;                // +0x148
    float flShakePhaseY;                // +0x14c
    int field_0x150;                    // +0x150
    int dragStartX;                     // +0x154
    int dragStartY;                     // +0x158
    uint8_t freeCamDragging;            // +0x15c  free-cam pan gesture active
    uint8_t menuOpen;                   // +0x15d  full-screen menu window open
    uint8_t freeCamMode;                // +0x15e  cinematic / free-cam mode
    int cinematicPrevCamMode;           // +0x160  camera mode saved before cinematic
    uint8_t cinematicPrevLookAt;        // +0x164  look-at flag saved before cinematic
    int flFastForwardFactor;            // +0x168
    int timeWarpState;                  // +0x16c
    int maneuverHoldTime;               // +0x178
    uint8_t maneuverActive;             // +0x17c
    int maneuverStartX;                 // +0x180
    int maneuverStartY;                 // +0x184
    int field_0x19c;                    // +0x19c
    int field_0x1a0;                    // +0x1a0
    uint8_t pauseSnapshot;              // +0x1a6  pause flag saved across suspend
    float flFastForwardWeight;          // +0x1a8
    int field_0x1ac;                    // +0x1ac
    int cloakAttribute;                 // +0x1b0
    int cloakAttributeMax;              // +0x1b4
    uint16_t thrustActive;              // +0x1b8  drag-thrust gesture active
    uint8_t thrustEngaged;              // +0x1b9  thrust threshold crossed
    int field_0x1bc;                    // +0x1bc
    int thrustStartY;                   // +0x1c0
    int field_0x1c4;                    // +0x1c4
    int thrustResetX;                   // +0x1c8
    int thrustThreshold;                // +0x1cc
    int thrustBase;                     // +0x1d0
    int field_0x1d4;                    // +0x1d4
    int field_0x1d8;                    // +0x1d8
    uint8_t field_0x1dd;                // +0x1dd
    int field_0x1e0;                    // +0x1e0
    uint16_t field_0x1e4;               // +0x1e4
    uint8_t field_0x1e6;                // +0x1e6
    int gameRecord;                     // +0x1e8  saved GameRecord* for replay
    Array<AbyssEngine::String*>* missionInfoLines;  // +0x1ec  wrapped mission-info overlay lines

    MGame();
    ~MGame();

    // Application-module lifecycle / per-frame callbacks.
    void OnInitialize();
    void OnRelease();
    void OnRender2D();
    void OnRender3D();
    void OnResume();
    void OnSuspend();
    void OnTouchBegin(int p1, int p2, void* touchId);
    void OnTouchEnd(int p1, int p2, void* touchId);
    void OnTouchMove(int p1, int y, void* touch);
    // Two-argument touch/key overrides: empty no-op stubs in this module (the real
    // dispatch lives in the void*-carrying overloads above).
    void OnTouchBegin(int p1, int p2);
    void OnTouchEnd(int p1, int p2);
    void OnTouchMove(int p1, int p2);
    long long OnKeyPress(long long key, long long mod);
    long long OnKeyRelease(long long key, long long mod);
    void showLiteScreen();
    int ShowLoadingScreen();
    void pause();
    void OnUpdate();

    void UseKhadorDrive();
    void dialogueEvent();
    void dockEvent(int p1, int p2);
    void freeCamTouchBegin(int x, int y, void* id);
    void freeCamTouchEnd(int p1, int p2, void* id);
    void freeCamTouchMove(int x, int y, void* touchId);
    void gameOverCheck();
    void handleAccelerometer();
    void maneuverTouchBegin(int x, int y, void* p);
    void maneuverTouchEnd(int a, int b, void* p);
    void maneuverTouchMove(int a, int b, void* p);
    int nextCamId(int cur);
    void pauseSounds();
    void reset();
    void resumeSounds();
    void setCinematicMode(bool on);
    void startChargingJumpDrive();
    void startJumpScene();
    void successCheck();
    void switchCamera(int id);
    void updateJumpScene();
    void useCloak();

    // Per-frame / dispatch bodies called by the thin entry points above.
    void tick(int frameDeltaMs);
    void dispatchTouchEndAction(int p1, int p2, void* touchId, unsigned hudResult,
                                int wasAutoPilot);
    void handleHudTouchAction(int p1, int p2, void* touchId, unsigned hudResult);

    // 2D overlay sub-draws (inlined in OnRender2D).
    void drawRadio();
    void drawRadar();
    void drawHud();
    void drawFadeMessage(int canvas);

    // Factored helpers used by OnInitialize / dockEvent / successCheck.
    void loadSoundResources();
    void restorePlayerStats();
    void setupWeaponsAndAudio();
    void buildDockChoice(int textId, int prefixLit, int suffixLit);
    void buildMissionFollowup();
};
#endif
