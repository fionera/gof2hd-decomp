#ifndef GOF2_OBJECTGUN_H
#define GOF2_OBJECTGUN_H
#include "gof2/common.h"

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
    void* vtable;
    int field_0x4;                      // init -1 (unused id/slot)
    Gun* gun;                           // owned weapon
    Level* level;                       // owning level
    uint32_t transform;                 // primary mesh transform handle
    int secondaryTransform;             // init -1, used by TransformSetLocal
    AEGeometry* geometry;               // muzzle/effect geometry
    uint8_t hasGeometry;                // geometry created flag
    uint8_t wasFiring;                  // prev-frame delayActive snapshot
    float rollAngle;                    // roll angle for secondary transform
    uint8_t useEgoOrientation;          // orientation source flag
    int meshId;                         // gun mesh id
    Array<Explosion*>* explosions;      // explosions for cluster guns
    uint8_t* explosionReady;            // per-explosion ready flags
    int deltaTime;                      // last update delta (ms)
    float scaleX;
    float scaleY;
    float scaleZ;
    float spinAngle;                    // beam/sentry roll angle
    uint8_t visible;
    Vector dir;                         // forward direction
    Vector up;                          // up vector
    Vector side;                        // side vector
    Matrix orientation;                 // world orientation matrix

    ObjectGun(int unused, Gun* gun, int mesh, uint32_t param, Level* level);
    ~ObjectGun();

    void setScaling(int x, int y);
    void replaceGun(int mesh);
    void setEnemies(Array<Player*>* enemies);
    void update(int dt);
    void render();
};
#endif
