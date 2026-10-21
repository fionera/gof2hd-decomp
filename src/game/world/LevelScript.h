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
    long long m_nScriptTime;
    uint8_t field_0x10;
    uint8_t m_bCinematicBreak; // returned by process(): a cutscene is running
    uint16_t field_0x12;
    TargetFollowCamera *m_pCamera;
    Level *m_pLevel;
    int m_nState;
    bool m_bStartSequence; // ASM (resetStartSequenceOver): m_bStartSequenceOver is at 0x21, so this is 1 byte
    bool m_bStartSequenceOver;
    int field_0x24;
    Vector m_tempVec; // scratch vector (camera seats, wormhole placement)
    Vector m_camVec;  // camera position derived from m_tempVec
    Vector m_dirVec;  // scratch direction
    Matrix m_matrix;
    // Original reserves 0x40 for the matrix region (0x4c..0x8b); our AEMath::Matrix is 0x3c, so the
    // trailing 4 bytes are explicit padding -- keeps field_0x8c.. at their Ghidra-named offsets.
    uint8_t _pad_0x88[4];
    int field_0x8c;
    long long m_nScriptTimerA;
    long long m_nScriptTimerB;
    long long m_nFlashCountdown; // 0xa0: delay before the station-shield flash (mission 157)
    uint8_t m_bRenderParticles;
    uint8_t field_0xa9; // fail-check enable flag (read by process() mission-fail tail @0x144e36)
    uint8_t field_0xaa; // cutscene-exit save of PlayerEgo::autoTurretIsEnabled() (process() case 154 @0x13a802)
    uint8_t field_0xab; // cutscene-exit save of PlayerEgo::isInTurretMode() (process() case 94 @0x13ad82)
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

    bool startSequenceOver();

    void resetCamera(Level *level);

    void skipSequence();

    bool startSequence();

    void setAutoPilotToProgrammedStation();

    bool canSkipCutsceneNow();

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
static_assert(__builtin_offsetof(LevelScript, m_nFlashCountdown) == 0xa0, "LevelScript::m_nFlashCountdown");
#endif
#endif
