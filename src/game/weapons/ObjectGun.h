#ifndef GOF2_OBJECTGUN_H
#define GOF2_OBJECTGUN_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"

// Galaxy on Fire 2 -- ObjectGun: the renderable/animated wrapper around a Gun.
// It owns the weapon's mesh transform, optional muzzle/effect geometry, and (for
// cluster weapons) a set of per-shot Explosion effects. Top-level class (no namespace).

class AEGeometry;
class Explosion;
class Gun;
class Level;
class Player;

class ObjectGun {
public:
    // (the +0x0 vptr is the compiler-managed C++ vtable; the class is polymorphic)
    int unusedSlot;                     // +0x4  init -1, never read (unused id/slot)
    Gun* gun;                           // +0x8  owned weapon
    Level* level;                       // +0xc  owning level
    uint32_t transform;                 // +0x10 primary mesh transform handle
    int secondaryTransform;             // +0x14 init -1, used by TransformSetLocal
    AEGeometry* geometry;               // +0x18 muzzle/effect geometry
    uint8_t hasGeometry;                // +0x1c geometry created flag
    uint8_t wasFiring;                  // +0x1d prev-frame delayActive snapshot
    float rollAngle;                    // +0x20 roll angle for secondary transform
    uint8_t useEgoOrientation;          // +0x24 orientation source flag
    int meshId;                         // +0x28 gun mesh id
    Array<Explosion*>* explosions;      // +0x2c explosions for cluster guns
    uint8_t* explosionReady;            // +0x30 per-explosion ready flags
    int deltaTime;                      // +0x34 last update delta (ms)
    float scaleX;                       // +0x3c
    float scaleY;                       // +0x40
    float scaleZ;                       // +0x44
    float spinAngle;                    // +0x48 beam/sentry roll angle
    uint8_t visible;                    // +0x4c
    Vector dir;                         // +0x50 forward direction
    Vector up;                          // +0x60 up vector
    Vector side;                        // +0x70 side vector
    Matrix orientation;                 // +0x84 world orientation matrix

    ObjectGun(int unused, Gun* gun, int mesh, uint32_t param, Level* level);
    virtual ~ObjectGun();
    virtual int isRocketGun();          // vtable slot 8 (base: not a rocket gun)

    void setScaling(int x, int y);
    void setScaling(int x, int y, int z);
    void replaceGun(int mesh);
    void replaceGun(unsigned int mesh, int unused);
    void setEnemies(Array<Player*>* enemies);
    void update(int dt);
    void render();
};
#endif
