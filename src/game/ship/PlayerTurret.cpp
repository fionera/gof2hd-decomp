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
    I(this, 0x164) = range;
}

void PlayerTurret::handleSentryGun(int delta)
{
    I(this, 0x130) = I(this, 0x130) + delta;
    ((PlayerTurret *)(this))->pickEnemy();
    void *enemy = P(this, 0x14c);
    if (enemy != 0 && UC(enemy, 0x5e) == 0) {
        AEGeometry *geometry = TP<AEGeometry>(this, 0x8);
        ((PlayerTurret *)(this))->handleRotation(delta, geometry, geometry);
    }
}

using AbyssEngine::AEMath::Vector;

void PlayerTurret::setHost(KIPlayer *host, const Vector &offset)
{
    P(this, 0x154) = host;
    *(Vector *)((Vector *)B(this, 0x158)) = *(const Vector *)(&offset);
}

void PlayerTurret::render()
{
    void *visible = P(this, 0x78);
    if (visible != 0) {
        ((AEGeometry *)(visible))->render();
    }
    int state = I(this, 0x88);
    if (state == 3) {
        ((Explosion *)(TP<Explosion>(this, 0x13c)))->render();
        state = I(this, 0x88);
    }
    if ((uint32_t)(state - 3) >= 2) {
        ((PlayerTurret *)(this))->renderBase();
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
    I(this, 0x130) = I(this, 0x130) + delta;
    ((PlayerTurret *)(this))->pickEnemy();
    void *enemy = P(this, 0x14c);
    if (enemy != 0 && UC(enemy, 0x5e) == 0) {
        ((PlayerTurret *)(this))->handleRotation(delta, TP<AEGeometry>(this, 0x148), TP<AEGeometry>(this, 0x144));
    }
}

void PlayerTurret::revive()
{
    int zero = 0;
    int one = 1;
    ((Player *)(TP<Player>(this, 0x4)))->reset();
    *(volatile int *)B(this, 0x78) = zero;
    *(volatile int *)B(this, 0x88) = one;
    *(volatile int *)B(this, 0xd8) = zero;
    ((Explosion *)(TP<Explosion>(this, 0x13c)))->reset();
    void *geometry = P(this, 0xc);
    *(volatile int *)B(this, 0x128) = zero;
    *(volatile uint8_t *)B(this, 0xf5) = one;
    if (geometry == 0) {
        geometry = P(this, 0x8);
    }
    ((AEGeometry *)(geometry))->setVisible(true);
}

using AbyssEngine::AEMath::Vector;

void PlayerTurret::setPosition(const Vector &position)
{
    ((AEGeometry *)(TP<AEGeometry>(this, 0x8)))->setPosition(position);
    I(this, 0x58) = *(const int *)B(&position, 0x0);
    I(this, 0x5c) = *(const int *)B(&position, 0x4);
    I(this, 0x60) = *(const int *)B(&position, 0x8);
}

void PlayerTurret::reset()
{
    ((KIPlayer *)(this))->reset();
    I(this, 0x88) = 0;
}

void PlayerTurret::setLevel(Level *level)
{
    ((KIPlayer *)(this))->setLevel(level);
    int manager = I(P(this, 0x54), 0x74);
    int matrix = (int)(intptr_t)(void *)&((AEGeometry *)(TP<AEGeometry>(this, 0x8)))->getReferenceMatrix();
    int system = ((ParticleSystemManager *)(manager))->addSystem((const void *)(long)matrix, 9, 0);
    I(this, 0x138) = system;
    ((ParticleSystemManager *)(I(P(this, 0x54), 0x74)))->enableSystemEmit(system, 0);
}

KIPlayer *PlayerTurret::getHost()
{
    return TP<KIPlayer>(this, 0x154);
}

// PlayerTurret deleting destructor (D0): run the complete dtor, then free.
void _ZN12PlayerTurretD0Ev(PlayerTurret *self)
{
    ::operator delete(((PlayerTurret *)(self))->completeDtor());
}

void PlayerTurret::setScaling(float scale)
{
    ((AEGeometry *)(TP<AEGeometry>(this, 0x148)))->setScaling(scale);
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

    __aeabi_memcpy(matrixBytes, B(this->f_14c, 0x4), 0x3c);
    ((Player *)this->f_14c)->getPosition((Vector *)positionBytes);
    MatrixGetDir((Vector *)dirBytes, matrixBytes);
    VectorNormalize((Vector *)normalBytes, (Vector *)dirBytes);
    *(Vector *)((Vector *)scaledBytes) = *(const Vector *)((Vector *)normalBytes) * (3000.0f);
    *(Vector *)((Vector *)sumBytes) = *(const Vector *)((Vector *)positionBytes) + *(const Vector *)((Vector *)scaledBytes);
    *(Vector *)((Vector *)B(this, 0x9c)) = *(const Vector *)((Vector *)sumBytes);

    if (UC(this, 0x3f) == 0) {
        void *base = ((AEGeometry *)(TP<AEGeometry>(this, 0x8)))->getMatrix();
        void *turret = ((AEGeometry *)(turretGeometry))->getMatrix();
        *(Matrix *)(tmpMatrixA) = *(const Matrix *)(base) * *(const Matrix *)(turret);
        void *main = ((AEGeometry *)(mainGeometry))->getMatrix();
        *(Matrix *)(tmpMatrixB) = *(const Matrix *)(tmpMatrixA) * *(const Matrix *)(main);
        *(Matrix *)(matrixBytes) = *(const Matrix *)(tmpMatrixB);
    } else {
        *(Matrix *)(matrixBytes) = ((AEGeometry *)(TP<AEGeometry>(this, 0x8)))->getMatrix();
    }

    *(AbyssEngine::AEMath::Vector *)positionBytes = AbyssEngine::AEMath::MatrixInverseTransformVector(
        *(const AbyssEngine::AEMath::Matrix *)matrixBytes,
        *(const AbyssEngine::AEMath::Vector *)B(this, 0x9c));
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
        if (UC(this, 0x3f) == 0 && I(this, 0x134) < 100) {
            this->f_150 = this->f_14c;
            I(this, 0x130) = I(this, 0x130) + delta;
            return;
        }
        float step = (float)I(this, 0x124);
        float next = (float)I(this, 0x134) - step;
        I(this, 0x134) = (int)next;
        ((AEGeometry *)(mainGeometry))->rotate(next, 0.0f, step * 0.001f * 0.25f);
        this->f_150 = 0;
        return;
    }
    if (normal->y < -0.05f) {
        if (UC(this, 0x3f) == 0 && I(this, 0x134) > 99) {
            this->f_150 = this->f_14c;
            I(this, 0x130) = I(this, 0x130) + delta;
            return;
        }
        float step = (float)I(this, 0x124);
        float next = (float)I(this, 0x134) + step;
        I(this, 0x134) = (int)next;
        ((AEGeometry *)(mainGeometry))->rotate(next, 0.0f, step * 0.001f * -0.25f);
        this->f_150 = 0;
        return;
    }

    if (ready) {
        ((Player *)(TP<Player>(this, 0x4)))->shoot(0, delta, delta >> 31, 0);
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
    I(this, 0x124) = delta;

    Player *player = TP<Player>(this, 0x4);
    if (((Player *)(player))->isActive() == 0) {
        return;
    }

    if (UC(this, 0x3f) != 0 && I(this, 0x128) < 3000) {
        ((Player *)(player))->setVulnerable(false);
        int time = I(this, 0x128) + delta;
        I(this, 0x128) = time;
        if (time > 2999) {
            ((Player *)(player))->setVulnerable(true);
        }
    }

    if (this->f_154 != 0) {
        __aeabi_memcpy(matrixBytes, B(P(TP<KIPlayer>(this, 0x154), 0x4), 0x4), 0x3c);
        *(AbyssEngine::AEMath::Vector *)vectorBytes = AbyssEngine::AEMath::MatrixRotateVector(
            *(const AbyssEngine::AEMath::Matrix *)matrixBytes,
            *(const AbyssEngine::AEMath::Vector *)B(this, 0x158));
        *(Vector *)((Vector *)B(this, 0x90)) = *(const Vector *)((Vector *)vectorBytes);
        ((AEGeometry *)(TP<AEGeometry>(this, 0x8)))->setMatrix(*(const AbyssEngine::AEMath::Matrix *)matrixBytes);
        ((AEGeometry *)(TP<AEGeometry>(this, 0x8)))->translate(*(Vector *)B(this, 0x90));
    }

    void *matrix = ((AEGeometry *)(TP<AEGeometry>(this, 0x8)))->getMatrix();
    *(Matrix *)(B(player, 0x4)) = *(const Matrix *)(matrix);
    ((AEGeometry *)((Vector *)vectorBytes))->getPosition();
    *(Vector *)((Vector *)B(this, 0x2c)) = *(const Vector *)((Vector *)vectorBytes);

    int hp = ((Player *)(player))->getHitpoints();
    int state = I(this, 0x88);
    if (hp < 1 && (uint32_t)(state - 3) >= 2) {
        I(this, 0x12c) = 0;
        I(this, 0x88) = 3;
        // The original reads play()'s sound-handle return into `value` and forwards it as the
        // (unused) p4 selector to emitManual; the modeled play() is void, so just emit the call.
        ((FModSound *)(**(int **)gPlayerTurretSound))->play(0x16, 0, 0, 0.0f);
        float value = 0.0f;
        Vector zero = {0.0f, 0.0f, 0.0f};
        ((ParticleSystemManager *)(I(this->f_54, 0x74)))->emitManual(I(this->f_54, 0x3c), (const float *)B(this, 0x2c), (int)(long)&zero, value);
        ((ParticleSystemManager *)(I(this->f_54, 0x74)))->enableSystemEmit(I(this, 0x138), 1);
        ((Explosion *)(TP<Explosion>(this, 0x13c)))->start((Vector *)B(this, 0x2c), &zero);

        int random = AbyssEngine::AERandom::nextInt(*gPlayerTurretRandom, 100);
        if (random < 0) {
            UC(this, 0x4c) = 1;
            Array<int> *array = new Array<int>();
            this->f_50 = array;
            array->push_back(99);
            this->f_50->push_back(AbyssEngine::AERandom::nextInt(*gPlayerTurretRandom, 10) + 1);
            ((KIPlayer *)(this))->createCrate(3);
            UC(this, 0x4c) = 1;
        } else {
            int levelPlayer = ((Level *)((Level *)this->f_54))->getPlayer();
            if (levelPlayer != 0) {
                levelPlayer = ((Level *)((Level *)this->f_54))->getPlayer();
                if (I((void *)levelPlayer, 0x14) != 0) {
                    levelPlayer = ((Level *)((Level *)this->f_54))->getPlayer();
                    if (P(P((void *)levelPlayer, 0x14), 0x1c) == this) {
                        levelPlayer = ((Level *)((Level *)this->f_54))->getPlayer();
                        P(P((void *)levelPlayer, 0x14), 0x1c) = 0;
                    }
                }
            }
        }
        state = I(this, 0x88);
    }

    if (state == 4) {
        return;
    }
    if (state == 3) {
        I(this, 0x12c) = I(this, 0x12c) + delta;
        ((Explosion *)(TP<Explosion>(this, 0x13c)))->update(delta, 0);
        if (I(this, 0x12c) > 4500) {
            ((ParticleSystemManager *)(I(this->f_54, 0x74)))->enableSystemEmit(I(this, 0x138), 0);
            I(this, 0x12c) = 0;
            I(this, 0x88) = 4;
            ((Player *)(player))->setActive(false);
            if (UC(this, 0x3f) != 0) {
                I(this->f_54, 0x6c) = I(this->f_54, 0x6c) - 1;
            }
        }
        return;
    }

    uint32_t standing = U(this, 0x28);
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

    if (UC(this, 0x3f) != 0) {
        handleSentryGun(delta);
    } else if (UC(this, 0x25) != 0 && this->f_144 != 0) {
        handleTurret(delta);
    }
}

using AbyssEngine::AEMath::Vector;

void PlayerTurret::pickEnemy()
{
    char vectorFrame[24];

    if (I(this, 0x130) > 3000) {
        int bestRange = I(this, 0x164);
        I(this, 0x130) = 0;
        this->f_14c = 0;

        Array<Player *> *enemies = ((Player *)(TP<Player>(this, 0x4)))->getEnemies();
        if (enemies != 0) {
            Vector *position = (Vector *)B(this, 0x2c);
            uint32_t i = 0;
            goto check;
        next:
            {
                Player *enemy = (*enemies)[i];

                if (((Player *)(enemy))->isDead() == 0 && ((Player *)(enemy))->isActive() != 0) {
                    bool accepted = false;
                    if (UC(enemy, 0x69) != 0 && UC(TP<Player>(this, 0x4), 0x5c) != 0) {
                        accepted = true;
                    } else if (UC(this, 0x3f) == 0) {
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
                            void *current = this->f_14c;
                            if (current == 0 || current != this->f_150) {
                                bestRange = distance;
                                this->f_14c = enemy;
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
    this->f_0 = (char *)gPlayerTurretVtable + 8;

    Explosion *explosion = TP<Explosion>(this, 0x13c);
    if (explosion != 0) {
        ::operator delete(Explosion_dtor(explosion));
    }
    this->f_13c = 0;

    AEGeometry *base = TP<AEGeometry>(this, 0x140);
    if (base != 0) {
        base->~AEGeometry(); ::operator delete(base);
    }
    this->f_140 = 0;

    AEGeometry *turret = TP<AEGeometry>(this, 0x144);
    if (turret != 0) {
        turret->~AEGeometry(); ::operator delete(turret);
    }
    this->f_144 = 0;

    AEGeometry *helper = TP<AEGeometry>(this, 0x148);
    if (helper != 0) {
        helper->~AEGeometry(); ::operator delete(helper);
    }
    this->f_148 = 0;

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

    this->f_0 = (char *)gPlayerTurretVtable_ctor + 8;
    UC(this, 0x25) = 1;
    UC(this, 0x3e) = 1;
    this->f_140 = 0;
    this->f_144 = 0;
    I(this, 0x128) = 0;
    this->f_154 = 0;
    this->f_158 = 0;
    this->f_15c = 0;
    I(this, 0x164) = 50000;

    AEGeometry *base = (AEGeometry *)operator new(0xc0);
    uint32_t *canvasHolder = gPlayerTurretCanvas;
    new ((void *)base) AEGeometry((uint16_t)mesh, (PaintCanvas *)(void *)*canvasHolder, false);
    this->f_140 = base;

    if (mesh == 0x381b) {
        AEGeometry *turret = (AEGeometry *)operator new(0xc0);
        new ((void *)turret) AEGeometry((uint16_t)0x381c, (PaintCanvas *)(void *)*canvasHolder, false);
        this->f_144 = turret;
        turret->setRotationOrder(2);
        Vector *v = (Vector *)vectorBytes;
        v->x = 0.0f;
        v->y = 0.0f;
        v->z = 0.0f;
        ((AEGeometry *)(turret))->setPosition(*v);
    } else if (mesh == 0x1a76) {
        AEGeometry *turret = (AEGeometry *)operator new(0xc0);
        new ((void *)turret) AEGeometry((uint16_t)0x1a77, (PaintCanvas *)(void *)*canvasHolder, false);
        this->f_144 = turret;
        turret->setRotationOrder(2);
        Vector *v = (Vector *)vectorBytes;
        v->x = 0.0f;
        v->y = 0.0f;
        v->z = 0.0f;
        ((AEGeometry *)(turret))->setPosition(*v);
    } else if (mesh == 0x1a74) {
        AEGeometry *turret = (AEGeometry *)operator new(0xc0);
        new ((void *)turret) AEGeometry((uint16_t)0x1a75, (PaintCanvas *)(void *)*canvasHolder, false);
        this->f_144 = turret;
        turret->setRotationOrder(2);
        Vector *v = (Vector *)vectorBytes;
        v->x = 0.0f;
        v->y = 0.0f;
        v->z = 0.0f;
        ((AEGeometry *)(turret))->setPosition(*v);
    }

    AEGeometry *helper = (AEGeometry *)operator new(0xc0);
    new ((void *)helper) AEGeometry((PaintCanvas *)(void *)*canvasHolder);
    this->f_148 = helper;
    uint32_t transform = (uint32_t)(uintptr_t)((PaintCanvas*)(long)*canvasHolder)->TransformGetTransform(U(helper, 0xc));
    I((void *)transform, 0xe0) = 0;

    Vector *initial = (Vector *)vectorBytes;
    initial->x = x;
    initial->y = y;
    initial->z = z;
    SetPositionFn setPosition = *(SetPositionFn *)((char *)this->f_0 + 0x44);
    setPosition(this, initial);

    if (mesh == 0x381b) {
        ((AEGeometry *)(TP<AEGeometry>(this, 0x140)))->rotate(0.0f, 0.0f, 0.0f);
        ((AEGeometry *)(TP<AEGeometry>(this, 0x144)))->rotate(0.0f, 0.0f, 0.0f);
        ((AEGeometry *)(TP<AEGeometry>(this, 0x148)))->addChild(U(this->f_144, 0xc));
    } else if ((mesh | 2U) == 0x1a76) {
        ((AEGeometry *)(TP<AEGeometry>(this, 0x148)))->addChild(U(this->f_144, 0xc));
    } else if ((uint32_t)(mesh - 0x49c0) < 3) {
        UC(this, 0x3f) = 1;
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

    ((AEGeometry *)(TP<AEGeometry>(this, 0x148)))->addChild(U(this->f_140, 0xc));
    if (UC(this, 0x3f) == 0) {
        ((AEGeometry *)(geometry))->addChild(U(this->f_148, 0xc));
    }

    Explosion *explosion = (Explosion *)operator new(0x68);
    Explosion_ctor(explosion, 0);
    this->f_13c = explosion;
    ((Explosion *)(explosion))->addFireStreaks();

    this->f_14c = 0;
    this->f_150 = 0;
    I(this, 0x12c) = 0;
    I(this, 0x130) = 0;
    I(this, 0x134) = 0;
    I(this, 0x138) = 0;
}
