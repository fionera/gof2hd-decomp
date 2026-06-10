#ifndef GOF2_PLAYERJUNK_H
#define GOF2_PLAYERJUNK_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 - PlayerJunk class (a KIPlayer subclass: destructible space junk).
// Android libgof2hdaa.so, armv7 Thumb. Top-level class (no AbyssEngine namespace).
// Field offsets recovered per-method from the target disassembly; we do NOT model a full
// layout - methods access fields via byte-offset casts from `this`.
//
// Known field offsets:
//   0x00 vtable        0x04 int (player/owner id)   0x4c uint8 flag
//   0x50 Array<int>*   0x54 ParticleSystem owner    0x58/5c/60 Vector (emit pos)
//   0x78 int (geometry present)   0x88 int state     0x124 int (last update tick)


struct Player;
struct AEGeometry;



// AbyssEngine::AEMath::Vector - 3 floats. Passed by const ref / value.
namespace AbyssEngine { namespace AEMath {

} }
typedef AbyssEngine::AEMath::Vector Vector;

void *operator new(__SIZE_TYPE__);

// Field accessor via byte offset.

struct PlayerJunk {
    Player* field_0x4;                  // +0x4
    uint8_t field_0x4c;                 // +0x4c
    void* field_0x50;                   // +0x50
    void* field_0x54;                   // +0x54
    V3 field_0x58;                      // +0x58
    void* field_0x78;                   // +0x78
    int field_0x88;                     // +0x88
    int field_0x124;                    // +0x124
};
#endif
