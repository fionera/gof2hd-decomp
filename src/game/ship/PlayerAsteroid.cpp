#include "game/ship/PlayerAsteroid.h"
#include "game/core/Globals.h"
#include "engine/core/AERandom.h"
#include "engine/render/AEGeometry.h"
#include "engine/math/AEMath.h"
#include "game/world/Level.h"
#include "game/mission/Explosion.h"
#include "game/ship/KIPlayer.h"
#include "game/ship/Player.h"
#include "engine/math/Transform.h"
#include "engine/render/PaintCanvas.h"

namespace AbyssEngine {
}



PlayerAsteroid::PlayerAsteroid(int playerId, AEGeometry *geometry, int explosionType,
                               int asteroidIndex, const Vector &position, float scaling,
                               int quality)
    : KIPlayer(playerId, -1, new Player(1500, 30, 0, 0, 0), geometry,
               position.x, position.y, position.z, false) {
    this->player->setKIPlayer(this);
    this->asteroidIndex = asteroidIndex;
    this->asteroidFlag = 0;
    this->scaling = scaling;
    this->quality = quality;
    this->player->setRadius((int) (static_cast<AbyssEngine::Transform *>(
            Globals::Canvas->TransformGetTransform(geometry->transform))->boundingSphere.radius * scaling * 0.7f));
    this->player->setMaxHitpoints((int) (scaling * 100.0f + 30.0f));
    this->lastHitpoints = this->player->getHitpoints();
    this->minable = quality > 3;

    this->explosion = new Explosion(explosionType + 2);
    this->explosion->setScaling(scaling);
    this->geometry->setScaling(scaling, scaling, scaling);
    this->setPosition(position);

    tmp_vector2 = Vector((float) (Globals::rnd->nextInt(8192) - 4096) * 0.00024414062f,
                         (float) (Globals::rnd->nextInt(8192) - 4096) * 0.00024414062f,
                         (float) (Globals::rnd->nextInt(8192) - 4096) * 0.00024414062f);

    this->geometry->setRotation((float) Globals::rnd->nextInt(100) * 0.01f * 6.2831855f,
                                (float) Globals::rnd->nextInt(100) * 0.01f * 6.2831855f,
                                (float) Globals::rnd->nextInt(100) * 0.01f * 6.2831855f);
    this->rotationEnabled = 1;
    this->secondaryGeometry() = nullptr;

    float size = scaling < 0.9f ? 0.9f : scaling;
    this->spin = Vector((float) (Globals::rnd->nextInt(3) - 1), (float) (Globals::rnd->nextInt(3) - 1),
                        (float) (Globals::rnd->nextInt(3) - 1)) * (1.0f - (size < 1.0f ? size : 1.0f));

    this->state = 0;
    this->stealFlagByte = 1;
    this->dropsLoot() = 1;
    emitTime = 0.0f;
    this->hitFlashTimer = 0.0f;
    this->field_0x160 = 0.0001f;
}

PlayerAsteroid::~PlayerAsteroid() {
    if (this->explosion != nullptr) {
        delete this->explosion;
    }
    this->explosion = nullptr;
}

void PlayerAsteroid::setAsteroidIndex(int asteroidIndex) {
    this->asteroidIndex = asteroidIndex;
}

Vector PlayerAsteroid::getPosition() {
    return this->geometry->getPosition();
}

void PlayerAsteroid::setPosition(const Vector &position) {
    this->geometry->setPosition(position);
}

void PlayerAsteroid::translate(const Vector &delta) {
    this->geometry->translate(delta);
}

uint8_t PlayerAsteroid::isMinable() {
    return this->minable;
}

int PlayerAsteroid::getQuality() {
    return this->quality;
}

float PlayerAsteroid::getScaling() {
    return this->scaling;
}

int PlayerAsteroid::getQualityFrameIndex() {
    return 7 - this->quality;
}

String PlayerAsteroid::getQualityString() {
    const char *grade;
    switch (this->quality) {
        case 7:
            grade = "A";
            break;
        case 6:
            grade = "B";
            break;
        case 5:
            grade = "C";
            break;
        default:
            grade = this->quality == 4 ? "D" : "E";
            break;
    }
    return String(grade);
}

void PlayerAsteroid::setAsteroidCenter(Vector center) {
    asteroidCenter = center;
    asteroidDistance = (int) VectorLength(center);
}

void PlayerAsteroid::setRotationEnabled(bool enabled) {
    this->rotationEnabled = enabled;
}

void PlayerAsteroid::initPush(const Vector &target, int duration) {
    float ratio = VectorLength(target - this->getPosition()) / (float) duration;
    float clamped = (ratio < 1.0f) ? ratio : 1.0f;
    int pushTime = (int) ((1.0f - clamped) * 5000.0f);
    this->pushTimer() = pushTime;
    this->pushDuration() = pushTime;
    this->pushDirection() = VectorNormalize(this->getPosition() - target);

    float spinX = (float) (Globals::rnd->nextInt(200) - 100);
    float spinY = (float) (Globals::rnd->nextInt(200) - 100);
    float spinZ = (float) (Globals::rnd->nextInt(200) - 100);
    this->pushSpin() = 0.2f * VectorNormalize(Vector{spinX, spinY, spinZ});
}

void PlayerAsteroid::push(int delta) {
    if (this->pushTimer() <= 0)
        return;

    this->pushTimer() -= delta;
    float t = (float) this->pushTimer() / (float) this->pushDuration();

    Matrix rotation;
    MatrixSetRotation(rotation, t * this->pushSpin().x, t * this->pushSpin().y, t * this->pushSpin().z);
    if (this->lastDelta > 0)
        this->geometry->setMatrix(this->geometry->getMatrix() * rotation);
    float speed = (float) this->lastDelta;
    AEGeometry *geometry = this->geometry;
    float duration = (float) this->pushDuration();
    geometry->translate(this->pushDirection() * speed * ((2.0f - t) * 3.0f * (duration / 5000.0f)));
}

void PlayerAsteroid::update(int delta) {
    this->lastDelta = delta;
    if (delta == 0)
        return;

    if (!this->player->isActive() && this->state == 4) {
        this->asteroidFlag = 0;
        return;
    }

    if (this->player->getHitpoints() <= 0 && this->state == 0) {
        this->state = 3;
        this->level->asteroidDied();
        Globals::status->field_d8++;
        if (this->dropsLoot() && ((this->quality == 7 && Globals::rnd->nextInt(100) < 4) ||
                                  (this->quality <= 6 && Globals::rnd->nextInt(100) < 20))) {
            this->loot() = new Array<int>();
            // Ore of the asteroid's type; the best quality yields the refined variant.
            if (this->asteroidIndex == 217)
                ArrayAdd(this->quality == 7 ? 218 : 217, *this->loot());
            else
                ArrayAdd(this->quality == 7 ? this->asteroidIndex + 11 : this->asteroidIndex, *this->loot());
            ArrayAdd(this->quality == 7 ? 1 : Globals::rnd->nextInt(3) + 1, *this->loot());
            this->createCrate(this->asteroidIndex == 164 ? 2 : 1);
        } else {
            this->loot() = nullptr;
            this->dropsLoot() = 0;
        }
        this->explosion->start(this->geometry->getMatrix());
        return;
    }

    if (this->state == 4) {
        this->setActive(false);
        this->player->setBombForce(0.0f);
    } else if (this->state == 3) {
        this->explosion->update(delta, (TargetFollowCamera *) nullptr);
        if (!this->explosion->isPlaying()) {
            this->state = 4;
            this->player->setBombForce(0.0f);
        }
    }

    if (this->lastHitpoints > this->player->getHitpoints()) {
        this->hitFlashTimer = 0.1f;
        this->field_0x160 = 0.001f;
        this->lastHitpoints = this->player->getHitpoints();
    }

    if (this->rotationEnabled)
        this->geometry->rotate(this->spin * ((float) delta * 0.001f));

    float bombForce = this->player->getBombForce();
    if (bombForce > 0.0f && this->state == 3) {
        this->hitVector() = this->player->getHitVector();
        float size = this->scaling;
        // Clamp the scale to [0.6, 1.0]. The binary evaluates the redundant inner
        // `size > 1.0f` test inside the `size < 0.6f` arm (four selects), so the
        // nesting is kept as written.
        if (size < 0.6f || size > 1.0f)
            size = size < 0.6f ? (size > 1.0f ? 1.0f : 0.6f) : 1.0f;
        this->hitVector() *= (float) (int) (bombForce * 1024.0f * (1.0f - size / 100.0f));
        this->translate(this->hitVector());
        this->explosion->translate(this->hitVector());
        if (this->secondaryGeometry() != nullptr)
            this->secondaryGeometry()->translate(this->hitVector());
        float nextForce = bombForce * 0.98f;
        if (nextForce < 0.05f)
            nextForce = 0.0f;
        this->player->setBombForce(nextForce);
    }
}

void PlayerAsteroid::render() {
    if (this->visibleFlag == 0)
        return;
    if (this->secondaryGeometry() != nullptr)
        this->secondaryGeometry()->render();
    if (this->state == 3)
        this->explosion->render();
    else if (this->state == 0)
        this->geometry->render();
}

int PlayerAsteroid::outerCollide(float x, float y, float z) {
    return this->collide(x, y, z);
}

int PlayerAsteroid::outerCollide(Vector point) {
    return this->collide(point.x, point.y, point.z);
}

Vector PlayerAsteroid::getProjectionVector(const Vector &value) {
    Vector projection = value;
    pos = this->geometry->getPosition();
    pos -= projection;
    projection = pos;
    projection = VectorNormalize(projection);
    return projection;
}

int PlayerAsteroid::collide(float x, float y, float z) {
    if (this->player->getHitpoints() < 1)
        return false;

    float px = this->geometry->getPosition().x;
    float py = this->geometry->getPosition().y;
    float pz = this->geometry->getPosition().z;
    int radius = this->player->radius;
    if (x - px < (float) radius && x - px > (float) -radius &&
        y - py < (float) radius && y - py > (float) -radius &&
        z - pz < (float) radius && z - pz > (float) -radius)
        return true;
    return false;
}

AbyssEngine::AEMath::Vector PlayerAsteroid::tmp_vector2;
AbyssEngine::AEMath::Vector PlayerAsteroid::asteroidCenter;
int PlayerAsteroid::asteroidDistance;
AbyssEngine::AEMath::Vector PlayerAsteroid::pos;
float PlayerAsteroid::emitTime;
