#ifndef GOF2_MINEGUN_H
#define GOF2_MINEGUN_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
struct Gun;
struct Level;
struct PlayerEgo;
struct Explosion;
struct AEGeometry;
struct TargetFollowCamera;

namespace AbyssEngine {
namespace AEMath {

}
}

typedef AbyssEngine::AEMath::Vector Vector;

typedef int v4i __attribute__((__vector_size__(16), __aligned__(4)));

// MineGun extends ObjectGun: it shares the ObjectGun base region (vtable, owning
// Gun, transform/geometry handles) and adds the mine-specific explosion pool,
// rumble state and a back-reference to the owning player. Layout recovered from
// per-method byte-offset access; ObjectGun base fields are named for the regions
// this class actually touches.
class MineGun {
public:
    void* vtable;                       // +0x0   (ObjectGun base) vtable
    int field_0x4;                      // +0x4   (ObjectGun base)
    Gun* gun;                           // +0x8   (ObjectGun base) owning gun
    Level* level;                       // +0xc   (ObjectGun base)
    uint32_t transformId;               // +0x10  (ObjectGun base)
    // ObjectGun base padding up to the MineGun-specific region.
    char base_pad[0x9c];                // +0x14 .. +0xb0 (ObjectGun base fields)
    PlayerEgo* player;                  // +0xb0  owning player (rumble target)
    Array<Explosion*>* explosions;      // +0xb4  per-mine explosion pool
    void* readyFlags;                   // +0xb8  per-mine "ready to detonate" byte flags (operator new[])
    AEGeometry* geometry;               // +0xbc  mine muzzle geometry
    int field_0xc0;                     // +0xc0  (ctor-zeroed; semantics not in binary)
    int field_0xc4;                     // +0xc4  (ctor-zeroed; semantics not in binary)
    int field_0xc8;                     // +0xc8  (ctor-zeroed; semantics not in binary)
    int rumbleTimer;                    // +0xcc  rumble decay timer
    float rumblePercentage;             // +0xd0  current rumble intensity

    void render();
    void setPlayer(PlayerEgo *player);
    void update(int delta);
};
#endif
