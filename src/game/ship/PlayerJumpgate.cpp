#include "game/ship/PlayerJumpgate.h"
#include "game/ship/Player.h"
#include "game/world/SolarSystem.h"
#include "engine/render/AEGeometry.h"
#include "game/mission/Status.h"
#include "engine/math/Transform.h"
#include "engine/math/BoundingSphere.h"
#include "engine/render/PaintCanvas.h"
#include "game/core/Globals.h"

bool PlayerJumpgate::timeToJump() {
    AbyssEngine::Transform *transform =
            (AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(this->transformHandle);
    return transform->currentTime > 1000LL;
}

void PlayerJumpgate::activate() {
    if (this->activated != 0)
        return;

    if (this->transformHandle != 0xffffffffU) {
        AbyssEngine::Transform *transform =
                (AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(this->transformHandle);
        transform->SetAnimationState((AbyssEngine::AnimationMode) 1, nullptr);

        Globals::Canvas->TransformRemoveChild(this->geometry->transform,
                                            this->geometry->childTransform);
        this->geometry->addChild(this->transformHandle);
    }

    this->activated = 1;
}

void PlayerJumpgate::addJumpAnimationHandle(uint32_t handle) {
    this->transformHandle = handle;
}

bool PlayerJumpgate::animationEnded() {
    if (this->activated == 0)
        return false;

    AbyssEngine::Transform *transform =
            (AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(this->transformHandle);
    return !transform->animating;
}

void PlayerJumpgate::update(int delta) {
    if (!this->visibleFlag)
        return;

    AbyssEngine::Transform *transform =
            (AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(this->geometry->transform);
    transform->Update((longlong) delta, true);
}

void PlayerJumpgate::setPosition(float x, float y, float z) {
    this->positionX = (int) x;
    this->positionY = (int) y;
    this->positionZ = (int) z;
    this->geometry->setPosition(x, y, z);
}

PlayerJumpgate::PlayerJumpgate(int playerId, AEGeometry *geometry, float x, float y, float z,
                               bool visible)
    : PlayerStaticFar(playerId, geometry, x, y, z) {
    this->setVisible(visible);

    if (visible) {
        this->boundingVolumes = new Array<BoundingVolume *>();
        ArraySetLength(1, *(this->boundingVolumes));

        int radius;
        if (Globals::status->inAlienOrbit()) {
            radius = 7500;
        } else {
            radius = ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getRace() == 1 ? 11250 : 7500;
        }

        this->player->setRadius(radius);

        (*this->boundingVolumes)[0] = new BoundingSphere(x, y, z, 0.0f, 0.0f, 0.0f, (float) radius);
    }

    this->activated = 0;
    this->transformHandle = 0xffffffffU;
    this->geometry->setRotation(0.0f, 3.1415927f, 0.0f);
}

PlayerJumpgate::~PlayerJumpgate() {
}
