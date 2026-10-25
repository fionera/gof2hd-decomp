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
    this->gun = gun;
    this->level = level;
    this->unusedSlot = -1;
    this->secondaryTransform = -1;
    Globals::Canvas->TransformCreate(this->transform);
    this->meshId = mesh;
    Globals::Canvas->TransformAddMesh(this->transform, (uint16_t) mesh, false);
    this->rollAngle = 0.0f;
    this->scaleX = 1.0f;
    this->scaleY = 1.0f;
    this->scaleZ = 1.0f;
    this->spinAngle = 0.0f;

    switch (gun->weaponType) {
        case 1:
        case 3:
        case 8:
            this->visible = 1;
            break;
        default:
            this->visible = 0;
            break;
    }
    if (Globals::iPad && (gun->weaponType == 1 || gun->weaponType == 3 || gun->weaponType == 8)) {
        this->scaleX = 0.6f;
        this->scaleY = 0.6f;
        this->scaleZ = 0.6f;
    }
    switch (gun->weaponType) {
        case 0xb:
            this->scaleX = 0.7f;
            this->scaleY = 0.7f;
            this->scaleZ = 0.7f;
            break;
        case 0x19:
            this->explosions = new Array<Explosion *>();
            ArraySetLength(gun->positions.count, *this->explosions);
            this->explosionReady = new uint8_t[this->explosions->size()];
            for (uint32_t i = 0; i < this->explosions->size(); ++i) {
                (*this->explosions)[i] =
                        new Explosion(gun->itemIndex == 0xb0 ? 8 : gun->itemIndex == 0xb1 ? 9 : 10);
                (*this->explosions)[i]->setWeaponIndex(gun->itemIndex);
                this->explosionReady[i] = 1;
            }
            break;
    }

    if (gun->field_0xa8 != 0)
        this->hasGeometry = 1;
    else if (gun->isPlayerGun() && kProjectileMeshes[gun->itemIndex] >= 0)
        this->hasGeometry = gun->weaponType != ITEM_SORT_MINE;
    else
        this->hasGeometry = 0;

    this->geometry = this->hasGeometry
                     ? new AEGeometry((uint16_t) kProjectileMeshes[gun->itemIndex], Globals::Canvas, false)
                     : nullptr;
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
    this->gun->setEnemies(enemies);
}

void ObjectGun::setEnemy(Player * /*enemy*/) {
}

void ObjectGun::update(int dt) {
    static_cast<AbyssEngine::Transform *>(Globals::Canvas->TransformGetTransform(this->transform))
            ->Update((int64_t) dt, false);
    this->deltaTime = dt;
    this->gun->update(dt);

    if (this->hasGeometry == 0 && !this->gun->isPlayerGun() && this->gun->owner != nullptr &&
        this->gun->owner->getKIPlayer() != nullptr && this->gun->owner->getKIPlayer()->field_0x3f_b != 0) {
        this->hasGeometry = 1;
        this->geometry = new AEGeometry((uint16_t) kProjectileMeshes[this->gun->itemIndex], Globals::Canvas, false);
    }

    if (this->hasGeometry != 0) {
        if (this->gun->delayActive != 0) {
            PlayerEgo *ego = this->level->getPlayer();
            Vector position = this->gun->isPlayerGun() ? ego->getPosition() : this->gun->owner->getPosition();

            Matrix shipMatrix = reinterpret_cast<const Matrix &>(
                    (this->gun->isPlayerGun() ? ego->player : this->gun->owner)->transform[0]);
            Vector barrel = this->gun->offset;
            barrel.z += -100.0f;
            position += AbyssEngine::AEMath::MatrixRotateVector(shipMatrix, barrel);
            this->geometry->setPosition(position);
            static_cast<AbyssEngine::Transform *>(Globals::Canvas->TransformGetTransform(this->geometry->transform))
                    ->Update((int64_t) dt, false);

            if (this->gun->weaponType == ITEM_SORT_TURRET) {
                Matrix turret = ego->turretRoot->getMatrix() * ego->turretGeometry->getMatrix();
                Gun *gun = this->gun;
                int item = gun->itemIndex;
                // Muzzle offset along the barrel per turret model.
                bool isB5 = false;
                float muzzleZ = 0.0f;
                if (item == 0x2f)
                    muzzleZ = 260.0f;
                else if (item == 0x30)
                    muzzleZ = 170.0f;
                else if (item == 0x31)
                    muzzleZ = 200.0f;
                else if (item == 0xb4)
                    muzzleZ = 172.0f;
                else if (item == 0xb5)
                    isB5 = true;
                else
                    muzzleZ = item == 0xb6 ? 115.0f : 205.0f;
                Vector muzzle = isB5 ? Vector{0.0f, 0.0f, 190.0f}
                                     : Vector{0.0f, 0.0f, muzzleZ};
                if (item == 0xb6)
                    muzzle.z = 150.0f;
                else if (item == 0xe0)
                    muzzle.z = 147.0f;
                if (gun->field_0xa4_b0) {
                    if (item == 0xb5)
                        muzzle.x = gun->field_0xa6 ? -35.0f : 35.0f;
                    else if (item == 0x30)
                        muzzle.x = gun->field_0xa6 ? gun->offset.x + -20.0f : 20.0f - gun->offset.x;
                    else
                        muzzle.x = gun->field_0xa6 ? gun->offset.x + -45.0f : 45.0f - gun->offset.x;
                }
                if (gun->field_0xa5) {
                    if (item != 0xe0) {
                        if (item == 0xb5)
                            muzzle.y = gun->field_0xa7 ? -16.5f : 16.5f;
                    } else {
                        muzzle.y = gun->field_0xa7 ? -13.0f : 13.0f;
                    }
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
        } else {
            // Idle: rewind the muzzle animation.
            static_cast<AbyssEngine::Transform *>(Globals::Canvas->TransformGetTransform(this->geometry->transform))
                    ->SetAnimationState((AbyssEngine::AnimationMode) 0, nullptr);
            static_cast<AbyssEngine::Transform *>(Globals::Canvas->TransformGetTransform(this->geometry->transform))
                    ->SetAnimationState((AbyssEngine::AnimationMode) 3, nullptr);
            static_cast<AbyssEngine::Transform *>(Globals::Canvas->TransformGetTransform(this->geometry->transform))
                    ->SetAnimationState((AbyssEngine::AnimationMode) 1, nullptr);
        }
    }

    this->wasFiring = this->gun->delayActive;
    if (this->gun->weaponType == ITEM_SORT_SCATTER_GUN) {
        for (uint32_t i = 0; i < this->gun->positions.count; ++i) {
            if (this->gun->hitFlags[i] == 0)
                continue;
            if (this->explosionReady[i] != 0) {
                (*this->explosions)[i]->start(this->gun->hitPositions.data()[i],
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
    using namespace AbyssEngine::AEMath;

    this->gun->render();

    Gun *gun = this->gun;
    if (gun->weaponType == ITEM_SORT_SCATTER_GUN) {
        for (uint32_t i = 0; i < gun->positions.count; ++i) {
            if (gun->hitFlags[i] != 0) {
                this->explosions->data()[i]->render();
                gun = this->gun;
            }
        }
    }

    if (gun->active != 0) {
        Matrix camera;
        if (gun->weaponType == ITEM_SORT_TURRET) {
            camera = *(const Matrix *) Globals::Canvas->CameraGetLocal(Globals::Canvas->CameraGetCurrent());
            if (this->visible != 0) {
                // Visible turrets are drawn turned around and spun about their own axis.
                Matrix spin;
                spin = MatrixSetRotation(spin, 0.0f, 3.14159265f, this->spinAngle);
                camera *= spin;
            }
        }

        int inactive = 0;
        for (uint32_t i = 0; i < this->gun->positions.count; ++i) {
            const Vector &position = this->gun->positions.data()[i];
            if (position.x != 50000.0f) {
                MatrixSetTranslation(this->orientation, position.x, position.y, position.z);
                this->dir = VectorNormalize(this->gun->velocities.data()[i]);

                if (this->visible != 0) {
                    // Camera-facing projectile (beam heads, flares): billboard along the flight direction.
                    Vector facing = -this->dir;
                    gun = this->gun;
                    if ((uint32_t) (gun->itemIndex - 0xb4) > 2) {
                        camera = *(const Matrix *) Globals::Canvas->CameraGetLocal(Globals::Canvas->CameraGetCurrent());
                        facing = MatrixGetDir(camera);
                        gun = this->gun;
                    }
                    this->orientation.m.e0 = camera.m.e0;
                    this->orientation.m.e4 = camera.m.e4;
                    this->orientation.m.e8 = camera.m.e8;
                    this->orientation.m.e1 = camera.m.e1;
                    this->orientation.m.e5 = camera.m.e5;
                    this->orientation.m.e9 = camera.m.e9;
                    this->orientation.m[2] = -facing.x;
                    this->orientation.m[6] = -facing.y;
                    this->orientation.m[10] = -facing.z;

                    const int lifetime = gun->lifetimes[i];
                    if (lifetime < 1000) {
                        const float grow = (float) lifetime / 1000.0f;
                        MatrixSetScaling(this->orientation, grow, grow, grow);
                    } else if (gun->hitSmall != 0) {
                        MatrixSetScaling(this->orientation, 1.0f, 1.0f, 1.0f);
                    }
                } else if (this->useEgoOrientation != 0) {
                    // Player-steered rocket: apply the pending pitch/yaw input to the ego rocket matrix.
                    PlayerEgo *player = this->level->getPlayer();
                    Matrix steer;
                    MatrixSetRotation(steer, player->rocketPitch, player->rocketYaw, 0.0f);
                    player->field_0x40 *= steer;
                    this->orientation = player->field_0x40;
                    const Vector &rocket = this->gun->positions.data()[i];
                    MatrixSetTranslation(this->orientation, rocket.x, rocket.y, rocket.z);
                    this->gun->velocities.data()[i] =
                            VectorNormalize(MatrixGetDir(this->orientation)) * this->gun->pitchRate;
                    player->rocketPitch = 0.0f;
                    player->rocketYaw = 0.0f;
                    MatrixSetRotation(steer, 0.0f, 0.0f, this->rollAngle);
                    Globals::Canvas->TransformSetLocal(this->secondaryTransform, steer);
                } else {
                    // Free projectile: build an orthonormal frame around the flight direction.
                    if (this->level != nullptr && this->level->getPlayer() != nullptr && this->gun->isPlayerGun() != 0) {
                        this->up = this->gun->upVectors.data()[i];
                    } else {
                        this->up = Vector(0.0f, 1.0f, 0.0f);
                    }
                    this->side = VectorCross(this->up, this->dir);
                    this->side = VectorNormalize(this->side);
                    this->up = VectorCross(this->dir, this->side);
                    this->up = VectorNormalize(this->up);

                    this->orientation.m[0] = this->side.x;
                    this->orientation.m[4] = this->side.y;
                    this->orientation.m[8] = this->side.z;
                    this->orientation.m[1] = this->up.x;
                    this->orientation.m[5] = this->up.y;
                    this->orientation.m[9] = this->up.z;
                    this->orientation.m[2] = this->dir.x;
                    this->orientation.m[6] = this->dir.y;
                    this->orientation.m[10] = this->dir.z;

                    gun = this->gun;
                    const int lifetime = gun->lifetimes[i];
                    if (lifetime < 1000) {
                        const float grow = (float) lifetime / 1000.0f;
                        MatrixSetScaling(this->orientation, grow, grow, grow);
                    } else if (gun->hitSmall != 0) {
                        MatrixSetScaling(this->orientation, 1.0f, 1.0f, 1.0f);
                        if (this->isRocketGun() == 0)
                            this->gun->hitSmall = 0;
                    }
                }

                if (Globals::iPad)
                    MatrixSetScaling(this->orientation, this->scaleX, this->scaleY, this->scaleZ);

                if (this->gun->weaponType == ITEM_SORT_MINE) {
                    // Mines tumble: rotate by the per-mine spin and keep winding it up.
                    Vector *spin = ((Array<Vector *> *) this->gun->wobbleOffsets)->data()[i];
                    if (spin != nullptr && this->deltaTime > 0) {
                        MatrixSetRotation(this->orientation, spin->x, spin->y, spin->z);
                        const float step = (float) this->deltaTime * 0.0005f;
                        spin->x += spin->x < 0.0f ? -step : step;
                        spin->y += spin->y < 0.0f ? -step : step;
                        spin->z += spin->z < 0.0f ? -step : step;
                    }
                    MatrixSetScaling(this->orientation, this->scaleX, this->scaleY, this->scaleZ);
                }

                Globals::Canvas->TransformSetLocal(this->transform, this->orientation);
                Globals::Canvas->DrawTransform(this->transform, nullptr);
            } else {
                // Parked slot (no live projectile).
                ++inactive;
            }
        }

        gun = this->gun;
        if (inactive >= (int) gun->velocities.count)
            gun->active = 0;
    }

    if (this->wasFiring != 0 && this->geometry != nullptr)
        this->geometry->render();
    this->deltaTime = 0;
}

#pragma GCC visibility push(hidden)
// Bounds-checked projectile mesh lookup for item indices.
uint32_t ObjectGunProjectileMesh(int item) {
    if (item < 0 || item >= 233)
        return 0;
    return (uint32_t) kProjectileMeshes[item];
}
#pragma GCC visibility pop

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

