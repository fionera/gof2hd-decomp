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

namespace AbyssEngine {
    namespace AEMath {
        Vector operator+(const Vector &lhs, const Vector &rhs);

        Matrix operator*(const Matrix &lhs, const Matrix &rhs);
    }
}

void MatrixRotateVector(Vector *out, const Matrix *matrix, const Vector *vector);

static inline void MatrixGetDir(Vector *out, const Matrix *matrix) { *out = AbyssEngine::AEMath::MatrixGetDir(*matrix); }

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
    int kind = meshKind == 0xe4 ? 2 : meshKind - 9;
    this->field_0x4 = 0;
    this->gun = gun;
    this->level = level;
    this->owner = owner;
    this->meshKind = kind;
    this->primaryGeometry =
            new AEGeometry((uint16_t) (meshKind == 0xe4 ? 0x4a92 : kind + 0x3795), Globals::Canvas, false);

    AEGeometry *secondary = nullptr;
    int mesh;
    if (gun->isPlayerGun() && (mesh = kSecondaryMeshes[gun->itemIndex]) > -1) {
        this->hasSecondary = gun->weaponType != ITEM_SORT_MINE;
        if (this->hasSecondary)
            secondary = new AEGeometry((uint16_t) mesh, Globals::Canvas, false);
    } else {
        this->hasSecondary = 0;
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
    Vector back;
    Vector rotated;
    Matrix playerMatrix;
    Vector transformed;
    Vector position;

    this->gun->update(elapsed);

    Gun *gun = this->gun;
    if (gun->active == 0) {
        this->primaryGeometry->setVisible(false);
        return;
    }

    PaintCanvas *canvas = Globals::Canvas;

    if (gun->hitSmall != 0) {
        AEGeometry *geometry = this->primaryGeometry;
        Transform *transform =
            static_cast<Transform *>(canvas->TransformGetTransform(geometry->transform));
        transform->SetAnimationState((AbyssEngine::AnimationMode) 3, 0);
        transform = static_cast<Transform *>(canvas->TransformGetTransform(geometry->transform));
        transform->SetAnimationState((AbyssEngine::AnimationMode) 1, 0);
        this->gun->hitSmall = 0;
    }

    Transform *primaryTransform =
        static_cast<Transform *>(canvas->TransformGetTransform(this->primaryGeometry->transform));
    primaryTransform->Update((long long) elapsed, false);

    gun = this->gun;
    this->primaryGeometry->setScaling(1.0f);

    Vector up;
    up.x = 0.0f;
    up.y = 1.0f;
    up.z = 0.0f;
    this->primaryGeometry->setDirection(*(Vector *) &gun->field_0x90, up);

    PlayerEgo *player = (PlayerEgo *) this->level->getPlayer();
    position = player->getPosition();

    Vector zero;
    zero.x = 0.0f;
    zero.y = 0.0f;
    zero.z = 0.0f;
    if (memcmp(&this->gun->offset, &zero, sizeof(float) * 3) != 0) {
        player = (PlayerEgo *) this->level->getPlayer();
        Matrix &firstMatrix = player->geometry->getMatrix();
        player = (PlayerEgo *) this->level->getPlayer();
        Matrix &secondMatrix = player->field_0x4->getMatrix();
        playerMatrix = firstMatrix * secondMatrix;

        gun = this->gun;
        back.x = 0.0f;
        back.y = 0.0f;
        back.z = -100.0f;
        rotated = gun->offset + back;
        MatrixRotateVector(&transformed, &playerMatrix, &rotated);
        position = position + transformed;
    }

    this->primaryGeometry->setPosition(position);
    this->primaryGeometry->setVisible(true);

    if (this->hasSecondary != 0) {
        gun = this->gun;
        if (gun->delayActive == 0) {
            AEGeometry *secondary = this->secondaryGeometry;
            Transform *t =
                static_cast<Transform *>(canvas->TransformGetTransform(secondary->transform));
            t->SetAnimationState((AbyssEngine::AnimationMode) 0, 0);
            t = static_cast<Transform *>(canvas->TransformGetTransform(secondary->transform));
            t->SetAnimationState((AbyssEngine::AnimationMode) 3, 0);
            t = static_cast<Transform *>(canvas->TransformGetTransform(secondary->transform));
            t->SetAnimationState((AbyssEngine::AnimationMode) 1, 0);
        } else {
            player = (PlayerEgo *) this->level->getPlayer();
            Vector playerPos = player->getPosition();
            playerMatrix = *(Matrix *) &playerPos;

            gun = this->gun;
            transformed = gun->offset;
            transformed.z = gun->offset.z + -100.0f;

            MatrixRotateVector(&rotated, &((Player *) player->player)->transform, &transformed);
            *(Vector *) &playerMatrix = *(Vector *) &playerMatrix + rotated;
            this->secondaryGeometry->setPosition(*(Vector *) &playerMatrix);

            AEGeometry *secondary = this->secondaryGeometry;
            Transform *t =
                static_cast<Transform *>(canvas->TransformGetTransform(secondary->transform));
            t->Update((long long) elapsed, false);

            MatrixGetDir(&rotated, &((Player *) player->player)->transform);
            back.x = 0.0f;
            back.y = 1.0f;
            back.z = 0.0f;
            this->secondaryGeometry->setDirection(rotated, back);
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
