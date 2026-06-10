#ifndef GOF2_LEVELSCRIPT_H
#define GOF2_LEVELSCRIPT_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
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



// Only P() is owned here: B() and I() are provided (identically) by sibling
// headers PlayerEgo.h / Explosion.h, so redefining them here would collide
// within the same translation unit.
static inline void *&P(void *self, int offset) { return *(void **)((char *)self + offset); }

struct LevelScript {
    void* _opaque;  // offset-accessed via the B/I/U/F/UC/US/P helpers

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
};
#endif
