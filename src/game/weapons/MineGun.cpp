#include "game/weapons/MineGun.h"
#include "engine/core/ArrayReleaseClasses.h"
#include "game/core/Globals.h"
#include "game/weapons/ObjectGun.h"
#include "game/weapons/Gun.h"
#include "game/ship/TargetFollowCamera.h"
#include "game/ship/PlayerEgo.h"
#include "game/mission/Explosion.h"
#include "engine/render/PaintCanvas.h"
#include "engine/render/AEGeometry.h"
#include "engine/math/AEMath.h"
#include "engine/math/Transform.h"

MineGun::MineGun(Gun *gun, int mesh, int param, int unused, Level *level)
    : ObjectGun(param, gun, mesh, 0, level) {
    this->field_0xc0 = 0;
    this->field_0xc4 = 0;
    this->field_0xc8 = 0;

    this->explosions = new Array<Explosion *>();
    ArraySetLength(gun->positions.count, *(this->explosions));

    this->readyFlags = new uint8_t[this->explosions->size()];
    for (uint32_t i = 0; i < this->explosions->size(); ++i) {
        (*this->explosions)[i] = new Explosion((gun->damage == 0) ? 7 : 0);
        (*this->explosions)[i]->setWeaponIndex(gun->itemIndex);
        this->readyFlags[i] = 1;
    }

    this->geometry = new AEGeometry((uint16_t) (mesh + 1), Globals::Canvas, false);
    Globals::Canvas->TransformAddChild(this->transform, this->geometry->transform);
    ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(this->geometry->transform))
            ->SetAnimationState((AbyssEngine::AnimationMode) 2, 0);
}

MineGun::~MineGun() {
    if (this->explosions != nullptr) {
        ArrayReleaseClasses(*this->explosions);
        delete this->explosions;
        this->explosions = nullptr;
    }

    delete[] this->readyFlags;
    this->readyFlags = nullptr;

    delete this->geometry;
    this->geometry = nullptr;
}

void MineGun::setPlayer(PlayerEgo *player) {
    this->player = player;
}

void MineGun::render() {
    ObjectGun::render();
    for (uint32_t i = 0; i < this->gun->positions.count; ++i) {
        if (this->gun->hitFlags[i] != 0) {
            (*this->explosions)[i]->render();
        }
    }
}

void MineGun::update(int delta) {
    ObjectGun::update(delta);
    if (this->gun->active)
        ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(this->geometry->transform))
                ->Update((long long) delta, false);

    for (uint32_t i = 0; i < this->gun->positions.count; ++i) {
        if (!this->gun->hitFlags[i])
            continue;
        if (this->readyFlags[i]) {
            // Detonation: shake the camera depending on the distance to the player.
            this->rumbleTimer = 0;
            float distance = AbyssEngine::AEMath::VectorLength(minePosition(i) - this->player->getPosition());
            this->rumblePercentage = 1.0f - (distance < 30000.0f ? distance : 30000.0f) / 30000.0f;
            this->player->getTargetFollowCamera()->setRumblePercentage(this->rumblePercentage, 50);
            (*this->explosions)[i]->start(minePosition(i), Vector{0.0f, 0.0f, 0.0f});
            this->readyFlags[i] = 0;
        }
        (*this->explosions)[i]->update(delta, (TargetFollowCamera *) nullptr);
        this->rumbleTimer = this->rumbleTimer + delta >= 2000 ? 2000 : this->rumbleTimer + delta;
        this->player->getTargetFollowCamera()->setRumblePercentage(
                this->rumblePercentage * ((float) this->rumbleTimer / -2000.0f + 1.0f), 50);
        if (!(*this->explosions)[i]->isPlaying()) {
            this->player->getTargetFollowCamera()->setRumblePercentage(0.0f, 0);
            Gun *gun = this->gun;
            this->rumbleTimer = 0;
            gun->hitFlags[i] = 0;
            gun->ignited = 0;
            this->readyFlags[i] = 1;
            (*this->explosions)[i]->reset();
        }
    }
}

int MineGun::isMineGun() {
    return 1;
}
