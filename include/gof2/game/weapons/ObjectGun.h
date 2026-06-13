#ifndef GOF2_OBJECTGUN_H
#define GOF2_OBJECTGUN_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
struct AEGeometry;
struct Explosion;
struct Gun;
struct Level;
struct ObjectGun;
struct PaintCanvas;
struct Player;
struct TargetFollowCamera;

struct Vec4 {
    float x;
    float y;
    float z;
    float w;
};

struct MeshId {
    uint16_t id;
    uint16_t pad;
};

class ObjectGun {
public:
    void* vtable;                       // +0x0   vtable pointer
    int field_0x4;                      // +0x4   init -1 (unused id/slot)
    Gun* gun;                           // +0x8   owned weapon
    Level* level;                       // +0xc   owning level
    uint32_t transform;                 // +0x10  primary mesh transform handle
    int secondaryTransform;             // +0x14  init -1, used by TransformSetLocal
    AEGeometry* geometry;               // +0x18  muzzle/effect geometry
    uint8_t hasGeometry;                // +0x1c  geometry created flag
    uint8_t wasFiring;                  // +0x1d  prev-frame delayActive snapshot
    float rollAngle;                    // +0x20  roll angle for secondary transform
    uint8_t useEgoOrientation;          // +0x24  orientation source flag
    int meshId;                         // +0x28  gun mesh id
    Array<Explosion*>* explosions;      // +0x2c  explosions for cluster guns
    uint8_t* explosionReady;            // +0x30  per-explosion ready flags
    int deltaTime;                      // +0x34  last update delta (ms)
    float scaleX;                       // +0x3c  scale x
    float scaleY;                       // +0x40  scale y
    float scaleZ;                       // +0x44  scale z
    float spinAngle;                    // +0x48  beam/sentry roll angle
    uint8_t visible;                    // +0x4c
    Vector dir;                         // +0x50  forward direction
    Vector up;                          // +0x5c  up vector
    Vector side;                        // +0x68  side vector
    Matrix orientation;                 // +0x74  world orientation matrix

    ObjectGun(int, Gun *gun, int mesh, uint32_t, Level *level);
    ~ObjectGun();
    void setScaling(int x, int y);
    void replaceGun(int mesh);
    void setEnemies(Array<Player*> *enemies);
    void update(int dt);
    void render();
};
#endif
