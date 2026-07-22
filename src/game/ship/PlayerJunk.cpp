#include "game/ship/PlayerJunk.h"
#include "engine/core/AERandom.h"
#include "game/ship/Player.h"
#include "game/ship/PlayerEgo.h"
#include "game/weapons/Radar.h"
#include "engine/render/AEGeometry.h"
#include "engine/audio/FModSound.h"
#include "game/world/Level.h"
#include "game/core/Globals.h"
#include "engine/render/ParticleSystemManager.h"

static FModSound **g_PJ_sound = &Globals::sound;

namespace AbyssEngine {
}

static AbyssEngine::AERandom **g_PJ_random = &Globals::rnd;

PlayerJunk::PlayerJunk(int type, Player *player, AEGeometry *geometry,
                       float x, float y, float z)
    : KIPlayer(type, -1, player, geometry, x, y, z, false) {
    this->countsAsEnemyExcludeFlag = 1;
}

PlayerJunk::~PlayerJunk() {
}

void PlayerJunk::reset() {
    KIPlayer::reset();
    this->state = 0;
    this->setVisible(true);
}

int PlayerJunk::collide(float x, float y, float z) {
    return 0;
}

int PlayerJunk::outerCollide(float x, float y, float z) {
    return 0;
}

void PlayerJunk::render() {
    if (this->crateGeometry != nullptr)
        this->crateGeometry->render();
    if ((uint32_t)(this->state - 3) > 1)
        KIPlayer::render();
}

void PlayerJunk::update(int elapsed) {
    this->field_0x124 = elapsed;
    if (this->player->getHitpoints() < 1) {
        if ((uint32_t)(this->state - 3) >= 2) {
            this->level->junkDied();
            this->state = 3;
            (*g_PJ_sound)->play(0x16, nullptr, nullptr, 0.0f);
            if ((*g_PJ_random)->nextInt(100) < 10) {
                this->hasCargo = 1;
                this->cargo = new Array<int>();
                ArrayAdd(99, *this->cargo);
                ArrayAdd((*g_PJ_random)->nextInt(10) + 1, *this->cargo);
                this->createCrate(3);
                this->hasCargo = 1;
            } else {
                this->player->setActive(false);

                if (((Radar *) this->level->getPlayer()->field_0x14)->field_0x1c == this)
                    ((Radar *) this->level->getPlayer()->field_0x14)->field_0x1c = nullptr;
            }

            Vector position;
            position.x = this->posX;
            position.y = this->posY;
            position.z = this->posZ;
            Vector zero;
            zero.x = 0.0f;
            zero.y = 0.0f;
            zero.z = 0.0f;

            Level *level = this->level;
            level->field_74->emitManual(
                level->field_34, position, 0, zero, -1.0f);
        }
    }

    if (this->state == 3)
        this->state = 4;
}
