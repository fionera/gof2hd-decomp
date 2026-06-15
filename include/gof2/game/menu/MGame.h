#ifndef GOF2_MGAME_H
#define GOF2_MGAME_H
#include "gof2/common.h"

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

// Small opaque singletons referenced only as pointers from MGame.cpp.
struct Music;
struct Cfg;

class MGame {
public:
    int field_0x0;
    unsigned field_0x4;
    int field_0x8;
    int field_0xc;
    int field_0x10;
    int field_0x14;
    uint8_t field_0x18;
    int field_0x1c;
    unsigned field_0x20;
    unsigned field_0x24;
    unsigned field_0x28;
    unsigned field_0x2c;
    int field_0x30;
    int field_0x34;
    int field_0x40;
    int field_0x44;
    unsigned field_0x48;
    int field_0x4c;
    int field_0x50;
    uint8_t field_0x54;
    PlayerEgo* field_0x58;
    int field_0x5c;
    uint8_t field_0x5d;
    uint8_t field_0x5e;
    uint8_t field_0x5f;
    uint8_t field_0x60;
    uint8_t field_0x61;
    AbyssEngine::String field_0x64;     // game-over message title
    int field_0x70;
    Hud* field_0x74;
    Level* field_0x78;
    LevelScript* field_0x7c;
    Radar* field_0x80;
    Radio* field_0x84;
    MenuTouchWindow* field_0x88;
    DialogueWindow* field_0x8c;
    StarMap* field_0x90;
    ChoiceWindow* field_0x94;
    int field_0x98;
    int field_0x9c;
    int field_0xa0;
    int field_0xa4;                     // free-cam finger-1 pos: Vector{0xa4,0xa8,0xac}
    int field_0xa8;
    int field_0xac;
    int field_0xb0;                     // free-cam finger-0 pos: Vector{0xb0,0xb4,0xb8}
    int field_0xb4;
    int field_0xb8;
    float field_0xbc;
    void* field_0xc0;
    uint8_t field_0xc1;
    uint8_t field_0xc2;
    uint8_t field_0xc3;
    uint8_t field_0xc4;
    uint8_t field_0xc5;
    uint8_t field_0xc6;
    uint8_t field_0xc7;
    uint16_t field_0xc8;
    uint8_t field_0xc9;
    int field_0xca;
    uint8_t field_0xcb;
    uint8_t field_0xcc;
    uint8_t field_0xcd;
    uint8_t field_0xce;
    uint8_t field_0xcf;
    int field_0xd0;
    uint8_t field_0xd4;
    uint8_t field_0xd5;
    uint8_t field_0xd6;
    int field_0xd8;
    uint8_t field_0xdc;
    uint8_t field_0xdd;
    uint8_t field_0xe0;
    int field_0xe4;
    int field_0xe8;
    int field_0xec;
    unsigned field_0xf0;
    TargetFollowCamera* field_0xf4;
    int field_0xf8;
    int field_0x100;
    int field_0x104;
    int field_0x108;
    int field_0x10c;
    uint8_t field_0x110;
    uint8_t field_0x111;
    AEGeometry* field_0x114;
    float field_0x118;
    float field_0x11c;
    int field_0x120;
    int field_0x124;
    int field_0x128;
    int field_0x12c;
    int field_0x130;
    int field_0x134;
    int field_0x138;
    float field_0x13c;
    float field_0x140;
    float field_0x148;
    float field_0x14c;
    int field_0x150;
    int field_0x154;
    int field_0x158;
    uint8_t field_0x15c;
    uint8_t field_0x15d;
    uint8_t field_0x15e;
    int field_0x160;
    uint8_t field_0x164;
    int field_0x168;
    int field_0x16c;
    int field_0x178;
    uint8_t field_0x17c;
    int field_0x180;
    int field_0x184;
    int field_0x19c;
    int field_0x1a0;
    uint8_t field_0x1a6;
    int field_0x1a8;
    int field_0x1ac;
    int field_0x1b0;
    int field_0x1b4;
    uint16_t field_0x1b8;
    uint8_t field_0x1b9;
    int field_0x1bc;
    int field_0x1c0;
    int field_0x1c4;
    int field_0x1c8;
    int field_0x1cc;
    int field_0x1d0;
    int field_0x1d4;
    int field_0x1d8;
    uint8_t field_0x1dd;
    int field_0x1e0;
    uint16_t field_0x1e4;
    uint8_t field_0x1e6;
    int field_0x1e8;
    Array<AbyssEngine::String*>* field_0x1ec;  // wrapped mission-info overlay lines

    MGame();
    ~MGame();

    // Application-module lifecycle / per-frame callbacks.
    void OnInitialize();
    void OnRelease();
    void OnRender2D();
    void OnRender3D();
    void OnSuspend();
    void OnTouchBegin(int p1, int p2, void* touchId);
    void OnTouchEnd(int p1, int p2, void* touchId);
    void OnTouchMove(int p1, int y, void* touch);
    void OnUpdate();

    void UseKhadorDrive();
    void dialogueEvent();
    void dockEvent();
    void freeCamTouchBegin(int x, int y, int id);
    void freeCamTouchEnd(int p1, int p2, int id);
    void freeCamTouchMove(int x, int y, void* touchId);
    void gameOverCheck();
    void handleAccelerometer();
    void maneuverTouchBegin(int x, int y);
    void maneuverTouchEnd(int a, void* p);
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

    // Per-frame / dispatch bodies called by the thin entry points above.
    void tick(int frameDeltaMs);
    void dispatchTouchEndAction(int p1, int p2, void* touchId, unsigned hudResult,
                                int wasAutoPilot);
    void handleHudTouchAction(int p1, int p2, void* touchId, unsigned hudResult);
    // Finish a hyperspace jump: advance to the next campaign mission.
    void jumpFinish();
    // The jump star map became visible: close the open HUD menu.
    void starMapShown();
    // Pan/zoom gesture finished while mining: clean no-op exit path.
    void freeCamPanDone(int touchY);
    // End the running game module and hand control back to application module `code`.
    void endRunModule(int code);

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
