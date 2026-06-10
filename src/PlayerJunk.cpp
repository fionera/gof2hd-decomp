#include "gof2/PlayerJunk.h"
// Player is used only as an opaque pointer here (all access goes through the
// extern "C" accessors below), so we forward-declare it rather than pull in
// Player.h (whose byte-layout static_asserts assume the 32-bit ARM target).
struct Player;




// ---- reset_15e77c.cpp ----
extern "C" void KIPlayer_reset(void *self);                 // blx 0x74518
extern "C" void PlayerJunk_resetTail(void *self, int one);  // b.w 0x1abe08 (veneer)

// PlayerJunk::reset() - reset base KIPlayer, clear state, tail-call the show/visible setter.
extern "C" void _ZN10PlayerJunk5resetEv(PlayerJunk *self) {
    KIPlayer_reset(self);
    self->field_0x88 = 0;
    return PlayerJunk_resetTail(self, 1);
}

// ---- _PlayerJunk_15e76c.cpp ----
extern "C" void *KIPlayer_dtor(void *self);                 // blx 0x732b8 (returns this)
extern "C" void PlayerJunk_dtorTail(void *self);            // b.w 0x1ab098 (veneer -> delete)

// PlayerJunk::~PlayerJunk() - destroy the base KIPlayer then tail-call the deleting veneer
// with the pointer the base dtor returns. Mangled so the symbol demangles to ~PlayerJunk.
extern "C" void _ZN10PlayerJunkD0Ev(void *self) {
    return PlayerJunk_dtorTail(KIPlayer_dtor(self));
}

// ---- PlayerJunk_15e720.cpp ----
// KIPlayer::KIPlayer(this, type, -1, Player*, AEGeometry*, float, float, float, 0) -> this
extern "C" void *KIPlayer_ctor(void *self, int type, int minusOne, Player *owner,
                               AEGeometry *geom, float a, float b, float c, int zero); // 0x75790

// Vtable base, accessed PC-relative; the live vtable pointer is (vtbl base) + 8.
__attribute__((visibility("hidden"))) extern int g_PlayerJunk_vtbl;

// PlayerJunk::PlayerJunk(int, Player*, AEGeometry*, float, float, float)
extern "C" void _ZN10PlayerJunkC1EiP6PlayerP10AEGeometryfff(
        PlayerJunk *self, int p1, Player *p2, AEGeometry *p3, float p4, float p5, float p6) {
    // KIPlayer::KIPlayer returns the same object pointer (this), so write through self.
    KIPlayer_ctor(self, p1, -1, p2, p3, p4, p5, p6, 0);
    self->field_0x3d = 1;
    self->field_0x0 = (void *)(g_PlayerJunk_vtbl + 8);
}

// ---- update_15e798.cpp ----
extern "C" int Player_getHitpoints(Player *self);                      // 0x724f0
extern "C" int Level_junkDied(void *level);                            // 0x77908
extern "C" void FModSound_play(void *self, int id, Vector *a, Vector *b, float volume); // 0x71548
extern "C" int AERandom_nextInt(void *self, int max);                  // 0x71848
extern "C" void KIPlayer_createCrate(void *self, int kind);          // 0x75904
extern "C" void Player_setActive(Player *self, bool active);          // 0x72580
extern "C" void *Level_getPlayer(void *level);                        // 0x72034
extern "C" void ParticleSystemManager_emitManual(int manager, Vector *emitter, Vector *position,
                                                 int unused, Vector *zero, float value); // 0x72a30

__attribute__((visibility("hidden"))) extern void **g_PJ_sound;       // -> FModSound
__attribute__((visibility("hidden"))) extern void **g_PJ_random;      // -> AERandom

// PlayerJunk::update(int) - when destroyed, play the death sound, maybe drop a crate, detach
// from the player target, and emit the wreck particle burst.
void PlayerJunk_update(PlayerJunk *self, int elapsed) {
    Vector zero;

    self->field_0x124 = elapsed;
    if (Player_getHitpoints(self->field_0x4) < 1) {
        int state = self->field_0x88;
        if ((uint32_t)(state - 3) >= 2) {
            Level_junkDied(self->field_0x54);
            self->field_0x88 = 3;
            FModSound_play(*g_PJ_sound, 0x16, 0, 0, 0.0f);
            void **randHolder = g_PJ_random;
            if (AERandom_nextInt(*randHolder, 100) < 10) {
                self->field_0x4c = 1;
                Array<int> *arr = new Array<int>();
                self->field_0x50 = arr;
                ArrayAdd(99, *arr);
                ArrayAdd(AERandom_nextInt(*randHolder, 10) + 1, *self->field_0x50);
                KIPlayer_createCrate(self, 3);
                self->field_0x4c = 1;
            } else {
                Player_setActive(self->field_0x4, false);
                // Level's player object holds a target reference at [0x14]->[0x1c];
                // clear it if it points at this junk (opaque Level-internal layout).
                void *player = Level_getPlayer(self->field_0x54);
                void **targetSlot = (void **)((char *)*(void **)((char *)player + 0x14) + 0x1c);
                if (*targetSlot == self) {
                    player = Level_getPlayer(self->field_0x54);
                    targetSlot = (void **)((char *)*(void **)((char *)player + 0x14) + 0x1c);
                    *targetSlot = 0;
                }
            }
            void *levelObject = self->field_0x54;
            Vector position = self->field_0x58;
            zero.x = 0.0f;
            zero.y = 0.0f;
            zero.z = 0.0f;
            // Level holds the particle-system-manager handle at +0x74 and an emitter
            // Vector* at +0x34 (opaque Level-internal layout, not modeled here).
            int psManager = *(int *)((char *)levelObject + 0x74);
            Vector *emitter = *(Vector **)((char *)levelObject + 0x34);
            ParticleSystemManager_emitManual(psManager, emitter,
                                             &position, 0, &zero, -1.0f);
        }
    }

    if (self->field_0x88 == 3)
        self->field_0x88 = 4;
}

// ---- render_15e8cc.cpp ----
extern "C" void AEGeometry_render(void *geom);   // blx 0x72238
extern "C" void PlayerJunk_renderTail(void *self);  // b.w 0x1ac3a8 (veneer)

// PlayerJunk::render() - render the geometry if present, then (unless the state is 3 or 4)
// tail-call the base render veneer.
extern "C" void _ZN10PlayerJunk6renderEv(PlayerJunk *self) {
    void *geom = self->field_0x78;
    if (geom != 0)
        AEGeometry_render(geom);
    if ((uint32_t)(self->field_0x88 - 3) > 1)
        return PlayerJunk_renderTail(self);
}
