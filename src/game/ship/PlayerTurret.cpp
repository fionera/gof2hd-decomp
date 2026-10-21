#include "game/ship/PlayerTurret.h"
#include "engine/core/AERandom.h"
#include "game/core/Globals.h"
#include "engine/render/AEGeometry.h"
#include "engine/audio/FModSound.h"
#include "game/world/Level.h"
#include "engine/render/ParticleSystemManager.h"
#include "engine/math/Transform.h"
#include "game/mission/Explosion.h"
#include "game/ship/KIPlayer.h"
#include "game/ship/Player.h"
#include "game/world/Standing.h"
#include "engine/render/PaintCanvas.h"
#include "game/mission/Status.h"

namespace AbyssEngine {
    namespace AEMath {
        Vector MatrixGetDir(const Matrix &matrix);

        Vector VectorNormalize(const Vector &v);

        float VectorLength(const Vector &v);

        Vector MatrixRotateVector(const Matrix &matrix, const Vector &vector);

        Vector MatrixInverseTransformVector(const Matrix &matrix, const Vector &vector);

        Vector operator+(const Vector &, const Vector &);

        Vector operator-(const Vector &, const Vector &);

        Vector operator*(const Vector &, float);

        Matrix operator*(const Matrix &, const Matrix &);
    }
}


void PlayerTurret::setTurretRange(int range) {
    this->turretRange = range;
}

void PlayerTurret::handleSentryGun(int delta) {
    this->pickEnemyTimer += delta;
    this->pickEnemy();
    Player *enemy = this->currentEnemy;
    if (enemy != nullptr && enemy->field_5e == 0) {
        AEGeometry *geometry = this->geometry;
        this->handleRotation(delta, geometry, geometry);
    }
}

void PlayerTurret::setHost(KIPlayer *host, const Vector &offset) {
    this->turretHost = host;
    this->hostOffset = offset;
}

void PlayerTurret::render() {
    AEGeometry *visible = this->crateGeometry;
    if (visible != nullptr) {
        visible->render();
    }
    int state = this->state;
    if (state == 3) {
        this->explosion->render();
        state = this->state;
    }

    if ((uint32_t)(state - 3) >= 2) {
        this->KIPlayer::render();
    }
}

int PlayerTurret::collide(float x, float y, float z) {
    return 0;
}

int PlayerTurret::outerCollide(float x, float y, float z) {
    return 0;
}

void PlayerTurret::handleTurret(int delta) {
    this->pickEnemyTimer += delta;
    this->pickEnemy();
    Player *enemy = this->currentEnemy;
    if (enemy != nullptr && enemy->field_5e == 0) {
        this->handleRotation(delta, this->helperGeometry, this->turretGeometry);
    }
}

void PlayerTurret::revive() {
    this->player->reset();
    this->state = 1;
    this->crateGeometry = nullptr;
    this->field_0xd8 = 0;
    this->setActive(true);
    this->explosion->reset();
    AEGeometry *geometry = this->parentGeometry;
    this->spawnInvulnTimer = 0;
    this->visibleFlag = 1;
    if (geometry == nullptr) {
        geometry = this->geometry;
    }
    geometry->setVisible(true);
}

void PlayerTurret::setPosition(const Vector &position) {
    this->geometry->setPosition(position);
    this->posX = position.x;
    this->posY = position.y;
    this->posZ = position.z;
}

void PlayerTurret::reset() {
    this->KIPlayer::reset();
    this->state = 0;
    this->setActive(true);
}

void PlayerTurret::setLevel(Level *level) {
    this->KIPlayer::setLevel(level);
    ParticleSystemManager *manager = (ParticleSystemManager *) this->level->field_74;
    int system = manager->addSystem(&this->geometry->getReferenceMatrix(), ParticleSettings::ParticleSet_9, false);
    this->particleSystemId = system;
    ((ParticleSystemManager *) this->level->field_74)->enableSystemEmit(system, false);
}

KIPlayer *PlayerTurret::getHost() {
    return this->turretHost;
}

void PlayerTurret::setScaling(float scale) {
    this->helperGeometry->setScaling(scale, scale, scale);
}

void PlayerTurret::handleRotation(int delta, AEGeometry *mainGeometry, AEGeometry *turretGeometry) {
    // Aim 1500 units ahead of the current enemy along its flight direction.
    Matrix matrix = *(const Matrix *) this->currentEnemy->transform;
    this->aimPoint() = this->currentEnemy->getPosition() + VectorNormalize(MatrixGetDir(matrix)) * 1500.0f;

    if (this->isSentryGun())
        matrix = this->geometry->getMatrix();
    else
        matrix = this->geometry->getMatrix() * mainGeometry->getMatrix() * turretGeometry->getMatrix();
    Vector aim = VectorNormalize(MatrixInverseTransformVector(matrix, this->aimPoint()));

    bool aligned;
    if (aim.x > 0.05f) {
        mainGeometry->rotate(0.0f, (float) delta * 0.00024414062f * 6.2831855f, 0.0f);
        aligned = false;
    } else if (aim.x < -0.05f) {
        mainGeometry->rotate(0.0f, (float) -delta * 0.00024414062f * 6.2831855f, 0.0f);
        aligned = false;
    } else {
        aligned = true;
    }

    if (aim.y > 0.05f) {
        if (!this->isSentryGun() && this->rotationAccum < -599) {
            this->pickEnemyTimer += delta;
            this->previousEnemy = this->currentEnemy;
            return;
        }
        float step = (float) this->lastDelta;
        this->rotationAccum = (int) ((float) this->rotationAccum - step);
        turretGeometry->rotate(step * 0.00024414062f * -6.2831855f, 0.0f, 0.0f);
        this->previousEnemy = nullptr;
    } else if (aim.y < -0.05f) {
        if (!this->isSentryGun() && this->rotationAccum > 99) {
            this->pickEnemyTimer += delta;
            this->previousEnemy = this->currentEnemy;
            return;
        }
        float step = (float) this->lastDelta;
        this->rotationAccum = (int) (step + (float) this->rotationAccum);
        turretGeometry->rotate(step * 0.00024414062f * 6.2831855f, 0.0f, 0.0f);
        this->previousEnemy = nullptr;
    } else if (aligned) {
        this->player->shoot(0, (long long) delta, false, matrix);
        ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(turretGeometry->transform))
                ->Update(delta, false);
    }
}

void PlayerTurret::update(int delta) {
    this->lastDelta = delta;
    if (!this->player->isActive())
        return;

    if (this->isSentryGun() && this->spawnInvulnTimer < 3000) {
        this->player->setVulnerable(false);
        this->spawnInvulnTimer += delta;
        if (this->spawnInvulnTimer >= 3000)
            this->player->setVulnerable(true);
    }

    if (this->turretHost != nullptr) {
        Matrix hostMatrix = *(const Matrix *) this->turretHost->player->transform;
        this->hostWorldOffset() = MatrixRotateVector(hostMatrix, this->hostOffset);
        this->geometry->setMatrix(hostMatrix);
        this->geometry->translate(this->hostWorldOffset());
    }
    *(Matrix *) this->player->transform = this->geometry->getMatrix();
    this->cachedPosition() = this->geometry->getPosition();

    if (this->player->getHitpoints() <= 0 && this->state != 3 && this->state != 4) {
        this->state = 3;
        this->explosionTimer = 0;
        Globals::sound->play(22, nullptr, nullptr, 0.0f);
        this->level->field_74->emitManual(this->level->field_3c, this->cachedPosition(), 0,
                                          Vector{0.0f, 0.0f, 0.0f}, -1.0f);
        this->level->field_74->enableSystemEmit(this->particleSystemId, true);
        this->explosion->start(this->cachedPosition(), Vector{0.0f, 0.0f, 0.0f});
        if (Globals::rnd->nextInt(100) < 0) {
            this->hasCargo = 1;
            this->cargo = new Array<int>();
            ArrayAdd(99, *this->cargo);
            ArrayAdd(Globals::rnd->nextInt(10) + 1, *this->cargo);
            this->createCrate(3);
            this->hasCargo = 1;
        } else if (this->level->getPlayer() != nullptr && this->level->getPlayer()->field_0x14 != nullptr &&
                   this->level->getPlayer()->field_0x14->field_0x1c == this) {
            this->level->getPlayer()->field_0x14->field_0x1c = nullptr;
        }
    }

    if (this->state == 4)
        return;
    if (this->state == 3) {
        this->explosionTimer += delta;
        this->explosion->update(delta, (TargetFollowCamera *) nullptr);
        if (this->explosionTimer > 4500) {
            this->level->field_74->enableSystemEmit(this->particleSystemId, false);
            this->state = 4;
            this->explosionTimer = 0;
            this->player->setActive(false);
            if (this->isSentryGun())
                this->level->field_6c--;
        }
        return;
    }

    // Factions 8 and 9 are always hostile.
    this->player->enemyFlagsLo = (this->shipGroup & ~1) == 8 ||
                                 ((Standing *) (intptr_t) Globals::status->getStanding())->isEnemy(this->shipGroup);
    this->player->carriesFriendCargoFlag =
            (this->shipGroup & ~1) != 8 &&
            ((Standing *) (intptr_t) Globals::status->getStanding())->isFriend(this->shipGroup);
    if (this->player->turnedEnemy())
        reinterpret_cast<uint16_t &>(this->player->enemyFlagsLo) = 1;
    if (this->player->isAlwaysFriend())
        reinterpret_cast<uint16_t &>(this->player->enemyFlagsLo) = 0x100;

    if (this->isSentryGun())
        handleSentryGun(delta);
    else if (this->turretEnabled() && this->turretGeometry != nullptr)
        handleTurret(delta);
}

// Factions 8, 9 and 10 are hostile to everyone else; 0/1 and 2/3 are mutual enemies.
static bool factionsHostile(int a, int b) {
    if (a == 8 && b != 8)
        return true;
    if (a != 8 && b == 8)
        return true;
    if (a == 10 && b != 10)
        return true;
    if (a != 10 && b == 10)
        return true;
    if (a == 9 && b != 9)
        return true;
    if (a != 9 && b == 9)
        return true;
    return (a == 0 && b == 1) || (a == 1 && b == 0) || (a == 3 && b == 2) || (a == 2 && b == 3);
}

void PlayerTurret::pickEnemy() {
    if (this->pickEnemyTimer <= 3000)
        return;

    int bestRange = this->turretRange;
    this->pickEnemyTimer = 0;
    this->currentEnemy = nullptr;
    Array<Player *> *enemies = this->player->getEnemies();
    if (enemies == nullptr)
        return;

    // The turret's world position is the KIPlayer vector at 0x2c.
    const Vector &position = reinterpret_cast<const Vector &>(this->autoPilotState);
    for (unsigned int i = 0; i < enemies->size(); ++i) {
        Player *enemy = (*enemies)[i];
        if (enemy->isDead() || !enemy->isActive())
            continue;
        if (!(enemy->egoFlag && this->player->enemyFlagsLo)) {
            if (this->field_0x3f_b) {
                if (!enemy->enemyFlagsLo)
                    continue;
            } else {
                if (enemy->getKIPlayer() == nullptr ||
                    !factionsHostile(enemy->getKIPlayer()->shipGroup, this->shipGroup))
                    continue;
            }
        }

        int distance = (int) VectorLength(position - enemy->getPosition());
        if (distance < bestRange &&
            (this->currentEnemy == nullptr || this->currentEnemy != this->previousEnemy)) {
            bestRange = distance;
            this->currentEnemy = enemy;
        }
    }
}

PlayerTurret::~PlayerTurret() {
    if (this->explosion != nullptr) {
        delete this->explosion;
    }
    this->explosion = nullptr;

    if (this->baseGeometry != nullptr) {
        delete this->baseGeometry;
    }
    this->baseGeometry = nullptr;

    if (this->turretGeometry != nullptr) {
        delete this->turretGeometry;
    }
    this->turretGeometry = nullptr;

    if (this->helperGeometry != nullptr) {
        delete this->helperGeometry;
    }
    this->helperGeometry = nullptr;
}

PlayerTurret::PlayerTurret(int mesh, Player *player, AEGeometry *geometry, float x, float y, float z)
    : KIPlayer(mesh, -1, player, geometry, x, y, z, false) {
    this->turretEnabled() = 1;
    this->field_0x3e = 1;
    this->baseGeometry = nullptr;
    this->turretGeometry = nullptr;
    this->spawnInvulnTimer = 0;
    this->turretHost = nullptr;
    this->turretRange = 50000;

    this->baseGeometry = new AEGeometry((uint16_t) mesh, Globals::Canvas, false);
    if (mesh == 14363) {
        this->turretGeometry = new AEGeometry((uint16_t) 14364, Globals::Canvas, false);
        this->turretGeometry->setRotationOrder(AbyssEngine::AEMath::ROTATION_ORDER_YXZ);
        this->turretGeometry->setPosition(Vector{0.0f, 565.0f, -528.0f});
    } else if (mesh == 6772) {
        this->turretGeometry = new AEGeometry((uint16_t) 6773, Globals::Canvas, false);
        this->turretGeometry->setRotationOrder(AbyssEngine::AEMath::ROTATION_ORDER_YXZ);
        this->turretGeometry->setPosition(Vector{0.0f, 60.0f, 0.0f});
    } else if (mesh == 6774) {
        this->turretGeometry = new AEGeometry((uint16_t) 6775, Globals::Canvas, false);
        this->turretGeometry->setRotationOrder(AbyssEngine::AEMath::ROTATION_ORDER_YXZ);
        this->turretGeometry->setPosition(Vector{0.0f, 60.0f, 0.0f});
    }

    this->helperGeometry = new AEGeometry(Globals::Canvas);
    ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(this->helperGeometry->transform))
            ->boundingRadius = 1000000.0f;
    this->setPosition(Vector{x, y, z});

    if (mesh == 14363) {
        this->baseGeometry->rotate(0.0f, 3.1415927f, 0.0f);
        this->turretGeometry->rotate(0.0f, 3.1415927f, 0.0f);
        this->helperGeometry->addChild(this->turretGeometry->transform);
    } else if ((mesh | 2) == 6774) {
        this->helperGeometry->addChild(this->turretGeometry->transform);
    } else if ((unsigned int) (mesh - 18880) < 3) {
        this->isSentryGun() = 1;
        unsigned short partMesh = 18888;
        if (mesh == 18881)
            partMesh = 18887;
        if (mesh == 18880)
            partMesh = 18886;
        AEGeometry *part = new AEGeometry(partMesh, Globals::Canvas, false);
        geometry->addChild(part->transform);
        delete part;
        geometry->setScaling(0.5f);
    }

    this->helperGeometry->addChild(this->baseGeometry->transform);
    if (!this->isSentryGun())
        geometry->addChild(this->helperGeometry->transform);

    this->explosion = new Explosion(0);
    this->explosion->addFireStreaks();
    this->currentEnemy = nullptr;
    this->previousEnemy = nullptr;
    this->explosionTimer = 0;
    this->pickEnemyTimer = 0;
    this->rotationAccum = 0;
    this->particleSystemId = -1;
}
