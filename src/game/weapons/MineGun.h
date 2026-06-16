#ifndef GOF2_MINEGUN_H
#define GOF2_MINEGUN_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "game/weapons/ObjectGun.h"

class PlayerEgo;
class Explosion;
class AEGeometry;

// MineGun is a cluster gun that detonates proximity mines: it drives one
// Explosion per barrel and feeds the player camera a distance-based rumble
// while a mine is going off.
class MineGun : public ObjectGun {
public:
    PlayerEgo*           player;            // owning player (rumble target)
    Array<Explosion*>*   explosions;        // per-mine explosion pool
    uint8_t*             readyFlags;        // per-mine "ready to detonate" flags
    AEGeometry*          geometry;          // mine muzzle geometry
    int                  field_0xc0;
    int                  field_0xc4;
    int                  field_0xc8;
    int                  rumbleTimer;       // rumble decay timer
    float                rumblePercentage;  // current rumble intensity

    MineGun(Gun* gun, int mesh, int param, int unused, Level* level);
    ~MineGun();

    void render();
    void setPlayer(PlayerEgo* player);
    void update(int delta);
};
#endif
