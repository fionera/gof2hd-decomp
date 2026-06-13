#ifndef GOF2_SENTRYGUN_H
#define GOF2_SENTRYGUN_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 — SentryGun. Top-level, no namespace. Subclass of ObjectGun.
// Field offsets recovered from per-method target disassembly.
//
// Known fields (relative to SentryGun*):
//   +0x00  vtable
//   +0x08  Gun*  gun     (the ObjectGun-owned weapon, set by base ctor)
//   +0x0c  Level* level
//   +0xb0  int   cooldown  (init from gun->0x58 * 3 - 0x279)

struct SentryGun;
struct Gun;
struct Level;

// Field accessor via byte offset.

class SentryGun {
public:
    Gun* gun;                     // +0x8
    void* level;                    // +0xc
    int cooldown;                     // +0xb0

    // ---- methods (converted from free functions) ----
    SentryGun * ctor(Gun *gun, int p2, int p3, int p4, Level *level);
    void update(int dt);

    // Tail-dispatch reached at the end of update()/setDead()/reset(): the spawned
    // pool object is handed off (with flag 1) to a dynamically-imported activation
    // routine. That target is a load-time-resolved PLT veneer with no static body,
    // so this member reproduces the only statically knowable behaviour: forwarding
    // the (object, flag) tail-call to that resolved import.
    static void fire_tail(void *obj, int flag);
};
#endif
