#include "game/ship/PlayerCreature.h"
#include "engine/core/AERandom.h"
#include "engine/render/AEGeometry.h"
#include "engine/audio/FModSound.h"
#include "game/world/Level.h"
#include "engine/render/ParticleSystemManager.h"
#include "game/ship/KIPlayer.h"
#include "game/ship/Player.h"
#include "game/ship/PlayerJunk.h"
#include "game/ship/PlayerEgo.h"
#include "game/weapons/Radar.h"

#include "game/core/Globals.h"
#include "engine/math/AEMath.h"

// Per creature type: weight, rage modifier and endurance modifier share one table.
static const int kCreatureTypeFactor[4] = {1, 1, 1, 0};

PlayerCreature::PlayerCreature(int kind, int itemIndex, Player *player, AEGeometry *geometry,
                               float x, float y, float z)
    : KIPlayer(kind, -1, player, geometry, x, y, z, false) {
    this->caught = false;
    this->rageScale = 1.0f;
    this->itemIndex = itemIndex;
    calmDown();
    this->lastHitpoints = this->player->getHitpoints();
    this->maxEndurance = this->endurance =
            (int) ((float) kCreatureTypeFactor[kind] / 10.0f * 8000.0f) + 8000;
    reset();
}

void PlayerCreature::calmDown() {
    this->rageTimer = 0;
    this->raging = false;
    this->hooked = false;
    this->endurance = this->maxEndurance;
}

void PlayerCreature::reset() {
    KIPlayer::reset();
    this->state = 0;
    this->setActive(true);
    this->endurance = this->maxEndurance;
    this->lastHitpoints = this->player->getHitpoints();
    this->rageMatrix = AbyssEngine::AEMath::Matrix();
}

PlayerCreature::~PlayerCreature() {
}

void PlayerCreature::rage(int amount) {
    this->rageTimer = 0;
    this->raging = true;
    this->hooked = true;
    this->rageScale = ((float) amount / -100.0f + 1.0f) * ((float) kCreatureTypeFactor[this->creatureType()] + 1.0f);
}

uint8_t PlayerCreature::isHooked() {
    return this->hooked;
}

uint8_t PlayerCreature::isCaught() {
    return this->caught;
}

void PlayerCreature::hook(int value) {
    this->hooked = true;
    rage(value);
}

void PlayerCreature::unhook() {
    this->caught = false;
    this->raging = false;
    this->rageTimer = 0;
    this->hooked = false;
    this->endurance = this->maxEndurance;
}

void PlayerCreature::update(int elapsed) {
    this->lastElapsed = elapsed;
    if (this->lastHitpoints > this->player->getHitpoints() && this->state != 4)
        this->raging = true;
    this->lastHitpoints = this->player->getHitpoints();

    if (this->raging) {
        this->rageTimer += elapsed;
        this->endurance -= elapsed;
        this->rageScale += (float) elapsed * 0.0007f;
        elapsed = (int) (this->rageScale * (float) elapsed);
        if (this->rageTimer < 4000) {
            this->geometry->setMatrix(AbyssEngine::AEMath::MatrixMultiply(this->geometry->getMatrix(), this->rageMatrix));
        } else {
            // Pick a new random wobble.
            int half = elapsed >> 1;
            float offset = (float) half * -0.5f;
            float xAngle = (offset + (float) Globals::rnd->nextInt(half)) * (1.0f / 4096.0f);
            float yAngle = (offset + (float) Globals::rnd->nextInt(half)) * (1.0f / 4096.0f);
            AbyssEngine::AEMath::MatrixSetRotation(this->rageMatrix, (xAngle + xAngle) * 3.1415927f,
                                                   (yAngle + yAngle) * 3.1415927f, 0.0f);
            this->rageTimer = 0;
        }
        if (this->endurance < 1) {
            if (this->hooked) {
                this->caught = true;
                this->raging = false;
            } else {
                calmDown();
            }
        }
    }

    if (this->player->getHitpoints() < 1 && (uint32_t) (this->state - 3) >= 2) {
        this->state = 3;
        Globals::sound->play(22, nullptr, nullptr, 0.0f);
        this->player->setActive(false);
        if (this->level->getPlayer()->field_0x14->field_0x1c == this)
            this->level->getPlayer()->field_0x14->field_0x1c = nullptr;
        this->level->field_74->emitManual(this->level->field_34, Vector{this->posX, this->posY, this->posZ}, 0,
                                          Vector{0.0f, 0.0f, 0.0f}, -1.0f);
    }

    if (this->state != 3) {
        if (this->state == 0 && !this->caught)
            this->geometry->moveForward((float) elapsed);
    } else {
        this->state = 4;
    }

    this->player->transform = this->geometry->getMatrix();
}

void PlayerCreature::render() {
    if (this->renderGeometry() != nullptr) {
        this->renderGeometry()->render();
    }

    if ((uint32_t)(this->state - 3) >= 2) {
        this->KIPlayer::render();
    }
}

int PlayerCreature::collide(float /*x*/, float /*y*/, float /*z*/) {
    return 0;
}

int PlayerCreature::outerCollide(float /*x*/, float /*y*/, float /*z*/) {
    return 0;
}

int PlayerCreature::getWeight() {
    return kCreatureTypeFactor[this->creatureType()];
}

int PlayerCreature::getItemIndex() {
    return this->itemIndex;
}

int PlayerCreature::getMaxEndurance() {
    return this->maxEndurance;
}

int PlayerCreature::getEndurance() {
    return this->endurance;
}
