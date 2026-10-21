#include "game/ship/KIPlayer.h"
#include "engine/math/AEMath.h"
#include "game/ui/Hud.h"
#include "game/ship/PlayerEgo.h"
#include "game/world/Level.h"
#include "game/mission/Achievements.h"
#include "game/ship/PlayerGasCloud.h"
#include "game/mission/Status.h"
#include "engine/render/PaintCanvas.h"
#include "engine/math/Transform.h"
#include "game/core/Globals.h"
#include "game/weapons/Gun.h"
#include <new>
#include "engine/core/AERandom.h"
#include <cstdint>
#include "game/ship/Player.h"
#include "game/mission/Item.h"
#include "engine/render/Sparks.h"
#include "engine/render/AEGeometry.h"
#include "engine/core/Array.h"
#include "engine/math/Vector.h"

namespace {
    struct GunItemRegistry {
        int32_t field_0x0;
        int32_t *itemTable;
        uint8_t pad_0x08[0xc8 - 0x08];
        int32_t nukeDetonations;
    };

    struct GunUpdateGlobals {
        uint8_t pad_0x00[0x12c];
        int32_t field_0x12c;
    };

    struct GunLevelHandle {
        uint8_t pad_0x00[0x69];
        uint8_t field_0x69;
    };

    struct GunTransformHandle {
        uint8_t pad_0x00[0xed];
        uint8_t visible_0xed;
    };

#if __SIZEOF_POINTER__ == 4
    static_assert(__builtin_offsetof(GunItemRegistry, itemTable) == 0x04,
                  "GunItemRegistry::itemTable offset");
    static_assert(__builtin_offsetof(GunItemRegistry, nukeDetonations) == 0xc8,
                  "GunItemRegistry::nukeDetonations offset");
    static_assert(__builtin_offsetof(GunUpdateGlobals, field_0x12c) == 0x12c,
                  "GunUpdateGlobals::field_0x12c offset");
    static_assert(__builtin_offsetof(GunLevelHandle, field_0x69) == 0x69,
                  "GunLevelHandle::field_0x69 offset");
    static_assert(__builtin_offsetof(GunTransformHandle, visible_0xed) == 0xed,
                  "GunTransformHandle::visible_0xed offset");
#endif
}

typedef Array<Vector> VecArray;

static inline void Gun_VecArray_ctor(VecArray *a) { new(a) VecArray(); }

void Gun_VecPtrArray_ctor(Array<int> * a);

void Gun_VecArray_setLength(int n, VecArray *a);

void Gun_VecPtrArray_setLength(int n, Array<int> *a);

Gun::~Gun() noexcept(false) {
    delete[] this->lifetimes;
    this->lifetimes = 0;

    delete[] this->hitFlags;
    this->hitFlags = 0;

    delete[] this->geometries;
    this->geometries = 0;

    delete[] this->randomFlags;
    this->randomFlags = 0;

    Array<Vector *> *arr = reinterpret_cast<Array<Vector *> *>(this->wobbleOffsets);
    if (arr != 0) {
        ArrayReleaseClasses(*arr);
        Array<Vector *> *arr2 = reinterpret_cast<Array<Vector *> *>(this->wobbleOffsets);
        if (arr2 != 0) {
            delete arr2;
        }
    }
    this->wobbleOffsets = 0;

    reinterpret_cast<VecArray *>(&this->hitPositionsCount)->~VecArray();
    reinterpret_cast<VecArray *>(&this->upVectorsCount)->~VecArray();
    reinterpret_cast<VecArray *>(&this->velocitiesCount)->~VecArray();
    reinterpret_cast<VecArray *>(&this->count)->~VecArray();
}

void Gun::setFriendGun(bool v) {
    this->friendGun = v;
}

int Gun::getMagnitude() {
    return this->magnitude;
}

void *Gun::getEnemies() { // lint: void_ptr exported method return type baked into header; consumed as void* in RocketGun.cpp
    // lint: void_ptr exported method return type baked into header; consumed as void* in RocketGun.cpp
    return this->enemies;
}

void Gun::setEnemies(Array<Player *> *enemies) {
    this->enemies = enemies;
}

void Gun::setMagnitude(int v) {
    this->magnitude = v;
}

void Gun::setErrorMagnitudePercentage(int v) {
    this->errorMagnitudePercentage = (float) v;
}

void Gun::setImpact(Sparks *impact) {
    this->impact = impact;
}

void Gun::setPlayerGun(bool v) {
    this->playerGun = v;
}

uint8_t Gun::isPlayerGun() {
    return this->playerGun;
}

void Gun::setLevelCollision(bool v) {
    this->levelCollision = v;
}

void Gun::setLevel(Level *lvl) {
    this->level = lvl;
}

void Gun::removeAllEnemies() {
    this->enemies = 0;
}

// Projectile mesh id per item index (-1: weapon has no visible projectile mesh).
// 233 entries
static const int kGunProjectileMeshes[233] = {
    14600, 14600, 14600, 14601, 14601, 14602, 14602, 14603, 14603, 14600, 14603, 14602, 14601, 14601, 14601, 14601,
    14604, 14604, 14604, 14605, 14605, 14605, 14606, 14606, 14606, 14606, 14606, 14606, 14606, 14605, 14602, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 14603,
    14605, 14601, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, 14600, 14600, 14600, 14603, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, 14600, -1, -1, -1, 14600, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 14600, -1, -1,
    14600, -1, -1, -1, 14601, 14601, 14606, 14602, -1,
};

void Gun::setIndex(int index) {
    this->itemIndex = index;
    this->homing = index == 228 || (unsigned) (index - 9) < 3;
    this->empDamage = (*Globals::items)[index]->getAttribute(10);
    int mesh = kGunProjectileMeshes[index];
    if (mesh >= 0) {
        this->geometries = new int[this->count];
        this->randomFlags = new uint8_t[this->count];
        for (unsigned i = 0; i < this->count; ++i) {
            AEGeometry *geom = new AEGeometry((uint16_t) mesh, Globals::Canvas, false);
            this->geometries[i] = geom->transform;
            this->randomFlags[i] = Globals::rnd->nextInt(2) == 0;
            ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(this->geometries[i]))
                    ->SetAnimationState((AbyssEngine::AnimationMode) 0, nullptr);
            delete geom;
        }
    }
}

static const short gSO_table[10][10][3] = {
        {{0,   0,    0},   {0,   0,    0},   {0,    0,    0},   {0,    0,    0},   {0,    0,    0},   {0,    0,    0},   {0,    0,    0},   {0,    0,    0},   {0,    0,    0},   {0,    0,    0}},
        {{270, 0,    100}, {-270, 0,   100}, {0,    0,    0},   {0,    0,    0},   {0,    0,    0},   {0,    0,    0},   {0,    0,    0},   {0,    0,    0},   {0,    0,    0},   {0,    0,    0}},
        {{320, 0,    100}, {0,   0,    100}, {-320, 0,    100}, {0,    0,    0},   {0,    0,    0},   {0,    0,    0},   {0,    0,    0},   {0,    0,    0},   {0,    0,    0},   {0,    0,    0}},
        {{370, 0,    100}, {170, 100,  150}, {-170, 100,  150}, {-370, 0,    100}, {0,    0,    0},   {0,    0,    0},   {0,    0,    0},   {0,    0,    0},   {0,    0,    0},   {0,    0,    0}},
        {{350, 100,  200}, {270, -100, 150}, {0,    0,    100}, {-270, -100, 150}, {-350, 100,  200}, {0,    0,    0},   {0,    0,    0},   {0,    0,    0},   {0,    0,    0},   {0,    0,    0}},
        {{500, 100,  150}, {500, -100, 150}, {200,  0,    100}, {-200, 0,    100}, {-500, -100, 150}, {-500, 100,  150}, {0,    0,    0},   {0,    0,    0},   {0,    0,    0},   {0,    0,    0}},
        {{500, -100, 150}, {400, 100,  150}, {200,  -50,  100}, {0,    0,    150}, {-200, -50,  100}, {-400, 100,  150}, {-500, -100, 150}, {0,    0,    0},   {0,    0,    0},   {0,    0,    0}},
        {{550, -100, 100}, {550, 100,  100}, {300,  0,    100}, {100,  0,    150}, {-100, 0,    150}, {-300, 0,    100}, {-550, 100,  100}, {-550, -100, 100}, {0,    0,    0},   {0,    0,    0}},
        {{550, -100, 100}, {550, 100,  100}, {300,  0,    100}, {100,  100,  150}, {0,    -100, 100}, {-100, 100,  150}, {-300, 0,    100}, {-550, 100,  100}, {-550, -100, 100}, {0,    0,    0}},
        {{600, -120, 100}, {600, 120,  100}, {400,  0,    100}, {200,  100,  150}, {200,  -100, 100}, {-200, -100, 100}, {-200, 100,  150}, {-400, 0,    100}, {-600, 120,  100}, {-600, -120, 100}},
};

void Gun::setOffset(int a, int b) {
    const short *row = gSO_table[b - 1][a];
    float fx = (float) row[0];
    float fy = (float) row[1];
    float fz = (float) row[2];
    float sx = this->offset.x + fx;
    float sy = this->offset.y + fy;
    float sz = this->offset.z + fz;
    Vector local;
    local.x = sx;
    local.y = sy;
    local.z = sz;
    this->offset = local;
}

// Item attribute value meaning "attribute not present".
static const int kNoAttribute = -979797979;

void Gun::ignite() {
    switch (this->weaponType) {
        case ITEM_SORT_NUKE:
            Globals::status->field_c8++;
            // fall through
        case ITEM_SORT_EMP_BOMB:
            reinterpret_cast<GunLevelHandle *>(this->level)->field_0x69 = 0;
            break;
    }
    this->ignited = 1;
    if (this->enemies != 0) {
        this->lastHitKIPlayer = 0;
        for (unsigned e = 0; e < this->enemies->size(); ++e) {
            this->target = (*this->enemies)[e];
            if (this->weaponType == ITEM_SORT_EMP_BOMB && this->target->isAsteroid()) {
                continue;
            }
            if (!this->target->isActive()) {
                continue;
            }
            for (unsigned i = 0; i < this->count; ++i) {
                this->basePos = Vector(reinterpret_cast<Vector *>(this->positions)[i]);
                this->targetDir = this->target->getPosition();
                this->targetDir -= this->basePos;
                int distance = (int) AbyssEngine::AEMath::VectorLength(this->targetDir);
                if (distance < this->magnitude) {
                    this->hitFlags[i] = 1;
                    reinterpret_cast<Vector *>(this->hitPositions)[i] = this->basePos;
                    float range = (float) this->magnitude;
                    float remaining = (float) (this->magnitude - distance);
                    float falloff;
                    if (this->weaponType == ITEM_SORT_MINE) {
                        falloff = (float) (10000 - distance) / 10000.0f;
                    } else {
                        falloff = remaining / range;
                    }
                    if (falloff > 1.0f) {
                        falloff = 1.0f;
                    } else if (falloff < 0.0f) {
                        falloff = 0.0f;
                    }
                    int emp = (*Globals::items)[this->itemIndex]->getAttribute(10);
                    switch (this->weaponType) {
                        case ITEM_SORT_NUKE:
                        case ITEM_SORT_MINE:
                        case ITEM_SORT_IONIZING_MISSILE:
                        case ITEM_SORT_SHOCK_BLAST:
                            if (emp != kNoAttribute) {
                                this->target->setEmpForce(falloff * (float) emp);
                            }
                            this->target->setBombForce(falloff);
                            if (this->target->isAsteroid()) {
                                falloff *= 0.6f;
                            }
                            if (this->damage > 0) {
                                this->target->damage((int) (falloff * (float) this->damage), this->friendGun,
                                                     this->itemIndex);
                                if (this->target->isGasCloud() && this->weaponType == ITEM_SORT_IONIZING_MISSILE) {
                                    ((PlayerGasCloud *) this->target->getKIPlayer())
                                            ->explode(this->itemIndex, reinterpret_cast<Vector *>(this->positions)[i],
                                                      (float) this->magnitude);
                                }
                                if (this->itemIndex == 179 && this->target->isAsteroid() &&
                                    falloff * (float) this->damage >= (float) this->target->getHitpoints() &&
                                    !Globals::achievements->hasMedal(44, 1)) {
                                    int destroyed = ++Globals::status->field_144;
                                    if (destroyed >= Globals::achievements->getValue(44, 1)) {
                                        ((Hud *) ((Level *) this->level)->getPlayer()->getHUD())->hudEventMedal(44, 100);
                                        Globals::status->field_148 = 1;
                                    }
                                }
                            }
                            if (emp > 0) {
                                this->target->damageEmp((int) (falloff * (float) emp), this->friendGun);
                            }
                            if (this->weaponType == ITEM_SORT_SHOCK_BLAST && this->target->getKIPlayer() != 0) {
                                this->target->getKIPlayer()->initPush(this->basePos, this->magnitude);
                            }
                            break;
                        default:
                            this->target->damageEmp((int) (falloff * (float) emp), this->friendGun);
                            break;
                    }
                    this->targetDir = AbyssEngine::AEMath::VectorNormalize(this->targetDir);
                    this->target->setHitVector(this->targetDir.x, this->targetDir.y, this->targetDir.z);
                    this->targetDir = Vector(reinterpret_cast<Vector *>(this->velocities)[i]);
                    this->targetDir = AbyssEngine::AEMath::VectorNormalize(this->targetDir);
                    this->lastHitKIPlayer = (int) (intptr_t) this->target->getKIPlayer();
                    this->lifetimes[i] = -1;
                }
                if (this->weaponType != ITEM_SORT_MINE) {
                    this->lifetimes[i] = -1;
                }
            }
        }
        if (this->weaponType == ITEM_SORT_EMP_BOMB) {
            this->lifetimes[0] = -1;
        }
    }
    if (this->impact != 0) {
        this->impact->explode(*reinterpret_cast<Vector *>(this->positions));
    }
}

void Gun::render() {
    if (this->impact != 0) {
        this->impact->render();
    }
    if (this->geometries != 0) {
        for (unsigned i = 0; i < this->count; ++i) {
            if (((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(this->geometries[i]))->visible) {
                Matrix view = *(Matrix *) Globals::Canvas->CameraGetLocal(Globals::Canvas->CameraGetCurrent());
                this->targetDir = AbyssEngine::AEMath::MatrixGetPosition(
                        *(Matrix *) Globals::Canvas->TransformGetLocal(this->geometries[i]));
                AbyssEngine::AEMath::MatrixSetTranslation(view, this->targetDir.x, this->targetDir.y,
                                                          this->targetDir.z);
                Globals::Canvas->TransformSetLocal(this->geometries[i], view);
                Globals::Canvas->DrawTransform(this->geometries[i], nullptr);
            }
        }
    }
}

Gun::Gun(int kind, int p2, int count, int p4, int p5, int p6, float p7, Vector dir, Vector vel) {
    Gun_VecArray_ctor(reinterpret_cast<VecArray *>(&this->count));
    Gun_VecArray_ctor(reinterpret_cast<VecArray *>(&this->velocitiesCount));
    Gun_VecArray_ctor(reinterpret_cast<VecArray *>(&this->upVectorsCount));
    Gun_VecArray_ctor(reinterpret_cast<VecArray *>(&this->hitPositionsCount));
    this->offset.x = 0;
    this->offset.y = 0;
    this->offset.z = 0;
    this->field_0x90 = 0;
    this->field_0x94 = 0;
    this->field_0x98 = 0;
    this->hitPositionsCap = 0;
    this->level = nullptr;
    this->lifetimes = 0;
    this->enemies = 0;
    this->impact = 0;
    this->geometries = 0;
    this->randomFlags = 0;
    this->slotIndex = kind;
    this->damage = p2;
    this->pitchRate = p7;
    this->targetDir.z = 0;
    this->velocity.x = 0;
    this->velocity.y = 0;
    this->velocity.z = 0;
    this->field_0xd0 = 0;
    this->field_0xd4 = 0;
    this->targetDir.x = 0;
    this->targetDir.y = 0;
    this->basePos.x = 0;
    this->basePos.y = 0;
    this->basePos.z = 0;
    this->field_0xcc = 0;
    this->ammoCount = 0;
    this->field_0x78 = 0;
    this->playerGun = 0;
    this->field_0xa8 = 0;

    this->offset = dir;
    vel *= p7;
    this->velocity = vel;
    this->initialLifetime = p5;
    this->fireDelay = p6;
    this->timer = p6;
    this->fireIndex = 0;
    this->ignited = 0;
    this->delayActive = 0;
    this->ammoCount = p4;
    this->field_0x78 = p4 << 1;
    this->lifetimes = new int[count];
    this->hitFlags = new uint8_t[count];
    Array<int> *arr = static_cast<Array<int> *>(::operator new(0xc));
    Gun_VecPtrArray_ctor(arr);
    this->wobbleOffsets = arr;
    Gun_VecArray_setLength(count, reinterpret_cast<VecArray *>(&this->count));
    Gun_VecArray_setLength(count, reinterpret_cast<VecArray *>(&this->velocitiesCount));
    Gun_VecArray_setLength(count, reinterpret_cast<VecArray *>(&this->upVectorsCount));
    Gun_VecArray_setLength(count, reinterpret_cast<VecArray *>(&this->hitPositionsCount));
    Gun_VecPtrArray_setLength(count, this->wobbleOffsets);
    Vector *positions = reinterpret_cast<Vector *>(this->positions);
    for (int i = 0; i < (int) count; i = i + 1) {
        positions[i].x = 0;
        this->lifetimes[i] = 0;
        this->hitFlags[i] = 0;
        this->wobbleOffsets->data_[i] = 0;
    }
    this->impact = 0;
    this->field_0x54 = 0;
    this->active = 0;
    this->enemies = 0;
    this->itemIndex = -1;
    this->weaponType = static_cast<ItemSort>(-1);
    this->useCustomRadius = 0;
    this->levelCollision = 1;
    this->errorMagnitudePercentage = 0;
    this->field_0x89 = 0;
    this->owner = 0;
    this->field_0xb0 = 0;
    this->empDamage = 0;
    reinterpret_cast<int &>(this->field_0xa4_b0) = 0;
}

void Gun::shoot(Matrix m, int n, bool b) {
    this->shootAt(m, n, 0, b);
}

void Gun::setEnemy(Player *enemy) {
    delete this->enemies;
    this->enemies = 0;
    Array<Player *> *a = new Array<Player *>();
    this->enemies = a;
    ArrayAdd(enemy, *a);
}

static const float kZOffset = 100.0f;

void Gun::setOffset(Vector *v) {
    float z = v->z;
    Vector local;
    local.x = v->x;
    local.y = v->y;
    local.z = z + kZOffset;
    this->offset = local;
}

static inline bool isRocketLike(int sort) {
    switch (sort) {
        case ITEM_SORT_ROCKET:
        case ITEM_SORT_MISSILE:
        case ITEM_SORT_CLUSTER_MISSILE:
            return true;
        default:
            return false;
    }
}

void Gun::update(int dt) {
    this->timer += dt;
    if (this->delayActive != 0) {
        this->delayTimer += dt;
        if (this->delayTimer >= this->fireDelay) {
            this->delayActive = 0;
        }
    }
    if (this->impact != 0) {
        this->impact->update(dt);
    }
    if (this->geometries != 0) {
        for (unsigned i = 0; i < this->count; ++i) {
            ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(this->geometries[i]))
                    ->Update((long long) dt, false);
        }
    }

    if (this->active != 0 && this->weaponType != ITEM_SORT_SENTRY_GUN) {
        this->calcCharacterCollision();
        float fdt = (float) dt;
        for (unsigned i = 0; i < this->count; ++i) {
            int lifetime = this->lifetimes[i];
            if (lifetime > (isRocketLike(this->weaponType) ? -2000 : 0)) {
                this->lifetimes[i] = lifetime - dt;
                if (this->weaponType == ITEM_SORT_MINE) {
                    Vector step = reinterpret_cast<Vector *>(this->velocities)[i] * fdt;
                    float damping = (float) (this->initialLifetime - this->lifetimes[i]) / -500.0f + 1.0f;
                    reinterpret_cast<Vector *>(this->positions)[i] += step * (damping > 0.0f ? damping : 0.0f);
                } else {
                    reinterpret_cast<Vector *>(this->positions)[i] += reinterpret_cast<Vector *>(this->velocities)[i] * fdt;
                }
                if (this->lifetimes[i] <= 0) {
                    unsigned k = (unsigned) (this->weaponType - ITEM_SORT_EMP_BOMB);
                    if (k <= 28 && ((1u << k) & 0x10000003u) != 0) {
                        this->ignite();
                    }
                    if (this->lifetimes[i] <= -2000 && isRocketLike(this->weaponType)) {
                        Globals::status->field_12c = 0;
                    }
                }
                if (this->weaponType == ITEM_SORT_SHOCK_BLAST) {
                    this->ignite();
                }
            } else {
                reinterpret_cast<Vector *>(this->positions)[i].x = 50000.0f;
                reinterpret_cast<Vector *>(this->positions)[i].y = 50000.0f;
                reinterpret_cast<Vector *>(this->positions)[i].z = 50000.0f;
                reinterpret_cast<Vector *>(this->velocities)[i].x = 0.0f;
                reinterpret_cast<Vector *>(this->velocities)[i].y = 0.0f;
                reinterpret_cast<Vector *>(this->velocities)[i].z = 0.0f;
            }
        }
    }
}

void Gun::translate(const Vector &v) {
    for (unsigned i = 0; i < this->count; i = i + 1) {
        reinterpret_cast<Vector *>(this->positions)[i] += v;
    }
}

void Gun::shootAt(Matrix m, int n, Player *p, bool b) {
    (void) m;
    this->damage = n;
    this->target = p;
    this->ignited = b;
}

static int *const gCC_status = nullptr;

void Gun::calcCharacterCollision() {
    Array<Player *> *enemies = this->enemies;
    if (enemies == 0)
        return;

    for (unsigned ei = 0; ei < enemies->size(); ei = ei + 1) {
        Player *target = enemies->data()[ei];
        this->target = target;
    }
}

void Gun::calcLevelCollision() {
}
