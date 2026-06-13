#ifndef GOF2_MGAME_H
#define GOF2_MGAME_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 — MGame class (Android libgof2hdaa.so, armv7 Thumb).
// Top-level class (no AbyssEngine:: namespace). Field offsets recovered from each
// per-method target disassembly. We do NOT model a full layout: methods access
// fields via byte-offset casts from `this` (which arrives in r0 just like the first
// explicit arg of an extern "C" function).
//
// The verify harness (tools/gofdiff.py) matches the built symbol by *substring* of
// the demangled name, so each method is authored as an extern "C" function whose
// name contains the target method name (e.g. MGame_OnResume). This sidesteps C++
// mangling while still producing the exact target code.
//
// Known field offsets (from per-method disasm):
//   0x00 vtable ptr
//   0x58 PlayerEgo*       0x5d  (byte) some pause flag source   0x64 String
//   0x78 Level*           0x1a6 (byte) paused flag
//   0x178 maneuver timer(int)  0x17c maneuver-active(bool)  0x180 maneuver startX
//   0x184 maneuver startPtr/Y

struct MGame;

// MGame-local opaque helper types (used only as pointers in this class).
struct Music;
struct Cfg;

// Enemy list returned by Level_getEnemies(): a simple {count, KIPlayer*[]} view.
//
// ---- methods (converted from free functions) ----
//   void OnInitialize();
//   void OnRelease();
//   void OnRender2D();
//   void OnRender3D();
//   void OnSuspend();
//   void OnTouchBegin(int p1, int p2, void *touchId);
//   void OnTouchEnd(int p1, int p2, void *touchId);
//   void OnTouchMove(int p1, int y, void *touch);
//   void OnUpdate();
//   void UseKhadorDrive();
//   MGame * ctor();
//   void deleting_dtor();
//   void dialogueEvent();
//   void dockEvent();
//   MGame * dtor();
//   void freeCamTouchBegin(int x, int y, int id);
//   void freeCamTouchEnd(int p1, int p2, int id);
//   void freeCamTouchMove(int x, int y, void *touchId);
//   void gameOverCheck();
//   void handleAccelerometer();
//   void maneuverTouchBegin(int x, int y);
//   void maneuverTouchEnd(int a, void *p);
//   void maneuverTouchMove(int a, int b);
//   int nextCamId(int cur);
//   void pauseSounds();
//   void reset();
//   void resumeSounds();
//   void setCinematicMode(bool on);
//   void startChargingJumpDrive();
//   void startJumpScene();
//   void successCheck();
//   void switchCamera(int id);
//   void updateJumpScene();
//   void useCloak();
struct EnemyList { uint32_t size; KIPlayer **data; };

// The recovered code uses the bare name String for the 12-byte stack String temp.
using AbyssEngine::String;

// Field accessor via byte offset.

class MGame {
public:
    int field_0x0;                      // +0x0
    unsigned field_0x4;                 // +0x4
    int field_0x8;                      // +0x8
    int field_0xc;                      // +0xc
    int field_0x10;                     // +0x10
    int field_0x14;                     // +0x14
    uint8_t field_0x18;                 // +0x18
    int field_0x1c;                     // +0x1c
    unsigned field_0x20;                // +0x20
    unsigned field_0x24;                // +0x24
    unsigned field_0x28;                // +0x28
    unsigned field_0x2c;                // +0x2c
    int field_0x30;                     // +0x30
    int field_0x34;                     // +0x34
    int field_0x40;                     // +0x40
    int field_0x44;                     // +0x44  (alt ship-field selected at runtime: 0x40 or 0x44)
    unsigned field_0x48;                // +0x48
    int field_0x4c;                     // +0x4c
    int field_0x50;                     // +0x50
    uint8_t field_0x54;                 // +0x54
    PlayerEgo* field_0x58;              // +0x58
    int field_0x5c;                     // +0x5c
    uint8_t field_0x5d;                 // +0x5d
    uint8_t field_0x5e;                 // +0x5e
    uint8_t field_0x5f;                 // +0x5f
    uint8_t field_0x60;                 // +0x60
    uint8_t field_0x61;                 // +0x61
    AbyssEngine::String field_0x64;     // +0x64  (game-over message title)
    int field_0x70;                     // +0x70
    Hud* field_0x74;                    // +0x74
    Level* field_0x78;                  // +0x78
    LevelScript* field_0x7c;            // +0x7c
    Radar* field_0x80;                  // +0x80
    Radio* field_0x84;                  // +0x84
    MenuTouchWindow* field_0x88;        // +0x88
    DialogueWindow* field_0x8c;         // +0x8c
    StarMap* field_0x90;                // +0x90
    ChoiceWindow* field_0x94;           // +0x94
    int field_0x98;                     // +0x98
    int field_0x9c;                     // +0x9c
    int field_0xa0;                     // +0xa0
    int field_0xa4;                     // +0xa4  (free-cam finger-1 pos: Vector{0xa4,0xa8,0xac})
    int field_0xa8;                     // +0xa8
    int field_0xac;                     // +0xac
    int field_0xb0;                     // +0xb0  (free-cam finger-0 pos: Vector{0xb0,0xb4,0xb8})
    int field_0xb4;                     // +0xb4
    int field_0xb8;                     // +0xb8
    float field_0xbc;                   // +0xbc
    void* field_0xc0;                   // +0xc0
    uint8_t field_0xc1;                 // +0xc1
    uint8_t field_0xc2;                 // +0xc2
    uint8_t field_0xc3;                 // +0xc3
    uint8_t field_0xc4;                 // +0xc4
    uint8_t field_0xc5;                 // +0xc5
    uint8_t field_0xc6;                 // +0xc6
    uint8_t field_0xc7;                 // +0xc7
    uint16_t field_0xc8;                // +0xc8
    uint8_t field_0xc9;                 // +0xc9
    int field_0xca;                     // +0xca
    uint8_t field_0xcb;                 // +0xcb
    uint8_t field_0xcc;                 // +0xcc
    uint8_t field_0xcd;                 // +0xcd
    uint8_t field_0xce;                 // +0xce
    uint8_t field_0xcf;                 // +0xcf
    int field_0xd0;                     // +0xd0
    uint8_t field_0xd4;                 // +0xd4
    uint8_t field_0xd5;                 // +0xd5
    uint8_t field_0xd6;                 // +0xd6
    int field_0xd8;                     // +0xd8
    uint8_t field_0xdc;                 // +0xdc
    uint8_t field_0xdd;                 // +0xdd
    uint8_t field_0xe0;                 // +0xe0
    int field_0xe4;                     // +0xe4
    int field_0xe8;                     // +0xe8
    int field_0xec;                     // +0xec
    unsigned field_0xf0;                // +0xf0
    TargetFollowCamera* field_0xf4;     // +0xf4
    int field_0xf8;                     // +0xf8
    int field_0x100;                    // +0x100
    int field_0x104;                    // +0x104
    int field_0x108;                    // +0x108
    int field_0x10c;                    // +0x10c
    uint8_t field_0x110;                // +0x110
    uint8_t field_0x111;                // +0x111
    AEGeometry* field_0x114;            // +0x114
    float field_0x118;                  // +0x118
    float field_0x11c;                  // +0x11c
    int field_0x120;                    // +0x120
    int field_0x124;                    // +0x124
    int field_0x128;                    // +0x128
    int field_0x12c;                    // +0x12c
    int field_0x130;                    // +0x130
    int field_0x134;                    // +0x134
    int field_0x138;                    // +0x138
    float field_0x13c;                  // +0x13c
    float field_0x140;                  // +0x140
    float field_0x148;                  // +0x148
    float field_0x14c;                  // +0x14c
    int field_0x150;                    // +0x150
    int field_0x154;                    // +0x154
    int field_0x158;                    // +0x158
    uint8_t field_0x15c;                // +0x15c
    uint8_t field_0x15d;                // +0x15d
    uint8_t field_0x15e;                // +0x15e
    int field_0x160;                    // +0x160
    uint8_t field_0x164;                // +0x164
    int field_0x168;                    // +0x168
    int field_0x16c;                    // +0x16c
    int field_0x178;                    // +0x178
    uint8_t field_0x17c;                // +0x17c
    int field_0x180;                    // +0x180
    int field_0x184;                    // +0x184
    int field_0x19c;                    // +0x19c
    int field_0x1a0;                    // +0x1a0
    uint8_t field_0x1a6;                // +0x1a6
    int field_0x1a8;                    // +0x1a8
    int field_0x1ac;                    // +0x1ac
    int field_0x1b0;                    // +0x1b0
    int field_0x1b4;                    // +0x1b4
    uint16_t field_0x1b8;               // +0x1b8
    uint8_t field_0x1b9;                // +0x1b9
    int field_0x1bc;                    // +0x1bc
    int field_0x1c0;                    // +0x1c0
    int field_0x1c4;                    // +0x1c4
    int field_0x1c8;                    // +0x1c8
    int field_0x1cc;                    // +0x1cc
    int field_0x1d0;                    // +0x1d0
    int field_0x1d4;                    // +0x1d4
    int field_0x1d8;                    // +0x1d8
    uint8_t field_0x1dd;                // +0x1dd
    int field_0x1e0;                    // +0x1e0
    uint16_t field_0x1e4;               // +0x1e4
    uint8_t field_0x1e6;                // +0x1e6
    int field_0x1e8;                    // +0x1e8
    Array<AbyssEngine::String*>* field_0x1ec;  // +0x1ec  (wrapped mission-info overlay lines)

    // ---- methods (converted from free functions) ----
    void OnInitialize();
    void OnRelease();
    void OnRender2D();
    void OnRender3D();
    void OnSuspend();
    void OnTouchBegin(int p1, int p2, void *touchId);
    void OnTouchEnd(int p1, int p2, void *touchId);
    void OnTouchMove(int p1, int y, void *touch);
    void OnUpdate();
    void UseKhadorDrive();
    MGame();
    ~MGame();
    void dialogueEvent();
    void dockEvent();
    void freeCamTouchBegin(int x, int y, int id);
    void freeCamTouchEnd(int p1, int p2, int id);
    void freeCamTouchMove(int x, int y, void *touchId);
    void gameOverCheck();
    void handleAccelerometer();
    void maneuverTouchBegin(int x, int y);
    void maneuverTouchEnd(int a, void *p);
    void maneuverTouchMove(int a, int b);
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

    // ---- per-frame / dispatch bodies (recovered, called by the thin entry points) ----
    // Full per-frame world update (the body of OnUpdate); receives the clamped delta.
    void tick(int frameDeltaMs);
    // Button-action dispatch tail of OnTouchEnd (consumes the hud touch-result bits).
    void dispatchTouchEndAction(int p1, int p2, void *touchId, unsigned hudResult,
                                int wasAutoPilot);
    // HUD touch-result dispatch tail of OnTouchBegin.
    void handleHudTouchAction(int p1, int p2, void *touchId, unsigned hudResult);
    // Finish a hyperspace jump: advance to the next campaign mission.
    void jumpFinish();
    // The jump star map / direct-jump just became visible: close the open HUD menu.
    void starMapShown();
    // Pan/zoom gesture finished while mining: clean no-op exit path.
    void freeCamPanDone(int touchY);
    // End the running game module and hand control back to application module `code`.
    void endRunModule(int code);
    // 2D overlay helpers (sub-draws inlined in OnRender2D).
    void drawRadio();
    void drawRadar();
    void drawHud();
    void drawFadeMessage(int canvas);

    // ---- recovered factored helpers (wave 2) ----
    // Build the per-session FMOD sound-resource list (part of OnInitialize).
    void loadSoundResources();
    // Restore the player's HP/shield/armor/gamma and per-ship max caps after a level
    // is (re)initialised (part of OnInitialize).
    void restorePlayerStats();
    // Pick the active secondary weapon, start engine/music/particle/post-effect state
    // and raise the mission-info overlay (tail of OnInitialize).
    void setupWeaponsAndAudio();
    // Build and raise the "dock at <station>?" choice dialogue (part of dockEvent).
    void buildDockChoice(int textId, int prefixLit, int suffixLit);
    // Convert a completed campaign mission into its follow-up delivery mission and
    // raise its briefing (part of successCheck).
    void buildMissionFollowup();
};
#endif
