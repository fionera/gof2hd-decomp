#include "game/ship/PlayerStaticFar.h"

#include <vector>

#include "engine/math/AEMath.h"
#include "engine/math/BoundingVolume.h"
#include "engine/render/AEGeometry.h"
#include "game/ship/PlayerStatic.h"
#include "game/ship/Player.h"
#include "engine/render/PaintCanvas.h"
#include "game/core/Globals.h"


PlayerStaticFar::PlayerStaticFar(int playerId, AEGeometry *geometry, float x, float y, float z)
    : PlayerStatic(playerId, geometry, x, y, z), viewDirection() {
    this->posX = x;
    this->posY = y;
    this->posZ = z;
    this->player->setRadius(0x1d4c);
    this->boundingVolumes = nullptr;
}

PlayerStaticFar::~PlayerStaticFar() = default;

Vector PlayerStaticFar::getProjectionVector(const Vector &value) {
    if (this->boundingVolumes != nullptr) {
        return (*this->boundingVolumes)[0]->getProjectionVector(value);
    }
    return {0.0f, 0.0f, 0.0f};
}

void PlayerStaticFar::render() {
    this->KIPlayer::render();
}

Vector PlayerStaticFar::projectCollisionOnSurface(const Vector &value) {
    if (this->boundingVolumes != nullptr) {
        return BoundingVolume::staticProjectCollisionOnSurface(value, this->boundingVolumes);
    }
    return {0.0f, 0.0f, 0.0f};
}

int PlayerStaticFar::outerCollide(float x, float y, float z) {
    return this->collide(x, y, z);
}

Vector PlayerStaticFar::getInitPosition(Vector /*value*/) {
    Vector v;
    v.x = this->posX;
    v.y = this->posY;
    v.z = this->posZ;
    return v;
}

void PlayerStaticFar::setYRotation(int /*yRotation*/) {
}

int PlayerStaticFar::outerCollide(Vector value) {
    return this->collide(value.x, value.y, value.z);
}

void PlayerStaticFar::update(int /*delta*/) {
    if (this->geometry == nullptr)
        return;

    PaintCanvas *canvas = Globals::Canvas;
    this->cameraPosition() = MatrixGetPosition(
        *reinterpret_cast<AbyssEngine::AEMath::Matrix *>(canvas->CameraGetLocal(canvas->CameraGetCurrent())));
    this->objectPosition() = {(float) this->positionX, (float) this->positionY, (float) this->positionZ};
    this->viewDirection = this->objectPosition() - this->cameraPosition();

    // Far objects are projected onto a sphere of radius 295000 around the camera and scaled down
    // (in 1/4096 steps) so they keep their apparent size.
    int distance = (int) VectorLength(this->viewDirection);
    if (distance > 295000) {
        this->viewDirection = VectorNormalize(this->viewDirection);
        this->viewDirection *= 295000.0f;
        this->viewDirection += this->cameraPosition();
        this->geometry->setPosition(this->viewDirection);

        float scale = (float) (int) (295000.0f / (float) distance * 4096.0f) * (1.0f / 4096.0f);
        this->geometry->setScaling(scale, scale, scale);
    } else {
        this->geometry->setScaling(1.0f, 1.0f, 1.0f);
        this->geometry->setPosition((float) this->positionX, (float) this->positionY, (float) this->positionZ);
    }
}

int PlayerStaticFar::collide(float x, float y, float z) {
    int radius = this->player->radius;
    float dx = x - (float) this->positionX;
    if (dx < (float) radius && dx > (float) -radius) {
        float dy = y - (float) this->positionY;
        if (dy < (float) radius && dy > (float) -radius) {
            float dz = z - (float) this->positionZ;
            if (dz > (float) -radius && dz < (float) radius)
                return true;
        }
    }
    return false;
}
