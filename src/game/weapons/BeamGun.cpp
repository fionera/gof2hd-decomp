#include "game/weapons/BeamGun.h"
#include "game/core/Globals.h"
#include "game/world/Level.h"
#include "game/world/LevelScript.h"
#include "engine/math/Transform.h"
#include "game/weapons/Gun.h"
#include "game/ship/Player.h"
#include "game/ship/PlayerEgo.h"
#include "engine/render/AEGeometry.h"

using ::AbyssEngine::Transform;

#include "engine/math/AEMath.h"

using AbyssEngine::AEMath::operator+;
using AbyssEngine::AEMath::operator*;
using AbyssEngine::AEMath::operator!=;

// Muzzle-effect mesh shown with the beam of each player weapon item (-1 = none).
static const int kSecondaryMeshes[233] = {
    14500, 14500, 14500, 14501, 14501, 14502, 14502, 14503, 14503, 14500, 14503, 14502,
    14501, 14501, 14501, 14501, 14504, 14504, 14504, 14505, 14505, 14505, 14506, 14506,
    14507, 14507, 14508, 14508, 14506, 14505, 14502, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 14503,
    14505, 14501, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, 14506, 14503, 14502, -1,
    14509, 14510, 14506, 14503, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, 14500, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, 14505, -1, -1, -1,
    14501, 14501, 14508, 14502, -1,
};

BeamGun::BeamGun(int owner, Gun *gun, int meshKind, Level *level) {
    int kind = meshKind - 9;
    if (meshKind == 0xe4)
        kind = 2;
    this->field_0x4 = 0;
    this->gun = gun;
    this->level = level;
    this->owner = owner;
    this->meshKind = kind;
    uint16_t primaryMesh = (uint16_t) (kind + 0x3795);
    if (meshKind == 0xe4)
        primaryMesh = 0x4a92;
    this->primaryGeometry = new AEGeometry(primaryMesh, Globals::Canvas, false);

    AEGeometry *secondary = nullptr;
    int mesh;
    if (!gun->isPlayerGun() || (mesh = kSecondaryMeshes[gun->itemIndex]) < 0) {
        this->hasSecondary = 0;
    } else {
        this->hasSecondary = gun->weaponType != ITEM_SORT_MINE;
        if (this->hasSecondary)
            secondary = new AEGeometry((uint16_t) mesh, Globals::Canvas, false);
    }
    this->secondaryVisible = 0;
    this->secondaryGeometry = secondary;
}

BeamGun::~BeamGun() {
    if (this->primaryGeometry != nullptr)
        delete this->primaryGeometry;
    this->primaryGeometry = nullptr;

    if (this->secondaryGeometry != nullptr)
        delete this->secondaryGeometry;
    this->secondaryGeometry = nullptr;
}

void BeamGun::render() {
    if (this->gun->active == 0)
        return;

    this->primaryGeometry->render();

    if (this->secondaryVisible != 0 && this->secondaryGeometry != nullptr)
        this->secondaryGeometry->render();
}

void BeamGun::update(int elapsed) {
    this->gun->update(elapsed);
    if (!this->gun->active) {
        this->primaryGeometry->setVisible(false);
        return;
    }
    if (this->gun->hitSmall) {
        // Restart the beam animation.
        ((Transform *) Globals::Canvas->TransformGetTransform(this->primaryGeometry->transform))
                ->SetAnimationState((AbyssEngine::AnimationMode) 3, nullptr);
        ((Transform *) Globals::Canvas->TransformGetTransform(this->primaryGeometry->transform))
                ->SetAnimationState((AbyssEngine::AnimationMode) 1, nullptr);
        this->gun->hitSmall = 0;
    }
    ((Transform *) Globals::Canvas->TransformGetTransform(this->primaryGeometry->transform))
            ->Update((long long) elapsed, false);

    // The beam is stretched along its length to the current range.
    this->primaryGeometry->setScaling(1.0f, 1.0f, (float) this->gun->field_0x8c);
    this->primaryGeometry->setDirection(*(Vector *) &this->gun->field_0x90, Vector{0.0f, 1.0f, 0.0f});

    Vector position = this->level->getPlayer()->getPosition();
    if (this->gun->offset != Vector{0.0f, 0.0f, 0.0f}) {
        Matrix rotation = this->level->getPlayer()->geometry->getMatrix() *
                          this->level->getPlayer()->field_0x4->getMatrix();
        position += AbyssEngine::AEMath::MatrixRotateVector(rotation, this->gun->offset + Vector{0.0f, 0.0f, -100.0f});
    }
    this->primaryGeometry->setPosition(position);
    this->primaryGeometry->setVisible(true);

    if (this->hasSecondary) {
        if (this->gun->delayActive) {
            PlayerEgo *player = this->level->getPlayer();
            Vector muzzle = player->getPosition();
            Vector offset = this->gun->offset;
            offset.z += -100.0f;
            muzzle += AbyssEngine::AEMath::MatrixRotateVector(player->player->transform, offset);
            this->secondaryGeometry->setPosition(muzzle);
            ((Transform *) Globals::Canvas->TransformGetTransform(this->secondaryGeometry->transform))
                    ->Update((long long) elapsed, false);
            this->secondaryGeometry->setDirection(AbyssEngine::AEMath::MatrixGetDir(player->player->transform),
                                                  Vector{0.0f, 1.0f, 0.0f});
        } else {
            ((Transform *) Globals::Canvas->TransformGetTransform(this->secondaryGeometry->transform))
                    ->SetAnimationState((AbyssEngine::AnimationMode) 0, nullptr);
            ((Transform *) Globals::Canvas->TransformGetTransform(this->secondaryGeometry->transform))
                    ->SetAnimationState((AbyssEngine::AnimationMode) 3, nullptr);
            ((Transform *) Globals::Canvas->TransformGetTransform(this->secondaryGeometry->transform))
                    ->SetAnimationState((AbyssEngine::AnimationMode) 1, nullptr);
        }
    }
    this->secondaryVisible = this->gun->delayActive;
}

void BeamGun::setEnemies(Array<Player *> *enemies) {
    this->gun->setEnemies(enemies);
}

void BeamGun::setEnemy(Player *enemy) {
    this->gun->setEnemy(enemy);
}

void BeamGun::translate(const Vector & /*v*/) {
}

void BeamGun::replaceGun(unsigned int /*mesh*/, int /*unused*/) {
}
