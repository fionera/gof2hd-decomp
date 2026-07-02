#ifndef GOF2_LEVELSCRIPT_H
#define GOF2_LEVELSCRIPT_H
#include "engine/core/Array.h"
#include "../../engine/core/AEString.h"
#include "game/ship/TargetFollowCamera.h"

#include "engine/math/Matrix.h"

class Radar;

class AEGeometry;
class Explosion;
class Hud;
class Level;
class TargetFollowCamera;

class LevelScript {
public:
    int m_nTimeLimit;
    int field_0x4;
    int field_0x8;
    int field_0xc;
    uint16_t m_nFlags;
    uint16_t field_0x12;
    TargetFollowCamera *m_pCamera;
    Level *m_pLevel;
    int m_nState;
    uint8_t m_bStartSequence; // ASM (resetStartSequenceOver): m_bStartSequenceOver is at 0x21, so this is 1 byte
    uint8_t m_bStartSequenceOver;
    int field_0x24;
    int field_0x28;
    int field_0x2c;
    int field_0x30;
    int field_0x34;
    int field_0x38;
    int field_0x3c;
    int field_0x40;
    int field_0x44;
    int field_0x48;
    Matrix m_matrix;
    // Original reserves 0x40 for the matrix region (0x4c..0x8b); our AEMath::Matrix is 0x3c, so the
    // trailing 4 bytes are explicit padding -- keeps field_0x8c.. at their Ghidra-named offsets.
    uint8_t _pad_0x88[4];
    int field_0x8c;
    int m_nScriptTimerA;
    int m_nScriptCounterA;
    int m_nScriptTimerB;
    int m_nScriptCounterB;
    int field_0xa0;
    int field_0xa4;
    uint8_t m_bRenderParticles;
    uint8_t field_0xa9; // fail-check enable flag (read by process() mission-fail tail @0x144e36)
    uint8_t field_0xaa; // cutscene-exit save of PlayerEgo::autoTurretIsEnabled() (process() case 154 @0x13a802)
    AEGeometry *m_pParticleGeom0;
    AEGeometry *m_pParticleGeom1;
    AEGeometry *m_pParticleGeom2;
    AEGeometry *m_pGeometry0;
    AEGeometry *m_pGeometry1;
    AEGeometry *m_pGeometry2;
    AEGeometry *m_pGeometry3;
    Explosion *m_pExplosion;
    AEGeometry *m_pGeometry4;
    Hud *m_pHud;
    Radar *m_pRadar;
    AEGeometry *m_pGeometry5;
    AEGeometry *m_pGeometry6;
    int field_0xe0; // particle-emit system handle A (process() case 41 @0x139dac)
    int field_0xe4; // particle-emit system handle B (process() case 41 @0x139dba)

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

    int process(int delta);

    void lookBehind();

    int getEvent();

    void setEvent(int event);
};

#if __SIZEOF_POINTER__ == 4
#include <cstddef>
static_assert(__builtin_offsetof(LevelScript, field_0x8c) == 0x8c, "LevelScript::field_0x8c");
static_assert(__builtin_offsetof(LevelScript, field_0xa0) == 0xa0, "LevelScript::field_0xa0");
static_assert(__builtin_offsetof(LevelScript, field_0xa4) == 0xa4, "LevelScript::field_0xa4");
#endif
#endif
