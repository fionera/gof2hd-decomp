#include "game/ship/PlayerJunk.h"
#include "engine/render/AEGeometry.h"
#include "engine/audio/FModSound.h"
#include "game/world/Level.h"
#include "engine/render/ParticleSystemManager.h"

// Player is dereferenced here only for getHitpoints()/setActive(); we declare a
// minimal local view of those two methods rather than pull in the full Player.h
// (whose byte-layout static_asserts assume the 32-bit ARM target, owned by
// another translation unit's batch).
class Player {
public:
    int  getHitpoints();
    void setActive(bool value);
};

// Death sound source and crate-drop RNG, owned by the engine.
extern FModSound* g_PJ_sound;
namespace AbyssEngine { namespace AERandom { int nextInt(void* rng, int bound); } }
extern void* g_PJ_random;

// PlayerJunk(type, player, geometry, x, y, z): a junk actor with no AI route and
// inactive on spawn.
PlayerJunk::PlayerJunk(int type, Player* player, AEGeometry* geometry,
                       float x, float y, float z)
    : KIPlayer(type, -1, player, geometry, x, y, z, false) {
}

PlayerJunk::~PlayerJunk() {
}

// reset(): reset the KIPlayer base, clear our state, then re-show the junk.
void PlayerJunk::reset() {
    KIPlayer::reset();
    this->state = 0;
    this->setVisible(true);
}

// collide / outerCollide: junk exposes no collidable surface, so both report a miss.
int PlayerJunk::collide(float x, float y, float z) {
    return 0;
}

int PlayerJunk::outerCollide(float x, float y, float z) {
    return 0;
}

// render(): draw the crate geometry if present, then (unless dying) render the base.
void PlayerJunk::render() {
    if (this->crateGeometry != nullptr)
        this->crateGeometry->render();
    if ((uint32_t)(this->state - 3) > 1)
        KIPlayer::render();
}

// update(elapsed): when the underlying Player is destroyed, play the death sound,
// maybe drop a crate, detach from the player target, and emit the wreck particle
// burst.
void PlayerJunk::update(int elapsed) {
    this->field_0x120 = elapsed;
    if (this->player->getHitpoints() < 1) {
        if ((uint32_t)(this->state - 3) >= 2) {
            this->level->junkDied();
            this->state = 3;
            g_PJ_sound->play(0x16, nullptr, nullptr, 0.0f);
            if (AbyssEngine::AERandom::nextInt(g_PJ_random, 100) < 10) {
                this->hasCargo = 1;
                this->cargo = new Array<int>();
                ArrayAdd(99, *this->cargo);
                ArrayAdd(AbyssEngine::AERandom::nextInt(g_PJ_random, 10) + 1, *this->cargo);
                this->createCrate(3);
            } else {
                this->player->setActive(false);
                // The Level's player object holds a target reference at
                // [0x14]->[0x1c]; clear it if it points at this junk (opaque
                // Level-internal layout, not modelled here).
                void* playerObj = (void*)this->level->getPlayer();
                void** targetSlot = (void**)((char*)*(void**)((char*)playerObj + 0x14) + 0x1c);
                if (*targetSlot == this)
                    *targetSlot = nullptr;
            }

            Vector position;
            position.x = this->posX;
            position.y = this->posY;
            position.z = this->posZ;
            Vector zero;
            zero.x = 0.0f;
            zero.y = 0.0f;
            zero.z = 0.0f;
            // The Level holds the particle-system-manager handle at +0x74 and an
            // emitter Vector* at +0x34 (opaque Level-internal layout).
            Level* level = this->level;
            int psManager = *(int*)((char*)level + 0x74);
            Vector* emitter = *(Vector**)((char*)level + 0x34);
            ((ParticleSystemManager*)psManager)->emitManual(
                (int)(intptr_t)emitter, position, 0, zero, 0.0f);
        }
    }

    if (this->state == 3)
        this->state = 4;
}
