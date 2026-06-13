#ifndef GOF2_PLAYERWORMHOLE_H
#define GOF2_PLAYERWORMHOLE_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 -- PlayerWormHole (Android libgof2hdaa.so, armv7 Thumb).
// Target names are top-level PlayerWormHole::... (no AbyssEngine namespace).
// Field offsets come from the per-method work-items and are accessed by byte casts.

struct PlayerWormHole;
struct AEGeometry;

namespace AbyssEngine {
struct String;

namespace AEMath {

}
}

using Vector = AbyssEngine::AEMath::Vector;
using Matrix = AbyssEngine::AEMath::Matrix;

// Typed byte-offset accessor for foreign opaque pointers (e.g. the AEGeometry/Transform
// objects this class holds but whose layout is owned by other classes).

class PlayerWormHole {
public:
    char* vtable;                    // +0x0   vtable
    void* player;                    // +0x4   Player*
    void* geometry;                    // +0x8   AEGeometry*
    void* level;                   // +0x54  Level*
    float positionX;                   // +0x58  position x
    float positionY;                   // +0x5c  position y
    float positionZ;                   // +0x60  position z
    float directionX;                   // +0x90  direction/target vec x
    float directionY;                   // +0x94  direction/target vec y
    float directionZ;                   // +0x98  direction/target vec z
    uint8_t visible;                 // +0xf5  visible/active flag
    int intPositionX;                    // +0x124 int position x
    int intPositionY;                    // +0x128 int position y
    int intPositionZ;                    // +0x12c int position z
    int timer;                    // +0x150 timer
    int field_0x154;                    // +0x154 scale (12.4 fixed)
    uint8_t missionLock;                // +0x15c mission-lock flag

    bool isShrinking();
    int open(char *__file, int __oflag, ...);
    Vector getPosition();
    void freeMissionLock();
    void render();
    void reset(bool shrinking);
    void setPosition(float x, float y, float z);
    void update(int elapsed);
    PlayerWormHole(int playerId, AEGeometry *geometry, float x, float y, float z, bool visible);
};
#endif
