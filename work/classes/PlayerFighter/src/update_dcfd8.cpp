#include "class.h"

extern "C" void *__stack_chk_guard;
extern "C" void __stack_chk_fail(unsigned diff);

extern "C" int Explosion_isPlaying(int e);
extern "C" void AEGeometry_getPosition(void *out, int geom);
extern "C" void AEMath_VectorAssign(void *dst, void *src);
extern "C" int KIPlayer_isWingMan(PlayerFighter *self);
extern "C" int Status_getStanding();
extern "C" int Standing_isEnemy(int standing);
// Tail veneer taken on the dead+exploded early-out path (-> 0x1abe18).
extern "C" void PF_update_dead(PlayerFighter *self);
// The remaining per-frame update: AI steering, weapons, cloaking, trail, camera, route
// following. A ~10KB state machine; delegated to a helper so this translation keeps the
// recoverable entry flow (early-out, timers, position, enemy flag) while staying tractable.
extern "C" void PF_update_body(PlayerFighter *self, int dt);

extern void *const gUpd_guard __attribute__((visibility("hidden")));   // DAT_000ed348

// PlayerFighter::update(int dt) — self in r0, dt in r1.
extern "C" void PlayerFighter_update(PlayerFighter *self, int dt)
{
    int *guardP = *(int **)gUpd_guard;
    volatile int saved = *guardP;

    // Dead-and-explosion-finished early-out: tear down via the dead veneer.
    if (F<int32_t>(self, 0x88) == 4 && Explosion_isPlaying(F<int32_t>(self, 0x124)) == 0 &&
        (F<signed char>(self, 0x4c) == 0 || 60000 < F<int32_t>(self, 0xd8))) {
        if (*guardP != saved) {
            __stack_chk_fail((unsigned)(*guardP - saved));
        }
        PF_update_dead(self);
        return;
    }

    // Advance per-frame timers.
    F<int32_t>(self, 0x1c0) += dt;
    F<int32_t>(self, 0x1b8) += dt;
    if (F<int32_t>(self, 0x28) == 1) {
        F<int32_t>(self, 0x28) = 1;
    }
    F<int32_t>(self, 0x1d0) = dt;
    F<int32_t>(self, 0x1d4) = dt >> 31;

    // Sync world position from the geometry.
    float pos[3];
    AEGeometry_getPosition(pos, F<int32_t>(self, 8));
    AEMath_VectorAssign((char *)self + 0x2c, pos);

    // Recompute the "is enemy" flag unless the ship is in a non-combat mode.
    if (F<signed char>(self, 0x43) == 0) {
        unsigned char enemy;
        if ((F<uint32_t>(self, 0x28) & 0xfffffffe) == 8) {
            enemy = 1;
        } else if (KIPlayer_isWingMan(self) != 0) {
            enemy = 0;
        } else {
            enemy = (unsigned char)Standing_isEnemy(Status_getStanding());
        }
        *(unsigned char *)(F<int32_t>(self, 4) + 0x5c) = enemy;
    }

    // Remaining per-frame state machine.
    PF_update_body(self, dt);

    if (*guardP == saved) {
        return;
    }
    __stack_chk_fail((unsigned)(*guardP - saved));
}
