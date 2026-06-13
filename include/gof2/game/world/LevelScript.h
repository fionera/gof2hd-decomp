#ifndef GOF2_LEVELSCRIPT_H
#define GOF2_LEVELSCRIPT_H
#include "gof2/common.h"
// Field layout recovered from the decompiled accessor/ctor bodies and the
// android_2.0.16 Ghidra struct (LevelScript). Offsets are the natural 64-bit
// layout of this port; the "// +0xNN" comments are the in-object byte offsets.
struct Level;
struct Hud;
struct Radar;
struct TargetFollowCamera;
struct AEGeometry;
struct Explosion;
struct KIPlayer;
struct Player;
struct PlayerFighter;
struct RadioMessage;
struct Route;
struct SolarSystem;
struct StarSystem;
struct Station;

class LevelScript {
public:
    int m_nTimeLimit;            // +0x00  Level::getTimeLimit() snapshot
    int field_0x4;               // +0x04
    int field_0x8;               // +0x08
    int field_0xc;               // +0x0c
    uint16_t m_nFlags;           // +0x10  init 0x100
    uint16_t field_0x12;         // +0x12
    TargetFollowCamera *m_pCamera; // +0x14
    Level *m_pLevel;             // +0x18
    int m_nState;                // +0x1c  current scripted-event / start-sequence step id
    uint16_t m_bStartSequence;   // +0x20  start-sequence active flag (init 1)
    uint8_t m_bStartSequenceOver; // +0x21
    int field_0x24;              // +0x24  mission cinematic timer/flag
    int field_0x28;              // +0x28
    int field_0x2c;              // +0x2c
    int field_0x30;              // +0x30
    int field_0x34;              // +0x34
    int field_0x38;              // +0x38
    int field_0x3c;              // +0x3c
    int field_0x40;              // +0x40
    int field_0x44;              // +0x44
    int field_0x48;              // +0x48
    Matrix m_matrix;             // +0x4c  16 floats (64 bytes)
    int field_0x8c;              // +0x8c
    int field_0x90;              // +0x90  mission script timer
    int field_0x94;              // +0x94  mission script counter
    int field_0x98;              // +0x98  mission script timer
    int field_0x9c;              // +0x9c
    int field_0xa0;              // +0xa0
    int field_0xa4;              // +0xa4
    uint8_t field_0xa8;          // +0xa8  enables the gRenderProc particle pass
    void *m_pParticleGeom0;      // +0xac  AEGeometry* rendered via gRenderProc
    void *m_pParticleGeom1;      // +0xb0  AEGeometry*
    void *m_pParticleGeom2;      // +0xb4  AEGeometry*
    void *m_pGeometry0;          // +0xb8  AEGeometry*
    void *m_pGeometry1;          // +0xbc  AEGeometry*
    void *m_pGeometry2;          // +0xc0  AEGeometry*
    void *m_pGeometry3;          // +0xc4  AEGeometry*
    void *m_pExplosion;          // +0xc8  Explosion*
    void *m_pGeometry4;          // +0xcc  AEGeometry*
    void *m_pHud;                // +0xd0  Hud*
    void *m_pRadar;              // +0xd4  Radar*
    void *m_pGeometry5;          // +0xd8  AEGeometry*
    void *m_pGeometry6;          // +0xdc  AEGeometry*

    LevelScript(Level *level, Hud *hud, Radar *radar, TargetFollowCamera *camera);
    ~LevelScript();
    void render3D();
    uint8_t startSequenceOver();
    void resetCamera(Level *level);
    void skipSequence();
    uint8_t startSequence();
    void setAutoPilotToProgrammedStation();
    uint32_t canSkipCutsceneNow();
    void resetStartSequenceOver();
    void skipCutscene();
    void process(int delta);
    void lookBehind();
    int getEvent();
    void setEvent(int event);
};
#endif
