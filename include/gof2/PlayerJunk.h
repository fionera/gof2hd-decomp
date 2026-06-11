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

// PlayerJunk is a KIPlayer subclass; we model only the fields the methods touch.
// Field offsets are recovered per-method from the target; this is not a full layout.
struct PlayerJunk {
    void* vtable;                    // +0x0   vtable pointer
    Player* player;                  // +0x4   player/owner this junk targets
    uint8_t initialized;                 // +0x3d  init flag (set in ctor)
    uint8_t droppedCrate;                 // +0x4c  "dropped a crate" flag
    Array<int>* crateContents;             // +0x50  crate contents (item id + amount)
    void* level;                   // +0x54  Level owner
    Vector emitPosition;                  // +0x58  emit position (3 floats)
    void* geometry;                   // +0x78  AEGeometry
    int state;                     // +0x88  state machine
    int lastUpdateTick;                    // +0x124 last update tick

    // ---- methods (converted from free functions) ----
    void update(int elapsed);
};
#endif
