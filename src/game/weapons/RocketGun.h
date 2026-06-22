#ifndef GOF2_ROCKETGUN_H
#define GOF2_ROCKETGUN_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "engine/math/Matrix.h"
#include "engine/math/Vector.h"
#include "game/weapons/ObjectGun.h"

class Gun;
class Level;
class Radar;

class RocketGun : public ObjectGun {
public:
    Radar *radar; // owning radar (provides the level/target)
    int steerX; // accumulated steering vector toward target
    int steerY;
    int steerZ;
    uint8_t homing; // shots track the locked target
    int activeShotId; // +0xc4 active homing shot index (-1 == none)
    float turnRate; // homing turn-rate scalar (0.3)
    int particleSystem; // single trail/muzzle particle system handle
    int rocketKind; // rocket weapon kind
    int fadeTimer; // trail fade-out countdown (ms)
    Array<AbyssEngine::AEMath::Matrix> *trailMatrices; // per-shot trail transform matrices
    Array<int> *trailSystems; // per-shot particle-system handles
    Array<int> *trailTimers; // per-shot trail fade timers
    int particleManager; // ParticleSystemManager handle for the trails

    RocketGun(int meshVariantId, Gun *gun, int mesh, int unused4,
              uint32_t flags, int rocketKind, bool homing, Level *level);

    ~RocketGun();

    int isRocketGun() override; // vtable slot 8: yes

    void render();

    void translate(const AbyssEngine::AEMath::Vector &v); // no-op override (rockets ignore camera translation)
    void setRadar(Radar *radar);

    void seekEnemy(int unused, int index);

    void update(int elapsed);
};
#endif
