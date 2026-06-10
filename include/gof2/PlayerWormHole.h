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

struct PlayerWormHole {
    char* field_0x0;                    // +0x0   vtable
    void* field_0x4;                    // +0x4   Player*
    void* field_0x8;                    // +0x8   AEGeometry*
    void* field_0x54;                   // +0x54  Level*
    float field_0x58;                   // +0x58  position x
    float field_0x5c;                   // +0x5c  position y
    float field_0x60;                   // +0x60  position z
    float field_0x90;                   // +0x90  direction/target vec x
    float field_0x94;                   // +0x94  direction/target vec y
    float field_0x98;                   // +0x98  direction/target vec z
    uint8_t field_0xf5;                 // +0xf5  visible/active flag
    int field_0x124;                    // +0x124 int position x
    int field_0x128;                    // +0x128 int position y
    int field_0x12c;                    // +0x12c int position z
    int field_0x150;                    // +0x150 timer
    int field_0x154;                    // +0x154 scale (12.4 fixed)
    uint8_t field_0x15c;                // +0x15c mission-lock flag

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
