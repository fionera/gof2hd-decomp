#include "engine/core/ArrayReleaseClasses.h"
#include "game/ship/TargetFollowCamera.h"
#include "game/ship/Ship.h"
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
#include <algorithm>
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


// Item attribute value meaning "attribute not present".
static const int kNoAttribute = -979797979;


static const float kZOffset = 100.0f;


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

Gun::Gun(int kind, int damage, int count, int ammo, int lifetime, int fireDelay, float speed, Vector dir, Vector vel) {
    this->field_0x90 = 0;
    this->field_0x94 = 0;
    this->field_0x98 = 0;
    this->level = nullptr;
    this->lifetimes = nullptr;
    this->enemies = nullptr;
    this->impact = nullptr;
    this->geometries = nullptr;
    this->randomFlags = nullptr;
    this->slotIndex = kind;
    this->damage = damage;
    this->pitchRate = speed;
    this->ammoCount = 0;
    this->field_0x78 = 0;
    this->playerGun = false;
    this->field_0xa8 = 0;

    this->offset = dir;
    vel *= 0.15339f;
    this->velocity = vel;
    this->initialLifetime = lifetime;
    this->fireDelay = fireDelay;
    this->timer = fireDelay;
    this->currentProjectile = 0;
    this->ignited = 0;
    this->delayActive = 0;
    this->ammoCount = ammo;
    this->field_0x78 = ammo << 1;
    this->lifetimes = new int[count];
    this->hitFlags = new bool[count];
    this->wobbleOffsets = reinterpret_cast<Array<int> *>(new Array<Vector *>());
    ArraySetLength(count, this->positions);
    ArraySetLength(count, this->velocities);
    ArraySetLength(count, this->upVectors);
    ArraySetLength(count, this->hitPositions);
    ArraySetLength(count, *reinterpret_cast<Array<Vector *> *>(this->wobbleOffsets));
    for (int i = 0; i < count; ++i) {
        this->positions.data()[i].x = 50000.0f;
        this->lifetimes[i] = -1000000;
        this->hitFlags[i] = 0;
        (*reinterpret_cast<Array<Vector *> *>(this->wobbleOffsets))[i] = nullptr;
    }
    this->field_0x54 = 0;
    this->impact = nullptr;
    this->active = 0;
    this->hitSmall = 0;
    this->enemies = nullptr;
    this->itemIndex = -1;
    this->weaponType = -1;
    this->useCustomRadius = 0;
    this->levelCollision = 1;
    this->friendGun = false;
    this->errorMagnitudePercentage = 0;
    this->field_0x89 = 0;
    this->owner = nullptr;
    this->field_0xb0 = 0;
    this->empDamage = 0;
    reinterpret_cast<int &>(this->field_0xa4_b0) = 0;
}
Gun::~Gun() {
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

}
void Gun::setIndex(int index) {
    this->itemIndex = index;
    this->homing = index == 228 || (unsigned) (index - 9) < 3;
    this->empDamage = (*Globals::items)[index]->getAttribute(10);
    int mesh = kGunProjectileMeshes[index];
    if (mesh >= 0) {
        this->geometries = new int[this->positions.count];
        this->randomFlags = new uint8_t[this->positions.count];
        for (unsigned i = 0; i < this->positions.count; ++i) {
            AEGeometry *geom = new AEGeometry((uint16_t) mesh, Globals::Canvas, false);
            this->geometries[i] = geom->transform;
            this->randomFlags[i] = Globals::rnd->nextInt(2) == 0;
            ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(this->geometries[i]))
                    ->SetAnimationState((AbyssEngine::AnimationMode) 0, nullptr);
            delete geom;
        }
    }
}
void Gun::setErrorMagnitudePercentage(int v) {
    this->errorMagnitudePercentage = (float) v;
}
void Gun::setFriendGun(bool v) {
    this->friendGun = v;
}
void Gun::setMagnitude(int v) {
    this->magnitude = v;
}
int Gun::getMagnitude() {
    return this->magnitude;
}
void Gun::setLevelCollision(bool v) {
    this->levelCollision = v;
}
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
void Gun::setOffset(Vector *v) {
    float z = v->z;
    Vector local;
    local.x = v->x;
    local.y = v->y;
    local.z = z + kZOffset;
    this->offset = local;
}
void Gun::setImpact(Sparks *impact) {
    this->impact = impact;
}
void Gun::setEnemies(Array<Player *> *enemies) {
    this->enemies = enemies;
}
void *Gun::getEnemies() { // lint: void_ptr exported method return type baked into header; consumed as void* in RocketGun.cpp
    // lint: void_ptr exported method return type baked into header; consumed as void* in RocketGun.cpp
    return this->enemies;
}
void Gun::setEnemy(Player *enemy) {
    delete this->enemies;
    this->enemies = 0;
    Array<Player *> *a = new Array<Player *>();
    this->enemies = a;
    ArrayAdd(enemy, *a);
}
void Gun::removeAllEnemies() {
    this->enemies = 0;
}
void Gun::setLevel(Level *lvl) {
    this->level = lvl;
}
void Gun::setPlayerGun(bool v) {
    this->playerGun = v;
}
bool Gun::isPlayerGun() {
    return this->playerGun;
}
bool Gun::shootAt(Matrix m, int n, Player *p, bool b) {
    this->owner = p;
    Item *ammo = nullptr;
    if (!this->friendGun) {
        ammo = (*Globals::status->getShip()->getEquipment())[this->slotIndex];
        if (ammo == nullptr || ammo->getAmount() == 0) {
            return false;
        }
    }

    if (this->homing) {
        if (this->lifetimes[0] >= 1) {
            return this->weaponType == ITEM_SORT_CLUSTER_MISSILE;
        }
        this->timer = 0;
        this->delayTimer = 0;
        this->active = 1;
        this->hitSmall = 1;
        if (this->playerGun) {
            this->delayActive = 1;
        }
        int nearest = 99999999;
        this->fireIndex = -1;
        for (unsigned e = 0; e < this->enemies->size(); ++e) {
            Player *candidate = (*this->enemies)[e];
            if (candidate->getKIPlayer() != nullptr && candidate->getKIPlayer()->field_0x73 &&
                candidate->isActive() && !candidate->isDead()) {
                int distance = (int) AbyssEngine::AEMath::VectorLength(
                        candidate->getPosition() - ((Level *) this->level)->getPlayer()->getPosition());
                if (distance < nearest) {
                    this->fireIndex = e;
                    nearest = distance;
                }
            }
        }
        int fireIndex = this->fireIndex;
        this->lifetimes[0] = this->initialLifetime;
        if (fireIndex >= 0) {
            reinterpret_cast<Vector &>(this->field_0x90) = AbyssEngine::AEMath::VectorNormalize(
                    (*this->enemies)[this->fireIndex]->getPosition() -
                    ((Level *) this->level)->getPlayer()->getPosition());
            *this->positions.data() = (*this->enemies)[this->fireIndex]->getPosition();
            *this->velocities.data() =
                    ((Level *) this->level)->getPlayer()->geometry->getDirection();
            this->field_0x8c = nearest;
        } else {
            reinterpret_cast<Vector &>(this->field_0x90) =
                    ((Level *) this->level)->getPlayer()->geometry->getDirection();
            *this->positions.data() =
                    ((Level *) this->level)->getPlayer()->getPosition() +
                    30000.0f * ((Level *) this->level)->getPlayer()->geometry->getDirection();
            *this->velocities.data() =
                    ((Level *) this->level)->getPlayer()->geometry->getDirection();
            this->field_0x8c = 30000;
        }
        return true;
    }
    bool fired = false;
    for (unsigned i = 0; i < this->positions.count; ++i) {
        int lifetime = this->lifetimes[i];
        if (lifetime > (isRocketLike(this->weaponType) ? -2000 : 0)) {
            continue;
        }
        if (this->weaponType == ITEM_SORT_SENTRY_GUN && ((Level *) this->level)->field_6c > 2) {
            continue;
        }
        this->timer = 0;
        this->delayTimer = 0;
        this->active = 1;
        this->hitSmall = 1;
        this->currentProjectile = i;
        this->positions.data()[i] = AbyssEngine::AEMath::MatrixGetPosition(m);

        if (this->offset != Vector{0, 0, 0}) {
          if (this->weaponType != ITEM_SORT_SHOCK_BLAST) {
            if (this->field_0xa5 || this->field_0xa4_b0) {
                // alternate the muzzle between the mirrored offsets
                if (this->field_0xa4_b0) {
                    this->targetDir = this->offset;
                    if (this->field_0xa6) {
                        this->targetDir.x = -this->targetDir.x;
                    }
                    this->field_0xa6 = !this->field_0xa6;
                    this->positions.data()[i] += AbyssEngine::AEMath::MatrixRotateVector(m, this->targetDir);
                }
            } else {
                this->positions.data()[i] += AbyssEngine::AEMath::MatrixRotateVector(m, this->offset);
            }
            if (this->field_0xa5 && this->field_0xa6) {
                this->targetDir = this->offset;
                if (this->field_0xa7) {
                    this->targetDir.y = -this->targetDir.y;
                }
                this->field_0xa7 = !this->field_0xa7;
                this->positions.data()[i] += AbyssEngine::AEMath::MatrixRotateVector(m, this->targetDir);
            }
          }
        }
        Vector direction = {0, 0, 0};
        if (this->weaponType == ITEM_SORT_MINE) {
            direction = AbyssEngine::AEMath::MatrixGetDir(m) + AbyssEngine::AEMath::MatrixGetUp(m);
            Array<Vector *> *wobble = reinterpret_cast<Array<Vector *> *>(this->wobbleOffsets);
            if ((*wobble)[i] == nullptr) {
                (*wobble)[i] = new Vector{0, 0, 0};
            }
            (*reinterpret_cast<Array<Vector *> *>(this->wobbleOffsets))[i]->x =
                    (float) (Globals::rnd->nextInt(200) - 100) / 50.0f;
            (*reinterpret_cast<Array<Vector *> *>(this->wobbleOffsets))[i]->y =
                    (float) (Globals::rnd->nextInt(200) - 100) / 50.0f;
            (*reinterpret_cast<Array<Vector *> *>(this->wobbleOffsets))[i]->z =
                    (float) (Globals::rnd->nextInt(200) - 100) / 50.0f;
        } else {
            direction = AbyssEngine::AEMath::MatrixGetDir(m);
        }
        direction += AbyssEngine::AEMath::MatrixRotateVector(m, this->velocity);
        this->velocities.data()[i] = direction;
        this->upVectors.data()[i] = AbyssEngine::AEMath::MatrixGetUp(m);
        if (this->errorMagnitudePercentage > 0.0f) {
            float spread = this->errorMagnitudePercentage * 0.005f;
            this->velocities.data()[i].x +=
                    (float) Globals::rnd->nextInt((int) this->errorMagnitudePercentage) * 0.01f - spread;
            this->velocities.data()[i].y +=
                    (float) Globals::rnd->nextInt((int) this->errorMagnitudePercentage) * 0.01f - spread;
            this->velocities.data()[i].z +=
                    (float) Globals::rnd->nextInt((int) this->errorMagnitudePercentage) * 0.01f - spread;
        }
        this->velocities.data()[i] =
                AbyssEngine::AEMath::VectorNormalize(this->velocities.data()[i]);
        this->velocities.data()[i] *= this->pitchRate;
        this->lifetimes[i] = this->weaponType == ITEM_SORT_SENTRY_GUN ? 0 : this->initialLifetime;

        bool keepAmmo = true;
        if (!this->friendGun) {
            if (this->ammoCount >= 1 && ammo != nullptr && ammo->getType() == 1) {
                keepAmmo = false;
                if (this->itemIndex == 179) {
                    Globals::status->field_144 = 0;
                }
            }
        }
        this->delayActive = 1;
        if (this->playerGun && this->hitSmall) {
            ((TargetFollowCamera *) ((Level *) this->level)->getPlayer()->getTargetFollowCamera())->hitSmall();
        }
        fired = true;
        int cluster = this->weaponType == ITEM_SORT_CLUSTER_MISSILE;
        if (keepAmmo || cluster) {
            if (!cluster) {
                return true;
            }
        } else {
            this->ammoCount--;
            ammo->changeAmount(-1);
            if (this->ammoCount == 0) {
                Globals::status->getShip()->freeSlot(ammo);
            }
            return true;
        }
    }
    if (this->weaponType != ITEM_SORT_CLUSTER_MISSILE) {
        return false;
    }
    if (!fired) {
        return true;
    }
    this->ammoCount--;
    if (ammo != nullptr) {
        ammo->changeAmount(-1);
        if (this->ammoCount == 0) {
            Globals::status->getShip()->freeSlot(ammo);
        }
    }
    return true;
}
bool Gun::shoot(Matrix m, int n, bool b) {
    return this->shootAt(m, n, nullptr, b);
}

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
            for (unsigned i = 0; i < this->positions.count; ++i) {
                this->basePos = Vector(this->positions.data()[i]);
                this->targetDir = this->target->getPosition();
                this->targetDir -= this->basePos;
                int distance = (int) AbyssEngine::AEMath::VectorLength(this->targetDir);
                if (distance < this->magnitude) {
                    this->hitFlags[i] = 1;
                    this->hitPositions.data()[i] = this->basePos;
                    float range = (float) this->magnitude;
                    float remaining = (float) (this->magnitude - distance);
                    float falloff = remaining / range;
                    if (this->weaponType == ITEM_SORT_MINE) {
                        falloff = (float) (10000 - distance) / 10000.0f;
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
                                            ->explode(this->itemIndex, this->positions.data()[i],
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
                    this->targetDir = Vector(this->velocities.data()[i]);
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
        this->impact->explode(*this->positions.data());
    }
}
void Gun::calcCharacterCollision() {
    volatile int explosive;
    volatile int explodeOnContact;
    if (this->enemies == nullptr) {
        return;
    }
    const int weaponType = this->weaponType;
    const int itemIndex = this->itemIndex;
    const bool bomb = (weaponType == ITEM_SORT_EMP_BOMB || weaponType == ITEM_SORT_NUKE ||
                      weaponType == ITEM_SORT_SHOCK_BLAST || weaponType == ITEM_SORT_IONIZING_MISSILE);
    bool scatter = weaponType == ITEM_SORT_SCATTER_GUN;
    bool mine = weaponType == ITEM_SORT_MINE;
    explosive = 1;
    if (!bomb && !isRocketLike(weaponType)) {
        explosive = 0;
    }
    explodeOnContact = 0;
    if (bomb || mine) {
        explodeOnContact = 1;
    }

    this->lastHitKIPlayer = 0;

    for (unsigned e = 0; e < this->enemies->size(); ++e) {
        Player *enemy = (*this->enemies)[e];
        if (mine) {
            if (!enemy->enemyFlagsLo) continue;
            if (enemy->getKIPlayer() != nullptr && enemy->getKIPlayer()->deadFlag) continue;
        }
        if (!enemy->isActive()) continue;
        if (enemy->getHitpoints() < 1) continue;

        for (unsigned i = 0; i < this->positions.count; ++i) {
            Vector position = this->positions.data()[i];
            Vector velocity = this->velocities.data()[i];
            Vector enemyPosition =
                    AbyssEngine::AEMath::MatrixGetPosition(reinterpret_cast<Matrix &>(enemy->transform[0]));
            Level *level = (Level *) this->level;
            int radius = this->useCustomRadius ? this->customRadius : enemy->radius;
            if (mine) {
                radius *= 5;
            } else if (scatter) {
                int distance = (int) AbyssEngine::AEMath::VectorLength(
                        enemyPosition - ((Level *) this->level)->getPlayer()->getPosition());
                if (enemy->getKIPlayer() != nullptr && enemy->getKIPlayer()->field_0x71) {
                    radius = (int) ((float) radius * 0.5f);
                } else if (distance > 20000) {
                    radius = (int) (float) (radius * 3);
                } else {
                    radius = (int) (distance > 10000 ? (float) (radius * 2) : (float) radius * 1.5f);
                }
            }

            KIPlayer *ki = enemy->getKIPlayer();
            if (!enemy->egoFlag && (ki == nullptr || !(uint8_t) ki->field_0x8c)) {
                continue;
            }
            bool outer = ki != nullptr && ki->field_0x40;
            if (!(outer && ki->outerCollide(position))) {
                float r = (float) radius;
                if (!(enemyPosition.x - position.x + velocity.x < r &&
                      enemyPosition.x - position.x + velocity.x > (float) -radius &&
                      enemyPosition.y - position.y + velocity.y < r &&
                      enemyPosition.y - position.y + velocity.y > (float) -radius &&
                      enemyPosition.z - position.z + velocity.z < r &&
                      enemyPosition.z - position.z + velocity.z > (float) -radius && !outer)) {
                    continue;
                }
            }

            if (enemy->isAsteroid()) {
                if (mine) {
                    continue;
                }
                enemy->setBombForce(0.0f);
                if (explosive) {
                    enemy->damage(9999, false, this->itemIndex);
                    if (!Globals::achievements->hasMedal(41, 1) &&
                        (isRocketLike(this->weaponType) || this->weaponType == ITEM_SORT_IONIZING_MISSILE)) {
                        int count = ++Globals::status->field_12c;
                        if (count >= Globals::achievements->getValue(41, 1)) {
                            ((Hud *) ((Level *) this->level)->getPlayer()->getHUD())->hudEventMedal(41, 100);
                            Globals::status->field_130 = 1;
                        }
                    } else if (this->itemIndex == 179 && !Globals::achievements->hasMedal(44, 1)) {
                        int count = ++Globals::status->field_144;
                        if (count >= Globals::achievements->getValue(44, 1)) {
                            ((Hud *) ((Level *) this->level)->getPlayer()->getHUD())->hudEventMedal(44, 100);
                            Globals::status->field_148 = 1;
                        }
                    }
                    return;
                }
            } else if (mine) {
                float r = (float) enemy->radius;
                if (enemyPosition.x - position.x + velocity.x < r &&
                    enemyPosition.x - position.x + velocity.x > (float) -enemy->radius &&
                    enemyPosition.y - position.y + velocity.y < r &&
                    enemyPosition.y - position.y + velocity.y > (float) -enemy->radius &&
                    enemyPosition.z - position.z + velocity.z < r &&
                    enemyPosition.z - position.z + velocity.z > (float) -enemy->radius) {
                    this->ignite();
                    return;
                }
                this->velocities.data()[i] =
                        AbyssEngine::AEMath::VectorNormalize(enemyPosition - position);
                this->positions.data()[i] +=
                        this->velocities.data()[i] * this->pitchRate * 100.0f;
                return;
            }
            if (explodeOnContact) {
                this->ignite();
                return;
            }

            if (this->empDamage != kNoAttribute) {
                enemy->damageEmp(this->empDamage, this->friendGun);
            }
            int amount = this->damage;
            if (this->owner == nullptr || this->owner->enemyFlagsLo) {
                if (enemy->egoFlag && level->getPlayer() != nullptr &&
                    level->getPlayer()->isDockedToDockingPoint()) {
                    amount = (int) ((float) this->damage * 3.0f * 0.25f);
                }
            } else if (enemy->egoFlag) {
                amount = (int) ((float) this->damage * 0.2f);
            }
            enemy->damage(amount, this->friendGun, itemIndex);
            reinterpret_cast<uint8_t *>(&enemy->field_54)[0] = 1;
            if (this->playerGun) {
                ((Level *) this->level)->field_30 = 1;
            }
            enemy->setHitVector(-velocity.x, -velocity.y, -velocity.z);
            this->lifetimes[i] = -1000000;
            this->lastHitKIPlayer = (int) (intptr_t) enemy->getKIPlayer();
            if (explosive) {
                ((Level *) this->level)->field_74->emitManual(((Level *) this->level)->field_3c, position, 0, velocity,
                                                              -1.0f);
            } else if (this->geometries != nullptr) {
                ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(this->geometries[i]))
                        ->SetAnimationState((AbyssEngine::AnimationMode) 3, nullptr);
                ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(this->geometries[i]))
                        ->SetAnimationState((AbyssEngine::AnimationMode) 1, nullptr);
                Matrix view = *(Matrix *) Globals::Canvas->CameraGetLocal(Globals::Canvas->CameraGetCurrent());
                AbyssEngine::AEMath::MatrixSetTranslation(view, position.x, position.y, position.z);
                Globals::Canvas->TransformSetLocal(this->geometries[i], view);
            }
            if (this->impact != nullptr) {
                if (this->impact->isRocket()) {
                    reinterpret_cast<uint8_t *>(&enemy->field_54)[1] = 1;
                }
                if (this->impact != nullptr) {
                    this->impact->explode(position);
                }
            }
            if (scatter) {
                this->hitFlags[i] = 1;
                this->hitPositions.data()[i] = position;
                for (unsigned k = 0; k < this->enemies->size(); ++k) {
                    Player *other = (*this->enemies)[k];
                    this->targetDir = other->getPosition();
                    this->targetDir -= position;
                    int distance = (int) AbyssEngine::AEMath::VectorLength(this->targetDir);
                    if (distance < this->magnitude) {
                        float falloff = (float) (this->magnitude - distance) / (float) this->magnitude;
                        if (falloff > 1.0f) {
                            falloff = 1.0f;
                        } else if (falloff < 0.0f) {
                            falloff = 0.0f;
                        }
                        other->damage((int) (falloff * (float) this->damage), this->friendGun, this->itemIndex);
                        if (this->empDamage > 0) {
                            other->damageEmp((int) (falloff * (float) this->empDamage), this->friendGun);
                        }
                    }
                }
            }
        }
    }
}
void Gun::calcLevelCollision() {
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
        for (unsigned i = 0; i < this->positions.count; ++i) {
            ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(this->geometries[i]))
                    ->Update((long long) dt, false);
        }
    }

    if (this->active != 0 && this->weaponType != ITEM_SORT_SENTRY_GUN) {
        this->calcCharacterCollision();
        float fdt = (float) dt;
        for (unsigned i = 0; i < this->positions.count; ++i) {
            int lifetime = this->lifetimes[i];
            if (lifetime > (isRocketLike(this->weaponType) ? -2000 : 0)) {
                this->lifetimes[i] = lifetime - dt;
                if (this->weaponType == ITEM_SORT_MINE) {
                    // mines decelerate over their first 500 ms of flight
                    this->positions.data()[i] +=
                            this->velocities.data()[i] * fdt *
                            std::max((float) (this->initialLifetime - this->lifetimes[i]) / -500.0f + 1.0f, 0.0f);
                } else {
                    this->positions.data()[i] += this->velocities.data()[i] * fdt;
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
                this->positions.data()[i].z = 50000.0f;
                this->positions.data()[i].y = 50000.0f;
                this->positions.data()[i].x = 50000.0f;
                this->velocities.data()[i].z = 0.0f;
                this->velocities.data()[i].y = 0.0f;
                this->velocities.data()[i].x = 0.0f;
            }
        }
    }
}
void Gun::render() {
    if (this->impact != 0) {
        this->impact->render();
    }
    if (this->geometries != 0) {
        for (unsigned i = 0; i < this->positions.count; ++i) {
            if (((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(this->geometries[i]))->animating) {
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
void Gun::translate(const Vector &v) {
    for (unsigned i = 0; i < this->positions.count; i = i + 1) {
        this->positions.data()[i] += v;
    }
}