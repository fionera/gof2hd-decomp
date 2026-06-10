#include "PlayerJunk.h"




// ---- reset_15e77c.cpp ----
extern "C" void KIPlayer_reset(void *self);                 // blx 0x74518
extern "C" void PlayerJunk_resetTail(void *self, int one);  // b.w 0x1abe08 (veneer)

// PlayerJunk::reset() - reset base KIPlayer, clear state, tail-call the show/visible setter.
extern "C" void _ZN10PlayerJunk5resetEv(void *self) {
    KIPlayer_reset(self);
    F<int>(self, 0x88) = 0;
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
        void *self, int p1, Player *p2, AEGeometry *p3, float p4, float p5, float p6) {
    void *base = KIPlayer_ctor(self, p1, -1, p2, p3, p4, p5, p6, 0);
    F<uint8_t>(base, 0x3d) = 1;
    F<int>(base, 0x0) = g_PlayerJunk_vtbl + 8;
}

// ---- update_15e798.cpp ----
extern "C" int Player_getHitpoints(Player *self);                      // 0x724f0
extern "C" int Level_junkDied(void *level);                            // 0x77908
extern "C" void FModSound_play(void *self, int id, Vector *a, Vector *b, float volume); // 0x71548
extern "C" int AERandom_nextInt(void *self, int max);                  // 0x71848
extern "C" void Array_int_ctor(void *array);                          // 0x701f8
extern "C" void ArrayAdd_int(int value, void *array);                 // 0x7021c
extern "C" void KIPlayer_createCrate(void *self, int kind);          // 0x75904
extern "C" void Player_setActive(Player *self, bool active);          // 0x72580
extern "C" void *Level_getPlayer(void *level);                        // 0x72034
extern "C" void ParticleSystemManager_emitManual(int manager, Vector *emitter, Vector *position,
                                                 int unused, Vector *zero, float value); // 0x72a30

__attribute__((visibility("hidden"))) extern void **g_PJ_sound;       // -> FModSound
__attribute__((visibility("hidden"))) extern void **g_PJ_random;      // -> AERandom

// PlayerJunk::update(int) - when destroyed, play the death sound, maybe drop a crate, detach
// from the player target, and emit the wreck particle burst.
void PlayerJunk_update(void *self, int elapsed) {
    Vector zero;
    char workBytes[12];

    F<int>(self, 0x124) = elapsed;
    if (Player_getHitpoints(F<Player *>(self, 0x4)) < 1) {
        int state = F<int>(self, 0x88);
        if ((uint32_t)(state - 3) >= 2) {
            Level_junkDied(F<void *>(self, 0x54));
            F<int>(self, 0x88) = 3;
            FModSound_play(*g_PJ_sound, 0x16, 0, 0, 0.0f);
            void **randHolder = g_PJ_random;
            if (AERandom_nextInt(*randHolder, 100) < 10) {
                F<uint8_t>(self, 0x4c) = 1;
                void *arr = operator new(0xc);
                Array_int_ctor(arr);
                F<void *>(self, 0x50) = arr;
                ArrayAdd_int(99, arr);
                ArrayAdd_int(AERandom_nextInt(*randHolder, 10) + 1, F<void *>(self, 0x50));
                KIPlayer_createCrate(self, 3);
                F<uint8_t>(self, 0x4c) = 1;
            } else {
                Player_setActive(F<Player *>(self, 0x4), false);
                void *player = Level_getPlayer(F<void *>(self, 0x54));
                if (F<void *>(F<void *>(player, 0x14), 0x1c) == self) {
                    player = Level_getPlayer(F<void *>(self, 0x54));
                    F<void *>(F<void *>(player, 0x14), 0x1c) = 0;
                }
            }
            void *levelObject = F<void *>(self, 0x54);
            struct V3 { uint32_t x, y, z; };
            V3 position = F<V3>(self, 0x58);
            zero.x = 0.0f;
            zero.y = 0.0f;
            zero.z = 0.0f;
            ParticleSystemManager_emitManual(F<int>(levelObject, 0x74),
                                             F<Vector *>(levelObject, 0x34),
                                             (Vector *)&position, 0, &zero, -1.0f);
        }
    }

    if (F<int>(self, 0x88) == 3)
        F<int>(self, 0x88) = 4;
}

// ---- render_15e8cc.cpp ----
extern "C" void AEGeometry_render(void *geom);   // blx 0x72238
extern "C" void PlayerJunk_renderTail(void *self);  // b.w 0x1ac3a8 (veneer)

// PlayerJunk::render() - render the geometry if present, then (unless the state is 3 or 4)
// tail-call the base render veneer.
extern "C" void _ZN10PlayerJunk6renderEv(void *self) {
    void *geom = F<void *>(self, 0x78);
    if (geom != 0)
        AEGeometry_render(geom);
    if ((uint32_t)(F<int>(self, 0x88) - 3) > 1)
        return PlayerJunk_renderTail(self);
}
