#include "game/weapons/ObjectGun.h"
#include "engine/core/ArrayReleaseClasses.h"

#include <cstddef>

#include "game/world/Level.h"
#include "engine/math/Transform.h"
#include "game/mission/Explosion.h"
#include "game/ship/PlayerEgo.h"
#include "game/weapons/Gun.h"
#include "game/ship/Player.h"
#include "engine/render/AEGeometry.h"
#include "engine/render/PaintCanvas.h"
#include "game/core/Globals.h"

struct EgoPlayerView {
    char header[0x40];
    Matrix orientation;
};
#if __SIZEOF_POINTER__ == 4
static_assert(offsetof(EgoPlayerView, orientation) == 0x40,
              "EgoPlayerView.orientation must sit at 0x40");
#endif

namespace AbyssEngine {
    namespace AEMath {
        Matrix MatrixMultiply(const Matrix &, const Matrix &);
    }
}

static PaintCanvas **g_PaintCanvas = nullptr;

uint32_t TransformGetTransform(void *canvas, uint32_t transform); // lint: void_ptr free-function signature, retype changes mangling

void MatrixRotateVector(void *out, const void *matrix, const void *vec); // lint: void_ptr free-function signature, retype changes mangling

static inline void MatrixGetDir(Vector *out, const Matrix *matrix) { *out = AbyssEngine::AEMath::MatrixGetDir(*matrix); }

static inline void MatrixGetUp(Vector *out, const Matrix *matrix) { *out = AbyssEngine::AEMath::MatrixGetUp(*matrix); }

void MatrixSetRotation(Matrix *dst, float x, float y, float z);

void MatrixSetTranslation(Matrix *dst, float x, float y, float z);

void MatrixSetScaling(Matrix *dst, float x, float y, float z);

static inline void VectorNormalize(Vector *out, const Vector *in) { *out = AbyssEngine::AEMath::VectorNormalize(*in); }

static inline void VectorCross(Vector *out, const Vector *a, const Vector *b) { *out = AbyssEngine::AEMath::VectorCross(*a, *b); }

void ObjectGun_setEnemies_impl(void *items); // lint: void_ptr free-function signature, retype changes mangling

static uint8_t *g_ObjectGunRenderScaleFlag = nullptr;
// Projectile mesh per item index (-1: the item has no visible projectile).
static const int kProjectileMeshes[233] = {
    14500, 14500, 14500, 14501, 14501, 14502, 14502, 14503, 14503, 14500, 14503, 14502,
    14501, 14501, 14501, 14501, 14504, 14504, 14504, 14505, 14505, 14505, 14506, 14506,
    14507, 14507, 14508, 14508, 14506, 14505, 14502, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 14503,
    14505, 14501, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, 14506, 14503, 14502, -1,
    14509, 14510, 14506, 14503, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, 14500, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, 14505, -1, -1, -1,
    14501, 14501, 14508, 14502, -1,
};

static uint32_t (*g_TransformGetObject)(PaintCanvas *canvas, uint32_t mesh) = nullptr;

static void (*g_TransformSetState)(uint32_t object, int state, int value) = nullptr;










ObjectGun::ObjectGun(int /*unused*/, Gun *gun, int mesh, uint32_t /*param*/, Level *level)
    : dir(), up(), side() {
    this->useEgoOrientation = 0;
    this->explosions = nullptr;
    this->explosionReady = nullptr;
    this->unusedSlot = -1;
    this->gun = gun;
    this->level = level;
    this->secondaryTransform = -1;
    Globals::Canvas->TransformCreate(this->transform);
    this->meshId = mesh;
    Globals::Canvas->TransformAddMesh(this->transform, (uint16_t) mesh, false);
    this->rollAngle = 0.0f;
    this->scaleX = 1.0f;
    this->scaleY = 1.0f;
    this->scaleZ = 1.0f;
    this->spinAngle = 0.0f;

    int type = gun->weaponType;
    switch (type) {
        case 1:
        case 3:
        case 8:
            this->visible = 1;
            break;
        default:
            this->visible = 0;
            break;
    }
    if (Globals::iPad && (type == 1 || type == 3 || type == 8)) {
        this->scaleX = 0.6f;
        this->scaleY = 0.6f;
        this->scaleZ = 0.6f;
    }
    if (type == 0xb) {
        this->scaleX = 0.7f;
        this->scaleY = 0.7f;
        this->scaleZ = 0.7f;
    }
    if (type == 0x19) {
        this->explosions = new Array<Explosion *>();
        ArraySetLength(gun->count, *this->explosions);
        this->explosionReady = new uint8_t[this->explosions->size()];
        for (uint32_t i = 0; i < this->explosions->size(); ++i) {
            (*this->explosions)[i] =
                    new Explosion(gun->itemIndex == 0xb0 ? 8 : gun->itemIndex == 0xb1 ? 9 : 10);
            (*this->explosions)[i]->setWeaponIndex(gun->itemIndex);
            this->explosionReady[i] = 1;
        }
    }

    if (gun->field_0xa8 != 0)
        this->hasGeometry = 1;
    else if (gun->isPlayerGun() && kProjectileMeshes[gun->itemIndex] >= 0)
        this->hasGeometry = gun->weaponType != ITEM_SORT_MINE;
    else
        this->hasGeometry = 0;

    AEGeometry *geometry = nullptr;
    if (this->hasGeometry)
        geometry = new AEGeometry((uint16_t) kProjectileMeshes[gun->itemIndex], Globals::Canvas, false);

    this->geometry = geometry;
    this->wasFiring = 0;
    this->deltaTime = 0;
}

ObjectGun::~ObjectGun() {
    delete this->geometry;
    this->geometry = nullptr;

    if (this->explosions != nullptr) {
        ArrayReleaseClasses(*this->explosions);
        delete this->explosions;
        this->explosions = nullptr;
    }

    delete[] this->explosionReady;
    this->explosionReady = nullptr;
}

void ObjectGun::setEnemies(Array<Player *> *enemies) {
    ObjectGun_setEnemies_impl(this->gun);
}

void ObjectGun::setEnemy(Player * /*enemy*/) {
}

void ObjectGun::update(int dt) {
    static_cast<AbyssEngine::Transform *>(Globals::Canvas->TransformGetTransform(this->transform))
            ->Update((int64_t) dt, false);
    this->deltaTime = dt;
    this->gun->update(dt);

    if (this->hasGeometry == 0) {
        if (!this->gun->isPlayerGun() && this->gun->owner != nullptr &&
            this->gun->owner->getKIPlayer() != nullptr && this->gun->owner->getKIPlayer()->field_0x3f_b != 0) {
            this->hasGeometry = 1;
            this->geometry = new AEGeometry((uint16_t) kProjectileMeshes[this->gun->itemIndex], Globals::Canvas,
                                            false);
        }
        if (this->hasGeometry == 0)
            goto tail;
    }

    if (this->gun->delayActive == 0) {
        // Idle: rewind the muzzle animation.
        static_cast<AbyssEngine::Transform *>(Globals::Canvas->TransformGetTransform(this->geometry->transform))
                ->SetAnimationState((AbyssEngine::AnimationMode) 0, nullptr);
        static_cast<AbyssEngine::Transform *>(Globals::Canvas->TransformGetTransform(this->geometry->transform))
                ->SetAnimationState((AbyssEngine::AnimationMode) 3, nullptr);
        static_cast<AbyssEngine::Transform *>(Globals::Canvas->TransformGetTransform(this->geometry->transform))
                ->SetAnimationState((AbyssEngine::AnimationMode) 1, nullptr);
    } else {
        PlayerEgo *ego = this->level->getPlayer();
        Vector position;
        if (this->gun->isPlayerGun())
            position = ego->getPosition();
        else
            position = this->gun->owner->getPosition();

        Gun *gun = this->gun;
        Matrix shipMatrix = reinterpret_cast<const Matrix &>(
                (gun->isPlayerGun() ? ego->player : gun->owner)->transform[0]);
        Vector barrel = gun->offset;
        barrel.z += -100.0f;
        position += AbyssEngine::AEMath::MatrixRotateVector(shipMatrix, barrel);
        this->geometry->setPosition(position);
        static_cast<AbyssEngine::Transform *>(Globals::Canvas->TransformGetTransform(this->geometry->transform))
                ->Update((int64_t) dt, false);

        if (this->gun->weaponType == ITEM_SORT_TURRET) {
            Matrix turret = ego->turretRoot->getMatrix() * ego->turretGeometry->getMatrix();
            gun = this->gun;
            int item = gun->itemIndex;
            Vector muzzle;
            muzzle.x = 0.0f;
            muzzle.y = 0.0f;
            switch (item) {
                case 0x2f: muzzle.z = 260.0f;
                    break;
                case 0xb5: muzzle.z = 190.0f;
                    break;
                case 0x31: muzzle.z = 200.0f;
                    break;
                case 0xb4: muzzle.z = 172.0f;
                    break;
                case 0x30: muzzle.z = 170.0f;
                    break;
                case 0xe0: muzzle.z = 147.0f;
                    break;
                case 0xb6: muzzle.z = 150.0f;
                    break;
                default: muzzle.z = 205.0f;
                    break;
            }
            if (gun->field_0xa4_b0) {
                if (item == 0xb5) {
                    muzzle.x = gun->field_0xa6 ? -35.0f : 35.0f;
                } else {
                    float side = item == 0x30 ? 20.0f : 45.0f;
                    muzzle.x = gun->field_0xa6 ? gun->offset.x + -side : side - gun->offset.x;
                }
            }
            if (gun->field_0xa5) {
                if (item == 0xe0)
                    muzzle.y = gun->field_0xa7 ? -13.0f : 13.0f;
                else if (item == 0xb5)
                    muzzle.y = gun->field_0xa7 ? -16.5f : 16.5f;
            }
            Vector rotated = AbyssEngine::AEMath::MatrixRotateVector(turret, muzzle);
            this->geometry->setMatrix(turret);
            this->geometry->translate(rotated);
        } else {
            PaintCanvas *canvas = Globals::Canvas;
            Matrix camera = *reinterpret_cast<const Matrix *>(canvas->CameraGetLocal(canvas->CameraGetCurrent()));
            this->geometry->setDirection(AbyssEngine::AEMath::MatrixGetDir(shipMatrix),
                                         AbyssEngine::AEMath::MatrixGetUp(camera));
        }
    }

tail:
    this->wasFiring = this->gun->delayActive;
    if (this->gun->weaponType == ITEM_SORT_SCATTER_GUN) {
        for (uint32_t i = 0; i < this->gun->count; ++i) {
            if (this->gun->hitFlags[i] == 0)
                continue;
            if (this->explosionReady[i] != 0) {
                (*this->explosions)[i]->start(reinterpret_cast<Vector *>(this->gun->hitPositions)[i],
                                              Vector{0.0f, 0.0f, 0.0f});
                this->explosionReady[i] = 0;
            }
            (*this->explosions)[i]->update(dt, static_cast<TargetFollowCamera *>(nullptr));
            if (!(*this->explosions)[i]->isPlaying()) {
                this->gun->hitFlags[i] = 0;
                this->gun->ignited = 0;
                this->explosionReady[i] = 1;
                (*this->explosions)[i]->reset();
            }
        }
    }
}

void ObjectGun::render() {
    Matrix local;
    Matrix cameraLocal;
    Matrix rotate;
    Matrix scaleMatrix;
    Vector muzzle;
    Vector dir;
    Vector up;
    Vector side;

    Gun *gun = this->gun;
    gun->render();

    if (gun->weaponType == ITEM_SORT_SCATTER_GUN) {
        for (uint32_t i = 0; i < gun->count; ++i) {
            if (gun->hitFlags[i] != 0) {
                this->explosions->data()[i]->render();
                gun = this->gun;
            }
        }
    }

    if (gun->active != 0) {
        for (uint32_t i = 0; i < 15; ++i)
            cameraLocal.m[i] = 0.0f;
        cameraLocal.m[0] = 1.0f;
        cameraLocal.m[5] = 1.0f;
        cameraLocal.m[14] = 1.0f;
        if (gun->weaponType == ITEM_SORT_TURRET) {
            PaintCanvas *canvas = Globals::Canvas;
            cameraLocal = *(const Matrix *) canvas->CameraGetLocal(canvas->CameraGetCurrent());
            if (this->visible != 0) {
                for (uint32_t mi = 0; mi < 15; ++mi)
                    rotate.m[mi] = 0.0f;
                rotate.m[0] = 1.0f;
                rotate.m[5] = 1.0f;
                rotate.m[14] = 1.0f;
                MatrixSetRotation(&scaleMatrix, this->spinAngle, 0.0f, 0.0f);
                rotate = scaleMatrix;
                AbyssEngine::AEMath::MatrixMultiply(cameraLocal, rotate);
            }
        }

        uint32_t inactive = 0;
        for (uint32_t i = 0; i < gun->count; ++i) {
            Vector *gunPos = &((Vector *) gun->positions)[i];
            if (gunPos->x != -1000.0f) {
                MatrixSetTranslation(&local, gunPos->x, gunPos->y, gunPos->z);
                ::VectorNormalize((Vector *) &local, &((Vector *) gun->velocities)[i]);
                this->dir = *(const Vector *) &local;

                if (this->visible != 0) {
                    muzzle.x = -this->dir.x;
                    muzzle.y = -this->dir.y;
                    muzzle.z = -this->dir.z;
                    if ((uint32_t)(gun->itemIndex - 0xb4) > 2) {
                        PaintCanvas *canvas = Globals::Canvas;
                        cameraLocal = *(const Matrix *) canvas->CameraGetLocal(canvas->CameraGetCurrent());
                        MatrixGetDir(&dir, &cameraLocal);
                        muzzle = dir;
                        gun = this->gun;
                    }

                    this->orientation.m[0] = cameraLocal.m[0];
                    this->orientation.m[1] = cameraLocal.m[1];
                    this->orientation.m[4] = cameraLocal.m[4];
                    this->orientation.m[5] = cameraLocal.m[5];
                    this->orientation.m[8] = cameraLocal.m[10];
                    this->orientation.m[9] = cameraLocal.m[11];
                    this->orientation.m[2] = -muzzle.x;
                    this->orientation.m[6] = -muzzle.y;
                    this->orientation.m[10] = -muzzle.z;

                    int scale = gun->lifetimes[i];
                    if (scale < 1000) {
                        float fscale = (float) scale / 750.0f;
                        MatrixSetScaling(&local, fscale, fscale, fscale);
                    } else if (gun->hitSmall != 0) {
                        MatrixSetScaling(&local, 1.0f, 1.0f, 1.0f);
                    }
                } else if (this->useEgoOrientation == 0) {
                    Vector base;
                    if (this->level == nullptr || this->level->getPlayer() == 0 ||
                        gun->isPlayerGun() == 0) {
                        base.x = 0.0f;
                        base.y = 1.0f;
                        base.z = 0.0f;
                    } else {
                        base = ((const Vector *) gun->upVectors)[i];
                    }
                    this->up = base;
                    VectorCross(&side, &this->up, &this->dir);
                    ::VectorNormalize(&this->side, &side);
                    VectorCross(&side, &this->side, &this->dir);
                    ::VectorNormalize(&this->up, &side);

                    this->orientation.m[0] = this->side.x;
                    this->orientation.m[1] = this->up.x;
                    this->orientation.m[2] = this->dir.x;
                    this->orientation.m[4] = this->side.y;
                    this->orientation.m[5] = this->up.y;
                    this->orientation.m[6] = this->dir.y;
                    this->orientation.m[8] = this->side.z;
                    this->orientation.m[9] = this->up.z;
                    this->orientation.m[10] = this->dir.z;

                    int scale = gun->lifetimes[i];
                    if (scale < 1000) {
                        float fscale = (float) scale / 750.0f;
                        MatrixSetScaling(&local, fscale, fscale, fscale);
                    } else if (gun->hitSmall != 0) {
                        MatrixSetScaling(&local, 1.0f, 1.0f, 1.0f);
                        if (this->isRocketGun() == 0)
                            gun->hitSmall = 0;
                    }
                } else {
                    Player *player = (Player *) (uint64_t) this->level->getPlayer();
                    EgoPlayerView *playerView = (EgoPlayerView *) player;
                    for (uint32_t mi = 0; mi < 15; ++mi)
                        local.m[mi] = 0.0f;
                    local.m[0] = 1.0f;
                    local.m[5] = 1.0f;
                    local.m[14] = 1.0f;

                    // NOTE: re-reads the int EMP counters' bits as floats (decompiler artifact; see Player.h).
                    MatrixSetRotation(&scaleMatrix, reinterpret_cast<float &>(player->empPoints),
                                      reinterpret_cast<float &>(player->maxEmpPoints), 0.0f);
                    AbyssEngine::AEMath::MatrixMultiply(playerView->orientation, local);
                    this->orientation = playerView->orientation;
                    MatrixSetTranslation(&scaleMatrix, gunPos->x, gunPos->y, gunPos->z);
                    MatrixGetDir(&dir, &scaleMatrix);
                    ::VectorNormalize(&muzzle, &dir);
                    muzzle *= gun->pitchRate;
                    ((Vector *) gun->velocities)[i] = muzzle;
                    player->empPoints = 0;
                    player->maxEmpPoints = 0;
                    MatrixSetRotation(&scaleMatrix, this->rollAngle, 0.0f, 0.0f);
                    Globals::Canvas->TransformSetLocal(this->secondaryTransform, scaleMatrix);
                }

                if (*g_ObjectGunRenderScaleFlag != 0)
                    MatrixSetScaling(&local, this->scaleX, this->scaleY, this->scaleZ);

                if (this->gun->weaponType == ITEM_SORT_MINE) {
                    Array<Vector *> *spins = (Array<Vector *> *) this->gun->wobbleOffsets;
                    Vector *spin = spins->data()[i];
                    if (spin != nullptr && this->deltaTime > 0) {
                        MatrixSetRotation(&local, spin->x, spin->y, spin->z);
                        float step = (float) this->deltaTime * 0.02f;
                        float neg = -step;
                        spin->x += spin->x < 0.0f ? neg : step;
                        spin->y += spin->y < 0.0f ? neg : step;
                        spin->z += spin->z < 0.0f ? neg : step;
                    }
                    MatrixSetScaling(&local, this->scaleX, this->scaleY, this->scaleZ);
                }

                PaintCanvas *canvas = Globals::Canvas;
                canvas->TransformSetLocal(this->transform, this->orientation);
                canvas->DrawTransform(this->transform, nullptr);
            } else {
                ++inactive;
            }
            gun = this->gun;
        }
        if (gun->positionsCap <= (int) inactive)
            gun->active = 0;
    }

    if (this->wasFiring != 0 && this->geometry != nullptr)
        this->geometry->render();
    this->deltaTime = 0;
}

void ObjectGun::translate(const Vector & /*v*/) {
}

void ObjectGun::setScaling(int x, int y, int /*z*/) {
    this->scaleX = (float) x;
    this->scaleY = (float) y;
}

void ObjectGun::replaceGun(unsigned int mesh, int /*unused*/) {
    PaintCanvas **canvas = &Globals::Canvas;
    (*canvas)->TransformRemoveMesh(this->transform, this->meshId);
    this->meshId = (int) mesh;
    (*canvas)->TransformAddMesh(this->transform, (uint16_t) mesh, false);
}

