#ifndef GOF2_BEAMGUN_H
#define GOF2_BEAMGUN_H
#include <cstdint>
#include "gof2/common.h"

// Galaxy on Fire 2 -- BeamGun: a beam weapon that renders a primary beam
// geometry plus an optional secondary (muzzle) geometry tracking the gun's
// delay state.

class Gun;
class Level;
class Player;
class AEGeometry;

class BeamGun {
public:
    void* vtable;
    int32_t field_0x4;                  // zero-initialised; semantics not recovered
    Gun* gun;                           // owning gun
    Level* level;
    int32_t owner;                      // owner id
    int32_t meshKind;                   // mesh-kind index
    AEGeometry* primaryGeometry;        // primary beam geometry
    AEGeometry* secondaryGeometry;      // secondary beam geometry
    uint8_t hasSecondary;               // secondary geometry present
    uint8_t secondaryVisible;           // secondary visible (tracks gun delay)

    BeamGun(int owner, Gun* gun, int meshKind, Level* level);
    ~BeamGun();

    void render();
    void update(int elapsed);
    void setEnemies(Array<Player*>* enemies);
    void setEnemy(Player* enemy);
};
#endif
