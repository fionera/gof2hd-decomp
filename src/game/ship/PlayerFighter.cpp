#include "game/ship/PlayerFighter.h"
#include "game/core/Globals.h"
#include "engine/core/AERandom.h"
#include "engine/render/PaintCanvas.h"
#include "engine/render/Mesh.h"
#include "engine/render/Material.h"
#include "game/mission/Mission.h"
#include "engine/render/AEGeometry.h"
#include "engine/render/ParticleSystemManager.h"
#include "game/mission/Generator.h"
#include "game/world/Level.h"
#include "game/mission/Status.h"
#include "engine/render/Trail.h"
#include "game/mission/Explosion.h"
#include "game/ship/KIPlayer.h"
#include "game/ship/Player.h"
#include "game/world/Route.h"
#include "game/world/Standing.h"
#include "engine/math/Transform.h"
#include "engine/math/EaseInOutMatrix.h"
#include "game/weapons/Radar.h"
#include "engine/audio/FModSound.h"
#include "game/world/SpacePoint.h"
#include "game/world/Wanted.h"
#include "game/ship/PlayerFixedObject.h"
#include "game/ship/PlayerEgo.h"
#include "game/world/Waypoint.h"
#include "game/world/Station.h"
#include "game/ship/Ship.h"
#include "engine/core/GameText.h"
#include "game/ui/Layout.h"
#include "game/ship/TargetFollowCamera.h"



namespace AbyssEngine {
    namespace AEMath {
        float VectorLength(const Vector &value);

        Vector operator-(const Vector &lhs, const Vector &rhs);

        Vector operator*(float lhs, const Vector &rhs);
    }
}


static const float gIP_strength = 5000.0f;


static unsigned **const gR_g4 = nullptr;
static unsigned **const gR_g5 = nullptr;


static const float gPush_div = 5000.0f;


static const float gHC_divIn = 2000.0f;
static const float gHC_divOut = -2000.0f;


int PlayerFighter::stationRouteAliens;

// Docking-port capture radius per ship type (this TU's copy of the shared table).
static const int kShipDockRadius[64] = {
    160, 150, 220, 310, 160, 180, 140, 170, 170, 110, 250, 205, 240, 250, 250, 250,
    170, 170, 190, 460, 170, 210, 140, 170, 160, 190, 170, 240, 220, 280, 200, 200,
    140, 160, 180, 150, 310, 390, 480, 200, 300, 300, 170, 200, 170, 370, 370, 300,
    440, 340, 170, 470, 170, 170, 170, 170, 200, 170, 250, 150, 170, 250, 270, 170,
};

PlayerFighter::PlayerFighter(int faction, int wingmanCmd, Player *player, AEGeometry *geom,
                             float x, float y, float z, bool flag)
    : KIPlayer(faction, -1, player, geom, x, y, z, flag),
      field_0x200(0), field_0x204(0), field_0x208(0),
      workingPosition(), targetPos(), distanceToTarget(), field_0x17c(), dyingSpin(), dyingDirection() {
    // patrol route: the four corners of a box in random order, visited 2-4 times
    Vector c0 = {(float) (Globals::rnd->nextInt(25000) - 30000), (float) (Globals::rnd->nextInt(10000) - 10000),
                 (float) (Globals::rnd->nextInt(25000) + 20000)};
    Vector c1 = {(float) (Globals::rnd->nextInt(25000) + 5000), (float) (Globals::rnd->nextInt(10000) - 10000),
                 (float) (Globals::rnd->nextInt(25000) + 20000)};
    Vector c2 = {(float) (Globals::rnd->nextInt(25000) + 5000), (float) (Globals::rnd->nextInt(10000) - 10000),
                 (float) (Globals::rnd->nextInt(25000) + 55000)};
    Vector c3 = {(float) (Globals::rnd->nextInt(25000) - 30000), (float) (Globals::rnd->nextInt(10000) - 10000),
                 (float) (Globals::rnd->nextInt(25000) + 55000)};
    Vector corners[4] = {c0, c1, c2, c3};
    int count = Globals::rnd->nextInt(3) * 3 + 6;
    char used[4] = {0, 0, 0, 0};
    int *points = new int[count];
    for (int i = 0; i < count; i += 3) {
        int corner;
        do {
            corner = Globals::rnd->nextInt(4);
        } while (used[corner] != 0);
        used[corner] = 1;
        points[i] = (int) corners[corner].x;
        points[i + 1] = (int) corners[corner].y;
        points[i + 2] = (int) corners[corner].z;
    }
    this->defaultRoute = new Route(points, count);
    delete[] points;

    if (PlayerFighter::stationRouteAliens == 0) {
        int stationWaypoints[12] = {
            40000, 0, 40000,
            40000, 0, -40000,
            -40000, 0, -40000,
            -40000, 0, 40000
        };
        PlayerFighter::stationRouteAliens = (int) (intptr_t) new Route(stationWaypoints, 12);
    }

    this->field_0x130 = -1;
    this->field_0x134 = -1;
    this->field_0x138 = -1;
    this->rotate = 2.0f;
    this->shootError = 500.0f * 1.525878e-5f;
    this->speed = 2.0f;
    this->field_0x1b0 = 1500;
    this->field_0x128 = 50000;
    this->boostProb = 5;
    this->field_0x38 = 0;
    this->field_0x140 = 0;
    this->field_0x12d = 0;
    this->targetIsActive = 0;
    this->field_0x13d = 0;
    this->field_0x13e = 0;
    this->dyingTimer = 0;
    this->deathTimer() = 0;
    this->field_0x12f = 0;
    this->smoking = 0;
    this->damageEscape = 0;
    this->trailTick = 0;
    this->boosting = 0;
    this->field_0x1f5 = 0;
    this->behaviourChangeTick = 0;
    this->shipGroup = wingmanCmd;
    this->field_0x1bc = 0;
    this->boostTick = 0;
    this->boostTimeLimit = 0;
    this->targetPlayer = nullptr;
    this->commandRoute = nullptr;
    this->boundingVolumes = nullptr;
    this->trail = nullptr;
    this->frameTime = 0;
    this->hitpoints = 0;
    this->damageAccumulator = 0;
    this->initialWaypoint = 0;
    this->currentSpeed = 0;
    this->targetSpeed = 0;
    this->currentRotate = 0;

    this->workingPosition = Vector{x, y, z};
    this->crateCaptured() = 1;
    this->field_0x13d = 1;
    this->currentSpeed = this->speed;
    this->currentRotate = this->rotate;
    this->defaultRoute->setLoop(true);
    ((Route *) (intptr_t) PlayerFighter::stationRouteAliens)->setLoop(true);
    this->routeClone() = nullptr;
    if (Globals::status->getCurrentCampaignMission() != 41) {
        this->routeClone() = wingmanCmd == 9 ? ((Route *) (intptr_t) PlayerFighter::stationRouteAliens)->clone()
                                             : this->defaultRoute->clone();
    }
    this->field_0x12d = 0;
    if (wingmanCmd == 9) {
        this->lootList() = nullptr;
    } else {
        Generator *generator = new Generator();
        this->lootList() = generator->getLootList(-1, -1);
        delete generator;
    }
    this->field_0x128 = Globals::status->inAlienOrbit() ? 100000 : 50000;
    this->explosion = new Explosion(0);
    this->explosion->addFireStreaks();
    this->field_0x13e = 1;
    this->hitpoints = this->player->getHitpoints();
    this->damageAccumulator = 0;
    this->damageEscape = 0;
    this->field_0xe4 = 1;
    this->field_0x25 = 1;
    this->engineTrailSystem = -1;
    if (Globals::status->getCurrentCampaignMission() == 1) {
        this->fov() = -1;
    } else {
        this->fov() = this->field_0xdc() == 0 ? 46 : 48;
    }
    this->targetRoll = 0;
    this->smoothRoll = 0;
    this->field_0x214 = 0;
    this->maxRoll = 750.0f;
    this->rollFactor = 15.1393566f;
    for (int i = 0; i < 5; ++i) {
        this->rollSamples[i] = 0;
    }
    this->rollSampleIndex = 0;
    this->rollBufferFilled = 0;
    this->rollActive() = 0;
    this->field_0x254 = 0;
    this->field_0x255 = 0;
    this->easeMatrix = nullptr;
    this->field_0x13c = 0;
    this->spacePoint = 0;
    this->cloakTimer = 0;
    this->cloakDuration = 0;
    this->cloakActive = 0;
    this->cloakCooldown = 0;
    this->cloakingPossible = 1;
    this->cloakMaterial = 0xffffffff;
    this->aiDisabled = 0;
    this->gunSwitchTimer = 0;
}
PlayerFighter::~PlayerFighter() {
    delete this->defaultRoute;
    this->defaultRoute = 0;

    if (this->boundingVolumes != 0) {
        ArrayReleaseClasses(*this->boundingVolumes);
        delete this->boundingVolumes;
    }
    this->boundingVolumes = 0;

    delete this->trail;
    this->trail = 0;

    delete this->explosion;
    this->explosion = 0;

    delete this->easeMatrix;
    this->easeMatrix = 0;
}
void PlayerFighter::reset() {
    KIPlayer::reset();
    this->crateCaptured() = 1;

    this->workingPosition = Vector{this->posX, this->posY, this->posZ};
    this->shipPosition = Vector(this->workingPosition);

    this->frameTime = 0;
    this->field_0x38 = 0;
    this->targetIsActive = 0;
    this->targetPlayer = nullptr;
    this->field_0x12c = 0;
    this->behaviourChangeTick = 0;
    this->boostTick = 0;
    this->boostTimeLimit = 0;
    this->trailTick = 0;
    if (this->state != 5) {
        this->state = 0;
    }

    this->tempVector = Vector{0, 0, 0};
    this->targetPos = Vector{0, 0, 0};
    this->distanceToTarget = Vector{0, 0, 0};

    this->isMissionCrate() = 0;
    this->missionCrateLost() = 0;
    this->missionCrateCaptured() = 0;
    this->crateLost() = 0;
    this->crateCaptured() = 1;
    this->field_0x140 = 0;
    this->smoking = 0;
    this->field_0x13c = 0;
    this->cloakActive = 0;
    this->cloakTimer = 0;
    this->cloakDuration = 0;
    this->cloakCooldown = 0;
    this->cloakingPossible = 1;
    this->aiDisabled = 0;
}
void PlayerFighter::setLevel(Level *lvl) {
    KIPlayer::setLevel(lvl);

    int handle = this->KIPlayer::level->field_74->addSystem(
            &this->KIPlayer::geometry->getReferenceMatrix(), (ParticleSettings::ParticleSet) 9, false);
    this->engineTrailSystem = handle;
    this->KIPlayer::level->field_74->enableSystemEmit(handle, false);

    handle = this->KIPlayer::level->particleEmitBoolPtr->addSystem(
            &this->KIPlayer::geometry->getReferenceMatrix(), (ParticleSettings::ParticleSet) 15, false);
    this->field_0x80 = handle;
    this->KIPlayer::level->particleEmitBoolPtr->enableSystemEmit(handle, false);

    handle = this->KIPlayer::level->particleRenderBoolPtr->addSystem(
            &this->KIPlayer::geometry->getReferenceMatrix(), (ParticleSettings::ParticleSet) 42, false);
    this->field_0x84 = handle;
    this->KIPlayer::level->particleRenderBoolPtr->enableSystemEmit(handle, false);

    handle = this->KIPlayer::level->field_8c->addSystem(
            &this->KIPlayer::geometry->getReferenceMatrix(), (ParticleSettings::ParticleSet) 17, false);
    this->field_0x134 = handle;
    this->KIPlayer::level->field_8c->enableSystemEmit(handle, false);

    handle = this->KIPlayer::level->field_8c->addSystem(
            &this->KIPlayer::geometry->getReferenceMatrix(), (ParticleSettings::ParticleSet) 18, false);
    this->field_0x138 = handle;
    this->KIPlayer::level->field_8c->enableSystemEmit(handle, false);
}
void PlayerFighter::setMissionCrate(bool on) {
    this->isMissionCrate() = on;
    if (on) {
        this->lootList() = nullptr;
        this->lootList() = new Array<int>();
        int mission = (int) (intptr_t) Globals::status->getMission();
        int type = ((Mission *) (mission))->getType();
        int item = (type == 5) ? 0x74 : 0x75;
        ArrayAdd(item, *(this->lootList()));

        ArrayAdd<int>(1, *this->lootList());
    }
}
uint8_t PlayerFighter::hasMissionCrateCaptured() {
    return this->missionCrateCaptured();
}
uint8_t PlayerFighter::hasMissionCrateLost() {
    return this->missionCrateLost();
}
void PlayerFighter::setShipGroup(AEGeometry *geom, int group, bool flag) {
    KIPlayer::setShipGroup(geom, group, flag);
}
uint8_t PlayerFighter::hasCrateCaptured() {
    return this->crateCaptured() == 0;
}
uint8_t PlayerFighter::hasCrateLost() {
    return this->crateLost();
}
void PlayerFighter::setPosition(const Vector &v) {
    this->setPosition(v.x, v.y, v.z);
}
void PlayerFighter::setPosition(float x, float y, float z) {
    this->posX = x;
    this->posY = y;
    this->posZ = z;

    this->geometry->setPosition(x, y, z);
    this->workingPosition = Vector{x, y, z};
    if (this->trail != 0) {
        this->trail->reset(this->workingPosition);
    }
    this->player->transform =
            this->geometry->getMatrix();

    return;
}
void PlayerFighter::setWingmanCommand(int cmd, KIPlayer *target) {
    int saved = this->field_0xe4;
    KIPlayer::setWingmanCommand(cmd, target);
    switch (cmd) {
        case 2:
        case 3:
            this->behaviourChangeTick = 0x1389;
            if (this->currentSpeed != 5.5f) {
                this->boostTick = 0x1389;
            }
            if (cmd == 2) {
                if (this->level->getPlayerRoute() != 0) {
                    this->commandRoute = this->level->getPlayerRoute()->getExactClone();
                    this->initialWaypoint = this->commandRoute->getCurrent();
                    break;
                }
            } else if (target != 0) {
                break;
            }
            this->field_0xe4 = saved;
            break;
        case 1:
            this->damageEscape = 0;
            this->field_0x13d = 0;
            break;
        case 0:
            this->field_0xe4 = saved;
            this->field_0x140 = (this->field_0x140 == 0);
            break;
    }
    this->currentSpeed = this->speed;
    this->currentRotate = this->rotate;
}
void PlayerFighter::setSpeed(float v) {
    this->field_0x13d = 0;
    this->speed = v;
    this->currentSpeed = v;
}
void PlayerFighter::setRotate(int v) {
    float f = (float) v;
    this->field_0x13d = 0;
    this->rotate = f;
    this->currentRotate = f;
}
void PlayerFighter::setShootError(int v) {
    this->shootError = (float) v;
}
void PlayerFighter::setBoostProb(int v) {
    this->boostProb = v;
}
void PlayerFighter::removeTrail() {
    delete this->trail;
    this->trail = 0;
}
void PlayerFighter::setExhaustVisible(bool vis) {
    AEGeometry *geom = this->geometry;
    if (geom != 0) {
        AEGeometry *sub = this->parentGeometry;
        if (sub != 0) {
            int id = (int) sub->childTransform;
            if (id == -1)
                return;
            unsigned t = (unsigned) (unsigned long) Globals::Canvas->TransformGetTransform(id);

            return ((AbyssEngine::Transform *) (unsigned long) t)->SetVisible(vis);
        }

        int id = (int) geom->childTransform;
        if (id != -1) {
            unsigned t = (unsigned) (unsigned long) Globals::Canvas->TransformGetTransform(id);

            return ((AbyssEngine::Transform *) (unsigned long) t)->SetVisible(vis);
        }
    }
}
void PlayerFighter::cloak(int dur, bool b) {
    if (dur <= 0) {
        dur = Globals::rnd->nextInt(5000) + 5000;
    }
    this->cloakDuration = dur + 4000;
    this->cloakActive = 1;
    this->field_0x2d9 = b;
}
void PlayerFighter::setCloakingPossible(bool v) {
    PlayerFighter *self = this;
    self->cloakingPossible = v;
    if (!v && self->field_0x13c != 0) {
        self->cloakTimer = self->cloakDuration + 1;

        return self->handleCloaking();
    }
}
void PlayerFighter::handleCloaking() {
    if (this->shipGroup != 10) return;
    if (this->geometry == 0) return;
    if (KIPlayer::isDead()) return;
    if (this->player->empDisabledByte != 0) return;
    if (this->cloakingPossible == 0) return;

    if (this->cloakActive == 0) {
        if (this->damageEscape != 0 && Globals::rnd->nextInt(100) < 50) {
            this->cloak(-1, false);
        } else {
            this->cloakCooldown += this->frameTime;
            if (this->cloakCooldown > 8000) {
                this->cloakCooldown = 0;
                if (Globals::rnd->nextInt(100) < 30) {
                    this->cloak(-1, false);
                }
            }
        }
    } else {
        if (this->cloakTimer == 0) {
            this->field_0x13c = 1;
            if (this->cloakMaterial == 0xffffffff) {
                Globals::Canvas->MeshCloneMaterial(this->parentGeometry->meshId,
                                                   this->cloakMaterial);
                this->cloakSavedMode =
                        ((AbyssEngine::Material *) Globals::Canvas
                                 ->MeshGetPointer(this->parentGeometry->meshId)
                                 ->material)
                                ->blendMode;
            }
            ((AbyssEngine::Material *) Globals::Canvas->MaterialGetMaterial(this->cloakMaterial))->blendMode = 14;
            Globals::Canvas->MeshChangeMaterial(this->parentGeometry->meshId,
                                                (unsigned short) this->cloakMaterial);
            if (this->field_0x2d9 != 0) {
                this->cloakTimer = 2000;
                this->field_0x2d9 = 0;
            }
        }
        this->cloakTimer += this->frameTime;

        if (this->cloakTimer - this->frameTime <= 2000) {
            if (this->cloakTimer >= 2000) {
                this->setExhaustVisible(false);
                this->exhaustHidden() = 1;
            }
            Globals::Canvas->MeshChangeShaderAnimValue(
                    Globals::Canvas->MeshGetPointer(this->parentGeometry->meshId),
                    (float) this->cloakTimer / 2000.0f, 1u);
        } else if (this->cloakTimer > this->cloakDuration) {
            int material = this->cloakMaterial;
            int savedMode = this->cloakSavedMode;
            this->cloakTimer = 0;
            this->cloakActive = 0;
            this->field_0x13c = 0;
            ((AbyssEngine::Material *) Globals::Canvas->MaterialGetMaterial(material))->blendMode = savedMode;
            this->setExhaustVisible(true);
        } else if (this->cloakTimer > this->cloakDuration - 2000) {
            this->exhaustHidden() = 0;
            Globals::Canvas->MeshChangeShaderAnimValue(
                    Globals::Canvas->MeshGetPointer(this->parentGeometry->meshId),
                    ((float) this->cloakTimer - (float) (this->cloakDuration - 2000)) / -2000.0f + 1.0f, 1u);
        }
    }
}
void PlayerFighter::setAIDisabled(bool v) {
    this->aiDisabled = v;
}
void PlayerFighter::update(int dt) {
    using namespace AbyssEngine::AEMath;

    // wreck burnt out (no crate left, or the crate drifted for a minute): retire the fighter
    if (this->state == 4 && !this->explosion->isPlaying() &&
        (!this->crateCaptured() || this->deathTimer() > 60000)) {
        this->setActive(false);
        return;
    }

    this->boostTick += dt;
    this->behaviourChangeTick += dt;
    if (this->shipGroup == 1) {
        this->shipGroup = 1;
    }
    this->frameTime = dt;
    this->shipPosition = this->geometry->getPosition();

    // enemy / friend flags from the standing towards this fighter's race (shipGroup)
    if (!this->reviveLockFlag) {
        this->player->enemyFlagsLo = (this->shipGroup & ~1) == 8 ? true
                                     : this->isWingMan()      ? false
                                     : ((Standing *) Globals::status->getStanding())->isEnemy(this->shipGroup);
        this->player->carriesFriendCargoFlag = (this->shipGroup & ~1) == 8 ? false
                                               : this->isWingMan()      ? true
                                               : ((Standing *) Globals::status->getStanding())->isFriend(this->shipGroup);
    }
    if (this->reviveLockFlag &&
        this->player->damageDoneByPlayer > this->player->getMaxHitpoints() / 20) {
        this->player->enemyFlagsLo = true;
    }

    // bounty targets switch between their primary and secondary weapons every 20 s
    if (__builtin_expect(this->field_0x42, 1)) { // lint: arm_builtin
        this->gunSwitchTimer += dt;
        if (this->gunSwitchTimer > 20000) {
            this->gunSwitchTimer = 0;
            if (this->player->gunAvailable(1)) {
                this->field_0x140 = !this->field_0x140;
            }
        }

        // Locking a bounty target makes it hostile and uncovers its wanted entry.
        if (!this->player->isAlwaysEnemy() && this->level->getPlayer() &&
            this->level->getPlayer()->field_0x14->lockedEnemy == this && !this->reviveLockFlag) {
            this->player->setAlwaysEnemy(true);
            this->level->uncoverWanted(this->field_0x48);
        }
    }

    if (Globals::status->inBlackMarketSystem() && Globals::status->field_110 && this->shipGroup == 8) {
        this->player->enemyFlagsLo = false;
        this->player->carriesFriendCargoFlag = false;
    }
    if (this->player->turnedEnemy()) {
        this->player->enemyFlagsLo = true;
        this->player->carriesFriendCargoFlag = false;
    }
    if (this->player->isAlwaysFriend()) {
        this->player->enemyFlagsLo = false;
        this->player->carriesFriendCargoFlag = true;
    }

    if (!this->wingmanFlag && !this->KIPlayer::route) {
        this->KIPlayer::route = this->defaultRoute->clone();
    }

    this->trailTick += dt;
    if (this->trailTick > 200) {
        if (this->trail) {
            this->trail->update(this->shipPosition, this->workingPosition);
            this->workingPosition = this->shipPosition;
        }
        this->trailTick = 0;
    }

    this->player->transform = this->geometry->getMatrix();
    if (this->parentGeometry) {
        this->player->transform *= this->parentGeometry->getMatrix();
    }

    // wingmen in formation fly a one-point route next to the ego ship
    if (this->wingmanFlag && this->state != 4 && this->KIPlayer::route) {
        this->tempVector = this->level->getPlayer()->geometry->getPosition();
        if (this->field_0xe4 == 1) {
            switch (this->wingmanCommand) {
                case 2:
                    this->auxVector = this->level->getPlayer()->geometry->getUpVector();
                    this->auxVector *= 2000.0f;
                    this->tempVector += this->auxVector;
                    this->tempVector -= VectorNormalize(this->level->getPlayer()->geometry->getDirection()) * 2000.0f;
                    break;
                case 1:
                    this->auxVector = this->level->getPlayer()->geometry->getRightVector();
                    this->auxVector *= 4000.0f;
                    this->tempVector += this->auxVector;
                    this->tempVector -= VectorNormalize(this->level->getPlayer()->geometry->getDirection()) * 3000.0f;
                    break;
                case 0:
                    this->auxVector = this->level->getPlayer()->geometry->getRightVector();
                    this->auxVector *= 4000.0f;
                    this->tempVector -= this->auxVector;
                    this->tempVector -= VectorNormalize(this->level->getPlayer()->geometry->getDirection()) * 3000.0f;
                    break;
            }
            if (this->KIPlayer::route->length() > 1) {
                int coords[3] = {(int) this->tempVector.x, (int) this->tempVector.y, (int) this->tempVector.z};
                delete this->KIPlayer::route;
                this->KIPlayer::route = nullptr;
                this->KIPlayer::route = new Route(coords, 3);
            } else {
                this->KIPlayer::route->setNewCoords(this->tempVector);
            }
            this->KIPlayer::route->setLoop(true);
        }
    }

    if (this->state != 3 && this->state != 4) {
        this->player->update(dt, this->fov() != -1 && this->state != 5);
        Array<Player *> *enemies = this->player->getEnemies();
        if (this->aiDisabled) {
            goto commandRoute;
        }
        if (enemies) {
            if ((uint32_t) this->field_0x38 >= enemies->size()) {
                this->field_0x38 = -1;
            }
            if (!this->targetIsActive) {
                this->field_0x38 = -1;
            } else if (this->field_0x38 >= 0 && !(*enemies)[this->field_0x38]->isActive()) {
                this->targetIsActive = 0;
            }

            this->targetPlayer = nullptr;
            if (this->behaviourChangeTick > 5000) {
                this->field_0x12d = this->field_0x12d ? false : Globals::rnd->nextInt(100) < 20;
                this->behaviourChangeTick = 0;
                // occasionally pick a random enemy in sight
                if (Globals::rnd->nextInt(100) < 30 && enemies->size() > 1) {
                    this->targetIsActive = 0;
                    for (int i = 0; i < 5 && !this->targetIsActive; ++i) {
                        this->field_0x38 = Globals::rnd->nextInt(enemies->size());
                        if (!(*enemies)[this->field_0x38]->isActive()) {
                            continue;
                        }
                        this->tempVector = (*enemies)[this->field_0x38]->getPosition();
                        if ((this->shipGroup != 8 && this->player->turnedEnemy()) ||
                            (this->shipPosition.x - this->tempVector.x < this->field_0x128 &&
                             this->shipPosition.x - this->tempVector.x > -this->field_0x128 &&
                             this->shipPosition.y - this->tempVector.y < this->field_0x128 &&
                             this->shipPosition.y - this->tempVector.y > -this->field_0x128 &&
                             this->shipPosition.z - this->tempVector.z < this->field_0x128 &&
                             this->shipPosition.z - this->tempVector.z > -this->field_0x128)) {
                            this->targetIsActive = 1;
                            break;
                        }
                    }
                }
                if (!this->targetIsActive) {
                    this->field_0x38 = 0;
                }
                // wingman ordered to attack a specific ship
                if (this->wingmanFlag && this->field_0xe4 == 3 && this->wingmanTarget) {
                    if (this->wingmanTarget->isDead()) {
                        this->setWingmanCommand(1, nullptr);
                    } else {
                        for (uint32_t i = 0; i < enemies->size(); ++i) {
                            if ((*enemies)[i] == this->wingmanTarget->player && (*enemies)[i]->isActive() &&
                                !(*enemies)[i]->isAlwaysFriend()) {
                                this->field_0x38 = i;
                                break;
                            }
                        }
                    }
                }
                if ((*enemies)[this->field_0x38]->isActive() && !(*enemies)[this->field_0x38]->isDead()) {
                    this->tempVector = (*enemies)[this->field_0x38]->getPosition();
                    if (!(this->shipPosition.x - this->tempVector.x < this->field_0x128 &&
                          this->shipPosition.x - this->tempVector.x > -this->field_0x128 &&
                          this->shipPosition.y - this->tempVector.y < this->field_0x128 &&
                          this->shipPosition.y - this->tempVector.y > -this->field_0x128 &&
                          this->shipPosition.z - this->tempVector.z < this->field_0x128 &&
                          this->shipPosition.z - this->tempVector.z > -this->field_0x128)) {
                        this->field_0x38 = -1;
                    }
                } else {
                    this->field_0x38 = -1;
                    this->targetIsActive = 0;
                }
            } else if (!this->targetIsActive) {
                // first enemy in sight
                for (uint32_t i = 0; i < enemies->size(); ++i) {
                    if (!(*enemies)[i] || !(*enemies)[i]->isActive() || (*enemies)[i]->isDead()) {
                        continue;
                    }
                    this->tempVector = (*enemies)[i]->getPosition();
                    if ((this->shipGroup != 8 && this->player->turnedEnemy()) ||
                        (this->shipPosition.x - this->tempVector.x < this->field_0x128 &&
                         this->shipPosition.x - this->tempVector.x > -this->field_0x128 &&
                         this->shipPosition.y - this->tempVector.y < this->field_0x128 &&
                         this->shipPosition.y - this->tempVector.y > -this->field_0x128 &&
                         this->shipPosition.z - this->tempVector.z < this->field_0x128 &&
                         this->shipPosition.z - this->tempVector.z > -this->field_0x128)) {
                        this->field_0x38 = i;
                        this->targetIsActive = 1;
                        break;
                    }
                }
            }

            // a friendly fighter never targets the ego ship (enemy 0)
            if (!this->player->enemyFlagsLo && this->field_0x38 == 0) {
                this->field_0x38 = 1;
                this->targetIsActive = 0;
            }
            if (!(this->wingmanFlag && this->field_0xe4 == 3) && this->field_0x38 > 0) {
                this->field_0x38 = -1;
                for (uint32_t i = 1; i < enemies->size(); ++i) {
                    if (!(*enemies)[i]->isActive() || (*enemies)[i]->isDead()) {
                        continue;
                    }
                    if (!(*enemies)[i]->getKIPlayer() && !(*enemies)[i]->egoFlag) {
                        continue;
                    }
                    int otherRace = (*enemies)[i]->egoFlag ? 0 : (*enemies)[i]->getKIPlayer()->shipGroup;
                    bool hostile;
                    if (this->wingmanFlag) {
                        hostile = (*enemies)[i]->enemyFlagsLo ||
                                  (this->wingmanTarget && (*enemies)[i] == this->wingmanTarget->player);
                    } else {
                        int race = this->shipGroup;
                        hostile = (otherRace == 8 && race != 8) || (otherRace != 8 && race == 8) ||
                                  (otherRace == 9 && race != 9) || (otherRace != 9 && race == 9) ||
                                  (otherRace == 0 && race == 1) || (otherRace == 1 && race == 0) ||
                                  (otherRace == 3 && race == 2) || (otherRace == 2 && race == 3) ||
                                  (otherRace == 10 && race != 10) || (otherRace != 10 && race == 10);
                    }
                    if (hostile) {
                        this->field_0x38 = i;
                        this->targetIsActive = 1;
                        break;
                    }
                }
            }
            if (this->wingmanFlag && !this->targetIsActive && this->field_0xe4 == 1) {
                this->field_0x38 = -1;
            }

            this->field_0x12c = false;
            if ((this->field_0x38 == -1 && this->KIPlayer::route) || this->player->doesNeverAttack()) {
                if (this->KIPlayer::route->getWaypoint() && this->player->isActive()) {
                    this->KIPlayer::route->update(this->geometry->getPosition());
                    if (this->KIPlayer::route->getWaypoint()) {
                        if (!this->KIPlayer::route->getDockingTarget()) {
                            this->targetPos.x = (float) this->KIPlayer::route->getWaypoint()->x;
                            this->targetPos.y = (float) this->KIPlayer::route->getWaypoint()->y;
                            this->targetPos.z = (float) this->KIPlayer::route->getWaypoint()->z;
                        } else {
                            if (this->state == 1) {
                                this->state = 7;
                            }
                            if (this->state == 7) {
                                // approach the nearest free navigation point of the docking target
                                KIPlayer *dock = this->KIPlayer::route->getDockingTarget();
                                SpacePoint *nav = dock->getNearestNavigationPoint(this->getPosition(), this->spacePoint);
                                if (nav) {
                                    if (this->spacePoint != nav) {
                                        if (this->spacePoint) {
                                            this->spacePoint->giveFree();
                                        }
                                        this->spacePoint = nav;
                                        nav->take();
                                    }
                                    this->auxVector = this->KIPlayer::route->getDockingTarget()->getPosition() +
                                                          MatrixRotateVector(this->KIPlayer::route->getDockingTarget()->geometry->getMatrix(), nav->position);
                                    if ((int) VectorLength(this->auxVector - this->getPosition()) >= 2000) {
                                        this->targetPos = this->auxVector;
                                    } else {
                                        // close enough: ease into the docking point
                                        this->state = 8;
                                        delete this->easeMatrix;
                                        this->easeMatrix = nullptr;
                                        KIPlayer *target = this->KIPlayer::route->getDockingTarget();
                                        SpacePoint *dockPoint = target->getNearestDockingPoint(
                                                this->KIPlayer::route->getDockingTarget()->getPosition() +
                                                MatrixRotateVector(this->KIPlayer::route->getDockingTarget()->geometry->getMatrix(), this->spacePoint->position));
                                        Vector dockDir = VectorNormalize(MatrixRotateVector(
                                                this->KIPlayer::route->getDockingTarget()->geometry->getMatrix(), dockPoint->direction));
                                        Vector navPos = MatrixRotateVector(this->KIPlayer::route->getDockingTarget()->geometry->getMatrix(), nav->position);
                                        Vector dockPos = MatrixRotateVector(this->KIPlayer::route->getDockingTarget()->geometry->getMatrix(), dockPoint->position);
                                        Vector up = VectorNormalize(navPos - dockPos);
                                        Vector right = VectorNormalize(VectorCross(up, dockDir));
                                        Matrix dockMatrix;
                                        MatrixSetRotation(dockMatrix, right, up, dockDir);
                                        this->easeMatrix = new AbyssEngine::EaseInOutMatrix(this->player->transform, dockMatrix, 3000);
                                    }
                                }
                            }
                        }
                        this->field_0x12c = true;
                    }
                } else {
                    this->field_0x38 = 0;
                    this->targetPlayer = this->player->getEnemy(0);
                    if (this->targetPlayer) {
                        this->tempVector = this->targetPlayer->getPosition();
                        this->targetPos.x = this->tempVector.x;
                        this->targetPos.y = this->tempVector.y;
                        this->targetPos.z = this->tempVector.z;
                    }
                }
            } else {
                if (this->field_0x38 < 0 || (uint32_t) this->field_0x38 >= this->player->getEnemies()->size()) {
                    this->field_0x38 = 0;
                }
                this->targetPlayer = this->player->getEnemy(this->field_0x38);
                this->tempVector = this->targetPlayer->getPosition();
                this->targetPos.x = this->tempVector.x;
                this->targetPos.y = this->tempVector.y;
                this->targetPos.z = this->tempVector.z;
            }
        } else if (this->KIPlayer::route) {
            this->KIPlayer::route->update(this->geometry->getPosition());
            if (this->KIPlayer::route->getWaypoint()) {
                this->targetPos.x = (float) this->KIPlayer::route->getWaypoint()->x;
                this->targetPos.y = (float) this->KIPlayer::route->getWaypoint()->y;
                this->targetPos.z = (float) this->KIPlayer::route->getWaypoint()->z;
                this->field_0x12c = true;
            }
        } else {
            this->state = 5;
        }

        // jumpers following their route leave the system after 20 s
        if (this->jumperFlag && this->field_0x12c) {
            this->field_0xf0 += dt;
            if (this->field_0xf0 >= 20000) {
                this->field_0x12c = false;
                this->field_0xf0 = 0;
                this->state = 6;
            }
        } else {
            this->field_0xf0 = 0;
        }

    commandRoute:
        // wingman ordered to fly the player's route
        if (this->wingmanFlag && this->field_0xe4 == 2) {
            if (!this->commandRoute) {
                this->setWingmanCommand(1, nullptr);
            } else {
                this->commandRoute->update(this->geometry->getPosition());
                if (this->commandRoute->getCurrent() > this->initialWaypoint) {
                    delete this->commandRoute;
                    this->commandRoute = nullptr;
                    this->setWingmanCommand(1, nullptr);
                } else if (this->commandRoute->getWaypoint()) {
                    this->targetPos.x = (float) this->commandRoute->getWaypoint()->x;
                    this->targetPos.y = (float) this->commandRoute->getWaypoint()->y;
                    this->targetPos.z = (float) this->commandRoute->getWaypoint()->z;
                    this->field_0x12c = true;
                }
            }
        }
    } else {
        this->player->StopEngineSound();
    }

    this->distanceToTarget.x = this->targetPos.x - this->shipPosition.x;
    this->distanceToTarget.y = this->targetPos.y - this->shipPosition.y;
    this->distanceToTarget.z = this->targetPos.z - this->shipPosition.z;

    // sleeping enemies wake up when the ego ship (or its guided rocket) comes near
    if (this->player->enemyFlagsLo && (this->state == 5 || (this->wingmanFlag && this->field_0xe4 == 1)) &&
        this->field_0x128 > 0) {
        if (this->level->getPlayer()->isInRocketControl()) {
            this->tempVector = this->level->getPlayer()->getTargetFollowCamera()->getTarget()->getPosition();
        } else {
            this->tempVector = this->level->getPlayer()->player->getPosition();
        }
        this->distanceToTarget.x = this->tempVector.x - this->shipPosition.x;
        this->distanceToTarget.y = this->tempVector.y - this->shipPosition.y;
        this->distanceToTarget.z = this->tempVector.z - this->shipPosition.z;
        if (this->distanceToTarget.x < 25000.0f && this->distanceToTarget.x > -25000.0f &&
            this->distanceToTarget.y < 25000.0f && this->distanceToTarget.y > -25000.0f &&
            this->distanceToTarget.z < 25000.0f && this->distanceToTarget.z > -25000.0f) {
            this->state = 1;
            (this->parentGeometry ? this->parentGeometry : this->geometry)->setVisible(true);
            this->player->setActive(true);
        }
    }

    if (this->boundingVolumes) {
        for (uint32_t i = 0; i < this->boundingVolumes->size(); ++i) {
            (*this->boundingVolumes)[i]->update(this->shipPosition.x, this->shipPosition.y,
                                                this->shipPosition.z);
        }
    }

    {
        int hp = this->player->getHitpoints();
        float hpFloat = (float) hp;

        // smoke trail below a third of the hull
        if (!this->smoking && (float) this->player->getMaxHitpoints() * 0.33f > hpFloat) {
            if (reinterpret_cast<const float &>(Globals::options[40]) > 0.0f) {
                if (this->isDocked()) {
                    this->level->particleEmitBoolPtr->enableSystemEmit(this->field_0x80, false);
                    this->level->particleRenderBoolPtr->enableSystemEmit(this->field_0x84, false);
                } else {
                    this->level->particleEmitBoolPtr->enableSystemEmit(this->field_0x80, true);
                    this->level->particleRenderBoolPtr->enableSystemEmit(this->field_0x84, true);
                }
            }
            this->smoking = true;
        } else if (this->smoking && (float) this->player->getMaxHitpoints() * 0.33f <= hpFloat) {
            this->smoking = false;
            this->level->particleEmitBoolPtr->enableSystemEmit(this->field_0x80, false);
            this->level->particleRenderBoolPtr->enableSystemEmit(this->field_0x84, false);
        }

        if (hp <= 0 && this->state != 3 && this->state != 4) {
            // destroyed
            Mission *freelance = Globals::status->getFreelanceMission();
            if (freelance && freelance->getType() == 13 &&
                Globals::status->getStation()->getIndex() == freelance->getTargetStation() &&
                !Globals::status->field_0xf1) {
                if (this->name.length != 0 && !this->name.Compare(*Globals::gameText->getText(1663))) {
                    Globals::status->field_0xf0 = 1;
                } else {
                    Globals::status->field_0xf0 = 0;
                    Globals::status->field_0xf1 = 1;
                }
            }
            if (this->field_0x42) {
                // bounty collected
                (*Globals::status->wanted)[this->field_0x48]->setTerminated(true);
                (*Globals::status->wanted)[this->field_0x48]->setActive(false);
                Globals::layout->showMissionRewardMessage((*Globals::status->wanted)[this->field_0x48]->getReward(), true);
                Globals::status->setCredits(Globals::status->getCredits() +
                                            (*Globals::status->wanted)[this->field_0x48]->getReward());
                Globals::status->incCollectedBounties((*Globals::status->wanted)[this->field_0x48]->getBoard());
            }
            if (this->player->enemyFlagsLo) {
                if (this->shipGroup == 8) {
                    if (!this->player->destroyedByte) {
                        Globals::status->incPirateKills();
                    }
                } else if (this->shipGroup == 9) {
                    this->cargo = new Array<int>();
                    ArrayAdd(131, *this->cargo);
                    ArrayAdd(Globals::rnd->nextInt(3) + 1, *this->cargo);
                } else if (!this->carriesMissionCrate && this->crateCaptured() && Globals::options[53] &&
                           Globals::rnd->nextInt(100) < 0 && this->shipGroup <= 3) {
                    delete this->cargo;
                    this->cargo = nullptr;
                    this->cargo = new Array<int>();
                    ArrayAdd(this->shipGroup + 189, *this->cargo);
                    ArrayAdd(1, *this->cargo);
                }
                this->level->enemyDied(this->type, this->player->destroyedByte);
            } else {
                this->level->friendDied();
                if (this->wingmanFlag) {
                    this->level->wingmanDied(this->name);
                }
            }
            this->field_0x73 = 0;
            this->state = 3;
            this->dyingTimer = Globals::rnd->nextInt(1500) + 1500;
            this->dyingDirection = this->geometry->getDirection();
            this->level->field_74->emitManual(this->level->field_3c, this->shipPosition, 0, Vector(), -1.0f);
            Globals::sound->play(20, Globals::options[15] ? &this->shipPosition : nullptr, nullptr, 0.0f);
            float x = (float) (Globals::rnd->nextInt(200) - 100);
            float y = (float) (Globals::rnd->nextInt(200) - 100);
            float z = (float) (Globals::rnd->nextInt(200) - 100);
            this->dyingSpin = VectorNormalize(Vector{x, y, z});
            this->dyingSpin *= 0.05f;
            this->level->field_74->enableSystemEmit(this->engineTrailSystem, true);
            this->level->field_8c->enableSystemEmit(this->field_0x130, false);
            this->level->field_8c->enableSystemEmit(this->field_0x134, false);
            this->level->field_8c->enableSystemEmit(this->field_0x138, false);
            this->crateCaptured() = this->cargoAvailable();
            this->setExhaustVisible(false);
        }

        if (this->aiDisabled) {
            this->geometry->updateReferenceMatrix();
            this->handleCloaking();
            return;
        }

        switch (this->state) {
            case 0:
                this->state = 1;
                return;
            case 1:
            case 7:
                this->handleCloaking();
                if (this->field_0x13d) {
                    // heavy damage makes the fighter boost away
                    if (this->hitpoints > hp) {
                        this->damageAccumulator += this->hitpoints - hp;
                        this->hitpoints = hp;
                        if ((float) this->damageAccumulator / (float) this->player->getMaxHitpoints() * 100.0f > 40.0f) {
                            this->damageAccumulator = 0;
                            this->boostTick = 10000;
                            this->damageEscape = true;
                        }
                    }
                    if (this->boostTick > 5000 && !this->boosting) {
                        this->boostTick = 0;
                        if (this->damageEscape || Globals::rnd->nextInt(100) < this->boostProb) {
                            this->boostTimeLimit = Globals::rnd->nextInt(3000) + 5000;
                            this->boosting = true;
                            this->targetSpeed = 5.5f;
                            this->currentRotate = 1.3f;
                        }
                    }
                    if (this->boosting) {
                        if (this->boostTick > this->boostTimeLimit) {
                            this->boostTick = 0;
                            this->damageEscape = false;
                            this->targetSpeed = this->speed;
                            this->currentRotate = this->rotate;
                        }
                        if (this->targetSpeed > 0.0f) {
                            this->currentSpeed *= this->targetSpeed > this->currentSpeed ? 1.05f : 0.95f;
                            if (this->currentSpeed >= 5.5f || this->currentSpeed < this->speed) {
                                this->currentSpeed = this->targetSpeed;
                                if (this->targetSpeed == this->speed) {
                                    this->boosting = false;
                                }
                                this->targetSpeed = 0.0f;
                            }
                        }
                    }
                }

                // too close to the target: veer off sideways
                if (this->targetPlayer && !this->field_0x12c) {
                    int range = this->targetPlayer->egoFlag && this->level->getPlayer() &&
                                        this->level->getPlayer()->isDockedToDockingPoint() ? 12000 : 8000;
                    if (this->distanceToTarget.x < range && this->distanceToTarget.x > -range &&
                        this->distanceToTarget.y < range && this->distanceToTarget.y > -range &&
                        this->distanceToTarget.z < range && this->distanceToTarget.z > -range) {
                        this->distanceToTarget = this->geometry->getRightVector();
                    }
                }
                {
                    Vector toTarget = VectorNormalize(this->distanceToTarget);
                    this->distanceToTarget = MatrixRotateVector(MatrixGetInverse(this->player->transform), toTarget);
                    this->distanceToTarget.y = -this->distanceToTarget.y;
                    this->field_0x24 = this->player->empDisabledByte;

                    if (this->targetIsActive && !this->field_0x12c && this->targetPlayer &&
                        !this->player->doesNeverAttack()) {
                        if (this->targetPlayer->field_5e) {
                            this->targetIsActive = 0;
                            if (this->isShooting) {
                                this->player->stopShooting(this->field_0x140);
                                this->isShooting = false;
                            }
                        } else if (this->distanceToTarget.x < this->shootError &&
                                   this->distanceToTarget.x > -this->shootError &&
                                   this->distanceToTarget.y < this->shootError &&
                                   this->distanceToTarget.y > -this->shootError &&
                                   this->targetPos.x - this->shipPosition.x < 35000.0f &&
                                   this->targetPos.x - this->shipPosition.x > -35000.0f &&
                                   this->targetPos.y - this->shipPosition.y < 35000.0f &&
                                   this->targetPos.y - this->shipPosition.y > -35000.0f &&
                                   this->targetPos.z - this->shipPosition.z < 35000.0f &&
                                   this->targetPos.z - this->shipPosition.z > -35000.0f) {
                            if (this->field_0x25 && this->targetPlayer->isActive() && !this->targetPlayer->isDead() &&
                                !(this->targetPlayer->egoFlag && this->level->getPlayer() &&
                                  this->level->getPlayer()->isDockedToDockingPoint() &&
                                  this->targetPlayer->getPosition().y > this->player->getPosition().y)) {
                                this->player->shoot(this->field_0x140, (long long) dt, false);
                                this->isShooting = true;
                            } else {
                                this->targetIsActive = 0;
                            }
                        }
                    }

                    Matrix matrix = this->geometry->getMatrix();
                    if (this->field_0x12d) {
                        this->targetRoll = 0.0f;
                        this->rollSampleIndex = 0;
                        this->rollBufferFilled = false;
                    } else {
                        // turn towards the target by at most dt * 48 / 65536 per frame
                        Vector wanted = toTarget;
                        Vector dir = MatrixGetDir(matrix);
                        toTarget -= dir;
                        toTarget = VectorNormalize(toTarget);
                        toTarget *= (float) (dt * 48) / 65536.0f;
                        this->distanceToTarget = toTarget + dir;
                        this->distanceToTarget = VectorNormalize(this->distanceToTarget);
                        float dx = this->distanceToTarget.x - wanted.x;
                        float dy = this->distanceToTarget.y - wanted.y;
                        float dz = this->distanceToTarget.z - wanted.z;
                        if ((dx > 0.0f ? dx : -dx) + (dy > 0.0f ? dy : -dy) + (dz > 0.0f ? dz : -dz) < 0.0625f) {
                            this->distanceToTarget = wanted;
                        }

                        // bank into the turn: average of the last turn angles
                        float dot = VectorDot(VectorNormalize(dir), this->distanceToTarget);
                        float angle = 0.0f;
                        if (dot < 1.0f && dot > -1.0f) {
                            angle = ACosf(dot);
                        }
                        float turn = angle > 0.0f ? angle : -angle;
                        if (turn != 0.0f) {
                            float negTurn = -turn;
                            if (ACosf(VectorDot(MatrixGetRight(matrix), Vector(this->distanceToTarget))) < 1.57079637f) {
                                turn = negTurn;
                            }
                        }
                        this->rollSamples[this->rollSampleIndex] = turn;
                        if (this->rollBufferFilled) {
                            float sum = 0.0f;
                            for (int i = 0; i < 5; ++i) {
                                sum += this->rollSamples[i];
                            }
                            turn = sum / 5.0f;
                        } else if (this->rollSampleIndex != 0) {
                            float sum = 0.0f;
                            for (int i = 0; i < this->rollSampleIndex; ++i) {
                                sum += this->rollSamples[i];
                            }
                            turn = sum / (float) this->rollSampleIndex;
                        }
                        if (++this->rollSampleIndex >= 5) {
                            this->rollSampleIndex = 0;
                            if (!this->rollBufferFilled) {
                                this->rollBufferFilled = true;
                            }
                        }
                        this->targetRoll = turn * this->maxRoll * this->rollFactor;
                        if (this->targetRoll > this->maxRoll) {
                            this->targetRoll = this->maxRoll;
                        } else if (this->targetRoll < -this->maxRoll) {
                            this->targetRoll = -this->maxRoll;
                        }

                        Vector right = VectorCross(MatrixGetUp(matrix), this->distanceToTarget);
                        right = VectorNormalize(right);
                        Vector up = VectorCross(this->distanceToTarget, right);
                        up = VectorNormalize(up);
                        matrix = MatrixSetRotation(matrix, right, up, this->distanceToTarget);
                    }

                    if (this->rollActive()) {
                        this->roll(dt);
                    }
                    if (this->smoothRoll != this->targetRoll) {
                        float step = (float) dt * 1.25f / 3.8999999f;
                        if (this->smoothRoll < this->targetRoll) {
                            this->smoothRoll += step;
                            if (this->smoothRoll > this->targetRoll) {
                                this->smoothRoll = this->targetRoll;
                            }
                        } else if (this->smoothRoll > this->targetRoll) {
                            this->smoothRoll -= step;
                            if (this->smoothRoll < this->targetRoll) {
                                this->smoothRoll = this->targetRoll;
                            }
                        }
                    }
                    if ((int) this->field_0xfc > 0) {
                        this->field_0xfc -= dt;
                        if ((int) this->field_0xfc <= 0) {
                            this->rollActive() = 1;
                        }
                    }
                    if (this->targetRoll == this->smoothRoll) {
                        if ((int) this->field_0xfc > 0) {
                            this->field_0xfc -= dt;
                            if ((int) this->field_0xfc <= 0) {
                                this->rollActive() = 1;
                            }
                        }
                    } else {
                        this->field_0xfc = 750;
                    }

                    Matrix bank;
                    bank = MatrixSetRotation(bank, 0.0f, 0.0f, this->smoothRoll / 4096.0f * 3.14159274f);
                    if (this->parentGeometry) {
                        this->parentGeometry->setMatrix(bank);
                    }
                    if (this->player->empDisabledByte) {
                        // stunned: drift, engines sparking
                        this->level->field_8c->enableSystemEmit(this->field_0x130, true);
                        this->level->field_8c->enableSystemEmit(this->field_0x134, true);
                        this->level->field_8c->enableSystemEmit(this->field_0x138, true);
                    } else {
                        float move = (float) (int) (this->currentSpeed * (float) dt);
                        matrix[3] += matrix[2] * move;
                        matrix[7] += matrix[6] * move;
                        matrix[11] += matrix[10] * move;
                        if (this->rollActive()) {
                            matrix *= this->rollMatrix;
                        }
                        Globals::Canvas->TransformSetLocal(this->geometry->transform, matrix);
                        this->player->transform = matrix;
                        if (this->parentGeometry) {
                            this->player->transform *= this->parentGeometry->getMatrix();
                        }
                        this->level->field_8c->enableSystemEmit(this->field_0x130, false);
                        this->level->field_8c->enableSystemEmit(this->field_0x134, false);
                        this->level->field_8c->enableSystemEmit(this->field_0x138, false);
                    }
                    this->geometry->updateReferenceMatrix();
                }

                // steer around landmarks and other ships
                if (this->field_0x13e) {
                    Array<KIPlayer *> *landmarks = this->level->getLandmarks();
                    if (landmarks) {
                        for (uint32_t i = 0; i < landmarks->size(); ++i) {
                            if (!(*landmarks)[i] || !(*landmarks)[i]->outerCollide(this->shipPosition)) {
                                continue;
                            }
                            Vector away = (*landmarks)[i]->getProjectionVector(this->shipPosition);
                            if (away != Vector{0.0f, 0.0f, 0.0f}) {
                                this->tempVector = this->geometry->getDirection();
                                away -= this->tempVector;
                                away *= this->currentSpeed * 0.03f;
                                this->auxVector = this->tempVector + away;
                                this->auxVector = VectorNormalize(this->auxVector);
                                this->geometry->setDirection(this->auxVector, Vector{0.0f, 1.0f, 0.0f});
                                this->geometry->moveForward(this->currentSpeed * (float) dt);
                                break;
                            }
                        }
                    }
                    Array<KIPlayer *> *ships = this->level->getEnemies();
                    if (ships) {
                        for (uint32_t i = 0; i < ships->size(); ++i) {
                            if (!(*ships)[i] || !(*ships)[i]->outerCollide(this->shipPosition)) {
                                continue;
                            }
                            Vector away = (*ships)[i]->getProjectionVector(this->shipPosition);
                            if (away != Vector{0.0f, 0.0f, 0.0f}) {
                                this->tempVector = this->geometry->getDirection();
                                away -= this->tempVector;
                                away *= this->currentSpeed * 0.03f;
                                this->auxVector = this->tempVector + away;
                                this->auxVector = VectorNormalize(this->auxVector);
                                this->geometry->setDirection(this->auxVector, Vector{0.0f, 1.0f, 0.0f});
                                this->geometry->moveForward(this->currentSpeed * (float) dt);
                                break;
                            }
                        }
                    }
                }
                this->push(dt);
                return;
            case 2:
                return;
            case 3: {
                // tumbling before the final explosion
                this->field_0x13e = 0;
                this->field_0x101 = 0;
                Matrix spin;
                MatrixSetRotation(spin, this->dyingSpin.x, this->dyingSpin.y, this->dyingSpin.z);
                if (dt > 0) {
                    this->geometry->setMatrix(this->geometry->getMatrix() * spin);
                }
                this->geometry->translate(this->dyingDirection * (float) dt * this->currentSpeed);
                this->geometry->updateReferenceMatrix();
                this->dyingTimer -= dt;
                if (this->dyingTimer >= 0) {
                    return;
                }
                this->explosion->start(this->shipPosition, Vector{0.0f, 0.0f, 0.0f});
                this->level->field_74->enableSystemEmit(this->engineTrailSystem, false);
                if (reinterpret_cast<const float &>(Globals::options[40]) > 0.0f) {
                    this->level->particleEmitBoolPtr->enableSystemEmit(this->field_0x80, false);
                    this->level->particleRenderBoolPtr->enableSystemEmit(this->field_0x84, false);
                }
                this->player->setBombForce((float) Globals::rnd->nextInt(50) * 0.01f + 50.0f);
                float x = (float) (Globals::rnd->nextInt(200) - 100);
                float y = (float) (Globals::rnd->nextInt(200) - 100);
                float z = (float) (Globals::rnd->nextInt(200) - 100);
                this->field_0x17c = VectorNormalize(Vector{x, y, z});
                this->state = 4;
                this->dyingTimer = 0;
                if (this->spacePoint && !this->spacePoint->isFree()) {
                    this->spacePoint->giveFree();
                    this->spacePoint = nullptr;
                }
                if (this->crateCaptured()) {
                    if (this->carriesMissionCrate && !this->missionCrateCaptured()) {
                        this->missionCrateLost() = 1;
                        this->crateCaptured() = 0;
                    } else {
                        this->createCrate(this->shipGroup == 9 ? 4 : 0);
                    }
                }
                return;
            }
            case 4: {
                this->dyingTimer += dt;
                if (this->dyingTimer > 0) {
                    this->field_0x73 = 0;
                } else {
                    Matrix spin;
                    MatrixSetRotation(spin, this->dyingSpin.x, this->dyingSpin.y, this->dyingSpin.z);
                    if (dt > 0) {
                        this->geometry->setMatrix(this->geometry->getMatrix() * spin);
                    }
                    this->geometry->translate(this->dyingDirection * (float) dt * this->currentSpeed);
                    this->geometry->updateReferenceMatrix();
                }
                this->geometry->updateReferenceMatrix();
                this->explosion->update(dt, this->level->getPlayer() ? this->level->getPlayer()->getTargetFollowCamera() : nullptr);
                this->deathTimer() += dt;
                if (this->crateCaptured() && this->player->isActive() && dt > 0 && this->wreckGeometry()) {
                    // the cargo crate drifts away from the wreck
                    float bombForce = this->player->getBombForce();
                    if (bombForce > 0.0f) {
                        this->tempVector = this->field_0x17c * bombForce;
                        this->wreckGeometry()->translate(this->tempVector);
                        this->geometry->translate(this->tempVector);
                        bombForce *= 0.98f;
                        if (bombForce < 0.05f) {
                            bombForce = 0.0f;
                        }
                        this->player->setBombForce(bombForce);
                    }
                    float angle = (float) (int) ((float) (dt / 2) / 65536.0f * 6.28318548f);
                    this->wreckGeometry()->rotate(angle, angle, angle);
                    this->player->transform = this->wreckGeometry()->getMatrix();
                    if (this->deathTimer() <= 60000 || this->explosion->isPlaying()) {
                        return;
                    }
                    if (this->level->getPlayer() && this->level->getPlayer()->field_0x14 &&
                        this->level->getPlayer()->field_0x14->field_0x1c == this) {
                        this->level->getPlayer()->field_0x14->field_0x1c = nullptr;
                    }
                    if (this->carriesMissionCrate && !this->missionCrateCaptured()) {
                        this->missionCrateLost() = 1;
                    }
                    delete this->wreckGeometry();
                    this->wreckGeometry() = nullptr;
                    this->deathTimer() = 0;
                } else {
                    if (this->explosion->isPlaying()) {
                        return;
                    }
                    if (this->carriesMissionCrate && !this->missionCrateCaptured()) {
                        this->missionCrateLost() = 1;
                    }
                    if (this->deathTimer() > 60000) {
                        this->setActive(false);
                    }
                    this->field_0x101 = 1;
                    return;
                }
                this->setActive(false);
                this->field_0x101 = 1;
                return;
            }
            case 5:
                // asleep: hidden from far away, wakes when the target comes into sight
                if (this->player->enemyFlagsLo && Globals::status->getCurrentCampaignMission() > 1) {
                    (this->parentGeometry ? this->parentGeometry : this->geometry)->setVisible(false);
                }
                if (this->targetPlayer && !this->targetPlayer->field_5e &&
                    this->distanceToTarget.x < this->field_0x128 && this->distanceToTarget.x > -this->field_0x128 &&
                    this->distanceToTarget.y < this->field_0x128 && this->distanceToTarget.y > -this->field_0x128 &&
                    this->distanceToTarget.z < this->field_0x128 && this->distanceToTarget.z > -this->field_0x128) {
                    this->state = 1;
                    (this->parentGeometry ? this->parentGeometry : this->geometry)->setVisible(true);
                    this->player->setActive(true);
                    if (this->field_0x12f) {
                        this->level->pirateStationAction(true);
                    }
                }
                return;
            case 6:
                // jumping out
                this->currentSpeed *= 1.1f;
                this->geometry->moveForward((float) (int) (this->currentSpeed * (float) dt));
                if (this->currentSpeed > 100.0f) {
                    this->setDead();
                }
                return;
            case 8: {
                // easing into the docking point
                KIPlayer *dock = this->KIPlayer::route->getDockingTarget();
                SpacePoint *dockPoint = dock->getNearestDockingPoint(
                        this->KIPlayer::route->getDockingTarget()->getPosition() +
                        MatrixRotateVector(this->KIPlayer::route->getDockingTarget()->geometry->getMatrix(), this->spacePoint->position));
                this->auxVector = this->KIPlayer::route->getDockingTarget()->getPosition() +
                                      MatrixRotateVector(this->KIPlayer::route->getDockingTarget()->geometry->getMatrix(), dockPoint->position);
                if (kShipDockRadius[this->type] > (int) VectorLength(this->auxVector - this->getPosition())) {
                    this->setExhaustVisible(false);
                    this->dockTimer = 0;
                    this->passengersMoved = 0;
                    this->state = 9;
                    if (this->smoking) {
                        this->level->particleEmitBoolPtr->enableSystemEmit(this->field_0x80, false);
                        this->level->particleRenderBoolPtr->enableSystemEmit(this->field_0x84, false);
                    }
                    return;
                }
                float step = (float) dt;
                this->easeMatrix->Increase(step);
                Matrix eased = this->easeMatrix->GetValue();
                MatrixSetTranslation(eased, this->getPosition() + (this->auxVector - this->getPosition()) * step * 0.0006f);
                this->geometry->setMatrix(eased);
                this->player->transform = eased;
                return;
            }
            case 9: {
                // docked: unload passengers, then undock after the docking time
                Mission *mission = Globals::status->getMission();
                this->dockTimer += dt;
                if (!mission->isEmpty() && mission->getType() == 184 &&
                    ((PlayerFixedObject *) this->KIPlayer::route->getDockingTarget())->getDockingType() == 1) {
                    int moved = this->passengersMoved;
                    this->passengersMoved = (int) ((float) this->dockTimer / (this->shipGroupFlag == 51 ? 200.0f : 1500.0f));
                    if ((Globals::status->getCurrentCampaignMission() != 94 ||
                         mission->getStatusValue() > Globals::status->getShip()->getMaxPassengers()) &&
                        this->passengersMoved > moved &&
                        this->level->getNumDeliveredPassengers() < mission->getProductionGoodAmount()) {
                        mission->setStatusValue(mission->getStatusValue() + moved - this->passengersMoved);
                        if (mission->getStatusValue() < 0) {
                            mission->setStatusValue(0);
                        }
                    }
                }
                if (this->dockTimer > this->KIPlayer::route->getDockingTime()) {
                    this->setExhaustVisible(true);
                    this->KIPlayer::route->reachWaypoint(this->KIPlayer::route->getCurrent());
                    this->state = 1;
                    if (this->smoking) {
                        this->level->particleEmitBoolPtr->enableSystemEmit(this->field_0x80, true);
                        this->level->particleRenderBoolPtr->enableSystemEmit(this->field_0x84, true);
                    }
                    if (this->spacePoint) {
                        this->spacePoint->giveFree();
                        this->spacePoint = nullptr;
                    }
                }
                return;
            }
        }
    }
}
void PlayerFighter::roll(int angle) {
    if (this->rollActive() == 0) {
        return;
    }
    AbyssEngine::AEMath::Matrix &m = this->geometry->getMatrix();
    float rightX = m.m[4];
    float rightY = m.m[5];
    float absX = rightX > 0.0f ? rightX : -rightX;
    if (angle >= 60) {
        angle = 60;
    }
    float bank; // like the original, left unset when no case below matches (rightX == 0)
    if (rightY > 0.0f && absX < 0.015f) {
        this->rollMatrix = AbyssEngine::AEMath::MatrixIdentity(this->rollMatrix);
        this->rollActive() = 0;
        this->field_0x254 = 0;
        this->field_0x255 = 0;
        return;
    }

    if (this->field_0x254 != 0) {
        if (rightX < 0.0f && (rightY < 0.0f || rightY >= 0.0f)) {
            bank = 0.0002f;
        } else {
            bank = -0.0002f;
        }
    } else if (rightX > 0.0f && this->field_0x255 == 1) {
        this->field_0x254 = 1;
        bank = -0.00035f;
    } else if (rightX < 0.0f && this->field_0x255 == 2) {
        this->field_0x254 = 1;
        bank = 0.00035f;
    } else if (rightX < 0.0f && rightY < 0.0f) {
        bank = 0.00075f;
    } else if (rightX < 0.0f && rightY >= 0.0f) {
        bank = rightX > -0.3f ? 0.00025f : 0.00035f;
    } else if (rightX >= 0.0f && rightY < 0.0f) {
        bank = -0.00075f;
    } else if (rightX > 0.0f && rightY > 0.0f) {
        if (rightX <= 0.3f) {
            bank = -0.00025f;
        } else {
            bank = -0.00035f;
        }
    }

    if (rightX < 0.0f || rightX > 0.0f) {
        this->field_0x255 = rightX < 0.0f ? 1 : 2;
    }
    this->rollActive() = 1;
    this->rollMatrix = AbyssEngine::AEMath::MatrixSetRotation(this->rollMatrix, 0.0f, 0.0f, bank * (float) angle);
}
void PlayerFighter::render() {
    if (this->wreckGeometry() != 0) {
        this->wreckGeometry()->render();
    }
    bool active = this->player->isActive();
    if (active ? (this->state != 3 && this->state != 4) : this->state == 5) {
        if (this->parentGeometry != nullptr) {
            Matrix saved = *(Matrix *) Globals::Canvas->TransformGetLocal(this->parentGeometry->transform);
            Globals::Canvas->TransformSetLocal(this->parentGeometry->transform,
                                               this->player->transform);
            this->parentGeometry->render();
            Globals::Canvas->TransformSetLocal(this->parentGeometry->transform, saved);
        } else {
            this->KIPlayer::geometry->render();
        }
        if (this->trail != 0) {
            this->trail->render();
        }
    } else if (active) {
        // dying (3) / exploding (4): explosion plus the sub-geometry (cockpit/debris)
        if (this->explosion != 0) {
            this->explosion->render();
        }
        if (this->state == 3) {
            if (this->parentGeometry == nullptr) {
                this->KIPlayer::geometry->render();
            } else {
                Matrix saved = *(Matrix *) Globals::Canvas->TransformGetLocal(this->parentGeometry->transform);
                Globals::Canvas->TransformSetLocal(this->parentGeometry->transform,
                                                   this->player->transform);
                this->parentGeometry->render();
                Globals::Canvas->TransformSetLocal(this->parentGeometry->transform, saved);
            }
        } else if (this->state == 4 && this->dyingTimer < 300) {
            Matrix saved = *(Matrix *) Globals::Canvas->TransformGetLocal(this->parentGeometry->transform);
            Globals::Canvas->TransformSetLocal(this->parentGeometry->transform,
                                               this->player->transform);
            this->parentGeometry->render();
            Globals::Canvas->TransformSetLocal(this->parentGeometry->transform, saved);
        }
    }
}
void PlayerFighter::setBV(Array<BoundingVolume *> *v) {
    this->boundingVolumes = v;
}
void PlayerFighter::setBV(BoundingVolume *bv) {
    Array<BoundingVolume *> *a = new Array<BoundingVolume *>();
    this->boundingVolumes = a;

    ArrayAdd<BoundingVolume *>(bv, *a);
}
int PlayerFighter::collide(float x, float y, float z) {
    if ((unsigned) (this->state - 3) > 1) {
        Array<BoundingVolume *> *a = this->boundingVolumes;
        if (a != 0) {
            for (unsigned i = 0; i < a->size(); i++) {
                BoundingVolume *e = a->data()[i];
                if (e->collide(x, y, z) != 0) {
                    return 1;
                }
                a = this->boundingVolumes;
            }
        }
    }
    return 0;
}
int PlayerFighter::outerCollide(float x, float y, float z) {
    if ((unsigned) (this->state - 3) > 1) {
        Array<BoundingVolume *> *a = this->boundingVolumes;
        if (a != 0) {
            for (unsigned i = 0; i < a->size(); i++) {
                BoundingVolume *e = a->data()[i];
                if (e->outerCollide(x, y, z) != 0) {
                    return 1;
                }
                a = this->boundingVolumes;
            }
        }
    }
    return 0;
}
void PlayerFighter::initPush(const Vector &target, int radius) {
    float t = AbyssEngine::AEMath::VectorLength(target - this->getPosition()) / (float) radius;
    t = t < 1.0f ? t : 1.0f;
    int duration = (int) ((1.0f - t) * gIP_strength);
    this->pushTimer() = duration;
    this->pushDuration() = duration;
    this->pushNormal() = AbyssEngine::AEMath::VectorNormalize(this->getPosition() - target);
    float x = (float) (Globals::rnd->nextInt(200) - 100);
    float y = (float) (Globals::rnd->nextInt(200) - 100);
    float z = (float) (Globals::rnd->nextInt(200) - 100);
    this->pushImpulse() = 0.2f * AbyssEngine::AEMath::VectorNormalize(Vector{x, y, z});
}
void PlayerFighter::push(int dt) {
    if (this->pushTimer() > 0) {
        this->pushTimer() -= dt;
        float frac = (float) this->pushTimer() / (float) this->pushDuration();

        Matrix rot;
        AbyssEngine::AEMath::MatrixSetRotation(rot, frac * this->pushImpulse().x, frac * this->pushImpulse().y,
                                               frac * this->pushImpulse().z);
        if (this->frameTime > 0) {
            AEGeometry *geom = this->geometry;
            geom->setMatrix(geom->getMatrix() * rot);
        }
        float speed = (float) this->frameTime;
        AEGeometry *geom = this->geometry;
        float duration = (float) this->pushDuration();
        geom->translate(this->pushNormal() * speed * this->currentSpeed *
                        ((2.0f - frac) * 3.0f * (duration / gPush_div)));
    }
}
void PlayerFighter::awake() {
    this->state = 1;
    this->player->setActive(1);
    ((PlayerFighter *) (this))->setExhaustVisible(true);
    AEGeometry *geom = this->parentGeometry;
    this->field_0xf5() = 1;
    if (geom == 0) {
        geom = this->geometry;
    }

    return geom->setVisible(true);
}
void PlayerFighter::revive() {
    unsigned char enemy = this->player->turnedEnemy();
    this->player->reset();
    if (enemy != 0) {
        this->player->turnEnemy();
    }
    this->name = String();
    this->state = 1;
    this->wreckGeometry() = 0;
    this->targetIsActive = 0;
    this->field_0x38 = -1;
    this->KIPlayer::route->reset();
    this->hitpoints = this->player->getHitpoints();
    this->damageAccumulator = 0;
    this->damageEscape = 0;
    this->deathTimer() = 0;
    this->setActive(true);
    this->field_0xf0 = 0;
    this->currentSpeed = this->speed;
    this->explosion->reset();
    this->field_0x24 = 0;
    this->pushTimer() = 0;
    this->setExhaustVisible(true);

    this->field_0xf5() = 1;
    (this->parentGeometry != nullptr ? this->parentGeometry : this->KIPlayer::geometry)->setVisible(true);

    if ((unsigned) (this->shipGroup - 9) < 2) {
        delete this->lootList();
        this->lootList() = 0;
    } else {
        Generator *g = new Generator();
        if (this->lootList() != nullptr) {
            delete this->lootList();
            this->lootList() = 0;
        }
        this->lootList() = g->getLootList(-1, -1);
        delete g;
    }
}