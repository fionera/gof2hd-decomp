#include "gof2/PlayerTurret.h"
#include "gof2/Explosion.h"
#include "gof2/KIPlayer.h"
#include "gof2/Player.h"
#include "gof2/Standing.h"


extern "C" void PlayerTurret_pickEnemy(PlayerTurret *self);
extern "C" void PlayerTurret_handleRotation(PlayerTurret *self, int delta, AEGeometry *main, AEGeometry *turret);
extern "C" Vector *Vector_assign(Vector *dst, const Vector *src);
extern "C" void AEGeometry_render(void *self);
extern "C" void PlayerTurret_renderBase(PlayerTurret *self);
extern "C" void Player_reset(Player *self);
extern "C" void AEGeometry_setVisible(void *self, bool visible);
extern "C" void AEGeometry_setPosition(AEGeometry *self, const Vector *position);
extern "C" void KIPlayer_setLevel(PlayerTurret *self, Level *level);
extern "C" int AEGeometry_getReferenceMatrix(AEGeometry *self);
extern "C" int ParticleSystemManager_addSystem(int manager, int matrix, int kind, int flags);
extern "C" void ParticleSystemManager_enableSystemEmit(int manager, int system, int enable);
extern "C" PlayerTurret *PlayerTurret_completeDtor(PlayerTurret *self);
extern "C" void PlayerTurret_operator_delete(void *ptr);
extern "C" void AEGeometry_setScaling(AEGeometry *self, float x, float y, float z);
extern "C" void *AEGeometry_getMatrix(AEGeometry *self);
extern "C" void AEGeometry_rotate(AEGeometry *self, float x, float y, float z);
extern "C" void *KIPlayer_dtor(void *self);
extern "C" void Player_getPosition(Vector *out, Player *self);
void MatrixGetDir(Vector *out, const void *matrix);
void VectorNormalize(Vector *out, const Vector *v);
extern "C" void Vector_scale(Vector *out, const Vector *v, float value);
extern "C" void Vector_add(Vector *out, const Vector *a, const Vector *b);
extern "C" void Matrix_assign(void *dst, const void *src);
extern "C" void Matrix_mul(void *out, const void *a, const void *b);
void MatrixInverseTransformVector(Vector *out, const void *matrix, const Vector *v);
extern "C" uint32_t PaintCanvas_TransformGetTransform(uint32_t canvas, uint32_t transform);
extern "C" void Transform_Update(uint32_t transform, int amount, int amountHigh, int flags);
extern "C" int Level_getPlayer(Level *self);
extern "C" int Status_getStanding();
extern "C" void AEGeometry_setMatrix(AEGeometry *self, const void *matrix);
extern "C" void AEGeometry_translate(AEGeometry *self, const Vector *position);
extern "C" void AEGeometry_getPosition(Vector *out, AEGeometry *self);
void MatrixRotateVector(Vector *out, const void *matrix, const Vector *vector);
extern "C" void *FModSound_play(int sound, int id, Vector *pos, float volume);
extern "C" void Explosion_update(Explosion *self, int delta, TargetFollowCamera *camera);
extern "C" int AERandom_nextInt(int rng, int max);
extern "C" void Array_int_ctor(IntArray *array);
extern "C" void ArrayAdd_int(int value, IntArray *array);
extern "C" PlayerArray *Player_getEnemies(Player *self);
extern "C" void Vector_sub(Vector *out, const Vector *a, const Vector *b);
float VectorLength(const Vector *v);
extern "C" void *Explosion_dtor(Explosion *self);
extern "C" void *AEGeometry_dtor(AEGeometry *self);
extern "C" void AEGeometry_ctorMesh(AEGeometry *self, uint16_t mesh, void *canvas, bool flag);
extern "C" void AEGeometry_ctor(AEGeometry *self, void *canvas);
extern "C" void AEGeometry_setRotationOrder(AEGeometry *self, int order);
extern "C" void AEGeometry_addChild(AEGeometry *self, uint32_t child);
extern "C" void Explosion_ctor(Explosion *self, int kind);

// ---- setTurretRange_157418.cpp ----

void PlayerTurret::setTurretRange(int range)
{
    I(this, 0x164) = range;
}

// ---- handleSentryGun_15774c.cpp ----

void PlayerTurret::handleSentryGun(int delta)
{
    I(this, 0x130) = I(this, 0x130) + delta;
    PlayerTurret_pickEnemy(this);
    void *enemy = P(this, 0x14c);
    if (enemy != 0 && UC(enemy, 0x5e) == 0) {
        AEGeometry *geometry = TP<AEGeometry>(this, 0x8);
        PlayerTurret_handleRotation(this, delta, geometry, geometry);
    }
}

// ---- setHost_15741e.cpp ----
using AbyssEngine::AEMath::Vector;



void PlayerTurret::setHost(KIPlayer *host, const Vector &offset)
{
    P(this, 0x154) = host;
    Vector_assign((Vector *)B(this, 0x158), &offset);
}

// ---- render_157bcc.cpp ----

void PlayerTurret::render()
{
    void *visible = P(this, 0x78);
    if (visible != 0) {
        AEGeometry_render(visible);
    }
    int state = I(this, 0x88);
    if (state == 3) {
        ((Explosion *)(TP<Explosion>(this, 0x13c)))->render();
        state = I(this, 0x88);
    }
    if ((uint32_t)(state - 3) >= 2) {
        PlayerTurret_renderBase(this);
    }
}

// ---- handleTurret_157782.cpp ----

void PlayerTurret::handleTurret(int delta)
{
    I(this, 0x130) = I(this, 0x130) + delta;
    PlayerTurret_pickEnemy(this);
    void *enemy = P(this, 0x14c);
    if (enemy != 0 && UC(enemy, 0x5e) == 0) {
        PlayerTurret_handleRotation(this, delta, TP<AEGeometry>(this, 0x148), TP<AEGeometry>(this, 0x144));
    }
}

// ---- revive_157c08.cpp ----

void PlayerTurret::revive()
{
    int zero = 0;
    int one = 1;
    Player_reset(TP<Player>(this, 0x4));
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
    AEGeometry_setVisible(geometry, true);
}

// ---- setPosition_1573fc.cpp ----
using AbyssEngine::AEMath::Vector;



void PlayerTurret::setPosition(const Vector &position)
{
    AEGeometry_setPosition(TP<AEGeometry>(this, 0x8), &position);
    I(this, 0x58) = *(const int *)B(&position, 0x0);
    I(this, 0x5c) = *(const int *)B(&position, 0x4);
    I(this, 0x60) = *(const int *)B(&position, 0x8);
}

// ---- reset_157432.cpp ----

void PlayerTurret::reset()
{
    ((KIPlayer *)(this))->reset();
    I(this, 0x88) = 0;
}

// ---- setLevel_1573bc.cpp ----

void PlayerTurret::setLevel(Level *level)
{
    KIPlayer_setLevel(this, level);
    int manager = I(P(this, 0x54), 0x74);
    int matrix = AEGeometry_getReferenceMatrix(TP<AEGeometry>(this, 0x8));
    int system = ParticleSystemManager_addSystem(manager, matrix, 9, 0);
    I(this, 0x138) = system;
    ParticleSystemManager_enableSystemEmit(I(P(this, 0x54), 0x74), system, 0);
}

// ---- getHost_15742c.cpp ----

KIPlayer *PlayerTurret::getHost()
{
    return TP<KIPlayer>(this, 0x154);
}

// ---- _PlayerTurret_1573ac.cpp ----
// PlayerTurret deleting destructor (D0): run the complete dtor, then free.
void _ZN12PlayerTurretD0Ev(PlayerTurret *self)
{
    PlayerTurret_operator_delete(PlayerTurret_completeDtor(self));
}

// ---- setScaling_1573f0.cpp ----

void PlayerTurret::setScaling(float scale)
{
    AEGeometry_setScaling(TP<AEGeometry>(this, 0x148), scale, scale, scale);
}

// ---- handleRotation_157908.cpp ----
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
    Player_getPosition((Vector *)positionBytes, (Player *)this->f_14c);
    MatrixGetDir((Vector *)dirBytes, matrixBytes);
    VectorNormalize((Vector *)normalBytes, (Vector *)dirBytes);
    Vector_scale((Vector *)scaledBytes, (Vector *)normalBytes, 3000.0f);
    Vector_add((Vector *)sumBytes, (Vector *)positionBytes, (Vector *)scaledBytes);
    Vector_assign((Vector *)B(this, 0x9c), (Vector *)sumBytes);

    if (UC(this, 0x3f) == 0) {
        void *base = AEGeometry_getMatrix(TP<AEGeometry>(this, 0x8));
        void *turret = AEGeometry_getMatrix(turretGeometry);
        Matrix_mul(tmpMatrixA, base, turret);
        void *main = AEGeometry_getMatrix(mainGeometry);
        Matrix_mul(tmpMatrixB, tmpMatrixA, main);
        Matrix_assign(matrixBytes, tmpMatrixB);
    } else {
        Matrix_assign(matrixBytes, AEGeometry_getMatrix(TP<AEGeometry>(this, 0x8)));
    }

    MatrixInverseTransformVector((Vector *)positionBytes, matrixBytes, (Vector *)B(this, 0x9c));
    VectorNormalize((Vector *)sumBytes, (Vector *)positionBytes);
    Vector *normal = (Vector *)sumBytes;

    bool ready = false;
    float yaw = 0.0f;
    if (normal->x > 0.0f) {
        yaw = (float)delta;
        AEGeometry_rotate(turretGeometry, 0.0f, yaw * 0.001f * 0.25f, 0.0f);
    } else if (normal->x < -0.05f) {
        yaw = (float)-delta;
        AEGeometry_rotate(turretGeometry, 0.0f, yaw * 0.001f * 0.25f, 0.0f);
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
        AEGeometry_rotate(mainGeometry, next, 0.0f, step * 0.001f * 0.25f);
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
        AEGeometry_rotate(mainGeometry, next, 0.0f, step * 0.001f * -0.25f);
        this->f_150 = 0;
        return;
    }

    if (ready) {
        ((Player *)(TP<Player>(this, 0x4)))->shoot(0, delta, delta >> 31, 0);
        uint32_t transform = PaintCanvas_TransformGetTransform(*gPlayerTurretCanvas_rotation,
                                                               U(turretGeometry, 0xc));
        Transform_Update(transform, delta, delta >> 31, 0);
    }
}

// ---- update_157450.cpp ----
using AbyssEngine::AEMath::Matrix;
using AbyssEngine::AEMath::Vector;

extern "C" void ParticleSystemManager_emitManual(int manager, int settings, Vector *pos,
                                                 Vector *velocity, float value);

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
        MatrixRotateVector((Vector *)vectorBytes, matrixBytes, (Vector *)B(this, 0x158));
        Vector_assign((Vector *)B(this, 0x90), (Vector *)vectorBytes);
        AEGeometry_setMatrix(TP<AEGeometry>(this, 0x8), matrixBytes);
        AEGeometry_translate(TP<AEGeometry>(this, 0x8), (Vector *)B(this, 0x90));
    }

    void *matrix = AEGeometry_getMatrix(TP<AEGeometry>(this, 0x8));
    Matrix_assign(B(player, 0x4), matrix);
    AEGeometry_getPosition((Vector *)vectorBytes, TP<AEGeometry>(this, 0x8));
    Vector_assign((Vector *)B(this, 0x2c), (Vector *)vectorBytes);

    int hp = ((Player *)(player))->getHitpoints();
    int state = I(this, 0x88);
    if (hp < 1 && (uint32_t)(state - 3) >= 2) {
        I(this, 0x12c) = 0;
        I(this, 0x88) = 3;
        float value = (float)(uint32_t)(__UINTPTR_TYPE__)FModSound_play(**(int **)gPlayerTurretSound, 0x16, 0, 0.0f);
        Vector zero = {0.0f, 0.0f, 0.0f};
        ParticleSystemManager_emitManual(I(this->f_54, 0x74), I(this->f_54, 0x3c),
                                         (Vector *)B(this, 0x2c), &zero, value);
        ParticleSystemManager_enableSystemEmit(I(this->f_54, 0x74), I(this, 0x138), 1);
        ((Explosion *)(TP<Explosion>(this, 0x13c)))->start((Vector *)B(this, 0x2c), &zero);

        int random = AERandom_nextInt(*gPlayerTurretRandom, 100);
        if (random < 0) {
            UC(this, 0x4c) = 1;
            IntArray *array = (IntArray *)operator new(0xc);
            Array_int_ctor(array);
            this->f_50 = array;
            ArrayAdd_int(99, array);
            ArrayAdd_int(AERandom_nextInt(*gPlayerTurretRandom, 10) + 1, (IntArray *)this->f_50);
            ((KIPlayer *)(this))->createCrate(3);
            UC(this, 0x4c) = 1;
        } else {
            int levelPlayer = Level_getPlayer((Level *)this->f_54);
            if (levelPlayer != 0) {
                levelPlayer = Level_getPlayer((Level *)this->f_54);
                if (I((void *)levelPlayer, 0x14) != 0) {
                    levelPlayer = Level_getPlayer((Level *)this->f_54);
                    if (P(P((void *)levelPlayer, 0x14), 0x1c) == this) {
                        levelPlayer = Level_getPlayer((Level *)this->f_54);
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
        Explosion_update(TP<Explosion>(this, 0x13c), delta, 0);
        if (I(this, 0x12c) > 4500) {
            ParticleSystemManager_enableSystemEmit(I(this->f_54, 0x74), I(this, 0x138), 0);
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
        int s = Status_getStanding();
        UC(player, 0x5c) = ((Standing *)(s))->isEnemy(standing);
        if ((standing & 0xfffffffeU) == 8) {
            UC(player, 0x5d) = 0;
        } else {
            UC(player, 0x5d) = ((Standing *)(Status_getStanding()))->isFriend(standing);
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

// ---- pickEnemy_1577b8.cpp ----
using AbyssEngine::AEMath::Vector;



void PlayerTurret::pickEnemy()
{
    char vectorFrame[24];

    if (I(this, 0x130) > 3000) {
        int bestRange = I(this, 0x164);
        I(this, 0x130) = 0;
        this->f_14c = 0;

        PlayerArray *enemies = Player_getEnemies(TP<Player>(this, 0x4));
        if (enemies != 0) {
            Vector *position = (Vector *)B(this, 0x2c);
            uint32_t i = 0;
            goto check;
        next:
            {
                Player *enemy = enemies->data[i];

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
                        Player_getPosition((Vector *)vectorFrame, enemy);
                        Vector_sub((Vector *)(vectorFrame + 12), position, (Vector *)vectorFrame);
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
            if (i < enemies->length) {
                goto next;
            }
        }
    }
}

// ---- _PlayerTurret_157340.cpp ----
extern void *gPlayerTurretVtable __attribute__((visibility("hidden")));



PlayerTurret::~PlayerTurret() noexcept(false)
{
    this->f_0 = (char *)gPlayerTurretVtable + 8;

    Explosion *explosion = TP<Explosion>(this, 0x13c);
    if (explosion != 0) {
        PlayerTurret_operator_delete(Explosion_dtor(explosion));
    }
    this->f_13c = 0;

    AEGeometry *base = TP<AEGeometry>(this, 0x140);
    if (base != 0) {
        PlayerTurret_operator_delete(AEGeometry_dtor(base));
    }
    this->f_140 = 0;

    AEGeometry *turret = TP<AEGeometry>(this, 0x144);
    if (turret != 0) {
        PlayerTurret_operator_delete(AEGeometry_dtor(turret));
    }
    this->f_144 = 0;

    AEGeometry *helper = TP<AEGeometry>(this, 0x148);
    if (helper != 0) {
        PlayerTurret_operator_delete(AEGeometry_dtor(helper));
    }
    this->f_148 = 0;

    KIPlayer_dtor(this);
}

// ---- PlayerTurret_157040.cpp ----
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
    AEGeometry_ctorMesh(base, (uint16_t)mesh, (void *)*canvasHolder, false);
    this->f_140 = base;

    if (mesh == 0x381b) {
        AEGeometry *turret = (AEGeometry *)operator new(0xc0);
        AEGeometry_ctorMesh(turret, 0x381c, (void *)*canvasHolder, false);
        this->f_144 = turret;
        AEGeometry_setRotationOrder(turret, 2);
        Vector *v = (Vector *)vectorBytes;
        v->x = 0.0f;
        v->y = 0.0f;
        v->z = 0.0f;
        AEGeometry_setPosition(turret, v);
    } else if (mesh == 0x1a76) {
        AEGeometry *turret = (AEGeometry *)operator new(0xc0);
        AEGeometry_ctorMesh(turret, 0x1a77, (void *)*canvasHolder, false);
        this->f_144 = turret;
        AEGeometry_setRotationOrder(turret, 2);
        Vector *v = (Vector *)vectorBytes;
        v->x = 0.0f;
        v->y = 0.0f;
        v->z = 0.0f;
        AEGeometry_setPosition(turret, v);
    } else if (mesh == 0x1a74) {
        AEGeometry *turret = (AEGeometry *)operator new(0xc0);
        AEGeometry_ctorMesh(turret, 0x1a75, (void *)*canvasHolder, false);
        this->f_144 = turret;
        AEGeometry_setRotationOrder(turret, 2);
        Vector *v = (Vector *)vectorBytes;
        v->x = 0.0f;
        v->y = 0.0f;
        v->z = 0.0f;
        AEGeometry_setPosition(turret, v);
    }

    AEGeometry *helper = (AEGeometry *)operator new(0xc0);
    AEGeometry_ctor(helper, (void *)*canvasHolder);
    this->f_148 = helper;
    uint32_t transform = PaintCanvas_TransformGetTransform(*canvasHolder, U(helper, 0xc));
    I((void *)transform, 0xe0) = 0;

    Vector *initial = (Vector *)vectorBytes;
    initial->x = x;
    initial->y = y;
    initial->z = z;
    SetPositionFn setPosition = *(SetPositionFn *)((char *)this->f_0 + 0x44);
    setPosition(this, initial);

    if (mesh == 0x381b) {
        AEGeometry_rotate(TP<AEGeometry>(this, 0x140), 0.0f, 0.0f, 0.0f);
        AEGeometry_rotate(TP<AEGeometry>(this, 0x144), 0.0f, 0.0f, 0.0f);
        AEGeometry_addChild(TP<AEGeometry>(this, 0x148), U(this->f_144, 0xc));
    } else if ((mesh | 2U) == 0x1a76) {
        AEGeometry_addChild(TP<AEGeometry>(this, 0x148), U(this->f_144, 0xc));
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
        AEGeometry_ctorMesh(child, childMesh, (void *)*canvasHolder, false);
        AEGeometry_addChild(geometry, U(child, 0xc));
        PlayerTurret_operator_delete(AEGeometry_dtor(child));
        AEGeometry_setScaling(geometry, 0.5f, 0.5f, 0.5f);
    }

    AEGeometry_addChild(TP<AEGeometry>(this, 0x148), U(this->f_140, 0xc));
    if (UC(this, 0x3f) == 0) {
        AEGeometry_addChild(geometry, U(this->f_148, 0xc));
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
