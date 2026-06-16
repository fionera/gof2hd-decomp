#ifndef GOF2_ROCKETGUN_H
#define GOF2_ROCKETGUN_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "mathtypes.h"
#include "game/weapons/ObjectGun.h"

// Galaxy on Fire 2 -- RocketGun: an ObjectGun that fires homing/cluster rockets.
// Owns the per-shot rocket-trail effects (transform matrices, particle systems
// and fade timers) and, for homing weapons, steers active shots toward the
// current target. Top-level class (no namespace).

class Gun;
class Level;
namespace AbyssEngine { class Radar; }

class RocketGun : public ObjectGun {
public:
    AbyssEngine::Radar* radar;          // owning radar (provides the level/target)
    int steerX;                         // accumulated steering vector toward target
    int steerY;
    int steerZ;
    uint8_t homing;                     // shots track the locked target
    int activeShotId;                   // +0xc4 active homing shot index (-1 == none)
    uint32_t turnRate;                  // homing turn-rate scalar
    int particleSystem;                 // single trail/muzzle particle system handle
    int rocketKind;                     // rocket weapon kind
    int fadeTimer;                      // trail fade-out countdown (ms)
    Array<Matrix>* trailMatrices;       // per-shot trail transform matrices
    Array<int>* trailSystems;           // per-shot particle-system handles
    Array<int>* trailTimers;            // per-shot trail fade timers
    int particleManager;                // ParticleSystemManager handle for the trails

    RocketGun(int param_1, Gun* param_2, int param_3, int param_4,
              uint32_t param_5, int param_6, bool param_7, Level* param_8);
    ~RocketGun();

    void render();
    void setRadar(AbyssEngine::Radar* radar);
    void seekEnemy(int unused, int index);
    void update(int elapsed);
};
#endif
