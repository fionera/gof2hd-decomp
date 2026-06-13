#include "gof2/PlayerJunk.h"
#include "gof2/AEGeometry.h"
#include "gof2/FModSound.h"
#include "gof2/Level.h"
#include "gof2/ParticleSystemManager.h"
#include "gof2/KIPlayer.h"
// Player is dereferenced here only for getHitpoints()/setActive(); we declare a
// minimal local view of those two methods rather than pull in the full Player.h
// (whose byte-layout static_asserts assume the 32-bit ARM target, and which is
// owned by another translation unit's batch).
struct Player {
    int getHitpoints();
    void setActive(bool value);
};




// ---- reset_15e77c.cpp ----
// blx 0x74518
// b.w 0x1abe08 (veneer)

// PlayerJunk::reset() - reset base KIPlayer, clear state, tail-call the show/visible setter.
void _ZN10PlayerJunk5resetEv(PlayerJunk *self) {
    ((KIPlayer *)(self))->reset();
    self->state = 0;
    return ((PlayerJunk *)(self))->resetTail(1);
}

// ---- _PlayerJunk_15e76c.cpp ----
extern "C" void *KIPlayer_dtor(void *self);                 // blx 0x732b8 (returns this)
// b.w 0x1ab098 (veneer -> delete)

// PlayerJunk::~PlayerJunk() - destroy the base KIPlayer then tail-call the deleting veneer
// with the pointer the base dtor returns. Mangled so the symbol demangles to ~PlayerJunk.
void _ZN10PlayerJunkD0Ev(void *self) {
    return ((PlayerJunk *)(KIPlayer_dtor(self)))->dtorTail();
}

// ---- PlayerJunk_15e720.cpp ----
// KIPlayer::KIPlayer(this, type, -1, Player*, AEGeometry*, float, float, float, 0) -> this
// 0x75790

// Vtable base, accessed PC-relative; the live vtable pointer is (vtbl base) + 8.
__attribute__((visibility("hidden"))) extern int g_PlayerJunk_vtbl;

// PlayerJunk::PlayerJunk(int, Player*, AEGeometry*, float, float, float)
void _ZN10PlayerJunkC1EiP6PlayerP10AEGeometryfff(
        PlayerJunk *self, int p1, Player *p2, AEGeometry *p3, float p4, float p5, float p6) {
    // KIPlayer::KIPlayer returns the same object pointer (this), so write through self.
    self->initialized = 1;
    self->vtable = (void *)(g_PlayerJunk_vtbl + 8);
}

// ---- update_15e798.cpp ----
// 0x724f0
// 0x77908
// 0x71548
// AERandom::nextInt(rng, max) — bounded PRNG draw at 0x71848. Matches the codebase-wide
// AbyssEngine::AERandom::nextInt(rng, bound) call convention (AERandom used as a namespace).
namespace AbyssEngine { namespace AERandom { int nextInt(void *rng, int bound); } }
// 0x75904
// 0x72580
// 0x72034
// 0x72a30

__attribute__((visibility("hidden"))) extern void **g_PJ_sound;       // -> FModSound
__attribute__((visibility("hidden"))) extern void **g_PJ_random;      // -> AERandom

// PlayerJunk::update(int) - when destroyed, play the death sound, maybe drop a crate, detach
// from the player target, and emit the wreck particle burst.
void PlayerJunk::update(int elapsed) {
    PlayerJunk *self = this;
    Vector zero;

    self->lastUpdateTick = elapsed;
    if (((Player *)(self->player))->getHitpoints() < 1) {
        int state = self->state;
        if ((uint32_t)(state - 3) >= 2) {
            ((Level *)(self->level))->junkDied();
            self->state = 3;
            ((FModSound *)(*g_PJ_sound))->play(0x16, 0, 0, 0.0f);
            void **randHolder = g_PJ_random;
            if (AbyssEngine::AERandom::nextInt(*randHolder, 100) < 10) {
                self->droppedCrate = 1;
                Array<int> *arr = new Array<int>();
                self->crateContents = arr;
                ArrayAdd(99, *arr);
                ArrayAdd(AbyssEngine::AERandom::nextInt(*randHolder, 10) + 1, *self->crateContents);
                ((KIPlayer *)(self))->createCrate(3);
                self->droppedCrate = 1;
            } else {
                ((Player *)(self->player))->setActive(false);
                // Level's player object holds a target reference at [0x14]->[0x1c];
                // clear it if it points at this junk (opaque Level-internal layout).
                void *player = (void *)((Level *)(self->level))->getPlayer();
                void **targetSlot = (void **)((char *)*(void **)((char *)player + 0x14) + 0x1c);
                if (*targetSlot == self) {
                    player = (void *)((Level *)(self->level))->getPlayer();
                    targetSlot = (void **)((char *)*(void **)((char *)player + 0x14) + 0x1c);
                    *targetSlot = 0;
                }
            }
            void *levelObject = self->level;
            Vector position = self->emitPosition;
            zero.x = 0.0f;
            zero.y = 0.0f;
            zero.z = 0.0f;
            // Level holds the particle-system-manager handle at +0x74 and an emitter
            // Vector* at +0x34 (opaque Level-internal layout, not modeled here).
            int psManager = *(int *)((char *)levelObject + 0x74);
            Vector *emitter = *(Vector **)((char *)levelObject + 0x34);
            ((ParticleSystemManager *)(psManager))->emitManual((int)(intptr_t)emitter, (const float *)&position, 0, (float)(intptr_t)&zero);
        }
    }

    if (self->state == 3)
        self->state = 4;
}

// ---- render_15e8cc.cpp ----
// blx 0x72238
// b.w 0x1ac3a8 (veneer)

// PlayerJunk::render() - render the geometry if present, then (unless the state is 3 or 4)
// tail-call the base render veneer.
void _ZN10PlayerJunk6renderEv(PlayerJunk *self) {
    void *geom = self->geometry;
    if (geom != 0)
        ((AEGeometry *)(geom))->render();
    if ((uint32_t)(self->state - 3) > 1)
        return ((PlayerJunk *)(self))->renderTail();
}

// ---- reset/render/dtor tail fragments ----
// These are the veneers each of reset()/render()/~PlayerJunk() tail-calls. They simply
// complete the operation with the corresponding KIPlayer base behaviour.

// reset() tail: re-show the junk (the base visible/show setter, called with show==1).
void PlayerJunk::resetTail(int show) {
    ((KIPlayer *)this)->setVisible(show != 0);
}

// render() tail: render through the KIPlayer base.
void PlayerJunk::renderTail() {
    ((KIPlayer *)this)->render();
}

// deleting-destructor tail: the base subobject is already torn down, so just free.
void PlayerJunk::dtorTail() {
    ::operator delete(this);
}
