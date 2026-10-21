#include "game/ship/PlayerWormHole.h"
#include "game/core/Globals.h"

#include "engine/render/AEGeometry.h"
#include "engine/render/PaintCanvas.h"
#include "engine/math/Transform.h"
#include "engine/math/AEMath.h"
#include "engine/core/GameText.h"
#include "game/world/Level.h"
#include "game/world/Station.h"
#include "game/mission/Status.h"
#include "game/ship/Player.h"
#include "game/ship/PlayerEgo.h"
#include "game/ship/PlayerStaticFar.h"
#include "game/ui/Hud.h"
#include "game/core/String.h"
#include "engine/core/AERandom.h"

static inline int wormholeSign(int value) {
    return value == 0 ? 1 : -1;
}

PlayerWormHole::PlayerWormHole(int playerId, AEGeometry *geometry, float x, float y, float z, bool visible)
    : PlayerStaticFar(playerId, geometry, x, y, z) {
    this->name = *Globals::gameText->getText(0x221);
    this->setVisible(visible);
    this->player->setRadius(40000);
    ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(this->geometry->transform))
            ->SetAnimationState((AbyssEngine::AnimationMode) 2, 0);

    this->missionLock = 1;
    this->timer = 0;
    this->scale = 0x1000;
}

void PlayerWormHole::reset(bool shrinking) {
    this->timer = shrinking ? 59000 : 0;
    this->scale = 0x1000;
}

PlayerWormHole::~PlayerWormHole() {
}

int PlayerWormHole::open() {
    this->timer = -3000;
    this->scale = 0;
    return (int) (intptr_t) this;
}

bool PlayerWormHole::isShrinking() {
    return this->timer > 60000;
}

void PlayerWormHole::setPosition(float x, float y, float z) {
    this->posX = x;
    this->posY = y;
    this->posZ = z;
    this->positionY = (int) y;
    this->positionX = (int) x;
    this->positionZ = (int) z;
    this->geometry->setPosition(x, y, z);
}

void PlayerWormHole::render() {
    if (!this->visibleFlag)
        return;
    this->geometry->render();
}

void PlayerWormHole::freeMissionLock() {
    this->missionLock = 0;
}

void PlayerWormHole::update(int elapsed) {
    ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(this->geometry->transform))
            ->Update((long long) elapsed, false);
    if (!this->visibleFlag)
        return;

    this->timer += elapsed;
    if (this->timer < 0) {
        // Opening: grow over 3 seconds.
        this->scale = 0x1000 - (int) ((float) -this->timer / 3000.0f * 4096.0f);
    } else if (this->timer > 60000) {
        int mission = Globals::status->getCurrentCampaignMission();
        if (this->missionLock) {
            // Story missions keep the wormhole open.
            if (mission == 40) {
                if (!Globals::status->inAlienOrbit())
                    this->timer = 60000;
            } else if (mission == 42 && Globals::status->inAlienOrbit()) {
                this->timer = 60000;
            }
        }
        // Closing: shrink over 3 seconds, then reappear elsewhere.
        this->scale = 0x1000 - (int) ((float) (this->timer - 60000) / 3000.0f * 4096.0f);
        if (this->timer > 63000) {
            if ((!Globals::status->inAlienOrbit() && !Globals::status->getStation()->isAttackedByAliens()) ||
                (this->missionLock && !Globals::status->inAlienOrbit() && mission == 42)) {
                this->visibleFlag = 0;
                this->geometry->setVisible(false);
            } else {
                this->timer = -3000;
                int x, y, z;
                if (Globals::status->inAlienOrbit()) {
                    x = (Globals::rnd->nextInt(60000) + 30000) * wormholeSign(Globals::rnd->nextInt(2));
                    y = Globals::rnd->nextInt(40000) + 20000;
                    z = -60000 - Globals::rnd->nextInt(40000);
                } else {
                    x = (Globals::rnd->nextInt(40000) + 20000) * wormholeSign(Globals::rnd->nextInt(2));
                    y = (Globals::rnd->nextInt(40000) + 20000) * wormholeSign(Globals::rnd->nextInt(2));
                    z = (Globals::rnd->nextInt(40000) + 20000) * wormholeSign(Globals::rnd->nextInt(2));
                }
                if (mission == 29 || mission == 41) {
                    this->cameraPosition() = this->level->getPlayer()->getPosition();
                    x = (int) (this->cameraPosition().x + (float) x * 1.7f + (float) x);
                    y = (int) (this->cameraPosition().y + (float) y * 1.7f + (float) y);
                    z = (int) (this->cameraPosition().z + (float) z * 1.7f + (float) z);
                }
                this->setPosition((float) x, (float) y, (float) z);
                if (this->level->getPlayer()->goingToWormhole()) {
                    this->level->getPlayer()->getHUD()->hudEvent(6, this->level->getPlayer(), 0);
                    this->level->getPlayer()->setAutoPilot(nullptr);
                }
            }
        }
    }

    this->PlayerStaticFar::update(elapsed);

    // Face the camera.
    AbyssEngine::PaintCanvas *canvas = Globals::Canvas;
    this->cameraPosition() = AbyssEngine::AEMath::MatrixGetPosition(
            *(AbyssEngine::AEMath::Matrix *) canvas->CameraGetLocal(canvas->CameraGetCurrent()));
    float scale = (float) (this->scale << 4) * (1.0f / 65536.0f);
    this->geometry->setScaling(scale, scale, scale);
    this->viewDirection = this->geometry->getPosition();
    this->cameraPosition() -= this->viewDirection;
    this->cameraPosition() = AbyssEngine::AEMath::VectorNormalize(this->cameraPosition());
    this->cameraPosition().x += 0.5f;
    this->geometry->setDirection(this->cameraPosition(), Vector{0.0f, 1.0f, 0.0f});
}

Vector PlayerWormHole::getPosition() {
    float fy = (float) this->positionY;
    float fx = (float) this->positionX;
    float fz = (float) this->positionZ;
    Vector result;
    result.x = fx;
    result.y = fy;
    result.z = fz;
    return result;
}
