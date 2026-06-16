#include "game/ship/PlayerTurret.h"
#include "engine/render/AEGeometry.h"
#include "engine/audio/FModSound.h"
#include "game/world/Level.h"
#include "engine/render/ParticleSystemManager.h"
#include "engine/math/Transform.h"
#include "game/mission/Explosion.h"
#include "game/ship/KIPlayer.h"
#include "game/ship/Player.h"
#include "game/world/Standing.h"
#include "game/core/PaintCanvasClass.h"
#include "platform/libc.h"

using AbyssEngine::AEMath::Vector;
using AbyssEngine::AEMath::Matrix;

// Status singleton. Status isn't otherwise modelled in this TU, so declare the
// minimal surface needed here: the canonical singleton pointer plus getStanding()
// (which returns the Standing table id, cast to Standing* at the call site).
class Status {
public:
    int getStanding();
};
extern Status* gStatus;            // canonical Status singleton (binary .bss 0x2281b0)

namespace AbyssEngine {
namespace AEMath {
Vector MatrixGetDir(const Matrix& matrix);
Vector VectorNormalize(const Vector& v);
float  VectorLength(const Vector& v);
Vector MatrixRotateVector(const Matrix& matrix, const Vector& vector);
Vector MatrixInverseTransformVector(const Matrix& matrix, const Vector& vector);
Vector operator+(const Vector&, const Vector&);
Vector operator-(const Vector&, const Vector&);
Vector operator*(const Vector&, float);
Matrix operator*(const Matrix&, const Matrix&);
} // namespace AEMath
namespace AERandom { int nextInt(int rng, int max); }
} // namespace AbyssEngine

using AbyssEngine::AEMath::MatrixGetDir;
using AbyssEngine::AEMath::VectorNormalize;
using AbyssEngine::AEMath::VectorLength;
using AbyssEngine::AEMath::MatrixRotateVector;
using AbyssEngine::AEMath::MatrixInverseTransformVector;
using AbyssEngine::AEMath::operator+;
using AbyssEngine::AEMath::operator-;
using AbyssEngine::AEMath::operator*;

// Engine globals: sound bank and RNG id (file-local, not shared singletons).
extern FModSound** g_turretSound;
extern int         g_turretRandom;
// Canonical render-canvas singleton. PaintCanvas.h clashes with PaintCanvasClass.h
// in this TU, so declare the canonical pointer locally (matches the real definition).
extern PaintCanvas* gCanvas;       // canonical render canvas singleton (binary .bss 0x2281b8)

void PlayerTurret::setTurretRange(int range)
{
    this->turretRange = range;
}

void PlayerTurret::handleSentryGun(int delta)
{
    this->pickEnemyTimer += delta;
    this->pickEnemy();
    Player* enemy = this->currentEnemy;
    if (enemy != nullptr && enemy->field_5e == 0) {
        AEGeometry* geometry = this->geometry;
        this->handleRotation(delta, geometry, geometry);
    }
}

void PlayerTurret::setHost(KIPlayer* host, const Vector& offset)
{
    this->turretHost = host;
    this->hostOffset = offset;
}

void PlayerTurret::render()
{
    AEGeometry* visible = this->crateGeometry;
    if (visible != nullptr) {
        visible->render();
    }
    int state = this->state;
    if (state == 3) {
        this->explosion->render();
        state = this->state;
    }
    if ((uint32_t)(state - 3) >= 2) {
        this->renderBase();
    }
}

// render() tail: draw the inherited KIPlayer representation (the turret geometry)
// whenever the turret is not in the mid-explosion states (state in {3,4}).
void PlayerTurret::renderBase()
{
    this->KIPlayer::render();
}

void PlayerTurret::handleTurret(int delta)
{
    this->pickEnemyTimer += delta;
    this->pickEnemy();
    Player* enemy = this->currentEnemy;
    if (enemy != nullptr && enemy->field_5e == 0) {
        this->handleRotation(delta, this->helperGeometry, this->turretGeometry);
    }
}

void PlayerTurret::revive()
{
    this->player->reset();
    this->crateGeometry = nullptr;
    this->state = 1;
    this->reviveFlag = 0;
    this->explosion->reset();
    AEGeometry* geometry = this->parentGeometry;
    this->spawnInvulnTimer = 0;
    this->visibleFlag = 1;
    if (geometry == nullptr) {
        geometry = this->geometry;
    }
    geometry->setVisible(true);
}

void PlayerTurret::setPosition(const Vector& position)
{
    this->geometry->setPosition(position);
    this->posX = position.x;
    this->posY = position.y;
    this->posZ = position.z;
}

void PlayerTurret::reset()
{
    this->KIPlayer::reset();
    this->state = 0;
}

void PlayerTurret::setLevel(Level* level)
{
    this->KIPlayer::setLevel(level);
    ParticleSystemManager* manager = (ParticleSystemManager*)this->level->field_74;
    int system = manager->addSystem(&this->geometry->getReferenceMatrix(), 9, false);
    this->particleSystemId = system;
    manager->enableSystemEmit(system, false);
}

KIPlayer* PlayerTurret::getHost()
{
    return this->turretHost;
}

void PlayerTurret::setScaling(float scale)
{
    this->helperGeometry->setScaling(scale);
}

void PlayerTurret::handleRotation(int delta, AEGeometry* mainGeometry, AEGeometry* turretGeometry)
{
    Matrix matrix;
    matrix = *(const Matrix*)this->currentEnemy->transform;
    Vector enemyPos;
    this->currentEnemy->getPosition(&enemyPos);

    Vector dir = MatrixGetDir(matrix);
    Vector normal = VectorNormalize(dir);
    Vector scaled = normal * 3000.0f;
    this->aimPoint = enemyPos + scaled;

    if (!this->isSentryGun) {
        Matrix base = this->geometry->getMatrix();
        Matrix turret = turretGeometry->getMatrix();
        Matrix main = mainGeometry->getMatrix();
        matrix = (base * turret) * main;
    } else {
        matrix = this->geometry->getMatrix();
    }

    Vector local = MatrixInverseTransformVector(matrix, this->aimPoint);
    Vector aim = VectorNormalize(local);

    bool ready = false;
    float yaw;
    if (aim.x > 0.0f) {
        yaw = (float)delta;
        turretGeometry->rotate(0.0f, yaw * 0.001f * 0.25f, 0.0f);
    } else if (aim.x < -0.05f) {
        yaw = (float)-delta;
        turretGeometry->rotate(0.0f, yaw * 0.001f * 0.25f, 0.0f);
    } else {
        ready = true;
    }

    if (aim.y > 0.0f) {
        if (!this->isSentryGun && this->rotationAccum < 100) {
            this->previousEnemy = this->currentEnemy;
            this->pickEnemyTimer += delta;
            return;
        }
        float step = (float)this->frameDelta;
        float next = (float)this->rotationAccum - step;
        this->rotationAccum = (int)next;
        mainGeometry->rotate(next, 0.0f, step * 0.001f * 0.25f);
        this->previousEnemy = nullptr;
        return;
    }
    if (aim.y < -0.05f) {
        if (!this->isSentryGun && this->rotationAccum > 99) {
            this->previousEnemy = this->currentEnemy;
            this->pickEnemyTimer += delta;
            return;
        }
        float step = (float)this->frameDelta;
        float next = (float)this->rotationAccum + step;
        this->rotationAccum = (int)next;
        mainGeometry->rotate(next, 0.0f, step * 0.001f * -0.25f);
        this->previousEnemy = nullptr;
        return;
    }

    if (ready) {
        this->player->shoot(0, delta, delta >> 31, 0);
        AbyssEngine::Transform* transform =
            (AbyssEngine::Transform*)gCanvas->TransformGetTransform(turretGeometry->transform);
        transform->Update(delta, delta >> 31);
    }
}

void PlayerTurret::update(int delta)
{
    this->frameDelta = delta;

    Player* player = this->player;
    if (!player->isActive()) {
        return;
    }

    if (this->isSentryGun && this->spawnInvulnTimer < 3000) {
        player->setVulnerable(false);
        int time = this->spawnInvulnTimer + delta;
        this->spawnInvulnTimer = time;
        if (time > 2999) {
            player->setVulnerable(true);
        }
    }

    if (this->turretHost != nullptr) {
        const Matrix& hostMatrix = *(const Matrix*)this->turretHost->player->transform;
        this->hostWorldOffset = MatrixRotateVector(hostMatrix, this->hostOffset);
        this->geometry->setMatrix(hostMatrix);
        this->geometry->translate(this->hostWorldOffset);
    }

    Matrix geomMatrix = this->geometry->getMatrix();
    *(Matrix*)player->transform = geomMatrix;
    this->cachedPosition = this->geometry->getPosition();

    int hp = player->getHitpoints();
    int state = this->state;
    if (hp < 1 && (uint32_t)(state - 3) >= 2) {
        this->explosionTimer = 0;
        this->state = 3;
        (*g_turretSound)->play(0x16, nullptr, nullptr, 0.0f);
        Vector zero = {0.0f, 0.0f, 0.0f};
        ParticleSystemManager* manager = (ParticleSystemManager*)this->level->field_74;
        manager->emitManual(this->level->field_3c, (const float*)&this->cachedPosition, 0, 0.0f);
        manager->enableSystemEmit(this->particleSystemId, true);
        this->explosion->start(&this->cachedPosition, &zero);

        int random = AbyssEngine::AERandom::nextInt(g_turretRandom, 100);
        if (random < 0) {
            this->cargo = new Array<int>();
            this->cargo->push_back(99);
            this->cargo->push_back(AbyssEngine::AERandom::nextInt(g_turretRandom, 10) + 1);
            this->createCrate(3);
        } else {
            Player* levelPlayer = (Player*)(intptr_t)this->level->getPlayer();
            if (levelPlayer != nullptr && levelPlayer->kiPlayer != nullptr &&
                levelPlayer->kiPlayer->wingmanTarget == (KIPlayer*)this) {
                levelPlayer->kiPlayer->wingmanTarget = nullptr;
            }
        }
        state = this->state;
    }

    if (state == 4) {
        return;
    }
    if (state == 3) {
        this->explosionTimer += delta;
        this->explosion->update(delta, nullptr);
        if (this->explosionTimer > 4500) {
            ((ParticleSystemManager*)this->level->field_74)->enableSystemEmit(this->particleSystemId, false);
            this->explosionTimer = 0;
            this->state = 4;
            player->setActive(false);
            if (this->isSentryGun) {
                this->level->field_6c -= 1;
            }
        }
        return;
    }

    int faction = this->shipGroup;
    if ((faction & 0xfffffffeU) == 8) {
        player->enemyFlags = 1;
    } else {
        Standing* standing = (Standing*)(intptr_t)gStatus->getStanding();
        bool enemy = standing->isEnemy(faction);
        bool friendly = false;
        if ((faction & 0xfffffffeU) != 8) {
            friendly = ((Standing*)(intptr_t)gStatus->getStanding())->isFriend(faction);
        }
        player->enemyFlags = (uint16_t)((friendly ? 0x100 : 0) | (enemy ? 1 : 0));
    }
    if (Player_turnedEnemy(player) != 0) {
        player->enemyFlags = 1;
    }
    if (player->isAlwaysFriend() != 0) {
        player->enemyFlags = 0x100;
    }

    if (this->isSentryGun) {
        handleSentryGun(delta);
    } else if (this->turretEnabled && this->turretGeometry != nullptr) {
        handleTurret(delta);
    }
}

void PlayerTurret::pickEnemy()
{
    if (this->pickEnemyTimer <= 3000) {
        return;
    }

    int bestRange = this->turretRange;
    this->pickEnemyTimer = 0;
    this->currentEnemy = nullptr;

    Array<Player*>* enemies = this->player->getEnemies();
    if (enemies == nullptr) {
        return;
    }

    const Vector& position = this->cachedPosition;
    for (uint32_t i = 0; i < enemies->size(); ++i) {
        Player* enemy = (*enemies)[i];
        if (enemy->isDead() || !enemy->isActive()) {
            continue;
        }

        bool accepted = false;
        if ((enemy->empDisabled >> 8) != 0 && (this->player->enemyFlags & 0xff) != 0) {
            accepted = true;
        } else if (!this->isSentryGun) {
            if (enemy->getKIPlayer() != nullptr) {
                accepted = true;
            }
        } else if ((enemy->enemyFlags & 0xff) != 0) {
            accepted = true;
        }

        if (!accepted) {
            continue;
        }

        Vector enemyPos;
        enemy->getPosition(&enemyPos);
        Vector diff = position - enemyPos;
        int distance = (int)VectorLength(diff);
        if (distance < bestRange) {
            Player* current = this->currentEnemy;
            if (current == nullptr || current != this->previousEnemy) {
                bestRange = distance;
                this->currentEnemy = enemy;
            }
        }
    }
}

PlayerTurret::~PlayerTurret()
{
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

PlayerTurret::PlayerTurret(int mesh, Player* player, AEGeometry* geometry, float x, float y, float z)
    : KIPlayer(0, 0, player, geometry, x, y, z, false)
{
    this->turretEnabled = true;
    this->field_0x3e = 1;
    this->isSentryGun = false;
    this->baseGeometry = nullptr;
    this->turretGeometry = nullptr;
    this->spawnInvulnTimer = 0;
    this->turretHost = nullptr;
    this->hostOffset = Vector{0.0f, 0.0f, 0.0f};
    this->turretRange = 50000;

    this->baseGeometry = new AEGeometry((uint16_t)mesh, gCanvas, false);

    if (mesh == 0x381b) {
        AEGeometry* turret = new AEGeometry((uint16_t)0x381c, gCanvas, false);
        this->turretGeometry = turret;
        turret->setRotationOrder(2);
        turret->setPosition(Vector{0.0f, 0.0f, 0.0f});
    } else if (mesh == 0x1a76) {
        AEGeometry* turret = new AEGeometry((uint16_t)0x1a77, gCanvas, false);
        this->turretGeometry = turret;
        turret->setRotationOrder(2);
        turret->setPosition(Vector{0.0f, 0.0f, 0.0f});
    } else if (mesh == 0x1a74) {
        AEGeometry* turret = new AEGeometry((uint16_t)0x1a75, gCanvas, false);
        this->turretGeometry = turret;
        turret->setRotationOrder(2);
        turret->setPosition(Vector{0.0f, 0.0f, 0.0f});
    }

    this->helperGeometry = new AEGeometry(gCanvas);
    AbyssEngine::Transform* helperTransform =
        (AbyssEngine::Transform*)gCanvas->TransformGetTransform(this->helperGeometry->transform);
    helperTransform->field_0x58 = 0;

    this->setPosition(Vector{x, y, z});

    if (mesh == 0x381b) {
        this->baseGeometry->rotate(0.0f, 0.0f, 0.0f);
        this->turretGeometry->rotate(0.0f, 0.0f, 0.0f);
        this->helperGeometry->addChild(this->turretGeometry->transform);
    } else if ((mesh | 2U) == 0x1a76) {
        this->helperGeometry->addChild(this->turretGeometry->transform);
    } else if ((uint32_t)(mesh - 0x49c0) < 3) {
        this->isSentryGun = true;
        uint16_t childMesh = 0x49c8;
        if (mesh == 0x49c1) {
            childMesh = 0x49c7;
        }
        if (mesh == 0x49c0) {
            childMesh = 0x49c6;
        }
        AEGeometry* child = new AEGeometry(childMesh, gCanvas, false);
        geometry->addChild(child->transform);
        delete child;
        geometry->setScaling(0.5f);
    }

    this->helperGeometry->addChild(this->baseGeometry->transform);
    if (!this->isSentryGun) {
        geometry->addChild(this->helperGeometry->transform);
    }

    this->explosion = new Explosion(0);
    this->explosion->addFireStreaks();

    this->currentEnemy = nullptr;
    this->previousEnemy = nullptr;
    this->explosionTimer = 0;
    this->pickEnemyTimer = 0;
    this->rotationAccum = 0;
    this->particleSystemId = 0;
}
