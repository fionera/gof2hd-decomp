#include "gof2/game/ship/PlayerJumpgate.h"
#include "gof2/game/ship/Player.h"
#include "gof2/game/world/SolarSystem.h"
#include "gof2/engine/render/AEGeometry.h"
#include "gof2/game/mission/Status.h"
#include "gof2/engine/math/Transform.h"
#include "gof2/engine/math/BoundingSphere.h"
#include "gof2/game/core/PaintCanvasClass.h"

// Game singletons (raw engine globals, kept as externs).
extern void* g_PaintCanvas;   // PaintCanvas singleton pointer
extern void* g_Status;        // Status singleton pointer

static PaintCanvas* paintCanvas()
{
    return (PaintCanvas*)g_PaintCanvas;
}

bool PlayerJumpgate::timeToJump()
{
    AbyssEngine::Transform* transform =
        (AbyssEngine::Transform*)paintCanvas()->TransformGetTransform(this->transformHandle);
    return transform->currentTime > 1000LL;
}

void PlayerJumpgate::activate()
{
    if (this->activated != 0)
        return;

    if (this->transformHandle != 0xffffffffU) {
        AbyssEngine::Transform* transform =
            (AbyssEngine::Transform*)paintCanvas()->TransformGetTransform(this->transformHandle);
        transform->SetAnimationState(AbyssEngine::AnimationMode_0, nullptr);

        paintCanvas()->TransformRemoveChild(this->geometry->transform,
                                            this->geometry->childTransform);
        this->geometry->addChild(this->transformHandle);
    }

    this->activated = 1;
}

void PlayerJumpgate::addJumpAnimationHandle(uint32_t handle)
{
    this->transformHandle = handle;
}

bool PlayerJumpgate::animationEnded()
{
    if (this->activated == 0)
        return false;

    AbyssEngine::Transform* transform =
        (AbyssEngine::Transform*)paintCanvas()->TransformGetTransform(this->transformHandle);
    return !transform->animating;
}

void PlayerJumpgate::update(int delta)
{
    if (!this->isVisible())
        return;

    AbyssEngine::Transform* transform =
        (AbyssEngine::Transform*)paintCanvas()->TransformGetTransform(this->geometry->transform);
    transform->Update((longlong)delta, true);
}

void PlayerJumpgate::setPosition(float x, float y, float z)
{
    this->positionX = (int)x;
    this->positionY = (int)y;
    this->positionZ = (int)z;
    Vector pos = { x, y, z };
    this->geometry->setPosition(pos);
}

PlayerJumpgate::PlayerJumpgate(int playerId, AEGeometry* geometry, float x, float y, float z,
                               bool visible)
    : PlayerStaticFar(playerId, geometry, x, y, z)
{
    this->setVisible(visible);

    if (visible) {
        this->boundingVolumes = new std::vector<BoundingVolume*>();
        this->boundingVolumes->resize(1);

        Status* status = (Status*)*(void**)g_Status;
        int radius;
        if (status->inAlienOrbit()) {
            radius = 0x1d4c;
        } else {
            SolarSystem* system = (SolarSystem*)(intptr_t)status->getSystem();
            radius = system->getRace() == 1 ? 0x2bf2 : 0x1d4c;
        }

        this->player->setRadius(radius);

        (*this->boundingVolumes)[0] = new BoundingSphere(x, y, z, 0.0f, 0.0f, 0.0f, (float)radius);
    }

    this->transformHandle = 0xffffffffU;
    this->activated = 0;
    this->geometry->setRotation(0.0f, 3.1415927f, 0.0f);
}
