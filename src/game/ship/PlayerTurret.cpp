#include "gof2/game/ship/PlayerTurret.h"
#include "gof2/engine/render/AEGeometry.h"
#include "gof2/engine/audio/FModSound.h"
#include "gof2/game/world/Level.h"
#include "gof2/engine/render/ParticleSystemManager.h"
#include "gof2/engine/math/Transform.h"
#include "gof2/game/mission/Explosion.h"
#include "gof2/game/ship/KIPlayer.h"
#include "gof2/game/ship/Player.h"
#include "gof2/game/world/Standing.h"
#include "gof2/game/core/PaintCanvasClass.h"

// Status singleton accessor. The decompiler emitted bare Status_getStanding() (the self
// argument was dropped); the original reads the global Status* singleton (DAT_189d44) and
// calls Status::getStanding(). Status isn't otherwise modeled in this TU, so declare the
// minimal surface needed: the singleton global plus getStanding().
class Status {
public:
    Standing *getStanding();
};
extern "C" __attribute__((visibility("hidden"))) Status **g_status;

extern "C" void *KIPlayer_dtor(void *self);
void MatrixGetDir(Vector *out, const void *matrix);
void VectorNormalize(Vector *out, const Vector *v);
namespace AbyssEngine { namespace AEMath {
Vector MatrixRotateVector(const Matrix &matrix, const Vector &vector);
Vector MatrixInverseTransformVector(const Matrix &matrix, const Vector &vector);
Vector operator+(const Vector &, const Vector &);
Vector operator-(const Vector &, const Vector &);
Vector operator*(const Vector &, float);
Matrix operator*(const Matrix &, const Matrix &);
} }
namespace AbyssEngine { namespace AERandom { int nextInt(int rng, int max); } }
float VectorLength(const Vector *v);
extern "C" void *Explosion_dtor(Explosion *self);
extern "C" void Explosion_ctor(Explosion *self, int kind);

void PlayerTurret::setTurretRange(int range)
{
    this->turretRange = range;
}

void PlayerTurret::handleSentryGun(int delta)
{
    this->pickEnemyTimer = this->pickEnemyTimer + delta;
    this->pickEnemy();
    Player *enemy = this->currentEnemy;
    if (enemy != 0 && UC(enemy, 0x5e) == 0) {
        AEGeometry *geometry = this->geometry;
        this->handleRotation(delta, geometry, geometry);
    }
}

using AbyssEngine::AEMath::Vector;

void PlayerTurret::setHost(KIPlayer *host, const Vector &offset)
{
    this->host = host;
    this->hostOffset = offset;
}

void PlayerTurret::render()
{
    AEGeometry *visible = this->visibleGeometry;
    if (visible != 0) {
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

// PlayerTurret::renderBase() -- tail of render(): draw the inherited KIPlayer
// representation (the actual turret geometry). render() falls through to this
// whenever the turret is not in the mid-explosion states (0x88 in {3,4}).
void PlayerTurret::renderBase()
{
    ((KIPlayer *)this)->render();
}

void PlayerTurret::handleTurret(int delta)
{
    this->pickEnemyTimer = this->pickEnemyTimer + delta;
    this->pickEnemy();
    Player *enemy = this->currentEnemy;
    if (enemy != 0 && UC(enemy, 0x5e) == 0) {
        this->handleRotation(delta, this->helperGeometry, this->turretGeometry);
    }
}

void PlayerTurret::revive()
{
    this->player->reset();
    this->visibleGeometry = 0;
    this->state = 1;
    this->field_0xd8 = 0;
    this->explosion->reset();
    AEGeometry *geometry = this->parentGeometry;
    this->spawnInvulnTimer = 0;
    this->visibleFlag = 1;
    if (geometry == 0) {
        geometry = this->geometry;
    }
    geometry->setVisible(true);
}

using AbyssEngine::AEMath::Vector;

void PlayerTurret::setPosition(const Vector &position)
{
    this->geometry->setPosition(position);
    this->posX = position.x;
    this->posY = position.y;
    this->posZ = position.z;
}

void PlayerTurret::reset()
{
    ((KIPlayer *)(this))->reset();
    this->state = 0;
}

void PlayerTurret::setLevel(Level *level)
{
    ((KIPlayer *)(this))->setLevel(level);
    int manager = I(this->level, 0x74);
    int matrix = (int)(intptr_t)(void *)&this->geometry->getReferenceMatrix();
    int system = ((ParticleSystemManager *)(manager))->addSystem((const void *)(long)matrix, 9, 0);
    this->particleSystemId = system;
    ((ParticleSystemManager *)(I(this->level, 0x74)))->enableSystemEmit(system, 0);
}

KIPlayer *PlayerTurret::getHost()
{
    return this->host;
}

// PlayerTurret deleting destructor (D0): run the complete dtor, then free.
void _ZN12PlayerTurretD0Ev(PlayerTurret *self)
{
    ::operator delete(((PlayerTurret *)(self))->completeDtor());
}

void PlayerTurret::setScaling(float scale)
{
    this->helperGeometry->setScaling(scale);
}

using AbyssEngine::AEMath::Vector;

extern uint32_t *gPlayerTurretCanvas_rotation __attribute__((visibility("hidden")));

void PlayerTurret::handleRotation(int delta, AEGeometry *mainGeometry, AEGeometry *turretGeometry)
{
    char matrixBytes[60];
    char positionBytes[12];
    char dirBytes[12];
    char normalBytes[12];
    char scaledBytes[12];
    char sumBytes[12];
    char tmpMatrixA[60];
    char tmpMatrixB[60];

    __aeabi_memcpy(matrixBytes, B(this->currentEnemy, 0x4), 0x3c);
    this->currentEnemy->getPosition((Vector *)positionBytes);
    MatrixGetDir((Vector *)dirBytes, matrixBytes);
    VectorNormalize((Vector *)normalBytes, (Vector *)dirBytes);
    *(Vector *)((Vector *)scaledBytes) = *(const Vector *)((Vector *)normalBytes) * (3000.0f);
    *(Vector *)((Vector *)sumBytes) = *(const Vector *)((Vector *)positionBytes) + *(const Vector *)((Vector *)scaledBytes);
    this->aimPoint = *(const Vector *)((Vector *)sumBytes);

    if (this->isSentryGun == 0) {
        void *base = this->geometry->getMatrix();
        void *turret = ((AEGeometry *)(turretGeometry))->getMatrix();
        *(Matrix *)(tmpMatrixA) = *(const Matrix *)(base) * *(const Matrix *)(turret);
        void *main = ((AEGeometry *)(mainGeometry))->getMatrix();
        *(Matrix *)(tmpMatrixB) = *(const Matrix *)(tmpMatrixA) * *(const Matrix *)(main);
        *(Matrix *)(matrixBytes) = *(const Matrix *)(tmpMatrixB);
    } else {
        *(Matrix *)(matrixBytes) = this->geometry->getMatrix();
    }

    *(AbyssEngine::AEMath::Vector *)positionBytes = AbyssEngine::AEMath::MatrixInverseTransformVector(
        *(const AbyssEngine::AEMath::Matrix *)matrixBytes,
        this->aimPoint);
    VectorNormalize((Vector *)sumBytes, (Vector *)positionBytes);
    Vector *normal = (Vector *)sumBytes;

    bool ready = false;
    float yaw = 0.0f;
    if (normal->x > 0.0f) {
        yaw = (float)delta;
        ((AEGeometry *)(turretGeometry))->rotate(0.0f, yaw * 0.001f * 0.25f, 0.0f);
    } else if (normal->x < -0.05f) {
        yaw = (float)-delta;
        ((AEGeometry *)(turretGeometry))->rotate(0.0f, yaw * 0.001f * 0.25f, 0.0f);
    } else {
        ready = true;
    }

    if (normal->y > 0.0f) {
        if (this->isSentryGun == 0 && this->rotationAccum < 100) {
            this->previousEnemy = this->currentEnemy;
            this->pickEnemyTimer = this->pickEnemyTimer + delta;
            return;
        }
        float step = (float)this->frameDelta;
        float next = (float)this->rotationAccum - step;
        this->rotationAccum = (int)next;
        ((AEGeometry *)(mainGeometry))->rotate(next, 0.0f, step * 0.001f * 0.25f);
        this->previousEnemy = 0;
        return;
    }
    if (normal->y < -0.05f) {
        if (this->isSentryGun == 0 && this->rotationAccum > 99) {
            this->previousEnemy = this->currentEnemy;
            this->pickEnemyTimer = this->pickEnemyTimer + delta;
            return;
        }
        float step = (float)this->frameDelta;
        float next = (float)this->rotationAccum + step;
        this->rotationAccum = (int)next;
        ((AEGeometry *)(mainGeometry))->rotate(next, 0.0f, step * 0.001f * -0.25f);
        this->previousEnemy = 0;
        return;
    }

    if (ready) {
        this->player->shoot(0, delta, delta >> 31, 0);
        uint32_t transform = (uint32_t)(uintptr_t)((PaintCanvas*)(long)*gPlayerTurretCanvas_rotation)->TransformGetTransform(
                                                               U(turretGeometry, 0xc));
        ((AbyssEngine::Transform *)(transform))->Update(delta, delta >> 31);
    }
}

using AbyssEngine::AEMath::Matrix;
using AbyssEngine::AEMath::Vector;

extern int *gPlayerTurretSound __attribute__((visibility("hidden")));
extern int *gPlayerTurretRandom __attribute__((visibility("hidden")));
extern int *gPlayerTurretStanding __attribute__((visibility("hidden")));

void PlayerTurret::update(int delta)
{
    char matrixBytes[60];
    char vectorBytes[12];
    this->frameDelta = delta;

    Player *player = this->player;
    if (((Player *)(player))->isActive() == 0) {
        return;
    }

    if (this->isSentryGun != 0 && this->spawnInvulnTimer < 3000) {
        ((Player *)(player))->setVulnerable(false);
        int time = this->spawnInvulnTimer + delta;
        this->spawnInvulnTimer = time;
        if (time > 2999) {
            ((Player *)(player))->setVulnerable(true);
        }
    }

    if (this->host != 0) {
        __aeabi_memcpy(matrixBytes, B(P(this->host, 0x4), 0x4), 0x3c);
        *(AbyssEngine::AEMath::Vector *)vectorBytes = AbyssEngine::AEMath::MatrixRotateVector(
            *(const AbyssEngine::AEMath::Matrix *)matrixBytes,
            this->hostOffset);
        this->hostWorldOffset = *(const Vector *)((Vector *)vectorBytes);
        this->geometry->setMatrix(*(const AbyssEngine::AEMath::Matrix *)matrixBytes);
        this->geometry->translate(this->hostWorldOffset);
    }

    void *matrix = this->geometry->getMatrix();
    *(Matrix *)(B(player, 0x4)) = *(const Matrix *)(matrix);
    ((AEGeometry *)((Vector *)vectorBytes))->getPosition();
    this->position = *(const Vector *)((Vector *)vectorBytes);

    int hp = ((Player *)(player))->getHitpoints();
    int state = this->state;
    if (hp < 1 && (uint32_t)(state - 3) >= 2) {
        this->explosionTimer = 0;
        this->state = 3;
        // The original reads play()'s sound-handle return into `value` and forwards it as the
        // (unused) p4 selector to emitManual; the modeled play() is void, so just emit the call.
        ((FModSound *)(**(int **)gPlayerTurretSound))->play(0x16, 0, 0, 0.0f);
        float value = 0.0f;
        Vector zero = {0.0f, 0.0f, 0.0f};
        ((ParticleSystemManager *)(I(this->level, 0x74)))->emitManual(I(this->level, 0x3c), (const float *)&this->position, (int)(long)&zero, value);
        ((ParticleSystemManager *)(I(this->level, 0x74)))->enableSystemEmit(this->particleSystemId, 1);
        this->explosion->start(&this->position, &zero);

        int random = AbyssEngine::AERandom::nextInt(*gPlayerTurretRandom, 100);
        if (random < 0) {
            this->lootSpawned = 1;
            Array<int> *array = new Array<int>();
            this->loot = array;
            array->push_back(99);
            this->loot->push_back(AbyssEngine::AERandom::nextInt(*gPlayerTurretRandom, 10) + 1);
            ((KIPlayer *)(this))->createCrate(3);
            this->lootSpawned = 1;
        } else {
            int levelPlayer = ((Level *)((Level *)this->level))->getPlayer();
            if (levelPlayer != 0) {
                levelPlayer = ((Level *)((Level *)this->level))->getPlayer();
                if (I((void *)levelPlayer, 0x14) != 0) {
                    levelPlayer = ((Level *)((Level *)this->level))->getPlayer();
                    if (P(P((void *)levelPlayer, 0x14), 0x1c) == this) {
                        levelPlayer = ((Level *)((Level *)this->level))->getPlayer();
                        P(P((void *)levelPlayer, 0x14), 0x1c) = 0;
                    }
                }
            }
        }
        state = this->state;
    }

    if (state == 4) {
        return;
    }
    if (state == 3) {
        this->explosionTimer = this->explosionTimer + delta;
        this->explosion->update(delta, 0);
        if (this->explosionTimer > 4500) {
            ((ParticleSystemManager *)(I(this->level, 0x74)))->enableSystemEmit(this->particleSystemId, 0);
            this->explosionTimer = 0;
            this->state = 4;
            ((Player *)(player))->setActive(false);
            if (this->isSentryGun != 0) {
                I(this->level, 0x6c) = I(this->level, 0x6c) - 1;
            }
        }
        return;
    }

    uint32_t standing = this->standing;
    if ((standing & 0xfffffffeU) == 8) {
        UC(player, 0x5c) = 1;
        UC(player, 0x5d) = 0;
    } else {
        Standing *s = ((Status *)(*g_status))->getStanding();
        UC(player, 0x5c) = ((Standing *)(s))->isEnemy(standing);
        if ((standing & 0xfffffffeU) == 8) {
            UC(player, 0x5d) = 0;
        } else {
            UC(player, 0x5d) = ((Standing *)(((Status *)(*g_status))->getStanding()))->isFriend(standing);
        }
    }
    if (Player_turnedEnemy((Player *)(player)) != 0) {
        US(player, 0x5c) = 1;
    }
    if (((Player *)(player))->isAlwaysFriend() != 0) {
        US(player, 0x5c) = 0x100;
    }

    if (this->isSentryGun != 0) {
        handleSentryGun(delta);
    } else if (this->turretEnabled != 0 && this->turretGeometry != 0) {
        handleTurret(delta);
    }
}

using AbyssEngine::AEMath::Vector;

void PlayerTurret::pickEnemy()
{
    char vectorFrame[24];

    if (this->pickEnemyTimer > 3000) {
        int bestRange = this->turretRange;
        this->pickEnemyTimer = 0;
        this->currentEnemy = 0;

        Array<Player *> *enemies = this->player->getEnemies();
        if (enemies != 0) {
            Vector *position = &this->position;
            uint32_t i = 0;
            goto check;
        next:
            {
                Player *enemy = (*enemies)[i];

                if (((Player *)(enemy))->isDead() == 0 && ((Player *)(enemy))->isActive() != 0) {
                    bool accepted = false;
                    if (UC(enemy, 0x69) != 0 && UC(this->player, 0x5c) != 0) {
                        accepted = true;
                    } else if (this->isSentryGun == 0) {
                        if (((Player *)(enemy))->getKIPlayer() != 0) {
                            accepted = true;
                        }
                    } else if (UC(enemy, 0x5c) != 0) {
                        accepted = true;
                    }

                    if (accepted) {
                        ((Player *)enemy)->getPosition((Vector *)vectorFrame);
                        *(Vector *)((Vector *)(vectorFrame + 12)) = *(const Vector *)(position) - *(const Vector *)((Vector *)vectorFrame);
                        int distance = (int)VectorLength((Vector *)(vectorFrame + 12));
                        if (distance < bestRange) {
                            Player *current = this->currentEnemy;
                            if (current == 0 || current != this->previousEnemy) {
                                bestRange = distance;
                                this->currentEnemy = enemy;
                            }
                        }
                    }
                }
            }
            i = i + 1;
        check:
            if (i < enemies->size()) {
                goto next;
            }
        }
    }
}

extern void *gPlayerTurretVtable __attribute__((visibility("hidden")));

PlayerTurret::~PlayerTurret() noexcept(false)
{
    this->vtable = (char *)gPlayerTurretVtable + 8;

    Explosion *explosion = this->explosion;
    if (explosion != 0) {
        ::operator delete(Explosion_dtor(explosion));
    }
    this->explosion = 0;

    AEGeometry *base = this->baseGeometry;
    if (base != 0) {
        base->~AEGeometry(); ::operator delete(base);
    }
    this->baseGeometry = 0;

    AEGeometry *turret = this->turretGeometry;
    if (turret != 0) {
        turret->~AEGeometry(); ::operator delete(turret);
    }
    this->turretGeometry = 0;

    AEGeometry *helper = this->helperGeometry;
    if (helper != 0) {
        helper->~AEGeometry(); ::operator delete(helper);
    }
    this->helperGeometry = 0;

    KIPlayer_dtor(this);
}

// PlayerTurret complete destructor (D1): runs the object destructor in place and returns `this`
// so the deleting-destructor caller can hand the same pointer to operator delete.
PlayerTurret *PlayerTurret::completeDtor()
{
    this->~PlayerTurret();
    return this;
}

using AbyssEngine::AEMath::Vector;

extern void *gPlayerTurretVtable_ctor __attribute__((visibility("hidden")));
extern uint32_t *gPlayerTurretCanvas __attribute__((visibility("hidden")));

typedef void (*SetPositionFn)(PlayerTurret *self, const Vector *position);

PlayerTurret::PlayerTurret(int mesh, Player *player, AEGeometry *geometry, float x, float y, float z)
{
    char vectorBytes[12];

    this->vtable = (char *)gPlayerTurretVtable_ctor + 8;
    this->turretEnabled = 1;
    this->field_0x3e = 1;
    this->baseGeometry = 0;
    this->turretGeometry = 0;
    this->spawnInvulnTimer = 0;
    this->host = 0;
    this->hostOffset.x = 0.0f;
    this->hostOffset.y = 0.0f;
    this->hostOffset.z = 0.0f;
    this->turretRange = 50000;

    AEGeometry *base = (AEGeometry *)operator new(0xc0);
    uint32_t *canvasHolder = gPlayerTurretCanvas;
    new ((void *)base) AEGeometry((uint16_t)mesh, (PaintCanvas *)(void *)*canvasHolder, false);
    this->baseGeometry = base;

    if (mesh == 0x381b) {
        AEGeometry *turret = (AEGeometry *)operator new(0xc0);
        new ((void *)turret) AEGeometry((uint16_t)0x381c, (PaintCanvas *)(void *)*canvasHolder, false);
        this->turretGeometry = turret;
        turret->setRotationOrder(2);
        Vector *v = (Vector *)vectorBytes;
        v->x = 0.0f;
        v->y = 0.0f;
        v->z = 0.0f;
        ((AEGeometry *)(turret))->setPosition(*v);
    } else if (mesh == 0x1a76) {
        AEGeometry *turret = (AEGeometry *)operator new(0xc0);
        new ((void *)turret) AEGeometry((uint16_t)0x1a77, (PaintCanvas *)(void *)*canvasHolder, false);
        this->turretGeometry = turret;
        turret->setRotationOrder(2);
        Vector *v = (Vector *)vectorBytes;
        v->x = 0.0f;
        v->y = 0.0f;
        v->z = 0.0f;
        ((AEGeometry *)(turret))->setPosition(*v);
    } else if (mesh == 0x1a74) {
        AEGeometry *turret = (AEGeometry *)operator new(0xc0);
        new ((void *)turret) AEGeometry((uint16_t)0x1a75, (PaintCanvas *)(void *)*canvasHolder, false);
        this->turretGeometry = turret;
        turret->setRotationOrder(2);
        Vector *v = (Vector *)vectorBytes;
        v->x = 0.0f;
        v->y = 0.0f;
        v->z = 0.0f;
        ((AEGeometry *)(turret))->setPosition(*v);
    }

    AEGeometry *helper = (AEGeometry *)operator new(0xc0);
    new ((void *)helper) AEGeometry((PaintCanvas *)(void *)*canvasHolder);
    this->helperGeometry = helper;
    uint32_t transform = (uint32_t)(uintptr_t)((PaintCanvas*)(long)*canvasHolder)->TransformGetTransform(U(helper, 0xc));
    I((void *)transform, 0xe0) = 0;

    Vector *initial = (Vector *)vectorBytes;
    initial->x = x;
    initial->y = y;
    initial->z = z;
    SetPositionFn setPosition = *(SetPositionFn *)((char *)this->vtable + 0x44);
    setPosition(this, initial);

    if (mesh == 0x381b) {
        this->baseGeometry->rotate(0.0f, 0.0f, 0.0f);
        this->turretGeometry->rotate(0.0f, 0.0f, 0.0f);
        this->helperGeometry->addChild(U(this->turretGeometry, 0xc));
    } else if ((mesh | 2U) == 0x1a76) {
        this->helperGeometry->addChild(U(this->turretGeometry, 0xc));
    } else if ((uint32_t)(mesh - 0x49c0) < 3) {
        this->isSentryGun = 1;
        AEGeometry *child = (AEGeometry *)operator new(0xc0);
        uint16_t childMesh = 0x49c8;
        if (mesh == 0x49c1) {
            childMesh = 0x49c7;
        }
        if (mesh == 0x49c0) {
            childMesh = 0x49c6;
        }
        new ((void *)child) AEGeometry((uint16_t)childMesh, (PaintCanvas *)(void *)*canvasHolder, false);
        ((AEGeometry *)(geometry))->addChild(U(child, 0xc));
        [&]{ AEGeometry *g_ = (AEGeometry *)(child); if (g_) { g_->~AEGeometry(); ::operator delete(g_); } }();
        ((AEGeometry *)(geometry))->setScaling(0.5f);
    }

    this->helperGeometry->addChild(U(this->baseGeometry, 0xc));
    if (this->isSentryGun == 0) {
        ((AEGeometry *)(geometry))->addChild(U(this->helperGeometry, 0xc));
    }

    Explosion *explosion = (Explosion *)operator new(0x68);
    Explosion_ctor(explosion, 0);
    this->explosion = explosion;
    ((Explosion *)(explosion))->addFireStreaks();

    this->currentEnemy = 0;
    this->previousEnemy = 0;
    this->explosionTimer = 0;
    this->pickEnemyTimer = 0;
    this->rotationAccum = 0;
    this->particleSystemId = 0;
}
